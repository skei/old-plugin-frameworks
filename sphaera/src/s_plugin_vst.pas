{$include sphaera.inc}
unit s_plugin_vst;

{
  todo:
  - SPluginInstance
}

{$define S_MAXMIDISEND   := 1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_debug,
  DAEffect,
  DAEffectX,
  {$ifdef SPH_REAPER} s_reaper_ext, {$endif}
  {$ifdef SPH_GUI} s_editor, {$endif}
  s_parameter,
  s_program,
  s_plugin_base,
  s_types;

type

  //----------

  SVstEvents = record
    numEvents : VstInt32;
    reserved  : VstIntPtr;
    events    : array[0..S_MAXMIDISEND-1] of PVstEvent;
  end;
  PSVstEvents = ^SVstEvents;

  //----------

  SPluginVst = class(SPluginBase)
  private
    FAEffect      : AEffect;
    FMaster       : TAudioMasterCallbackFunc;
    FVstEvents    : SVstEvents;
    FMidiEvents   : array[0..S_MAXMIDISEND-1] of VstMidiEvent; //VstEvent;
    FFirstResume  : Boolean;
    FIsOpen       : Boolean;
    {$ifdef SPH_REAPER} FReaper : SReaperExt; {$endif}
    {$ifdef SPH_GUI} FEditor : SEditor; {$endif}
  private
    FProcessing   : Boolean;
    FSampleRate   : Single;
    FBlockSize    : LongInt;
    FPlayState    : LongInt;
    FPrevState    : LongInt;
    FSamplePos    : Single;
    FTempo        : Single;
    FBeatPos      : Single;
    FProgram      : LongInt;
    FHostCanDo    : LongWord;
    FHostVersion  : LongWord;
    FHostVstVers  : LongWord;
    FHostId       : LongWord;
  public
    property      _processing : Boolean read FProcessing;
    property      _sampleRate : Single read FSampleRate;
    property      _blockSize  : LongInt read FBlockSize;
    property      _playState  : LongInt read FPlayState;
    property      _samplePos  : Single read FSamplePos;
    property      _tempo      : Single read FTempo;
    property      _beatPos    : Single read FBeatPos;
    {$ifdef SPH_GUI} property _editor : SEditor read FEditor; {$endif}
    {$ifdef SPH_REAPER} property _reaper : SReaperExt read FReaper; {$endif}
  public
    constructor create;
    destructor  destroy; override;
  public
    function    initialize({%H-}ptr:Pointer) : Pointer; override;
    function    getTime(var ASampleRate,ASamplePos,ATempo,ABeatPos:Single) : LongInt; override;
    procedure   midiOut(AOffset:integer; AMsg1,AMsg2,AMsg3:byte); override;
    procedure   midiFlush; override;
    procedure   updateSync; override;
    procedure   updateHostInfo; override;
    procedure   updateHostFromEditor(AIndex:LongInt; AValue:Single); override;
  private
    function    vst_getparameter(AIndex:VstInt32): Single;
    procedure   vst_setparameter(AIndex:VstInt32; AValue:Single);
    procedure   vst_process(inputs,outputs:PPSingle; sampleframes:VstInt32);
    procedure   vst_processdouble({%H-}inputs,{%H-}outputs:PPDouble; {%H-}sampleFrames:VstInt32);
    function    vst_dispatcher(opcode,index:VstInt32; Value:VstIntPtr; ptr:pointer; opt:single) : VstIntPtr;
  private
    procedure   host_automate(param:VstInt32; val:single);
    function    host_beginEdit(index:VstInt32) : boolean;
    function    host_canDo(AText:PChar) : boolean;
    function    host_currentId : VstInt32;
    function    host_endEdit(index:VstInt32) : boolean;
    function    host_getProductString(buffer:PChar) : boolean;
    function    host_getSampleRate : single;
    function    host_getTime(filter:VstInt32) : PVstTimeInfo;
    function    host_getVendorString(buffer:PChar) : boolean;
    function    host_getVendorVersion : integer;
    procedure   host_idle;
    function    host_ioChanged : boolean;
    function    host_processEvents(events:PVstEvents) : boolean;
    function    host_sizeWindow(aWidth,aHeight:integer) : boolean;
    function    host_version : VstInt32;
  end;

  //----------

  SPluginImpl = SPluginVst;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_memory,
  //s_rect,
  s_string,
  s_util;

//----------------------------------------------------------------------
// vst callbacks
//----------------------------------------------------------------------

{
  * these just call their vst_ equivalents in SPluginVst class
    (vObject in AEffect struct)
  * dispatcher checks for effClose, and deletes the plugin
}

//----------------------------------------------------------------------

function vst_dispatcher_callback(effect: PAEffect; opcode, index: VstInt32;
  Value: VstIntPtr; ptr: pointer; opt: single): VstIntPtr; cdecl;
var
  plugin: SPluginVst;
begin
  Result := 0;
  plugin := SPluginVst(effect^.vObject);
  if assigned(plugin) then
  begin
    if opCode = effClose then
    begin
      plugin.vst_dispatcher(opcode, index, Value, ptr, opt);
      plugin.on_destroy;
      plugin.Destroy;
      Result := 1; // exit(1);
    end
    else
      Result := plugin.vst_dispatcher(opcode, index, Value, ptr, opt);
  end;
end;

//----------

procedure vst_process_callback(effect: PAEffect; inputs, outputs: PPSingle;
  sampleframes: VstInt32); cdecl;
var
  plugin: SPluginVst;
begin
  plugin := SPluginVst(effect^.vObject);
  if assigned(plugin) then
    plugin.vst_process(inputs, outputs, sampleframes);
end;

//----------

procedure vst_processdouble_callback(effect: PAEffect; inputs, outputs: PPDouble;
  sampleFrames: VstInt32); cdecl;
var
  plugin: SPluginVst;
begin
  plugin := SPluginVst(effect^.vObject);
  if assigned(plugin) then
    plugin.vst_processdouble(inputs, outputs, sampleframes);
end;

//----------

procedure vst_setparameter_callback(effect: PAEffect; index: VstInt32;
  Value: single); cdecl;
var
  plugin: SPluginVst;
begin
  plugin := SPluginVst(effect^.vObject);
  if assigned(plugin) then
    plugin.vst_setparameter(index, Value);
end;

//----------

function vst_getparameter_callback(effect: PAEffect; index: VstInt32): single; cdecl;
var
  plugin: SPluginVst;
begin
  plugin := SPluginVst(effect^.vObject);
  Result := 0;
  if assigned(plugin) then
    Result := plugin.vst_getparameter(index);
end;

//
//----------------------------------------------------------------------
//
// SPluginVst
//
//----------------------------------------------------------------------
//

constructor SPluginVst.create;
var
  i : LongInt;
begin
  inherited;
  SMemset( @FAEffect, 0, sizeof(AEffect));
  FMaster := nil;
  FVstEvents.numEvents := 0;
  FVstEvents.reserved  := 0;
  SMemset(@FMidiEvents,0,sizeof(FMidiEvents));
  for i := 0 to S_MAXMIDISEND-1 do FVstEvents.events[i] := @FMidiEvents[i];
  FFirstResume  := True;
  FIsOpen       := False;
  FProcessing   := False;
  FSampleRate   := 0;
  FBlockSize    := 0;
  FPlayState    := 0;
  FPrevState    := 0;
  FSamplePos    := 0;
  FTempo        := 0;
  FBeatPos      := 0;
  FProgram      := 0;
  FHostCanDo    := shc_none;
  FHostVersion  := 0;
  FHostVstVers  := 0;
  FHostId       := shi_unknown;
  {$ifdef SPH_GUI} FEditor := SEditor.create(self); {$endif}
  {$ifdef SPH_REAPER} FReaper := SReaperExt.create; {$endif}
end;

//----------

destructor SPluginVst.destroy;
begin
  //FHost := nil;
  //inherited;
  {$ifdef SPH_GUI} FEditor.destroy; {$endif}
  {$ifdef SPH_REAPER} FReaper.destroy; {$endif}
  inherited;
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

function SPluginVst.initialize(ptr:Pointer) : Pointer;
var
  flags : longword;
begin
  self.on_create;
  //self.setDefaultParameters; // not now... FSampleRate is not ready.. wait until first resume
  FMaster := TAudioMasterCallbackFunc(ptr);
  SMemset( @FAEffect, 0, sizeof(AEffect));
  flags := effFlagsCanReplacing;
  if SHasFlag(FFlags,spf_hasEditor) then flags += effFlagsHasEditor;
  if SHasFlag(FFlags,spf_isSynth) then flags += effFlagsIsSynth;
  FAEffect.magic                  := SFourChar('VstP');
  FAEffect.uniqueID               := FUniqueId;
  FAEffect.version                := FVersion;
  FAEffect.flags                  := flags;
  FAEffect.numParams              := FParameters.size;
  FAEffect.numPrograms            := FPrograms.size;
  FAEffect.numInputs              := FNumInputs;
  FAEffect.numOutputs             := FNumOutputs;
  FAEffect.initialDelay           := FInitialDelay;
  FAEffect.dispatcher             := @vst_dispatcher_callback;
  FAEffect.setParameter           := @vst_setparameter_callback;
  FAEffect.getParameter           := @vst_getparameter_callback;
  FAEffect.processReplacing       := @vst_process_callback;
  FAEffect.processDoubleReplacing := @vst_processdouble_callback;
  FAEffect.process                := nil; // (deprecated)
  FAEffect.vObject                := self;
  FAEffect.user                   := nil;// self; pae?
  Result := @FAEffect;
  //updateHostInfo;
end;

//----------

function SPluginVst.getTime(var ASampleRate,ASamplePos,ATempo,ABeatPos:Single) : LongInt;
var
  flags : longword;
  ti    : PVstTimeInfo;
begin
  //todo: check hostCanDo (but some hosts return 0)
  if Assigned(FMaster) then
  begin
    flags := kVstPpqPosValid + kVstTempoValid;
    //Hint: Conversion between ordinals and pointers is not portable
    //ti := {%H-}PVstTimeInfo( FMaster(@FAEffect,audioMasterGetTime,0,flags,nil,0) );
    ti := host_getTime(flags);
    if Assigned(ti) then
    begin
      //todo: check flags?
      result      := ti^.flags and $ff;
      ASamplePos  := ti^.samplePos;
      ASampleRate := ti^.sampleRate;
      ABeatPos    := ti^.ppqPos;
      ATempo      := ti^.tempo;
    end;
    //if FHostId = shi_reaper then FReaper.getPlayState ...;
  end;
  {$ifdef SPH_REAPER}
  if SHasFlag(FFlags,spf_reaper) then
  begin
    if (FReaper.getPlayState and 2) <> 0 then Result += spt_pause;
  end;
  {$endif}
end;

//----------

procedure SPluginVst.midiOut(AOffset: integer; AMsg1, AMsg2, AMsg3: byte);
var
  num: longint;
  event: PVstMidiEvent;
begin
  num := FVstEvents.numEvents;
  event := @FMidiEvents[num];
  event^.vType := kVstMidiType;
  event^.deltaFrames := AOffset;
  event^.midiData[0] := AMsg1;
  event^.midiData[1] := AMsg2;
  event^.midiData[2] := AMsg3;
  event^.midiData[3] := 0;
  event^.byteSize    := sizeof(VstMidiEvent) - 2*sizeof(LongInt);
  event^.flags       := 0;  // kVstMidiEventIsRealtime;
  event^.noteLength  := 0;
  event^.noteOffset  := 0;
  event^.detune      := 0;
  inc(num);
  FVstEvents.numEvents := num;
end;

//----------

procedure SPluginVst.midiFlush;
begin
  if FVstEvents.numEvents > 0 then
  begin
    host_processEvents(@FVstEvents);
    FVstEvents.numEvents := 0;
  end;
end;

//----------

{
  kVstTransportChanged     = 1;         // indicates that play, cycle or record state has changed
  kVstTransportPlaying     = 1 shl 1;   // set if Host sequencer is currently playing
  kVstTransportCycleActive = 1 shl 2;   // set if Host sequencer is in cycle mode
  kVstTransportRecording   = 1 shl 3;   // set if Host sequencer is in record mode
  kVstAutomationWriting    = 1 shl 6;   // set if automation write mode active (record parameter changes)
  kVstAutomationReading    = 1 shl 7;   // set if automation read mode active (play parameter changes)
}

{
  todo: check and double-check return values and flags!
}

procedure SPluginVst.updateSync;
begin
  // todo: check if we receive valid answer?
  FPlayState := getTime(FSampleRate{%H-},FSamplePos{%H-},FTempo{%H-},FBeatPos{%H-}) and $ff;
  //if (FPlayState and spt_changed) <> 0
  if FPlayState <> FPrevState then on_transportChange(FPlayState);
  FPrevState := FPlayState;
end;

//----------

{
  ext:        214
  reaper:     4255
  live:       10495
  fk studio:  4311
}

procedure SPluginVst.updateHostInfo;
var
  buffer : array[0..256] of char;
  //ptr : pointer;
  //lw : longword;
  //d : double;
begin
  FHostCanDo := shc_none;
  if host_canDo('sendVstEvents')                  then FHostCanDo += shc_sendVstEvents;
  if host_canDo('sendVstMidiEvent')               then FHostCanDo += shc_sendVstMidiEvent;
  if host_canDo('sendVstTimeInfo')                then FHostCanDo += shc_sendVstTimeInfo;
  if host_canDo('receiveVstEvents')               then FHostCanDo += shc_receiveVstEvents;
  if host_canDo('receiveVstMidiEvent')            then FHostCanDo += shc_receiveVstMidiEvent;
  if host_canDo('reportConnectionChanges')        then FHostCanDo += shc_reportConnectionChanges;
  if host_canDo('acceptIOChanges')                then FHostCanDo += shc_acceptIOChanges;
  if host_canDo('sizeWindow')                     then FHostCanDo += shc_sizeWindow;
  if host_canDo('offline')                        then FHostCanDo += shc_offline;
  if host_canDo('openFileSelector')               then FHostCanDo += shc_openFileSelector;
  if host_canDo('closeFileSelector')              then FHostCanDo += shc_closeFileSelector;
  if host_canDo('startStopProcess')               then FHostCanDo += shc_startStopProcess;
  if host_canDo('shellCategory')                  then FHostCanDo += shc_shellCategory;
  if host_canDo('sendVstMidiEventFlagIsRealtime') then FHostCanDo += shc_sendVstMidiEventFlagIsRealtime;
  FHostId := shi_unknown;
  host_getProductString(buffer);
  if SStrcmp('REAPER',buffer) = 0         then FHostId := shi_reaper;
  if SStrcmp('energyXT',buffer) = 0       then FHostId := shi_energyxt;
  if SStrcmp('Live',buffer) = 0           then FHostId := shi_live;
  if SStrcmp('Fruity Wrapper',buffer) = 0 then FHostId := shi_fruity;
  FHostVersion := host_getVendorVersion;
  FHostVstVers := host_version;
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

procedure SPluginVst.updateHostFromEditor(AIndex:LongInt; AValue:Single);
begin
  if SHasFlag(FFlags,spf_automate) then
  begin
    host_beginEdit(AIndex);
    host_automate(AIndex,AValue);
    host_endEdit(AIndex);
  end;
end;

//----------------------------------------------------------------------
// vst
//----------------------------------------------------------------------

{
  if transport is playing, the events are stuffed into a fifo queue, and the
  entire list is read at the start of processReplacing, and parameter
  changes sent to our plugin..
  if process is not being called, the list will not be updated,

  FHostValues must be updated from gui
}

function SPluginVst.vst_getparameter(AIndex:VstInt32) : Single;
begin
  //Result := getHostValue(AIndex);
  Result := getParamValue(AIndex);
end;

//----------

procedure SPluginVst.vst_setparameter(AIndex:VstInt32; AValue:Single);
begin
  //setHostValue(AIndex,AValue);
  setParamValue(AIndex,AValue); // moved to the next function
  {if FIsOpen then}
    updateParameterFromHost(AIndex,AValue);
end;

//----------

{
  todo:
  - multi-channel..
  - inputs = outputs
}

procedure SPluginVst.vst_process(inputs, outputs: PPSingle; sampleframes: VstInt32);
var
  ins, outs: array[0..1] of PSingle;
  len: longword;
begin
  updateParametersInProcess;
  if SHasFlag(FFlags,spf_autoSync) then updateSync;
  if {Assigned(outputs) and} (sampleframes>0) then
  begin
    if sampleframes > FBlockSize then sampleframes := FBlockSize;
    on_processBlock(inputs, outputs, sampleframes);
    if SHasFlag(FFlags,spf_perSample) then
    begin
      ins[0] := inputs[0];
      ins[1] := inputs[1];
      outs[0] := outputs[0];
      outs[1] := outputs[1];
      len := sampleframes;
      while len > 0 do
      begin
        on_ProcessSample(ins,outs);
        Inc(ins[0]);
        Inc(ins[1]);
        Inc(outs[0]);
        Inc(outs[1]);
        Dec(len);
      end;
    end;
    on_postProcess;
  end; // not 0/nil
  if SHasFlag(FFlags,spf_sendMidi) then midiFlush;
end;

//----------

procedure SPluginVst.vst_processdouble(inputs,outputs: PPDouble; sampleFrames: VstInt32);
{var
  ins, outs: array[0..1] of PDouble;
  len: longword;}
begin
  {updateParametersInProcess;
  if SHasFlag(FFlags,spf_autoSync) then updateSync;
  on_processBlock(inputs, outputs, sampleframes);
  if SHasFlag(FFlags,spf_perSample) then
  begin
    ins[0] := inputs[0];
    ins[1] := inputs[1];
    outs[0] := outputs[0];
    outs[1] := outputs[1];
    len := sampleframes;
    while len > 0 do
    begin
      on_ProcessSample(ins,outs);
      Inc(ins[0]);
      Inc(ins[1]);
      Inc(outs[0]);
      Inc(outs[1]);
      Dec(len);
    end;
  end;
  on_postProcess;
  if SHasFlag(FFlags,spf_sendMidi) then midiFlush;}
end;

//----------------------------------------------------------------------
// vst_dispatcher
//----------------------------------------------------------------------

function SPluginVst.vst_dispatcher(opcode,index: VstInt32; Value: VstIntPtr; ptr: pointer; opt: single): VstIntPtr;
var
  pin   : PVstPinProperties;
  pc    : PChar;
  ev    : PVstEvents;
  num   : LongInt;
  i     : LongInt;
  event : PVstMidiEvent;
  pa    : SParameter;
  pr    : SProgram;
  //r     : SRect;
  rec   : ERect;
  v     : single;
begin
  Result := 0;
  //STrace('dispatcher',opcode);
  case opcode of

    //----------

    {
      initialise this plugin instance
    }

    effOpen: // 00
    begin
      //FDefParams := True;
      FIsOpen := True;
      updateHostInfo;
      {$ifdef S_REAPER} FReaper.initialize(FMaster); {$endif}
      on_stateChange(sps_open);
    end;

    //----------

    {
      exit this plugin instance, release all memory and other resources
    }

    effClose: // 01
    begin
      FIsOpen := False;
      on_stateChange(sps_close);
    end;

    //----------

    {
      host has changed the current program number
      host must call this inside a plugBeginSetProgram ... plugEndSetProgram
      sequence..
      value: program number
    }

    effSetProgram: // 02
    begin
      FProgram := VstInt32(Value);
      on_programChange(FProgram);
    end;

    //----------

    {
      host requests the current program number
      return: current program number
    }

    // called repeatedly (per idle?) by reaper

    // EnergyXt calls this before after each effEditIdle,
    // even if we don't have an editor.
    // reaper calls if editor is open?

    effGetProgram: // 03
    begin
      Result := FProgram;
    end;

    //----------

    {
      host has changed the current program name
      ptr: pointer to new program name (max 24 char + 0)
    }

    effSetProgramName: // 04
    begin
      if FPrograms.size > 0 then
      begin
        pr := FPrograms[FProgram];
        pr.setName(ptr);
        result := 1;
      end;
    end;

    //----------

    {
      host requests the current program name
      ptr: returns string-pointer to program name (max 24 char + 0)
    }

    effGetProgramName: // 05
    begin
      if FPrograms.size > 0 then
      begin
        pr := FPrograms[FProgram];
        SStrcpy(ptr,pr.getName);
      end
      else
      begin
        SStrcpy(ptr,'default');
      end;
      Result := 1;
    end;

    //----------

    {
      host requests parameter label
      index: parameter index
      ptr: returns string-pointer to parameter label (max 8 char + 0),
           examples: sec, dB, type
    }

    effGetParamLabel: // 06
    begin
      pa := FParameters[index];
      SStrcpy(ptr, pa.getLabel);
      //SStrcpy(ptr, '');
      Result := 1;
    end;

    //----------

    {
      host requests parameter textual representation
      index: parameter index
      ptr: returns string-pointer to parameter textual representation
           (max 8 char + 0), examples: examples: 0.5, -3, PLATE
    }

    effGetParamDisplay: // 07
    begin
      pa := FParameters[index];
      //v := getHostValue(index);
      v := getParamValue(index);
      SStrcpy(ptr, pa.getDisplay(v));
      Result := 1;
    end;

    //----------

    {
      host requests parameter name
      index: parameter index
      ptr: returns string-pointer to parameter name (max 31 char + 0)
    }

    effGetParamName: // 08
    begin
      pa := FParameters[index];
      SStrcpy(ptr, pa.getName);
      //SStrcpy(ptr, 'name');
      Result := 1;
    end;

    //----------

    {
      called if (flags and (effFlagsHasClip or effFlagsHasVu))
      return: 'VU' value; > 1.0 means clipped
    }

    //effGetVu:  // 09 // deprecated

    //----------

    {
      host sets SampleRate
      opt: float value in Hz, for example 44100.0Hz
    }

    effSetSampleRate: // 10
    begin
      FSampleRate := opt;
      on_stateChange(sps_sampleRate);
    end;

    //----------

    {
      host sets BlockSize
      value: maximum number of sampleframes an audio block may contain
    }

    effSetBlockSize: // 11
    begin
      FBlockSize := VstInt32(Value);
      on_stateChange(sps_blockSize);
    end;

    //----------

    {
      the host has switched the 'power on' button. This only switches audio
      processing on/off.
      value: 0 = OFF, the plugin must call Resume() which in turn must flush
                 any delay buffers etc. here because otherwise pending data
                 would sound again when the effect is switched on next time.
             1 = ON, the plug must call Suspend()

      --

      This method is called when the effect is turned off by the user.
      ex: The buffer gets flushed here because otherwise pending data would
          sound again when the effect is switched on next time.

      --

      called from non-rt thread..

      I believe resume() is called from a non-real-time thread, so you can
      allocate memory and stuff. startProcessing() is called from a real-time
      thread, so it's supposed to be "fast" in each plug-in. Thus, you can do
      things like taking initial timer measurements, spin up hardware buffers
      if needed, etc. startProcessing() gives you a chance to do the "last half"
      of your process() functionality, if you're interfacing with something that
      needs double-buffering, for example.
      -
      Ah, right, now that I checked it, the documentation says startProcess()/
      stopProcess() are supposed to be called in RT thread, and resume()/
      suspend() are not in the list. Ok fine, that makes sense.
      -
      flush event list(s) on resume?
    }

    effMainsChanged: // 12
    begin
      //STrace('effMainsChanged: ',Value);
      if Value = 0 then
      begin { suspend }
        FProcessing := False;
        //clearEvents;
        on_stateChange(sps_suspend);
      end
      else
      begin { resume }
        //clearEvents;
        if FFirstResume then
        begin
          setDefaultParameters;
          FFirstResume := False;
          //updateHostInfo;
        end
        else
          //checkParametersInResume;
          //resetParameters;
        begin
        end;
        FProcessing := True;
        {$ifndef S_SPH_NORESETQUEUEINRESUME}
        clearEvents;
        {$endif}
        // updateParametersInResume ??
        on_stateChange(sps_resume);
        //this.wantEvents(1); //deprecated as of vst2.4, but keep it --> backward compatibility!!!
      end;
    end;

    //----------

    {
      ptr: pointer to rect*(top, left, bottom, right)
      return: 1 = successful

      called repeatedly by reaper
    }

    effEditGetRect: // 13
    begin
      {$ifdef SPH_GUI}
      if SHasFlag(FFlags,spf_hasEditor) then
      begin
        //r := FEditorRect;
        rec.left    := FEditor.getRect.x;
        rec.top     := FEditor.getRect.y;
        rec.right   := FEditor.getRect.x2; // +1
        rec.bottom  := FEditor.getRect.y2; // +1
        PPointer(ptr)^ := @rec;
        Result := 1;
      end;
      {$endif}
    end;

    //----------

    {
      ptr: contains system dependant Window handle/pointer
      return: 1 = successful
    }

    // ext2/linux:  display pre (32-bit?)

    effEditOpen: // 14
    begin
      {$ifdef SPH_GUI}
      if SHasFlag(FFlags,spf_hasEditor) then
      begin
        if not FEditor.isOpen then
        begin
          FEditor.open(FEditorRect,ptr);
          on_openEditor;
          FEditor.setupWidgetValues;
          FEditor.getWindow.on_align;
          FEditor.getWindow.redraw;
          Result := 1;
        end;
      end;
      {$endif}
    end;

    //----------

    {
      no arguments
    }

    effEditClose: // 15
    begin
      {$ifdef SPH_GUI}
      if SHasFlag(FFlags,spf_hasEditor) then
      begin
        if FEditor.isOpen then
        begin
          FEditorRect := FEditor.getRect;
          on_closeEditor;
          //FEditor.cleanupDefaultWidgets;
          FEditor.close;
          Result := 1;
        end;
      end;
      {$endif}
    end;

    //----------

    //effEditDraw:  // 16 // deprecated
    //effEditMouse:  // 17 // deprecated
    //effEditKey:  // 18 // deprecated

    //----------

    {
      no arguments. Be gentle!
      (it would be better if this call could be avoided)

      called repeatedly (per idle?) by reaper,
      also in 'pause' state (when processing is not being called)

      don't push stuff or changes to editor,
      notify editor that we want to change, and do it in the editor
    }

    { EnergyXt calls this even if we don't have our own editor! }

    effEditIdle: // 19
    begin
      {$ifdef SPH_GUI}
      if SHasFlag(FFlags,spf_hasEditor) then
      begin
        if FEditor.isOpen then
        begin
          FEditor.idle;
          on_idleEditor;
          Result := 1;
        end;
      end;
      {$endif}
    end;

    //----------

    //effEditTop:  // 20 // deprecated
    //effEditSleep:  // 21 // deprecated

    //----------

    {
      return: 'NvEf' as a 32-bit integer
    }

    {effIdentify:} 22: // deprecated
    begin
      Result := SFourChar('NvEf');
    end;

    //----------

    {
      host requests pointer to a Bank/Program chunk, plugin is responsible for
      memory allocation/de-allocation of chunk
      index: 0 = isBank, 1 = isProgram
      ptr: pointer to chunk memory-block
      return: byteSize
    }

    {
      VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
      data - should point to the newly allocated memory block containg state data. You can savely release it in next suspend/resume call.
      isPreset - true when saving a single program, false for all programs
      note: If your plug-in is configured to use chunks (see AudioEffect::programsAreChunks),
       the Host will ask for a block of memory describing the current plug-in state for saving.
      To restore the state at a later stage, the same data is passed back to AudioEffect::setChunk.
      Alternatively, when not using chunk, the Host will simply save all parameter values.
    }

    {
      host stores plug-in state. Returns the size in bytes of the chunk
      (plug-in allocates the data array)
      trace("axFormatVst.dispatcher :: effGetChunk");
      v = getChunk((void** )ptr, index ? true : false);
    }

    {
      SONAR is sending the setChunks on separated cores. (dual-core machine here)
      Or on separated threads.
    }

    effGetChunk: // 23
    begin
    end;

    //----------

    {
      plug-in receives a Bank/Program chunk, host is responsible for memory
      allocation/de-allocation of chunk
      index: 0 = isBank, 1 = isProgram
      value: byteSize
      ptr: pointer to chunk memory-block
    }

    {
      VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
      data - pointer to state data (owned by Host)
      byteSize - size of state data
      isPreset - true when restoring a single program, false for all programs
    }

    {
      host restores plug-in state
      trace("axFormatVst.dispatcher :: effSetChunk");
      v = setChunk(ptr, (VstInt32)value, index ? true : false);
    }

    effSetChunk: // 24
    begin
    end;

    //---------- aeffectx.h (vst 2)

    {
      ptr: pointer to VstEvents*, host usually call ProcessEvents just before
           calling ProcessReplacing
      return: VST2.4: host should ignore return value, <=VST2.3: 0 = 'wants no
              more', else return=1
    }

    {
      assumptions:
        called just before process(), from the same thread,
        non-overlapping (non-concurrent?)
    }

    effProcessEvents: // 25
    begin
      if SHasFlag(FFlags,spf_receiveMidi) then
      begin
        ev := PVstEvents(ptr);
        num := ev^.numEvents;
        if num > 0 then
        begin
          for i := 0 to num - 1 do
          begin
            event := PVstMidiEvent(ev^.events[i]);
            if event^.vType = kVstMidiType then
            begin
              on_midiEvent(event^.deltaFrames, event^.midiData[0], event^.midiData[1], event^.midiData[2]);
            end;
          end;
        end;
        Result := 1;
      end;
    end;

    //----------

    {
      hosts asks if parameter can be automated, this should be called each time
      before attempting to call SetParameter()
      index: parameter index
      return: 1 = parameter can be automated
    }

    effCanBeAutomated: // 26
    begin
      if SHasFlag( FParameters[index].getFlags,spf_automate) then result := 1;
    end;

    //----------

    {
      host must call this two-fold:
      1) host checks the capability of plugin if it can convert a string into a parameter float value
          index: parameter index
          ptr: NULL
          return: 1 = plug CAN convert string2Parameter,
                  0 = plug CANNOT convert string2Parameter
      2) host requests plug to set a parameter from the parameter textual representation into a parameter float value (range 0.0 to 1.0), if this conversion is successful then internally the plugin must call setParameter as part of this call
          index: parameter index
          ptr: string-pointer to textual representation of parameter
          return: 1 = conversion successful,
                  0 = fail
    }

    effString2Parameter: // 27
    begin
    end;

    //----------

    //effGetNumProgramCategories: // 28 // deprecated

    //----------

    {
      host requests program name
      index: program index
      value: deprecated in VST2.4
      ptr: returns string-pointer to program name (max 24 char + 0)
      return: 1 = OK, 0 = fail
    }

    {
      energy xt 2 calls this before effClose ??? (investigate)
    }

    effGetProgramNameIndexed: // 29
    begin
      if FPrograms.size > 0 then
      begin
        pr := FPrograms[index];
        SStrcpy(ptr,pr.getName);
        result := 1;
      end
      else
      begin
        // temp
        SStrcpy(ptr, 'default');
        Result := 1;
      end;
    end;

    //----------

    //effCopyProgram: // 30 // deprecated
    //effConnectInput: // 31 // deprecated
    //effConnectOutput: // 32 // deprecated

    //----------

    {
      host requests plugin's input pin properties
      index: input pin number
      ptr: pointer to VstPinProperties*, memory is already allocated by host
      return: 1 = successful
    }

    effGetInputProperties: // 33
    begin
      pin := ptr;
      pc := SStrcpy(pin^.vLabel, 'input ');
      pc[0] := S_HEX_TABLE[index and $0F];
      pc[1] := #0;
      pin^.flags := 1; // active
      if ((index and 1) = 0) then pin^.flags := pin^.flags or 2; // first of stereo pair
      Result := 1;
    end;

    //----------

    {
      host requests plugin's output pin properties
      index: output pin number
      ptr: pointer to VstPinProperties*, memory is already allocated by host
      return: 1 = successful
    }

    effGetOutputProperties: // 34
    begin
      pin := ptr;
      pc := SStrcpy(pin^.vLabel, 'output ');
      pc[0] := S_HEX_TABLE[index and $0F];
      pc[1] := #0;
      pin^.flags := 1; // active
      if ((index and 1) = 0) then pin^.flags := pin^.flags or 2; // first of stereo pair
      Result := 1;
    end;

    //----------

    {
      host requests plugin category
      return: category number:
         0 = Unknown,
         1 = Effect,
         2 = Synth,
         3 = Analysis,
         4 = Mastering,
         5 = Spacializer(panners),
         6 = RoomFx(delays and reverbs),
         7 = SurroundFx(dedicated surround processors),
         8 = Restoration,
         9 = OfflineProcess,
        10 = Shell(is only a container of plugins),
        11 = Generator
    }

    effGetPlugCategory: // 35
    begin
      if SHasFlag(FFlags,spf_isSynth) then result := kPlugCategSynth
      else result := kPlugCategEffect;
    end;

    //----------

    //effGetCurrentPosition: // 36 // deprecated
    //effGetDestinationBuffer: // 37 // deprecated

    //----------

    {
      index: start flag
      value: count
      ptr: pointer to VstAudioFile array
      return: 1 = successful
    }

    effOfflineNotify: // 38
    begin
    end;

    //----------

    {
      value: count
      ptr: pointer to VstOfflineTask array
      return: 1 = successful
    }

    effOfflinePrepare: // 39
    begin
    end;

    //----------

    {
      value: count
      ptr: pointer to VstOfflineTask array
      return: 1 = successful
    }

    effOfflineRun: // 40
    begin
    end;

    //----------

    {
      ptr: pointer to VariableIO*
      return: 1 = successful
    }

    effProcessVarIo: // 41
    begin
    end;

    //----------

    {
      value: pointer to VstSpeakerArrangement* pluginInput
      ptr: pointer to VstSpeakerArrangement* pluginOutput
      return: 1 = successful
    }

    effSetSpeakerArrangement: // 42
    begin
    end;

    //effSetBlockSizeAndSampleRate: // 43 // deprecated

    //----------

    {
      the host has switched the 'softBypass' state
      value: onOff (0 = off, 1 = On)
      return: 1 = successful
      ---
      returns true if the plug-in supports SoftBypass, which allows the process
      to be called, even if the plug-in was bypassed. This is extremely useful
      if the plug-in should be able to maintain a processing state even when
      turned off, e.g. surround decoders/encoders. Moreover, this feature may be
      automated by the host.
    }

    effSetBypass: // 44
    begin
      if Value = 0 then on_stateChange(sps_bypassOff)
      else on_stateChange(sps_bypass);
      result := 1;
    end;

    //----------

    {
      ptr: returns string-pointer to a string with effect name (max 32 bytes)
      return: 1 = successful
    }

    effGetEffectName: // 45
    begin
      SStrcpy(ptr, FName);
      Result := 1;
    end;

    //----------

    //effGetErrorText: // 46 // deprecated

    //----------

    {
      ptr: returns string-pointer to a string identifying the vendor (max 64 ch)
      return: 1 = successful
    }

    effGetVendorString: // 47
    begin
      SStrcpy(ptr, FAuthor);
      Result := 1;
    end;

    //----------

    {
      ptr: returns string-pointer to a string with product name (max 64 char)
      return: 1 = successful
    }

    effGetProductString: // 48
    begin
      SStrcpy(ptr, FProduct);
      Result := 1;
    end;

    //----------

    {
      return: vendor-specific version
    }

    effGetVendorVersion: // 49
    begin
      Result := FVersion;
    end;

    //----------

    {
      no definition, vendor specific handling
      The MouseWheel handling is a bit of an after thought, and has been
      implemented by Stenberg via the effVendorSpecific call as follows:
      index: 'stCA' / $73744341
      value: 'Whee' / $57686565
      ptr: 1.0 = wheel-up, -1.0 = wheel-down
      return: 1 = MouseWheel used
    }

    //effVendorSpecific:
    //   if (index == effGetParamDisplay && ptr)
    //   {
    //     if (value>=0 && value<NUM_PARAMS)
    //     {
    //       sprintf(ptr,'%f',opt);
    //       return 0xbeef;
    //     }
    //   }

    effVendorSpecific: // 50
    begin

      // reaper: http://www.reaper.fm/sdk/vst/vst_ext.php

      {
        Gets the formatted display of a particular value for a parameter,
        without setting the parameter. REAPER uses this when displaying the
        value of a VST's automated parameter on an envelope tooltip, for
        example.
      }

      if (index = effGetParamDisplay) and Assigned(ptr) then
      begin
        if (value >= 0) and (value < FParameters.size) then
        begin
          result := $beef;
        end;
      end;

    end;

    //----------

    {
      host queries a plugin 'cando' capability
      ptr: string-pointer to cando string
      return: 1='cando', 0='don't know' (default), -1='No'
    }

    effCanDo: // 51
    begin
      if SHasFlag(FFlags,spf_sendMidi) then
      begin
        if Pos('sendVstEvents',PChar(ptr)) <> 0 then result := 1;
        if Pos('sendVstMidiEvent',PChar(ptr)) <> 0 then result := 1;
      end;
      if SHasFlag(FFlags,spf_receiveMidi) then
      begin
        if Pos('receiveVstEvents',PChar(ptr)) <> 0 then result := 1;
        if Pos('receiveVstMidiEvent',PChar(ptr)) <> 0 then result := 1;
      end;
      if Pos('receiveVstTimeInfo',PChar(ptr)) <> 0 then result := 1;
      if Pos('bypass',PChar(ptr)) <> 0 then result := 1; // supports function setBypass
      //if Pos('offline',PChar(ptr)) <> 0 then result := 1;
      //if Pos('midiProgramNames',PChar(ptr)) <> 0 then result := 1;

      {
        http://www.reaper.fm/sdk/vst/vst_ext.php
        A REAPER aware VST plug-in can respond to effCanDo "hasCockosExtensions",
        with 0xbeef0000 (returning this value), which will inform the host that
        it can call certain extended calls. A plug-in that responds to
        "hasCockosExtensions" may choose to implement any subset of the extended
        calls.
      }

      {$ifdef SPH_REAPER}
      if SHasFlag(FFlags,spf_reaper) then
      begin
        // Warning: range check error while evaluating constants
        //and (Pos(ptr,'hasCockosExtensions') <> 0) then result := {%H-}$beef0000;
        if (Pos('hasCockosExtensions', PChar(ptr)) <> 0) then result := {%H-}$beef0000;
      end;
      {$endif}




    end;

    //----------

    {
      returns tail size; 0 is default (return 1 for 'no tail')
    }

    effGetTailSize: // 52
    begin
    end;

    //----------

    {
      host sends this idle call in response to plugin's 'audioMasterneedIdle'
      request
      return: 1 must be returned to keep idle calls being issued by host
    }

    // [energyXt2 calls this]
    // reaper too, if editor is open?

    53: //effIdle: // 53 // deprecated
    begin
    end;

    //----------

    //effGetIcon: // 54 // deprecated
    //effSetViewPosition: // 55 // deprecated

    //----------

    {
      of parameter <index>, VstParameterProperties* in <ptr>
      return=1 when successful
    }

    effGetParameterProperties: // 56
    begin
    end;

    //----------

    //effKeysRequired: // 57 // deprecated

    //----------

    {
      returns 2; older versions return 0
    }

    effGetVstVersion: // 58
    begin
      //result := 2;
    end;

    //---------- VST_2_1_EXTENSIONS

    {
      index: character
      value: virtual key-code
      opt: key-modifiers
      return: 0 = KeyDown not used, 1 = KeyDown used
    }

    effEditKeyDown: // 59
    begin
    end;

    //----------

    {
      index: character
      value: virtual key-code
      opt: key-modifiers
      return: 0 = not KeyUp used, 1 = KeyUp used
    }

    effEditKeyUp: // 60
    begin
    end;

    //----------

    {
      value: knobmode 0=circular, 1=circular relativ, 2=linear
      return: 1 = successful
    }

    effSetEditKnobMode: // 61
    begin
    end;

    //----------

    {
      ptr: passed points to MidiProgramName struct.
      ptr: struct will be filled with information for 'thisProgramIndex'.
      return: number of used programIndexes.
              if 0 is returned, no MidiProgramNames supported.
    }

    effGetMidiProgramName: // 62
    begin
    end;

    //----------

    {
      ptr: passed points to MidiProgramName struct.
      ptr: struct will be filled with information for the current program.
      return: the programIndex of the current program.
    }

    effGetCurrentMidiProgram: // 63
    begin
    end;

    //----------

    {
      ptr: passed points to MidiProgramCategory struct.
      ptr: struct will be filled with information for 'thisCategoryIndex'.
      return: number of used categoryIndexes.
              if 0 is returned, no MidiProgramCategories supported.
    }

    effGetMidiProgramCategory: // 64
    begin
    end;

    //----------

    {
      return: 1 if the MidiProgramNames or MidiKeyNames had changed on this
              channel, 0 otherwise. <ptr> ignored.
    }

    effHasMidiProgramsChanged: // 65
    begin
    end;

    //----------

    {
      ptr: passed points to MidiKeyName struct.
      ptr: struct will be filled with information for 'thisProgramIndex' and
           'thisKeyNumber'.
    }

    effGetMidiKeyName: // 66
    begin
    end;

    // called before all setParameter calls when changing program..

    //----------

    {
      host calls this before a new program (effSetProgram) is loaded
    }

    effBeginSetProgram: // 67
    begin
    end;

    //----------

    {
      host calls this after the new program (effSetProgram) has been loaded
    }

    effEndSetProgram: // 68
    begin
    end;

    //---------- VST_2_3_EXTENSIONS

    {
      value: pointer to VstSpeakerArrangement** pluginInput
      ptr: pointer to VstSpeakerArrangement** pluginOutput
    }

    effGetSpeakerArrangement: // 69
    begin
    end;

    //----------

    {
      This opcode is only called, if plugin is of type kPlugCategShell.
      <ptr> points to a char buffer of size 64, which is to be filled with the
      name of the plugin including the terminating zero.
      return: the next plugin's uniqueID.
    }

    effShellGetNextPlugin: // 70
    begin
    end;

    //----------

    {
      host calls this before the start of process call and after e.g. resume()

      http://ygrabit.steinberg.de/~ygrabit/public_html/vstsdk/OnlineDoc/vstsdk2.3/html/plug/2.0/AudioEffectX.html#startProcess
      Called one time before the start of process call.
      example:
        resume () // the user activates the plugin (User Interface Thread)
        startProcess () // Audio Thread
        process () // or ProcessReplacing ()
        process ()
        process ()
        stopProcess () // something has stopped the engine...or delayed the call
        startProcess ()
        process ()
        process ()
        process ()
        stopProcess () // yet the user has really stopped the plugin (Off)
        suspend ()
      }

    {
      pre vst 2.3 hosts don't call this, do they?
      should we handle it in suspend/resume?

      ext2 2.5 never calls this?
      http://www.kvraudio.com/forum/viewtopic.php?t=277899
    }

    effStartProcess: // 71
    begin
      // never called in energy xt2 linux
      FProcessing := True;
      on_stateChange(sps_start);
    end;

    //----------

    {
      host calls this after the stop of process call and before e.g. suspend()
    }

    effStopProcess: // 72
    begin
      // never called in energy xt2 linux
      FProcessing := False;
      on_stateChange(sps_stop);
    end;

    //----------

    {
      Called in offline (non RealTime) Process before process is called,
      indicates how many sample will be processed
    }

    effSetTotalSampleToProcess: // 73
    begin
    end;

    //----------

    {
      PanLaw : Type (Linear, Equal Power,.. see enum PanLaw Type) in <value>,
               Gain in <opt>: for Linear :
               [1.0 => 0dB PanLaw], [~0.58 => -4.5dB], [0.5 => -6.02dB]
    }

    effSetPanLaw: // 74
    begin
    end;

    //----------

    {
      Called before a Bank is loaded, <ptr> points to VstPatchChunkInfo structure
      return: -1 if the Bank cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
    }

    // ala beginSetProgram
    // called by: ableton live

    effBeginLoadBank: // 75
    begin
    end;

    //----------

    {
      Called before a Program is loaded, <ptr> points to VstPatchChunkInfo structure
      return: -1 if the Program cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
    }

    effBeginLoadProgram: // 76
    begin
    end;

    //---------- VST_2_4_EXTENSIONS

    {
      value: @see VstProcessPrecision
    }

    effSetProcessPrecision: // 77
    begin
    end;

    //----------

    {
      return: number of used MIDI input channels (1-15)
    }

    effGetNumMidiInputChannels: // 78
    begin
    end;

    //----------

    {
      return: number of used MIDI output channels (1-15)
    }

    effGetNumMidiOutputChannels: // 79
    begin
    end;

    //----------

    //else
    //  writeln('unhandled vst dispatcher: ' << opcode << ' : ' << vst_opcodes[opcode]);

  end; // case
end;

//----------------------------------------------------------------------
// host
//----------------------------------------------------------------------

{
  An important thing to notice is that if the user changes a parameter in your
  editor (which is out of the host's control Ðbecause we are not using the
  default string based interface), you should call...
  setParameterAutomated(index,newValue);
  This ensures that the host is notified of the parameter change, which allows
  it to record these changes for automation.

  Note that setParameterAutomated calls the plug's setParameter, and in the
  case of an AEffGuiEditor, the editor's setparameter will also be called after
  setParameterAutomated.
}

procedure SPluginVst.host_automate(param:VstInt32; val:single);
begin
  if assigned(FMaster) then FMaster(@FAEffect,audioMasterAutomate,param,0,nil,val);
end;

//----------

{
  To be called before setParameterAutomated (on Mouse Down). This will be used
  by the Host for specific Automation Recording.
  ---
  called before setParameterAutomated(), to tell the host that it should start
  recording automation data for the parameter specified byindex
}

function SPluginVst.host_beginEdit(index:VstInt32) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterBeginEdit,index,0,nil,0) ) = 1 then result := true;
  end;
end;

//----------

{
  sendVstEvents                   Host supports send of Vst events to plug-in
  sendVstMidiEvent                Host supports send of MIDI events to plug-in
  sendVstTimeInfo                 Host supports send of VstTimeInfo to plug-in
  receiveVstEvents                Host can receive Vst events from plug-in
  receiveVstMidiEvent             Host can receive MIDI events from plug-in
  reportConnectionChanges         Host will indicates the plug-in when something change in plug-inÅ½s routing/connections with suspend/resume/setSpeakerArrangement
  acceptIOChanges                 Host supports ioChanged
  sizeWindow                      used by VSTGUI
  offline                         Host supports offline feature
  openFileSelector                Host supports function openFileSelector
  closeFileSelector               Host supports function closeFileSelector
  startStopProcess                Host supports functions startProcess and stopProcess
  shellCategory                   'shell' handling via uniqueID. If supported by the Host and the Plug-in has the category kPlugCategShell
  sendVstMidiEventFlagIsRealtime  Host supports flags for VstMidiEvent
}

function SPluginVst.host_canDo(AText:PChar) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterCanDo,0,0,AText,0) ) = 1 then result := true;
  end;
end;

//----------

function SPluginVst.host_currentId : VstInt32;
begin
  result := 0;
  if assigned(FMaster) then result := VstInt32( FMaster(@FAEffect,audioMasterCurrentId,0,0,nil,0) );
end;

//----------

{
  To be called after setParameterAutomated (on Mouse Up).
  ---
  called after setParameterAutomated(), and tells the host that this parameter
  is no longer subject to changes by the user.
}

function SPluginVst.host_endEdit(index:VstInt32) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterEndEdit,index,0,nil,0) ) = 1 then result := true;
  end;
end;

//----------

function SPluginVst.host_getProductString(buffer:PChar) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterGetProductString,0,0,buffer,0) ) = 1 then result := true;
  end;
end;

//----------

function SPluginVst.host_getSampleRate : single;
begin
  result := 0;
  if assigned(FMaster) then
  begin
    result := VstIntPtr( FMaster(@FAEffect,audioMasterGetSampleRate,0,0,nil,0) );
  end
end;

//----------

function SPluginVst.host_getTime(filter:VstInt32) : PVstTimeInfo;
begin
  result := nil;
  if Assigned(FMaster) then
    // Hint: Conversion between ordinals and pointers is not portable
    result := {%H-}PVstTimeInfo( FMaster(@FAEffect,audioMasterGetTime,0,filter,nil,0) );
end;

//----------

function SPluginVst.host_getVendorString(buffer:PChar) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterGetVendorString,0,0,buffer,0) ) = 1 then result := true;
  end;
end;

//----------

function SPluginVst.host_GetVendorVersion : integer;
begin
  result := 0;
  if assigned(FMaster) then result := VstInt32( FMaster(@FAEffect,audioMasterGetVendorVersion,0,0,nil,0) );
end;

//----------

procedure SPluginVst.host_idle;
begin
  if assigned(FMaster) then FMaster(@FAEffect,audioMasterIdle,0,0,nil,0);
end;

//----------

function SPluginVst.host_IOChanged : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterIOChanged,0,0,nil,0) ) <> 0 then result := true;
  end;
end;

//----------

function SPluginVst.host_processEvents(events:PVstEvents) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterProcessEvents,0,0,events,0) ) = 1 then result := true;
  end;
end;

//----------

function SPluginVst.host_sizeWindow(aWidth,aHeight:integer) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(@FAEffect,audioMasterSizeWindow,aWidth,aHeight,nil,0) ) = 1 then result := true;
  end;
end;

//----------

function SPluginVst.host_version : VstInt32;
begin
  result := 1;
  if assigned(FMaster) then result := VstInt32( FMaster(@FAEffect,audioMasterVersion,0,0,nil,0) );
  if result = 0 then result := 1; // old (!version)
end;



//----------------------------------------------------------------------
end.

{
  * http://www.oifii.org/ns-org/nsd/ar/cp/audio_linuxsampler-cvscheckout/vstsdk2.4/doc/html/main.html
  * we can call setParameter ourselves, use either negative indexes, or some user-defined range
}

