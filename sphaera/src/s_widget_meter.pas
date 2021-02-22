{$include sphaera.inc}
unit s_widget_meter;

{
  drag mouse up/down to change value (0..1)
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  s_color,
  s_rect,
  s_widget;

type

  SWidgetMeter = class(SWidget)
  protected
  private
    FBackColor  : SColor;
    FMeterColor : SColor;
    FLevel      : Single;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
    property    level : Single write FLevel;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  s_window;

//----------

constructor SWidgetMeter.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText := 'SWidgetPanel';
  FBackColor  := s_dark_green;
  FMeterColor := s_light_green;
  FLevel      := 0;
end;

//----------

destructor SWidgetMeter.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetMeter.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  r:SRect;
  v:single;
begin
  r := FRect;
  //r.crop(ARect);
  //if not r.empty then
  //begin
    ACanvas.setColor(FBackColor);
    ACanvas.rectangle(r.x,r.y,r.w,r.h);
    ACanvas.fill;
  //end;
  v := (FLevel * r.h);
  ACanvas.setColor(FMeterColor);
  ACanvas.rectangle( r.x, r.y2 - v, r.w, v );
  ACanvas.fill;
  inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

