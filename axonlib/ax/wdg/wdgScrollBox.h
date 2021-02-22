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

#ifndef wdgScrollBox_included
#define wdgScrollBox_included
//----------------------------------------------------------------------
/*
  currently uses a ScrollBar and a panel
  todo: let user decide container instead of wdgPanel
*/

#include "gui/axWidget.h"
#include "wdg/wdgScrollBar.h"

class wdgScrollBox : public axWidget
{
  protected:
    wdgScrollBar* wScrollBar;
    axWidget*     wContainer;
    int           mScrolledX;   // how much we've already scrolled
    int           mScrolledY;   // -_"--
    float         mVisible;     // how much of the content is visible (calculated in doRealign)

  public:

    wdgScrollBox(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener, aRect, aAlignment)
      {
        axWidget::appendWidget( wScrollBar  = new wdgScrollBar(this,axRect(0,0,15,0),wa_Right) );
        //axWidget::appendWidget( wContainer  = new wdgPanel(this,NULL_RECT,wa_Client) );
        axWidget::appendWidget( wContainer  = new axWidget(this,NULL_RECT,wa_Client) );
        wContainer->setFlag(wf_Clip);
        wScrollBar->setFlag(wf_Vertical);
        wScrollBar->setThumbSize(1);
        mScrolledX = mScrolledY = 0;
        mVisible = 1;
      }

    //virtual ~wdgScrollBox()
    //  {
    //  }

    //--------------------------------------------------

    // accessors
    inline axWidget*      getContainer(void) { return wContainer; }
    inline wdgScrollBar*  getScrollBar(void) { return wScrollBar; }

    //void setContainer(axWidget* c)
    //{
    //  if (wContainer) delete wContainer; // safe?
    //  wContainer = c;
    //  mWidgets[1] = c;
    //  //doRealign();
    //  c->applySkin(mSkin,true,true);
    //}

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    // calculates the size of the thumb-bar (scrollbar), based on
    // mContent (calculated in doRealign), and how much off the
    // content that is visible

    void calc_thumbsize(void)
      {
        float height  = (float)wContainer->getRect().h;       // size of container
        float content = (float)wContainer->getContent().h;    // size of content
        if (content>height)
        {
          mVisible = height / content;
          wScrollBar->setThumbSize(mVisible);
        }
        else
        {
          mVisible = 1;
          wScrollBar->setThumbSize(1);
        }
      }

    //------------------------------

    // called from doRealign

    void un_scroll(void)
      {
        //do_scroll(-mScrolledX,-mScrolledY);
        wContainer->doScroll(mScrolledX,mScrolledY);
        mScrolledX = 0;
        mScrolledY = 0;
        wScrollBar->setValue(0);
      }

    //-----

    // x/y = 0..1

    void re_scroll(float x, float y)
      {
        float scrollable = (float)wContainer->getContent().h * (1-mVisible);
        int yscroll = (int)(scrollable*y);
        int dy = yscroll - mScrolledY;
        wContainer->doScroll(0,-dy);
        mScrolledX += 0;
        mScrolledY += dy;

      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    // append sub-widgets to wContainer, not self

    virtual int appendWidget(axWidget* aWidget)
      {
        int index = wContainer->appendWidget(aWidget);
        return index;
      }

    //--------------------------------------------------

    virtual void doRealign(void)
      {
        float y = wScrollBar->getValue();
        un_scroll();
        axWidget::doRealign();
        calc_thumbsize();
        re_scroll(0,y);
        wScrollBar->setValue(y);
      }

    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        if (aWidget==wScrollBar)
        {
          //trace("wdgScrollBax.onChange");
          float val = wScrollBar->getValue();
          float scrollable = (float)wContainer->getContent().h * (1-mVisible);
          int i = (int)(scrollable*val);
          int dy = i - mScrolledY;
          //trace("  val:" << val);
          //trace("  scrollable:" << scrollable);
          //trace("  i:" << i);
          //trace("  dy:" << dy);
          //do_scroll(0,dy);
          wContainer->doScroll(0,-dy);
          mScrolledX += 0;
          mScrolledY += dy;
          mListener->onRedraw(wContainer);
        }
        mListener->onChange(aWidget);
      }

};

//----------------------------------------------------------------------
#endif

