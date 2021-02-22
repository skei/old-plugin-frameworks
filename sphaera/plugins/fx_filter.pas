{$include sphaera.inc}
unit fx_filter;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_dsp_rc;

type

  myPlugin = class(SPlugin)
  private
    FLowpass    : Boolean;
    //cur0,cur1   : Single;
    //FWeight     : Single;
    rc0,rc1 : SDspRC;
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
  FName := 'fx_filter';
  FAuthor := 'th.skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  appendParameter( SParameter.create('tone',0.5) );
  //
  FLowpass  := True;
  //cur0      := 0;
  //cur1      := 0;
  //FWeight   := 0;
  rc0 := SDspRC.create;
  rc1 := SDspRC.create;
end;

//----------

procedure myPlugin.on_destroy;
begin
  rc0.destroy;
  rc1.destroy;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  v,vv : Single;
begin
  if AIndex = 0 then
  begin
    v := AValue;
    if v < 0.5 then
    begin
      FLowpass := True;
      v *= 2;
    end
    else
    begin
      FLowpass := False;
      v := (v-0.5)*2;
    end;
    //FWeight := v*v*v*v;
    vv := v*v*v*v;
    rc0.speed := vv;
    rc1.speed := vv;
  end; // 0
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  //cur0 += (spl0-cur0) * FWeight;
  //cur1 += (spl1-cur1) * FWeight;
  if FLowpass then
  begin
    AOutputs[0]^ := rc0.process(spl0);
    AOutputs[1]^ := rc1.process(spl0);
  end
  else
  begin
    AOutputs[0]^ := spl0 - rc0.process(spl0);
    AOutputs[1]^ := spl1 - rc1.process(spl0);
  end;
end;

//----------------------------------------------------------------------
end.

