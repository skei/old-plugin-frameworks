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
  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter;

type

  myDescriptor = class(ZDescriptor)
  public
    procedure on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
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
    procedure on_processBlock({%H-}AInputs,{%H-}AOutputs:PPZSample; {%H-}ALength:LongWord); override;
    procedure on_processSample(AInputs, AOutputs: PPZSample); override;
  end;

//----------

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_util;

const
  txt_dir : array[0..1] of PChar = ('up','down');

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
  FName     := 'midi_fibonacci';
  FAuthor   := 'th.skei';
  FProduct  := 'powered by zenith';
  FVersion  := 0;
  FUniqueId := Z_MAGIC + $0201;
  ZSetFlag(FFlags, zpf_perSample + zpf_sendMidi );
  appendParameter( ZParamInt.create( 'speed (ms)',125, 30, 300 ));
  appendParameter( ZParamInt.create( 'first num', 0,  -10, 10 ));
  appendParameter( ZParamInt.create( 'second num',1,  -10, 10 ));
  appendParameter( ZParamInt.create( 'note start',60,  0,  127 ));
  appendParameter( ZParamInt.create( 'num notes', 24,  1,  127 ));
  appendParameter( ZParamText.create('direction', 0,   2,  txt_dir ));

end;

//--------------------------------------------------
// instance
//--------------------------------------------------

procedure myInstance.on_create;
begin
  count := 0;
  num1  := 0;
  num2  := 0;
  prev_note := -1;
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var i : longint;
    p : ZParameter;
    v : single;
begin
  p := FParameters[AIndex];
  v := p.from01(AValue);
  i := trunc(v);
  //writeln('FSampleRate: ',FSampleRate);
  //writeln('AIndex: ',AIndex);
  //writeln('v: ',v:6:3);
  //writeln('i: ',i);
  case AIndex of
    0 : begin
          speed := trunc( FSampleRate * v / 1000 );
          count := count mod speed;
        end;
    1 : num1 := i;
    2 : num2 := i;
    3 : nstart := i;
    4 : begin
          nnum := i;
          writeln('num notes: ',nnum);
        end;
    5 : dir := i;
  end;
  //writeln('speed: ',speed);
  //_out   := 0;
  count  := 0;
end;

//----------

procedure myInstance.on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
begin
  offset := 0;
end;

//----------

procedure myInstance.on_processSample(AInputs, AOutputs: PPZSample);
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
    if prev_note >= 0 then FHost.midiOut(offset,$80,prev_note,0);
    FHost.midiOut(offset,$90,note,127);
    prev_note := note;
  end;
  AOutputs[0]^ := AInputs[0]^;
  AOutputs[1]^ := AInputs[1]^;
end;

//----------------------------------------------------------------------
end.

