{$include sphaera.inc}
unit s_widget_text;

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

  SWidgetText = class(SWidget)
  protected
    FDrawBack   : Boolean;
    FBackColor  : SColor;
    FTextColor  : SColor;
    FTextAlign  : LongInt;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
    property    drawBack  : Boolean read FDrawBack write FDrawBack;
    property    backColor : SColor read FBackColor write FBackColor;
    property    textColor : SColor read FTextColor write FTextColor;
    property    text      : PChar read FText write FText;
    property    textAlign : LongInt read FTextAlign write FTextAlign;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const;

//----------

constructor SWidgetText.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FDrawBack   := False;
  FBackColor  := s_dark_grey;
  FTextColor  := s_light_grey;
  FText       := 'SWidgetText';
  FTextAlign  := sta_center;
end;

//----------

destructor SWidgetText.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetText.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  r:SRect;
begin
  r := FRect;
  if FDrawBack then
  begin
    ACanvas.setColor(FBackColor);
    ACanvas.rectangle(r.x,r.y,r.w,r.h);
    ACanvas.fill;
  end;
  R.add(2,2,-4,-4);
  ACanvas.setColor(FTextColor);
  ACanvas.setFontSize(12);
  ACanvas.drawTextBox(r.x,r.y,r.w,r.h,FText,FTextAlign);
  inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

