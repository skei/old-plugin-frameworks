{$include sphaera.inc}
unit s_widget_slider;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  s_color,
  s_rect,
  s_widget_value;

type

  SWidgetSlider = class(SWidgetValue)
  protected
    //FBackColor  : SColor;
    FBarColor   : SColor;
    //FTextColor  : SColor;
    //FOnText     : PChar;
    //FOffText    : PChar;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
    property    barColor : SColor read FBackColor write FBackColor;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_parameter,
  s_util,
  s_window;

//----------

constructor SWidgetSlider.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText      := 'SWidgetSlider';
  FBackColor := s_dark_grey;
  FBarColor  := s_green;
  FTextColor := s_white;
end;

//----------

destructor SWidgetSlider.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetSlider.on_Paint(ACanvas:SCanvas; ARect:SRect);
var
  r : SRect;
  s : PChar;
  ss : array[0..255] of char; // 2..255 ??
  width : single;
  v : single;
  p : SParameter;
begin
  r := FRect;
  v := FValue;
  p := FParameter;
  { background }
  ACanvas.setColor(FBackColor);
  ACanvas.rectangle(r.x,r.y,r.w,r.h);
  ACanvas.fill;
  { value-bar, aka slider }
  width := R.w * v;//AWidget.getValue;
  ACanvas.setColor(FBarColor);
  ACanvas.rectangle(r.x,r.y,width,r.h);
  ACanvas.fill;
  { value/text }
  ACanvas.setColor(FTextColor);
  //ACanvas.selectFontFace('Arial',sfs_Normal,sfw_Bold);
  ACanvas.setFontSize(12);
  { text }
  if Assigned(p) then s := p.getName
  else s := 'param';
  ACanvas.drawTextBox(r.x+3,r.y,r.w-6,r.h,s,sta_left);
  { value }
  if Assigned(p) then s := p.getDisplay(v)
  else
  begin
    SFloatToString(@ss,v);
    s := pchar(ss);
  end;
  ACanvas.drawTextBox(r.x+3,r.y,r.w-6,r.h,s,sta_right);
  //inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

