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

#ifndef axWidget_included
#define axWidget_included
//----------------------------------------------------------------------
/*

notes:
- a widget knows about a listener, but not a parent.
  the listener has a set of methods (see the axWidgetListener methods)
  that can be called, and depending on widget-type, this results in some response,
  or it can be further passed on to that widget's listener,
- a widget has a list of sub-widgets, but theses widgets doesn't necessarily know
  about the parent/owner, only a listener. often this refers back to the same parent,
  and thus have some of the same effect (but not all, only the listener-methods)
- the rect is relative to its parent. depending on widget alignment, it might get
  moved and resized, and only some of the fields in the rect struct might have any effect
  (see examples, and doRealign()

TODO:
- realign, take spacing & borders into account
- clipping, --"--
- clip-stack

*/
//----------------------------------------------------------------------

//#include "core/axRect.h"
#include "gui/axCanvas.h"
#include "gui/axSkin.h"
#include "par/axParameter.h"

//----------------------------------------------------------------------

// widget alignment
#define wa_None         0
#define wa_Client       1
#define wa_Left         2
#define wa_Right        3
#define wa_Top          4
#define wa_Bottom       5
#define wa_LeftTop      6
#define wa_RightTop     7
#define wa_LeftBottom   8
#define wa_RightBottom  9
#define wa_TopLeft      10
#define wa_TopRight     11
#define wa_BottomLeft   12
#define wa_BottomRight  13
//#define wa_Stacked      14
#define wa_StackedHoriz 15
#define wa_StackedVert  16

//----------

// widget flags
#define wf_None         0
#define wf_Active       1
#define wf_Visible      2
#define wf_Capture      4
#define wf_Hover        8
#define wf_Align        16
#define wf_Vertical     32
#define wf_Clip         64
#define wf_Fill         128

//----------------------------------------------------------------------
//
// widget listener
//
//----------------------------------------------------------------------

class axWidget;
typedef axArray<axWidget*> axWidgets;

//----------

// used when you need to notify the 'owner' about something
class axWidgetListener
{
  public:
    virtual void onChange(axWidget* aWidget) {}
    virtual void onRedraw(axWidget* aWidget) {}
    virtual void onCursor(int aCursor/*=DEF_PENWIDTH*/) {}
    virtual void onHint(axString aHint) {}
    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode) {}
    virtual void onModal(bool aModal, axWidget* aWidget) {}
};

//----------------------------------------------------------------------
//
// widget
//
//----------------------------------------------------------------------

class axWidget : public axWidgetListener
{
  protected:
    axString          mName;
    int               mFlags;
    axRect            mRect;
    axRect            mOrig;
    int               mAlignment;
    float             mValue;
    int               mConnection;            // which parameter (if any) this is conected to (set in axEditor.connect)
    axParameter*      mParameter;             // direct access to the parameter (set in axEditor.connect)
    int               mMinWidth, mMinHeight;
    int               mMaxWidth, mMaxHeight;
    int               mMarginX,  mMarginY;    // container inner space (between outer border & widgets)
    int               mPaddingX, mPaddingY;   // space between wal_Stacked widgets
    axSkin*           mSkin;
    //int               mSkinMode;
  protected:
    axWidgetListener* mListener;
    axWidgets         mWidgets;
    axWidget*         mCapturedWidget;
    axWidget*         mHoverWidget;
    axRect            mClient;                // current Client area
    axRect            mContent;               // rect encapsulating all sub-widgets (updated in doRealign)
    int               mStackedX, mStackedY;   // where to put next wal_Stacked widget
    int               mIndex;
  public:
    int               mId;
    void*             mPtr;

  public:

    axWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
      {
        mName           = "";
        mFlags          = wf_Active|wf_Visible|wf_Capture|wf_Align;
        mRect           = aRect;
        mOrig           = mRect;
        mAlignment      = aAlignment;
        mValue          = 0;
        mConnection     = -1;
        mParameter      = NULL;
        mMinWidth       = 0;
        mMinHeight      = 0;
        mMaxWidth       = 999999;
        mMaxHeight      = 999999;
        mMarginX        = 0;
        mMarginY        = 0;
        mPaddingX       = 0;
        mPaddingY       = 0;
        mSkin           = NULL;
        //mSkinMode       = 0;

        mListener       = aListener;
        mCapturedWidget = NULL;
        mHoverWidget    = this;
        //mModalWidget    = NULL;
        //mModalIndex     = -1;
        mClient         = mRect;
        mContent        = NULL_RECT;
        mStackedX       = 0;
        mStackedY       = 0;

        mIndex = -1;

        mId             = 0;      //aId;
        mPtr            = NULL;   //aPtr;
      }

    virtual ~axWidget()
      {
        #ifndef AX_NOAUTODELETE
        deleteWidgets();
        #endif
      }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    inline void setName(axString aName) { mName=aName; }
    inline void setListener(axWidgetListener* aListener) { mListener=aListener; }

    inline int  getIndex(void) { return mIndex; }
    inline void setIndex(int aNum) { mIndex = aNum; }

    //inline void setSkinMode(int aMode) { mSkinMode = aMode; }

    // flags

    inline void         setFlag(int aFlag)                    { mFlags |= aFlag; }
    inline void         setAllFlags(int aFlags)               { mFlags = aFlags; }
    inline void         clearFlag(int aFlag)                  { mFlags &= ~aFlag; }
    inline bool         hasFlag(int aFlag)                    { return (mFlags&aFlag); }
    inline axRect       getRect(void)                         { return mRect; }
    inline int          getFlags(void)                        { return mFlags; }

    inline bool         isActive(void)                        { return (mFlags&wf_Active); }
    inline bool         isVisible(void)                       { return (mFlags&wf_Visible); }
    inline bool         isClipping(void)                      { return (mFlags&wf_Clip); }
    inline bool         canCapture(void)                      { return (mFlags&wf_Capture); }

    // rect

    virtual bool        intersects(axRect aRect)              { return mRect.intersects(aRect); }
    virtual bool        contains(int aXpos, int aYpos)        { return mRect.contains(aXpos,aYpos); }

    // parameter

    inline float        getValue(void)                        { return mValue; }
    inline void         setValue(float aValue)                { mValue=aValue; }

    inline int          getConnection(void)                   { return mConnection; }
    inline void         setConnection(int aNum)               { mConnection=aNum; }

    inline axParameter* getParameter(void)                    { return mParameter; }
    inline void         setParameter(axParameter* aParameter) { mParameter=aParameter; }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    //virtual void initMouseState(void)
    //  {
    //    mCapturedWidget = NULL;
    //  }

    //----------

    //virtual void goModal(axWidget* aWidget)
    //  {
    //    //unCapture();
    //    mModalWidget = aWidget;
    //  }

    //virtual void unCapture(void)
    //  {
    //    if (mCapturedWidget)
    //    {
    //      mCapturedWidget=NULL;
    //      for (int i=0; i<mWidgets.size(); i++) mWidgets[i]->unCapture();
    //    }
    //  }

    //--------------------------------------------------
    //
    //
    //
    //--------------------------------------------------

    virtual axRect getContent(void)         { return mContent; }
    virtual int getNumWidgets(void)         { return mWidgets.size(); }
    virtual axWidget* getWidget(int aIndex) { return mWidgets[aIndex]; }

    //--------------------------------------------------

    virtual int appendWidget(axWidget* aWidget)
      {
        //trace("axWidget.appendWidget");
        int index = mWidgets.size();
        aWidget->applySkin(mSkin,true,true);
        //aWidget->doSetPos( mRect.x + aWidget->getRect().x, mRect.y + aWidget->getRect().y );
        mWidgets.append(aWidget);
        return index;
      }

    //----------

    virtual void removeWidget(int aIndex)
      {
        mWidgets.remove(aIndex);
      }

    //----------

    virtual void deleteWidgets(void)
      {
        for (int i=0; i<mWidgets.size(); i++) delete mWidgets[i];
        mWidgets.clear();
      }

    //------------------------------

    virtual void setBorders(int aMarginX, int aMarginY, int aPaddingX, int aPaddingY)
      {
        mMarginX  = aMarginX;
        mMarginY  = aMarginY;
        mPaddingX = aPaddingX;
        mPaddingY = aPaddingY;
      }

    //----------

    virtual void setLimits(int aMinW, int aMinH, int aMaxW=999999, int aMaxH=999999)
      {
        mMinWidth   = aMinW;
        mMinHeight  = aMinH;
        mMaxWidth   = aMaxW;
        mMaxHeight  = aMaxH;
      }

    //----------

    virtual void applySkin(axSkin* aSkin, bool aSub=false, bool aOnlyIfNull=false)
      {
        //trace("axWidget.setSkin");
        if (aOnlyIfNull) { if (!mSkin) mSkin=aSkin; }
        else mSkin = aSkin;
        if (aSub) { for (int i=0; i<mWidgets.size(); i++) mWidgets[i]->applySkin(aSkin,aSub,aOnlyIfNull); }
      }

    //--------------------------------------------------
    //
    // do...
    //
    //--------------------------------------------------

    // find first widget that contains x,y
    // or 'self' is no sub-widgets found or hit
    //
    // start searching from end of list, so that widgets that are painted last
    // (topmost) are found first.

    virtual axWidget* doFindWidget(int aXpos, int aYpos)
      {
        axWidget* widget = this;
        int num = mWidgets.size();
        if (num>0)
        {
          for (int i=num-1; i>=0; i--)
          {
            axWidget* w = mWidgets[i];
            if (w->isActive())
            {
              if (w->contains(aXpos,aYpos))
              {
                widget = w->doFindWidget(aXpos,aYpos);
                if (widget!=w) return widget;
                else return w;
              } //contains
            } //active
          } //for num
        } //num>0
        return widget;
      }

    //----------

    virtual void doSetPos(int aXpos, int aYpos)
      {
        int deltax = aXpos - mRect.x;
        int deltay = aYpos - mRect.y;
        for (int i=0; i<mWidgets.size(); i++)
        {
          axWidget* wdg = mWidgets[i];
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          wdg->doSetPos( wx+deltax, wy+deltay );
        }
        mRect.setPos(aXpos,aYpos);
      }

    //----------

    virtual void doSetSize(int aWidth, int aHeight)
      {
        if (aWidth  < mMinWidth)  aWidth  = mMinWidth;
        if (aWidth  > mMaxWidth)  aWidth  = mMaxWidth;
        if (aHeight < mMinHeight) aHeight = mMinHeight;
        if (aHeight > mMaxHeight) aHeight = mMaxHeight;
        mRect.setSize(aWidth,aHeight);
        //doRealign();
      }

    //----------

    // not used??

    //virtual void doMove(int aDeltaX, int aDeltaY)
    //  {
    //    //mRect.setPos(x,y);
    //    int x = mRect.x + aDeltaX;
    //    int y = mRect.y + aDeltaY;
    //    this->doSetPos(x,y);
    //  }

    //----------

    // only used in wdgScrollBox.doResize
    // (and that is not colled from anywhere???

    //virtual void doResize(int aDeltaX, int aDeltaY)
    //  {
    //    int w = mRect.w + aDeltaX;
    //    int h = mRect.h + aDeltaY;
    //    this->doSetSize(w,h);
    //    //doRealign();
    //  }

    //----------

    // move sub-widgets only

    virtual void doScroll(int dX, int dY)
      {
        for( int i=0;i<mWidgets.size(); i++ )
        {
          axWidget* wdg = mWidgets[i];
          int wx = wdg->getRect().x;
          int wy = wdg->getRect().y;
          wdg->doSetPos( wx + dX, wy + dY );
        }
      }

    //----------

    // realign sub-widgets according to their alignment setting.
    // also, it keeps track of a mContent rectangle, that encapsulates all the sub-widgets

    //TODO. update stackx,stacky when we update the available client area
    //      so we can interleave them...

    /*

    M = margin
    P = padding (between widgets when stacked)

     ________________________________________ _ _          _ ____
    |          ^                                                  |
    |          M                                                  |
    |      ____v___       ________       ____ _            _      |
    |     |        |     |        |     |                   |     |
    |<-M->|        |<-P->|        |<-P->|                   |<-M->|
    |     |________|     |________|     |______ _       _ __|     |
    |          ^                                                  :
    |          P
    |      ____v____      ___ _
    |     |         |    |
    |     :         .
    |

    TODO:
    - skip widget if not enough space for it?
      (null or negative space left)
    - break up this (too large) function into smaller pieces
      to make it easier to follow and see any overview...

    */

    virtual void doRealign(void)
      {
        if (mFlags&wf_Align)
        {
          axRect parent = mRect;
          parent.add( mMarginX, mMarginY, -(mMarginX*2), -(mMarginY*2) );
          axRect client = parent;

          //mContent.set(0,0,0,0);//mMarginX*2,mMarginY*2);
          mContent.set( mRect.x, mRect.y,0,0);

          int stackx   = client.x;
          int stacky   = client.y;
          int largestw = 0;
          int largesth = 0;

          for( int i=0; i<mWidgets.size(); i++ )
          {
            axWidget* wdg = mWidgets[i];
            int ww = wdg->getRect().w;  // current widget width
            int wh = wdg->getRect().h;  // height
            switch (wdg->mAlignment)
            {

              //  _____
              // |  _  |
              // | |_| |
              // |_____|
              //

              case wa_None:
                wdg->doSetPos(wdg->mOrig.x+parent.x, wdg->mOrig.y+parent.y);
                break;

              //   _____
              //  |     |
              //  |     |
              //  |_____|
              //

              case wa_Client:
                wdg->doSetPos(  client.x, client.y );
                wdg->doSetSize( client.w, client.h );
                break;

//----------

              //  _____
              // |  |
              // |  |
              // |__|__
              //

              case wa_Left:

                wdg->doSetPos( client.x, client.y );
                wdg->doSetSize( ww, client.h );
                client.x += (ww+mPaddingX);
                client.w -= (ww+mPaddingX);
                stackx    = client.x;
                stacky    = client.y;
                largesth  = 0;

                break;

              //  ______
              //     |  |
              //     |  |
              //  __ |__|
              //

              case wa_Right:

                wdg->doSetPos( client.x2()-ww+1, client.y );
                wdg->doSetSize( ww, client.h );
                client.w -= (ww + mPaddingX);
                break;

              //  ______
              // |______|
              // |      |
              //

              case wa_Top:

                wdg->doSetPos( client.x, client.y );
                wdg->doSetSize( client.w, wh );
                client.y += (wh+mPaddingY);
                client.h -= (wh+mPaddingY);
                stackx    = client.x;
                stacky    = client.y;
                largestw  = 0;
                break;

              //
              // |      |
              // |______|
              // |______|
              //

              case wa_Bottom:

                wdg->doSetPos( client.x, client.y2()-wh+1 );
                wdg->doSetSize( client.w, wh );
                client.h -= (wh+mPaddingY);
                break;

//----------

              //  __________
              // |    |
              // |____|
              // |    :
              //      .

              case wa_LeftTop:
                wdg->doSetPos( client.x, client.y );
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                stackx = client.x;
                stacky = client.y;
                break;

              //  __________
              //      |     |
              //      |_____|
              //      :     |
              //      .

              case wa_RightTop:

                wdg->doSetPos( client.x2()-ww+1, client.y );
                client.w -= (ww + mPaddingX);
                break;

              //   __________
              //  |    .     |
              //  |____:     |
              //  |    |     |
              //  |____|_____|
              //

              case wa_LeftBottom:

                wdg->doSetPos( client.x, client.y2()-wh+1 );
                client.x += (ww + mPaddingX);
                client.w -= (ww + mPaddingX);
                stackx = client.x;
                stacky = client.y;
                break;

              //  __________
              //      .     |
              //      :_____|
              //      |     |
              //  ____|_____|
              //

              case wa_RightBottom:

                wdg->doSetPos( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doSetSize( ww, client.h );
                client.w -= (ww + mPaddingX);
                break;

//----------

              //  __________
              // |    |
              // |____|.....
              // |
              //

              case wa_TopLeft:
                wdg->doSetPos( client.x, client.y );
                client.y += (wh + mPaddingY);
                client.h -= (wh + mPaddingY);
                stackx = client.x;
                stacky = client.y;
                break;

              //  __________
              //        |   |
              //   .....|___|
              //            |
              //

              case wa_TopRight:

                wdg->doSetPos( client.x2()-ww+1, client.y );
                //wdg->doSetSize( ww, client.h );
                client.y += (wh + mPaddingY);
                client.h -= (wh + mPaddingY);
                stackx = client.x;
                stacky = client.y;
                break;

              //   __________
              //  |          |
              //  |___ ...   |
              //  |   |      |
              //  |___| _____|
              //

              case wa_BottomLeft:

                wdg->doSetPos( client.x, client.y2()-wh+1 );
                //client.y += (wh + mPaddingY);
                client.h -= (wh + mPaddingY);
                break;

              //  __________
              //      .     |
              //      :_____|
              //      |     |
              //  ____|_____|
              //

              case wa_BottomRight:

                wdg->doSetPos( client.x2()-ww+1, client.y2()-wh+1 );
                //wdg->doSetSize( ww, client.h );
                client.h -= (wh + mPaddingY);
                break;

//----------

              //  __________________
              // |    |    |   |
              // |____|____|___|...... .
              // |
              //

              case wa_StackedHoriz:

                {
                  int remain = client.x2() - stackx + 1;
                  if (remain >= ww)
                  {
                    // enough space
                    wdg->doSetPos(stackx,stacky);
                    stackx += (ww+mPaddingX);
                    if (wh>largesth) largesth = wh;
                    int prevy = client.y;
                    int curry = stacky + (largesth+mPaddingY);
                    int diff  = curry-prevy;
                    client.y  = curry;
                    if (diff>0) client.h -= diff;
                  }
                  else
                  {
                    // not enougb space
                    stackx    = client.x;
                    stacky   += (largesth+mPaddingY);
                    largesth  = wh;
                    wdg->doSetPos(stackx,stacky);
                    client.y += (largesth+mPaddingY);
                    client.h -= (largesth+mPaddingY);
                    stackx += (ww+mPaddingX);
                  }
                }
                break;

              //  ________
              // |___|
              // |___|
              // |___|
              // |   |
              // |   :
              //     .

              case wa_StackedVert:

                {
                  int remain = client.y2() - stacky + 1;
                  if (remain >= wh)
                  {
                    // enough space
                    wdg->doSetPos(stackx,stacky);
                    stacky += (wh+mPaddingY);
                    if (ww>largestw) largestw = ww;
                    int prevx = client.x;
                    int currx = stackx + (largestw+mPaddingX);
                    int diff  = currx-prevx;
                    client.x  = currx;
                    if (diff>0) client.w -= diff;
                  }
                  else
                  {
                    // not enougb space
                    stackx   += (largestw+mPaddingX);
                    stacky    = client.y;
                    largestw  = ww;
                    wdg->doSetPos(stackx,stacky);
                    client.x += (largestw+mPaddingX);
                    client.w -= (largestw+mPaddingX);
                    stacky   += (wh+mPaddingY);
                  }
                }
                break;

              //
              //
              //
              //
              //

            } // switch alignment
            mContent.combine( wdg->getRect() ); // keep track of outer boundary
            wdg->doRealign();
          } // for all widgets
          //mContent.add(-mMarginX,-mMarginY,mMarginX*2,mMarginY*2);
        } // if align
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        //if (mFlags&wf_Visible)
        if (isVisible()) // self
        {
          if (mRect.intersects(aRect)) // self
          {
            //if (mFlags&wf_Clip)
            if (isClipping()) // self
            {
              //aCanvas->setClipRect(mRect.x+mMarginX,mRect.y+mMarginY,mRect.x2()-mMarginX,mRect.y2()-mMarginY);
              aCanvas->setClipRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
            }
            for (int i=0; i<mWidgets.size(); i++)
            {
              axWidget* wdg = mWidgets[i];
              if (wdg->isVisible())
              {
                if (/*wdg->intersects(aRect) &&*/ wdg->intersects(mRect))
                  wdg->doPaint(aCanvas,aRect);
              }
            } //for
            //if (mFlags&wf_Clip) aCanvas->clearClipRect(); // resetClipRect();
            if (isClipping()) aCanvas->clearClipRect(); // resetClipRect();
          } //intersect
        } //visible
      }

    //----------

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        //if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
        //else
        if (mCapturedWidget) mCapturedWidget->doMouseDown(aXpos,aYpos,aButton);
        else
        {
          if (isActive())
          {
            axWidget* hover = doFindWidget(aXpos,aYpos);
            if (hover!=this)
            {
              if (mFlags&wf_Capture) mCapturedWidget = hover;
              hover->doMouseDown(aXpos,aYpos,aButton);
            } // !hover
          } // active
        } // !capture
      }

    //----------

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        //if (mModalWidget) mModalWidget->doMouseUp(aXpos,aYpos,aButton);
        //else
        if (mCapturedWidget)
        {
          mCapturedWidget->doMouseUp(aXpos,aYpos,aButton);
          mCapturedWidget = NULL;
        } //capture
        else
        {
          if (isActive())
          {
            axWidget* hover = doFindWidget(aXpos,aYpos);
            if (hover!=this) hover->doMouseUp(aXpos,aYpos,aButton);
          } //active
        }
      }

    //----------

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        //if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
        //else
        if (mCapturedWidget) mCapturedWidget->doMouseMove(aXpos,aYpos,aButton);
        else
        {
          axWidget* hover = doFindWidget(aXpos,aYpos);
          if (hover!=mHoverWidget)
          {
            mHoverWidget->doLeave(mCapturedWidget);
            mHoverWidget = hover;
            mHoverWidget->doEnter(mCapturedWidget);
          }
          if (hover!=this) hover->doMouseMove(aXpos,aYpos,aButton);
        }
      }

    //----------

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        //if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
        //else
        if (mCapturedWidget) mCapturedWidget->doKeyDown(aKeyCode,aState);
      }

    //----------

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        //if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
        //else
        if (mCapturedWidget) mCapturedWidget->doKeyUp(aKeyCode,aState);
      }

    //----------

    virtual void doEnter(axWidget* aWidget)
      {
      }

    //----------

    virtual void doLeave(axWidget* aWidget)
      {
      }

    //--------------------------------------------------
    //
    // axWidgetListener
    //
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        trace("axWidget.onChange");
        trace("mListener: " << mListener);
        mListener->onChange(aWidget);
      }

    //----------

    virtual void onRedraw(axWidget* aWidget)
      {
        mListener->onRedraw(aWidget);
      }

    //----------

    virtual void onCursor(int aCursor/*=DEF_PENWIDTH*/)
      {
        mListener->onCursor(aCursor);
      }

    //----------

    virtual void onHint(axString aHint)
      {
        mListener->onHint(aHint);
      }

    //----------

    // called from wdgSizer
    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode)
      {
        int w = mRect.w + aDeltaX;
        int h = mRect.h + aDeltaY;
        axWidget::doSetSize(w,h);
        mListener->onSize(aWidget,aDeltaX,aDeltaY,aMode);
      }

    //----------

    virtual void onModal(bool aModal, axWidget* aWidget)
      {
        mListener->onModal(aModal,aWidget);
      }

};

//----------------------------------------------------------------------
#endif
