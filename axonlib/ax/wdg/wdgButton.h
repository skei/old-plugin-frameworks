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

#ifndef wdgButton_included
#define wdgButton_included
//----------------------------------------------------------------------

#include "gui/axWidget.h"

#define bm_Switch 0
#define bm_Spring 1

class wdgButton : public axWidget
{
  private:
    bool mButtonDown;
  protected:
    bool      mState;
    axString  mOffText, mOnText;
    int       mTextAlign;
    int       mMode;

  public:
    wdgButton(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None,
              bool aState=false, axString aOffText="Off", axString aOnText="On", int aTextAlign=ta_Center, int aMode=bm_Switch)
    : axWidget(aListener,aRect,aAlignment)
      {
        mButtonDown = false;
        mMode = aMode;//bm_Switch;
        mState = aState;
        mOffText = aOffText;
        mOnText = aOnText;
        mTextAlign = aTextAlign;
      }

    //----------

    inline void setMode(int aMode) { mMode=aMode; }
    inline void setState(bool aState) { mState=aState; }
    inline bool getState(void) { return mState; }

    //----------

    inline void setText(axString aOffText, axString aOnText, int aTextAlign=ta_Center)
      {
        mOffText = aOffText;
        mOnText = aOnText;
        mTextAlign = aTextAlign;
      }

    //--------------------------------------------------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        bool state = false;;
        switch (mMode)
        {
          case bm_Switch:
            state = (mValue>0.5);
            break;
          case bm_Spring:
            state = mState;
            break;
        }
        if (mSkin) mSkin->drawButton(aCanvas,mRect,mState?mOnText:mOffText,mTextAlign,state/*,mSkinMode*/);
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left)
        {
          if (mValue>=0.5) mState=true; else mState=false;
          switch (mMode)
          {
            case bm_Switch:
              if (mState)
              {
                mState = false;
                mValue = 0;
              }
              else
              {
                mState = true;
                mValue = 1;
              }
              mListener->onChange(this);
              break;
            case bm_Spring:
              mButtonDown = true;
              mState = true;
              //mValue = 1;
              mListener->onRedraw(this);
              break;
          } //switch
        } //bu_Left
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (aButton==bu_Left && mButtonDown)
        {
          switch (mMode)
          {
            case bm_Switch:
              break;
            case bm_Spring:
              if (mButtonDown)
              {
                mButtonDown = false;
                mState = false;
                //mValue = 0;
                if (mRect.contains(aXpos,aYpos)) mListener->onChange(this);
                else mListener->onRedraw(this);
              }
              break;
          } //switch
        } //bu_Left
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        switch (mMode)
        {
          case bm_Switch:
            break;
          case bm_Spring:
            if (mButtonDown)
            {
              if (mRect.contains(aXpos,aYpos))
              {
                if (!mState)
                {
                  mState = true;
                  //mValue = 1;
                  mListener->onRedraw(this);
                }
              }
              else
              {
                mState = false;
                //mValue = 0;
                mListener->onRedraw(this);
              }
            }

            break;
        } //switch

      }

    //--------------------------------------------------

    virtual void doEnter(axWidget* aCapture)
      {
        //mListener->onCursor(cu_Hand);
        mListener->onCursor(cu_Finger);
        if (mMode==bm_Switch) mListener->onHint("wdgButton [switch]");
        if (mMode==bm_Spring) mListener->onHint("wdgButton [spring]");
      }

    //----------

    virtual void doLeave(axWidget* aCapture)
      {
        mListener->onCursor(DEF_CURSOR);
        mListener->onHint("");
      }

    //----------

};

//----------------------------------------------------------------------
#endif

