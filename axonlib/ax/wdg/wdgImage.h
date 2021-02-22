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

#ifndef wdgImage_included
#define wdgImage_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgImage : public axWidget//axContainer
{
  protected:
    bool            mSurfaceLoaded;
    axSurface*      mSurface;
    //axImage*        mImage;
    axSurfaceBase*  mImage;

  public:

    wdgImage(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
             //axImage* aImage=NULL)
             axSurfaceBase* aImage=NULL)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
        mSurfaceLoaded = false;
        mImage = aImage;
      }

    //----------

    virtual ~wdgImage()
      {
        if (mSurfaceLoaded) delete mSurface;
      }

    //----------

    //inline void setImage(axImage* aImage) { mImage = aImage; }
    inline void setImage(axSurfaceBase* aImage) { mImage = aImage; }

    //----------

    virtual void loadBitmap(axEditor* aEditor, char* aBuffer, int aWidth, int aHeight, int aDepth)
    {
      axBitmap* bitmap = aEditor->createBitmap(aWidth,aHeight,aDepth);
      bitmap->createBuffer(aBuffer);
      bitmap->convertRgbaBgra();
      bitmap->premultAlpha();
      bitmap->prepare();
      //return bitmap;
      /*axSurface**/ mSurface = aEditor->createSurface(aWidth,aHeight,aDepth);
      mSurface->getCanvas()->drawBitmap( bitmap, 0,0, 0,0,aWidth,aHeight );
      mImage = mSurface;
      mSurfaceLoaded = true;
      delete bitmap;
    }

    //----------

    // todo: clipping

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mImage)
        {
          int dstx = mRect.x;
          int dsty = mRect.y;
          int srcx = 0;
          int srcy = 0;
          int srcw = mRect.w;
          int srch = mRect.h;
          //aCanvas->drawImage(mImage,dstx,dsty, srcx,srcy,srcw,srch);
          aCanvas->drawSurface(mImage,dstx,dsty, srcx,srcy,srcw,srch);
        }
        axWidget::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

