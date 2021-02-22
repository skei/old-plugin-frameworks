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

/**
  \file wdgGrid.h
  \brief grid widget
*/

#ifndef wdgGrid_included
#define wdgGrid_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class wdgGrid : public axWidget
{
  private:
    //axBrush*  mBackBrush;
    //axPen*    mGridPen;
  //
    int       mWidth;
    int       mHeight;

  public:

    wdgGrid(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
        //mBackBrush = new axBrush(AX_GREY_DARK);
        //mGridPen = new axPen(AX_GREY);
        mWidth  = 8;
        mHeight = 8;
      }

    virtual ~wdgGrid()
      {
        //delete mBackBrush;
        //delete mGridPen;
      }

    //----------

    void setup(int aWidth, int aHeight)
      {
        mWidth = aWidth;
        mHeight = aHeight;
      }

    //--------------------------------------------------
    // "grid listener"
    //--------------------------------------------------

    virtual void on_ClickCell(int aX, int aY, int aB) {}
    virtual void on_DrawCell(axCanvas* aCanvas, axRect aRect, int aX, int aY) {}

    //--------------------------------------------------
    // ..base
    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        if (mSkin)
        {
          if (mWidth>0 && mHeight>0)
          {
            int i;
            float xcell = ((float)mRect.w / (float)mWidth);
            float ycell = ((float)mRect.h / (float)mHeight);
            // background
            //aCanvas->selectBrush(mBackBrush);
            aCanvas->setBrushColor(mSkin->getDarkColor());
            aCanvas->fillRect(mRect.x,mRect.y,mRect.x2()-1,mRect.y2()-1);
            // cells
            for (int xx=0; xx<mWidth; xx++ )
            {
              for (int yy=0; yy<mHeight; yy++ )
              {
                axRect R = axRect( mRect.x+(xx*xcell), mRect.y+(yy*ycell), xcell,ycell);
                on_DrawCell(aCanvas,R,xx,yy);
              }
            }
            // grid
            float x = (float)mRect.x + xcell - 1;
            float y = (float)mRect.y + ycell - 1;
            //aCanvas->selectPen(mGridPen);
            aCanvas->setPenColor(mSkin->getLightColor());
            for (i=0; i<mWidth-1; i++ )
            {
              aCanvas->drawLine( (int)x, mRect.y, (int)x, mRect.y2() );
              x += xcell;
            } //width
            for (i=0; i<mHeight-1; i++ )
            {
              aCanvas->drawLine( mRect.x, (int)y, mRect.x2(), (int)y );
              y += ycell;
            } //height
          } //w&h>0
        } //mSkin
      }

    //----------

    virtual void doMouseDown(int aX, int aY, int aB)
      {
        float xcell  = ((float)mRect.w / (float)mWidth);
        float ycell  = ((float)mRect.h / (float)mHeight);
        int x = (int)axFloor(  (float)(aX-mRect.x) / xcell  );
        int y = (int)axFloor(  (float)(aY-mRect.y) / ycell  );
        on_ClickCell(x,y,aB);
        mListener->onChange(this);
      }

};

//----------------------------------------------------------------------
#endif
