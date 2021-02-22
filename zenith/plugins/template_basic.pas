unit template_basic;
{$include zenith.inc}

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
  public
    procedure on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPZSample); override;
  end;

//----------

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
  FName       := 'fx_template';
  FAuthor     := 'zenith';
  FProduct    := 'powered by zenith';
  FVersion    := 0;
  FUniqueId   := Z_MAGIC + $0000;
  FNumInputs  := 2;
  FNumOutputs := 2;
  ZSetFlag(FFlags,zpf_perSample+zpf_hasEditor);
  //appendParameter( ZParameter.create( 'param1', 1 ) );
end;

//----------------------------------------------------------------------

procedure myInstance.on_create;
begin
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : Single;
begin
  p := FParameters[AIndex];
  v := p.from01(AValue);
  case AIndex of
    0 : v := v;
  end;
end;

//----------

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  //...
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

