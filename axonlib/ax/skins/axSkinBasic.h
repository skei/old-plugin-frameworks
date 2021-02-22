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

#ifndef axSkinBasic_included
#define axSkinBasic_included
//----------------------------------------------------------------------

#include "gui/axSkin.h"

class axSkinBasic : public axSkin
{

  public:

    axSkinBasic(axCanvas* aCanvas)
    //: axSkin(aCanvas)
      {
        mFillColor  = aCanvas->getColor(AX_GREY);
        mLightColor = aCanvas->getColor(AX_GREY_LIGHT);
        mDarkColor  = aCanvas->getColor(AX_GREY_DARK);
        mTextColor  = aCanvas->getColor(AX_WHITE);
      }

    virtual ~axSkinBasic()
      {
      }

    //--------------------------------------------------
    // internal
    //--------------------------------------------------

    virtual void fill_back(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mFillColor);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }

    //----------

    virtual void fill_dark(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mDarkColor);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }

    //----------

    virtual void fill_light(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mLightColor);
        aCanvas->fillRect(aRect.x,aRect.y,aRect.x2(),aRect.y2());
      }

    //----------

    virtual void draw_frame(axCanvas* aCanvas, axRect aRect, bool aDown=false)
      {
        if (aDown) aCanvas->setPenColor(mDarkColor);
        else aCanvas->setPenColor(mLightColor);
        aCanvas->drawLine(aRect.x,aRect.y,aRect.x2()-1,aRect.y);
        aCanvas->drawLine(aRect.x,aRect.y,aRect.x,aRect.y2()-1);
        if (aDown) aCanvas->setPenColor(mLightColor);
        else aCanvas->setPenColor(mDarkColor);
        aCanvas->drawLine(aRect.x+1,aRect.y2(),aRect.x2(),aRect.y2());
        aCanvas->drawLine(aRect.x2(),aRect.y+1,aRect.x2(),aRect.y2());
        //aCanvas->setTextColor(mTextColor);
      }

    //----------

    virtual void draw_text(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign)
      {
        aCanvas->setTextColor(mTextColor);
        aCanvas->drawText(aRect.x,aRect.y,aRect.x2(),aRect.y2(),aText,aTextAlign);
      }

    //--------------------------------------------------
    //
    // draw...
    //
    //--------------------------------------------------

    virtual void drawPanel(axCanvas* aCanvas, axRect aRect, int aMode=0)
      {
        fill_back(aCanvas,aRect);
        draw_frame(aCanvas,aRect);
      }

    //----------

    virtual void drawSizer(axCanvas* aCanvas, axRect aRect, int aMode=0)
      {
        fill_dark(aCanvas,aRect);
      }

    //----------

    virtual void drawLabel(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, int aMode=0)
      {
        draw_text(aCanvas,aRect,aText,aTextAlign);
      }

    //----------

    virtual void drawValue(axCanvas* aCanvas, axRect aRect, axString aName, axString aDisp, float aValue, int aMode=0)
      {
        fill_dark( aCanvas,aRect);
        axRect r = aRect;
        r.add(3,3,-6,-6);
        draw_text(aCanvas,r,aName,ta_Left);
        draw_text(aCanvas,r,aDisp,ta_Right);
      }

    //----------

    virtual void drawButton(axCanvas* aCanvas, axRect aRect, axString aText, int aTextAlign, bool aState, int aMode=0)
      {
        fill_back( aCanvas,aRect);
        draw_frame(aCanvas,aRect,aState);
        draw_text( aCanvas,aRect,aText,aTextAlign);
      }

    //----------

    virtual void drawSlider(axCanvas* aCanvas, axRect aRect, float aValue, axString aText1, axString aText2, bool aVertical, int aMode=0)
      {
        if (aVertical)
        {
          int size = (int)((float)aRect.h * (1-aValue));
          int y1 = aRect.y;
          int y2 = aRect.y+size;
          int h1 = size;
          int h2 = aRect.h-size;
          fill_dark( aCanvas,axRect(aRect.x, y1, aRect.w, h1 ));
          fill_back( aCanvas,axRect(aRect.x, y2, aRect.w, h2 ));
          draw_frame(aCanvas,axRect(aRect.x, y2, aRect.w, h2 ));
        }
        else
        {
          int size = (int)((float)aRect.w * aValue);
          fill_back( aCanvas,axRect(aRect.x,     aRect.y, size,        aRect.h));
          draw_frame(aCanvas,axRect(aRect.x,     aRect.y, size,        aRect.h));
          fill_dark( aCanvas,axRect(aRect.x+size,aRect.y, aRect.w-size,aRect.h));
          axRect r = aRect;
          r.add(3,3,-6,-6);
          draw_text(aCanvas,r,aText1,ta_Left);
          draw_text(aCanvas,r,aText2,ta_Right);
        }
      }

    //----------

    virtual void drawKnob(axCanvas* aCanvas, axRect aRect, axString aName, axString aDisp, float aValue, int aMode=0)
      {
        int x  = aRect.x;
        int y  = aRect.y;
        int size = axMinInt(aRect.w,aRect.h);
        aCanvas->setPenColor(mDarkColor);
        aCanvas->setPenWidth(5);
        aCanvas->drawArc(x+2,y+2,x+(size-2),y+(size-2),0.6,0.8);
        aCanvas->resetPen();
        aCanvas->setPenColor(mLightColor);
        aCanvas->setPenWidth(5);
        aCanvas->drawArc(x+2,y+2,x+(size-2),y+(size-2),0.6,aValue*0.8);
        aCanvas->resetPen();
        int th = aCanvas->textHeight("Xj");
        if (aRect.h >= (2*th))
        {
          aCanvas->setTextColor(mTextColor);
          aCanvas->drawText(x+size+8,y,   aRect.x2(),aRect.y2(),aName,ta_Top|ta_Left);
          aCanvas->drawText(x+size+8,y+th,aRect.x2(),aRect.y2(),aDisp,ta_Top|ta_Left);
        //aCanvas->drawText(x+size+5,y,   aRect.x2(),aRect.y2(),aDisp,ta_Bottom|ta_Left);
        }
        else
        {
          aCanvas->setTextColor(mTextColor);
          aCanvas->drawText(x+size+5,y,aRect.x2(),aRect.y2(),aDisp,ta_Left);
        }
      }

    //----------

    virtual void drawScrollBar(axCanvas* aCanvas, axRect aRect, float aValue, bool aVertical, float aThumbSize, int aMode=0)
      {
        fill_dark(aCanvas,aRect);
        if (aVertical)
        {
          int thumb = (int)((float) aRect.h*aThumbSize);
          int ipos  = (int)((float)(aRect.h-thumb)*aValue);
          int x = aRect.x;
          int y = aRect.y + ipos;
          int x2 = aRect.x2();
          int y2 = y + thumb;
          int w = (x2-x+0);
          int h = (y2-y+0);
          fill_back(aCanvas,axRect(x,y,w,h));
          draw_frame(aCanvas,axRect(x,y,w,h)/*,false*/);
        }
        else
        {
          int thumb = (int)((float) aRect.w*aThumbSize);
          int ipos  = (int)((float)(aRect.w-thumb)*aValue);
          int x = aRect.x + ipos;
          int y = aRect.y;
          int x2 = x+thumb;
          int y2 = aRect.y2();
          int w = (x2-x+0);
          int h = (y2-y+0);
          fill_back(aCanvas,axRect(x,y,w,h));
          draw_frame(aCanvas,axRect(x,y,w+1,h+1)/*,false*/);
        }
      }

};

//----------------------------------------------------------------------
#endif
