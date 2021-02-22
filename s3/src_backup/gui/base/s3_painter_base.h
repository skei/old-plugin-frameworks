#ifndef s3_painter_base_included
#define s3_painter_base_included
//----------------------------------------------------------------------

#include "gui/s3_pen.h"
#include "gui/s3_brush.h"
#include "gui/s3_font.h"

//typedef S3_Stack<S3_Rect,S3_PAINTER_CLIPSTACK_SIZE> S3_ClipStack;

//----------------------------------------------------------------------

class S3_Painter_Base {

  public:

    S3_Painter_Base(S3_Drawable* ADrawable) {}
    virtual ~S3_Painter_Base() {}

  public:

    /*
      implemented in:
        s3_painter_gdi.h
    */

    virtual void lock(void) {}
    virtual void unlock(void) {}
    virtual void sync(void) {}
    virtual void flush(void) {}
    virtual void selectPen(S3_Pen* APen, bool ARemember=true) {}
    virtual void resetPen(void) {}
    virtual void selectBrush(S3_Brush* ABrush, bool ARemember=true) {}
    virtual void resetBrush(void) {}
    virtual void selectFont(S3_Font* AFont, bool ARemember=true) {}
    virtual void resetFont(void) {}
    virtual void selectDrawable(S3_Drawable* ADrawable, bool ARemember=true) {}
    virtual void resetDrawable(void) {}
    virtual void setDrawColor(S3_Color AColor) {}
    virtual void setFillColor(S3_Color AColor) {}
    virtual void setTextColor(S3_Color AColor) {}
    //virtual void setTextSize(int32 ASize) {}
    //virtual int32 getTextWidth(const char* AText) {}       // <--- ?????+
    virtual int32 getTextHeight(const char* AText) { return 0; }
    virtual void drawPoint(int32 AX, int32 AY) {}
    virtual void drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void drawRoundedRectangle(void) {}
    virtual void drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {}
    virtual void drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {}
    virtual void fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void fillRoundedRectangle(void) {}
    virtual void fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {}
    virtual void fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {}
    virtual void gradientRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AUpperLeft, S3_Color ALowerRight, bool AVertical=true) {}
    virtual void gradientTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3, S3_Color AColor1, S3_Color AColor2, S3_Color AColor3) {}
    virtual void blendRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void drawText(int32 AX, int32 AY, const char* AText) {}
    virtual void drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {}
    virtual void drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void maskBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH, S3_Drawable* AMask, int32 AMaskX, int32 AMaskY) {}
    virtual void tileBitmap(void) {}
    virtual void transparentBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH, S3_Color ATransparentColor) {}
    //virtual void blendBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {}
    virtual void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void noClip(void) {}

  public:

    /*
      implemented in:
        s3_painter.h
    */

    virtual void clipRect(int32 x, int32 y, int32 w, int32 h) {}
    virtual void pushClip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {}
    virtual void popClip(void) {}

  public:

    /*

    void drawPoint(S3_Point APoint) {
      this->drawPoint(APoint.x,APoint.y);
    }

    void drawRectangle(S3_Rect ARect) {
      this->drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void drawEllipse(S3_Rect ARect) {
      this->drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void drawArc(S3_Rect ARect, float AAngle1, float AAngle2) {
      this->drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    }

    //----------

    void fillRectangle(S3_Rect ARect) {
      this->fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void fillEllipse(S3_Rect ARect) {
      this->fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void fillArc(S3_Rect ARect, float AAngle1, float AAngle2) {
      this->fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    }

    //----------

    void gradientRectangle(S3_Rect ARect, S3_Color AUpperLeft, S3_Color ALowerRight, bool AVertical=false) {
      this->gradientRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AUpperLeft,ALowerRight,AVertical);
    }

    void gradientTriangle(S3_Point APoint1, S3_Point APoint2, S3_Point APoint3, S3_Color AColor1, S3_Color AColor2, S3_Color AColor3) {
      this->gradientTriangle(APoint1.x,APoint1.y,APoint2.x,APoint2.y,APoint3.x,APoint3.y,AColor1,AColor2,AColor3);
    }

    //----------

    void blendRectangle(S3_Rect ARect) {
      this->blendRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    //----------

    void drawText(S3_Point APoint, const char* AText) {
      this->drawText(APoint.x,APoint.y,AText);
    }

    void drawText(S3_Rect ARect, const char* AText, uint32 AAlign) {
      this->drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
    }

    //----------

    void drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc) {
      this->drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void drawBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->drawBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void maskBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc, S3_Drawable* AMask, int32 AMaskX, int32 AMaskY) {
      this->maskBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h,AMask,AMaskX,AMaskY);
    }

    void blendBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->blendBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void stretchBitmap(S3_Rect ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->stretchBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    //----------

    void clip(S3_Rect ARect) {
      this->clip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void pushClip(S3_Rect ARect) {
      pushClip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    */

};

//----------------------------------------------------------------------
#endif
