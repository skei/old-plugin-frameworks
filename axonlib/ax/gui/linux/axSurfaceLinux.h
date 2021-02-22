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

#ifndef axSurfaceLinux_included
#define axSurfaceLinux_included
//----------------------------------------------------------------------

//#ifdef AX_XRENDER
#ifdef AX_ALPHA
#include <X11/extensions/Xrender.h>
#endif

//#include "base/axContext.h"
#include "core/axDefines.h"
//#include "base/axSurfaceBase.h"
#include "gui/axSurfaceBase.h"
#include "gui/axCanvas.h"
//#include "axonlib.h"

//#include "gui/axCanvas.h"
class axCanvas;

// platform (linux) dependent things

class axSurfaceLinux : public axSurfaceBase
{
  private:
    //axFormat* mFormat;
    //axInterface*  mInterface;
    axBase*       mBase;
    Display*      mDisplay;
    Drawable      mDrawable;
    Pixmap        mPixmap;
    int           mPicture;

  //protected:
  //  int       mWidth;
  //  int       mHeight;
  //  int       mDepth;
  //  axCanvas* mCanvas;
  public:

    //Pixmap XCreatePixmap(display, d, width, height, depth)
    //  Display *display;           // Specifies the connection to the X server.
    //  Drawable d;                 // Specifies which screen the pixmap is created on.
    //  unsigned int width, height; // 	Specify the width and height, which define the dimensions of the pixmap.
    //  unsigned int depth; // Specifies the depth of the pixmap.

    axSurfaceLinux(axBase* aBase, void* aSource, int aWidth, int aHeight, int aDepth)
    : axSurfaceBase(aWidth,aHeight,aDepth)
      {
        mBase = aBase;
        //mInterface = aInterface;
        //mDisplay  = (Display*)aInterface->getHandle();
        mDisplay = (Display*)aBase->getInterface()->getHandle();
        mDrawable = *(Drawable*)aSource;
        mWidth    = aWidth;
        mHeight   = aHeight;
        mDepth    = aDepth;
        mPixmap   = XCreatePixmap(mDisplay,mDrawable,mWidth,mHeight,mDepth);
        mCanvas   = createCanvas();
        //#ifdef AX_XRENDER
        #ifdef AX_ALPHA
          XRenderPictFormat* fmt;
          if (mDepth==24)
            fmt = XRenderFindStandardFormat(mDisplay,PictStandardRGB24);
          else
            fmt = XRenderFindStandardFormat(mDisplay,PictStandardARGB32);
          XRenderPictureAttributes pict_attr;
          pict_attr.poly_edge = PolyEdgeSmooth;
          pict_attr.poly_mode = PolyModeImprecise;
          //pict_attr.component_alpha = true;
          int pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
          mPicture = XRenderCreatePicture(mDisplay,/*mDrawable*/mPixmap,fmt,pict_bits,&pict_attr);
          mCanvas->setPicture(mPicture);

        #endif


      }

    //----------

    virtual ~axSurfaceLinux()
      {
        delete mCanvas;
        #ifdef AX_ALPHA
        XRenderFreePicture(mDisplay,mPicture);
        #endif
        // The XFreePixmap() function first deletes the association between the pixmap ID and the pixmap.
        // Then, the X server frees the pixmap storage when there are no references to it.
        XFreePixmap(mDisplay,mPixmap);
      }

    //----------

    // called from axCanvas.blit
    virtual void* getHandle(void)  { return (void*)mPixmap; }
    virtual long getPicture(void)  { return mPicture; }

    //----------

    virtual  int      getWidth(void)  { return 0; }
    virtual  int      getHeight(void) { return 0; }
    virtual  int      getDepth(void)  { return 0; }
    virtual axCanvas* getCanvas(void) { return mCanvas; }

    //----------

    virtual axCanvas* createCanvas(void)
      {
        axCanvas* can = new axCanvas(mBase,(void*)&mPixmap);
        return can;
      }

};

typedef axSurfaceLinux axSurfaceImpl;

#endif
//----------------------------------------------------------------------
