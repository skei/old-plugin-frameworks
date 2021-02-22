{$include sphaera.inc}
unit s_dsp_env;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const
  env_Off       = 0;
  env_Attack    = 1;
  env_Decay     = 2;
  env_Sustain   = 3;
  env_Release   = 4;
  env_Finished  = 5;
  env_Threshold = S_EPSILON;
  env_MaxStages = 5;

type

  SDspEnvStage = record
    FTarget : Single;
    FRate : Single;
  end;

  //----------

  SDspEnv = class
  private
    FScale : Single;
    FStage : LongInt;
    FValue : Single;
  public
    constructor create;
    procedure   noteOn; virtual;
    procedure   noteOff; virtual;
    function    process : Single; virtual;
    property    scale : Single read FScale write FScale;
    property    stage : LongInt read FStage write FStage;
    property    value : Single read FValue write FValue;
  end;

  SDspEnvADSR = class(SDspEnv)
  private
    FStages : array[0..4] of SDspEnvStage; // -,a,d,s,r
  public
  constructor create;
  procedure   setStage(AStage:LongInt; ATarget,ARate:Single);
  procedure   setADSR(a,d,s,r:Single);

  procedure   noteOn; override;
  procedure   noteOff; override;
  function    process : Single; override;

  end;


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_math;

//----------

constructor SDspEnv.create;
begin
  inherited;
  FScale := 50.0;//6.0f;
  FStage := env_Off;
  FValue := 0;
end;

//----------

procedure SDspEnv.noteOn;
begin
  FStage := env_Attack;
  FValue := 1;
end;

//----------

procedure SDspEnv.noteOff;
begin
  FStage := env_Finished;
  FValue := 0;
end;

//----------

function SDspEnv.process : Single;
begin
  result := FValue;
end;

//----------------------------------------------------------------------

constructor SDspEnvADSR.create;
begin
  inherited;
  FScale := 50.0;//6.0f;
  FStage := env_Off;
  FValue := 0;
end;

//----------

procedure SDspEnvADSR.setStage(AStage:LongInt; ATarget,ARate:Single);
var
  r1,r2 : Single;
begin
  r1 := ARate*FScale;
  r2 := r1*r1*r1 + 1;
  //float r2 = r1*r1 + 1;
  FStages[AStage].FTarget := ATarget;
  FStages[AStage].FRate   := 1 / r2;
end;

//----------

procedure SDspEnvADSR.setADSR(a,d,s,r:Single);
begin
  setStage(env_Attack, 1,a);
  setStage(env_Decay,  s,d);
  //setStage(env_Sustain,s,1);
  FStages[env_Sustain].FTarget := s;
  FStages[env_Sustain].FRate   := 1;
  setStage(env_Release,0,r);
end;

//----------

procedure SDspEnvADSR.noteOn;
begin
  FStage := env_Attack;
  FValue := 0;
end;

//----------

procedure SDspEnvADSR.noteOff;
begin
  FStage := env_Release;
end;

//----------

function SDspEnvADSR.process : Single;
var
  target,rate : single;
begin
  if FStage = env_Off then exit(0);
  if FStage = env_Finished then exit(0);
  if FStage = env_Sustain then exit(FValue);
  target := FStages[FStage].FTarget;
  rate   := FStages[FStage].FRate;
  FValue += (target-FValue) * rate;
  if  SAbs(target-FValue) <= env_Threshold then FStage+=1;
  result := FValue;
end;

//----------------------------------------------------------------------
end.

