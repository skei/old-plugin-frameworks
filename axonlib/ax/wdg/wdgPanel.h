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

#ifndef wdgPanel_included
#define wdgPanel_included
//----------------------------------------------------------------------

//#include "gui/axContainer.h"
#include "gui/axWidget.h"

class wdgPanel : public axWidget//axContainer
{
  public:
    wdgPanel(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        //trace("wdgPanel : aListener = " << aListener);
      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (isVisible() && mSkin)
        {
          mSkin->drawPanel(aCanvas,mRect/*,mSkinMode*/);
          axWidget::doPaint(aCanvas,aRect);
        }
      }
    //virtual void doEnter(axWidget* aCapture) { mListener->onCursor(cu_ArrowLeftRight); }
    //virtual void doLeave(axWidget* aCapture) { mListener->onCursor(DEF_CURSOR); }
};

//----------------------------------------------------------------------
#endif


