unit z_parameter;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{
  name
  def val
  min, max, step
  ptr
}

uses
  z_array;

type

  //ZParameter = class;
  //ZParameterArray = specialize ZArray<ZParameter>;
  //ZParameterClass = class of ZParameter;

  //----------

  ZParameter = class
  private
    FDefault  : Single;
    FDisplay  : array[0..255] of char;
    FFlags    : LongWord;
    FIndex    : LongInt;
    FLabel    : PChar;
    FName     : PChar;
  public
    constructor create;
    constructor create(AName:PChar; AValue:Single=0);
    destructor  destroy; override;
    { }
    function    from01(AValue:Single) : Single; virtual;
    function    getFlags : LongWord; virtual;
    function    getIndex : LongInt; virtual;
    function    getDefault : Single; virtual;
    function    getName : PChar; virtual;
    function    getLabel : PChar; virtual;
    function    getDisplay(AValue:Single) : PChar; virtual;
    procedure   setIndex(AIndex:LongInt); virtual;
    function    to01(AValue:Single) : Single; virtual;
  end;

  ZParameterArray = specialize ZArray<ZParameter>;

  //----------

  ZParamFloat = class(ZParameter)
  protected
    FMin        : single;
    FMax        : single;
    FStep       : single;
    FRange      : single;
    FInvRange   : single;
    FStepped    : boolean;
    FNumSteps   : integer;
    FStepSize   : single;   // 0..1
    FHalfStep   : single;
  private
    procedure   setup(AName,ALabel:PChar; AVal,AMin,AMax,AStep:single); //override;
  public
    constructor create(AName:PChar; AVal:Single; AMin:Single=0; AMax:Single=1; AStep:Single=0);
    function    to01(AValue:single) : single; override;
    function    from01(AValue:single) : single; override;
  end;

  //----------

  ZParamInt = class(ZParamFloat)
  public
    constructor create(AName:PChar; AVal:single; AMin:single=0; AMax:single=1);
    function    getDisplay(AValue:single) : PChar; override;
  end;

  //----------

  ZParamText = class(ZParamInt)
  private
    FStrings    : PPChar;
  public
    constructor create(AName:PChar; AIndex,ANum:integer; AText:PPChar);
    function    getDisplay(AValue:Single) : PChar; override;
  end;

  //ZParamBool
  //ZParamProc // callback

  //----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_const,
  z_string,
  z_util;

//----------------------------------------------------------------------
// ZParameter
//----------------------------------------------------------------------

constructor ZParameter.create;
begin
  FDefault := 0;
  FDisplay := '';
  FFlags := zpf_automate;
  FIndex := -1;
  FLabel := '';
  FName := 'param';
end;

//----------

constructor ZParameter.create(AName:PChar; AValue:Single);
begin
  FDefault := AValue;
  FDisplay := '';
  FFlags := zpf_automate;
  FIndex := -1;
  FLabel := '';
  FName := AName;
end;

//----------

{constructor ZParameter.create(AName,ALabel:PChar; AValue:Single);
begin
  FDefault := AValue;
  FDisplay := '';
  FFlags := 0;
  FIndex := -1;
  FLabel := ALabel;
  FName := AName;
end;}

//----------

destructor ZParameter.destroy;
begin
end;

//----------

function ZParameter.from01(AValue:Single) : Single;
begin
  result := AValue;
end;

//----------

function ZParameter.getDefault : Single;
begin
  result := FDefault;
end;

//----------

function ZParameter.getDisplay(AValue:Single) : PChar;
begin
   ZFloatToString(FDisplay,AValue);
   result := FDisplay;
end;

//----------

function ZParameter.getFlags : LongWord;
begin
  result := FFlags;
end;

//----------

function ZParameter.getIndex : LongInt;
begin
  result := FIndex;
end;

//----------

function ZParameter.getLabel : PChar;
begin
  result := FLabel;
end;

//----------

function ZParameter.getName : PChar;
begin
  result := FName;
end;

//----------

procedure ZParameter.setIndex(AIndex:LongInt);
begin
  FIndex := AIndex;
end;

//----------

function ZParameter.to01(AValue:Single) : Single;
begin
  result := AValue;
end;

//----------------------------------------------------------------------
// ZParamFloat
//----------------------------------------------------------------------

constructor ZParamFloat.create(AName:PChar; AVal:single; AMin:single=0; AMax:single=1; AStep:single=0);
begin
  inherited create;//(AName,AVal);
  setup(AName,'',AVal,AMin,AMax,AStep);
end;

//----------

procedure ZParamFloat.setup(AName,ALabel:PChar; AVal,AMin,AMax,AStep:single);
begin
  //inherited setup(AName,ALabel,AVal);
  FIndex    := -1;
  FName     := AName;
  FLabel    := ALabel;
  //FDefault  := AVal;
  FMin      := AMin;
  FMax      := AMax;
  FStep     := AStep;
  FRange    := FMax - FMin;
  FInvRange := 1 / FRange;
  if FStep > 0 then
  begin
    FStepped  := true;
    FNumSteps := 1 + round( FRange / FStep );
    FStepSize := 1 / (FNumSteps - 1);
    FHalfStep := 0.5 * FStepSize;
  end
  else
  begin
    FStepped  := false;
    FNumSteps := 1;
    FStepSize := 1;
    FHalfStep := 0;
  end;
  FDefault  := to01(AVal);
end;

//----------

function ZParamFloat.to01(AValue:single) : single;
begin
  result := ((AValue-FMin)*FInvRange);
end;

//----------

function ZParamFloat.from01(AValue:single) : single;
var
  n  : single;
  st : integer;
begin
  if FStepped then
  begin
    n := AValue * FNumSteps;
    st := trunc(n);
    if st > (FNumSteps-1) then st := (FNumSteps-1);
    result := FMin + (st * FStep);
  end
  else
  begin
    result := FMin+(AValue*FRange);
  end;
end;

//----------------------------------------------------------------------
// ZParamInt
//----------------------------------------------------------------------

constructor ZParamInt.create(AName:PChar; AVal:single; AMin:single=0; AMax:single=1);
begin
  inherited create(AName,AVal,AMin,AMax,1);
  //setup(AName,'',AVal,AMin,AMax,1);
end;

//----------

function ZParamInt.getDisplay(AValue:single) : PChar;
begin
  ZIntToString(FDisplay,trunc(AValue));
  result := FDisplay;
end;

//----------------------------------------------------------------------
// ZParamText
//----------------------------------------------------------------------

constructor ZParamText.create(AName:PChar; AIndex,ANum:LongInt; AText:PPChar);
begin
  inherited create(AName,AIndex,0,ANum-1);
  FStrings := AText;
end;

//----------

function ZParamText.getDisplay(AValue:single) : PChar;
var
  i:integer;
begin
  i := trunc(AValue);
  ZStrcpy(FDisplay, FStrings[i] );
  result := FDisplay;
end;

//----------------------------------------------------------------------
end.


