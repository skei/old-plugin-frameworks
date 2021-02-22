unit midi_syncdelay;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter;

const
  Z_MAXEVENTS = 1024;

type

  ZMidiEvent = object
    FOffset : LongWord;
    FMsg1 : Byte;
    FMsg2 : Byte;
    FMsg3 : Byte;
    FReserved : Byte;
  end;

  myDescriptor = class(ZDescriptor)
  public
    procedure on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    FEvents     : array[0..Z_MAXEVENTS-1] of ZMidiEvent;
    //FMaxEvents  : LongWord;
    numevents   : LongWord;
    //prevstate   : LongWord;
    //isplaying   : Boolean;//LongWord;
    countdown   : LongWord;
    beats       : Single;
    play_state,
    prev_state : LongWord;
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte); override;
    procedure on_processBlock({%H-}AInputs,{%H-}AOutputs:PPZSample; {%H-}ALength:LongWord); override;
    procedure on_processSample(AInputs, AOutputs: PPZSample); override;
  end;

//----------

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //z_memory,
  z_util;

//----------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

//--------------------------------------------------
// descriptor
//--------------------------------------------------

procedure myDescriptor.on_create;
begin
  FName     := 'midi_syncdelay';
  FAuthor   := 'th.skei';
  FProduct  := 'powered by zenith';
  FVersion  := 0;
  FUniqueId := Z_MAGIC + $0201;
  ZSetFlag(FFlags, zpf_perSample + zpf_receiveMidi + zpf_sendMidi );
  appendParameter( ZParamFloat.create('beats', 4, 0.25, 16, 0.25 ) );
end;

//--------------------------------------------------
// instance
//--------------------------------------------------

procedure myInstance.on_create;
begin
  numevents  := 0;
  countdown  := 0;
  play_state := 0;
  prev_state := 0;
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : single;
begin
  p := FParameters[AIndex];
  v := p.from01(AValue);
  case AIndex of
    0 : beats := v;
  end;
end;

//----------

procedure myInstance.on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
begin
  if numevents < Z_MAXEVENTS then
  begin
    FEvents[numevents].FOffset := AOffset;
    FEvents[numevents].FMsg1 := AMsg1;
    FEvents[numevents].FMsg2 := AMsg2;
    FEvents[numevents].FMsg3 := AMsg3;
    inc(numevents);
  end;
end;

//----------

procedure myInstance.on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
var
  i : longword;
  srate,tempo : single;
  spos,bpos : single;
  offset,block : LongWord;
  isplaying : Boolean;
begin
  bpos:=0; spos:=0; tempo:=0; srate:=0;
  play_state := FHost.getTime(srate,spos,tempo,bpos);
  if play_state <> prev_state then
  begin
    if (play_state and zpt_play) <> 0 then
    begin
      isplaying := true;
      countdown := 0; // speed;
    end
    else isplaying := false;
    prev_state := play_state;
  end; // play <> prev
  if isplaying then
  begin
    offset := 0;
    block := FBlockSize;
    while block > 0 do
    begin
      if countdown >= block then
      begin
        countdown -= block;
        block := 0;
      end
      else
      begin // count<block
        offset += countdown;
        block -= countdown;
        if numevents > 0 then
        begin
          for i := 0 to numevents-1 do
          begin
            FHost.midiOut(offset,FEvents[i].FMsg1,FEvents[i].FMsg2,FEvents[i].FMsg3);
          end;
          numevents := 0;
        end;
        countdown := trunc( beats * srate{FSampleRate} / (tempo/60) );
        if countdown < 64 then countdown := 64; // safety...
      end; // count<block
    end; // block > 0
  end; // isplaying
end;

//----------

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
begin
  AOutputs[0]^ := AInputs[0]^;
  AOutputs[1]^ := AInputs[1]^;
end;

//----------------------------------------------------------------------
end.

