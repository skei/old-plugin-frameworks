#ifndef s3_painter_xlib_included
#define s3_painter_xlib_included
//----------------------------------------------------------------------

#include "common/s3_color.h"
#include "gui/s3_drawable.h"
#include "gui/s3_painter_base.h"

//class S3_Painter_Xlib;
//typedef S3_Painter_Xlib S3_Painter_Implementation;

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Painter_Xlib
: public S3_Painter_Base {
  //private:
  protected:
    Display*      MDisplay;   // from drawable
    Drawable      MDrawable;  // from drawable
    int32         MDepth;
    GC            MGC;
    XGCValues     MGCvalues;
    XFontStruct*  MFontStruct;
    #ifdef S3_XRENDER
    Picture       MPicture;
    #endif

  //private:
  protected:
    S3_Color      MDrawColor; // COLORREF
    S3_Color      MFillColor; // COLORREF
    S3_Color      MTextColor; // COLORREF
    int32         MTextSize;
    int32         MBlendMode;
  public:
    S3_Painter_Xlib(S3_Drawable* ADrawable);
    virtual ~S3_Painter_Xlib();
  public:
    Display*  display(void)   { return MDisplay; }  // from drawable
    Drawable  drawable(void)  { return MDrawable; } // from drawable
    GC        gc(void)        { return MGC; }
    int32     depth(void)     { return MDepth; }
  public:
    void      setDrawColor(S3_Color AColor) override;
    void      setFillColor(S3_Color AColor) override;
    void      setTextColor(S3_Color AColor) override;
    void      setTextSize(int32 ASize) override;
    int32     getTextWidth(const char* AText) override;
    int32     getTextHeight(const char* AText) override;
  public:
    void      drawPoint(int32 AX, int32 AY) override;
    void      drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void      drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void      drawRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) override;
    void      drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void      drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) override;
    void      drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) override;
  public:
    void      fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void      fillRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) override;
    void      fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) override;
    void      fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) override;
    void      fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) override;
    void      fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AColor1, S3_Color AColor2, bool AVertical=true) override;
  public:
    void      drawText(int32 AX, int32 AY, const char* AText) override;
    void      drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) override;
  public:
    void      drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) override;
    void      stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) override;

  //----------

  public:

    void  drawPoint(S3_Point APoint)                                                  { drawPoint(APoint.x,APoint.y);  }
    //void  drawHLine(S3_Point APoint, int32 AX2)                                       { drawHLine(APoint.x,APoint.y,AX2); }
    //void  drawVLine(S3_Point APoint, int32 AY2)                                       { drawHLine(APoint.x,APoint.y,AY2); }
    void  drawLine(S3_Point APoint1, S3_Point APoint2)                                { drawLine(APoint1.x,APoint1.y,APoint2.x,APoint2.y); }
    void  drawLine(S3_Rect ARect)                                                     { drawLine(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  drawRectangle(S3_Rect ARect)                                                { drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  drawRoundedRectangle(S3_Rect ARect, int32 AR)                               { drawRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR); }
    void  drawEllipse(S3_Rect ARect)                                                  { drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  drawArc(S3_Rect ARect, float AAngle1, float AAngle2)                        { drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2); }
    void  fillRectangle(S3_Rect ARect)                                                { fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  fillRoundedRectangle(S3_Rect ARect, int32 AR)                               { drawRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR); }
    void  fillArc(S3_Rect ARect, float AAngle1, float AAngle2)                        { fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2); }
    void  fillEllipse(S3_Rect ARect, float AAngle1, float AAngle2)                    { fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }
    void  fillGradient(S3_Rect R, S3_Color C1, S3_Color C2, bool V=true)              { fillGradient(R.x,R.y,R.x2(),R.y2(),C1,C2,V); }

    void  drawText(S3_Point APoint, const char* AText)                                { drawText(APoint.x,APoint.y,AText);  }
    void  drawText(S3_Rect ARect, const char* AText, uint32 AAlign)                   { drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign); }
    void  drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc)    { drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h); }
    void  drawBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc)               { drawBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h); }
    void  stretchBitmap(S3_Rect ADst, S3_Drawable* ASource, S3_Rect ASrc)             { stretchBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h); }
    //void  clip(S3_Rect ARect)                                                       { clip(ARect.x,ARect.y,ARect.x2(),ARect.y2()); }

};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Painter_Xlib::S3_Painter_Xlib(S3_Drawable* ADrawable)
: S3_Painter_Base(ADrawable) {

  MDisplay = ADrawable->display();
  MDrawable = ADrawable->drawable();
  MDepth = ADrawable->depth();
  MGC = XCreateGC(MDisplay,MDrawable,0,&MGCvalues);
  MFontStruct = XQueryFont(MDisplay,XGContextFromGC(MGC));
  //clipRect(0,0,ADrawable->width(),ADrawable->height());

  MDrawColor = S3_White;
  MFillColor = S3_Grey;
  MTextColor = S3_Black;
  MTextSize  = 12;
  //MBlendMode = s3_bm_pictOpOver;

  #ifdef S3_XRENDER
    MBlendMode = PictOpOver;
    MPicture = ADrawable->picture();
  #endif

}

//----------

S3_Painter_Xlib::~S3_Painter_Xlib(void) {
  XFreeFontInfo(S3_NULL,MFontStruct,1);   // 1??
  XFreeGC(MDisplay,MGC);
}

//----------------------------------------
//
//----------------------------------------

/*
XColor _makeXColor(S3_Color AColor) {
  XColor col;
  col.red   = (int)(AColor.r * 255) << 8;
  col.green = (int)(AColor.g * 255) << 8;
  col.blue  = (int)(AColor.b * 255) << 8;
  col.flags = (DoRed | DoGreen | DoBlue);
  XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&col);
  return col;
}
*/

//----------

#ifdef S3_XRENDER
XRenderColor _makeXRenderColor(S3_Color AColor) {
  XRenderColor col;
  col.red   = (int)((AColor.r*AColor.a) * 255) << 8;
  col.green = (int)((AColor.g*AColor.a) * 255) << 8;
  col.blue  = (int)((AColor.b*AColor.a) * 255) << 8;
  col.alpha = (int)((AColor.a) * 255) << 8;
  //col.flags = (DoRed | DoGreen | DoBlue);
  //XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&col);
  return col;
}
#endif

//----------------------------------------
// set/get
//----------------------------------------

void S3_Painter_Xlib::setDrawColor(S3_Color AColor) {
  MDrawColor = AColor;
  XSetForeground(MDisplay,MGC,S3_BGR(AColor));  // was S3_RGB
}

//----------

void S3_Painter_Xlib::setFillColor(S3_Color AColor) {
  MFillColor = AColor;
  //XSetBackground(MDisplay,MGC,_rgb(AColor));
  XSetForeground(MDisplay,MGC,S3_BGR(AColor));  // was S3_RGB
}

//----------

void S3_Painter_Xlib::setTextColor(S3_Color AColor) {
  MTextColor = AColor;
  XSetForeground(MDisplay,MGC,S3_BGR(AColor));  // was S3_RGB
}

//----------

void S3_Painter_Xlib::setTextSize(int32 ASize) {
  MTextSize = ASize;
}

int32 S3_Painter_Xlib::getTextWidth(const char* AText) {
  return XTextWidth(MFontStruct, AText, S3_Strlen(AText));
}

//----------

int32 S3_Painter_Xlib::getTextHeight(const char* AText) {
  return MFontStruct->ascent + MFontStruct->descent;
}

//----------------------------------------
// shapes
//----------------------------------------

void S3_Painter_Xlib::drawPoint(int32 AX, int32 AY) {
  XDrawPoint(MDisplay,MDrawable,MGC,AX,AY);
}

//----------

void S3_Painter_Xlib::drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  //#ifdef S3_XRENDER
  //#else
    XDrawLine(MDisplay,MDrawable,MGC,AX1,AY1,AX2,AY2);
  //#endif
}

//----------

void S3_Painter_Xlib::drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  XDrawRectangle(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1);
}

//----------

void S3_Painter_Xlib::drawRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) {
  int32 r  = AR;// - 1;
  int32 r2 = r*2;
  drawArc(  AX1,      AY1,      AX1+r2-2, AY1+r2-3, 0.75, 0.25 ); // upper left
  drawArc(  AX2-r2+1, AY1,      AX2-1,    AY1+r2-2, 0.00, 0.25 ); // upper right
  drawArc(  AX1,      AY2-r2+1, AX1+r2-2, AY2-1,    0.50, 0.25 ); // lower left
  drawArc(  AX2-r2+1, AY2-r2+2, AX2-1,    AY2-1,    0.25, 0.25 ); // lower right
  drawLine( AX1+r,    AY1,      AX2-r,    AY1       );  // top
  drawLine( AX1+r,    AY2,      AX2-r,    AY2       );  // bottom
  drawLine( AX1,      AY1+r,    AX1,      AY2-r     );  // left
  drawLine( AX2,      AY1+r,    AX2,      AY2-r     );  // right
}

//----------

void S3_Painter_Xlib::drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  XDrawArc(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1,0*64,360*64);
}

//----------

/*
  angle 1 = start angle, relative to 3 o'clock
  angle 2 = 'distance' 0..1, counter-clockwise
*/

void S3_Painter_Xlib::drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
  // start angle = 12 o'clock
  float a1 = -AAngle1 + 0.25;
  // positive = clockwise, negative = counter-clockwise
  float a2 = -AAngle2;
  XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
}

//----------

void S3_Painter_Xlib::drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
  XPoint v[4];
  v[0].x = AX1;
  v[0].y = AY1;
  v[1].x = AX2;
  v[1].y = AY2;
  v[2].x = AX3;
  v[2].y = AY3;
  v[3].x = AX1;
  v[3].y = AY1;
  XDrawLines(
    MDisplay,
    MDrawable,
    MGC,
    v,
    4,
    CoordModeOrigin
  );
}

//----------------------------------------
// filled shapes
//----------------------------------------

void S3_Painter_Xlib::fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  #ifdef S3_XRENDER
  XRenderColor col = _makeXRenderColor(MFillColor);
  XRenderFillRectangle(
    MDisplay,
    MBlendMode,//PictOpOver,
    MPicture,
    &col,
    AX1,
    AY1,
    (AX2-AX1+1),
    (AY2-AY1+1)
  );
  #else
    XFillRectangle(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1+1,AY2-AY1+1);
  #endif
}

//----------

//----------

void S3_Painter_Xlib::fillRoundedRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AR) {
  int32 r  = AR;// - 1;
  int32 r2 = r * 2;
  fillArc(       AX1-1,  AY1-1,   AX1+r2,   AY1+r2,   0.75, 0.25 ); // upper left
  fillArc(       AX2-r2, AY1-1,   AX2,      AY1+r2-1, 0.00, 0.25 ); // upper right
  fillArc(       AX1-1,  AY2-r2,  AX1+r2-1, AY2,      0.50, 0.25 ); // lower left
  fillArc(       AX2-r2, AY2-r2,  AX2,      AY2,      0.25, 0.25 ); // lower right
  fillRectangle( AX1+r,  AY1,     AX2-r,    AY1+r-1   );  // top
  fillRectangle( AX1,    AY1+r,   AX2,      AY2-r     );  // mid
  fillRectangle( AX1+r,  AY2-r+1, AX2-r,    AY2       );  // bot
}

//----------

void S3_Painter_Xlib::fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  XFillArc(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1,0*64,360*64);
}

//----------

// angle 1 = start angle, relative to 3 o'clock
// angle 2 = 'distance' 0..1, counter-clockwise

void S3_Painter_Xlib::fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
  //if (abs(AAngle2) >= 0.01) { //EPSILON
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    XFillArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(int)(a1*(360*64)),(int)(a2*(360*64)));
  //}
}

//----------

//     XRenderPictFormat


void S3_Painter_Xlib::fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
  #ifdef S3_XRENDER
    XTriangle tri;
    tri.p1.x = XDoubleToFixed (AX1);
    tri.p1.y = XDoubleToFixed (AY1);
    tri.p2.x = XDoubleToFixed (AX2);
    tri.p2.y = XDoubleToFixed (AY2);
    tri.p3.x = XDoubleToFixed (AX3);
    tri.p3.y = XDoubleToFixed (AY3);
    Picture brush = s3_xrender_create_brush(MDisplay,MDrawable,MFillColor,32/*MDepth*/);
    XRenderPictFormat* format = s3_xrender_find_format(MDisplay, MDepth);
    XRenderCompositeTriangles(MDisplay,MBlendMode,brush,MPicture,format,0,0,&tri,1);
    s3_xrender_delete_picture(MDisplay,brush);
  #else
    XPoint v[3];
    v[0].x = AX1;
    v[0].y = AY1;
    v[1].x = AX2;
    v[1].y = AY2;
    v[2].x = AX3;
    v[2].y = AY3;
    XFillPolygon(
      MDisplay,
      MDrawable,
      MGC,
      v,
      3,
      Nonconvex,        // Complex, Convex, Nonconvex
      CoordModeOrigin   // coordModeOrigin, CoordModePrevious
    );
  #endif
}

//----------

void S3_Painter_Xlib::fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AColor1, S3_Color AColor2, bool AVertical) {
  if (AVertical) {
    int32 h = AY2-AY1+1;
    if (h>0) {
      float v = 0;
      float vadd = 1.0f / (float)(h-1);
      for (int32 i=0; i<h; i++) {
        //STrace("v %.2f\n",v);
        S3_Color c = S3_BlendColor(v,AColor1,AColor2);
        v += vadd;
        c.a = 1;
        //S3_Painter_Xlib::setDrawColor(c);
        //S3_Painter_Xlib::drawLine(AX1,AY1+i,AX2,AY1+i);
        setDrawColor(c);
        drawLine(AX1,AY1+i,AX2,AY1+i);

      } // for i
    } // h>0
  } // vertical
  else {
    int32 w = AX2-AX1+1;
    if (w>0) {
      float v = 0;
      float vadd = 1.0f / (float)(w-1);
      for (int32 i=0; i<w; i++) {
        //STrace("v %.2f\n",v);
        S3_Color c = S3_BlendColor(v,AColor1,AColor2);
        v += vadd;
        c.a = 1;
        //S3_Painter_Xlib::setDrawColor(c);
        //S3_Painter_Xlib::drawLine(AX1+i,AY1,AX1+i,AY2);
        setDrawColor(c);
        drawLine(AX1+i,AY1,AX1+i,AY2);
      } // for i
    } // h>0
  }
}

//----------------------------------------
// text
//----------------------------------------

/*
  hack alert!
  draw to a rect size 1000x1000, and align to upper left
*/

//----------

void S3_Painter_Xlib::drawText(int32 AX, int32 AY, const char* AText) {
  XDrawString(MDisplay,MDrawable,MGC,AX,AY+MFontStruct->ascent,AText,S3_Strlen(AText));
}

//----------

void S3_Painter_Xlib::drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {
  int32 x,y,w;
  if (AAlign & s3_ta_top) y = AY1 + MFontStruct->ascent;
  else if (AAlign & s3_ta_bottom) y = AY2 - MFontStruct->descent;
  else y = AY1 + (MFontStruct->ascent >> 1) + ((AY2-AY1) >> 1);
  w = getTextWidth(AText);
  if (AAlign & s3_ta_left) x = AX1;
  else if (AAlign & s3_ta_right) x = AX2 - w;
  else x = AX1 + ((AX2 - AX1) >> 1) - ( w >> 1);
  XDrawString(MDisplay,MDrawable,MGC,x,y,AText, S3_Strlen(AText));
}

//----------------------------------------
// bitmaps
//----------------------------------------

void S3_Painter_Xlib::drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
  bool same_depth = (MDepth == ASource->depth());
  if (ASource->isImage()) {
    if (same_depth) XPutImage(MDisplay,MDrawable,MGC,ASource->image(),ASrcX,ASrcY,ADstX,ADstY,ASrcW,ASrcH);
  }
  else if (ASource->isSurface()) {
    #ifdef S3_XRENDER
      XRenderComposite(MDisplay,MBlendMode,ASource->picture(),None,MPicture,ASrcX,ASrcY,0,0,ADstX,ADstY,ASrcW,ASrcH);
    #else
      if (same_depth) XCopyArea(MDisplay,ASource->drawable(),MDrawable,MGC,ASrcX,ASrcY,ASrcW,ASrcH,ADstX,ADstY);
    #endif
  }
}

//----------

void S3_Painter_Xlib::stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
  //bool same_depth = (MDepth == ASource->depth());
  if (ASource->isImage()) {
  }
  else if (ASource->isSurface()) {
    #ifdef S3_XRENDER
      //XTransform xform;
      double xs,ys,zs;
      int32 x,y; //float
      Picture pic;
      //int32 op;
      if (ADstW > 0) {
        if (ADstH > 0) {
          xs = (float)ASrcW / (float)ADstW;
          ys = (float)ASrcH / (float)ADstH;
          zs = 1;
          //TXTransform = array[0..2] {row} of array[0..2] {col} of TXFixed;
          XTransform xform = {{
            { XDoubleToFixed(xs), XDoubleToFixed(0 ), XDoubleToFixed(0 ) },
            { XDoubleToFixed(0 ), XDoubleToFixed(ys), XDoubleToFixed(0 ) },
            { XDoubleToFixed(0 ), XDoubleToFixed(0 ), XDoubleToFixed(zs) }
          }};
          //Picture pic = aImage->getPicture();
          pic = ASource->picture();
          XRenderSetPictureTransform(MDisplay, pic, &xform );
          // is srcx, srcy transformed by the matrix?
          x = S3_Trunc( (float)ASrcX / xs );
          y = S3_Trunc( (float)ASrcY / ys );
          XRenderComposite(MDisplay,MBlendMode,pic,None,MPicture,x,y,0,0,ADstX,ADstY,ADstW,ADstH);
          XRenderSetPictureTransform(MDisplay,pic,&s3_xrender_identity_matrix);
        } //h>0
      } //w>0
    #endif
  }
}

//----------------------------------------------------------------------
#endif
