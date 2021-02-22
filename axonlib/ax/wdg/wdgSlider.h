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

#ifndef wdgSlider_included
#define wdgSlider_included
//----------------------------------------------------------------------

#include "core/axDefines.h"
#include "wdg/wdgValue.h"

//class wdgSlider : public axWidget
class wdgSlider : public wdgValue
{
  public:
    wdgSlider(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
              axString aName="", float aValue=0, bool aVertical=false)
    : wdgValue(aListener,aRect,aAlignment,aName,aValue)
      {
        mIsDragging = false;
        if (aVertical)
        {
          setFlag(wf_Vertical);
          if (aRect.h>0) mSens1 = 1.0f/(float)aRect.h;
        }
        else
        {
          clearFlag(wf_Vertical);
          if (aRect.w>0) mSens1 = 1.0f/(float)aRect.w;
        }
      }

    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        axWidget::doSetSize(aWidth,aHeight);
        if (hasFlag(wf_Vertical)) { if (aHeight>0) mSens1 = 1.0f/(float)aHeight; }
        else                      { if (aWidth >0) mSens1 = 1.0f/(float)aWidth; }
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin)
        {
          if (mParameter)
          {
            char labelbuf[32];
            mParameter->doGetDisplay(mDisp);
            mParameter->doGetLabel(labelbuf);
            axStrcat(mDisp," ");
            axStrcat(mDisp,labelbuf);
          }
          else axFtoa(mDisp,mValue);
          mSkin->drawSlider(aCanvas,mRect,mValue,mName,mDisp,hasFlag(wf_Vertical)/*,mSkinMode*/);
        }
      }

    //----------

    //virtual void doEnter(axWidget* aCapture)
    //  {
    //    if (hasFlag(wf_Vertical)) mListener->onCursor(cu_ArrowUpDown);
    //    else mListener->onCursor(cu_ArrowLeftRight);
    //  }

    //----------

    //virtual void doLeave(axWidget* aCapture)
    //  {
    //    mListener->onCursor(DEF_CURSOR);
    //  }

};

//----------------------------------------------------------------------
#endif

