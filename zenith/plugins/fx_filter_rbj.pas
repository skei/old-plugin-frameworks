unit fx_filter_rbj;
{$include zenith.inc}

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
  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter,
  z_filter_rbj;

type

  myDescriptor = class(ZDescriptor)
  public
    procedure   on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    FType : LongInt;
    FFreq : Single;
    FQ    : Single;
    FGain : Single;
    FBW   : Boolean;
    FFilter0  : ZFilterRBJ;
    FFilter1  : ZFilterRBJ;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPZSample); override;
  end;

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_util;

//----------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

//----------

var
  txt_type : array[0..8] of PChar = ('lowpass','highpass','bandpass csg','bandpass czpg','notch','allpass','peaking','lowshelf','hishelf');
  txt_bw : array[0..1] of PChar = ('q','bandwidth');

  //----------

procedure myDescriptor.on_create;
begin
  FName := 'fx_filter_rbj';
  FAuthor := 'th.skei';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := Z_MAGIC + $0102;
  //FFlags += [ zpf_perSample ];
  ZSetFlag(FFlags,zpf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  appendParameter(ZParamText.create( 'type', 0, 9, txt_type     ));
  appendParameter(ZParamFloat.create('freq', 1000, 20, 20000 ));
  appendParameter(ZParamFloat.create('q',    0.5, 0.001, 0.999 ));
  appendParameter(ZParamFloat.create('gain', 0, -24, 24 ));
  appendParameter(ZParamText.create( 'bw',   0, 2, txt_bw ));
end;

//----------

{function myPlugin.on_createInstance(AHost:ZBaseHost) : ZBaseInstance;
begin
  result := myInstance.Create(AHost,self);
end;}

//----------------------------------------------------------------------

procedure myInstance.on_create;
begin
  FType := z_rbj_lowpass;
  FFreq := 1000;
  FQ    := 0.75;
  FGain := 0;
  FBW   := false;
  FFilter0 := ZFilterRBJ.create;
  FFilter1 := ZFilterRBJ.create;
end;

procedure myInstance.on_destroy;
begin
  FFilter0.Destroy;
  FFilter1.Destroy;
end;

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : Single;
  sr : Single;
begin
  p := FParameters[AIndex];
  v := p.from01(AValue);
  case AIndex of
    0: FType := trunc(v);
    1: FFreq := v;
    2: FQ := v;
    3: FGain := v;
    4: if v > 0.5 then FBW := true else FBW := false;
  end;
  // ouch, will cause a burst of recalcs in init..
  sr := 44100;//getSampleRate;
  FFilter0.CalcFilterCoeffs(sr,FType,FFreq,FQ,FGain,FBW);
  FFilter1.CalcFilterCoeffs(sr,FType,FFreq,FQ,FGain,FBW);
end;

//----------

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
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
