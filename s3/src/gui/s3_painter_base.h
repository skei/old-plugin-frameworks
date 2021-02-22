#ifndef s3_painter_base_included
#define s3_painter_base_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include "common/s3_point.h"
#include "common/s3_rect.h"

//----------

class S3_Painter_Base {
  public:
    S3_Painter_Base(S3_Drawable* ADrawable);
    virtual ~S3_Painter_Base();
  //public:
  //  virtual void  flush(void) {}
  public:
    virtual void  setDrawColor(S3_Color AColor) {}
    virtual void  setFillColor(S3_Color AColor) {}
    virtual void  setTextColor(S3_Color AColor) {}
    virtual void  setPenSize(int32 ASize) {}
    virtual void  setPenDash(const double* ADashes, int32 ANumDashes, double AOffset) {}
    virtual void  setLineCap(int32 ALineCap) {}
    virtual void  setLineJoin(int32 ALineJoin) {}
    virtual void  setTextSize(int32 ASize) {}
    virtual void  setTextFont(const char* AName, int32 ASlant, int32 AWeight) {}
    virtual int32 getTextWidth(const char* AText) { return 0;}
    virtual int32 getTextHeight(const char* AText) { return 0; }
  public:
    virtual void  drawPoint(int32 AX, int32 AY) {}
    virtual void  drawHLine(int32 AX1, int32 AY1, int32 AX2) {}
    virtual void  drawVLine(int32 AX1, int32 AY1, int32 AY2) {}
    virtual void  drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void  drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void  drawRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) {}
    virtual void  drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void  drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {}
    virtual void  drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {}
    virtual void  drawCurve(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3, int32 AX4, int32 AY4) {}
  public:
    virtual void  fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void  fillRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) {}
    virtual void  fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void  fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {}
    virtual void  fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {}
    virtual void  fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AColor1, S3_Color AColor2, bool AVertical=true) {}
  public:
    virtual void  drawText(int32 AX, int32 AY, const char* AText) {}
    virtual void  drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {}
  public:
    virtual void  drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void  stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
  public:
    virtual void  clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void  resetClip(void) {}
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Painter_Base::S3_Painter_Base(S3_Drawable* ADrawable) {
}

//----------

S3_Painter_Base::~S3_Painter_Base() {
}

//----------------------------------------------------------------------
#endif
