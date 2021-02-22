{$include sphaera.inc}
unit s_widget_image_knob;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  //s_color,
  //s_const,
  s_rect,
  s_surface,
  //s_widget,
  s_widget_knob;

const
  // image-strip direction
  sid_vertical    = 0;
  sid_horizontal  = 1;

type

  SWidgetImageKnob = class(SWidgetKnob)
  protected
    FImage      : SSurface;
    FImageNum   : LongInt;
    FImageDir   : LongInt;
    FImageSize  : LongInt;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   setup(AImage:SSurface; ANum:LongInt; ADir:LongInt);
  public
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
  public
    property    image    : SSurface read FImage write FImage;
    property    imageNum : LongInt read FImageNum write FImageNum;
    property    imageDir : LongInt read FImageDir write FImageDir;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
//  s_const,
  s_parameter;
//  s_util,
//  s_window;

//----------

constructor SWidgetImageKnob.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText := 'SWidgetImageStrip';
  FImage := nil;
  FImageNum := 0;
  FImageDir := sid_vertical;
  FImageSize := 0;;
end;

//----------

destructor SWidgetImageKnob.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetImageKnob.setup(AImage:SSurface; ANum:LongInt; ADir:LongInt);
var
  s : longint;
begin
  FImage     := AImage;
  FImageNum  := ANum;
  FImageDir  := ADir;
  s := 0;
  case FImageDir of
    sid_vertical:   s := FImage.getHeight;
    sid_horizontal: s := FImage.getWidth;
  end; // case
  FImageSize := s div FImageNum;
end;

//----------

procedure SWidgetImageKnob.on_Paint(ACanvas:SCanvas; ARect:SRect);
var
  wr  : SRect;
  //r   : SRect;
  v   : single;
  p   : SParameter;
  o   : LongInt;
begin
  wr := FRect;
  //r := wr;
  //r.crop(ARect);
  //if not r.empty then
  //begin
    p := FParameter;
    {if Assigned(p) then v := p.FValue else} v := FValue;
    o := trunc( (FImageNum * 0.99) * v ) * FImageSize;
    case FImageDir of
      sid_vertical:   ACanvas.setSourceSurface(FImage.getHandle,wr.x,wr.y-o);
      sid_horizontal: ACanvas.setSourceSurface(FImage.getHandle,wr.x+o,wr.y);
    end; // case
    ACanvas.rectangle(wr.x,wr.y,wr.w,wr.h);
    ACanvas.fill;
  //end;
  //inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

