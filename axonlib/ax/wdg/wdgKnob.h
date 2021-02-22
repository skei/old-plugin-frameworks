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

#ifndef wdgKnob_included
#define wdgKnob_included
//----------------------------------------------------------------------

#include "wdg/wdgValue.h"

class wdgKnob : public wdgValue
{

  public:
    wdgKnob(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
            axString aName="", float aValue=0)
    : wdgValue(aListener,aRect,aAlignment,aName,aValue)
      {
        setFlag(wf_Vertical);
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
          mSkin->drawKnob(aCanvas,mRect,mName,mDisp,mValue/*,mSkinMode*/);
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

