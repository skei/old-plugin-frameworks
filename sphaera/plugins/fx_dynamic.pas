{$include sphaera.inc}
unit fx_dynamic;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin;//,
  //s_dsp_compress;

type

  myPlugin = class(SPlugin)
  private
    FPreGain    : Single;
    FPostGain   : Single;
    FThreshold  : Single;
    FRatio      : Single;
    FAttack     : Single;
    FRelease    : SIngle;
    //
    FGain       : Single;
  public
    procedure   on_create; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  //s_math,
  s_parameter,
  s_util;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_dynamic';
  FAuthor := 'tor-helge skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  //
  appendParameter(SParamFloat.create( 'Pre Gain (db)',  0,   -24, 24   )); // db
  appendParameter(SParamFloat.create( 'Post Gain (db)', 0,   -24, 24   )); // db
  appendParameter(SParamFloat.create( 'Threshold (db)', 0,   -24, 0    )); // db
  appendParameter(SParamFloat.create( 'Ratio',          3,    1,  10   ));
  appendParameter(SParamFloat.create( 'Attack (db/s)',  2000, 1,  3000 )); // db/sec
  appendParameter(SParamFloat.create( 'Release (db/s)', 300,  1,  3000 )); // db/sec
  //
  FGain := 1;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
//var
//  v : Single;
begin
  case AIndex of
    0:  FPreGain   := power(2,AValue/6);
    1:  FPostGain  := power(2,AValue/6);
    2:  FThreshold := power(2,AValue/6);
    3:  FRatio     := 1 / AValue;
    4:  FAttack    := 1 / power(2,(AValue/_sampleRate)/6);
    5:  FRelease   := power(2,(AValue/_sampleRate)/6);
  end;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  input : Single;
  target : Single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  input := FPreGain * max(abs(spl0),abs(spl1));
  if input > FThreshold then
  begin
    target := FThreshold + ((input-FThreshold)*FRatio);
  end
  else target := 1;
  if FGain > target then FGain*=FAttack else  FGain*=FRelease;
  spl0 *= (FGain*FPostGain);
  spl1 *= (FGain*FPostGain);
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

