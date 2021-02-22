{$include sphaera.inc}
unit s_canvas_cairo;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Cairo,
  //CairoWin32,
  s_canvas_base,
  //s_color,
  //s_const,
  //s_rect,
  s_surface_base;

const

  { }

  saa_Default  = CAIRO_ANTIALIAS_DEFAULT;
  saa_None     = CAIRO_ANTIALIAS_NONE;
  saa_Gray     = CAIRO_ANTIALIAS_GRAY;
  saa_Subpixel = CAIRO_ANTIALIAS_SUBPIXEL;

  sfr_Winding  = CAIRO_FILL_RULE_WINDING;
  sfr_EvenOdd  = CAIRO_FILL_RULE_EVEN_ODD;

  sfs_Normal   = CAIRO_FONT_SLANT_NORMAL;
  sfs_Italic   = CAIRO_FONT_SLANT_ITALIC;
  sfs_Oblique  = CAIRO_FONT_SLANT_OBLIQUE;

  sfw_Normal   = CAIRO_FONT_WEIGHT_NORMAL;
  sfw_Bold     = CAIRO_FONT_WEIGHT_BOLD;

  slc_Butt     = CAIRO_LINE_CAP_BUTT;
  slc_Round    = CAIRO_LINE_CAP_ROUND;
  slc_Square   = CAIRO_LINE_CAP_SQUARE;

  slj_Miter    = CAIRO_LINE_JOIN_MITER;
  slj_Round    = CAIRO_LINE_JOIN_ROUND;
  slj_Bevel    = CAIRO_LINE_JOIN_BEVEL;

  sop_Clear    = CAIRO_OPERATOR_CLEAR;
  sop_Source   = CAIRO_OPERATOR_SOURCE;
  sop_Over     = CAIRO_OPERATOR_OVER;
  sop_In       = CAIRO_OPERATOR_IN;
  sop_Out      = CAIRO_OPERATOR_OUT;
  sop_Atop     = CAIRO_OPERATOR_ATOP;
  sop_Dest     = CAIRO_OPERATOR_DEST;
  sop_DestOver = CAIRO_OPERATOR_DEST_OVER;
  sop_DestIn   = CAIRO_OPERATOR_DEST_IN;
  sop_DestOut  = CAIRO_OPERATOR_DEST_OUT;
  sop_DestAtop = CAIRO_OPERATOR_DEST_ATOP;
  sop_Xor      = CAIRO_OPERATOR_XOR;
  sop_Add      = CAIRO_OPERATOR_ADD;
  sop_Saturate = CAIRO_OPERATOR_SATURATE;

  sop_Multiply      = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  1 );
  sop_Screen        = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  2 );
  sop_Overlay       = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  3 );
  sop_Darken        = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  4 );
  sop_Lighten       = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  5 );
  sop_ColorDodge    = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  6 );
  sop_ColorBurn     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  7 );
  sop_HardLight     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  8 );
  sop_SoftLight     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) +  9 );
  sop_Difference    = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 10 );
  sop_Exclusion     = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 11 );
  sop_HslHue        = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 12 );
  sop_HslSaturation = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 13 );
  sop_HslColor      = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 14 );
  sop_HslLuminosity = cairo_operator_t( ord(CAIRO_OPERATOR_SATURATE) + 15 );

type

  TAntiAlias    = cairo_antialias_t;
  TFillRule     = cairo_fill_rule_t;
  TFontSlant    = cairo_font_slant_t;
  TFontWeight   = cairo_font_weight_t;
  TLineCap      = cairo_line_cap_t;
  TLineJoin     = cairo_line_join_t;
  TOperator     = cairo_operator_t;
  TTextExtents  = cairo_text_extents_t;

  PAntiAlias    = ^TAntiAlias;
  PFillRule     = ^TFillRule;
  PFontSlant    = ^TFontSlant;
  PFontWeight   = ^TFontWeight;
  PLineCap      = ^TLineCap;
  PLineJoin     = ^TLineJoin;
  POperator     = ^TOperator;
  PTextExtents  = ^TTextExtents;

  { }

  SCanvasCairo = class(SCanvasBase)
  private
    mCairo      : Pcairo_t;
    FSurface    : SSurfaceBase;
    mTextExt    : cairo_text_extents_t;
  public
    property    _cairo   : Pcairo_t read mCairo;
    property    _surface : SSurfaceBase read FSurface;
  public
    constructor create(ASurface:SSurfaceBase);
    destructor  destroy; override;
  public
    procedure   arc(x,y,r,a1,a2:double); override;
    procedure   arc_negative(x,y,r,a1,a2:double); override;
    procedure   clip; override;
    procedure   clipPreserve; override;
    procedure   closePath; override;
    procedure   curveTo(x1,y1,x2,y2,x3,y3:double); override;
    procedure   deviceToUser(x,y:PDouble); override;
    procedure   deviceToUserDistance(x,y:PDouble); override;
    procedure   fill; override;
    procedure   fillPreserve; override;
    procedure   getCurrentPoint(x,y:PDouble); override;
    procedure   identityMatrix; override;
    procedure   lineTo(x,y:double); override;
    procedure   moveTo(x,y:double); override;
    procedure   newPath; override;
    procedure   newSubPath; override;
    procedure   paint; override;
    procedure   paintWithAlpha(a:double); override;
    procedure   rectangle(x,y,w,h:double); override;
    procedure   relCurveTo(x1,y1,x2,y2,x3,y3:double); override;
    procedure   relLineTo(x,y:double); override;
    procedure   relMoveTo(x,y:double); override;
    procedure   resetClip; override;
    procedure   restore; override;
    procedure   rotate(a:double); override;
    procedure   save; override;
    procedure   scale(x,y:double); override;
    procedure   selectFontFace(AFamily:PChar; ASlant:cairo_font_slant_t; AWeight:cairo_font_weight_t); override;
    procedure   setAntialias(a:cairo_antialias_t); override;
    procedure   setDash(const ADashes:PDouble; ANumDashes:longint; AOffset:double); override;
    procedure   setFillRule(r:cairo_fill_rule_t); override;
    procedure   setFontSize(ASize:double); override;
    procedure   setLineCap(c:cairo_line_cap_t); override;
    procedure   setLineJoin(j:cairo_line_join_t); override;
    procedure   setLineWidth(s:double); override;
    procedure   setMiterLimit(l:double); override;
    procedure   setOperator(o:cairo_operator_t); override;
    procedure   setSourceColor(r,g,b:double); override;
    procedure   setSourceColor(r,g,b,a:double); override;
    procedure   setSourceSurface(srf:Pcairo_surface_t; x,y:double); override;
    procedure   setTolerance(t:double); override;
    procedure   showText(t:PChar); override;
    procedure   stroke; override;
    procedure   strokePreserve; override;
    procedure   textPath(t:PChar); override;
    function    textExtents(AText:PChar) : PTextExtents; virtual{override}; // ------------------------ !!!
    procedure   translate(x,y:double); override;
    procedure   userToDevice(x,y:PDouble); override;
    procedure   userToDeviceDistance(x,y:PDouble); override;
  end;

  SCanvasImpl = SCanvasCairo;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------


//----------

constructor SCanvasCairo.create(ASurface:SSurfaceBase);
begin
  inherited create;
  FSurface := ASurface;
  mCairo := cairo_create(FSurface.getHandle);
  cairo_set_line_width(mCairo,1);
end;

//----------

destructor SCanvasCairo.destroy;
begin
  cairo_destroy(mCairo);
  inherited;
end;

//----------------------------------------------------------------------

procedure SCanvasCairo.arc(x,y,r,a1,a2:double);
begin
  cairo_arc(mCairo,x,y,r,a1,a2);
end;

//----------

procedure SCanvasCairo.arc_negative(x,y,r,a1,a2:double);
begin
  cairo_arc(mCairo,x,y,r,a1,a2);
end;

//----------

procedure SCanvasCairo.clip;
begin
  cairo_clip(mCairo);
end;

//----------

procedure SCanvasCairo.clipPreserve;
begin
  cairo_clip_preserve(mCairo);
end;

//----------

procedure SCanvasCairo.closePath;
begin
  cairo_close_path(mCairo);
end;

//----------

procedure SCanvasCairo.curveTo(x1,y1,x2,y2,x3,y3:double);
begin
  cairo_curve_to(mCairo,x1,y1,x2,y2,x3,y3);
end;

//----------

procedure SCanvasCairo.deviceToUser(x,y:PDouble);
begin
  cairo_device_to_user(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.deviceToUserDistance(x,y:PDouble);
begin
  cairo_device_to_user_distance(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.fill;
begin
  cairo_fill(mCairo);
end;

//----------

procedure SCanvasCairo.fillPreserve;
begin
  cairo_fill_preserve(mCairo);
end;

//----------

procedure SCanvasCairo.getCurrentPoint(x,y:PDouble);
begin
  cairo_get_current_point(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.identityMatrix;
begin
  cairo_identity_matrix(mCairo);
end;

//----------

procedure SCanvasCairo.lineTo(x,y:double);
begin
  cairo_line_to(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.moveTo(x,y:double);
begin
  cairo_move_to(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.newPath;
begin
  cairo_new_path(mCairo);
end;

//----------

procedure SCanvasCairo.newSubPath;
begin
  cairo_new_sub_path(mCairo);
end;

//----------

procedure SCanvasCairo.paint;
begin
  cairo_paint(mCairo);
end;

//----------

procedure SCanvasCairo.paintWithAlpha(a:double);
begin
  cairo_paint_with_alpha(mCairo,a);
end;

//----------

procedure SCanvasCairo.rectangle(x,y,w,h:double);
begin
  cairo_rectangle(mCairo,x,y,w,h);
end;

//----------

procedure SCanvasCairo.relCurveTo(x1,y1,x2,y2,x3,y3:double);
begin
  cairo_curve_to(mCairo,x1,y1,x2,y2,x3,y3);
end;

//----------

procedure SCanvasCairo.relLineTo(x,y:double);
begin
  cairo_line_to(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.relMoveTo(x,y:double);
begin
  cairo_move_to(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.resetClip;
begin
  cairo_reset_clip(mCairo);
end;

//----------

procedure SCanvasCairo.restore;
begin
  cairo_restore(mCairo);
end;

//----------

procedure SCanvasCairo.rotate(a:double);
begin
  cairo_rotate(mCairo,a);
end;

//----------

procedure SCanvasCairo.save;
begin
  cairo_save(mCairo);
end;

//----------

procedure SCanvasCairo.scale(x,y:double);
begin
  cairo_scale(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.selectFontFace(AFamily:PChar; ASlant:cairo_font_slant_t; AWeight:cairo_font_weight_t);
begin
  cairo_select_font_face(mCairo,AFamily,ASlant,AWeight);
end;

//----------

procedure SCanvasCairo.setAntialias(a:cairo_antialias_t);
begin
  cairo_set_antialias(mCairo,a);
end;

//----------

procedure SCanvasCairo.setDash(const ADashes:PDouble; ANumDashes:longint; AOffset:double);
begin
  cairo_set_dash(mCairo,ADashes,ANumDashes,AOffset);
end;

//----------

procedure SCanvasCairo.setFillRule(r:cairo_fill_rule_t);
begin
  cairo_set_fill_rule(mCairo,r);
end;

//----------

procedure SCanvasCairo.setFontSize(ASize:double);
begin
  cairo_set_font_size(mCairo,ASize);
end;

//----------

procedure SCanvasCairo.setLineCap(c:cairo_line_cap_t);
begin
  cairo_set_line_cap(mCairo,c);
end;

//----------

procedure SCanvasCairo.setLineJoin(j:cairo_line_join_t);
begin
  cairo_set_line_join(mCairo,j);
end;

//----------

procedure SCanvasCairo.setLineWidth(s:double);
begin
  cairo_set_line_width (mCairo,s);
end;

//----------

procedure SCanvasCairo.setMiterLimit(l:double);
begin
  cairo_set_miter_limit(mCairo,l);
end;

//----------

procedure SCanvasCairo.setOperator(o:cairo_operator_t);
begin
  cairo_set_operator(mCairo,o);
end;

//----------

procedure SCanvasCairo.setSourceColor(r,g,b:double);
begin
  cairo_set_source_rgb(mCairo,r,g,b);
end;

//----------

procedure SCanvasCairo.setSourceColor(r,g,b,a:double);
begin
  cairo_set_source_rgba(mCairo,r,g,b,a);
end;

//----------

procedure SCanvasCairo.setSourceSurface(srf:Pcairo_surface_t; x,y:double);
begin
  cairo_set_source_surface(mCairo,srf,x,y);
end;

//----------

procedure SCanvasCairo.setTolerance(t:double);
begin
  cairo_set_tolerance(mCairo,t);
end;

//----------

procedure SCanvasCairo.showText(t:PChar);
begin
  cairo_show_text(mCairo,t);
end;

//----------

procedure SCanvasCairo.stroke;
begin
  cairo_stroke(mCairo);
end;

//----------

procedure SCanvasCairo.strokePreserve;
begin
  cairo_stroke_preserve(mCairo);
end;

//----------

function SCanvasCairo.textExtents(AText:PChar) : PTextExtents;
begin
  cairo_text_extents(mCairo,AText,@mTextExt);
  result := @mTextExt;
end;

procedure SCanvasCairo.textPath(t:PChar);
begin
  cairo_text_path(mCairo,t);
end;

//----------

procedure SCanvasCairo.translate(x,y:double);
begin
  cairo_translate(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.userToDevice(x,y:PDouble);
begin
  cairo_user_to_device(mCairo,x,y);
end;

//----------

procedure SCanvasCairo.userToDeviceDistance(x,y:PDouble);
begin
  cairo_user_to_device_distance(mCairo,x,y);
end;

//----------------------------------------------------------------------
end.
