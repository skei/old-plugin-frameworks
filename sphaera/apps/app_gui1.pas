{$include sphaera.inc}
unit app_gui1;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_application,
  //s_console,
  s_debug,
  s_surface,
  s_widget_button,
  s_widget_image,
  s_widget_image_knob,
  s_widget_knob,
  //s_widget_meter,
  s_widget_panel,
  s_widget_slider,
  s_widget_text,
  s_widget_value,
  //s_widget_wave,
  s_window;

type

  myApp = class(SApplication)
  private
    FWindow : SWindow;
    img_skei : SSurface;
    img_knob : SSurface;
  public
    procedure on_create; override;
    procedure on_destroy; override;
  end;

  SPHAERA_APPLICATION = myApp;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_basepath,
  s_color,
  s_const,
  s_rect,
  s_string,
  s_widget;

  {$include knob1.inc}

//----------

procedure myApp.on_create;
var
  panel : SWidgetPanel;
  wdg   : SWidget;
  path : array[0..256] of Char;
  //filepath : array[0..256] of Char;
begin

  //STrace('create');
  { get exe/dll path }
  //path[0] := #0;
  //SGetBasePath(path);
  { load png from disk }
  //filepath[0] := #0;
  //SStrcat(filepath,path);
  //SStrcat(filepath,'skei.png');
  //img_skei := SSurface.create(filepath);

  { load png from disk }
  img_skei := SSurface.create( SFromBasePath(path,'skei.png') );
  { load png from memory }
  img_knob := SSurface.create(@knob1,knob1_size);
  { window }
  FWindow := SWindow.create(rect(640,480));
  FWindow.setTitle('Sphaera :: app_test1');
  { panel }
  panel := SWidgetPanel.create(rect(0),swa_fillClient);
  FWindow.appendWidget(panel);
  //
  { image }
  wdg := SWidgetImage.create(rect(120,10,256,256),swa_fillRight{,img_skei});
  (wdg as SWidgetImage).image := img_skei;
  panel.appendWidget(wdg);
  { button 1 - action }
  wdg := SWidgetButton.create(rect(10,10,45,20),swa_none);
  FWindow{panel}.appendWidget(wdg);
  { button 2 - toggle }
  wdg := SWidgetButton.create(rect(65,10,45,20),swa_none);
  with (wdg as SWidgetButton) do
  begin
    mode := scm_toggle;
    onColor := color(1,1,0,0.75);
    offColor := s_dark_yellow;
  end;
  panel.appendWidget(wdg);
  { slider }
  wdg := SWidgetSlider.create(rect(10,40,100,20),swa_none);
  panel.appendWidget(wdg);
  { knob }
  wdg := SWidgetKnob.create(rect(10,70,100,30),swa_none);
  panel.appendWidget(wdg);
  { image strip }
  wdg := SWidgetImageKnob.create(rect(10,110,32,32),swa_none);
  (wdg as SWidgetImageKnob).setup(img_knob,65,sid_vertical);
  panel.appendWidget(wdg);
  { text }
  wdg := SWidgetText.create(rect(10,150,100,20),swa_none);
  with (wdg as SWidgetText) do
  begin
    text := 'tekst.. blabla';
    drawBack := True;
    _cursor := smc_Wait;
  end;
  panel.appendWidget(wdg);
  { value }
  wdg := SWidgetValue.create(rect(10,180,100,20),swa_none);
  panel.appendWidget(wdg);
  //
  FWindow.show;
  FWindow.redraw;
  FWindow.eventLoop;
end;

//----------

procedure myApp.on_destroy;
begin
  FWindow.hide;
  FWindow.destroy;
  img_skei.destroy;
  img_knob.destroy;
end;

//----------------------------------------------------------------------
end.

