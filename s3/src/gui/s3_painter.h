#ifndef s3_painter_included
#define s3_painter_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #ifdef S3_GUI_CAIRO
    #include "gui/s3_painter_cairo.h"
    typedef S3_Painter_Cairo S3_Painter_Implementation;
  #else
    #include "gui/s3_painter_xlib.h"
    typedef S3_Painter_Xlib S3_Painter_Implementation;
  #endif
#endif

#include "common/s3_point.h"
#include "common/s3_rect.h"
#include "common/s3_stack.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

typedef S3_Stack<S3_Rect,S3_PAINTER_CLIPSTACK_SIZE> S3_ClipStack;

//----------

class S3_Painter
: public S3_Painter_Implementation {
  private:
    S3_Rect       MClipRect;
    S3_ClipStack  MClipStack;
  public:
    S3_Painter(S3_Drawable* ADrawable);
    virtual ~S3_Painter();
  public:

    void  pushClip(int32 AX1, int32 AY1, int32 AX2, int32 AY2);
    void  pushClip(S3_Rect ARect);
    void  popClip(void);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Painter::S3_Painter(S3_Drawable* ADrawable)
: S3_Painter_Implementation(ADrawable) {
  MClipRect.set(0,0,ADrawable->width(),ADrawable->height());
}

//----------

S3_Painter::~S3_Painter() {
}

//----------------------------------------------------------------------

void S3_Painter::pushClip(int32 AX1, int32 AY1, int32 AX2, int32 AY2) {
  MClipStack.push(MClipRect);
  clip(AX1,AY1,AX2,AY2);
  MClipRect = S3_Rect(AX1,AY1,AX2-AX1+1,AY2-AY1+1);
}

//----------

/*
  cairo can only _shrink_ the clipping rectangle..
  - todo: call resetClip() in cairo's clip() ?
*/

void S3_Painter::pushClip(S3_Rect ARect) {
  pushClip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
}

//----------

void S3_Painter::popClip(void) {
  S3_Rect r = MClipStack.pop();
  /*this.*/clip(r.x,r.y,r.x2(),r.y2());
  MClipRect = r;
}

//----------------------------------------------------------------------

    /*

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

    */


//----------------------------------------------------------------------
#endif
