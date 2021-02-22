{$include sphaera.inc}
unit fx_vumeter;

{$define S_DECAY_SPEED := 0.0001}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  //s_editor,
  s_plugin,
  s_types,
  s_widget_meter;

type

  myPlugin = class(SPlugin)
  private
    w_meter   : SWidgetMeter;
    FLevel    : Single;
  public
    procedure on_create; override;
    procedure on_processSample(AInputs,AOutputs:PPSingle); override;
    procedure on_openEditor; override;
    procedure on_closeEditor; override;
    procedure on_idleEditor; override;
  end;

  //----------

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //s_color,
  s_const,
  s_math,
  s_rect,
  s_util,
  s_window;

//----------

procedure myPlugin.on_create;
begin
  FName       := 'fx_vumeter';
  FAuthor     := 'th.skei';
  FVersion    := 0;
  FUniqueId   := S_MAGIC + $0101;
  FNumInputs  := 2;
  FNumOutputs := 2;
  FEditorRect := rect(260,190);
  SSetFlag(FFlags,spf_perSample + spf_hasEditor);
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  v : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  v := SAbs(spl0);
  if v > FLevel then FLevel := v;
  FLevel -= (FLevel*S_DECAY_SPEED);

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------

procedure myPlugin.on_openEditor;
var
  win   : SWindow;
begin
  win     := _editor.getWindow;
  w_meter := SWidgetMeter.create( rect(10), swa_client );
  {back}win.appendWidget(w_meter);
end;

//----------

procedure myPlugin.on_closeEditor;
begin
end;

//----------

procedure myPlugin.on_idleEditor;
begin

  w_meter.level := FLevel;
  _editor.getWindow.redraw(w_meter);
end;

//----------------------------------------------------------------------
end.

