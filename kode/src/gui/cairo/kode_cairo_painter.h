#ifndef kode_cairo_painter_included
#define kode_cairo_painter_included
//----------------------------------------------------------------------

/*
  Most surface types allow accessing the surface without using Cairo functions.
  If you do this, keep in mind that it is mandatory that you call
  cairo_surface_flush() before reading from or writing to the surface and that
  you must use cairo_surface_mark_dirty() after modifying it.
*/

#include "base/kode.h"
#include "base/kode_stack.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_painter.h"
#include "gui/cairo/kode_cairo.h"

typedef KODE_Stack<KODE_Rect,16> KODE_RectStack;

//----------------------------------------------------------------------//----------------------------------------------------------------------

const
cairo_font_slant_t kode_font_slant[3] = {
  CAIRO_FONT_SLANT_NORMAL,
  CAIRO_FONT_SLANT_ITALIC,
  CAIRO_FONT_SLANT_OBLIQUE
};

const
cairo_font_weight_t kode_font_weight[2] = {
  CAIRO_FONT_WEIGHT_NORMAL,
  CAIRO_FONT_WEIGHT_BOLD
};

const
cairo_line_cap_t kode_line_cap[3] = {
  CAIRO_LINE_CAP_BUTT,
  CAIRO_LINE_CAP_ROUND,
  CAIRO_LINE_CAP_SQUARE
};

const
cairo_line_join_t kode_line_join[3] = {
  CAIRO_LINE_JOIN_MITER,
  CAIRO_LINE_JOIN_ROUND,
  CAIRO_LINE_JOIN_BEVEL
};

//----------------------------------------------------------------------

class KODE_CairoPainter
: public KODE_BasePainter {

//------------------------------
private:
//------------------------------

  cairo_t*            MCairo        = nullptr;
  cairo_surface_t*    MCairoSurface = nullptr;
  KODE_Drawable*      MTarget       = nullptr;
  KODE_Color          MDrawColor    = KODE_LightGrey;
  KODE_Color          MFillColor    = KODE_DarkGrey;
  KODE_Color          MTextColor    = KODE_White;
  bool                MIsWindow     = false;
  
  KODE_Rect           MClipRect;
  KODE_RectStack      MClipStack;

//------------------------------
public:
//------------------------------

  /*
    cairo_create()
    returns a newly allocated cairo_t with a reference count of 1. The initial
    reference count should be released with cairo_destroy() when you are done
    using the cairo_t. This function never returns NULL. If memory cannot be
    allocated, a special cairo_t object will be returned on which cairo_status()
    returns CAIRO_STATUS_NO_MEMORY. If you attempt to target a surface which
    does not support writing (such as cairo_mime_surface_t) then a
    CAIRO_STATUS_WRITE_ERROR will be raised. You can use this object normally,
    but no drawing will be done.

    If drawable is a Window, then the function cairo_xcb_surface_set_size()
    must be called whenever the size of the window changes.
  */

  KODE_CairoPainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {
    //KODE_Assert(ATarget->isCairo());
    MTarget = ATarget;
    if (ATarget->isWindow()) { MIsWindow = true; }
    MCairoSurface = ATarget->getCairoSurface();
    MCairo = cairo_create(MCairoSurface);
    //check_cairo_errors(MCairo);
    cairo_set_line_width(MCairo,1);
    
    MClipRect = KODE_Rect( 0, 0, ATarget->getWidth()-1, ATarget->getHeight()-1 );
    //KODE_Trace("initial cliprect x %.0f y %.0f w %.0f h %.0f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    
  }

  //----------

  virtual ~KODE_CairoPainter() {
    //KODE_Trace("%s\n",cairo_status_to_string(cairo_status(MCairo)));
    //check_cairo_errors(MCairo);
    cairo_destroy(MCairo);
    //if (MCairoSurface) cairo_surface_destroy(MCairoSurface);
  }

//------------------------------
public:
//------------------------------

  KODE_Drawable* getTarget() override {return MTarget; }

  virtual cairo_t*          getCairo(void)        { return MCairo; }
  virtual cairo_surface_t*  getCairoSurface(void) { return MCairoSurface; }

  //----------

  void resize(float w, float h) override {
    if (MIsWindow) {
      cairo_xcb_surface_set_size(MCairoSurface,w,h);
    }
  }

//------------------------------
public:
//------------------------------

  /*
    Do any pending drawing for the surface and also restore any temporary
    modifications cairo has made to the surface's state. This function must be
    called before switching from drawing on the surface with cairo to drawing
    on it directly with native APIs, or accessing its memory outside of Cairo.
    If the surface doesn't support direct access, then this function does
    nothing.
  */

  void flush() override {
    cairo_surface_flush(MCairoSurface);
  }

  //----------

  /*
    Tells cairo that drawing has been done to surface using means other than
    cairo, and that cairo should reread any cached areas. Note that you must
    call cairo_surface_flush() before doing such drawing.
  */

  void dirty() override {
    cairo_surface_mark_dirty(MCairoSurface);
  }

  void dirty(float AX1, float AY1, float AX2, float AY2) override {
    cairo_surface_mark_dirty_rectangle(MCairoSurface,AX1,AY1,AX2,AY2);
  }

  void dirty(KODE_Rect ARect) override {
    cairo_surface_mark_dirty_rectangle(MCairoSurface,ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------

  /*
    This function finishes the surface and drops all references to external
    resources. For example, for the Xlib backend it means that cairo will no
    longer access the drawable, which can be freed. After calling
    cairo_surface_finish() the only valid operations on a surface are getting
    and setting user, referencing and destroying, and flushing and finishing it.
    Further drawing to the surface will not affect the surface but will instead
    trigger a CAIRO_STATUS_SURFACE_FINISHED error.
    When the last call to cairo_surface_destroy() decreases the reference count
    to zero, cairo will call cairo_surface_finish() if it hasn't been called
    already, before freeing the resources associated with the surface.
  */

  void finish() override {
    cairo_surface_finish(MCairoSurface);
  }

//------------------------------
private:
//------------------------------

  void _draw(bool APreserve=false) {
    if (APreserve) cairo_stroke_preserve(MCairo);
    else cairo_stroke(MCairo);
  }

  //----------

  void _fill(bool APreserve=false) {
    if (APreserve) cairo_fill_preserve(MCairo);
    else cairo_fill(MCairo);
  }

  //----------

  void _fillGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical, bool APreserve=false) {
    cairo_pattern_t *pat;
    if (AVertical) pat = cairo_pattern_create_linear( AX1,AY1, AX1,AY2 );
    else pat = cairo_pattern_create_linear( AX1,AY1, AX2,AY1 );
    cairo_pattern_add_color_stop_rgba( pat, 0, AColor1.r, AColor1.g, AColor1.b, AColor1.a );
    cairo_pattern_add_color_stop_rgba( pat, 1, AColor2.r, AColor2.g, AColor2.b, AColor2.a );
    cairo_set_source(MCairo,pat);
    if (APreserve) cairo_fill_preserve(MCairo);
    else cairo_fill(MCairo);
    cairo_pattern_destroy(pat);
  }

  //----------

  void _moveTo(float AX, float AY) {
    cairo_move_to(MCairo,AX,AY);
  }

  //----------

  void _lineTo(float AX, float AY) {
    cairo_line_to(MCairo,AX,AY);
  }

  //----------

  void _rectangle(float AX1, float AY1, float AX2, float AY2) {
    cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
  }

  //----------

  // cairo draws 41
  //             32

  void _roundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) {
    int32_t x = AX1;
    int32_t y = AY1;
    int32_t w = AX2-AX1+1;
    int32_t h = AY2-AY1+1;
    int32_t r = AR;
    float degrees = M_PI / 180.0;
    cairo_new_sub_path(MCairo);
    if (ACorners & KODE_CORNER_RIGHT_TOP) cairo_arc(MCairo, x+w-r-0, y+r, r, -90*degrees, 0*degrees);
    else cairo_move_to(MCairo, x+w-0, y);
    if (ACorners & KODE_CORNER_RIGHT_BOTTOM) cairo_arc(MCairo, x+w-r-0, y+h-r-0, r, 0*degrees, 90*degrees);
    else cairo_line_to(MCairo, x+w-0, y+h-0);
    if (ACorners & KODE_CORNER_LEFT_BOTTOM) cairo_arc(MCairo, x+r, y+h-r-0, r, 90*degrees, 180*degrees);
    else cairo_line_to(MCairo, x, y+h-0);
    if (ACorners & KODE_CORNER_LEFT_TOP) cairo_arc(MCairo, x+r, y+r, r, 180*degrees, 270*degrees);
    else cairo_line_to(MCairo, x, y);
    cairo_close_path(MCairo);
  }

  //----------

  void _ellipse(float AX1, float AY1, float AX2, float AY2) {
    float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
    float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
    cairo_save(MCairo);
    cairo_translate(MCairo,AX1+w2,AY1+h2);
    cairo_scale(MCairo,w2,h2);
    cairo_new_sub_path(MCairo); // ???
    cairo_arc(MCairo,0,0,1,0,(M_PI*2.0));
    cairo_restore(MCairo);
  }

  //----------

  void _arc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) {
    float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
    float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
    float a1 = (AAngle1+0.75) * (M_PI*2.0);
    float a2 = (AAngle1+AAngle2+0.75) * (M_PI*2.0);
    cairo_move_to(MCairo,AX1+w2,AY1+h2);
    cairo_save(MCairo);
    cairo_translate(MCairo,AX1+w2,AY1+h2);
    cairo_scale(MCairo,w2,h2);

    cairo_new_sub_path(MCairo);

    cairo_arc(MCairo,0,0,1,a1,a2);
    cairo_restore(MCairo);
  }

  //----------

  void _triangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) {
    cairo_move_to(MCairo,AX1,AY1);
    cairo_line_to(MCairo,AX2,AY2);
    cairo_line_to(MCairo,AX3,AY3);
    cairo_line_to(MCairo,AX1,AY1);
  }

  //----------

  void _curveTo(float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) {
    cairo_curve_to(MCairo,AX2,AY2,AX3,AY3,AX4,AY4);
  }

//------------------------------
public:
//------------------------------

  void setDrawColor(KODE_Color AColor) override {
    MDrawColor = AColor;
    cairo_set_source_rgba(MCairo,MDrawColor.r,MDrawColor.g,MDrawColor.b,MDrawColor.a);
  }

  //----------

  void setFillColor(KODE_Color AColor) override {
    MFillColor = AColor;
    cairo_set_source_rgba(MCairo,AColor.r,AColor.g,AColor.b,AColor.a);
  }

  //----------

  void setTextColor(KODE_Color AColor) override {
    MTextColor = AColor;
    cairo_set_source_rgba(MCairo,MTextColor.r,MTextColor.g,MTextColor.b,MTextColor.a);
  }

  void setPenSize(float ASize) override {
    cairo_set_line_width(MCairo,ASize);
  }

  void setPenDash(double* ADashes, uint32_t ANumDashes, float AOffset) override {
    cairo_set_dash(MCairo,ADashes,ANumDashes,AOffset);
  }

  void setLineCap(uint32_t ALineCap) override {
    cairo_set_line_cap(MCairo,kode_line_cap[ALineCap]);
  }

  void setLineJoin(uint32_t ALineJoin) override {
    cairo_set_line_join(MCairo,kode_line_join[ALineJoin]);
  }

  /*
    If text is drawn without a call to cairo_set_font_size(), the default font
    size is 10.0.
  */

  void setTextSize(float ASize) override {
    cairo_set_font_size(MCairo,ASize);
  }

  /*
    Replaces the current cairo_font_face_t object in the cairo_t with font_face.
    The replaced font face in the cairo_t will be destroyed if there are no
    other references to it.
  */

  void setTextFont(const char* AName, uint32_t ASlant, uint32_t AWeight) override {
    cairo_select_font_face(MCairo,AName,kode_font_slant[ASlant],kode_font_weight[AWeight]);
  }

  float getTextWidth(const char* AText) override {
    cairo_text_extents_t e;
    cairo_text_extents(MCairo,AText,&e);
    return e.width;
  }

  float getTextHeight(const char* AText) override {
    cairo_text_extents_t e;
    cairo_text_extents(MCairo,AText,&e);
    return e.height;
  }

  //void stroke(void) override {
  //}

  //void fill(void) override {
  //}

  void drawPoint(float AX, float AY) override {
    _ellipse(AX-0.5f,AY-0.5f,AX+0.5f,AY+0.5f);
    //_rectangle(AX,AY,AX+1,AY+1);
    _fill();
  }

  /*
    "How do I draw a sharp, single-pixel-wide line?"
    http://www.cairographics.org/FAQ/#sharp_lines
  */

  void drawHLine(float AX1, float AY1, float AX2) override {
    cairo_move_to(MCairo,AX1,AY1+0.5);
    cairo_line_to(MCairo,AX2,AY1+0.5);
    cairo_stroke(MCairo);
  }

  void drawVLine(float AX1, float AY1, float AY2) override {
    cairo_move_to(MCairo,AX1+0.5,AY1);
    cairo_line_to(MCairo,AX1+0.5,AY2);
    cairo_stroke(MCairo);
  }

  void drawLine(float AX1, float AY1, float AX2, float AY2) override {
    _moveTo(AX1,AY1);
    _lineTo(AX2,AY2);
    _draw();
  }

  //void drawLines(int32 ANum, float* ACoords) override {
  //  if (ANum > 0) {
  //    _moveTo(ACoords[0],ACoords[1]);
  //    for (int32 i=1; i<ANum; i++) {
  //      _lineTo(ACoords[i*2],ACoords[i*2+1]);
  //    }
  //    _draw();
  //  }
  //}


  void drawRectangle(float AX1, float AY1, float AX2, float AY2) override {
    _rectangle(AX1,AY1,AX2,AY2);
    _draw();
  }

  void drawRectangle(KODE_Rect ARect) override {
    drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  void drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) override {
    _roundedRectangle(AX1,AY1,AX2,AY2,AR,AC);
    _draw();
  }

  void drawRoundedRectangle(KODE_Rect ARect, float AR, uint32_t AC) override {
    drawRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR,AC);
  }

  void drawEllipse(float AX1, float AY1, float AX2, float AY2) override {
    _ellipse(AX1,AY1,AX2,AY2);
    _draw();
  }

  void drawEllipse(KODE_Rect ARect) override {
    drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  void drawArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    _arc(AX1,AY1,AX2,AY2,AAngle1,AAngle2);
    _draw();
  }

  void drawArc(KODE_Rect ARect, float AAngle1, float AAngle2) override {
    drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
  }

  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    _triangle(AX1,AY1,AX2,AY2,AX3,AY3);
    _draw();
  }

  void drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
    _moveTo(AX1,AY1);
    _curveTo(AX2,AY2,AX3,AY3,AX4,AY4);
    _draw();
  }

  void fillRectangle(float AX1, float AY1, float AX2, float AY2) override {
    //cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
    //cairo_fill(MCairo);
    _rectangle(AX1,AY1,AX2,AY2);
    _fill();
  }

  void fillRectangle(KODE_Rect ARect) override {
    fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  void fillRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) override {
    _roundedRectangle(AX1,AY1,AX2,AY2,AR,ACorners);
    _fill();
  }

  void fillRoundedRectangle(KODE_Rect ARect, float AR, uint32_t ACorners) override {
    fillRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR,ACorners );
  }

  void fillEllipse(float AX1, float AY1, float AX2, float AY2) override {
    _ellipse(AX1,AY1,AX2,AY2);
    _fill();
  }

  void fillEllipse(KODE_Rect ARect) override {
    fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  void fillArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    //float x = AX1 + ((AX2-AX1)*0.5f);
    //float y = AY1 + ((AY2-AY1)*0.5f);
    //_moveTo(x,y);
    _arc(AX1,AY1,AX2,AY2,AAngle1,AAngle2);
    _fill();
  }

  void fillArc(KODE_Rect ARect, float AAngle1, float AAngle2) override {
    //float x = ARect.x + (ARect.w * 0.5f);
    //float y = ARect.y + (ARect.h * 0.5f);
    //_moveTo(x,y);
    fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
  }

  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    _triangle(AX1,AY1,AX2,AY2,AX3,AY3);
    _fill();
  }

  //void fillPolygon(int32 ANum, float* ACoords) override {
  //  if (ANum > 0) {
  //    _moveTo(ACoords[0],ACoords[1]);
  //    for (int32 i=1; i<ANum; i++) {
  //      _lineTo(ACoords[i*2],ACoords[i*2+1]);
  //    }
  //    _fill();
  //  }
  //}

  //void fillCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {}
  //void fillCurve(KPoint AP1, KPoint AP2, KPoint AP3, KPoint AP4) override {}

  /*
  void fillRectangleGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
    _rectangle(AX1,AY1,AX2,AY2);
    _fillGradient(AX1,AY1,AX2,AY2,AColor1,AColor2,AVertical);
  }

  void fillRectangleGradient(KODE_Rect ARect, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
    fillRectangleGradient(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AColor1,AColor2,AVertical);
  }

  void fillRoundedRectangleGradient(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
    _roundedRectangle(AX1,AY1,AX2,AY2,AR,ACorners);
    _fillGradient(AX1,AY1,AX2,AY2,AColor1,AColor2,AVertical);
  }

  void fillRoundedRectangleGradient(KODE_Rect ARect, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
    fillRoundedRectangleGradient(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR,ACorners,AColor1,AColor2,AVertical);
  }
  */

  void drawText(float AX, float AY, const char* AText) override {
    cairo_move_to(MCairo,AX,AY);
    cairo_show_text(MCairo,AText);
  }

  void drawText(float AX1, float AY1, float AX2, float AY2, const char* AText, uint32_t AAlign) override {
    KODE_Assert(AText);
    cairo_text_extents_t e;
    float xx,yy;
    int32_t x = AX1;
    int32_t y = AY1;
    int32_t w = AX2-AX1+1;
    int32_t h = AY2-AY1+1;
    cairo_text_extents(MCairo,AText,&e);
    switch (AAlign) {
      case KODE_TEXT_ALIGN_LEFT:
        xx = x;
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      case KODE_TEXT_ALIGN_RIGHT:
        xx = (x+w-1) - (e.width + e.x_bearing);
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      case KODE_TEXT_ALIGN_TOP:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = y + e.height;
        break;
      case KODE_TEXT_ALIGN_BOTTOM:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = (y+h-1) - (e.height + e.y_bearing);
        break;
      case KODE_TEXT_ALIGN_CENTER:
        xx = (x + w/2) - (e.width/2  + e.x_bearing);
        yy = (y+h/2) - (e.height/2 + e.y_bearing);
        break;
      default:
        xx = AX1;
        yy = AY1;
    }
    cairo_move_to(MCairo,xx,yy);
    cairo_show_text(MCairo,AText);
  }

  void drawText(KODE_Rect ARect, const char* AText, uint32_t AAlign) override {
    drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
  }

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource) override {
    drawBitmap(ADstX,ADstY,ASource,0,0,ASource->getWidth(),ASource->getHeight());
  }

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) override {
    //if (ASource->isXimage()) {
    //  KXlibPainter::drawBitmap(ADstX,ADstY,ASource,ASrcX,ASrcY,ASrcW,ASrcH);
    //}
    //else
    if (ASource->isCairo()) {
      cairo_set_source_surface(MCairo,ASource->getCairoSurface(),/*0,0*/ADstX-ASrcX,ADstY-ASrcY);
      cairo_rectangle(MCairo,ADstX,ADstY,ASrcW,ASrcH);
      cairo_fill(MCairo);
    }
  }

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, KODE_Rect ASrc) override {
    drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
  }

  void drawBitmap(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) override {
    if (ASource->isCairo()) {
      float xscale = (float)ADstW / (float)ASrcW;
      float yscale = (float)ADstH / (float)ASrcH;
      cairo_rectangle(MCairo,ADstX,ADstY,ADstW,ADstH);
      cairo_save(MCairo);
      cairo_translate(MCairo,ADstX,ADstY);
      cairo_scale(MCairo,xscale,yscale);
      cairo_set_source_surface(MCairo,ASource->getCairoSurface(),0,0/*ASrcX,ASrcY*/);
      cairo_fill(MCairo);
      cairo_restore(MCairo);
    }
  }

  void drawBitmap(KODE_Rect ADst, KODE_Drawable* ASource, KODE_Rect ASrc) override {
    drawBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
  }

  /*
  - After cairo_clip(), the current path will be cleared from the cairo
    context.
  - Calling cairo_clip() can only make the clip region smaller,
    never larger.
  */

  void clip(float AX1, float AY1, float AX2, float AY2) override {
    //KODE_Trace("CLIP x %.0f y %.0f w %.0f h %.0f\n",AX1,AY1,AX2-AX1,AY2-AY1);
    cairo_reset_clip(MCairo);
    cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
    cairo_clip(MCairo);
    //cairo_new_path(MCairo); // path not consumed by clip()
  }

  void clip(KODE_Rect ARect) override {
    clip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  void resetClip(void) override {
    //KODE_Trace("RESET CLIP\n");
    cairo_reset_clip(MCairo);
  }
  
  //------------------------------
  
  void pushClip(KODE_Rect ARect) override {
    //KODE_Trace("pushing x %.0f y %.0f w %.0f h %.0f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
    MClipStack.push(MClipRect);
    MClipRect = ARect;
    resetClip();
    clip(MClipRect);
  }
  
  //----------

  void pushClip(float AX1, float AY1, float AX2, float AY2) override {
    pushClip( KODE_Rect(AX1,AY1,AX2-AX1+1,AY2-AY1+1) );
  }
  
  //----------

  void popClip() override {
    MClipRect = MClipStack.pop();
    resetClip();
    clip(MClipRect);
  }
  
  //----------
  
  KODE_Rect getClipRect() override {
    return MClipRect;
  }

};

//----------------------------------------------------------------------
#endif
