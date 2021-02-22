#ifndef s3_painter_included
#define s3_painter_included
//----------------------------------------------------------------------

#include "common/s3_edges.h"
#include "common/s3_integer.h"
#include "common/s3_rect.h"
#include "common/s3_stack.h"

#ifdef S3_LINUX
  #ifdef S3_GUI_XLIB
    #include "gui/linux/s3_painter_xlib.h"
    typedef S3_Painter_Xlib S3_Painter_Implementation;
  #endif
#endif

//----------------------------------------------------------------------

#ifdef S3_WIN32
  #ifdef S3_GUI_GDI
    #include "gui/win32/s3_painter_gdi.h"
    typedef S3_Painter_Gdi S3_Painter_Implementation;
  #endif
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//typedef S3_Painter_Implementation S3_Painter;

typedef S3_Stack<S3_Rect,S3_PAINTER_CLIPSTACK_SIZE> S3_ClipStack;

//----------

class S3_Painter
: public S3_Painter_Implementation {

  private:
    S3_Rect       MClipRect;
    S3_ClipStack  MClipStack;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Painter(S3_Drawable* ADrawable)
    : S3_Painter_Implementation(ADrawable) {
      MClipRect.set(0,0,ADrawable->width(),ADrawable->height());
    }

    //----------

    virtual ~S3_Painter() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual // S3_Painter_Base
    void pushClip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
      MClipStack.push(MClipRect);
      clip(AX1,AY1,AX2,AY2);
      MClipRect = S3_Rect(AX1,AY1,AX2-AX1+1,AY2-AY1+1);
    }

    //----------

    /*
      cairo can only _shrink_ the clipping rectangle..
      - todo: call resetClip() in cairo's clip() ?
    */

    virtual // S3_Painter_Base
    void popClip(void) {
      S3_Rect r = MClipStack.pop();
      /*this.*/clip(r.x,r.y,r.x2(),r.y2());
      MClipRect = r;
    }

    //----------

    void clipRect(int32 x, int32 y, int32 w, int32 h) {
      MClipRect.set(x,y,w,h);
    }

  //------------------------------
  // drawing
  //------------------------------

  public:

    //virtual
    void tileBitmap(int32 ADstX, int32 ADstY, int32 ADstW, int32 ADstH, S3_Drawable* ADrawable, int32 ASrcX, int32 ASrcY, int32 ASrcW, int32 ASrcH) {
    //var
    //  x,y,w,h : longint;
    //  ww,hh : longint;
      int32 y = ADstY;
      int32 h = ADstH;
      while (h > 0) {
        int32 hh = S3_MinI(h,ASrcH);
        int32 x = ADstX;
        int32 w = ADstW;
        while (w > 0) {
          int32 ww = S3_MinI(w,ASrcW);
          drawBitmap(x,y,ADrawable,ASrcX,ASrcY,ww,hh);
          //blendBitmap(x,y,ADrawable,ASrcX,ASrcY,ww,hh);
          x += ASrcW;
          w -= ASrcW;
        }
        y += ASrcH;
        h -= ASrcH;
      }
    }

    //----------

//    #define s3_fm_draw    0
//    #define s3_fm_stretch 1
//    #define s3_fm_tile    2

    //void flexBitmap(S3_Rect ARect, S3_Drawable* ADrawable, uint32 AMode, int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) {

    virtual
    void flexBitmap(S3_Rect ADst, S3_Drawable* ADrawable, uint32 AMode, S3_Rect ASrc, S3_Edges AEdges) {
      int32 dx1,dx2,dx3;
      int32 dy1,dy2,dy3;
      int32 dw1,dw2,dw3;
      int32 dh1,dh2,dh3;
      int32 sx1,sx2,sx3;
      int32 sy1,sy2,sy3;
      int32 sw1,sw2,sw3;
      int32 sh1,sh2,sh3;
      dx1 = ADst.x;
      dx2 = ADst.x + AEdges.left;
      dx3 = ADst.x + ADst.w - AEdges.right;
      dy1 = ADst.y;
      dy2 = ADst.y + AEdges.top;
      dy3 = ADst.y + ADst.h - AEdges.bottom;
      dw1 = AEdges.left;
      dw2 = ADst.w - AEdges.left - AEdges.right;
      dw3 = AEdges.right;
      dh1 = AEdges.top;
      dh2 = ADst.h - AEdges.top - AEdges.bottom;
      dh3 = AEdges.bottom;
      sx1 = ASrc.x;
      sx2 = ASrc.x + AEdges.left;
      sx3 = ASrc.x + ASrc.w - AEdges.right;
      sy1 = ASrc.y;
      sy2 = ASrc.y + AEdges.top;
      sy3 = ASrc.y + ASrc.h - AEdges.bottom;
      sw1 = AEdges.left;
      sw2 = ASrc.w - AEdges.left - AEdges.right;
      sw3 = AEdges.right;
      sh1 = AEdges.top;
      sh2 = ASrc.h - AEdges.top - AEdges.bottom;
      sh3 = AEdges.bottom;
      //if (sw1 + sw3) <= ADst.w then sw2 := 0;
      //if (sh1 + sh3) <= ADst.h then sh2 := 0;
      // top
      if (sh1 > 0) {
        //if (sw1 > 0 ) blendBitmap(dx1,dy1, ADrawable, sx1,sy1,sw1,sh1);
        if (sw1 > 0 ) drawBitmap(dx1,dy1, ADrawable, sx1,sy1,sw1,sh1);
        if (sw2 > 0) {
          switch(AMode) {
            case s3_fm_draw:
              //blendBitmap( dx2,dy1, ADrawable, sx2,sy1,sw2,sh1);
              drawBitmap( dx2,dy1, ADrawable, sx2,sy1,sw2,sh1);
              break;
            case s3_fm_stretch:
              stretchBitmap( dx2,dy1,dw2,dh1, ADrawable, sx2,sy1,sw2,sh1);
              break;
            case s3_fm_tile:
              tileBitmap( dx2,dy1,dw2,dh1, ADrawable, sx2,sy1,sw2,sh1);
              break;
          }
        }
        //if (sw3 > 0) blendBitmap( dx3,dy1, ADrawable, sx3,sy1,sw3,sh1);
        if (sw3 > 0) drawBitmap( dx3,dy1, ADrawable, sx3,sy1,sw3,sh1);
      }
      // middle
      if (sh2 > 0) {
        switch (AMode) {
          case s3_fm_draw:
            //if (sw1 > 0) blendBitmap( dx1,dy2, ADrawable, sx1,sy2,sw1,sh2);
            //if (sw2 > 0) blendBitmap( dx2,dy2, ADrawable, sx2,sy2,sw2,sh2);
            //if (sw3 > 0) blendBitmap( dx3,dy2, ADrawable, sx3,sy2,sw3,sh2);
            if (sw1 > 0) drawBitmap( dx1,dy2, ADrawable, sx1,sy2,sw1,sh2);
            if (sw2 > 0) drawBitmap( dx2,dy2, ADrawable, sx2,sy2,sw2,sh2);
            if (sw3 > 0) drawBitmap( dx3,dy2, ADrawable, sx3,sy2,sw3,sh2);
            break;
          case s3_fm_stretch:
            if (sw1 > 0) stretchBitmap( dx1,dy2,dw1,dh2, ADrawable, sx1,sy2,sw1,sh2);
            if (sw2 > 0) stretchBitmap( dx2,dy2,dw2,dh2, ADrawable, sx2,sy2,sw2,sh2);
            if (sw3 > 0) stretchBitmap( dx3,dy2,dw3,dh2, ADrawable, sx3,sy2,sw3,sh2);
            break;
          case s3_fm_tile:
            if (sw1 > 0) tileBitmap( dx1,dy2,dw1,dh2, ADrawable, sx1,sy2,sw1,sh2);
            if (sw2 > 0) tileBitmap( dx2,dy2,dw2,dh2, ADrawable, sx2,sy2,sw2,sh2);
            if (sw3 > 0) tileBitmap( dx3,dy2,dw3,dh2, ADrawable, sx3,sy2,sw3,sh2);
            break;
        }
      }
      // bottom
      if (sh3 > 0) {
        //if (sw1 > 0) blendBitmap( dx1,dy3, ADrawable, sx1,sy3,sw1,sh3);
        if (sw1 > 0) drawBitmap( dx1,dy3, ADrawable, sx1,sy3,sw1,sh3);
        if (sw2 > 0) {
          switch(AMode) {
            case s3_fm_draw:
              //blendBitmap( dx2,dy3, ADrawable, sx2,sy3,sw2,sh3);
              drawBitmap( dx2,dy3, ADrawable, sx2,sy3,sw2,sh3);
              break;
            case s3_fm_stretch:
              stretchBitmap( dx2,dy3,dw2,dh3, ADrawable, sx2,sy3,sw2,sh3);
              break;
            case s3_fm_tile:
              tileBitmap( dx2,dy3,dw2,dh3, ADrawable, sx2,sy3,sw2,sh3);
              break;
          }
        }
        //if (sw3 > 0) blendBitmap( dx3,dy3, ADrawable, sx3,sy3,sw3,sh3);
        if (sw3 > 0) drawBitmap( dx3,dy3, ADrawable, sx3,sy3,sw3,sh3);
      }
    }

    //----------

    virtual
    void drawRoundedRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 ARound) {
      int32 r  = ARound;// - 1;
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

    virtual
    void fillRoundedRect(int32 AX1, int32 AY1, int32 AX2, int32 AY2, int32 ARound) {
      int32 r  = ARound;// - 1;
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

    virtual
    void fillGradient(int32 AX1, int32 AY1, int32 AX2, int32 AY2, S3_Color AColor1, S3_Color AColor2, bool AVertical=true) {
      if (AVertical) {
        int32 h = AY2-AY1+1;
        if (h>0) {
          float v = 0;
          float vadd = 1.0f / (float)(h-1);
          for (int32 i=0; i<h; i++) {
            //STrace("v %.2f\n",v);
            S3_Color c = S3_BlendColor(v,AColor1,AColor2);
            v += vadd;
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
            setDrawColor(c);
            drawLine(AX1+i,AY1,AX1+i,AY2);
          } // for i
        } // h>0
      }
    }

    //----------


  //------------------------------
  // helpers
  //------------------------------

  public:

    virtual
    void pushClip(S3_Rect ARect) {
      this->pushClip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
    }

};

//----------------------------------------------------------------------
#endif
