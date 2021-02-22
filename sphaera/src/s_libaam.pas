{$include sphaera.inc}
unit s_libaam;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  dynlibs,
  s_library,
  s_types;

type

  TLibaamDispatcher = function(aIndex,aValue1,aValue2:LongInt) : LongInt; cdecl;
	TLibaamProcess    = function(aInputs,aOutputs:PPsingle; aNumIns,aNumOuts:LongInt; aFrames:LongInt; aMidiData:pointer; aMidiDataCount:LongInt) : LongInt; cdecl;
  PLibaamProcess    = ^TLibaamProcess;

  //----------

  SLibaam = class(SLibrary)
  private
    mDispatcher	: TLibaamDispatcher;
    mLoaded     : Boolean;
    mInited     : Boolean;
    mOpened     : Boolean;
  public
    constructor Create;
    destructor  Destroy; override;
    function    LoadLib(aFilename:PChar) : LongInt; override;
    procedure   CloseLib; override;
    //
		function		StreamInit(proc:TLibaamProcess) : LongInt;
		function		StreamOpen(num:LongInt) : LongInt;
		function		StreamClose : LongInt;
		function		StreamExit : LongInt;
		function		StreamAudioCount : LongInt;
		function		StreamAudioName(num:LongInt; str:pchar) : LongInt;
		function		StreamFrames(frames:LongInt=0) : LongInt;
		function		StreamRate(setRate:LongInt; getRate:psingle) : LongInt;
    //
		function		MidiInCount : LongInt;
		function		MidiInName(num:LongInt; str:pchar) : LongInt;
		function		MidiInEnable(num:LongInt; enabled:LongInt) : LongInt;
		function		MidiInEnabled(num:LongInt) : LongInt;
    //
		function		AudioInCount : LongInt;
		function		AudioInName(num:LongInt; str:pchar) : LongInt;
		function		AudioInEnable(num:LongInt; enabled:LongInt) : LongInt;
		function		AudioInEnabled(num:LongInt) : LongInt;
    //
		function		AudioOutCount : LongInt;
		function		AudioOutName(num:LongInt; str:pchar) : LongInt;
		function		AudioOutEnable(num:LongInt; enable:LongInt) : LongInt;
		function		AudioOutEnabled(num:LongInt) : LongInt;
    //

  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

const
	STREAM_INIT					= 0;
	STREAM_OPEN					= 1;
	STREAM_CLOSE				= 2;
//STREAM_PLAY					= 3;
//STREAM_STOP					= 4;
	STREAM_EXIT					= 5;
	STREAM_ABOUT				= 6;
	STREAM_AUDIO_COUNT	= 7;
	STREAM_AUDIO_NAME		= 8;
	STREAM_FRAMES				= 9;
	STREAM_RATE					= 10;

	STREAM_LATENCY			= 17;
	STREAM_RELOAD				= 18;

	MIDI_IN_COUNT				= 30;
	MIDI_IN_NAME				= 31;
	MIDI_IN_ENABLE			= 32;
	MIDI_IN_ENABLED			= 33;

	AUDIO_IN_COUNT			= 40;
	AUDIO_IN_NAME				= 41;
	AUDIO_IN_ENABLE			= 42;
	AUDIO_IN_ENABLED		= 43;

	AUDIO_OUT_COUNT			= 50;
	AUDIO_OUT_NAME			= 51;
	AUDIO_OUT_ENABLE		= 52;
	AUDIO_OUT_ENABLED		= 53;

//----------------------------------------------------------------------

//----------------------------------------------------------------------

constructor SLibaam.Create;
begin
  inherited;
  mDispatcher	:= nil;
  mLoaded := false;
  mInited := false;
  mOpened := false;
end;

//----------

destructor SLibaam.Destroy;
begin
  if mOpened then StreamClose;
  if mInited then StreamExit;
  if mLoaded then CloseLib;
  inherited;
end;

//----------

function SLibaam.LoadLib(aFilename:pchar) : LongInt;
begin
  inherited;
  pointer(mDispatcher) := GetProcedureAddress(mHandle,'libaam');
  if not Assigned(mDispatcher) then exit(-3);
  mLoaded := true;
  result := 0;
end;

//----------

procedure SLibaam.CloseLib;
begin
  inherited;
  mDispatcher := nil;
  mLoaded := false;
end;

//----------

//----------------------------------------------------------------------
//int libaam (int index, int value1, int value2) {
//  int i, result = -1;
//  switch (index) {
//----------------------------------------------------------------------

// init library

function SLibaam.StreamInit(proc:TLibaamProcess) : LongInt;
begin
	//case STREAM_INIT:
	//  process = (CProcess*)value1;
  if Assigned(mDispatcher) then
  begin
    result := mDispatcher(STREAM_INIT,LongInt(proc),0);
    mInited := true;
  end
  else result := -1;
end;

//----------

// open device

function SLibaam.StreamOpen(num:LongInt) : LongInt;
begin
	//case STREAM_OPEN:
	//  result = 1;
  if Assigned(mDispatcher) then
  begin
    result := mDispatcher(STREAM_OPEN,num,0);
    mOpened := true;
  end
  else result := -1;
end;

//----------

// close device

function SLibaam.StreamClose : LongInt;
begin
	//case STREAM_CLOSE:
	//  result = 1;
  if Assigned(mDispatcher) then
  begin
    result := mDispatcher(STREAM_CLOSE,0,0);
    mOpened := false;
  end
  else result := -1;
end;

//----------

// STREAM_PLAY
// STREAM_STOP

//----------

// exit library

function SLibaam.StreamExit : LongInt;
begin
	//case STREAM_EXIT:
  if Assigned(mDispatcher) then
  begin
    result := mDispatcher(STREAM_EXIT,0,0);
    mInited := false;
  end
  else result := -1;
end;

//----------

// STREAM_ABOUT

//----------

// device count

function SLibaam.StreamAudioCount : LongInt;
begin
	//case STREAM_AUDIO_COUNT:
	//  result = 1; // jack only
  if Assigned(mDispatcher) then result := mDispatcher(STREAM_AUDIO_COUNT,0,0)
  else result := -1;
end;

//----------

// device name

function SLibaam.StreamAudioName(num:LongInt; str:pchar) : LongInt;
begin
	//case STREAM_AUDIO_NAME:
	//  strcpy((char*)value2, "JACK Audio");
  if Assigned(mDispatcher) then result := mDispatcher(STREAM_AUDIO_NAME,num,LongInt(str))
  else result := -1;
end;

//----------

// get/set frames

function SLibaam.StreamFrames(frames:LongInt=0) : LongInt;
begin
	//case STREAM_FRAMES:
	//  if (value1 > 0) frames = value1;
	//  result = frames;
  if Assigned(mDispatcher) then result := mDispatcher(STREAM_FRAMES,frames,0)
  else result := -1;
end;

//----------

// get/set rate

function SLibaam.StreamRate(setRate:LongInt; getRate:psingle) : LongInt;
begin
	//case STREAM_RATE:
	//  if (value1 == 0) *(float*)value2 = rate;
	//  else if (value1 > 0) rate = value1;
  if Assigned(mDispatcher) then result := mDispatcher(STREAM_RATE,setRate,LongInt(getRate))
  else result := -1;
end;

//----------

// STREAM_LATENCY
// STREAM_RELOAD

//----------------------------------------------------------------------
// midi in
//----------------------------------------------------------------------

// midi in count

function SLibaam.MidiInCount : LongInt;
begin
	//case MIDI_IN_COUNT:
	//  return midiDevices;
  if Assigned(mDispatcher) then result := mDispatcher(MIDI_IN_COUNT,0,0)
  else result := -1;
end;

//----------

// midi in name

function SLibaam.MidiInName(num:LongInt; str:pchar) : LongInt;
begin
	//case MIDI_IN_NAME:
	//  if (value1 >= 0 && value1 < midiDevices)
	//    strcpy((char*)value2, mdevs[value1]->id);
  if Assigned(mDispatcher) then result := mDispatcher(MIDI_IN_NAME,num,LongInt(str))
  else result := -1;
end;

//----------

// enable midi in

function SLibaam.MidiInEnable(num:LongInt; enabled:LongInt) : LongInt;
begin
	//case MIDI_IN_ENABLE:
	//  if (value1 >= 0 && value1 < midiDevices) mdevs[value1]->enable(value2);
  if Assigned(mDispatcher) then result := mDispatcher(MIDI_IN_ENABLE,num,enabled)
  else result := -1;
end;

//----------

// is midi in enabled?

function SLibaam.MidiInEnabled(num:LongInt) : LongInt;
begin
	//case MIDI_IN_ENABLED:
	//  if (value1 >= 0 && value1 < midiDevices) return mdevs[value1]->enabled;
  if Assigned(mDispatcher) then result := mDispatcher(MIDI_IN_ENABLED,num,0)
  else result := -1;
end;

//----------------------------------------------------------------------
// audio in
//----------------------------------------------------------------------

// audio in count

function SLibaam.AudioInCount : LongInt;
begin
	//case AUDIO_IN_COUNT:
	//  return aincount;
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_IN_COUNT,0,0)
  else result := -1;
end;

//----------

// audio in name

function SLibaam.AudioInName(num:LongInt;str:pchar) : LongInt;
begin
	//case AUDIO_IN_NAME:
	//  if (value1 >= 0 && value1 < aincount)
	//    strcpy((char*)value2, ain[value1].id);
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_IN_NAME,num,LongInt(str))
  else result := -1;
end;

//----------

// audio out enable

function SLibaam.AudioInEnable(num:LongInt; enabled:LongInt) : LongInt;
begin
	//case AUDIO_IN_ENABLE:
	//  if (value1 >= 0 && value1 < maxChs)
	//    ain[value1].enabled = value2;
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_IN_ENABLE,num,enabled)
  else result := -1;
end;

//----------

// audio in enabled

function SLibaam.AudioInEnabled(num:LongInt) : LongInt;
begin
	//case AUDIO_IN_ENABLED:
	//  if (value1 >= 0 && value1 < maxChs)
	//    result = ain[value1].enabled;
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_IN_ENABLED,num,0)
  else result := -1;
end;

//----------------------------------------------------------------------
// audio out
//----------------------------------------------------------------------

// audio out count

function SLibaam.AudioOutCount : LongInt;
begin
	//case AUDIO_OUT_COUNT:
	//  return aoutcount;
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_OUT_COUNT,0,0)
  else result := -1;
end;

//----------

// audio out name

function SLibaam.AudioOutName(num:LongInt;str:pchar) : LongInt;
begin
	//case AUDIO_OUT_NAME:
	//  if (value1 >= 0 && value1 < aoutcount)
	//    strcpy((char*)value2, aout[value1].id);
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_OUT_NAME,num,LongInt(str))
  else result := -1;
end;

//----------

// audio out enable

function SLibaam.AudioOutEnable(num:LongInt; enable:LongInt) : LongInt;
begin
	//case AUDIO_OUT_ENABLE:
	//  if (value1 >= 0 && value1 < maxChs)
	//    aout[value1].enabled = value2;
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_OUT_ENABLE,num,enable)
  else result := -1;
end;

//----------

// audio out enabled

function SLibaam.AudioOutEnabled(num:LongInt) : LongInt;
begin
	//case AUDIO_OUT_ENABLED:
	//  if (value1 >= 0 && value1 < maxChs)
	//    result = aout[value1].enabled;
  if Assigned(mDispatcher) then result := mDispatcher(AUDIO_OUT_ENABLED,num,0)
  else result := -1;
end;

//return result; }

//----------------------------------------------------------------------
// global
// - libaam
// - libaam_process
//----------------------------------------------------------------------

{
  todo: callback?
}

(*
function libaam_process(
  aInputs, aOutputs	: ppsingle;
  aNumIns, aNumOuts	: LongInt;
  aFrames						: LongInt;
  aMidiData					: pointer;
  aMidiDataCount		: LongInt) : LongInt; cdecl;
begin
  result := 0;
end;
*)

//----------------------------------------------------------------------



//----------------------------------------------------------------------
end.




