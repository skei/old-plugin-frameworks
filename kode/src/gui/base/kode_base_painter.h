#ifndef kode_base_painter_included
#define kode_base_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

class KODE_BasePainter {
public:
  KODE_BasePainter(KODE_Drawable* ATarget) {}
  virtual ~KODE_BasePainter() {}
public:
  virtual KODE_Drawable* getTarget() { return nullptr; }
public:
  virtual void  flush() {}
  virtual void  dirty() {}
  virtual void  dirty(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  dirty(KODE_Rect ARect) {}
  virtual void  finish() {}
  virtual void  resize(float w, float h) {}

  virtual void  setDrawColor(KODE_Color AColor) {}
  virtual void  setFillColor(KODE_Color AColor) {}
  virtual void  setTextColor(KODE_Color AColor) {}
  virtual void  setPenSize(float ASize) {}
  virtual void  setPenDash(double* ADashes, uint32_t ANumDashes, float AOffset) {}
  virtual void  setLineCap(uint32_t ALineCap) {}
  virtual void  setLineJoin(uint32_t ALineJoin) {}
  virtual void  setTextSize(float ASize) {}
  virtual void  setTextFont(const char* AName, uint32_t ASlant, uint32_t AWeight) {}

  virtual float getTextWidth(const char* AText) { return 0; }
  virtual float getTextHeight(const char* AText) { return 0;}

  virtual void  drawPoint(float AX, float AY) {}
  virtual void  drawHLine(float AX1, float AY1, float AX2) {}
  virtual void  drawVLine(float AX1, float AY1, float AY2) {}
  virtual void  drawLine(float AX1, float AY1, float AX2, float AY2) {}
  //virtual void  drawLines(int32_t ANum, int32_t* ACoords) {}
  virtual void  drawRectangle(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  drawRectangle(KODE_Rect ARect) {}
  virtual void  drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) {}
  virtual void  drawRoundedRectangle(KODE_Rect ARect, float AR, uint32_t AC) {}
  virtual void  drawEllipse(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  drawEllipse(KODE_Rect ARect) {}
  virtual void  drawArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) {}
  virtual void  drawArc(KODE_Rect ARect, float AAngle1, float AAngle2) {}
  virtual void  drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) {}
  virtual void  drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) {}

  virtual void  fillRectangle(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  fillRectangle(KODE_Rect ARect) {}
  virtual void  fillRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) {}
  virtual void  fillRoundedRectangle(KODE_Rect ARect, float AR, uint32_t ACorners) {}
  virtual void  fillEllipse(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  fillEllipse(KODE_Rect ARect) {}
  virtual void  fillArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) {}
  virtual void  fillArc(KODE_Rect ARect, float AAngle1, float AAngle2) {}
  virtual void  fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) {}

  //virtual void  fillPolygon(int32_t ANum, int32_t* ACoords) {}
  //virtual void  fillCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) {}
  //virtual void  fillRectangleGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) {}
  //virtual void  fillRectangleGradient(KODE_Rect ARect, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) {}
  //virtual void  fillRoundedRectangleGradient(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) {}
  //virtual void  fillRoundedRectangleGradient(KODE_Rect ARect, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) {}
  
  virtual void  drawText(float AX, float AY, const char* AText) {}
  virtual void  drawText(float AX1, float AY1, float AX2, float AY2, const char* AText, uint32_t AAlign) {}
  virtual void  drawText(KODE_Rect ARect, const char* AText, uint32_t AAlign) {}

  virtual void  drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource) {}
  virtual void  drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) {}
  virtual void  drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, KODE_Rect ASrc) {}
  virtual void  drawBitmap(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) {}
  virtual void  drawBitmap(KODE_Rect ADst, KODE_Drawable* ASource, KODE_Rect ASrc) {}
  
  virtual void  clip(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  clip(KODE_Rect ARect) {}
  virtual void  resetClip(void) {}
  
  virtual void  pushClip(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  pushClip(KODE_Rect ARect) {}
  virtual void  popClip()  {}
  
  virtual KODE_Rect getClipRect() { return KODE_Rect(0); }

};

//----------------------------------------------------------------------
#endif

