{$include sphaera.inc}
unit s_reaper_ext;

// http://www.reaper.fm/sdk/vst/vst_ext.php

{
  Implementation Notes

  REAPER currently implements a subset of the VST 2.4 standard as well as
  additional extensions-- this section discusses the former.

  Audio/sample processing: REAPER will use processDoubleReplacing if
  effFlagsCanDoubleReplacing is set in the flags.
  REAPER will use processReplacing if effFlagsCanReplacing is set and
  processReplacing != process (if it equals it assumes that it is a faulty
  implementation of processReplacing). Additionally there is an option in the
  preferences to not use processReplacing for buggy plug-ins. Note that for
  plug-ins that have Cockos extensions (see below), the option (to not use
  processReplacing) is ignored, allowing known-good plug-ins to always run at
  full speed.

  Threading/concurrency: Since audio processing can run in other threads from
  the UI, it's important to note what can run at the same time. REAPER is highly
  optimized to prevent UI and other actions from interrupting audio, so assume
  everything can (and WILL) run at the same time as your process/
  processReplacing, except:
    effOpen/effClose
    effSetChunk - while effGetChunk can run at the same time as audio (user
                  saves project, or for automatic undo state tracking),
                  effSetChunk is guaranteed to not run while audio is processing
  So nearly everything else should be threadsafe. For many things this doesn't
  mean having to do much, but there are plenty of plug-ins that barf when audio
  is running and you open the window (using effEditOpen). There is an option to
  bypass audio while opening the config window, but it sucks and shouldn't be
  required.

  Dynamic parameters: REAPER can deal with the number of parameters for a
  plug-in changing on the fly. Use the extended host audioMasterAutomate
  callback described below to notify the host of parameter count changes (i.e.
  parameter X deleted, or new parameter Y), to preserve automation of higher
  indexed parameters when adding/removing lower parameters.

  Multiple inputs/outputs: REAPER allows the user to dynamically connect input/
  output pins of VSTs wherever they want, so enable as much I/O as you need.
  REAPER also allows input/output counts to change, HOWEVER it is recommended
  that any changes you make be done from within processReplacing() or process(),
  and use the old value of numInputs/numOutputs until the next call.
  Additionally the initial value of numInputs/numOutputs should be set to the
  most common settings.

  Longer labels: effGetParamName/effGetParamLabel/effGetParamDisplay all support
  up to 256 character strings (255+null).

}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  DAEffect;
  //DAEffectX,

type

  reaper_getPlayPosition    = function : double; // double (*GetPlayPosition)();
  reaper_getPlayPosition2   = function : double; // double (*GetPlayPosition2)();
  reaper_getCursorPosition  = function : double; // double (*GetCursorPosition)();
  reaper_getPlayState       = function : longint; // int (*GetPlayState)();
  reaper_setEditCurPos      = procedure(time:double; moveview,seekplay:boolean); // void (*SetEditCurPos)(double time, bool moveview, bool seekplay);
  reaper_getsetRepeat       = function(parm:longint) : longint; // int (*GetSetRepeat)(int parm);
  reaper_getProjectPath     = procedure(but:pchar; bufsiz:longint); // void (*GetProjectPath)(char *buf, int bufsz);
  reaper_onPlayButton       = procedure; // void (*OnPlayButton)();
  reaper_onStopButton       = procedure; // void (*OnStopButton)();
  reaper_onPauseButton      = procedure; // void (*OnPauseButton)();
  reaper_isInRealTimeAudio  = function : longint; // int (*IsInRealTimeAudio)();
  reaper_audioIsRunning     = function : longint; // int (*Audio_IsRunning)();
  //reaper_

  //----------

  SReaperExt = class
  private
    FReaperGetPlayPosition   : reaper_getPlayPosition;
    FReaperGetPlayPosition2  : reaper_getPlayPosition2;
    FReaperGetCursorPosition : reaper_getCursorPosition;
    FReaperGetPlayState      : reaper_getPlayState;
    FReaperSetEditCurPos     : reaper_setEditCurPos;
    FReaperGetSetRepeat      : reaper_getsetRepeat;
    FReaperGetProjectPath    : reaper_getProjectPath;
    FReaperOnPlayButton      : reaper_onPlayButton;
    FReaperOnStopButton      : reaper_onStopButton;
    FReaperOnPauseButton     : reaper_onPauseButton;
    FReaperIsInRealTimeAudio : reaper_isInRealTimeAudio;
    FReaperAudioIsRunning    : reaper_audioIsRunning;
    { }
    FMaster:TAudioMasterCallbackFunc;
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize(AMaster:TAudioMasterCallbackFunc); // call in/after effOpen;
    //
    function  getPlayPosition : double;
    function  getPlayPosition2 : double;
    function  getCursorPosition : double;
    function  getPlayState : longint;
    procedure setEditCurPos(time:double; moveview,seekplay:boolean);
    function  getsetRepeat(parm:longint) : longint;
    procedure getProjectPath(buf:pchar; bufsiz:longint);
    procedure onPlayButton;
    procedure onStopButton;
    procedure onPauseButton;
    function  isInRealTimeAudio : longint;
    function  audioIsRunning : longint;

  end;


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SReaperExt.create;
begin
  inherited;
  FReaperGetPlayPosition    := nil;
  FReaperGetPlayState       := nil;
  FReaperGetPlayPosition    := nil;
  FReaperGetPlayPosition2   := nil;
  FReaperGetCursorPosition  := nil;
  FReaperGetPlayState       := nil;
  FReaperSetEditCurPos      := nil;
  FReaperGetSetRepeat       := nil;
  FReaperGetProjectPath     := nil;
  FReaperOnPlayButton       := nil;
  FReaperOnStopButton       := nil;
  FReaperOnPauseButton      := nil;
  FReaperIsInRealTimeAudio  := nil;
  FReaperAudioIsRunning     := nil;
  // etc
end;

//----------

destructor SReaperExt.destroy;
begin
  inherited;
end;

//----------

// call after effOpen..

procedure SReaperExt.initialize(AMaster:TAudioMasterCallbackFunc);
begin
  FMaster := AMaster;
  pointer(FReaperGetPlayPosition)    := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('GetPlayPosition'),0) );
  pointer(FReaperGetPlayPosition2)   := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('GetPlayPosition2'),0) );
  pointer(FReaperGetCursorPosition)  := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('GetCursorPosition'),0) );
  pointer(FReaperGetPlayState)       := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('GetPlayState'),0) );
  pointer(FReaperSetEditCurPos)      := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('SetEditCurPos'),0) );
  pointer(FReaperGetSetRepeat)       := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('GetSetRepeat'),0) );
  pointer(FReaperGetProjectPath)     := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('GetProjectPath'),0) );
  pointer(FReaperOnPlayButton)       := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('OnPlayButton'),0) );
  pointer(FReaperOnStopButton)       := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('OnStopButton'),0) );
  pointer(FReaperOnPauseButton)      := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('OnPauseButton'),0) );
  pointer(FReaperIsInRealTimeAudio)  := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('IsInRealTimeAudio'),0) );
  pointer(FReaperAudioIsRunning)     := pointer( FMaster(nil,$deadbeef{%H-},$deadf00d{%H-},0,pchar('Audio_IsRunning'),0) );
end;

//----------

{
  GetPlayPosition() returns the current playback position of the project. This
  is the time the user is hearing (and the transport shows etc). The value is in
  seconds.
}

function SReaperExt.getPlayPosition : double;
begin
  if Assigned(FReaperGetPlayPosition) then
  result := FReaperGetPlayPosition();
end;

//----------

{
  GetPlayPosition() returns the current playback decode position of the project.
  This is the time of the audio block that is being processed by the host. The
  value is in seconds. This may be behind where your plug-in is processing, due
  to anticipative processing and/or PDC.
}

function SReaperExt.getPlayPosition2 : double;
begin
  if Assigned(FReaperGetPlayPosition2) then
  result := FReaperGetPlayPosition2();
end;

//----------

{
  GetCursorPosition() returns the current edit cursor position (if any), in
  seconds.
}

function SReaperExt.getCursorPosition : double;
begin
  if Assigned(FReaperGetCursorPosition) then
  result := FReaperGetCursorPosition();
end;

//----------

{
  GetPlayState() returns an integer value representing the project play state.
    1 = play
    2 = paused
    5 = recording
    6 = record paused
}

function SReaperExt.getPlayState : longint;
begin
  if Assigned(FReaperGetPlayState) then
  result := FReaperGetPlayState();
end;

//----------

{
  SetEditCurPos() repositions the edit cursor to "time" (in seconds),
  optionally moving the view if necessary, and optionally seeking playback (if
  playing back). This function should ONLY be called from a UI context (i.e.
  from the editor window, NOT from audio processing etc).
}

procedure SReaperExt.setEditCurPos(time:double; moveview,seekplay:boolean);
begin
  if Assigned(FReaperSetEditCurPos) then
  FReaperSetEditCurPos(time,moveview,seekplay);
end;

//----------

{
  GetSetRepeat() is used to change or query the transport "loop/repeat" state.
  Pass a parameter of -1 to query the repeat state, 0 to clear, 1 to set, and >1
  to toggle. The return value is the new value of the repeat state. ONLY use
  this function to change the repeat state from the UI thread (however you can
  query safely at any time).
}

function SReaperExt.getsetRepeat(parm:longint) : longint;
begin
  if Assigned(FReaperGetSetRepeat) then
  result := FReaperGetSetRepeat(parm);
end;

//----------

{
  GetProjectPath() can be used to query the path that media files are stored in
  for the current project.
}

procedure SReaperExt.getProjectPath(buf:pchar; bufsiz:longint);
begin
  if Assigned(FReaperGetProjectPath) then
  FReaperGetProjectPath(buf,bufsiz);
end;

//----------

{
  This function control the main transport for the host app. Only call it from
  the UI thread.
}

procedure SReaperExt.onPlayButton;
begin
  if Assigned(FReaperOnPlayButton) then
  FReaperOnPlayButton();
end;

//----------

{
  This function control the main transport for the host app. Only call it from
  the UI thread.
}

procedure SReaperExt.onStopButton;
begin
  if Assigned(FReaperOnStopButton) then
  FReaperOnStopButton();
end;

//----------

{
  This function control the main transport for the host app. Only call it from
  the UI thread.
}

procedure SReaperExt.onPauseButton;
begin
  if Assigned(FReaperOnPauseButton) then
  FReaperOnPauseButton();
end;

//----------

{
  Returns nonzero if in the main audio thread, or in a thread doing synchronous
  multiprocessing. In these instances low latency is key. If this is 0, and you
  are in processReplacing, then you are being called in an anticipative
  processing thread.
}

function SReaperExt.isInRealTimeAudio : longint;
begin
  if Assigned(FReaperIsInRealTimeAudio) then
  result := FReaperIsInRealTimeAudio();
end;

//----------

{
  Returns nonzero if the audio device is open and running.
}

function SReaperExt.audioIsRunning : longint;
begin
  if Assigned(FReaperAudioIsRunning) then
  result := FReaperAudioIsRunning();
end;

//----------------------------------------------------------------------
end.

