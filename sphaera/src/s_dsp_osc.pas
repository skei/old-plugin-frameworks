{$include sphaera.inc}
unit s_dsp_osc;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const
  // osc types
  osc_None    = 0;
  osc_Const   = 1;
  osc_Input   = 2;
  osc_Noise   = 3;
  osc_Ramp    = 4;
  osc_Saw     = 5;
  osc_Squ     = 6;
  osc_Sin     = 7;

type

  SDspOsc = class
  private
    FType : LongInt;
    FFreq : Single;
    FPhase : Single;
    FIRate : Single;
  public
    constructor create;
    procedure   setup(AType:LongInt=0; AFreq:Single=0; APhase:Single=0);
    procedure   setSampleRate(ARate:Single);
    function    process(AValue:Single=0) : Single;
    function    process(AValue:Single; APD:Single) : Single;
  public
    property    _type  : LongInt read FType write FType;
    property    _freq  : Single read FFreq write FFreq;
    property    _phase : Single read FPhase write FPhase;
    property    _rate  : Single write setSampleRate;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //Math,
  s_const,
  s_random;

//----------

constructor SDspOsc.create;
begin
  inherited;
end;

//----------

procedure SDspOsc.setup(AType:LongInt; AFreq:Single; APhase:Single);
begin
  FType := AType;
  FFreq := AFreq;
  FPhase := APhase;
end;


procedure SDspOsc.setSampleRate(ARate:Single);
begin
  if ARate > 0 then FIRate := 1 / ARate
  else FIRate := 1;
end;

function SDspOsc.process(AValue:Single) : Single;
begin
  result := 0; // osc_None
  case FType of
    osc_Const: result := 1;
    osc_Input: result := AValue;
    osc_Noise: result := SRandomSigned;
    osc_Ramp:  result := (FPhase*2)-1;
    osc_Saw:   result := 1 - (FPhase*2);
    osc_Squ:   if FPhase < 0.5 then result := 1 else result := -1;
    osc_Sin:   result := sin(FPhase*S_PI2);
  end;
  FPhase += FFreq * FIRate;
  if FPhase >= 1 then FPhase -= 1;
  if FPhase < 0 then FPhase += 1;
  //while FPhase >= 1 do FPhase -= 1;
  //while FPhase < 0 do FPhase += 1;
end;

//----------

function SDspOsc.process(AValue:Single; APD:Single) : Single;
var
  p2 : Single;
begin
  p2 := FPhase + APD;
  if p2 >= 1 then p2 -= 1;
  if p2 < 0 then p2 += 1;
  result := 0; // osc_None
  case FType of
    osc_Const: result := 1;
    osc_Input: result := AValue;
    osc_Noise: result := SRandomSigned;
    osc_Ramp:  result := (p2*2)-1;
    osc_Saw:   result := 1-(p2*2);
    osc_Squ:   if p2 < 0.5 then result := 1 else result := -1;
    osc_Sin:   result := sin(p2*S_PI2);
  end;
  FPhase += FFreq * FIRate;
  if FPhase >= 1 then FPhase -= 1;
  if FPhase < 0 then FPhase += 1;
  //while FPhase >= 1 do FPhase -= 1;
  //while FPhase < 0 do FPhase += 1;
end;

//----------------------------------------------------------------------
end.

