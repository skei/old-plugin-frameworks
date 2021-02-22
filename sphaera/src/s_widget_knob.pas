{$include sphaera.inc}
unit s_widget_knob;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  s_color,
  s_rect,
  s_widget_slider;

type

  SWidgetKnob = class(SWidgetSlider)
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
  public
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_parameter,
  s_util;

//----------

constructor SWidgetKnob.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText       := 'SWidgetKnob';
  FBackColor  := s_dark_grey;
  FBarColor   := s_light_grey;
  FTextColor  := s_white;
end;

//----------

destructor SWidgetKnob.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetKnob.on_Paint(ACanvas:SCanvas; ARect:SRect);
var
  wr : SRect;
  v : single;
  r,x,y,a1,a2 : single;
  p : SParameter;
  s : PChar;
  ss : array[0..255] of char;
begin
  p  := FParameter;
  wr := FRect;
  v  := FValue;
  r  := (wr.h / 2);         // knob radius
  x  := wr.x + r + (5/2);   // knob center x
  y  := wr.y + r + (5/2);   // knob center y
  a1 := (S_PI2*0.375);      // start angle

  { background }
  ACanvas.setColor(s_grey);
  ACanvas.rectangle(wr.x,wr.y,wr.w,wr.h);
  //ACanvas.drawRoundedPath(wr.x,wr.y,wr.w,wr.h,5);
  ACanvas.fill;

  { arc background }
  ACanvas.setLineWidth(6);
  a2 := a1 + (S_PI2*0.75);
  ACanvas.setColor(FBackColor);
  ACanvas.newSubPath;
  ACanvas.arc( x,y,r-5,a1,a2);
  ACanvas.stroke;

  { arc value }
  a2 := a1 + (S_PI2*0.75*v);
  ACanvas.setColor(FBarColor);
  ACanvas.newSubPath;
  ACanvas.arc( x,y,r-5,a1,a2);
  ACanvas.stroke;

  // text
  ACanvas.setColor(FTextColor);
  //ACanvas.selectFontFace('Verdana',sfs_Normal,sfw_Bold);
  ACanvas.setFontSize(12);

  if Assigned(p) then s := p.getName
  else s := 'param';
  ACanvas.moveTo(wr.x+wr.h+10 ,wr.y+13);
  ACanvas.showText(s);

  if Assigned(p) then s := p.getDisplay(v)
  else
  begin
    //v := FValue;
    SFloatToString(@ss,v);
    s := pchar(ss);
  end;
  ACanvas.moveTo(wr.x+wr.h+10 ,wr.y+26); // todo: calc center, bottom
  ACanvas.showText(s);
  //inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

