/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axCanvasLinux_included
#define axCanvasLinux_included
//----------------------------------------------------------------------

#include <math.h>

//#ifdef AX_XRENDER
#ifdef AX_ALPHA
#include <X11/extensions/Xrender.h>
#endif

//#include "base/axContext.h"
#include "core/axDefines.h"
//#include "base/axCanvasBase.h"
#include "gui/axCanvasBase.h"
//#include "gui/axSurface.h"
#include "gui/axBitmap.h"

//----------------------------------------------------------------------

#define DEF_PENWIDTH 0

#ifdef AX_ALPHA
XTransform mat_ident =
{
  {
    { XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0) },
    { XDoubleToFixed(0), XDoubleToFixed(1), XDoubleToFixed(0) },
    { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1) }
  }
};
#endif

//----------------------------------------------------------------------

class axSurface;

// needs mDisplay & mWindow (Drawable) set in aContext

// drawable (context.window) = unsigned long
// window = HWND = void*




class axCanvasLinux : public axCanvasBase
{
  private:
    Display*      mDisplay;   // from context
    Drawable      mDrawable;  // from context
    GC            mGC;
    XGCValues     gcvalues;
    XFontStruct*  mFont;
    #ifdef AX_ALPHA
    Picture       mPicture;
    #endif

//    #ifdef AX_XRENDER
//    bool          mXRender;
//    Picture       mPicture;
//    //XRenderColor rendercolor;
//    #endif

  protected:
    int           mXpos;
    int           mYpos;
    int           mClipX1,mClipY1,mClipX2,mClipY2;

  public:

    axCanvasLinux(axBase* aBase, void* aSource)
    : axCanvasBase(/*aContext*/)
      {
        //mDisplay = (Display*)aInterface->getHandle();//(Display*)aFormat->getInterface()->getHandle();
        mDisplay = (Display*)aBase->getInterface()->getHandle();//(Display*)aFormat->getInterface()->getHandle();
        mDrawable = *(Drawable*)aSource;
        //mDisplay  = (Display*)aFormat->linux_getDisplay();//aContext->mDisplay;                         //trace(":: mDisplay = " << mDisplay);
        //mDrawable = aContext->mWindow;                          //trace(":: mDrawable = " << mDrawable);
        #ifdef AX_ALPHA
        mPicture  = -2;
        #endif
        mGC       = XCreateGC(mDisplay,mDrawable,0,&gcvalues);  //trace(":: mGC = " << mGC);
        mFont     = XQueryFont(mDisplay,XGContextFromGC(mGC));  //trace(":: mFont = " << mFont);
        mXpos     = 0;
        mYpos     = 0;
      }

    //----------

    virtual ~axCanvasLinux()
      {
        XFreeFontInfo(0,mFont,1);   // 1??
        XFreeGC(mDisplay,mGC);
        //TODO: delete picture etc...
      }

    //--------------------------------------------------

    //virtual int getHandle(void) { return (int)mDrawable; }
    virtual void setPicture(long pic)
      {
        #ifdef AX_ALPHA
        mPicture = pic;
        #endif
      }

    //--------------------------------------------------
    // get / set
    //--------------------------------------------------

    virtual void setPos(int aX, int aY)
      {
        mXpos = aX;
        mYpos = aY;
      }

    //----------

    virtual axColor getColor(int aRed, int aGreen, int aBlue)
    {
      XColor xcol;
      xcol.red   = aRed << 8;
      xcol.green = aGreen << 8;
      xcol.blue  = aBlue << 8;
      xcol.flags = (DoRed or DoGreen or DoBlue);
      XAllocColor(mDisplay,XDefaultColormap(mDisplay,0),&xcol);
      return axColor(xcol.pixel);
    }

    virtual void    setPenColor(axColor aColor)   { XSetForeground(mDisplay,mGC,aColor.get()); }
    virtual void    setBrushColor(axColor aColor) { XSetForeground(mDisplay,mGC,aColor.get()); }
    virtual void    setTextColor(axColor aColor)  { XSetForeground(mDisplay,mGC,aColor.get()); }

    //virtual void    clearPen(void)    { mClearPen = true; }
    virtual void    resetPen(void)    { setPenWidth(DEF_PENWIDTH); }
    //virtual void    clearBrush(void)  { mClearBrush}
    //virtual void    resetBrush(void)  {}

    virtual void setPenWidth(int aWidth)
      {
        //mPenWidth = aWidth;
        XSetLineAttributes(mDisplay,mGC,aWidth,LineSolid,CapRound,JoinRound);
      };

    //virtual void resetPenWidth(void)
    //  {
    //    mPenWidth = DEF_PENWIDTH;
    //    XSetLineAttributes(gDP,mGC,mPenWidth,LineSolid,CapRound,JoinRound);
    //  };

    virtual void setPenStyle(int aStyle)
      {
        //TODO
      };

    //----------

    virtual void setBrushStyle(int aStyle)
      {
        XSetFillStyle(mDisplay,mGC,aStyle); // FillSolid
      };


    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------

    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        //mClipRect.set(aX1,aY1,(aX2-aX1+1),(aY2-aY1+1));
        XRectangle r;
        r.x      = aX1;
        r.y      = aY1;
        r.width  = (aX2-aX1)+2; // xlib seems to cut off one pixel to the right & bottom... ?
        r.height = (aY2-aY1)+2; // so we compensate by adding another pixel
        XSetClipRectangles(mDisplay,mGC,0,0,&r,1,Unsorted);
        mClipX1 = aX1;
        mClipY1 = aY1;
        mClipX2 = aX2;
        mClipY2 = aY2;
      }

    //----------

    virtual void resetClipRect(void)
      {
        //clearClipRect();
        setClipRect(mClipX1,mClipY1,mClipX2,mClipY2);
      }

    //----------

    virtual void clearClipRect(void)
      {
        XSetClipMask(mDisplay,mGC,None);
      }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        XDrawPoint(mDisplay,mDrawable,mGC,aX,aY);
      }

    virtual void drawPoint(int aX, int aY, axColor aColor)
      {
        XSetForeground(mDisplay,mGC,aColor.get());
        XDrawPoint(mDisplay,mDrawable,mGC,aX,aY);
      }


    //----------

    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawLine(mDisplay,mDrawable,mGC,aX1,aY1,aX2,aY2);
      }

    //----------

    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawRectangle(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1,aY2-aY1);
      }

    //----------

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        //#ifdef AX_XRENDER
				//XRenderFillRectangle(mDisplay,PictOpOver,mPicture,&rendercolor,aX1,aY1,aX2-aX1+1,aY2-aY1+1);
        //#else
        XFillRectangle(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1+1,aY2-aY1+1);
        //#endif
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XDrawArc(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        XFillArc(mDisplay,mDrawable,mGC,aX1,aY1,aX2-aX1,aY2-aY1,0*64,360*64);
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( axAbs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XDrawArc(mDisplay, mDrawable,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( axAbs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          // start angle = 12 o'clock
          float a1 = -aAngle1 + 0.25;
          // positive = clockwise, negative = counter-clockwise
          float a2 = -aAngle2;
          XFillArc(mDisplay, mDrawable,mGC, aX1,aY1,(aX2-aX1+1),(aY2-aY1+1),a1*(360*64),a2*(360*64));
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------

    virtual void setTextSize(int aSize)
      {
      }

    virtual int textWidth(axString aText)
      {
        return XTextWidth(mFont, aText.ptr(), aText.length());
      }

    //----------

    virtual int textHeight(axString aText)
      {
        return mFont->ascent + mFont->descent;
      }

    //----------

    virtual void drawText(int aX, int aY, axString aText)
      {
        XDrawString(mDisplay,mDrawable,mGC,aX,aY+mFont->ascent,aText.ptr(),aText.length());
      }

    //--------

    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign)
      {
        int x,y;
        if (aAlign & ta_Top) y = aY1 + mFont->ascent;
        else if (aAlign & ta_Bottom) y = aY2 - mFont->descent;
        else y = aY1 + ((mFont->ascent)>>1) + ((aY2-aY1)>>1);
        int width = textWidth(aText);
        if (aAlign & ta_Left) x = aX1;
        else if (aAlign & ta_Right) x = aX2 - width;
        else x = aX1 + ((aX2 - aX1) >> 1) - ( width >> 1);
        XDrawString(mDisplay,mDrawable,mGC,x,y,aText.ptr(),aText.length());
      }

    //--------------------------------------------------
    // bitmap
    //--------------------------------------------------

    //XPutImage(display, d, gc, image, src_x, src_y, dest_x, dest_y, width, height)
    //        Display *display;
    //        Drawable d;
    //        GC gc;
    //        XImage *image;
    //        int src_x, src_y;
    //        int dest_x, dest_y;
    //        unsigned int width, height;
    //
    //display Specifies the connection to the X server.
    //d       Specifies the drawable.
    //gc      Specifies the GC.
    //image   Specifies the image you want combined with the rectangle.
    //src_x   Specifies the offset in X from the left edge of the image defined by the XImage structure.
    //src_y   Specifies the offset in Y from the top edge of the image defined by the XImage structure.
    //dest_x, dest_y  Specify the x and y coordinates, which are relative to the origin of the drawable and are the coordinates of the subimage.
    //width, height 	Specify the width and height of the subimage, which define the dimensions of the rectangle.

    virtual void drawBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        XPutImage(mDisplay,mDrawable,mGC,(XImage*)aBitmap->getHandle(),aSrcX,aSrcY,aX,aY,aSrcW,aSrcH);
      }

    //--------------------------------------------------
    // canvas (surface[drawable])
    //--------------------------------------------------

    //XCopyArea(display, src, dest, gc, src_x, src_y, width, height,  dest_x, dest_y)
    //      Display *display;
    //      Drawable src, dest;
    //      GC gc;
    //      int src_x, src_y;
    //      unsigned int width, height;
    //      int dest_x, dest_y;
    //
    //display       Specifies the connection to the X server.
    //src, dest     Specify the source and destination rectangles to be combined.
    //gc            Specifies the GC.
    //src_x, src_y 	Specify the x and y coordinates, which are relative to the origin of the source rectangle and specify its upper-left corner.
    //width,height 	Specify the width and height, which are the dimensions of both the source and destination rectangles.
    //dest_x,dest_y Specify the x and y coordinates, which are relative to the origin of the destination rectangle and specify its upper-left corner

    //virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void drawSurface(axSurfaceBase* aSurface, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //XCopyArea(mDisplay,aImage->getHandle(),mDrawable,mGC,aSrcX,aSrcY,aSrcW,aSrcH,aX,aY); // mWinHandle = dst
        XCopyArea(mDisplay,(Drawable)aSurface->getHandle(),mDrawable,mGC,aSrcX,aSrcY,aSrcW,aSrcH,aX,aY); // mWinHandle = dst
      }

    //void
    //XRenderComposite (Display   *dpy,
    //                  int       op,
    //                  Picture   src,
    //                  Picture   mask,
    //                  Picture   dst,
    //                  int       src_x,
    //                  int       src_y,
    //                  int       mask_x,
    //                  int       mask_y,
    //                  int       dst_x,
    //                  int       dst_y,
    //                  unsigned int  width,
    //                  unsigned int  height);

    //----------

    //	PictOp			Fa			Fb
    //	--------------------------------------------------
    //	Clear			      0			            0
    //	Src			        1			            0
    //	Dst			        0			            1
    //	Over			      1			            1-Aa
    //	OverReverse		  1-Ab			        1
    //	In			        Ab			          0
    //	InReverse		    0			            Aa
    //	Out			        1-Ab			        0
    //	OutReverse		  0			            1-Aa
    //	Atop			      Ab			          1-Aa
    //	AtopReverse		  1-Ab			        Aa
    //	Xor			        1-Ab			        1-Aa
    //	Add			        1			            1
    //	Saturate		    min(1,(1-Ab)/Aa)	1



    //	DisjointClear		0			            0
    //	DisjointSrc		  1			0
    //	DisjointDst		  0			1
    //	DisjointOver		1			min(1,(1-Aa)/Ab)
    //	DisjointOverReverse	min(1,(1-Ab)/Aa)	1
    //	DisjointIn		max(1-(1-Ab)/Aa,0)	0
    //	DisjointInReverse	0			max(1-(1-Aa)/Ab,0)
    //	DisjointOut		min(1,(1-Ab)/Aa)	0
    //	DisjointOutReverse	0			min(1,(1-Aa)/Ab)
    //	DisjointAtop		max(1-(1-Ab)/Aa,0)	min(1,(1-Aa)/Ab)
    //	DisjointAtopReverse	min(1,(1-Ab)/Aa)	max(1-(1-Aa)/Ab,0)
    //	DisjointXor		min(1,(1-Ab)/Aa)	min(1,(1-Aa)/Ab)
    //	ConjointClear		0			0
    //	ConjointSrc		1			0
    //	ConjointDst		0			1
    //	ConjointOver		1			max(1-Aa/Ab,0)
    //	ConjointOverReverse	max(1-Ab/Aa,0)		1
    //	ConjointIn		min(1,Ab/Aa)		0
    //	ConjointInReverse	0			min(Aa/Ab,1)
    //	ConjointOut		max(1-Ab/Aa,0)		0
    //	ConjointOutReverse	0			max(1-Aa/Ab,0)
    //	ConjointAtop		min(1,Ab/Aa)		max(1-Aa/Ab,0)
    //	ConjointAtopReverse	max(1-Ab/Aa,0)		min(1,Aa/Ab)
    //	ConjointXor		max(1-Ab/Aa,0)		max(1-Aa/Ab,0)



    //#define PictOpClear			    0
    //#define PictOpSrc			      1
    //#define PictOpDst			      2
    //#define PictOpOver			    3
    //#define PictOpOverReverse		4
    //#define PictOpIn			      5
    //#define PictOpInReverse			6
    //#define PictOpOut			      7
    //#define PictOpOutReverse		8
    //#define PictOpAtop			    9
    //#define PictOpAtopReverse		10
    //#define PictOpXor			      11
    //#define PictOpAdd			      12
    //#define PictOpSaturate			13
    //#define PictOpMaximum			  13
    //
    ///*
    // * Operators only available in version 0.2
    // */
    //#define PictOpDisjointMinimum			  0x10
    //#define PictOpDisjointClear			    0x10
    //#define PictOpDisjointSrc			      0x11
    //#define PictOpDisjointDst			      0x12
    //#define PictOpDisjointOver			    0x13
    //#define PictOpDisjointOverReverse		0x14
    //#define PictOpDisjointIn			      0x15
    //#define PictOpDisjointInReverse			0x16
    //#define PictOpDisjointOut			      0x17
    //#define PictOpDisjointOutReverse		0x18
    //#define PictOpDisjointAtop			    0x19
    //#define PictOpDisjointAtopReverse		0x1a
    //#define PictOpDisjointXor			      0x1b
    //#define PictOpDisjointMaximum			  0x1b
    //
    //#define PictOpConjointMinimum			    0x20
    //#define PictOpConjointClear			    0x20
    //#define PictOpConjointSrc			    0x21
    //#define PictOpConjointDst			    0x22
    //#define PictOpConjointOver			    0x23
    //#define PictOpConjointOverReverse		    0x24
    //#define PictOpConjointIn			    0x25
    //#define PictOpConjointInReverse			    0x26
    //#define PictOpConjointOut			    0x27
    //#define PictOpConjointOutReverse		    0x28
    //#define PictOpConjointAtop			    0x29
    //#define PictOpConjointAtopReverse		    0x2a
    //#define PictOpConjointXor			    0x2b
    //#define PictOpConjointMaximum			    0x2b
    //
    ///*
    // * Operators only available in version 0.11
    // */
    //#define PictOpBlendMinimum			    0x30
    //#define PictOpMultiply				    0x30
    //#define PictOpScreen				    0x31
    //#define PictOpOverlay				    0x32
    //#define PictOpDarken				    0x33
    //#define PictOpLighten				    0x34
    //#define PictOpColorDodge			    0x35
    //#define PictOpColorBurn				    0x36
    //#define PictOpHardLight				    0x37
    //#define PictOpSoftLight				    0x38
    //#define PictOpDifference			    0x39
    //#define PictOpExclusion				    0x3a
    //#define PictOpHSLHue				    0x3b
    //#define PictOpHSLSaturation			    0x3c
    //#define PictOpHSLColor				    0x3d
    //#define PictOpHSLLuminosity			    0x3e
    //#define PictOpBlendMaximum			    0x3e

    //virtual void renderImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void renderSurface(axSurfaceBase* aSurface, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        #ifdef AX_ALPHA
          int op = PictOpOver;
          //XRenderComposite(mDisplay,op,aImage->getPicture(),None,mPicture,aSrcX,aSrcY,0,0,aX,aY,aSrcW,aSrcH);
          XRenderComposite(mDisplay,op,(Picture)aSurface->getPicture(),None,mPicture,aSrcX,aSrcY,0,0,aX,aY,aSrcW,aSrcH);
        //#else
        //  drawImage(aImage,aX,aY,aSrcX,aSrcY,aSrcW,aSrcH);
        #endif
      }

    //----------

    //virtual void stretchImage(axImage* aImage, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void stretchSurface(axSurfaceBase* aSurface, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        #ifdef AX_ALPHA
        if (aW > 0)
        {
          if (aH > 0)
          {
            double xs = (float)aSrcW / (float)aW;
            double ys = (float)aSrcH / (float)aH;
            double zs = 1;
            XTransform xform =
            {
              {
                { XDoubleToFixed(xs), XDoubleToFixed(0 ), XDoubleToFixed(0 ) },
                { XDoubleToFixed(0 ), XDoubleToFixed(ys), XDoubleToFixed(0 ) },
                { XDoubleToFixed(0 ), XDoubleToFixed(0 ), XDoubleToFixed(zs) }
              }
            };
            //Picture pic = aImage->getPicture();
            Picture pic = (Picture)aSurface->getPicture();
            XRenderSetPictureTransform(mDisplay, pic, &xform );
            int op = PictOpOver;
            // hmmm.. is srcx, srcy transformed by the matrix too?
            float x = axFloor( (float)aSrcX / xs );
            float y = axFloor( (float)aSrcY / ys );
            XRenderComposite(mDisplay,op,pic,None,mPicture,(int)x,(int)y,0,0,aX,aY,aW,aH);
            XRenderSetPictureTransform(mDisplay, pic, &mat_ident );
          } //h>0
        } //w>0
        //#else
        #endif
      }

};

typedef axCanvasLinux axCanvasImpl;

//----------------------------------------------------------------------
#endif
