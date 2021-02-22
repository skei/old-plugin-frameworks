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

#ifndef wdgLabel_included
#define wdgLabel_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgLabel : public axWidget
{
  protected:
    axString  mText;
    int       mTextAlign;
  public:
    wdgLabel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
             axString aText="", int aTextAlign=ta_Center)
    : axWidget(aListener,aRect,aAlignment)
      {
        clearFlag(wf_Active);
        mText = aText;
        mTextAlign = aTextAlign;
      }

    inline void setText(axString aText, int aTextAlign=ta_Center)
      {
        mText = aText;
        mTextAlign = aTextAlign;
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin) mSkin->drawLabel(aCanvas,mRect,mText,mTextAlign/*,mSkinMode*/);
        //axWidget::doPaint(aCanvas,aRect);
      }

    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }

};

//----------------------------------------------------------------------
#endif

