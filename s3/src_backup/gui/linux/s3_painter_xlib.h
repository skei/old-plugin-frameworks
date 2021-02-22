#ifndef s3_painter_xlib_included
#define s3_painter_xlib_included
//----------------------------------------------------------------------

//#include <X11/X.h>
//#include <X11/Xlib.h>
//#include <X11/Xutil.h>
//#include <X11/cursorfont.h>
//#include <pthread.h>
//#include <unistd.h> // sleep
//
//#ifdef S3_XRENDER
//#include <X11/extensions/Xrender.h>
//#endif

//----------

#include "common/s3_stack.h"
#include "common/s3_rect.h"
#include "gui/base/s3_painter_base.h"
#include "gui/s3_drawable.h"
#include "gui/s3_pen.h"
#include "gui/s3_brush.h"
#include "gui/s3_font.h"
#include "gui/s3_surface.h"
#include "gui/s3_image.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef S3_XRENDER
static XTransform s3_xrender_identity_matrix = {{
  { XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0) },
  { XDoubleToFixed(0), XDoubleToFixed(1), XDoubleToFixed(0) },
  { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1) }
}};
#endif

//typedef S3_Stack<S3_Rect,S3_PAINTER_CLIPSTACK_SIZE> S3_ClipStack;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S3_Painter_Xlib
: public S3_Painter_Base {

  private:
    Display*      MDisplay;   // from drawable
    Drawable      MDrawable;  // from drawable
    int32         MDepth;
    GC            MGC;
    XGCValues     MGCvalues;
    XFontStruct*  MFontStruct;
    #ifdef S3_XRENDER
    Picture       MPicture;
    #endif

  private:
    S3_Pen*       MPen;
    S3_Brush*     MBrush;
    S3_Font*      MFont_;
    S3_Color      MDrawColor; // COLORREF
    S3_Color      MFillColor; // COLORREF
    S3_Color      MTextColor; // COLORREF
    int32         MTextSize;
    int32         MBlendMode;

  private:
    //S3_Rect       MClipRect;
    //S3_ClipStack  MClipStack;
    //FVisibleRect: SRect;

//  public:
//    SRect         cliprect(void)        { return MClipRect; }
//    void          cliprect(SRect ARect) { MClipRect=ARect; }
//    Display*      display(void)         { return MDisplay; }
//    Drawable      drawable(void)        { return MDrawable; }
//    GC            gc(void)              { return MGC; }
//    XFontStruct*  font(void)            { return MFont; }
//    //SRect visiblerect(void)         { return MVisibleRect; }
//    //void  visiblerect(SRect ARect)  { MVisibleRect=ARect; }


  //------------------------------
  //
  //------------------------------

  public:

    /*
      The XCreateGC() function creates a graphics context and returns a GC.
      The GC can be used with any destination drawable having the same root
      and depth as the specified drawable.

      ---

      todo:
        if a_Ptr=NULL: drawable = root window (desktop)
        NULL window-painter
        else surface-painter
    */

    //----------

    /*
      ADrawable
      - where we want to draw to (HBITMAP, aka S3_Surface)
      - uses width(), height() to set up initial clip rectangle
    */

    S3_Painter_Xlib(S3_Drawable* ADrawable)
    : S3_Painter_Base(ADrawable) {

      MDisplay    = ADrawable->display();
      MDrawable   = ADrawable->drawable();

      MDepth = ADrawable->depth();

      MGC         = XCreateGC(MDisplay,MDrawable,0,&MGCvalues);
      MFontStruct = XQueryFont(MDisplay,XGContextFromGC(MGC));

//      MClipRect.set(0,0,ADrawable->width(),ADrawable->height());
      clipRect(0,0,ADrawable->width(),ADrawable->height());

      #ifdef S3_XRENDER
        MPicture  = ADrawable->picture();
        MBlendMode = PictOpOver;
      #endif

      //MDrawColor  = SRed;
      //MFillColor  = SGreen;
      //MTextColor  = SBlue;
      //MTextSize   = 12;

      //#ifdef S3_XRENDER
      //MBlendMode = PictOpOver;
      //#endif

      MDrawColor      = S3_White;
      MFillColor      = S3_Grey;
      MTextColor      = S3_Black;

      MPen            = new S3_Pen();   // (MDrawColor);
      MBrush          = new S3_Brush(); // (MFillColor);
      MFont_          = new S3_Font(ADrawable);  // ("Arial",8,MTextColor);

    }

    //----------

    virtual ~S3_Painter_Xlib() {

      XFreeFontInfo(S3_NULL,MFontStruct,1);   // 1??
      XFreeGC(MDisplay,MGC);

      //TODO: delete picture etc...
      //self..noClip;
      //MClipStack.destroy;
      //inherited;

      delete MPen;
      delete MBrush;
      delete MFont_;
    }

  public:

    Display*  display(void)   { return MDisplay; }  // from drawable
    Drawable  drawable(void)  { return MDrawable; } // from drawable
    GC        gc(void)        { return MGC; }
    int32     depth(void)     { return MDepth; }
    void      blendMode(int32 AMode) { MBlendMode = AMode; }

  //------------------------------
  // internal
  //------------------------------

  private:

    void _moveTo(int32 AX, int32 AY) {
      //FXpos := x;
      //FYpos := y;
    }

    //----------

    void _clearPen(void) {
    }

    //----------

    void _resetPen(void) {
    }

    //----------

    void _clearBrush(void) {
    }

    //----------

    void _resetBrush(void) {
    }

    //----------

    void _setPen(uint32 AColor, int32 AWidth=S3_PAINTER_DEFAULT_PEN_WIDTH) {
    }

    //----------

    void _setBrush(uint32 AColor) {
    }

      //----------

    void _setPenWidth(int32 AWidth) {
      XSetLineAttributes(MDisplay,MGC,AWidth,LineSolid,CapRound,JoinRound);
    }

    //----------

    void _setPenStyle(uint32 AStyle) {
    }

    //----------

    void _setBrushStyle(uint32 AStyle) {
      XSetFillStyle(MDisplay,MGC,AStyle); // FillSolid
    }

  //------------------------------
  // color
  //------------------------------

  private:

    XColor _makeXColor(S3_Color AColor) {
      XColor col;
      col.red   = (int)(AColor.r * 255) << 8;
      col.green = (int)(AColor.g * 255) << 8;
      col.blue  = (int)(AColor.b * 255) << 8;
      col.flags = (DoRed | DoGreen | DoBlue);
      XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&col);
      return col;
    }

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

  //------------------------------
  // low level
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void lock(void) {
    }

    virtual // S3_Painter_Base
    void unlock(void) {
    }

    virtual // S3_Painter_Base
    void sync(void) {
    }

    virtual // S3_Painter_Base
    void flush(void) {
    }

  //------------------------------
  // select/reset
  //------------------------------

  public:

    // APen = S3_NULL -> invisible pen

    virtual // S3_Painter_Base
    void selectPen(S3_Pen* APen, bool ARemember=true) {
      if (APen) {
        MDrawColor = APen->color();
        //old = SelectObject(MHandle,APen->hpen());
      }
      else {
        //old = SelectObject(MHandle,MNullPen);
      }
      //if (ARemember) MOldPen = old;
    }

    //----------

    virtual // S3_Painter_Base
    void resetPen(void) {
      //SelectObject(MHandle,MOldPen);
    }

    //----------

    // ABrush = S3_NULL -> invisible brush

    virtual // S3_Painter_Base
    void selectBrush(S3_Brush* ABrush, bool ARemember=true) {
      if (ABrush) {
        MFillColor = ABrush->color();
        //old = SelectObject(MHandle,ABrush->hbrush());
      }
      else {
        //old = SelectObject(MHandle,MNullBrush);
      }
      //if (ARemember) MOldBrush = old;
    }

    //----------

    virtual // S3_Painter_Base
    void resetBrush(void) {
      //SelectObject(MHandle,MOldBrush);
    }

    //----------

    virtual // S3_Painter_Base
    void selectFont(S3_Font* AFont, bool ARemember=true) {
      //MTextColor = AFont->textColor();
      if (ARemember) {
        //MOldFont = SelectObject(MHandle,AFont->hfont());
      }
      else {
        //SelectObject(MHandle,AFont->hfont());
      }
    }

    //----------

    virtual // S3_Painter_Base
    void resetFont(void) {
      //SelectObject(MHandle,MOldFont);
    }

    //----------

    /*
      https://msdn.microsoft.com/en-us/library/windows/desktop/dd162957%28v=vs.85%29.aspx
      Bitmaps can only be selected into memory DC's. A single bitmap cannot be
      selected into more than one DC at the same time.
    */

    virtual // S3_Painter_Base
    void selectDrawable(S3_Drawable* ADrawable, bool ARemember=true) {
      //HBITMAP hbitmap = ADrawable->hbitmap();
      //if (hbitmap) {
      //  if (ARemember) MOldBitmap = SelectObject(MHandle,hbitmap);
      //  else SelectObject(MHandle,hbitmap);
      //}
    }

    //----------

    virtual // S3_Painter_Base
    void resetDrawable(void) {
      //SelectObject(MHandle,MOldBitmap);
    }

    //----------

    virtual
    Font loadFont(const char* AName, int32 ASize) {
      //MFontStruct = XQueryFont(MDisplay,XGContextFromGC(MGC));
      return XLoadFont(MDisplay,AName);
    }

    virtual
    void unloadFont(Font AFont) {
      //XFreeFontInfo(S3_NULL,MFontStruct,1);   // 1??
      XUnloadFont(MDisplay,AFont);
    }


  //------------------------------
  // get/set
  //------------------------------

  public:

    /*
      SetDCPenColor function sets the current device context (DC) pen color to
      the specified color value.
      If the function succeeds, the return value specifies the previous DC pen
      color as a COLORREF value. If the function fails, the return value is
      CLR_INVALID.
      The function returns the previous DC_PEN color, even if the stock pen
      DC_PEN is not selected in the DC; however, this will not be used in
      drawing operations until the stock DC_PEN is selected in the DC.
      The GetStockObject function with an argument of DC_BRUSH or DC_PEN can be
      used interchangeably with the SetDCPenColor and SetDCBrushColor
      functions.
    */

    virtual // S3_Painter_Basehj
    void setDrawColor(S3_Color AColor) {
      MDrawColor = AColor;
      XSetForeground(MDisplay,MGC,S3_BGR(AColor));  // was S3_RGB
    }

    //----------

    virtual // S3_Painter_Base
    void setFillColor(S3_Color AColor) {
      MFillColor = AColor;
      //XSetBackground(MDisplay,MGC,_rgb(AColor));
      XSetForeground(MDisplay,MGC,S3_BGR(AColor));  // was S3_RGB
    }

    //----------

    virtual // S3_Painter_Base
    void setTextColor(S3_Color AColor) {
      MTextColor = AColor;
      XSetForeground(MDisplay,MGC,S3_BGR(AColor));  // was S3_RGB
    }

    //----------

//    void setTextSize(int32 ASize) {
//      MTextSize = ASize;
//    //  LOGFONT lfont;
//    //  HFONT font = (HFONT)GetCurrentObject(MHandle,OBJ_FONT);
//    //  GetObject(font,sizeof(LOGFONT),&lfont);
//    //  lfont.lfHeight = -MulDiv(ASize,GetDeviceCaps(MHandle,LOGPIXELSY),72);
//    //  if (MFont) DeleteObject(MFont);
//    //  MFont = CreateFontIndirect(&lfont);
//    //  SelectObject(MHandle,MFont);
//    }

    //----------

    virtual // S3_Painter_Base
    int32 getTextWidth(const char* AText) {
      return XTextWidth(MFontStruct, AText, S3_Strlen(AText));
    }

    //----------

    virtual // S3_Painter_Base
    int32 getTextHeight(const char* AText) {
      return MFontStruct->ascent + MFontStruct->descent;
    }

    //----------

//    void setBlendMode(int32 AMode) {
//      MBlendMode = AMode;
//    }


  //------------------------------
  // shapes
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void drawPoint(int32 AX, int32 AY) {
      XDrawPoint(MDisplay,MDrawable,MGC,AX,AY);
    }

    //----------

    virtual // S3_Painter_Base
    void drawLine(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XDrawLine(MDisplay,MDrawable,MGC,AX1,AY1,AX2,AY2);
    }

    //----------

    virtual // S3_Painter_Base
    void drawRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XDrawRectangle(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1);
    }

    //----------

    virtual // S3_Painter_Base
    void drawRoundedRectangle(void) {
    }

    //----------

    virtual // S3_Painter_Base
    void drawEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XDrawArc(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1,0*64,360*64);
    }

    //----------

    /*
      angle 1 = start angle, relative to 3 o'clock
      angle 2 = 'distance' 0..1, counter-clockwise
    */

    virtual // S3_Painter_Base
    void drawArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
      //if (abs(AAngle2) >= 0.01) { /*EPSILON*/
        // start angle = 12 o'clock
        float a1 = -AAngle1 + 0.25;
        // positive = clockwise, negative = counter-clockwise
        float a2 = -AAngle2;
        XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
      //}
    }

    //----------

    virtual // S3_Painter_Base
    void drawTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
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

  //------------------------------
  // fill shapes
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void fillRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XFillRectangle(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1+1,AY2-AY1+1);
    }

    //----------

    virtual // S3_Painter_Base
    void fillRoundedRectangle(void) {
    }

    //----------

    virtual // S3_Painter_Base
    void fillEllipse(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      XFillArc(MDisplay,MDrawable,MGC,AX1,AY1,AX2-AX1,AY2-AY1,0*64,360*64);
    }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise

    virtual // S3_Painter_Base
    void fillArc(int32 AX1, int32 AY1, int32 AX2, int32 AY2, float AAngle1, float AAngle2) {
      //if (abs(AAngle2) >= 0.01) { //EPSILON
        // start angle = 12 o'clock
        float a1 = -AAngle1 + 0.25;
        // positive = clockwise, negative = counter-clockwise
        float a2 = -AAngle2;
        XFillArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(int)(a1*(360*64)),(int)(a2*(360*64)));
      //}
    }

    //----------

    virtual // S3_Painter_Base
    void fillTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3) {
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
    }

  //------------------------------
  // gradients
  //------------------------------

  /*
    gradients are not gamma correct..
  */

  public:

    virtual // S3_Painter_Base
    void gradientRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AUpperLeft, S3_Color ALowerRight, bool AVertical=true) {
      int32 w = AX2 - AX1 + 1;
      int32 h = AY2 - AY1 + 1;
      if ((w > 0) && (h > 0)) {
        float t = 0;
        if (AVertical) {
          float dt = 1.0f / (float)h;
          for (int32 i=0; i<h; i++) {
            S3_Color c = S3_BlendColor( t, AUpperLeft, ALowerRight );
            setDrawColor(c);
            drawLine(AX1,AY1+i,AX2,AY1+i);
            t += dt;
          }
        }
        else { // horizontal
          float dt = 1.0f / (float)w;
          for (int32 i=0; i<w; i++) {
            S3_Color c = S3_BlendColor( t, AUpperLeft, ALowerRight );
            setDrawColor(c);
            drawLine(AX1+i,AY1,AX1+i,AY2);
            t += dt;
          }
        }
      }
    }

    //----------

    virtual // S3_Painter_Base
    void gradientTriangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 AX3, int32 AY3, S3_Color AColor1, S3_Color AColor2, S3_Color AColor3) {
    }

  //------------------------------
  // blend shapes
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void blendRectangle(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
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
      #endif
    }

  //------------------------------
  // text
  //------------------------------

  public:

    /*
      hack alert!
      draw to a rect size 1000x1000, and align to upper left
    */

    //----------

    virtual // S3_Painter_Base
    void drawText(int32 AX, int32 AY, const char* AText) {
      XDrawString(MDisplay,MDrawable,MGC,AX,AY+MFontStruct->ascent,AText,S3_Strlen(AText));
    }

    //----------

    virtual // S3_Painter_Base
    void drawText(int32 AX1, int32 AY1, int32 AX2, int32 AY2, const char* AText, uint32 AAlign) {
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

    //----------

    virtual // S3_Painter_Base
    void drawFont(int32 AX, int32 AY, const char* AText, S3_Font* AFont) {
      #ifdef S3_XFT
      XftDrawString8(AFont->draw(), AFont->color(), AFont->font(), AX,AY , (XftChar8 *)AText, S3_Strlen(AText));
      XFlush(MDisplay);
      #else
      drawText(AX,AY,AText);
      #endif
    }

  //------------------------------
  // bitmaps
  //------------------------------

  public:

    /*

      The XPutImage() function combines an image with a rectangle of the
      specified drawable. The section of the image defined by the src_x, src_y,
      width, and height arguments is drawn on the specified part of the
      drawable. If XYBitmap format is used, the depth of the image must be one,
      or a BadMatch error results. The foreground pixel in the GC defines the
      source for the one bits in the image, and the background pixel defines
      the source for the zero bits. For XYPixmap and ZPixmap , the depth of the
      image must match the depth of the drawable, or a BadMatch error results.

      If the characteristics of the image (for example, byte_order and
      bitmap_unit) differ from what the server requires, XPutImage()
      automatically makes the appropriate conversions.
    */

    //----------

    /*
      The XCopyArea() function combines the specified rectangle of src with the
      specified rectangle of dest. The drawables must have the same root and
      depth, or a BadMatch error results.
    */

    //----------

    virtual // S3_Painter_Base
    void drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
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

    virtual // S3_Painter_Base
    void maskBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH, S3_Drawable* AMask, int32 AMaskX, int32 AMaskY) {
    }

    //----------

    virtual // S3_Painter_Base
    void transparentBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH, S3_Color ATransparentColor) {
    }

    //----------

    /*
    virtual // S3_Painter_Base
    void blendBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      //if (MDepth != ASource->depth()) { S3_Trace("depth doesn't match!\n!"); }
      if (ASource->isImage()) {
        // todo: upload to (temp) surface, blend..
      }
      else if (ASource->isSurface()) {
      #ifdef S3_XRENDER
        //STrace("MPicture: %i\n",MPicture);
        //STrace("picture: %i\n",ASurface->picture());
        XRenderComposite(MDisplay,MBlendMode,ASource->picture(),None,MPicture,ASrcX,ASrcY,0,0,ADstX,ADstY,ASrcW,ASrcH);
      #endif
      }
    }
    */

    //----------

    virtual // S3_Painter_Base
    void stretchBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ASource, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
      //bool same_depth = (MDepth == ASource->depth());
      if (ASource->isImage()) {
        // 1. upload to surface
        // 2. do the blending..
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

  //------------------------------
  // clipping
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void clip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {

      //MClipRect = S3_Rect(AX1,AY1,AX2-AX1+1,AY2-AY1+1);

      XRectangle r;
      r.x      = AX1;       // ARect.x;
      r.y      = AY1;       // ARect.y;
      r.width  = AX2-AX1+2; // ARect.w + 1;
      r.height = AY2-AY1+2; // ARect.h + 1;
      XSetClipRectangles(MDisplay,MGC,0,0,&r,1,Unsorted);
    }

    //----------

    virtual // S3_Painter_Base
    void noClip(void) {
      XSetClipMask(MDisplay,MGC,None);
    }

  //------------------------------
  // helpers
  //------------------------------

  public:

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

    void fillRectangle(S3_Rect ARect) {
      this->fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void fillEllipse(S3_Rect ARect) {
      this->fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void fillArc(S3_Rect ARect, float AAngle1, float AAngle2) {
      this->fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
    }

    void gradientRectangle(S3_Rect ARect, S3_Color AUpperLeft, S3_Color ALowerRight, bool AVertical=false) {
      this->gradientRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AUpperLeft,ALowerRight,AVertical);
    }

    void gradientTriangle(S3_Point APoint1, S3_Point APoint2, S3_Point APoint3, S3_Color AColor1, S3_Color AColor2, S3_Color AColor3) {
      this->gradientTriangle(APoint1.x,APoint1.y,APoint2.x,APoint2.y,APoint3.x,APoint3.y,AColor1,AColor2,AColor3);
    }

    void blendRectangle(S3_Rect ARect) {
      this->blendRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

    void drawText(S3_Point APoint, const char* AText) {
      this->drawText(APoint.x,APoint.y,AText);
    }

    void drawText(S3_Rect ARect, const char* AText, uint32 AAlign) {
      this->drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
    }

    void drawBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc) {
      this->drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void drawBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->drawBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void maskBitmap(int32 ADstX, int32 ADstY, S3_Drawable* ASource, S3_Rect ASrc, S3_Drawable* AMask, int32 AMaskX, int32 AMaskY) {
      this->maskBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h,AMask,AMaskX,AMaskY);
    }

    //void blendBitmap(S3_Point ADst, S3_Drawable* ASource, S3_Rect ASrc) {
    //  this->blendBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    //}

    void stretchBitmap(S3_Rect ADst, S3_Drawable* ASource, S3_Rect ASrc) {
      this->stretchBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
    }

    void clip(S3_Rect ARect) {
      this->clip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

};

//----------------------------------------------------------------------
#endif
