{$include sphaera.inc}
unit midi_syncdelay;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_parameter;

const
  S_MAXEVENTS = 1024;

type

  SMidiEvent = object
    FOffset : LongWord;
    FMsg1 : Byte;
    FMsg2 : Byte;
    FMsg3 : Byte;
    FReserved : Byte;
  end;

  //----------

  myPlugin = class(SPlugin)
  private
    FEvents     : array[0..S_MAXEVENTS-1] of SMidiEvent;
    FNumEvents  : LongWord;
    FCountdown  : LongWord;
    FBeats      : Single;
    FPrev  : LongWord;
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte); override;
    procedure on_processBlock({%H-}AInputs,{%H-}AOutputs:PPSingle; {%H-}ALength:LongInt); override;
    procedure on_processSample(AInputs, AOutputs: PPSingle); override;
  private
    function  handle_playstate(AState:LongWord) : Boolean;
    procedure handle_timing(ASize:LongWord);
    procedure handle_tick(AOffset:LongWord);
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_util;

//--------------------------------------------------

procedure myPlugin.on_create;
begin
  FName     := 'midi_syncdelay';
  FAuthor   := 'th.skei';
  FProduct  := 'powered by zenith';
  FVersion  := 0;
  FUniqueId := S_MAGIC + $0201;
  SSetFlag(FFlags, spf_perSample + spf_receiveMidi + spf_sendMidi );
  appendParameter( SParamFloat.create('beats', 4, 0.25, 16, 0.25 ) );
  //
  FNumEvents  := 0;
  FCountdown  := 0;
  FPrev := 0;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  //p : ZParameter;
  v : single;
begin
  //p := FParameters[AIndex];
  v := AValue;//p.from01(AValue);
  case AIndex of
    0 : FBeats := v;
  end;
end;

//----------

procedure myPlugin.on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
begin
  if FNumEvents < S_MAXEVENTS then
  begin
    FEvents[FNumEvents].FOffset := AOffset;
    FEvents[FNumEvents].FMsg1 := AMsg1;
    FEvents[FNumEvents].FMsg2 := AMsg2;
    FEvents[FNumEvents].FMsg3 := AMsg3;
    inc(FNumEvents);
  end;
end;

//----------

procedure myPlugin.on_processBlock(AInputs,AOutputs:PPSingle; ALength:LongInt);
var
  state   : LongInt;
  size    : LongWord;
  srate   : Single;
  tempo   : single;
  spos    : Single;
  beatpos : single;
begin
  state := getTime(srate{%H-},spos{%H-},tempo{%H-},beatpos{%H-});
  size := trunc( FBeats * srate / (tempo/60) );
  if handle_playstate(state) then handle_timing(size);
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
begin
  AOutputs[0]^ := AInputs[0]^;
  AOutputs[1]^ := AInputs[1]^;
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

function myPlugin.handle_playstate(AState:LongWord) : Boolean;
begin
  result := (AState and spt_play) <> 0; // playing
  if AState <> FPrev then
  begin
    if {playing} result then FCountdown := 0;
    FPrev := AState;
  end;
end;

//----------

procedure myPlugin.handle_timing(ASize:LongWord);
var
  offset,block : LongWord;
begin
  offset := 0;
  block  := _blockSize;
  while block > 0 do
  begin
    if FCountdown >= block then
    begin
      FCountdown -= block;
      block := 0;
    end
    else
    begin
      offset += FCountdown;
      block -= FCountdown;
      handle_tick(offset);
      FCountdown := ASize;
      if FCountdown < 64 then FCountdown := 64; // safety...
    end;
  end;
end;

//----------

procedure myPlugin.handle_tick(AOffset:LongWord);
var
  i:longint;
begin
  if FNumEvents > 0 then
  begin
    for i := 0 to FNumEvents-1 do
    begin
      midiOut(AOffset,FEvents[i].FMsg1,FEvents[i].FMsg2,FEvents[i].FMsg3);
    end;
    FNumEvents := 0;
  end;
end;

//----------------------------------------------------------------------
end.

