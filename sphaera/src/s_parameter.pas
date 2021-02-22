{$include sphaera.inc}
unit s_parameter;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_array;

type

  SParameter = class;
  SParameterArray = specialize SArray<SParameter>;

  //----------

  SParameter = class
  protected
    FType     : LongInt;
    FName     : PChar;
    FLabel    : PChar;
    FDisplay  : array[0..255] of char;
    FDefault  : Single;
    FFlags    : LongWord;
    FIndex    : LongInt;
    FConnection : LongInt;
  public
    constructor create;
    constructor create(AName:PChar; AValue:Single=0);
    destructor  destroy; override;
    { }
    function    getName : PChar; virtual;
    function    getLabel : PChar; virtual;
    function    getDisplay(AValue:Single) : PChar; virtual;
    function    getDefault : Single; virtual;
    function    getFlags : LongWord; virtual;
    function    getIndex : LongInt; virtual;
    procedure   setIndex(AIndex:LongInt); virtual;
    function    from01(AValue:single) : single; virtual;
    function    to01(AValue:single) : single; virtual;
  public
    property    index       : LongInt read FIndex write FIndex;
    property    connection  : LongInt read FConnection write FConnection;
  end;

  //----------

  SParamFloat = class(SParameter)
  protected
    FMin        : single;
    FMax        : single;
    FStep       : single;
    FRange      : single;
    FInvRange   : single;
    FStepped    : boolean;
    FNumSteps   : integer;
    FStepSize   : single;
    FHalfStep   : single;
  private
    procedure   setup(AName,ALabel:PChar; AVal,AMin,AMax,AStep:single);
  public
    constructor create(AName:PChar; AVal:Single; AMin:Single=0; AMax:Single=1; AStep:Single=0);
    function    to01(AValue:single) : single; override;
    function    from01(AValue:single) : single; override;
  end;

  //----------

  SParamInt = class(SParamFloat)
  public
    constructor create(AName:PChar; AVal:single; AMin:single=0; AMax:single=1);
    function    getDisplay(AValue:Single) : PChar; override;
  end;

  //----------

  SParamText = class(SParamInt)
  private
    FStrings    : PPChar;
  public
    constructor create(AName:PChar; AIndex,ANum:integer; AText:PPChar);
    function    getDisplay(AValue:Single) : PChar; override;
  end;

  //SParamBool
  //SParamProc // callback

  //----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_string,
  s_util;

//----------------------------------------------------------------------
// SParameter
//----------------------------------------------------------------------

constructor SParameter.create;
begin
  inherited;
  FType     := spt_parameter;
  FName     := 'param';
  FLabel    := '';
  FDisplay  := '';
  FDefault  := 0;
  FFlags    := spf_automate;
  FIndex    := -1;
  FConnection := -1;
end;

//----------

constructor SParameter.create(AName:PChar; AValue:Single);
begin
  inherited create;
  FType     := spt_parameter;
  FFlags    := spf_automate;
  FName     := AName;
  FLabel    := '';
  FDisplay  := '';
  FDefault  := AValue;//to01(AValue);
  FIndex    := -1;
end;

//----------

destructor SParameter.destroy;
begin
  inherited;
end;

//----------

function SParameter.getName : PChar;
begin
  result := FName;
end;

//----------

function SParameter.getLabel : PChar;
begin
  result := FLabel;
end;

//----------

function SParameter.getDisplay(AValue:Single) : PChar;
begin
   SFloatToString(FDisplay,from01(AValue));
   result := FDisplay;
end;

//----------

function SParameter.getDefault : Single;
begin
  result := FDefault;
end;

//----------

function SParameter.getFlags : LongWord;
begin
  result := FFlags;
end;

//----------

function SParameter.getIndex : LongInt;
begin
  result := FIndex;
end;

//----------

procedure SParameter.setIndex(AIndex:LongInt);
begin
  FIndex := AIndex;
end;

//----------

function SParameter.from01(AValue:single) : single;
begin
  result := AValue;
end;

//----------

function SParameter.to01(AValue:single) : single;
begin
  result := AValue;
end;

//----------------------------------------------------------------------
// SParamFloat
//----------------------------------------------------------------------

constructor SParamFloat.create(AName:PChar; AVal:single; AMin:single=0; AMax:single=1; AStep:single=0);
begin
  inherited create;
  FType := spt_paramFloat;
  setup(AName,'',AVal,AMin,AMax,AStep);
end;

//----------

procedure SParamFloat.setup(AName,ALabel:PChar; AVal,AMin,AMax,AStep:single);
begin
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

// 0..1 to user scale

function SParamFloat.from01(AValue:Single) : Single;
var
  n  : single;
  st : integer;
begin
  if FStepped then
  begin
    n := AValue * FNumSteps;
    st := trunc(n);
    if st > (FNumSteps-1) then st := (FNumSteps-1);
    Result := FMin + (st * FStep);
  end
  else
  begin
    Result := FMin + (AValue*FRange);
  end;
end;

//----------

// user scale to 0..1

function SParamFloat.to01(AValue:Single) : Single;
begin
  Result := ((AValue-FMin)*FInvRange);
end;

//----------------------------------------------------------------------
// SParamInt
//----------------------------------------------------------------------

constructor SParamInt.create(AName:PChar; AVal:single; AMin:single=0; AMax:single=1);
begin
  inherited create(AName,AVal,AMin,AMax,1);
  FType := spt_paramInt;
end;

//----------

function SParamInt.getDisplay(AValue:Single) : PChar;
begin
  SIntToString(FDisplay,trunc(from01(AValue)));
  result := FDisplay;
end;

//----------------------------------------------------------------------
// SParamText
//----------------------------------------------------------------------

constructor SParamText.create(AName:PChar; AIndex,ANum:LongInt; AText:PPChar);
begin
  inherited create(AName,AIndex,0,ANum-1);
  FType := spt_paramText;
  FStrings := AText;
end;

//----------

// TODO !!

function SParamText.getDisplay(AValue:Single) : PChar;
var
  i:integer;
begin
  i := trunc(from01(AValue));
  SStrcpy(FDisplay, FStrings[i] );
  result := FDisplay;
end;

//----------------------------------------------------------------------
end.



