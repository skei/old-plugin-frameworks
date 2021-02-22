{$include sphaera.inc}
unit midi_fibonacci;

//TODO:
//  - prev note off before noew note
//  - note length
//  - tempo sync to host tempo
//  - active only in play?

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
    count   : LongInt;
    num1    : LongInt;
    num2    : LongInt;
    speed   : LongInt;
    nstart  : LongInt;
    nnum    : LongInt;
    dir     : LongInt;
    //_out    : LongInt;
    //note    : LongInt;
    offset  : LongInt;
    prev_note : LongInt;
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processBlock({%H-}AInputs,{%H-}AOutputs:PPSingle; {%H-}ALength:LongInt); override;
    procedure on_processSample(AInputs, AOutputs: PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_util;

const
  txt_dir : array[0..1] of PChar = ('up','down');

//----------

procedure myPlugin.on_create;
begin
  FName     := 'midi_fibonacci';
  FAuthor   := 'th.skei';
  FVersion  := 0;
  FUniqueId := S_MAGIC + $0201;
  SSetFlag(FFlags, spf_perSample);
  SSetFlag(FFlags, spf_sendMidi);
  appendParameter( SParamInt.create( 'speed (ms)',125, 30, 300 ));
  appendParameter( SParamInt.create( 'first num', 0,  -10, 10 ));
  appendParameter( SParamInt.create( 'second num',1,  -10, 10 ));
  appendParameter( SParamInt.create( 'note start',60,  0,  127 ));
  appendParameter( SParamInt.create( 'num notes', 24,  1,  127 ));
  appendParameter( SParamText.create('direction', 0,   2,  txt_dir ));
  //
  count := 0;
  num1  := 0;
  num2  := 0;
  prev_note := -1;
end;

//----------

// ouch, FSampleRate is 0..

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var i : longint;
    //p : ZParameter;
    v : single;
begin
  //p := FParameters[AIndex];
  v := AValue;//p.from01(AValue);
  i := trunc(v);
  //writeln('FSampleRate: ',FSampleRate);
  //writeln('AIndex: ',AIndex);
  //writeln('v: ',v:6:3);
  //writeln('i: ',i);
  case AIndex of
    0 : begin
          speed := trunc( _sampleRate * v / 1000 );
          count := count mod speed;
        end;
    1 : num1 := i;
    2 : num2 := i;
    3 : nstart := i;
    4 : begin
          nnum := i;
          //writeln('num notes: ',nnum);
        end;
    5 : dir := i;
  end;
  //writeln('speed: ',speed);
  //_out   := 0;
  count  := 0;
end;

//----------

procedure myPlugin.on_processBlock(AInputs,AOutputs:PPSingle; ALength:LongInt);
begin
  offset := 0;
end;

//----------

procedure myPlugin.on_processSample(AInputs, AOutputs: PPSingle);
var
  _out : LongInt;
  note : LongInt;
begin
  offset += 1;
  count += 1;
  if count >= speed then
  begin
    //writeln('.');
    count := 0;
    _out := (num1 + num2) mod nnum;
    num1 := num2;
    num2 := _out;
    if dir < 0 then note := nstart + _out
               else note := nstart + nnum - 1 - _out;
    if prev_note >= 0 then {FHost.}midiOut(offset,$80,prev_note,0);
    {FHost.}midiOut(offset,$90,note,127);
    prev_note := note;
  end;
  AOutputs[0]^ := AInputs[0]^;
  AOutputs[1]^ := AInputs[1]^;
end;

//----------------------------------------------------------------------
end.

