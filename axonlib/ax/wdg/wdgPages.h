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

#ifndef wdgPages_included
#define wdgPages_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgPages : public axWidget
{
  protected:
    int   mActivePage;
  public:

    wdgPages(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        mActivePage = 1;
      }

    //----------

    int appendPage(axWidget* aPage)
      {
        aPage->clearFlag(wf_Active);
        aPage->clearFlag(wf_Visible);
        aPage->doSetSize(mRect.w,mRect.h);
        int index = axWidget::appendWidget(aPage);
        return index;
      }

    //----------

    void setPage(int aPage,bool redraw=false)
      {
        if (aPage>=mWidgets.size()) aPage = mWidgets.size()-1;
        //if( aPage!=mActivePage)
        //{
          for( int i=0; i<mWidgets.size(); i++ )
          {
            axWidget* wdg = mWidgets[i];
            if (i==aPage)
            {
              wdg->setFlag(wf_Active);
              wdg->setFlag(wf_Visible);
            }
            else
            {
              wdg->clearFlag(wf_Active);
              wdg->clearFlag(wf_Visible);
            }
          } //for
          //mPages->initMouseState();
          mActivePage = aPage;
          if (redraw) mListener->onRedraw(this);
        //} // !=aIndex
      }


};

//----------------------------------------------------------------------
#endif
