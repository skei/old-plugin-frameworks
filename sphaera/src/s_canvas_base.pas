{$include sphaera.inc}
unit s_canvas_base;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Cairo, // get rid of this (text extents!)
  //CairoWin32,
  s_color,
  s_const,
  s_rect;//,
  //s_surface;

type

  SCanvasBase = class
  //public
  //  constructor create(ASurface:SSurface);
  //  destructor  destroy; override;
  public
    procedure   arc(x,y,r,a1,a2:double); virtual;
    procedure   arc_negative(x,y,r,a1,a2:double); virtual;
    procedure   clip; virtual;
    procedure   clipPreserve; virtual;
    procedure   closePath; virtual;
    procedure   curveTo(x1,y1,x2,y2,x3,y3:double); virtual;
    procedure   deviceToUser(x,y:PDouble); virtual;
    procedure   deviceToUserDistance(x,y:PDouble); virtual;
    procedure   fill; virtual;
    procedure   fillPreserve; virtual;
    procedure   getCurrentPoint(x,y:PDouble); virtual;
    procedure   identityMatrix; virtual;
    procedure   lineTo(x,y:double); virtual;
    procedure   moveTo(x,y:double); virtual;
    procedure   newPath; virtual;
    procedure   newSubPath; virtual;
    procedure   paint; virtual;
    procedure   paintWithAlpha(a:double); virtual;
    procedure   rectangle(x,y,w,h:double); virtual;
    procedure   relCurveTo(x1,y1,x2,y2,x3,y3:double); virtual;
    procedure   relLineTo(x,y:double); virtual;
    procedure   relMoveTo(x,y:double); virtual;
    procedure   resetClip; virtual;
    procedure   restore; virtual;
    procedure   rotate(a:double); virtual;
    procedure   save; virtual;
    procedure   scale(x,y:double); virtual;
    procedure   selectFontFace(AFamily:PChar; ASlant:cairo_font_slant_t; AWeight:cairo_font_weight_t); virtual;
    procedure   setAntialias(a:cairo_antialias_t); virtual;
    procedure   setDash(const ADashes:PDouble; ANumDashes:longint; AOffset:double); virtual;
    procedure   setFillRule(r:cairo_fill_rule_t); virtual;
    procedure   setFontSize(ASize:double); virtual;
    procedure   setLineCap(c:cairo_line_cap_t); virtual;
    procedure   setLineJoin(j:cairo_line_join_t); virtual;
    procedure   setLineWidth(s:double); virtual;
    procedure   setMiterLimit(l:double); virtual;
    procedure   setOperator(o:cairo_operator_t); virtual;
    procedure   setSourceColor(r,g,b:double); virtual;
    procedure   setSourceColor(r,g,b,a:double); virtual;
    procedure   setSourceSurface(srf:Pcairo_surface_t; x,y:double); virtual;
    procedure   setTolerance(t:double); virtual;
    procedure   showText(t:PChar); virtual;
    procedure   stroke; virtual;
    procedure   strokePreserve; virtual;
    procedure   textPath(t:PChar); virtual;
    //function    textExtents(AText:PChar) : PTextExtents; virtual;
    procedure   translate(x,y:double); virtual;
    procedure   userToDevice(x,y:PDouble); virtual;
    procedure   userToDeviceDistance(x,y:PDouble); virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{constructor SCanvasBase.create(ASurface:SSurface);
begin
  inherited create;
end;}

{destructor SCanvasBase.destroy;
begin
  inherited;
end;}

//----------

procedure SCanvasBase.arc(x,y,r,a1,a2:double);
begin
end;

procedure SCanvasBase.arc_negative(x,y,r,a1,a2:double);
begin
end;

procedure SCanvasBase.clip;
begin
end;

procedure SCanvasBase.clipPreserve;
begin
end;

procedure SCanvasBase.closePath;
begin
end;

procedure SCanvasBase.curveTo(x1,y1,x2,y2,x3,y3:double);
begin
end;

procedure SCanvasBase.deviceToUser(x,y:PDouble);
begin
end;

procedure SCanvasBase.deviceToUserDistance(x,y:PDouble);
begin
end;

procedure SCanvasBase.fill;
begin
end;

procedure SCanvasBase.fillPreserve;
begin
end;

procedure SCanvasBase.getCurrentPoint(x,y:PDouble);
begin
end;

procedure SCanvasBase.identityMatrix;
begin
end;

procedure SCanvasBase.lineTo(x,y:double);
begin
end;

procedure SCanvasBase.moveTo(x,y:double);
begin
end;

procedure SCanvasBase.newPath;
begin
end;

procedure SCanvasBase.newSubPath;
begin
end;

procedure SCanvasBase.paint;
begin
end;

procedure SCanvasBase.paintWithAlpha(a:double);
begin
end;

procedure SCanvasBase.rectangle(x,y,w,h:double);
begin
end;

procedure SCanvasBase.relCurveTo(x1,y1,x2,y2,x3,y3:double);
begin
end;

procedure SCanvasBase.relLineTo(x,y:double);
begin
end;

procedure SCanvasBase.relMoveTo(x,y:double);
begin
end;

procedure SCanvasBase.resetClip;
begin
end;

procedure SCanvasBase.restore;
begin
end;

procedure SCanvasBase.rotate(a:double);
begin
end;

procedure SCanvasBase.save;
begin
end;

procedure SCanvasBase.scale(x,y:double);
begin
end;

procedure SCanvasBase.selectFontFace(AFamily:PChar; ASlant:cairo_font_slant_t; AWeight:cairo_font_weight_t);
begin
end;

procedure SCanvasBase.setAntialias(a:cairo_antialias_t);
begin
end;

procedure SCanvasBase.setDash(const ADashes:PDouble; ANumDashes:longint; AOffset:double);
begin
end;

procedure SCanvasBase.setFillRule(r:cairo_fill_rule_t);
begin
end;

procedure SCanvasBase.setFontSize(ASize:double);
begin
end;

procedure SCanvasBase.setLineCap(c:cairo_line_cap_t);
begin
end;

procedure SCanvasBase.setLineJoin(j:cairo_line_join_t);
begin
end;

procedure SCanvasBase.setLineWidth(s:double);
begin
end;

procedure SCanvasBase.setMiterLimit(l:double);
begin
end;

procedure SCanvasBase.setOperator(o:cairo_operator_t);
begin
end;

procedure SCanvasBase.setSourceColor(r,g,b:double);
begin
end;

procedure SCanvasBase.setSourceColor(r,g,b,a:double);
begin
end;

procedure SCanvasBase.setSourceSurface(srf:Pcairo_surface_t; x,y:double);
begin
end;

procedure SCanvasBase.setTolerance(t:double);
begin
end;

procedure SCanvasBase.showText(t:PChar);
begin
end;

procedure SCanvasBase.stroke;
begin
end;

procedure SCanvasBase.strokePreserve;
begin
end;

{function SCanvasBase.textExtents(AText:PChar) : PTextExtents;
begin
  result := nil;
end;}

procedure SCanvasBase.textPath(t:PChar);
begin
end;

procedure SCanvasBase.translate(x,y:double);
begin
end;

procedure SCanvasBase.userToDevice(x,y:PDouble);
begin
end;

procedure SCanvasBase.userToDeviceDistance(x,y:PDouble);
begin
end;

//----------------------------------------------------------------------
end.
