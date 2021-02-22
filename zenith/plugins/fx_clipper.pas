unit fx_clipper;
{$include zenith.inc}

// simplistic example..

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

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
    par_limit   : ZParameter;
  public
    procedure   on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    limit       : Single;
    gain        : Single;
  public
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

procedure myDescriptor.on_create;
begin
  FName := 'fx_bricklimit';
  FAuthor := 'th.skei';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := Z_MAGIC + $0101;
  //FFlags += [ zpf_perSample ];
  ZSetFlag(FFlags,zpf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  par_limit := ZParameter.create( 'level', 1 );
  appendParameter(par_limit);
end;

//----------------------------------------------------------------------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : Single;
begin
  if AIndex = 0 then
  begin
    p := FParameters[AIndex];
    v := p.from01(AValue);
    limit := v*v*v; // to make it a little logarithmic-like..
    if v < 0.00001 then v := 0.00001;
    {if MAutoGain then} gain := 1/limit;
  end;
end;

//----------

{
  primitive clipping with gain compensation..
}

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  if spl0 >  limit then spl0 :=  limit;
  if spl0 < -limit then spl0 := -limit;
  if spl1 >  limit then spl1 :=  limit;
  if spl1 < -limit then spl1 := -limit;
  AOutputs[0]^ := spl0*gain;
  AOutputs[1]^ := spl1*gain;
end;

//----------------------------------------------------------------------
end.

