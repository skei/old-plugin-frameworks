#ifndef s3_painter_cairo_included
#define s3_painter_cairo_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include "common/s3_point.h"
#include "common/s3_rect.h"
#include "gui/s3_painter_xlib.h"

//----------

class S3_Painter_Cairo
: public S3_Painter_Xlib {
  private:
    cairo_surface_t*  MCairoSurface;
    cairo_t*          MCairo;
  public:
    S3_Painter_Cairo(S3_Drawable* ADrawable);
    virtual ~S3_Painter_Cairo();
  public:
    bool              isCairo(void)       { return true; }
    cairo_t*          cairo(void)         { return MCairo; }
    cairo_surface_t*  cairoSurface(void)  { return MCairoSurface; }
  public:
    void  setDrawColor(S3_Color AColor) override;
    void  setFillColor(S3_Color AColor) override;
    void  setTextColor(S3_Color AColor) override;
    void  setPenSize(int32 ASize) override;
    void  setPenDash(const double* ADashes, int32 ANumDashes, double AOffset) override;
    void  setLineCap(int32 ALineCap) override;
    void  setLineJoin(int32 ALineJoin) override;
    void  setTextSize(int32 ASize) override;
    void  setTextFont(const char* AName, int32 ASlant, int32 AWeight) override;
    int32 getTextWidth(const char* AText) override;
    int32 getTextHeight(const char* AText) override;
  public:
    void  drawPoint(int32 AX, int32 AY) override;
    void  drawHLine(int32 AX1,int32 AY1,int32 AX2) override;
    void  drawVLine(int32 AX1, int32 AY1, int32 AY2) override;
    void  drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void  drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void  drawRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) override;
    void  drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void  drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) override;
    void  drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) override;
    void  drawCurve(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3, int32 AX4, int32 AY4) override;
  public:
    void  fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void  fillRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) override;
    void  fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void  fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) override;
    void  fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) override;
    void  fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AColor1, S3_Color AColor2, bool AVertical=true) override;
  public:
    void  drawText(int32 AX, int32 AY, const char* AText) override;
    void  drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) override;
  public:
    void  drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) override;
    void  stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) override;
  public:
    void  clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void  resetClip(void) override;

  //----------

  public:

    void  drawPoint(S3_Point APoint)                                                { drawPoint(APoint.x,APoint.y);  }
    void  drawHLine(S3_Point APoint, int32 AX2)                                     { drawHLine(APoint.x,APoint.y,AX2); }
    void  drawVLine(S3_Point APoint, int32 AY2)                                     { drawHLine(APoint.x,APoint.y,AY2); }
    void  drawLine(S3_Point APoint1, S3_Point APoint2)                              { drawLine(APoint1.x,APoint1.y,APoint2.x,APoint2.y); }
    void  drawLine(S3_Rect ARect)                                                   { drawLine(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  drawRectangle(S3_Rect ARect)                                              { drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  drawRoundedRectangle(S3_Rect ARect, int32 AR)                             { drawRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR); }
    void  drawEllipse(S3_Rect ARect)                                                { drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  drawArc(S3_Rect ARect, float AAngle1, float AAngle2)                      { drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2); }
    void  fillRectangle(S3_Rect ARect)                                              { fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  fillRoundedRectangle(S3_Rect ARect, int32 AR)                             { fillRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR); }
    void  fillArc(S3_Rect ARect, float AAngle1, float AAngle2)                      { fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2); }
    void  fillEllipse(S3_Rect ARect, float AAngle1, float AAngle2)                  { fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  fillGradient(S3_Rect R, S3_Color C1, S3_Color C2, bool V=true)            { fillGradient(R.x,R.y,R.x2(),R.y2(),C1,C2,V); }
    void  drawText(S3_Point APoint, const char* AText)                              { drawText(APoint.x,APoint.y,AText);  }
    void  drawText(S3_Rect ARect, const char* AText, uint32 AAlign)                 { drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign); }
    void  drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc)  { drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h); }
    void  drawBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc)             { drawBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h); }
    void  stretchBitmap(S3_Rect ADst, S3_Drawable* ASource, S3_Rect ASrc)           { stretchBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h); }
    void  clip(S3_Rect ARect)                                                       { clip(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
};

//----------

cairo_font_slant_t s3_font_slant[3] = {
  CAIRO_FONT_SLANT_NORMAL,
  CAIRO_FONT_SLANT_ITALIC,
  CAIRO_FONT_SLANT_OBLIQUE
};

cairo_font_weight_t s3_font_weight[2] = {
    CAIRO_FONT_WEIGHT_NORMAL,
    CAIRO_FONT_WEIGHT_BOLD
};

cairo_line_cap_t s3_line_cap[3] = {
  CAIRO_LINE_CAP_BUTT,
  CAIRO_LINE_CAP_ROUND,
  CAIRO_LINE_CAP_SQUARE
};

cairo_line_join_t s3_line_join[3] = {
  CAIRO_LINE_JOIN_MITER,
  CAIRO_LINE_JOIN_ROUND,
  CAIRO_LINE_JOIN_BEVEL
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Painter_Cairo::S3_Painter_Cairo(S3_Drawable* ADrawable)
: S3_Painter_Xlib(ADrawable) {
  MCairoSurface = ADrawable->cairoSurface();
  MCairo = cairo_create(MCairoSurface);
  setPenSize(1);
  setTextFont("sans",s3_fs_normal,s3_fw_normal);
  setTextSize(12);

  //MBlendMode = PictOpOver;
  //cairo_set_operator(MCairo,CAIRO_OPERATOR_OVER);

}


//----------

S3_Painter_Cairo::~S3_Painter_Cairo() {
  cairo_destroy(MCairo);
}

//----------------------------------------------------------------------

void S3_Painter_Cairo::setDrawColor(S3_Color AColor) {
  S3_Painter_Xlib::setDrawColor(AColor);
  cairo_set_source_rgba(MCairo,MDrawColor.r,MDrawColor.g,MDrawColor.b,MDrawColor.a);
}

//----------

void S3_Painter_Cairo::setFillColor(S3_Color AColor) {
  S3_Painter_Xlib::setFillColor(AColor);
  cairo_set_source_rgba(MCairo,MFillColor.r,MFillColor.g,MFillColor.b,MFillColor.a);
}

//----------

void S3_Painter_Cairo::setTextColor(S3_Color AColor) {
  S3_Painter_Xlib::setTextColor(AColor);
  cairo_set_source_rgba(MCairo,MTextColor.r,MTextColor.g,MTextColor.b,MTextColor.a);
}

//----------

void S3_Painter_Cairo::setPenSize(int32 ASize) {
  S3_Painter_Xlib::setPenSize(ASize);
  cairo_set_line_width(MCairo,ASize);
}

//----------

void S3_Painter_Cairo::setPenDash(const double* ADashes, int32 ANumDashes, double AOffset) {
  cairo_set_dash(MCairo,ADashes,ANumDashes,AOffset);
}

//----------

void S3_Painter_Cairo::setLineCap(int32 ALineCap) {
  cairo_set_line_cap(MCairo,s3_line_cap[ALineCap]);
}

//----------

void S3_Painter_Cairo::setLineJoin(int32 ALineJoin) {
  cairo_set_line_join(MCairo,s3_line_join[ALineJoin]);
}

//----------

void S3_Painter_Cairo::setTextSize(int32 ASize) {
  //S3_Painter_Xlib::setTextSize(ASize);
  cairo_set_font_size(MCairo,ASize);
}

//----------

void S3_Painter_Cairo::setTextFont(const char* AName, int32 ASlant, int32 AWeight) {
  //S3_Painter_Xlib::setTextFont(AName,ASlant,AWeight);
  cairo_select_font_face(MCairo,AName,s3_font_slant[ASlant],s3_font_weight[AWeight]);
}

//----------

int32 S3_Painter_Cairo::getTextWidth(const char* AText) {
  cairo_text_extents_t e;
  cairo_text_extents(MCairo,AText,&e);
  return e.width;
}

//----------

int32 S3_Painter_Cairo::getTextHeight(const char* AText) {
  cairo_text_extents_t e;
  cairo_text_extents(MCairo,AText,&e);
  return e.height;
}

//----------------------------------------------------------------------

void S3_Painter_Cairo::drawPoint(int32 AX, int32 AY) {
  cairo_rectangle(MCairo,AX,AY,1,1);
  cairo_fill(MCairo);
}

//----------

/*
  "How do I draw a sharp, single-pixel-wide line?"
  http://www.cairographics.org/FAQ/#sharp_lines
*/

void S3_Painter_Cairo::drawHLine(int32 AX1, int32 AY1, int32 AX2) {
  cairo_move_to(MCairo,AX1,AY1+0.5);
  cairo_line_to(MCairo,AX2,AY1+0.5);
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawVLine(int32 AX1, int32 AY1, int32 AY2) {
  cairo_move_to(MCairo,AX1+0.5,AY1);
  cairo_line_to(MCairo,AX1+0.5,AY2);
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  cairo_move_to(MCairo,AX1,AY1);
  cairo_line_to(MCairo,AX2,AY2);
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) {
  int32 x = AX1;
  int32 y = AY1;
  int32 w = AX2-AX1+1;
  int32 h = AY2-AY1+1;
  int32 r = AR;
  float degrees = S3_PI / 180.0;
  cairo_new_sub_path(MCairo);
  cairo_arc(MCairo,x + w-r, y+r,   r, -90  * degrees, 0   * degrees);
  cairo_arc(MCairo,x + w-r, y+h-r, r,  0   * degrees, 90  * degrees);
  cairo_arc(MCairo,x + r,   y+h-r, r,  90  * degrees, 180 * degrees);
  cairo_arc(MCairo,x + r,   y+r,   r,  180 * degrees, 270 * degrees);
  cairo_close_path(MCairo);
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
  float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
  //cairo_new_sub_path(FCairo);
  cairo_save(MCairo);
  cairo_translate(MCairo,AX1+w2,AY1+h2);
  cairo_scale(MCairo,w2,h2);
  cairo_new_sub_path(MCairo);
  cairo_arc(MCairo,0,0,1,0,S3_PI2);
  cairo_restore(MCairo);
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
  float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
  float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
  float a1 = (AAngle1+0.75) * S3_PI2;
  float a2 = (AAngle1+AAngle2+0.75) * S3_PI2;
  cairo_save(MCairo);
  cairo_translate(MCairo,AX1+w2,AY1+h2);
  cairo_scale(MCairo,w2,h2);
  cairo_new_sub_path(MCairo);
  cairo_arc(MCairo,0,0,1,a1,a2);
  cairo_restore(MCairo);
  cairo_stroke(MCairo);
}

//----------

void S3_Painter_Cairo::drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
  drawLine(AX1,AY1,AX2,AY2);
  drawLine(AX2,AY2,AX3,AY3);
  drawLine(AX3,AY3,AX1,AY1);
}

//----------

void S3_Painter_Cairo::drawCurve(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3, int32 AX4, int32 AY4) {
  cairo_move_to(MCairo,AX1,AY1);
  cairo_curve_to(MCairo,AX2,AY2,AX3,AY3,AX4,AY4);
  cairo_stroke(MCairo);
}

//----------------------------------------------------------------------

void S3_Painter_Cairo::fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
  cairo_fill(MCairo);
}

//----------

void S3_Painter_Cairo::fillRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) {
  int32 x = AX1;
  int32 y = AY1;
  int32 w = AX2-AX1+1;
  int32 h = AY2-AY1+1;
  int32 r = AR;
  float degrees = S3_PI / 180.0;
  cairo_new_sub_path(MCairo);
  cairo_arc(MCairo,x + w-r, y+r,   r, -90  * degrees, 0   * degrees);
  cairo_arc(MCairo,x + w-r, y+h-r, r,  0   * degrees, 90  * degrees);
  cairo_arc(MCairo,x + r,   y+h-r, r,  90  * degrees, 180 * degrees);
  cairo_arc(MCairo,x + r,   y+r,   r,  180 * degrees, 270 * degrees);
  cairo_close_path(MCairo);
  cairo_fill(MCairo);
}


//----------

void S3_Painter_Cairo::fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
  float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
  cairo_save(MCairo);
  cairo_translate(MCairo,AX1+w2,AY1+h2);
  cairo_scale(MCairo,w2,h2);
  cairo_new_sub_path(MCairo);
  cairo_arc(MCairo,0,0,1,0,S3_PI2);
  cairo_restore(MCairo);
  cairo_fill(MCairo);
}

//----------

void S3_Painter_Cairo::fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
  float w2 = (float)(AX2 - AX1 + 1) * 0.5f;
  float h2 = (float)(AY2 - AY1 + 1) * 0.5f;
  float a1 = (AAngle1+0.75f) * S3_PI2;
  float a2 = (AAngle1+AAngle2+0.75f) * S3_PI2;
  cairo_move_to(MCairo,AX1+w2,AY1+h2);
  cairo_save(MCairo);
  cairo_translate(MCairo,AX1+w2,AY1+h2);
  cairo_scale(MCairo,w2,h2);
  cairo_arc(MCairo,0,0,1,a1,a2);
  cairo_restore(MCairo);
  cairo_fill(MCairo);
}

//----------

void S3_Painter_Cairo::fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
  cairo_move_to(MCairo,AX1,AY1);
  cairo_line_to(MCairo,AX2,AY2);
  cairo_line_to(MCairo,AX3,AY3);
  cairo_line_to(MCairo,AX1,AY1);
  cairo_fill(MCairo);
}

//----------

void S3_Painter_Cairo::fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AColor1, S3_Color AColor2, bool AVertical) {
  cairo_pattern_t *pat;
  if (AVertical) pat = cairo_pattern_create_linear( AX1,AY1, AX1,AY2 );
  else pat = cairo_pattern_create_linear( AX1,AY1, AX2,AY1 );
  cairo_pattern_add_color_stop_rgba( pat, 0, AColor1.r, AColor1.g, AColor1.b, AColor1.a );
  cairo_pattern_add_color_stop_rgba( pat, 1, AColor2.r, AColor2.g, AColor2.b, AColor2.a );
  cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
  cairo_set_source(MCairo,pat);
  cairo_fill(MCairo);
  cairo_pattern_destroy(pat);
}

//----------------------------------------------------------------------

void S3_Painter_Cairo::drawText(int32 AX, int32 AY, const char* AText) {
  cairo_move_to(MCairo,AX,AY);
  cairo_show_text(MCairo,AText);
}

//----------

void S3_Painter_Cairo::drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {
  //e : PKTextExtents;
  cairo_text_extents_t e;
  float xx,yy;
  int32 x = AX1;
  int32 y = AY1;
  int32 w = AX2-AX1+1;
  int32 h = AY2-AY1+1;
  cairo_text_extents(MCairo,AText,&e);
  //ACairo.moveTo(R.x + R.w/2 - ext^.width/2 , R.y + R.h/2 + ext^.height/2 );
  switch (AAlign) {
    case s3_ta_left:
      xx = x;
      yy = (y+h/2) - (e.height/2 + e.y_bearing);
      break;
    case s3_ta_right:
      xx = (x+w-1) - (e.width + e.x_bearing);
      yy = (y+h/2) - (e.height/2 + e.y_bearing);
      break;
    case s3_ta_top:
      xx = (x + w/2) - (e.width/2  + e.x_bearing);
      yy = y + e.height;
      break;
    case s3_ta_bottom:
      xx = (x + w/2) - (e.width/2  + e.x_bearing);
      yy = (y+h-1) - (e.height + e.y_bearing);
      break;
    case s3_ta_center:
      xx = (x + w/2) - (e.width/2  + e.x_bearing);
      yy = (y+h/2) - (e.height/2 + e.y_bearing);
      break;
    default:
      xx = AX1;
      yy = AY1;
  }
  /*xx := (x + w/2) - (e^.width/2  + e^.x_bearing);*/
  /*yy := (y + h/2) - (e^.height/2 + e^.y_bearing);*/
  //_moveTo(xx,yy);
  cairo_move_to(MCairo,xx,yy);
  //_showText(AText);
  cairo_show_text(MCairo,AText);
}

//----------------------------------------------------------------------

void S3_Painter_Cairo::drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
  if (ASource->isCairo()) {
    //cairo_pattern_t* pattern;
    cairo_set_source_surface(MCairo,ASource->cairoSurface(),/*0,0*/ADstX-ASrcX,ADstY-ASrcY);
    cairo_rectangle(MCairo,ADstX,ADstY,ASrcW,ASrcH);
    cairo_fill(MCairo);
    /*cairo_save(FCairo);
    cairo_translate(FCairo,ASrcX,ASrcY);
    pattern := cairo_pattern_create_for_surface(ASurface._surface);
    cairo_pattern_set_extend(pattern,CAIRO_EXTEND_PAD);
    cairo_set_source(FCairo,pattern);
    cairo_rectangle(FCairo,ADstX,ADstY{0,0},ASrcW,ASrcH);
    cairo_fill(FCairo);
    cairo_pattern_destroy(pattern);
    cairo_restore(FCairo);*/
  }
  else {
    S3_Painter_Xlib::drawBitmap(ADstX,ADstY,ASource,ASrcX,ASrcY,ASrcW,ASrcH);
  }
}

void S3_Painter_Cairo::stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
  float xscale = ADstW / ASrcW;
  float yscale = ADstH / ASrcH;
  cairo_rectangle(MCairo,ADstX,ADstY,ADstW,ADstH);
  cairo_save(MCairo);
  cairo_translate(MCairo,ADstX,ADstY);
  cairo_scale(MCairo,xscale,yscale);
  cairo_set_source_surface(MCairo,ASource->cairoSurface(),0,0/*ASrcX,ASrcY*/);
  //cairo_paint(FCairo);
  //cairo_rectangle(FCairo,ADstX,ADstY,ADstW,ADstH);
  cairo_fill(MCairo);
  cairo_restore(MCairo);
}

//----------------------------------------------------------------------

void S3_Painter_Cairo::clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  /*
  cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
  cairo_clip(MCairo);
  */
  cairo_reset_clip(MCairo);
  cairo_rectangle(MCairo,AX1,AY1,(AX2-AX1+1),(AY2-AY1+1));
  cairo_clip(MCairo);
  //cairo_clip_preserve(MCairo);
  cairo_new_path(MCairo); // path not consumed by clip()
}

//----------

void S3_Painter_Cairo::resetClip(void) {
  cairo_reset_clip(MCairo);
}

//----------------------------------------------------------------------
#endif

/*

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

----------

int w, h;
cairo_surface_t *image;
cairo_arc (cr, 128.0, 128.0, 76.8, 0, 2*M_PI);
cairo_clip (cr);
cairo_new_path (cr); // path not consumed by clip()
image = cairo_image_surface_create_from_png ("data/romedalen.png");
w = cairo_image_surface_get_width (image);
h = cairo_image_surface_get_height (image);
cairo_scale (cr, 256.0/w, 256.0/h);
cairo_set_source_surface (cr, image, 0, 0);
cairo_paint (cr);
cairo_surface_destroy (image);

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

*/
