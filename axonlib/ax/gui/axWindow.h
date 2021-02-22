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

#ifndef axWindow_included
#define axWindow_included
//----------------------------------------------------------------------

#include "core/axDefines.h"
#include "base/axInterface.h"
#include "gui/axWindowBase.h"
//#include "gui/axSkin.h"
//#include "gui/axSkinDefault.h"
#include "par/axParameter.h"

//----------

#ifdef AX_LINUX
  #include "gui/linux/axWindowLinux.h"
#endif

#ifdef AX_WIN32
  #include "gui/win32/axWindowWin32.h"
#endif

//----------------------------------------------------------------------

class axWindow : public axWindowImpl

{
  protected:
    axWidget* mModalWidget;
    int       mModalIndex;
    #ifndef AX_WIDGET_NOUPDATELIST
    axWidgets mUpdateList;
    #endif

  public:

    axWindow(axBase* aBase, void* aParent,axRect aRect, int aWinFlags)
    : axWindowImpl(aBase,aParent,aRect,aWinFlags)
      {
        mModalWidget = NULL;
        mModalIndex  = -1;
      }

    virtual ~axWindow()
      {
      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void redrawAll(void)                        { invalidate( mRect.x, mRect.y, mRect.w, mRect.h ); }
    virtual void redrawRect(axRect aRect)               { invalidate( aRect.x, aRect.y, aRect.w, aRect.h ); }
    virtual void redrawWidget(axWidget* aWidget)        { redrawRect(aWidget->getRect()); }
    virtual void paramChanged(axParameter* aParameter)  { }

    //----------------------------------------

    virtual void goModal(axWidget* aWidget)
      {
        mModalWidget = aWidget;
        mModalIndex = appendWidget(mModalWidget);
        redrawAll();
      }

    //----------

    void unModal(void)
      {
        removeWidget(mModalIndex);
        delete mModalWidget;
        mModalWidget = NULL;
        redrawAll();
      }

    //----------------------------------------

    /*
      axArray.append looks like can be dangerous,
      because it increases the size before the item itself is added to the array..
      if a thread is reading the array size just after it has been increased,
      but before the item has been properly set up and written to the array buffer,
      things could go wrong...
    */

    #ifndef AX_WIDGET_NOUPDATELIST

    void clearUpdates(void)
      {
        //mutex_dirty.lock();
        mUpdateList.clear(false);
        //mutex_dirty.unlock();
      }

    //----------

    void appendUpdate(axWidget* aWidget)
      {
        for( int i=0; i<mUpdateList.size(); i++ ) if( mUpdateList[i]==aWidget ) return;
        //mutex_dirty.lock();
        mUpdateList.append(aWidget);
        //mutex_dirty.unlock();
      }

    //----------

    // if we're inside this redrawDirty (because of idleEditor),
    // we can't append new widgets to it!!
    // dangerous if we don't manage the redrawDirty ourselves...
    // we might need a redrawLock here
    // or we need to be very certain about which thread is adding
    // widgets to the array, and which is reading from the list...
    // appendUpdate vs redrawUpdates

    void redrawUpdates(void)
      {
        //mutex_dirty.lock();
        int num = mUpdateList.size();
        //trace("redrawUpdates: " << num);
        for( int i=0; i<num; i++ )
        {
          axWidget* wdg = mUpdateList[i];
          redrawWidget(wdg);
        }
        clearUpdates();
        //mutex_dirty.unlock();
        //flush();

      }

    #endif // AX_WIDGET_NOUPDATELIST

    //----------------------------------------
    // do..
    //----------------------------------------

    //virtual void doSetPos(int aXpos, int aYpos) {}

    virtual void doSetSize(int aWidth, int aHeight)
      {
        mRect.setSize(aWidth,aHeight);
        if (mFlags&wf_Align) doRealign();
      }

    //virtual void doMove(int aDeltaX, int aDeltaY) {}
    //virtual void doResize(int aDeltaX, int aDeltaY) {}
    //virtual void doRealign(void) {}
    //virtual void doPaint(axCanvas* aCanvas, axRect aRect) {}

    // if we're in a modal state, send these events only to
    // the modal widget (popup, etc)

    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseDown(aXpos,aYpos,aButton);
        else axWidget::doMouseDown(aXpos,aYpos,aButton);
      }

    virtual void doMouseUp(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseUp(aXpos,aYpos,aButton);
        else axWidget::doMouseUp(aXpos,aYpos,aButton);
      }

    virtual void doMouseMove(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget) mModalWidget->doMouseMove(aXpos,aYpos,aButton);
        else axWidget::doMouseMove(aXpos,aYpos,aButton);
      }

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyDown(aKeyCode,aState);
        else axWidget::doKeyDown(aKeyCode,aState);
      }

    virtual void doKeyUp(int aKeyCode, int aState)
      {
        if (mModalWidget) mModalWidget->doKeyUp(aKeyCode,aState);
        else axWidget::doKeyUp(aKeyCode,aState);
      }

    //virtual void doEnter(axWidget* aCapture) {}
    //virtual void doLeave(axWidget* aCapture) {}

    //----------------------------------------
    // on..
    //----------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        redrawWidget(aWidget);
      }

    virtual void onRedraw(axWidget* aWidget)
      {
        redrawWidget(aWidget);
      }

    virtual void onCursor(int aCursor)
      {
        setCursor(aCursor);
      }

    virtual void onHint(axString aHint)
      {
      }

    virtual void onSize(axWidget* aWidget, int aDeltaX, int aDeltaY, int aMode)
      {
        if (mFlags&wf_Align) doRealign();
        //if (mFlags&wf_Visible)
          redrawAll();
      }

    virtual void onModal(bool aModal, axWidget* aWidget)
      {
        if (aModal) goModal(aWidget);
        else unModal();
      }

};

//----------------------------------------------------------------------
#endif
