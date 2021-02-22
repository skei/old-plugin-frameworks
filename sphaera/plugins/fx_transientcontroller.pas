{$include sphaera.inc}
unit fx_transientcontroller;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$define log:=ln}

uses
  s_plugin,
  s_types;

type

  myPlugin = class(SPlugin)
  private
    attack, sustain, vol : single;
    b1Env1, a0Env1, b1Env2, a0Env2, b1Env3, a0Env3 : single;
    tmpEnv1, tmpEnv2, tmpEnv3 : single;
  public
    procedure on_create; override;
    procedure on_stateChange(AState:LongInt); override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPLugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  s_const,
  s_parameter,
  s_util;

//----------

{
slider1:0<-100,100,1>Attack (%)
slider2:0<-100,100,1>Sustain (%)
slider3:0<-12,6,.1>Output (dB)
}

procedure myPlugin.on_create;
begin
  FName       := 'fx_transientcontroller';
  FAuthor     := '...';
  FProduct    := 'ported by skei';
  FVersion    := 0;
  FUniqueId   := S_MAGIC + $0000;
  FNumInputs  := 2;
  FNumOutputs := 2;
  SSetFlag(FFlags,spf_perSample);
  appendParameter( SParamFloat.create( 'attack',  0,-100,100,1 ) );
  appendParameter( SParamFloat.create( 'sustain', 0,-100,100,1 ) );
  appendParameter( SParamFloat.create( 'output',  0,-12, 6,  1 ) );
  tmpEnv1 := 0;
  tmpEnv2 := 0;
  tmpEnv3 := 0;
end;

//----------

{
@init
b1Env1 = -exp(-30 / srate );
a0Env1 = 1.0 + b1Env1;
b1Env2 = -exp(-1250 / srate );
a0Env2 = 1.0 + b1Env2;
b1Env3 = -exp(-3 / srate );
a0Env3 = 1.0 + b1Env3;
}

procedure myPlugin.on_stateChange(AState:LongInt);
begin
  case AState of
    sps_resume:
      begin
        b1Env1 := -Exp(-30 / _sampleRate );
        a0Env1 := 1.0 + b1Env1;
        b1Env2 := -exp(-1250 / _sampleRate );
        a0Env2 := 1.0 + b1Env2;
        b1Env3 := -exp(-3 / _sampleRate );
        a0Env3 := 1.0 + b1Env3;
      end;
  end;
end;

//----------

{
@slider
attack=slider1/100;
sustain=slider2/50;
vol = 2^(slider3/6);
}

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  v : Single;
begin
  v := AValue;
  case AIndex of
    0 : attack := v/100;
    1 : sustain := v/50;
    2 : vol := power(2,v/6);
  end;
end;

//----------

{
@sample
maxSpls = max(abs(spl0),abs(spl1));
env1 = sqrt(tmpEnv1 = a0Env1*maxSpls - b1Env1*tmpEnv1);
env2 = sqrt(tmpEnv2 = a0Env2*maxSpls - b1Env2*tmpEnv2);
env3 = sqrt(tmpEnv3 = a0Env3*maxSpls - b1Env3*tmpEnv3);
gain = exp(log(max(env2/env1,1))*attack) * exp( log( max(env3/env1,1))*sustain);
spl0 *= (gain *= vol);
spl1 *= gain;
}

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  maxSpls,env1,env2,env3,gain : single;
  //tmpEnv1,tmpEnv2,tmpEnv3 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  //...
  maxSpls := Max(Abs(spl0),Abs(spl1));

  tmpEnv1 := a0Env1*maxSpls - b1Env1*tmpEnv1;
  tmpEnv2 := a0Env2*maxSpls - b1Env2*tmpEnv2;
  tmpEnv3 := a0Env3*maxSpls - b1Env3*tmpEnv3;

  env1    :=  Sqrt(tmpEnv1);
  env2    :=  Sqrt(tmpEnv2);
  env3    :=  Sqrt(tmpEnv3);

  gain    :=  Exp(log(max(env2/env1,1))*attack) * exp( log( max(env3/env1,1))*sustain);
  gain    *= vol;
  spl0    *= gain;
  spl1    *= gain;

  //...
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

