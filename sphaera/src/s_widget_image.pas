{$include sphaera.inc}
unit s_widget_image;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  //s_color,
  s_rect,
  s_surface,
  s_widget;

type

  SWidgetImage = class(SWidget)
  protected
    FImage : SSurface;
  public
    constructor create(ARect:SRect; AAlignment:Longword; AImage:SSurface=nil);
    destructor  destroy; override;
  public
    procedure on_paint(ACanvas:SCanvas; ARect:SRect); override;
  public
    property  image : SSurface read FImage write FImage;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  s_util,
//  s_window;

//----------

constructor SWidgetImage.create(ARect:SRect; AAlignment:Longword; AImage:SSurface);
begin
  inherited create(ARect,AAlignment);
  FText := 'SWidgetImage';
  FImage := AImage;//nil;
end;

//----------

destructor SWidgetImage.destroy;
begin
  inherited;
end;

//----------

{
  If you'd like to instead copy some (width, height) rectangle from (source_x,
  source_y) to some point (dest_x, dest_y) on the destination you would instead
  compute a new position for the source surface origin and then use cairo_fill
  instead of cairo_paint:
  cairo_set_source_surface (cr, source, dest_x - source_x, dest_y - source_y)
  cairo_rectangle (cr, dest_x, dest_y, width, height);
  cairo_fill (cr);
}

procedure SWidgetImage.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  wr{,r}:SRect;
begin
  if Assigned(FImage) then
  begin
    wr := FRect;
    //r := wr;
    //r.crop(ARect);
    //if not r.empty then
    //begin
      ACanvas.setSourceSurface(FImage.getHandle,wr.x,wr.y);
      //ACanvas.rectangle(r.x,r.y,r.w,r.h);
      ACanvas.rectangle(wr.x,wr.y,wr.w,wr.h);
      ACanvas.fill;
    //end;
  end;
  inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

