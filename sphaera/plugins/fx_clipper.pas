{$include sphaera.inc}
unit fx_clipper;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_parameter,
  s_plugin,
  s_types,
  s_widget_knob,
  s_window;

type

  myPlugin = class(SPlugin)
  private
    FLimit    : Single;
    FGain     : Single;
  public
    procedure on_create; override;
    //procedure on_transportChange(AState:LongInt); override;
    //procedure on_stateChange(AState: LongInt); override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPSingle); override;
    procedure on_openEditor; override;
  end;

  //----------

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_math,
  s_rect,
  s_util,
  s_widget;

//----------------------------------------------------------------------

procedure myPlugin.on_create;
begin
  FName       := 'fx_clipper';
  FAuthor     := 'th.skei';
  FVersion    := 0;
  FUniqueId   := S_MAGIC + $0101;
  FNumInputs  := 2;
  FNumOutputs := 2;
  FEditorRect := rect(200,100);
  SSetFlag(FFlags,spf_perSample + spf_hasEditor + spf_autoSync);
  appendParameter( SParameter.create( {FBufCanDo}'level',1) );
  FLimit      := 1;
  FGain       := 1;
end;

//----------

{procedure myPlugin.on_transportChange(AState:LongInt);
begin
  if SHasFlag(AState,spt_changed) then STrace('transport changed');
end;}

{procedure myPlugin.on_stateChange(AState: LongInt);
begin
  case AState of
    sps_suspend: STrace('suspend');
    sps_resume: STrace('resume');
    sps_start: STrace('start');
    sps_stop: STrace('stop');
    sps_open: STrace('open');
    sps_close: STrace('close');
    sps_sampleRate: STrace('samplerate');
    sps_blockSize: STrace('blocksize');
    sps_bypass : STrace('bypass');
    sps_bypassOff : STrace('bypassoff');
  end;
end;}

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  v : Single;
begin
  //STrace('parameter change');
  case AIndex of
    0:
      begin
        v := AValue;
        FLimit := v*v*v;
        if v < 0.00001 then v := 0.00001;
        FGain := 1 / v;
      end;
  end; // case
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  AOutputs[0]^ := SClamp(spl0,-FLimit,FLimit) * FGain;
  AOutputs[1]^ := SClamp(spl1,-FLimit,FLimit) * FGain;
end;

//----------

procedure myPlugin.on_openEditor;
var
  win : SWindow;
  wdg : SWidget;
begin
  win := _editor.getWindow;
  wdg := SWidgetKnob.create(FEditorRect,swa_fillTop);
  _editor.connect( FParameters[0], wdg );
  win.appendWidget(wdg);
end;

//----------------------------------------------------------------------
end.

