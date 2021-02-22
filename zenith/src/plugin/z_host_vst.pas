unit z_host_vst;
{$include zenith.inc}

{
  * enum hosts (energy xt, reaper, etc...
  * prepare eventual special treatment for hosts
    (things like window resizing, etc)
  * let hosts know we support eventual unique features
    - reaper
}

{$define Z_MAXMIDISEND:=1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  DAEffect,
  DAEffectX,
  z_plugin_base;

type

  ZVstEvents = record
    numEvents : VstInt32;
    reserved  : VstIntPtr;
    events    : array[0..Z_MAXMIDISEND-1] of PVstEvent;
  end;
  PZVstEvents = ^ZVstEvents;

  //----------

  ZHostVst = class(ZHostBase)
  private
    FMaster     : TAudioMasterCallbackFunc;
    FEffect     : PAEffect;
    FVstEvents  : ZVstEvents;
    FOutEvents  : array[0..Z_MAXMIDISEND-1] of VstMidiEvent; //VstEvent;
  public
    constructor create(AMaster:Pointer);
    destructor  destroy; override;
  public
    function    getEffect: Pointer; override;
    function    getMaster: Pointer; override;
    function    getTime(var ASampleRate,ASamplePos,ATempo,ABeatPos:Single) : LongInt; override;
    procedure   midiOut(AOffset:integer; AMsg1,AMsg2,AMsg3:byte); override;
    procedure   midiFlush; override;
    procedure   setEffect(ptr:Pointer); override;
    procedure   setMaster(ptr:Pointer); override;
    procedure   updateParameter(param:VstInt32; val:single); override;
  public
    procedure   vst_automate(param:VstInt32; val:single); virtual;
    function    vst_beginEdit(index:VstInt32) : boolean; virtual;
    function    vst_canDo(AText:PChar) : boolean; virtual;
    function    vst_currentId : VstInt32; virtual;
    function    vst_endEdit(index:VstInt32) : boolean; virtual;
    function    vst_getProductString(buffer:PChar) : boolean; virtual;
    function    vst_getSampleRate : single; virtual;
    function    vst_getTime(filter:VstInt32) : PVstTimeInfo; virtual;
    function    vst_getVendorString(buffer:PChar) : boolean; virtual;
    function    vst_getVendorVersion : integer; virtual;
    procedure   vst_idle; virtual;
    function    vst_ioChanged : boolean; virtual;
    function    vst_processEvents(events:PVstEvents) : boolean; virtual;
    function    vst_sizeWindow(aWidth,aHeight:integer) : boolean; virtual;
    function    vst_version : VstInt32; virtual;
  end;

  ZHostImpl = ZHostVst;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_memory;

//----------

constructor ZHostVst.create(AMaster:Pointer);
var i : longint;
begin
  inherited create;
  FMaster := TAudioMasterCallbackFunc(AMaster);
  FEffect := nil;
  FVstEvents.numEvents := 0;
  FVstEvents.reserved  := 0;
  ZMemset(@FOutEvents,0,sizeof(FOutEvents));
  for i := 0 to Z_MAXMIDISEND-1 do FVstEvents.events[i] := @FOutEvents[i];
end;

//----------

destructor ZHostVst.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------

function ZHostVst.getEffect : Pointer;
begin
  result := FEffect;
end;

//----------

function ZHostVst.getMaster : Pointer;
begin
  result := FMaster;
end;

//----------

function ZHostVst.getTime(var ASampleRate,ASamplePos,ATempo,ABeatPos:Single) : LongInt;
var
  flags : longword;
  ti    : PVstTimeInfo;
begin
  if Assigned(FMaster) then
  begin
    flags := kVstPpqPosValid + kVstTempoValid;
    // Hint: Conversion between ordinals and pointers is not portable
    ti := {%H-}PVstTimeInfo( FMaster(FEffect,audioMasterGetTime,0,flags,nil,0) );
    result      := ti^.flags and $ff;
    ASamplePos  := ti^.samplePos;
    ASampleRate := ti^.sampleRate;
    ABeatPos    := ti^.ppqPos;
    ATempo      := ti^.tempo;
  end;
end;

//----------

procedure ZHostVst.midiOut(AOffset: integer; AMsg1, AMsg2, AMsg3: byte);
var
  num: longint;
  event: PVstMidiEvent;
begin
  //writeln('sendMidi');
  num := FVstEvents.numEvents;
  event := @FOutEvents[num];
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

procedure ZHostVst.midiFlush;
begin
  if FVstEvents.numEvents > 0 then
  begin
    // if assigned(FMaster) then FMaster(FEffect,audioMasterProcessEvents,0,0,@FVstEvents,0);
    vst_processEvents(@FVstEvents);
    FVstEvents.numEvents := 0;
  end;
end;

//----------

procedure ZHostVst.setEffect(ptr:Pointer);
begin
  FEffect := ptr;
end;

//----------

procedure ZHostVst.setMaster(ptr:Pointer);
begin
  FMaster := TAudioMasterCallbackFunc(ptr);
end;

//----------

procedure ZHostVst.updateParameter(param:longint; val:single);
begin
  //if Assigned(FMaster) then FMaster(FEffect,audioMasterAutomate,param,0,nil,val);
  vst_automate(param,val);
end;

//----------------------------------------------------------------------
//
// vst_
//
//----------------------------------------------------------------------

function ZHostVst.vst_Version : VstInt32;
begin
  result := 1;
  if assigned(FMaster) then result := VstInt32( FMaster(FEffect,audioMasterVersion,0,0,nil,0) );
  if result = 0 then result := 1; // old (!version)
end;

//----------

function ZHostVst.vst_CurrentId : VstInt32;
begin
  result := 0;
  if assigned(FMaster) then result := VstInt32( FMaster(FEffect,audioMasterCurrentId,0,0,nil,0) );
end;

//----------

procedure ZHostVst.vst_Automate(param:VstInt32; val:single);
begin
  if assigned(FMaster) then FMaster(FEffect,audioMasterAutomate,param,0,nil,val);
end;

//----------

procedure ZHostVst.vst_Idle;
begin
  if assigned(FMaster) then FMaster(FEffect,audioMasterIdle,0,0,nil,0);
end;

//----------

function ZHostVst.vst_ProcessEvents(events:PVstEvents) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterProcessEvents,0,0,events,0) ) = 1 then result := true;
  end;
end;

//----------

function ZHostVst.vst_IOChanged : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterIOChanged,0,0,nil,0) ) <> 0 then result := true;
  end;
end;

//----------

function ZHostVst.vst_SizeWindow(aWidth,aHeight:integer) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterSizeWindow,aWidth,aHeight,nil,0) ) = 1 then result := true;
  end;
end;

//----------

function ZHostVst.vst_GetSampleRate : single;
begin
  result := 0;
  if assigned(FMaster) then
  begin
    result := VstIntPtr( FMaster(FEffect,audioMasterGetSampleRate,0,0,nil,0) );
  end
end;

//----------

function ZHostVst.vst_getTime(filter:VstInt32) : PVstTimeInfo;
begin
  result := nil;
  if Assigned(FMaster) then
    // Hint: Conversion between ordinals and pointers is not portable
    result := {%H-}PVstTimeInfo( FMaster(FEffect,audioMasterGetTime,0,filter,nil,0) );
end;

//----------

function ZHostVst.vst_GetVendorString(buffer:PChar) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterGetVendorString,0,0,buffer,0) ) = 1 then result := true;
  end;
end;

//----------

function ZHostVst.vst_GetProductString(buffer:PChar) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterGetProductString,0,0,buffer,0) ) = 1 then result := true;
  end;
end;

//----------

function ZHostVst.vst_GetVendorVersion : integer;
begin
  result := 0;
  if assigned(FMaster) then result := VstInt32( FMaster(FEffect,audioMasterGetVendorVersion,0,0,nil,0) );
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

function ZHostVst.vst_CanDo(AText:PChar) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterCanDo,0,0,AText,0) ) = 1 then result := true;
  end;
end;

//----------

function ZHostVst.vst_BeginEdit(index:VstInt32) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterBeginEdit,index,0,nil,0) ) = 1 then result := true;
  end;
end;

//----------

function ZHostVst.vst_EndEdit(index:VstInt32) : boolean;
begin
  result := false;
  if assigned(FMaster) then
  begin
    if VstInt32( FMaster(FEffect,audioMasterEndEdit,index,0,nil,0) ) = 1 then result := true;
  end;
end;

//----------------------------------------------------------------------
end.


