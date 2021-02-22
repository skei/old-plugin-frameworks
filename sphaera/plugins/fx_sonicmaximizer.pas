{$include sphaera.inc}
unit fx_sonicmaximizer;

// BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
// ported by tor-helge skei

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses

  s_const,
  s_parameter,
  s_plugin,
  s_types;

const

  cDenorm   = 10e-30; // ???
  c_ampdB   = 8.65617025;
  freqHP    = 2243.2;
  freqLP    = 224.32;
  freqHP_p2 = -S_PI2 * freqHP;
  freqLP_p2 = -S_PI2 * freqLP;

type

  myPlugin = class(SPlugin)
  private
    slider1,slider2,slider3 : single;
    band1,band2,band3,amp   : Single;
    tmplLP,tmprLP : single;
    tmplHP,tmprHP : single;
    xLP,xHP       : Single;
    a0LP,a0HP     : Single;
    b1LP,b1HP     : Single;
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPSingle); override;
    procedure on_stateChange(AState:LongInt); override;
  end;

  SPHAERA_PLUGIN = myPLugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //s_random,
  s_util;

procedure myPlugin.on_create;
begin
  FName     := 'fx_sonicmaximizer';
  FAuthor   := 'dominique guichaoua';
  FProduct  := 'ported by skei';
  FVersion  := 0;
  FUniqueId := S_MAGIC + $0105;
  SSetFlag(FFlags,spf_perSample);
  appendParameter( SParamFloat.create('Low Cont', 0,  0,  10, 0.1 ) );
  appendParameter( SParamFloat.create('Process',  0,  0,  10, 0.1 ) );
  appendParameter( SParamFloat.create('Output',   0, -30, 0,  0.1 ) );
  //
  slider1 := 0;
  slider2 := 0;
  slider3 := 0;
  band1   := 0;
  band2   := 1;
  band3   := 0;
  tmplLP  := 0;
  tmprLP  := 0;
  tmplHP  := 0;
  tmprHP  := 0;
end;

//----------

procedure myPlugin.on_stateChange(AState:LongInt);
begin
  case AState of
    sps_resume:
      begin
        xHP  := exp(freqHP_p2/_sampleRate);
        a0HP := 1.0 - xHP;
        b1HP := -xHP;
        xLP  := exp(freqLP_p2/_sampleRate);
        a0LP := 1.0 - xLP;
        b1LP := -xLP;
      end;
  end;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var i : longint;
    //p : SParameter;
    v : single;
begin
  i := AIndex;
  //p := FParameters[i];
  v := AValue;//p.from01(AValue);
  case i of
    0 :
      begin
        slider1 := v;
        band1 := exp( (slider1+3) / c_ampdB );
      end;
    1 :
      begin
        slider2 := v;
        band3 := exp( (slider2+3) / c_ampdB );
      end;
    2 :
      begin
        slider3 := v;
        amp := exp( slider3 / c_ampdB );
      end;
  end;
  //band2 := 1; // exp(0/c_ampdB);
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  s0,s1,sp0,sp1,sp2,sp3,sp4,sp5 : single;
begin
  s0 := AInputs[0]^;
  s1 := AInputs[1]^;
  tmplLP := a0LP*s0 - b1LP*tmplLP + cDenorm;
  tmprLP := a0LP*s1 - b1LP*tmprLP + cDenorm;
  sp0 := tmplLP;
  sp1 := tmprLP;
  tmplHP := a0HP*s0 - b1HP*tmplHP + cDenorm;
  tmprHP := a0HP*s1 - b1HP*tmprHP + cDenorm;
  sp4 := s0 - tmplHP;
  sp5 := s1 - tmprHP;
  sp2 := s0 - sp0 - sp4;
  sp3 := s1 - sp1 - sp5;
  AOutputs[0]^ := (sp0 * band1 + sp2 {* band2} + sp4 * band3) * amp;
  AOutputs[1]^ := (sp1 * band1 + sp3 {* band2} + sp5 * band3) * amp;
end;

//----------------------------------------------------------------------
end.

(*
  http://forum.cockos.com/showthread.php?t=91439

  //(C) 20011, Dominique Guichaoua.
  //BBE Sonic Maximizer modelisation
  desc:BBE Sonic Maximizer
  slider1:0<0,10,.1>Low Contour (dB)
  slider2:0<0,10,.1>Process (dB)
  slider3:0<-30,0,.1>Output Level (dB)
  @init
    cDenorm=10^-30;
    c_ampdB = 8.65617025;
    freqHP = 2243.2;
    xHP = exp(-2.0*$pi*freqHP/srate);
    a0HP = 1.0-xHP;
    b1HP = -xHP;
    freqLP = 224.32;
    xLP = exp(-2.0*$pi*freqLP/srate);
    a0LP = 1.0-xLP;
    b1LP = -xLP;
  @slider
    band1 = exp((slider1+3)/c_ampdB);
    band2 = exp(0/c_ampdB);
    band3 = exp((slider2+3)/c_ampdB);
    amp=exp(slider3/c_ampdB);
  @sample
    s0 = spl0;
    s1 = spl1;
    sp0 = (tmplLP = a0LP*s0 - b1LP*tmplLP + cDenorm);
    sp1 = (tmprLP = a0LP*s1 - b1LP*tmprLP + cDenorm);
    sp4 = s0 - (tmplHP = a0HP*s0 - b1HP*tmplHP + cDenorm);
    sp5 = s1 - (tmprHP = a0HP*s1 - b1HP*tmprHP + cDenorm);
    sp2 = s0 - sp0 - sp4;
    sp3 = s1 - sp1 - sp5;
    spl0 = (sp0 * band1 + sp2 * band2 + sp4 * band3)*amp;
    spl1 = (sp1 * band1 + sp3 * band2 + sp5 * band3)*amp;
*)

//----------

(*

// new version

// (C) 20011, Dominique Guichaoua.

//BBE Sonic Maximizer modelisation

desc:BBE Sonic Maximizer
slider1:0<0,10,.1>Low Contour (dB)
slider2:0<0,10,.1>Process (dB)
slider3:0<-30,0,.1>Output Level (dB)

@init
  c_ampdB = 8.65617025;
  //Electronic Components
  R=21.5e3;
  C1=0.0000000033;
  C2=0.000000033;
  Te=1/srate;
  //Input Gain
  Gin=1;
  //Band Pass Gain
  GBP=1;
  //Low Pass initial Gain (dB)
  GLP0=3;
  //High Pass initial Gain(dB)
  GHP0=3;
  //High Pass Coefficient
  a0HP = Te/(R*C1);
  //Low  Pass Coefficient
  a0LP = Te/(R*C2);
  //Peek Detector Coefficients
  a0cv = 0.0003;
  b0cv = 1 - a0cv;
  //Peek Detector close loop gain
  cv_k = 8;

//Front Panel Controls
@slider
  //Low Pass Gain
  GLP = exp((slider1+GLP0)/c_ampdB);
  //High Pass Gain
  GHP = exp((slider2+GHP0)/c_ampdB);
  //Ouput Gain
  amp=exp(slider3/c_ampdB);

@sample
  //Input Buffer
  s0=spl0*Gin;
  s1=spl1*Gin;
  //Peak Detector
  cv0 = 1 - abs(s0)*cv_k;
  cv1 = 1 - abs(s1)*cv_k;
  cv_rc0 = a0cv*cv0 + b0cv*cv_rc0;
  cv_rc1 = a0cv*cv1 + b0cv*cv_rc1;
  //Pseudo State-Variable Filter (Euler)
  BP0 = a0HP*HP0 + BP0;
  BP1 = a0HP*HP1 + BP1;
  LP0 = a0LP*BP0 + LP0;
  LP1 = a0LP*BP1 + LP1;
  HP0 = s0 - 1.5*BP0 - LP0;
  HP1 = s1 - 1.5*BP1 - LP1;
  //Summing Output Buffer
  spl0 = (LP0 * GLP + BP0 * GBP + HP0 * GHP * cv_rc0)*amp;
  spl1 = (LP1 * GLP + BP1 * GBP + HP1 * GHP * cv_rc1)*amp;
*)

