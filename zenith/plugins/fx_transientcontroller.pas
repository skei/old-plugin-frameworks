unit fx_transientcontroller;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$define log:=ln}

uses
  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter;

type

  myDescriptor = class(ZDescriptor)
  private
  public
    procedure on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    attack, sustain, vol : single;
    b1Env1, a0Env1, b1Env2, a0Env2, b1Env3, a0Env3 : single;
    tmpEnv1, tmpEnv2, tmpEnv3 : single;
  public
    procedure on_create; override;
    procedure on_stateChange(AState:LongWord); override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPZSample); override;
  end;

//----------

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  z_util;

//----------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

//----------

{
slider1:0<-100,100,1>Attack (%)
slider2:0<-100,100,1>Sustain (%)
slider3:0<-12,6,.1>Output (dB)
}

procedure myDescriptor.on_create;
begin
  FName       := 'fx_transientcontroller';
  FAuthor     := 'michael gruhn';
  FProduct    := 'powered by zenith';
  FVersion    := 0;
  FUniqueId   := Z_MAGIC + $0000;
  FNumInputs  := 2;
  FNumOutputs := 2;
  ZSetFlag(FFlags,zpf_perSample);
  appendParameter( ZParamFloat.create( 'attack',  0,-100,100,1 ) );
  appendParameter( ZParamFloat.create( 'sustain', 0,-100,100,1 ) );
  appendParameter( ZParamFloat.create( 'output',  0,-12, 6,  1 ) );
end;

//----------------------------------------------------------------------

procedure myInstance.on_create;
begin
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

procedure myInstance.on_stateChange(AState:LongWord);
begin
  case AState of
    zps_resume:
      begin
        b1Env1 := -Exp(-30 / FSampleRate );
        a0Env1 := 1.0 + b1Env1;
        b1Env2 := -exp(-1250 / FSampleRate );
        a0Env2 := 1.0 + b1Env2;
        b1Env3 := -exp(-3 / FSampleRate );
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

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : Single;
begin
  p := FParameters[AIndex];
  v := p.from01(AValue);
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

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
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

