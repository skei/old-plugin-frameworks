{$include sphaera.inc}
unit midi_transpose;

{
  untested -1 clearing of buffer...
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_parameter;

type

  myPlugin = class(SPlugin)
  private
    FTranspose  : LongInt;
    FNoteBuffer : array[0..255{127}] of LongInt;
    procedure noteOn(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
    procedure noteOff(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte); override;
    procedure on_processSample(AInputs, AOutputs: PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //z_memory,
  s_util;

//--------------------------------------------------

procedure myPlugin.on_create;
var i : longint;
begin
  FName     := 'midi_transpose';
  FAuthor   := 'th.skei';
  FProduct  := 'powered by zenith';
  FVersion  := 0;
  FUniqueId := S_MAGIC + $0201;
  SSetFlag(FFlags, spf_perSample + spf_receiveMidi + spf_sendMidi );
  appendParameter( SParamInt.create('transpose', 0, -24, 24 ) );
  //
  //ZMemset(@FNoteBuffer[0],128,0);
  for i := 0 to 255 do FNoteBuffer[i] := -1;
  FTranspose := 0;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  i : longint;
  //p : ZParameter;
  v : single;
begin
  i := AIndex;
  //p := FParameters[i];
  v := AValue;//p.from01(AValue);
  case i of
    0 :
      begin
        FTranspose := trunc(v);
      end;
  end;
end;

//----------

// [internal]
procedure myPlugin.noteOn(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
var
  note : Byte;
begin
  note := (AMsg2 + FTranspose) and $7F;
  FNoteBuffer[AMsg2] := note;
  midiOut(AOffset,AMsg1,note,AMsg3);
end;

//----------

// [internal]
procedure myPlugin.noteOff(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
var
  note : longint;
begin
  note := FNoteBuffer[AMsg2];
  if note <> -1 then midiOut(AOffset,AMsg1,note,AMsg3)
  else midiOut(AOffset,AMsg1,AMsg2,AMsg3);
  FNoteBuffer[AMsg2] := -1;
end;

//----------

procedure myPlugin.on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
begin
  case (AMsg1 and $F0) of
    $80: noteOff(AOffset,AMsg1,AMsg2,AMsg3);
    $90: if AMsg3 = 0 then noteOff(AOffset,AMsg1,AMsg2,AMsg3)
                      else noteOn(AOffset,AMsg1,AMsg2,AMsg3)
    else midiOut(AOffset,AMsg1,AMsg2,AMsg3);
  end;

end;

//----------

procedure myPlugin.on_processSample(AInputs, AOutputs: PPSingle);
begin
  AOutputs[0]^ := AInputs[0]^;
  AOutputs[1]^ := AInputs[1]^;
end;

//----------------------------------------------------------------------
end.

{

  // http://www.somascape.org/midi/tech/spec.html

  80  Note Off, note, velocity
  90  Note On, note, velocity
  A0  AfterTouch (ie, key pressure), note, pressure
  B0  Control Change, num, val
  C0  Program (patch) change, num
  D0  Channel Pressure, pressure
  E0  Pitch Wheel, low7bits, high7bits (0x2000=center)

  F0  Start sysex, vendor-id, ..., [until Sysex End]
  F1  MTC Quarter Frame, time code value
  F2  Song Position, 2 bytes (like pitchwheel) = Midi Beats (see below)
  F3  Song Select, num
  F4
  F5  [unofficial bus select, bus-number]
  F6  Tune Request
  F7  Sysex End
  F8  Midi Clock
  F9  [Tick (every 10 ms)]
  FA  Midi Start
  FB  Midi Continue
  FC  Midi Stop
  FD
  FE  Active Sense (approximately every 300 ms)
  FF  Reset

  Midi Meats

  - Songs are always assumed to start on a MIDI Beat of 0.
  - Each MIDI Beat spans 6 MIDI Clocks.
  - In other words, each MIDI Beat is a 16th note
    (since there are 24 MIDI Clocks in a quarter note).

  Midi Clock

  There are 24 MIDI Clocks in every quarter note. (12 MIDI Clocks in an eighth
  note, 6 MIDI Clocks in a 16th, etc). Therefore, when a slave device counts
  down the receipt of 24 MIDI Clock messages, it knows that one quarter note has
  passed. When the slave counts off another 24 MIDI Clock messages, it knows
  that another quarter note has passed. Etc

  Tick

  While a master device's "clock" is playing back, it will send a continuous
  stream of MIDI Tick events at a rate of one per every 10 milliseconds.

}
