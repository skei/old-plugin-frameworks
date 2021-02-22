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

#ifndef wdgSizer_included
#define wdgSizer_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

// sizer modes

#define sm_Window    -1
#define sm_None       0
#define sm_Horizontal 1
#define sm_Vertical   2

class wdgSizer : public axWidget
{
  protected:
    int       prevx,prevy;
    bool      mIsDragging;
    axWidget* mTarget;
    int       mSizeCursor;
    int       mMode;

  public:
    wdgSizer(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, int aMode=sm_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        mTarget     = NULL;
        mIsDragging = false;
        mMode       = aMode;
        switch (mMode)
        {
          case sm_None:       mSizeCursor = DEF_CURSOR;        break;
          case sm_Horizontal: mSizeCursor = cu_ArrowLeftRight; break;
          case sm_Vertical:   mSizeCursor = cu_ArrowUpDown;    break;
          case sm_Window:     mSizeCursor = cu_ArrowDiagRight; break;
        }
      }

    inline void setTarget(axWidget* aTarget) { mTarget=aTarget; }
    inline void setCursor(int aCursor) { mSizeCursor=aCursor; }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawSizer(aCanvas,mRect);
        //axWidget::doPaint(aCanvas,aRect);
      }

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left)
        {
          mIsDragging = true;
          prevx = aXpos;
          prevy = aYpos;
        }
      }

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left) mIsDragging = false;
      }

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mIsDragging)
        {
          int deltax = aXpos - prevx;
          int deltay = aYpos - prevy;
          if (mMode==sm_Horizontal) deltay=0;
          if (mMode==sm_Vertical)   deltax=0;
          if (mTarget) mTarget->onSize(this,deltax,deltay,mMode);
          else mListener->onSize(this,deltax,deltay,mMode);
          prevx = aXpos;
          prevy = aYpos;
        }
        //axWidget::doMouseMove(aXpos,aYpos,aButton);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    virtual void doEnter(axWidget* aCapture) { mListener->onCursor(mSizeCursor); }
    virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif
