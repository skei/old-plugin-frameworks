unit z_cairo;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_LINUX} z_cairo_linux, {$endif}
  {$ifdef Z_WIN32} z_cairo_win32, {$endif}
  Cairo;

const

  zaa_Default  = CAIRO_ANTIALIAS_DEFAULT;
  zaa_None     = CAIRO_ANTIALIAS_NONE;
  zaa_Gray     = CAIRO_ANTIALIAS_GRAY;
  zaa_Subpixel = CAIRO_ANTIALIAS_SUBPIXEL;

  zfr_Winding  = CAIRO_FILL_RULE_WINDING;
  zfr_EvenOdd  = CAIRO_FILL_RULE_EVEN_ODD;

  zfs_Normal   = CAIRO_FONT_SLANT_NORMAL;
  zfs_Italic   = CAIRO_FONT_SLANT_ITALIC;
  zfs_Oblique  = CAIRO_FONT_SLANT_OBLIQUE;

  zfw_Normal   = CAIRO_FONT_WEIGHT_NORMAL;
  zfw_Bold     = CAIRO_FONT_WEIGHT_BOLD;

  zlc_Butt     = CAIRO_LINE_CAP_BUTT;
  zlc_Round    = CAIRO_LINE_CAP_ROUND;
  zlc_Square   = CAIRO_LINE_CAP_SQUARE;

  zlj_Miter    = CAIRO_LINE_JOIN_MITER;
  zlj_Round    = CAIRO_LINE_JOIN_ROUND;
  zlj_Bevel    = CAIRO_LINE_JOIN_BEVEL;

  zop_Clear    = CAIRO_OPERATOR_CLEAR;
  zop_Source   = CAIRO_OPERATOR_SOURCE;
  zop_Over     = CAIRO_OPERATOR_OVER;
  zop_In       = CAIRO_OPERATOR_IN;
  zop_Out      = CAIRO_OPERATOR_OUT;
  zop_Atop     = CAIRO_OPERATOR_ATOP;
  zop_Dest     = CAIRO_OPERATOR_DEST;
  zop_DestOver = CAIRO_OPERATOR_DEST_OVER;
  zop_DestIn   = CAIRO_OPERATOR_DEST_IN;
  zop_DestOut  = CAIRO_OPERATOR_DEST_OUT;
  zop_DestAtop = CAIRO_OPERATOR_DEST_ATOP;
  zop_Xor      = CAIRO_OPERATOR_XOR;
  zop_Add      = CAIRO_OPERATOR_ADD;
  zop_Saturate = CAIRO_OPERATOR_SATURATE;

  zop_Multiply      = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  1 );
  zop_Screen        = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  2 );
  zop_Overlay       = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  3 );
  zop_Darken        = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  4 );
  zop_Lighten       = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  5 );
  zop_ColorDodge    = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  6 );
  zop_ColorBurn     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  7 );
  zop_HardLight     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  8 );
  zop_SoftLight     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  9 );
  zop_Difference    = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 10 );
  zop_Exclusion     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 11 );
  zop_HslHue        = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 12 );
  zop_HslSaturation = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 13 );
  zop_HslColor      = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 14 );
  zop_HslLuminosity = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 15 );

type

  ZAntiAlias    = cairo_antialias_t;
  ZFillRule     = cairo_fill_rule_t;
  ZFontSlant    = cairo_font_slant_t;
  ZFontWeight   = cairo_font_weight_t;
  ZLineCap      = cairo_line_cap_t;
  ZLineJoin     = cairo_line_join_t;
  ZOperator     = cairo_operator_t;
  ZTextExtents  = cairo_text_extents_t;

  PZAntiAlias    = ^ZAntiAlias;
  PZFillRule     = ^ZFillRule;
  PZFontSlant    = ^ZFontSlant;
  PZFontWeight   = ^ZFontWeight;
  PZLineCap      = ^ZLineCap;
  PZLineJoin     = ^ZLineJoin;
  PZOperator     = ^ZOperator;
  PZTextExtents  = ^ZTextExtents;

  {
    cairo_surface_create_for_rectangle
    cairo_surface_create_similar
  }

//----------------------------------------------------------------------

type

  ZCairoSurface = class(ZCairoSurfaceImpl)
  public
    constructor create(AWidth,AHeight,ADepth:LongInt);
    constructor create(ASurface:ZCairoSurface; AWidth,AHeight:LongInt);
    //constructor create(AFilename:PChar);
    destructor  destroy; override;
  end;

  //----------

  ZCairo = class
  private
    FCairo      : Pcairo_t;
    FSurface    : ZCairoSurface;
    FTextExt    : cairo_text_extents_t;
  public
    property    cairo : Pcairo_t read FCairo;
    property    surface : ZCairoSurface read FSurface;
  public
    constructor create(ASurface:ZCairoSurface);
    destructor  destroy; override;
  public
    procedure   arc(x,y,r,a1,a2:double);
    procedure   arc_negative(x,y,r,a1,a2:double);
    procedure   clip;
    procedure   clipPreserve;
    procedure   closePath;
    procedure   curveTo(x1,y1,x2,y2,x3,y3:double);
    procedure   deviceToUser(x,y:PDouble);
    procedure   deviceToUserDistance(x,y:PDouble);
    procedure   fill;
    procedure   fillPreserve;
    procedure   getCurrentPoint(x,y:PDouble);
    procedure   identityMatrix;
    procedure   lineTo(x,y:double);
    procedure   moveTo(x,y:double);
    procedure   newPath;
    procedure   newSubPath;
    procedure   paint;
    procedure   paintWithAlpha(a:double);
    procedure   rectangle(x,y,w,h:double);
    procedure   relCurveTo(x1,y1,x2,y2,x3,y3:double);
    procedure   relLineTo(x,y:double);
    procedure   relMoveTo(x,y:double);
    procedure   resetClip;
    procedure   restore;
    procedure   rotate(a:double);
    procedure   save;
    procedure   scale(x,y:double);
    procedure   selectFontFace(AFamily:PChar; ASlant:cairo_font_slant_t; AWeight:cairo_font_weight_t);
    procedure   setAntialias(a:cairo_antialias_t);
    procedure   setDash(const ADashes:PDouble; ANumDashes:longint; AOffset:double);
    procedure   setFillRule(r:cairo_fill_rule_t);
    procedure   setFontSize(ASize:double);
    procedure   setLineCap(c:cairo_line_cap_t);
    procedure   setLineJoin(j:cairo_line_join_t);
    procedure   setLineWidth(s:double);
    procedure   setMiterLimit(l:double);
    procedure   setOperator(o:cairo_operator_t);
    procedure   setSourceColor(r,g,b:double);
    procedure   setSourceColor(r,g,b,a:double);
    procedure   setSourceSurface(srf:Pcairo_surface_t; x,y:double);
    procedure   setTolerance(t:double);
    procedure   showText(t:PChar);
    procedure   stroke;
    procedure   strokePreserve;
    procedure   textPath(t:PChar);
    function    textExtents(AText:PChar) : PZTextExtents;
    procedure   translate(x,y:double);
    procedure   userToDevice(x,y:PDouble);
    procedure   userToDeviceDistance(x,y:PDouble);
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// surface
//----------------------------------------------------------------------

constructor ZCairoSurface.create(AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  //inherited create;
  fmt := CAIRO_FORMAT_ARGB32;
  case ADepth of
    8  : fmt := CAIRO_FORMAT_A8;
    24 : fmt := CAIRO_FORMAT_RGB24;
    32 : fmt := CAIRO_FORMAT_ARGB32;
  end;
  cairo_image_surface_create(fmt,AWidth,AHeight);
end;

//----------

{constructor ZCairoSurface.create(AFilename:PChar);
begin
  //inherited create;
  FSurface := cairo_image_surface_create_from_png(AFilename);
end;}

//----------

constructor ZCairoSurface.create(ASurface:ZCairoSurface; AWidth,AHeight:LongInt);
begin
  //inherited create;
  FSurface := cairo_surface_create_similar(ASurface.surface,CAIRO_CONTENT_COLOR_ALPHA,AWidth,AHeight);
end;


//----------

destructor ZCairoSurface.destroy;
begin
  //cairo_surface_destroy(FSurface);
  inherited;
end;

//----------------------------------------------------------------------
// cairo
//----------------------------------------------------------------------

constructor ZCairo.create(ASurface:ZCairoSurface);
begin
  inherited create;
  FSurface := ASurface;
  FCairo := cairo_create(FSurface.surface);
end;

//----------

destructor ZCairo.destroy;
begin
  cairo_destroy(FCairo);
  inherited;
end;

//----------------------------------------------------------------------

procedure ZCairo.arc(x,y,r,a1,a2:double);
begin
  cairo_arc(FCairo,x,y,r,a1,a2);
end;

//----------

procedure ZCairo.arc_negative(x,y,r,a1,a2:double);
begin
  cairo_arc(FCairo,x,y,r,a1,a2);
end;

//----------

procedure ZCairo.clip;
begin
  cairo_clip(FCairo);
end;

//----------

procedure ZCairo.clipPreserve;
begin
  cairo_clip_preserve(FCairo);
end;

//----------

procedure ZCairo.closePath;
begin
  cairo_close_path(FCairo);
end;

//----------

procedure ZCairo.curveTo(x1,y1,x2,y2,x3,y3:double);
begin
  cairo_curve_to(FCairo,x1,y1,x2,y2,x3,y3);
end;

//----------

procedure ZCairo.deviceToUser(x,y:PDouble);
begin
  cairo_device_to_user(FCairo,x,y);
end;

//----------

procedure ZCairo.deviceToUserDistance(x,y:PDouble);
begin
  cairo_device_to_user_distance(FCairo,x,y);
end;

//----------

procedure ZCairo.fill;
begin
  cairo_fill(FCairo);
end;

//----------

procedure ZCairo.fillPreserve;
begin
  cairo_fill_preserve(FCairo);
end;

//----------

procedure ZCairo.getCurrentPoint(x,y:PDouble);
begin
  cairo_get_current_point(FCairo,x,y);
end;

//----------

procedure ZCairo.identityMatrix;
begin
  cairo_identity_matrix(FCairo);
end;

//----------

procedure ZCairo.lineTo(x,y:double);
begin
  cairo_line_to(FCairo,x,y);
end;

//----------

procedure ZCairo.moveTo(x,y:double);
begin
  cairo_move_to(FCairo,x,y);
end;

//----------

procedure ZCairo.newPath;
begin
  cairo_new_path(FCairo);
end;

//----------

procedure ZCairo.newSubPath;
begin
  cairo_new_sub_path(FCairo);
end;

//----------

procedure ZCairo.paint;
begin
  cairo_paint(FCairo);
end;

//----------

procedure ZCairo.paintWithAlpha(a:double);
begin
  cairo_paint_with_alpha(FCairo,a);
end;

//----------

procedure ZCairo.rectangle(x,y,w,h:double);
begin
  cairo_rectangle(FCairo,x,y,w,h);
end;

//----------

procedure ZCairo.relCurveTo(x1,y1,x2,y2,x3,y3:double);
begin
  cairo_curve_to(FCairo,x1,y1,x2,y2,x3,y3);
end;

//----------

procedure ZCairo.relLineTo(x,y:double);
begin
  cairo_line_to(FCairo,x,y);
end;

//----------

procedure ZCairo.relMoveTo(x,y:double);
begin
  cairo_move_to(FCairo,x,y);
end;

//----------

procedure ZCairo.resetClip;
begin
  cairo_reset_clip(FCairo);
end;

//----------

procedure ZCairo.restore;
begin
  cairo_restore(FCairo);
end;

//----------

procedure ZCairo.rotate(a:double);
begin
  cairo_rotate(FCairo,a);
end;

//----------

procedure ZCairo.save;
begin
  cairo_save(FCairo);
end;

//----------

procedure ZCairo.scale(x,y:double);
begin
  cairo_scale(FCairo,x,y);
end;

//----------

procedure ZCairo.selectFontFace(AFamily:PChar; ASlant:cairo_font_slant_t; AWeight:cairo_font_weight_t);
begin
  cairo_select_font_face(FCairo,AFamily,ASlant,AWeight);
end;

//----------

procedure ZCairo.setAntialias(a:cairo_antialias_t);
begin
  cairo_set_antialias(FCairo,a);
end;

//----------

procedure ZCairo.setDash(const ADashes:PDouble; ANumDashes:longint; AOffset:double);
begin
  cairo_set_dash(FCairo,ADashes,ANumDashes,AOffset);
end;

//----------

procedure ZCairo.setFillRule(r:cairo_fill_rule_t);
begin
  cairo_set_fill_rule(FCairo,r);
end;

//----------

procedure ZCairo.setFontSize(ASize:double);
begin
  cairo_set_font_size(FCairo,ASize);
end;

//----------

procedure ZCairo.setLineCap(c:cairo_line_cap_t);
begin
  cairo_set_line_cap(FCairo,c);
end;

//----------

procedure ZCairo.setLineJoin(j:cairo_line_join_t);
begin
  cairo_set_line_join(FCairo,j);
end;

//----------

procedure ZCairo.setLineWidth(s:double);
begin
  cairo_set_line_width (FCairo,s);
end;

//----------

procedure ZCairo.setMiterLimit(l:double);
begin
  cairo_set_miter_limit(FCairo,l);
end;

//----------

procedure ZCairo.setOperator(o:cairo_operator_t);
begin
  cairo_set_operator(FCairo,o);
end;

//----------

procedure ZCairo.setSourceColor(r,g,b:double);
begin
  cairo_set_source_rgb(FCairo,r,g,b);
end;

//----------

procedure ZCairo.setSourceColor(r,g,b,a:double);
begin
  cairo_set_source_rgba(FCairo,r,g,b,a);
end;

//----------

procedure ZCairo.setSourceSurface(srf:Pcairo_surface_t; x,y:double);
begin
  cairo_set_source_surface(FCairo,srf,x,y);
end;

//----------

procedure ZCairo.setTolerance(t:double);
begin
  cairo_set_tolerance(FCairo,t);
end;

//----------

procedure ZCairo.showText(t:PChar);
begin
  cairo_show_text(FCairo,t);
end;

//----------

procedure ZCairo.stroke;
begin
  cairo_stroke(FCairo);
end;

//----------

procedure ZCairo.strokePreserve;
begin
  cairo_stroke_preserve(FCairo);
end;

//----------

function ZCairo.textExtents(AText:PChar) : PZTextExtents;
begin
  cairo_text_extents(FCairo,AText,@FTextExt);
  result := @FTextExt;
end;

procedure ZCairo.textPath(t:PChar);
begin
  cairo_text_path(FCairo,t);
end;

//----------

procedure ZCairo.translate(x,y:double);
begin
  cairo_translate(FCairo,x,y);
end;

//----------

procedure ZCairo.userToDevice(x,y:PDouble);
begin
  cairo_user_to_device(FCairo,x,y);
end;

//----------

procedure ZCairo.userToDeviceDistance(x,y:PDouble);
begin
  cairo_user_to_device_distance(FCairo,x,y);
end;

//----------------------------------------------------------------------
end.



// http://www.rodi.dk/programming_cairo.php

{procedure DrawWithCairo(pb:TPaintBox);
var
  cr      : Pcairo_t;
  surface : Pcairo_surface_t;
begin

  a) surface := cairo_win32_surface_create(pb.Canvas.Handle) // HDC
  b) surface := cairo_win32_surface_create_with_dib(CAIRO_FORMAT_ARGB32,pb.Width,pb.Height);

  cr      := cairo_create(surface);
  cairo_scale(cr,pb.Height,pb.Height)

  // Draw the entire context white
  cairo_set_source_rgba(cr,1,1,1,1);
  cairo_paint(cr);
  // Translate to the center and draw a black clock outline
  cairo_set_source_rgba(cr,0,0,0,1);
  cairo_translate(cr,0.5,0.5);
  cairo_arc(cr,0,0,0.4,0,pi*2);
  cairo_stroke(cr);

  b) BitBlt(pb.Canvas.Handle,0,0,pb.Width,pb.Height,cairo_win32_surface_get_dc(surface),0,0,SRCCOPY);

  cairo_surface_destroy(surface);

end;}

// or:

{cairo_destroy(cr);
cairo_surface_destroy(surface);}

//----------------------------------------------------------------------
end.

(*
int w, h;
cairo_surface_t *image;
cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2*M_PI);
cairo_clip (cr);
cairo_new_path (cr); /* path not consumed by clip()*/
image = cairo_image_surface_create_from_png ("data/romedalen.png");
w = cairo_image_surface_get_width (image);
h = cairo_image_surface_get_height (image);
cairo_scale (cr, 256.0/w, 256.0/h);
cairo_set_source_surface (cr, image, 0, 0);
cairo_paint (cr);
cairo_surface_destroy (image);
*)

(*
int w, h;
cairo_surface_t *image;
image = cairo_image_surface_create_from_png ("data/romedalen.png");
w = cairo_image_surface_get_width (image);
h = cairo_image_surface_get_height (image);
cairo_translate (cr, 128.0, 128.0);
cairo_rotate (cr, 45* M_PI/180);
cairo_scale  (cr, 256.0/w, 256.0/h);
cairo_translate (cr, -0.5*w, -0.5*h);
cairo_set_source_surface (cr, image, 0, 0);
cairo_paint (cr);
cairo_surface_destroy (image);
*)


