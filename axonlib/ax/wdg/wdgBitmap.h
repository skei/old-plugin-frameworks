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

#ifndef wdgBitmap_included
#define wdgBitmap_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgBitmap : public axWidget
{
  protected:
    axBitmap* mBitmap;
  public:
    wdgBitmap(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
              axBitmap* aBitmap=NULL)
    : axWidget(aListener,aRect,aAlignment)
      {
        //clearFlag(wf_Active);
        mBitmap = aBitmap;
      }

    //----------

    inline void setBitmap(axBitmap* aBitmap) { mBitmap = aBitmap; }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int dstx = mRect.x;
        int dsty = mRect.y;
        int srcx = 0;
        int srcy = 0;
        int srcw = mRect.w;
        int srch = mRect.h;
        aCanvas->drawBitmap(mBitmap, dstx,dsty, srcx,srcy,srcw,srch);
        axWidget::doPaint(aCanvas,aRect);
      }

    virtual void doEnter(axWidget* aCapture)
      {
        //mListener->onCursor(cu_ArrowLeftRight); }
        mListener->onHint("wdgBitmap");
      }

    virtual void doLeave(axWidget* aCapture)
      {
        //mListener->onCursor(DEF_CURSOR);
        mListener->onHint("");
      }

};

//----------------------------------------------------------------------
#endif

