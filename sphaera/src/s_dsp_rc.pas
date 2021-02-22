{$include sphaera.inc}
unit s_dsp_rc;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SDspRC = class
  private
    FValue      : Single;
    FTarget     : Single;
    FWeight     : Single;
  public
    constructor create;
    destructor  destroy; override;
    procedure   setup(AValue:Single=0; ATarget:Single=0; AWeight:Single=0);
    procedure   setFrequency(AFrequency:Single; ASampleRate:Single);
    procedure   setTime(ATime:Single);
    function    process : Single;
    function    process(AValue:Single) : Single;

  public
    property    _value  : Single read FValue write FValue;
    property    _target : Single read FTarget write FTarget;
    property    _weight : Single read FWeight write FWeight;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const;//,
  //s_math;

//----------

constructor SDspRC.create;
begin
  FValue  := 0;
  FTarget := 1;
  FWeight := 1;
end;

//----------

destructor SDspRC.destroy;
begin
end;

//----------

procedure SDspRC.setup(AValue:Single; ATarget:Single; AWeight:Single);
begin
  FValue  := AValue;
  FTarget := ATarget;
  FWeight := AWeight;
end;

  //----------


procedure SDspRC.setFrequency(AFrequency:Single; ASampleRate:Single);
begin
  if ASampleRate > 0 then FWeight := 1 - exp(-S_PI2 * AFrequency / ASampleRate )
  else FWeight := 0;
end;

//----------

{
  ATime is the time it takes the filter to decay to
  36.8% of its initial input or reach 63.2% of its final output.
}

procedure SDspRC.setTime(ATime:Single);
begin
  if ATime > 0 then FWeight := 1 - exp(-1 / ATime)
  else FWeight := 0;
end;

//----------

function SDspRC.process : Single;
begin
  FValue += (FTarget-FValue) * FWeight;
  result := FValue;
end;

//----------

function SDspRC.process(AValue:Single) : Single;
begin
  FTarget := AValue;
  result := self.process();
end;

//----------------------------------------------------------------------
end.
