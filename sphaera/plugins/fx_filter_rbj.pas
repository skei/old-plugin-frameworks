{$include sphaera.inc}
unit fx_filter_rbj;

{
  had to mess with the filter class..
  todo: fix-back...
  - fix the filter class as it should hav been,
  - fix this to match.

}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_parameter,
  s_dsp_rbj;

type

  myPlugin = class(SPlugin)
  private
    FType : LongInt;
    FFreq : Single;
    FQ    : Single;
    FGain : Single;
    FBW   : Boolean;
    FFilter0  : SDspRBJ;
    FFilter1  : SDspRBJ;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPLugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_util;

var
  txt_type : array[0..8] of PChar = ('lowpass','highpass','bandpass csg','bandpass czpg','notch','allpass','peaking','lowshelf','hishelf');
  txt_bw : array[0..1] of PChar = ('q','bandwidth');

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_filter_rbj';
  FAuthor := 'th.skei';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0102;
  //FFlags += [ spf_perSample ];
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  appendParameter(SParamText.create( 'type', 0, 9, txt_type     ));
  appendParameter(SParamFloat.create('freq', 1000, 20, 20000 ));
  appendParameter(SParamFloat.create('q',    0.5, 0.001, 0.999 ));
  appendParameter(SParamFloat.create('gain', 0, -24, 24 ));
  appendParameter(SParamText.create( 'bw',   0, 2, txt_bw ));
  //
  FType := sft_lowpass;
  FFreq := 1000;
  FQ    := 0.75;
  FGain := 0;
  FBW   := false;
  FFilter0 := SDspRBJ.create;
  FFilter1 := SDspRBJ.create;
end;

procedure myPlugin.on_destroy;
begin
  FFilter0.Destroy;
  FFilter1.Destroy;
end;

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  //p : ZParameter;
  v : Single;
  sr : Single;
begin
  //p := FParameters[AIndex];
  v := AValue;//p.from01(AValue);
  case AIndex of
    0: FType := trunc(v);
    1: FFreq := v;
    2: FQ    := v;
    3: FGain := v;
    4: if v > 0.5 then FBW := true else FBW := false;
  end;
  // ouch, will cause a burst of recalcs in init..
  sr := 44100;//getSampleRate;
  FFilter0.CalcFilterCoeffs(sr,FType,FFreq,FQ,FGain,FBW);
  FFilter1.CalcFilterCoeffs(sr,FType,FFreq,FQ,FGain,FBW);
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  spl0 := FFilter0.Process(spl0);
  spl1 := FFilter1.Process(spl1);
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

