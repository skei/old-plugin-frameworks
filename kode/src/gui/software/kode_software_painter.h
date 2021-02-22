#ifndef kode_software_painter_included
#define kode_software_painter_included
//----------------------------------------------------------------------

// dda
// mid-point
// bresenham
// bresenham_aa
// wu_aa
// gupta_sproull_aa
//
// polyfiller

//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_painter.h"
#include "gui/software/kode_software.h"

//----------------------------------------------------------------------

class KODE_SoftwarePainter
: public KODE_BasePainter {
  
//------------------------------
private:
//------------------------------

  KODE_Drawable*  MTarget         = KODE_NULL;
  KODE_Bitmap*    MBitmap         = KODE_NULL;
  
  //KODE_Color      MDrawColor      = KODE_Black;
  //KODE_Color      MFillColor      = KODE_DarkGrey;
  //KODE_Color      MTextColor      = KODE_DarkGrey;
  
  uint32_t        MDrawColor      = 0xff000000;
  uint32_t        MFillColor      = 0xff808080;
  uint32_t        MTextColor      = 0xffc0c0c0;
  
  float           MPenSize        = 1.0f;
  double*         MPenDashes      = KODE_NULL;
  uint32_t        MPenNumDashes   = 0;
  float           MPenDashOffset  = 0.0f;
  uint32_t        MLineCap        = 0;
  uint32_t        MLineJoin       = 0;
  float           MTextSize       = 16.0f;
  const char*     MTextFont       = "";
  uint32_t        MTextSlant      = 0;
  uint32_t        MTextWeight     = 0;
  
//------------------------------
public:
//------------------------------

  KODE_SoftwarePainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {
    MTarget = ATarget;
    KODE_Assert( ATarget->isBitmap() );
    MBitmap = (KODE_Bitmap*)ATarget;
  }
  
  //----------

  virtual ~KODE_SoftwarePainter() {
  }
  
//------------------------------
public:
//------------------------------

  virtual KODE_Drawable* getTarget() { return MTarget; }
  
//------------------------------
//private: // bresenham
public:
//------------------------------

  void _line_br(int x0, int y0, int x1, int y1) {
    KODE_Assert(MBitmap);
    int dx =  abs(x1-x0);
    int sx = x0<x1 ? 1 : -1;
    int dy = -abs(y1-y0);
    int sy = y0<y1 ? 1 : -1;
    int err = dx+dy;
    int e2;
    for (;;) {
      MBitmap->setPixel(x0,y0,MDrawColor);
      e2 = 2 * err;
      if (e2 >= dy) {
        if (x0 == x1) break;
        err += dy;
        x0 += sx;
      }
      if (e2 <= dx) {
        if (y0 == y1) break;
        err += dx;
        y0 += sy;
      }
    }
  }
  
//------------------------------
//private: // bresenham AA
public:
//------------------------------

  void _lineAA_br(int x0, int y0, int x1, int y1) {
    KODE_Assert(MBitmap);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1, x2;
    long dx = abs(x1-x0), dy = abs(y1-y0), err = dx*dx+dy*dy;
    long e2 = err == 0 ? 1 : 0xffff7fl/sqrt(err);
    dx *= e2; dy *= e2; err = dx-dy;
    for (;;) {
      MBitmap->blendPixel(x0,y0,MDrawColor,255-(abs(err-dx+dy)>>16));
      e2 = err; x2 = x0;
      if (2*e2 >= -dx) {
        if (x0 == x1) break;
        if (e2+dy < 0xff0000l) MBitmap->blendPixel(x0,y0+sy,MDrawColor,255-((e2+dy)>>16));
        err -= dy; x0 += sx;
      }
      if (2*e2 <= dy) {
        if (y0 == y1) break;
        if (dx-e2 < 0xff0000l) MBitmap->blendPixel(x2+sx,y0,MDrawColor,255-((dx-e2)>>16));
        err += dx; y0 += sy;
      }
    }
  }
  
//------------------------------
//private: // wu
public:
//------------------------------

  #define ipart_(X) ((int)(X))
  #define round_(X) ((int)(((double)(X))+0.5))
  #define fpart_(X) (((double)(X))-(double)ipart_(X))
  #define rfpart_(X) (1.0-fpart_(X))
  #define swap_(a, b) do{ __typeof__(a) tmp;  tmp = a; a = b; b = tmp; }while(0)

  //-----
    
  void _lineAA_wu(uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2) {
    KODE_Assert(MBitmap);
    double dx = (double)x2 - (double)x1;
    double dy = (double)y2 - (double)y1;
    if ( fabs(dx) > fabs(dy) ) {
      if ( x2 < x1 ) {
        swap_(x1, x2);
        swap_(y1, y2);
      }
      double gradient = dy / dx;
      double xend = round_(x1);
      double yend = y1 + gradient*(xend - x1);
      double xgap = rfpart_(x1 + 0.5);
      int xpxl1 = xend;
      int ypxl1 = ipart_(yend);
      MBitmap->blendPixelF(xpxl1, ypxl1,   MDrawColor, rfpart_(yend)*xgap);
      MBitmap->blendPixelF(xpxl1, ypxl1+1, MDrawColor, fpart_( yend)*xgap);
      double intery = yend + gradient;
       xend = round_(x2);
      yend = y2 + gradient*(xend - x2);
      xgap = fpart_(x2+0.5);
      int xpxl2 = xend;
      int ypxl2 = ipart_(yend);
      MBitmap->blendPixelF(xpxl2, ypxl2,     MDrawColor, rfpart_(yend) * xgap);
      MBitmap->blendPixelF(xpxl2, ypxl2 + 1, MDrawColor, fpart_(yend)  * xgap);
      int x;
      for(x=xpxl1+1; x < xpxl2; x++) {
        MBitmap->blendPixelF(x, ipart_(intery),     MDrawColor, rfpart_(intery));
        MBitmap->blendPixelF(x, ipart_(intery) + 1, MDrawColor, fpart_(intery));
        intery += gradient;
      }
    } else {
      if ( y2 < y1 ) {
        swap_(x1, x2);
        swap_(y1, y2);
      }
      double gradient = dx / dy;
      double yend = round_(y1);
      double xend = x1 + gradient*(yend - y1);
      double ygap = rfpart_(y1 + 0.5);
      int ypxl1 = yend;
      int xpxl1 = ipart_(xend);
      MBitmap->blendPixelF(xpxl1,     ypxl1, MDrawColor, rfpart_(xend)*ygap);
      MBitmap->blendPixelF(xpxl1 + 1, ypxl1, MDrawColor, fpart_(xend)*ygap);
      double interx = xend + gradient;
   
      yend = round_(y2);
      xend = x2 + gradient*(yend - y2);
      ygap = fpart_(y2+0.5);
      int ypxl2 = yend;
      int xpxl2 = ipart_(xend);
      MBitmap->blendPixelF(xpxl2,     ypxl2, MDrawColor, rfpart_(xend) * ygap);
      MBitmap->blendPixelF(xpxl2 + 1, ypxl2, MDrawColor, fpart_(xend) * ygap);
   
      int y;
      for(y=ypxl1+1; y < ypxl2; y++) {
        MBitmap->blendPixelF(ipart_(interx),     y, MDrawColor, rfpart_(interx));
        MBitmap->blendPixelF(ipart_(interx) + 1, y, MDrawColor, fpart_(interx));
        interx += gradient;
      }
    }
  }

  //-----
    
  #undef ipart_
  #undef round_
  #undef fpart_
  #undef rfpart_
  #undef swap_
  
  //----------
  
  // todo: horiz lines = filled ellipses/circles..

  void stroke4Pixels_wu(int32_t cx, int32_t cy, int32_t x, int32_t y, float f)  {
    uint8_t a = (f * 255.0f);
    if (a==0) return;
    //int32_t w = (x * 2) - 2;
    //int32_t h = (y * 2) - 2;
    //int32_t sx = (cx - x) + 1;
    //int32_t sy = (cy - y) + 1;
    //int32_t i;
    if ((x > 0) && (y > 0)) {
      // horizontal line
      MBitmap->blendPixel(cx-x, cy-y, MDrawColor, a);
      MBitmap->blendPixel(cx+x, cy-y, MDrawColor, a);
      //for (i=0; i<w; i++) setPixel(sx+i,cy-y,c);
      // horizontal line
      MBitmap->blendPixel(cx-x, cy+y, MDrawColor, a);
      MBitmap->blendPixel(cx+x, cy+y, MDrawColor, a);
      //for (i=0; i<w; i++) setPixel(sx+i,cy+y,c);
    }
    else if (x == 0) {
      // vertical
      MBitmap->blendPixel(cx+x, cy-y, MDrawColor, a);
      MBitmap->blendPixel(cx+x, cy+y, MDrawColor, a);
      //for (i=0; i<h; i++) setPixel(cx+x,sy+i,c);
    }
    else if (y == 0) {
      // horizontal
      MBitmap->blendPixel(cx-x, cy+y, MDrawColor, a);
      MBitmap->blendPixel(cx+x, cy+y, MDrawColor, a);
      //for (i=0; i<w; i++) setPixel(sx+i,cy+y,c);
    }
  }  
  
  //----------
  
void _ellipseAA_wu(int x0, int y0, int x1, int y1) {
  int32_t x = x0 + ((x1-x0)/2);
  int32_t y = y0 + ((y1-y0)/2);
  float ell_width = x1 - x0;
  float ell_height = y1 - y0;
  if ((ell_width <= 0) || (ell_height <= 0)) return;
  if (ell_width == ell_height) return;
  float    a   = ell_width / 2.0f;
  float    asq = a*a;
  float    b   = ell_height / 2.0f;
  float    bsq = b*b;
  uint32_t ffd = round(asq / sqrtf(bsq + asq));
  for (uint32_t xi=0; xi<ffd; xi++) {
    float    yj  = b*sqrtf(1-(xi*xi)/asq);
    uint32_t flr = floorf(yj);
    float    frc = yj - flr;
    stroke4Pixels_wu(x,y, xi, flr,    1-frc);
    stroke4Pixels_wu(x,y, xi, flr+1,  frc);
  }
  ffd = round(bsq / sqrt(bsq + asq));
  for (uint32_t yi=0; yi<ffd; yi++) {
    float    xj  = a * sqrt(1 - (yi*yi) / bsq);
    uint32_t flr = floorf(xj);
    float    frc = xj - flr;
    stroke4Pixels_wu(x,y, flr,   yi,  1-frc);
    stroke4Pixels_wu(x,y, flr+1, yi,  frc);
  }
}  

//------------------------------
//private: // gupta-sproull
public:
//------------------------------

  // buggy..
  /*
  void _lineAA_gs(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int d = 2 * dy - dx;
    int increment_E = 2 * dy;
    int increment_NE = 2 * (dy - dx);
    int dx2 = 0;
    int x = x0;
    int y = y0;
    double inv_denominator = 1.0 / (2.0 * sqrt(dx * dx + dy * dy));
    double inv_denominator_dx2 = 2.0 * dx * inv_denominator;
    MBitmap->blendPixelF(x,y,  MDrawColor,0);
    MBitmap->blendPixelF(x,y+1,MDrawColor,inv_denominator_dx2);
    MBitmap->blendPixelF(x,y-1,MDrawColor,inv_denominator_dx2);
    for (; x < x1;) {
      if (d < 0) {
        dx2 = d + dx;
        d = d + increment_E;
      }
      else {
        dx2 = d - dx;
        d = d + increment_NE;
        y = y + 1;
      }
      x = x + 1;
      MBitmap->blendPixelF(x,y,  MDrawColor,dx2 * inv_denominator);
      MBitmap->blendPixelF(x,y+1,MDrawColor,inv_denominator_dx2 - dx2 * inv_denominator);
      MBitmap->blendPixelF(x,y-1,MDrawColor,inv_denominator_dx2 - dx2 * inv_denominator);
    }
  }
  */

//------------------------------
public:
//------------------------------

  virtual void  flush() {}
  virtual void  dirty() {}
  virtual void  dirty(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  dirty(KODE_Rect ARect) {}
  virtual void  finish() {}
  virtual void  resize(float w, float h) {}

  virtual void  setDrawColor(KODE_Color AColor) {
    MDrawColor = KODE_BGRA(AColor); // AColor;
  }
  
  virtual void  setFillColor(KODE_Color AColor) {
    MFillColor = KODE_BGRA(AColor); // AColor;
  }
  
  virtual void  setTextColor(KODE_Color AColor) {
    MTextColor = KODE_BGRA(AColor); // AColor;
  }
  
  virtual void  setPenSize(float ASize) {
    MPenSize = ASize;
  }
  
  virtual void  setPenDash(double* ADashes, uint32_t ANumDashes, float AOffset) {
    MPenDashes = ADashes;
    MPenNumDashes = ANumDashes;
    MPenDashOffset = AOffset;
  }
  
  virtual void  setLineCap(uint32_t ALineCap) {
    MLineCap = ALineCap;
  }
  
  virtual void  setLineJoin(uint32_t ALineJoin) {
    MLineJoin = ALineJoin;
  }
  
  virtual void  setTextSize(float ASize) {
    MTextSize = ASize;
  }
  
  virtual void  setTextFont(const char* AName, uint32_t ASlant, uint32_t AWeight) {
    MTextFont = AName;
    MTextSlant = ASlant;
    MTextWeight = AWeight;
  }

  virtual float getTextWidth(const char* AText) {
    return 0;
  }
  
  virtual float getTextHeight(const char* AText) {
    return 0;
  }

  virtual void  drawPoint(float AX, float AY) {
    KODE_Assert(MBitmap);
    MBitmap->setPixel(AX,AY,MDrawColor);
  }
  
  virtual void  drawHLine(float AX1, float AY1, float AX2) {}
  virtual void  drawVLine(float AX1, float AY1, float AY2) {}
  
  virtual void  drawLine(float AX1, float AY1, float AX2, float AY2) {
    _lineAA_br(AX1,AY1,AX2,AY2);
  }
  
  //virtual void  drawLines(int32_t ANum, int32_t* ACoords) {}
  virtual void  drawRectangle(float AX1, float AY1, float AX2, float AY2) {}
  virtual void  drawRectangle(KODE_Rect ARect) {}
  virtual void  drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) {}
  virtual void  drawRoundedRectangle(KODE_Rect ARect, float AR, uint32_t AC) {}
  virtual void  drawEllipse(float AX1, float AY1, float AX2, float AY2) {
    _ellipseAA_wu(AX1,AY1,AX2,AY2);
  }
  
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
  
};

//----------------------------------------------------------------------
#endif


