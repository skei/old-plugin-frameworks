{$include sphaera.inc}
unit fx_resonantlowpass;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin;

type

  myPlugin = class(SPlugin)
  private
    cut_lp, res_lp, fb_lp : Single;
    n3,n4, rn3,rn4 : Single;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_parameter,
  s_util;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_resonantfilter';
  FAuthor := 'js';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  appendParameter( SParamFloat.create('freq (hz)',1000,20,20000) );
  appendParameter( SParamFloat.create('res',0.8,0,1) );
  //
  cut_lp := 0;
  res_lp := 0;
  fb_lp := 0;
  n3 := 0;
  n4 := 0;
  rn3 := 0;
  rn4 := 0;
end;

//----------

procedure myPlugin.on_destroy;
begin
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  case AIndex of
    0 : cut_lp := AValue*2 / _sampleRate;
    1 : res_lp := AValue;
  end;
  fb_lp := res_lp + res_lp / (1-cut_lp);
  //res_lp!=slider2 ? n3=n4=rn3=rn4=0;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  //

  n3 := n3 + cut_lp * (spl0-n3+fb_lp*(n3-n4));
  n4 := n4 + cut_lp * (n3-n4);
  spl0 := n4;

  rn3 := rn3+cut_lp*(spl1-rn3+fb_lp*(rn3-rn4));
  rn4 := rn4+cut_lp*(rn3-rn4);
  spl1 := rn4;

  //
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.


{
  // This effect Copyright (C) 2004 and later Cockos Incorporated
  // License: GPL - http://www.gnu.org/licenses/gpl.html

  desc: resonant lowpass filter (low quality)

  slider1:1000<20,20000>frequency (Hz)
  slider2:0.8<0,1>resonance (0..1)

  @slider
    cut_lp = slider1*2 / (srate);
    //res_lp!=slider2 ? n3=n4=rn3=rn4=0;
    res_lp=slider2;
    fb_lp=res_lp+res_lp/(1-cut_lp);

  @sample
    n3=n3+cut_lp*(spl0-n3+fb_lp*(n3-n4));
    n4=n4+cut_lp*(n3-n4);
    spl0=n4;
    rn3=rn3+cut_lp*(spl1-rn3+fb_lp*(rn3-rn4));
    rn4=rn4+cut_lp*(rn3-rn4);
    spl1=rn4;
}



