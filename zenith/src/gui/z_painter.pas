unit z_painter;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_cairo,
  z_widget,
  z_rect,
  z_color;

type

  ZPaintColor = class(ZWidgetPainter)
  private
    FColor      : ZColor;
  public
    constructor create(AColor:ZColor);
    procedure   on_paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect); override;
  end;

  ZPaintValue = class(ZWidgetPainter)
  private
    FBackColor  : ZColor;
    FBarColor   : ZColor;
    FTextColor  : ZColor;
  public
    constructor create;
    constructor create(ABack:ZColor; ABar:ZColor; AText:ZColor);
    procedure   on_Paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect); override;
  end;


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_util,
  z_parameter;

constructor ZPaintColor.create(AColor:ZColor);
begin
  FColor := AColor;
end;

procedure ZPaintColor.on_paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect);
var
  r:ZRect;
begin
  //writeln('paint!');
  r := AWidget.getRect;
  //r.crop(ARect);
  //f not r.empty then
  begin
    ACairo.setSourceColor(FColor.r,FColor.g,FColor.b);
    ACairo.rectangle(r.x,r.y,r.w,r.h);
    ACairo.fill;
  end;
end;

//----------------------------------------------------------------------

constructor ZPaintValue.create;
begin
  inherited create;
  FBackColor  := color(0.4);
  FBarColor   := color(0.6);
  FTextColor  := color(1,1,0);
end;

//----------

constructor ZPaintValue.create(ABack:ZColor; ABar:ZColor; AText:ZColor);
begin
  inherited create;
  FBackColor  := ABack;
  FBarColor   := ABar;
  FTextColor  := AText;
end;

//----------

procedure ZPaintValue.on_Paint(AWidget:ZWidget; ACairo:ZCairo; ARect:ZRect);
var
  R : ZRect;
  s : PChar;
  ss : array[2..255] of char;
  width : single;
  v : single;
  p : ZParameter;
begin
  writeln('ZValuePainter.on_Paint');
  R := AWidget.getRect;
  { background }
  ACairo.setSourceColor(FBackColor.r,FBackColor.g,FBackColor.b,FBackColor.a);
  ACairo.rectangle(R.x,R.y,R.w,R.h);
  ACairo.fill;
  { value-bar, aka slider }
  width := R.w * AWidget.getValue;
  ACairo.setSourceColor(FBarColor.r,FBarColor.g,FBarColor.b,FBarColor.a);
  ACairo.rectangle(R.x,R.y,width,R.h);
  ACairo.fill;
  { value-text }
  ACairo.setSourceColor(FTextColor.r,FTextColor.g,FTextColor.b,FTextColor.a);
  ACairo.selectFontFace('Arial',zfs_Normal,zfw_Normal);
  ACairo.setFontSize(16);
  ACairo.moveTo(R.x+4 ,R.y+16);
  p := AWidget.getParameter;
  if Assigned(p) then
  begin
    v := p.from01( AWidget.getValue );
    s := p.getDisplay( v );
  end
  else
  begin
    ZFloatToString(@ss, AWidget.getValue);
    s := @ss;
  end;
  ACairo.showText(s);
end;


end.
