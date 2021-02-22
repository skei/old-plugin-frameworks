{$include sphaera.inc}
unit fx_tilteq;

// (c) liteon

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_parameter;

type

  myPlugin = class(SPlugin)
  private
    //gain : Single;
    amp, sr3 : Single;
    outgain, lgain, hgain : Single;
    mono : LongInt;
    lp_out : Single;
    lp_out_r : Single;
    a0, b1 : Single;
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
  //math,
  s_util;

var
  str_proc : array[0..1] of PChar = ( 'stereo','mono');

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_tilteq';
  FAuthor := 'lubomir i. ivanov';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0107;
  //FFlags += [ zpf_perSample ];
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  appendParameter( SParamText.create(  'processing',   0,   2,  str_proc ) );
  appendParameter( SParamFloat.create( 'center freq',  50,  0,  100, 0.25 ) );
  appendParameter( SParamFloat.create( 'tilt (lo/hi)', 0,  -6,  6 ) );
  appendParameter( SParamFloat.create( 'gain',         0,  -25, 25 ) );
  //
  amp := 6/Ln(2);//log(2);
  a0 := 0;
  b1 := 0;
  lp_out := 0;
  lp_out_r := 0;
end;

//----------

procedure myPlugin.on_stateChange(AState:LongInt);
begin
  case AState of
    sps_Resume:
      begin
        sr3 := 3*_sampleRate;
      end;
  end;
end;

//----------

// Function LogN(Base, X: Extended): Extended; // ???

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  //p : ZParameter;
  v : Single;
  sx,f0,omega,n,gain,gfactor,g1,g2 : Single;
begin
  //p := FParameters[AIndex];
  v := AValue;//p.from01(AValue);
  case AIndex of
    0: // slider1
      begin
        mono := trunc(v);
      end;
    1: // slider2
      begin
        sx := 16+v*1.20103;
        //float f0 = floor(exp(sx*log(1.059))*8.17742);
        f0 := trunc(Exp(sx*ln(1.059))*8.17742); // log(x), log2? orig had Logf
        omega := 2*S_PI*f0;
        n := 1/(sr3 + omega); // sr3 = 0 in constructor...
        a0 := 2*omega*n;
        b1 := (sr3 - omega)*n;
      end;
    2: // slider3
      begin
        gain := v;
        gfactor := 4;
        if (gain>0) then
        begin
          g1 := -gfactor*gain;
          g2 := gain;
        end
        else
        begin
          g1 := -gain;
          g2 := gfactor*gain;
        end;
        lgain := Exp(g1/amp)-1;
        hgain := Exp(g2/amp)-1;
      end;
    3: // slider4
      begin
        outgain := Exp(v/amp);
      end;
  end;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs: PPSingle);
var
  spl0,spl1 : single;
  inp,outp : single;
  inp_r,outp_r : single;
begin
  spl0 := AInputs[0]^ ;//+ DENORM;
  spl1 := AInputs[1]^ ;//+ DENORM;
  if (mono=1) then
  begin // mono
    inp    := (spl0+spl1)*0.5;
    lp_out := a0*inp + b1*lp_out;
    outp   := inp + lgain*lp_out + hgain*(inp - lp_out);
    spl0   := outp*outgain;
    spl1   := outp*outgain;
  end
  else
  begin // stereo
    inp      := spl0;
    lp_out   := a0*inp + b1*lp_out;
    outp     := inp + lgain*lp_out + hgain*(inp - lp_out);
    spl0     := outp*outgain;
    inp_r    := spl1;
    lp_out_r := a0*inp_r + b1*lp_out_r;
    outp_r   := inp_r + lgain*lp_out_r + hgain*(inp_r - lp_out_r);
    spl1     := outp_r*outgain;
  end;

  AOutputs[0]^ := spl0 ;//- DENORM;
  AOutputs[1]^ := spl1 ;//- DENORM;

end;

//----------------------------------------------------------------------
end.

