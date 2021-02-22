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

#ifndef wdgValue_included
#define wdgValue_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"
#include "core/axStdlib.h"


class wdgValue : public axWidget
{
  //private:
  protected:
    bool  mIsDragging;
    int   mClickX;
    int   mClickY;
    float mClickVal;
    char  mDisp[16];
  protected:
    //axString  mName;
    float     mSens1;
    float     mSens2;

  public:

    wdgValue(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
             axString aName="", float aValue=0)
    : axWidget(aListener,aRect,aAlignment)
      {
        //trace("wdgValue : aListener = " << aListener);
        mName = aName;
        mValue = aValue;
        mIsDragging = false;
        mSens1 = 0.005;
        mSens2 = 0.05;
      }

    //----------

    inline void setSensitivity(float aSens1, float aSens2=0.05) { mSens1=aSens1; mSens2=aSens2; }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left)
        {
          mIsDragging = true;
          mClickX = aXpos;
          mClickY = aYpos;
          mClickVal = mValue;
        }
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left) mIsDragging = false;
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mIsDragging)
        {
          //trace("doMouseMove, dragging");
          int dx = aXpos - mClickX;
          int dy = aYpos - mClickY;
          float v;
          if (hasFlag(wf_Vertical)) v = -dy;
          else v = dx;

          float s = mSens1;
          if (aButton&bu_Ctrl) s*=mSens2;
          v *= s;
          mValue = mClickVal + v;
          mValue = axMin(1,mValue);
          mValue = axMax(0,mValue);
          //trace("mListener: " << mListener);
          mListener->onChange(this);
          //trace("ok");
        }
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin)
        {
          axFtoa(mDisp,mValue);
          mSkin->drawValue(aCanvas,mRect,mName,mDisp,mValue/*,mSkinMode*/);
        }
      }

    //----------

    virtual void doEnter(axWidget* aCapture)
      {
        if (hasFlag(wf_Vertical)) mListener->onCursor(cu_ArrowUpDown);
        else mListener->onCursor(cu_ArrowLeftRight);
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
      }

};

//----------------------------------------------------------------------
#endif

