{$include sphaera.inc}
unit s_widget_panel;

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

  SWidgetPanel = class(SWidget)
  protected
    FColor    : SColor;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
    property    color : SColor read FColor write FColor;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  s_window;

//----------

constructor SWidgetPanel.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText := 'SWidgetPanel';
  FColor := s_grey;
end;

//----------

destructor SWidgetPanel.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetPanel.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  r:SRect;
begin
  r := FRect;
  r.crop(ARect);
  ACanvas.setColor(FColor);
  ACanvas.rectangle(r.x,r.y,r.w,r.h);
  ACanvas.fill;
  ACanvas.setColor(s_black);
  ACanvas.setLineWidth(1);
  inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

