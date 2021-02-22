unit z_instance_vst;
{$include zenith.inc}

{

  http://www.oifii.org/ns-org/nsd/ar/cp/audio_linuxsampler-cvscheckout/vstsdk2.4/

  In general, processEvents(), startProcess(), stopProcess(), process(),
  processReplacing() and processDoubleReplacing() are called from a time-
  critical high priority thread (except for offline processing).

  To satisfy realtime constraints for low latency audio processing, memory
  allocations and lock-based syncronisation should be avoided.

  The same applies to setParameter() and setProgram() when playing automation
  from host. Any others functions are called from UI thread in general.

  The host takes care that processing and setup functions are isolated from each
  other. setSampleRate(), setBlockSize(), setProcessPrecision(), etc. calls
  occur only in suspended state, i.e. before resume() is called.

}

// {$define Z_MAXMIDISEND:=1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base,
  z_parameter,
  DAEffect,
  DAEffectX;

type

  ZInstanceVst = class(ZInstanceBase)
  private
    FMaster         : TAudioMasterCallbackFunc;
    FAEffect        : AEffect;
    FNeedDefParams  : Boolean;
  protected
    FBlockSize      : LongInt;
    FCurrentProgram : longInt;
    FSampleRate     : Single;
  public
    constructor create(AHost:ZHostBase; {%H-}ADescriptor: ZDescriptorBase);
    destructor  destroy; override;
    function    getEffect: pointer; override;
  public
    function    vst_dispatcher(opcode, index: VstInt32; Value: VstIntPtr; ptr: pointer; opt: single): VstIntPtr;
    procedure   vst_process(inputs, outputs: PPSingle; sampleframes: VstInt32);
    procedure   vst_processdouble({%H-}inputs,{%H-}outputs: PPDouble; {%H-}sampleFrames: VstInt32);
    procedure   vst_setparameter(index: VstInt32; Value: single);
    function    vst_getparameter(index: VstInt32): single;
  end;

  ZInstanceImpl = ZInstanceVst;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_const,
  z_rect,
  z_memory,
  z_util,
  z_string;

//----------------------------------------------------------------------

constructor ZInstanceVst.create(AHost:ZHostBase; ADescriptor: ZDescriptorBase);
begin
  inherited create;
  FMaster := TAudioMasterCallbackFunc(AHost.getMaster);
  ZMemset(@FAEffect,0,sizeof(AEffect));
  FNeedDefParams := True;
  FBlockSize := 0;
  FCurrentProgram := 0;
  FSampleRate := 0;
  // suspend?
end;

//----------

destructor ZInstanceVst.destroy;
begin
  if Assigned(FHost) then FHost.destroy;
  inherited;
end;

//----------------------------------------------------------------------

function ZInstanceVst.getEffect: Pointer;
begin
  result := @FAEffect;
end;

//----------------------------------------------------------------------
// vst (callbacks)
//----------------------------------------------------------------------

{
  assume this can be called from another thread than processReplacing, etc..
}

procedure ZInstanceVst.vst_setparameter(index: VstInt32; Value: single);
begin
  setParameterValue(index, Value);
  updateParameterFromHost(index, Value);
end;

//----------

{
}

function ZInstanceVst.vst_getparameter(index: VstInt32): single;
begin
  Result := FParameterValues[index];
end;


//----------

procedure ZInstanceVst.vst_process(inputs, outputs: PPSingle; sampleframes: VstInt32);
var
  ins, outs: array[0..1] of PSingle;
  len: longword;
begin
  updateParametersInProcess;
  on_processBlock(inputs, outputs, sampleframes);
  if ZHasFlag(FFlags,zpf_perSample) then
  begin // todo: multi-channel
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
  if ZHasFlag(FFlags,zpf_sendMidi) then FHost.midiFlush;
end;

//----------

procedure ZInstanceVst.vst_processdouble(inputs, outputs: PPDouble; sampleFrames: VstInt32);
begin
  // todo
end;

//--------------------------------------------------
//
//--------------------------------------------------

function ZInstanceVst.vst_dispatcher(opcode, index: VstInt32; Value: VstIntPtr;
  ptr: pointer; opt: single): VstIntPtr;
var
  ev: PVstEvents;
  event: PVstMidiEvent;
  pin: PVstPinProperties;
  rec: ERect;
  p: ZParameter;
  //pr : ZProgram;
  r: ZRect;
  num, i: integer;
  v: single;
  pc: PChar;
begin
  //writeln('ZVstInstance.vst_dispatcher');
  Result := 0;
  case opcode of

    //----------

    effOpen: // 00
    begin
      //writeln('effOpen');
      on_stateChange(zps_open);
    end;

    //----------

    effClose: // 01
    begin
      //writeln('effClose');
      on_stateChange(zps_close);
    end;

    //----------

    effSetProgram: // 02
    begin
      //writeln('effSetProgram');
      FCurrentProgram := VstInt32(Value);
    end;

    //----------

    {
      EnergyXt calls before after each effEditIdle, it seems..
      even if we don't have our own editor!
    }

    effGetProgram: // 03
    begin
      //writeln('effGetProgram');
      Result := FCurrentProgram;
    end;

    //----------

    effSetProgramName: // 04
    begin
      //writeln('effSetProgramName');
        {if getPlugin.getNumPrograms > 0 then
        begin
          pr := getPlugin.getProgram(FCurrentProgram);
          pr.setName(ptr);
          result := 1;
        end;}
    end;

    //----------

    effGetProgramName: // 05
    begin
      //writeln('effGetProgramName');
        {if getPlugin.getNumPrograms > 0 then
        begin
          pr := getPlugin.getProgram(FCurrentProgram);
          ZStrcpy(ptr,pr.getName);
        end
        else}
      begin
        ZStrcpy(ptr,'default');
      end;
      Result := 1;
    end;

    //----------

    effGetParamLabel: // 06
    begin
      //writeln('effGetParamLabel');
      p := FParameters[index];
      ZStrcpy(ptr, p.getLabel);
      Result := 1;
    end;

    //----------

    effGetParamDisplay: // 07
    begin
      //writeln('effGetParamDisplay');
      p := FParameters[index];
      v := p.from01(FParameterValues[index]);
      ZStrcpy(ptr, p.getDisplay(v));
      Result := 1;
    end;

    //----------

    effGetParamName: // 08
    begin
      //writeln('effGetParamName');
      p := FParameters[index];
      ZStrcpy(ptr, p.getName);
      Result := 1;
    end;

    //----------

    //effGetVu:  // 09 // deprecated

    //----------

    effSetSampleRate: // 10
    begin
      //writeln('effSetSampleRate');
      FSampleRate := opt;
      on_stateChange(zps_sampleRate);
    end;

    //----------

    effSetBlockSize: // 11
    begin
      //writeln('effSetBlockSize');
      FBlockSize := VstInt32(Value);
      on_stateChange(zps_blockSize);
    end;

    //----------

    effMainsChanged: // 12
    begin
      //writeln('effMainsChanged ',value);
      if Value = 0 then
      begin
        FProcessing := False;
        on_stateChange(zps_suspend);
      end
      else
      begin
        writeln('resume');

        if FNeedDefParams then
        begin
          setDefaultParameters;
          FNeedDefParams := False;
        end;
        FProcessing := True;
        on_stateChange(zps_resume); // before or after

        // this.wantEvents(1); //deprecated as of vst2.4
        //but keep it --> backward compatibility!!!

      end;
    end;

    //----------

    effEditGetRect: // 13
    begin
      //writeln('effEditGetRect');
      if ZHasFlag(FFlags,zpf_hasEditor) then
      begin
        r := FEditor.getRect;
        rec.left := r.x;
        rec.top := r.y;
        rec.right := r.x2; // +1
        rec.bottom := r.y2; // +1
        PPointer(ptr)^ := @rec;
        Result := 1;
      end;
    end;

    //----------

    // ext2/linux:  display pre (32-bit?)

    effEditOpen: // 14
    begin
      //writeln('effEditOpen: ',[ptr]);
      if ZHasFlag(FFlags,zpf_hasEditor) then
      begin
        if not FEditor.isOpen then
        begin
          FEditor.open(ptr);
          FEditor.on_open;
          Result := 1;
        end;
      end;
    end;

    //----------

    effEditClose: // 15
    begin
      //writeln('effEditClose');
      if ZHasFlag(FFlags,zpf_hasEditor) then
      begin
        if FEditor.isOpen then
        begin
        FEditor.on_close;
        FEditor.close;
        Result := 1;
        end;
      end;
    end;

    //----------

    //effEditDraw:  // 16 // deprecated
    //effEditMouse:  // 17 // deprecated
    //effEditKey:  // 18 // deprecated

    //----------

    { EnergyXt calls this even if we don't have our own editor! }

    effEditIdle: // 19
    begin
      //writeln('effEditIdle');
      if ZHasFlag(FFlags,zpf_hasEditor) then
      begin
        if FEditor.isOpen then
        begin
          FEditor.on_idle;
          FEditor.idle;
          Result := 1;
        end;
      end;
    end;

    //----------

    //effEditTop:  // 20 // deprecated
    //effEditSleep:  // 21 // deprecated

    //----------

    {effIdentify:} 22: // deprecated
    begin
      //writeln('effIdentify');
      Result := ZFourChar('NvEf');
    end;

    //----------

    // VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
    // data - should point to the newly allocated memory block containg state data. You can savely release it in next suspend/resume call.
    // isPreset - true when saving a single program, false for all programs
    // note: If your plug-in is configured to use chunks (see AudioEffect::programsAreChunks),
    //  the Host will ask for a block of memory describing the current plug-in state for saving.
    // To restore the state at a later stage, the same data is passed back to AudioEffect::setChunk.
    // Alternatively, when not using chunk, the Host will simply save all parameter values.

    // host stores plug-in state. Returns the size in bytes of the chunk
    // (plug-in allocates the data array)
    //trace("axFormatVst.dispatcher :: effGetChunk");
    //v = getChunk((void** )ptr, index ? true : false);

    effGetChunk: // 23
    begin
      //writeln('effGetChunk');
    end;

    //----------

    //VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
    //data - pointer to state data (owned by Host)
    //byteSize - size of state data
    //isPreset - true when restoring a single program, false for all programs

    // host restores plug-in state
    //trace("axFormatVst.dispatcher :: effSetChunk");
    //v = setChunk(ptr, (VstInt32)value, index ? true : false);

    effSetChunk: // 24
    begin
      //writeln('effSetChunk');
    end;

    //---------- aeffectx.h (vst 2)

    {
      assumptions:
        called just before pricess(), from the same thread,
        non-overlapping (non-concurrent?)
    }

    effProcessEvents: // 25
    begin
      //writeln('effProcessEvents');
      if ZHasFlag(FFlags,zpf_receiveMidi) then
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
      end;
      Result := 1;
    end;

    //----------

    effCanBeAutomated: // 26
    begin
      //writeln('effCanBeAutomated');
      if ZHasFlag( FParameters[index].getFlags,zpf_automate) then result := 1;
    end;

    //----------

    effString2Parameter: // 27
    begin
      //writeln('effString2Parameter');
    end;

    //----------

    //effGetNumProgramCategories: // 28 // deprecated

    //----------

    {
      energy xt 2 calls this before effClose ??? (investigate)
    }

    effGetProgramNameIndexed: // 29
    begin
      //writeln('effGetProgramNameIndexed');
        {if getPlugin.getNumPrograms > 0 then
        begin
          pr := getPlugin.getProgram(index);
          ZStrcpy(ptr,pr.getName);
          result := 1;
        end}

      // temp
      ZStrcpy(ptr, 'default');
      Result := 1;

    end;

    //----------

    //effCopyProgram: // 30 // deprecated
    //effConnectInput: // 31 // deprecated
    //effConnectOutput: // 32 // deprecated

    //----------

    effGetInputProperties: // 33
    begin
      //writeln('effGetInputProperties');
      pin := ptr;
      pc := ZStrcpy(pin^.vLabel, 'input ');
      pc[0] := Z_HEX_TABLE[index and $0F];
      pc[1] := #0;
      pin^.flags := 1; // active
      if ((index and 1) = 0) then
        pin^.flags := pin^.flags or 2; // first of stereo pair
      Result := 1;
    end;

    //----------

    effGetOutputProperties: // 34
    begin
      //writeln('effGetOutputProperties');
      pin := ptr;
      pc := ZStrcpy(pin^.vLabel, 'output ');
      pc[0] := Z_HEX_TABLE[index and $0F];
      pc[1] := #0;
      pin^.flags := 1; // active
      if ((index and 1) = 0) then
        pin^.flags := pin^.flags or 2; // first of stereo pair
      Result := 1;
    end;

    //----------

    effGetPlugCategory: // 35
    begin
      //writeln('effGetPlugCategory');
      if ZHasFlag(FFlags,zpf_isSynth) then result := kPlugCategSynth
      else result := kPlugCategEffect;
    end;

    //----------

    //effGetCurrentPosition: // 36 // deprecated
    //effGetDestinationBuffer: // 37 // deprecated

    //----------

    effOfflineNotify: // 38
    begin
      //writeln('effOfflineNotify');
    end;

    //----------

    effOfflinePrepare: // 39
    begin
      //writeln('effOfflinePrepare');
    end;

    //----------

    effOfflineRun: // 40
    begin
      //writeln('effOfflineRun');
    end;

    //----------

    effProcessVarIo: // 41
    begin
      //writeln('effProcessVarIo');
    end;

    //----------

    effSetSpeakerArrangement: // 42
    begin
      //writeln('effSetSpeakerArrangement');
    end;

    //effSetBlockSizeAndSampleRate: // 43 // deprecated

    //----------

    effSetBypass: // 44
    begin
      //writeln('effSetBypass');
      //if Value = 0 then FProcessing := False
      //else FProcessing := True;
    end;

    //----------

    effGetEffectName: // 45
    begin
      //writeln('effGetEffectName');
      ZStrcpy(ptr, FDescriptor.getName);
      Result := 1;
    end;

    //----------

    //effGetErrorText: // 46 // deprecated

    //----------

    effGetVendorString: // 47
    begin
      //writeln('effGetVendorString');
      ZStrcpy(ptr, FDescriptor.getAuthor);
      Result := 1;
    end;

    //----------

    effGetProductString: // 48
    begin
      //writeln('effGetProductString');
      ZStrcpy(ptr, FDescriptor.getProduct);
      Result := 1;
    end;

    //----------

    effGetVendorVersion: // 49
    begin
      //writeln('effGetVendorVersion');
      Result := FDescriptor.getVersion;
    end;

    //----------

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
      //writeln('effVendorSpecific');
    end;

    //----------

    effCanDo: // 51
    begin
      //writeln('effCanDo');
      if ZHasFlag(FFlags,zpf_sendMidi) then
      begin
        if Pos('sendVstEvents',    PChar(ptr)) <> 0 then result := 1;
        if Pos('sendVstMidiEvent', PChar(ptr)) <> 0 then result := 1;
      end;
      if ZHasFlag(FFlags,zpf_receiveMidi) then
      begin
        if Pos('receiveVstEvents',    PChar(ptr)) <> 0 then result := 1;
        if Pos('receiveVstMidiEvent', PChar(ptr)) <> 0 then result := 1;
      end;
      if Pos('receiveVstTimeInfo', PChar(ptr)) <> 0 then result := 1; // receive Time info from Host
      //if Pos(ptr,'offline') <> 0 then exit(1);              // supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
      //if Pos(ptr,'midiProgramNames') <> 0 then exit(1);     // supports function #getMidiProgramName ()
      //if Pos(ptr,'bypass') <> 0 then exit(1);               // supports function #setBypass ()
      //if Pos(ptr,'hasCockosExtensions') <> 0 then result := $beef0000;
    end;

    //----------

    effGetTailSize: // 52
    begin
      //writeln('effGetTailSize');
    end;

    //----------

    // [energyXt2 calls this]

    53: //effIdle: // 53 // deprecated
    begin
      //writeln('effIdle');
    end;

    //----------

    //effGetIcon: // 54 // deprecated
    //effSetViewPosition: // 55 // deprecated

    //----------

    effGetParameterProperties: // 56
    begin
      //writeln('effGetParameterProperties');
    end;

    //----------

    //effKeysRequired: // 57 // deprecated

    //----------

    effGetVstVersion: // 58
    begin
      //writeln('effGetVstVersion');
    end;

    //---------- VST_2_1_EXTENSIONS

    effEditKeyDown: // 59
    begin
      //writeln('effEditKeyDown');
    end;

    //----------

    effEditKeyUp: // 60
    begin
      //writeln('effEditKeyUp');
    end;

    //----------

    effSetEditKnobMode: // 61
    begin
      //writeln('effSetEditKnobMode');
    end;

    //----------

    effGetMidiProgramName: // 62
    begin
      //writeln('effGetMidiProgramName');
    end;

    //----------

    effGetCurrentMidiProgram: // 63
    begin
      //writeln('effGetCurrentMidiProgram');
    end;

    //----------

    effGetMidiProgramCategory: // 64
    begin
      //writeln('effGetMidiProgramCategory');
    end;

    //----------

    effHasMidiProgramsChanged: // 65
    begin
      //writeln('effHasMidiProgramsChanged');
    end;

    //----------

    effGetMidiKeyName: // 66
    begin
      //writeln('effGetMidiKeyName');
    end;

    // called before all setParameter calls when changing program..

    //----------

    effBeginSetProgram: // 67
    begin
      //writeln('effBeginSetProgram');
    end;

    //----------

    effEndSetProgram: // 68
    begin
      //writeln('effEndSetProgram');
    end;

    //---------- VST_2_3_EXTENSIONS

    effGetSpeakerArrangement: // 69
    begin
      //writeln('effGetSpeakerArrangement');
    end;

    //----------

    effShellGetNextPlugin: // 70
    begin
      //writeln('effShellGetNextPlugin');
    end;

    //----------

    {
      pre vst 2.1 hosts don't call this, do they?
      so we handle it in resume()
    }

    effStartProcess: // 71
    begin
      //writeln('effStartProcess'); // never printed in energy xt2 linux
      //FProcessing := true;
    end;

    //----------

    effStopProcess: // 72
    begin
      //writeln('effStopProcess');// never printed in energy xt2 linux
      //FProcessing := false;
    end;

    //----------

    effSetTotalSampleToProcess: // 73
    begin
      //writeln('effSetTotalSampleToProcess');
    end;

    //----------

    effSetPanLaw: // 74
    begin
      //writeln('effSetPanLaw');
    end;

    //----------

    // ala beginSetProgram
    // called by: ableton live

    effBeginLoadBank: // 75
    begin
      //writeln('effBeginLoadBank');
    end;

    //----------

    effBeginLoadProgram: // 76
    begin
      //writeln('effBeginLoadProgram');
    end;

    //---------- VST_2_4_EXTENSIONS

    effSetProcessPrecision: // 77
    begin
      //writeln('effSetProcessPrecision');
    end;

    //----------

    effGetNumMidiInputChannels: // 78
    begin
      //writeln('effGetNumMidiInputChannels');
    end;

    //----------

    effGetNumMidiOutputChannels: // 79
    begin
      //writeln('effGetNumMidiOutputChannels');
    end;

    //----------

    //else
    //  writeln('unhandled vst dispatcher: ' << opcode << ' : ' << vst_opcodes[opcode]);

  end; // case
end;

//----------------------------------------------------------------------
end.



(*
  {$IFDEF VST_USE_DEPRECATED}
  procedure AudioEffectX.WantEvents(Filter: VstInt32);
  begin
    if Assigned(FAudioMaster) then FAudioMaster(@FEffect, audioMasterWantMidi, 0, filter, nil, 0);
  end;
  {$ENDIF}
*)


