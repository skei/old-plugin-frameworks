{$include kode.inc}
unit fx_applefilter;

// original author: lubomir i ivanov
// for axonlib

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  kode_const,
  kode_types,
  kode_plugin;

type

  myPlugin = class(KPlugin)
  private
    mX1l : Single;
    mX2l : Single;
    mY1l : Single;
    mY2l : Single;
    mX1r : Single;
    mX2r : Single;
    mY1r : Single;
    mY2r : Single;
    //
    mono : Single;
    mA0,mA1,mA2,{mB0,}mB1,mB2 : Single;
    outgain : single;
    //
    res,sx,cx : Single;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongWord; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  KPluginClass = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  _plugin_id,
  math,
  kode_flags,
  kode_parameter,
  kode_utils;

var
  txt_proc : array[0..1] of pchar = ( 'stereo','mono' );

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_applefilter';
  FAuthor := 'skei.audio';
  FProduct := FName;
  FVersion := 0;
  FUniqueId := KODE_MAGIC + fx_applefilter_id;
  KSetFlag(FFlags,kpf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
(*
  slider1: 0   <  0,   1, 1 {Stereo,Mono} > Processing
  slider2: 100 <  0, 100, 0.05 > Cutoff (Scale)
  slider3: 0   <-25,  25, 0.05 > Res (dB)
  slider4: 0   <-25,  25, 0.05 > Output (dB)
*)
  appendParameter( KParamText.create( 'process',  0,   2,  txt_proc) );
  appendParameter( KParamFloat.create('cutoff',   100, 0,  100) );
  appendParameter( KParamFloat.create('res',      0,  -25, 25) );
  appendParameter( KParamFloat.create('output',   0,  -25, 25) );

  //
  mX1l := 0;
  mX2l := 0;
  mY1l := 0;
  mY2l := 0;
  mX1r := 0;
  mX2r := 0;
  mY1r := 0;
  mY2r := 0;

  //

  mono := 0;
  outgain := 0;
end;

//----------

procedure myPlugin.on_destroy;
begin
end;

//----------

{
@slider
  mono = slider1;
  sx = 16+slider2*1.20103;
  cx = floor(exp(sx*log(1.059))*8.17742);
  res = slider3;
  outgain = 10^(slider4/40);

  //coeffcients
  cutoff = 2 * cx / srate;
  res = pow(10, 0.05 * -res);
  k = 0.5 * res * sin($pi * cutoff);
  c1 = 0.5 * (1 - k) / (1 + k);
  c2 = (0.5 + c1) * cos($pi * cutoff);
  c3 = (0.5 + c1 - c2) * 0.25;

  mA0 = 2 * c3;
  mA1 = 2 * 2 * c3;
  mA2 = 2 * c3;
  mB1 = 2 * -c2;
  mB2 = 2 * c1;
}

procedure myPlugin.on_parameterChange(AIndex:LongWord; AValue:Single);
var
  cutoff,k,c1,c2,c3 : single;
begin
  case AIndex of
    0: // slider1
      begin
        mono := AValue;
      end;
    1: // slider2
      begin
        sx := 16 + AValue * 1.20103;
        cx := trunc(exp(sx*{log2}ln(1.059))*8.17742); // ln,log2,log10,logn
      end;
    2: // slider3
      begin
        res := AValue;
      end;
    3: // slider4
      begin
        outgain := power(10,AValue/40); //  10^(AValue/40);
      end;
  end;

  { hmm.. this is re-calculated every time a parameter is modified }

  //coeffcients
  cutoff := 2 * cx / FSampleRate;// _sampleRate;
  res := power(10, 0.05 * -res);
  k := 0.5 * res * sin(KODE_PI * cutoff);
  c1 := 0.5 * (1 - k) / (1 + k);
  c2 := (0.5 + c1) * cos(KODE_PI * cutoff);
  c3 := (0.5 + c1 - c2) * 0.25;

  mA0 := 2 * c3;
  mA1 := 2 * 2 * c3;
  mA2 := 2 * c3;
  mB1 := 2 * -c2;
  mB2 := 2 * c1;

end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  inputl,outputl : single;
  inputr,outputr : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  if mono > 0.5 then
  begin
    { mono }
    inputl := (spl0+spl1)/2;
    outputl := mA0*inputl + mA1*mX1l + mA2*mX2l - mB1*mY1l - mB2*mY2l;
    mX2l := mX1l;
    mX1l := inputl;
    mY2l := mY1l;
    mY1l := outputl;
    spl0 := outputl*outgain;
    spl1 := spl0;
  end
  else
  begin
    { stereo }
    inputl := spl0;
    inputr := spl1;
    outputl := mA0*inputl + mA1*mX1l + mA2*mX2l - mB1*mY1l - mB2*mY2l;
    mX2l := mX1l;
    mX1l := inputl;
    mY2l := mY1l;
    mY1l := outputl;
    outputr := mA0*inputr + mA1*mX1r + mA2*mX2r - mB1*mY1r - mB2*mY2r;
    mX2r := mX1r;
    mX1r := inputr;
    mY2r := mY1r;
    mY1r := outputr;
    spl0 := outputl*outgain;
    spl1 := outputr*outgain;
  end;
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.


