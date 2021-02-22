#ifndef kode_ctx_painter_included
#define kode_ctx_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_painter.h"
#include "gui/ctx/kode_ctx.h"
#include "gui/ctx/kode_ctx_utils.h"

//----------------------------------------------------------------------

/*
const
ctx_line_cap_t kode_line_cap[3] = {
  CAIRO_LINE_CAP_BUTT,
  CAIRO_LINE_CAP_ROUND,
  CAIRO_LINE_CAP_SQUARE
};

const
ctx_line_join_t kode_line_join[3] = {
  CAIRO_LINE_JOIN_MITER,
  CAIRO_LINE_JOIN_ROUND,
  CAIRO_LINE_JOIN_BEVEL
};
*/

/*
  typedef enum
  {
    CTX_FORMAT_GRAY8,
    CTX_FORMAT_GRAYA8,
    CTX_FORMAT_RGB8,
    CTX_FORMAT_RGBA8,
    CTX_FORMAT_BGRA8,
    CTX_FORMAT_RGB565,
    CTX_FORMAT_RGB565_BYTESWAPPED,
    CTX_FORMAT_RGB332,
    CTX_FORMAT_RGBAF,
    CTX_FORMAT_GRAYF,
    CTX_FORMAT_GRAY1,
    CTX_FORMAT_GRAY2,
    CTX_FORMAT_GRAY4
  } CtxPixelFormat;

  typedef enum
  {
    CTX_FILL_RULE_EVEN_ODD,
    CTX_FILL_RULE_WINDING
  } CtxFillRule;

  typedef enum
  {
    CTX_CAP_NONE,
    CTX_CAP_ROUND,
    CTX_CAP_SQUARE
  } CtxLineCap

  typedef enum
  {
    CTX_JOIN_BEVEL,
    CTX_JOIN_ROUND,
    CTX_JOIN_MITER
  } CtxLineJoin;
*/

//----------------------------------------------------------------------

class KODE_CtxPainter
: public KODE_BasePainter {
  
//------------------------------
  private:
//------------------------------

  Ctx*            MCtx    = KODE_NULL;
  KODE_Drawable*  MTarget = KODE_NULL;
  
//------------------------------
public:
//------------------------------

  KODE_CtxPainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {
    MTarget = ATarget;
    if (ATarget->isBitmap()) {
      KODE_Bitmap* bmp = (KODE_Bitmap*)ATarget;
      _new(bmp->getBuffer(),bmp->getWidth(),bmp->getHeight(),bmp->getStride(),CTX_FORMAT_BGRA8);
    }
    else {
      _new();
    }
  }
  
  //----------

  //----------

  virtual ~KODE_CtxPainter() {
    _free();
  }
  
//------------------------------
public:
//------------------------------

  virtual KODE_Drawable* getTarget() {
    return MTarget;
  }
  
//------------------------------
private:
//------------------------------

  void _new() {
    MCtx = ctx_new();
  }

  void _new(void* data, int width, int height, int stride, CtxPixelFormat pf) {
    MCtx = ctx_new_for_framebuffer(data,width,height,stride,pf);
  }

  void _free() {
    ctx_free(MCtx);
  }

  void _clear() {
    ctx_clear(MCtx);
  }
  
  void _empty() {
    ctx_empty(MCtx);
  }

  void _newPath() {
    ctx_new_path(MCtx);
  }

  void _closePath() {
    ctx_close_path(MCtx);
  }

  void _save() {
    ctx_save(MCtx);
  }

  void _restore() {
    ctx_restore(MCtx);
  }

  void _clip() {
    ctx_clip(MCtx);
  }

  void _translate(float x, float y) {
    ctx_translate(MCtx,x,y);
  }

  void _rotate(float x) {
    ctx_rotate(MCtx,x);
  }

  void _scale(float x, float y) {
    ctx_scale(MCtx,x,y);
  }

  void _fill() {
    ctx_fill(MCtx);
  }
  
  void _stroke() {
    ctx_stroke(MCtx);
  }
  
  void _paint() {
    ctx_paint(MCtx);
  }
  
  void _blit(void* d, int x, int y, int w, int h, int s, CtxPixelFormat pf) {
    ctx_blit(MCtx,d,x,y,w,h,s,pf);
  }
  
  int _loadTTF(const char *name, const uint8_t *ttf_contents) {
    return ctx_load_font_ttf(MCtx,name,ttf_contents);
  }
  
  int _loadFileTTF(const char *name, const char* path) {
    return ctx_load_font_ttf_file(MCtx,name,path);
  }
  
  void _setFont(const char* f) {
    ctx_set_font(MCtx,f);
  }
  
  void _setFontSize(float s) {
    ctx_set_font_size(MCtx,s);
  }
  
  void _setLineWidth(float w) {
    ctx_set_line_width(MCtx,w);
  }

  void _setRgba(float r, float g, float b, float a) {
    ctx_set_rgba(MCtx,r,g,b,a);
  }
  
  void _setRgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    ctx_set_rgba_u8(MCtx,r,g,b,a);
  }
  
  void _setRgb(float r, float g, float b) {
    ctx_set_rgb(MCtx,r,g,b);
  }
  
  void _setGray(float g) {
    ctx_set_gray(MCtx,g);
  }
  
  void _setGlobalAlpha(float a) {
    ctx_set_global_alpha(MCtx,a);
  }

  void _setFillRule(CtxFillRule rule) {
    ctx_set_fill_rule(MCtx,rule);
  }
  
  void _setLineCap(CtxLineCap cap) {
    ctx_set_line_cap(MCtx,cap);
  }
  
  void _setLineJoin(CtxLineJoin join) {
    ctx_set_line_join(MCtx,join);
  }
  
  void _getCurrentPoint(float* x, float * y) {
    ctx_current_point(MCtx,x,y);
  }
  
  float _getX() {
    return ctx_x(MCtx);
  }
  
  float _getY() {
    return ctx_y(MCtx);
  }

  void _moveTo(float x, float y) {
    ctx_move_to(MCtx,x,y);
  }
  
  void _lineTo(float x, float y) {
    ctx_line_to(MCtx,x,y);
  }
  
  void _curveTo(float cx0, float cy0, float cx1, float cy1,float x, float y) {
    ctx_curve_to(MCtx,cx0,cy0,cx1,cy1,x,y);
  }
  
  void _quadTo(float cx, float cy, float x, float y) {
    ctx_quad_to(MCtx,cx,cy,x,y);
  }
  
  void _moveToRel(float x, float y) {
    ctx_rel_move_to(MCtx,x,y);
  }
  
  void _lineToRel(float x, float y) {
    ctx_rel_line_to(MCtx,x,y);
  }
  
  void _curveToRel(float cx0, float cy0, float cx1, float cy1,float x, float y) {
    ctx_rel_curve_to(MCtx,cx0,cy0,cx1,cy1,x,y);
  }
  
  void _quadToRel(float cx, float cy, float x, float y) {
    ctx_rel_quad_to(MCtx,cx,cy,x,y);
  }
  
  void _arcTo(float x1, float y1, float x2, float y2, float r) {
    ctx_arc_to(MCtx,x1,y1,x2,y2,r);
  }

  void _setPixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    ctx_set_pixel_u8(MCtx,x,y,r,g,b,a);
  }
  
  void _rectangle(float x, float y, float w, float h) {
    ctx_rectangle(MCtx,x,y,w,h);
  }
  
  void _arc(float x, float y, float r, float a1, float a2, int d) {
    ctx_arc(MCtx,x,y,r,a1,a2,d);
  };
  
  void _linearGradient(float x0, float y0, float x1, float y1) {
    ctx_linear_gradient(MCtx,x0,y0,x1,y1);
  }
  
  void _radialGradient(float x0, float y0, float r0, float x1, float y1, float r1) {
    ctx_radial_gradient (MCtx,x0,y0,r0,x1,y1,r1);
  }
  
  void _gradientClearStops() {
    ctx_gradient_clear_stops(MCtx);
  }
  
  void _gradientAddStop(float pos, float r, float g, float b, float a) {
    ctx_gradient_add_stop(MCtx,pos,r,g,b,a);
  }
  
  void _gradientAddStop(float pos, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    ctx_gradient_add_stop_u8(MCtx,pos,r,g,b,a);
  }
  
  void _text(const char* s) {
    ctx_text(MCtx,s);
  }
  
  void _textStroke(const char* s) {
    ctx_text_stroke(MCtx,s);
  }
  
  float _textWidth(const char *s) {
    return ctx_text_width(MCtx,s);
  }

  float _textHeight(const char *s) {
    //return ctx_text_width(MCtx,s);
    CtxState *state = &MCtx->state;
    return state->gstate.font_size * state->gstate.line_spacing;  
  }

//------------------------------
public:
//------------------------------

  void flush() override {
  }
  
  //----------
  
  void dirty() override {
  }
  
  //----------
  
  void dirty(float AX1, float AY1, float AX2, float AY2) override {
  }
  
  //----------
  
  void dirty(KODE_Rect ARect) override {
  }
  
  //----------
  
  void finish() override {
  }
  
  //----------
  
  void resize(float w, float h) override {
  }
  
  //----------
  
  void setDrawColor(KODE_Color AColor) {
    _setRgba(AColor.r,AColor.g,AColor.b,AColor.a);
  }
    
  //----------
  
  void setFillColor(KODE_Color AColor) override {
    _setRgba(AColor.r,AColor.g,AColor.b,AColor.a);
  }

  //----------
  
  void setTextColor(KODE_Color AColor) override {
    _setRgba(AColor.r,AColor.g,AColor.b,AColor.a);
  }

  //----------
  
  void setPenSize(float ASize) override {
    _setLineWidth(ASize);
  }
  
  //----------
  
  void setPenDash(double* ADashes, uint32_t ANumDashes, float AOffset) override {
  }
  
  //----------
  
  void setLineCap(uint32_t ALineCap) override {
    //_setLineCap();
  }
  
  //----------
  
  void setLineJoin(uint32_t ALineJoin) override {
    //_setLineJoin();
  }
  
  //----------
  
  void setTextSize(float ASize) override {
    _setFontSize(ASize);
  }
  
  //----------
  
  void setTextFont(const char* AName, uint32_t ASlant, uint32_t AWeight) override {
  }

  //----------
  
  float getTextWidth(const char* AText) {
    return 0;
  }
  
  //----------
  
  float getTextHeight(const char* AText) {
    return 0;
  }
  
//------------------------------

  void drawPoint(float AX, float AY) override {
  }
  
  //----------
  
  void drawHLine(float AX1, float AY1, float AX2) override {
  }
  
  //----------
  
  void drawVLine(float AX1, float AY1, float AY2) override {
  }
  
  //----------
  
  void drawLine(float AX1, float AY1, float AX2, float AY2) override {
    _moveTo(AX1,AY1);
    _lineTo(AX2,AY2);
    _stroke();
  }
  
  //----------
  
  //void drawLines(int32_t ANum, int32_t* ACoords) override {
  //}
  
  //----------
  
  void drawRectangle(float AX1, float AY1, float AX2, float AY2) override {
    float w = (AX2 - AX1) + 1.0f;
    float h = (AY2 - AY1) + 1.0f;
    _rectangle(AX1,AY1,w,h);
    _stroke();
  }
  
  //----------
  
  void drawRectangle(KODE_Rect ARect) override {
    drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }
  
  //----------
  
  void drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) override {
  }
  
  //----------
  
  void drawRoundedRectangle(KODE_Rect ARect, float AR, uint32_t AC) override {
  }
  
  //----------
  
  void drawEllipse(float AX1, float AY1, float AX2, float AY2) override {
    float w05 = (AX2 - AX1 + 1.0f) * 0.5f;
    if (w05 <= 0.0f) return;
    float h05 = (AY2 - AY1 + 1.0f) * 0.5f;
    if (h05 <= 0.0f) return;
    float ratio = h05 / w05;
    //KODE_Trace("w05 %.3f h05 %.3f ratio %.3f\n",w05,h05,ratio);
    _save();
    _translate(AX1+w05,AY1+h05);
    _scale(1.0f,ratio);
    //_newPath(); // ???
    _arc(
      0,        // x
      0,        // y
      w05,//1,  // r
      0,        // a1
      KODE_PI2, // a2
      0         // dir
    );
    _stroke();
    _restore();
  }
  
  //----------
  
  void drawEllipse(KODE_Rect ARect) override {
    drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }
  
  //----------
  
  void drawArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    float w05 = (AX2 - AX1 + 1.0f) * 0.5f;
    if (w05 <= 0.0f) return;
    float h05 = (AY2 - AY1 + 1.0f) * 0.5f;
    if (h05 <= 0.0f) return;
    float ratio = h05 / w05;
    float a1 = (AAngle1+0.75) * KODE_PI2;
    float a2 = (AAngle1+AAngle2+0.75) * KODE_PI2;
    //KODE_Trace("w05 %.3f h05 %.3f ratio %.3f\n",w05,h05,ratio);
    _save();
    _translate(AX1+w05,AY1+h05);
    _scale(1.0f,ratio);
    //_newPath(); // ???
    _arc(
      0,        // x
      0,        // y
      w05,//1,  // r
      a1,       // a1
      a2,       // a2
      0         // dir
    );
    _stroke();
    _restore();
  }
  
  //----------
  
  void drawArc(KODE_Rect ARect, float AAngle1, float AAngle2) override {
    drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
  }
  
  //----------
  
  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    _moveTo(AX1,AY1);
    _lineTo(AX2,AY2);
    _lineTo(AX3,AY3);
    _lineTo(AX1,AY1);
    _stroke();
  }
  
  //----------
  
  void drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
  }

//------------------------------

  void fillRectangle(float AX1, float AY1, float AX2, float AY2) override {
    float w = (AX2 - AX1) + 1.0f;
    float h = (AY2 - AY1) + 1.0f;
    _rectangle(AX1,AY1,w,h);
    _fill();
  }

  //----------
  
  void fillRectangle(KODE_Rect ARect) override {
    drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------
  
  void fillRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) override {
  }

  //----------
  
  void fillRoundedRectangle(KODE_Rect ARect, float AR, uint32_t ACorners) override {
  }

  //----------
  
  void fillEllipse(float AX1, float AY1, float AX2, float AY2) override {
    float w05 = (AX2 - AX1 + 1.0f) * 0.5f;
    if (w05 <= 0.0f) return;
    float h05 = (AY2 - AY1 + 1.0f) * 0.5f;
    if (h05 <= 0.0f) return;
    float ratio = h05 / w05;
    //KODE_Trace("w05 %.3f h05 %.3f ratio %.3f\n",w05,h05,ratio);
    _save();
    _translate(AX1+w05,AY1+h05);
    _scale(1.0f,ratio);
    //_newPath(); // ???
    _arc(
      0,        // x
      0,        // y
      w05,//1,  // r
      0,        // a1
      KODE_PI2, // a2
      0         // dir
    );
    _fill();
    _restore();
  }

  //----------
  
  void fillEllipse(KODE_Rect ARect) override {
    fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------
  
  void fillArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    float w05 = (AX2 - AX1 + 1.0f) * 0.5f;
    if (w05 <= 0.0f) return;
    float h05 = (AY2 - AY1 + 1.0f) * 0.5f;
    if (h05 <= 0.0f) return;
    float ratio = h05 / w05;
    float a1 = (AAngle1+0.75) * KODE_PI2;
    float a2 = (AAngle1+AAngle2+0.75) * KODE_PI2;
    //KODE_Trace("w05 %.3f h05 %.3f ratio %.3f\n",w05,h05,ratio);
    _save();
    _translate(AX1+w05,AY1+h05);
    _scale(1.0f,ratio);
    //_newPath(); // ???
    _arc(
      0,        // x
      0,        // y
      w05,//1,  // r
      a1,       // a1
      a2,       // a2
      0         // dir
    );
    _fill();
    _restore();
  }

  //----------
  
  void fillArc(KODE_Rect ARect, float AAngle1, float AAngle2) override {
    fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
  }

  //----------
  
  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    _moveTo(AX1,AY1);
    _lineTo(AX2,AY2);
    _lineTo(AX3,AY3);
    _lineTo(AX1,AY1);
    _fill();
  }

//------------------------------

  //void fillPolygon(int32_t ANum, int32_t* ACoords) override {}
  //void fillCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {}
  //void fillRectangleGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {}
  //void fillRectangleGradient(KODE_Rect ARect, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) override {}
  //void fillRoundedRectangleGradient(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) override {}
  //void fillRoundedRectangleGradient(KODE_Rect ARect, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) override {}
  
//------------------------------

  void drawText(float AX, float AY, const char* AText) override {
    _moveTo(AX,AY);
    //_textStroke(AText);
    _text(AText);
  }

  //----------
  
  void drawText(float AX1, float AY1, float AX2, float AY2, const char* AText, uint32_t AAlign) override {
    float xx,yy;
    int32_t x = AX1;
    int32_t y = AY1;
    int32_t w = AX2-AX1+1;
    int32_t h = AY2-AY1+1;
    float tw = _textWidth(AText);
    float th = _textHeight(AText);
    switch (AAlign) {
      case KODE_TEXT_ALIGN_LEFT:
        xx = x;
        yy = (y + h/2) + th/2;
        break;
      case KODE_TEXT_ALIGN_RIGHT:
        xx = (x+w-1) - tw;
        yy = (y + h/2) + th/2;
        break;
      case KODE_TEXT_ALIGN_TOP:
        xx = (x + w/2) - tw/2;
        yy = y + th;
        break;
      case KODE_TEXT_ALIGN_BOTTOM:
        xx = (x + w/2) - tw/2;
        yy = (y+h-1);
        break;
      case KODE_TEXT_ALIGN_CENTER:
        xx = (x + w/2) - tw/2;
        yy = (y + h/2) + th/2;
        break;
      default:
        xx = AX1;
        yy = AY1;
    }
    _moveTo(xx,yy);
    _text(AText);
  }

  //----------
  
  void drawText(KODE_Rect ARect, const char* AText, uint32_t AAlign) override {
    drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
  }

//------------------------------

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource) override {
  }

  //----------
  
  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) override {
  }

  //----------
  
  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, KODE_Rect ASrc) override {
  }

  //----------
  
  void drawBitmap(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) override {
  }

  //----------
  
  void drawBitmap(KODE_Rect ADst, KODE_Drawable* ASource, KODE_Rect ASrc) override {
  }
  
//------------------------------

  void clip(float AX1, float AY1, float AX2, float AY2) override {
  }

  //----------
  
  void clip(KODE_Rect ARect) override {
  }

  //----------
  
  void resetClip(void) override {
  }

  //----------
  
  void pushClip(float AX1, float AY1, float AX2, float AY2) override {
  }

  //----------
  
  void pushClip(KODE_Rect ARect) override {
  }

  //----------
  
  void popClip()  override {
  }
  
};

//----------------------------------------------------------------------
#endif

