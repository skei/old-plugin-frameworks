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

#ifndef axWindowBase_included
#define axWindowBase_included
//----------------------------------------------------------------------

//#include "base/axContext.h"
#include "core/axRect.h"
#include "core/axString.h"
#include "gui/axWidget.h"
#include "gui/axSurface.h"
#include "gui/axCanvas.h"

#define AX_WIN_EMBEDDED   1
#define AX_WIN_BUFFERED   2
#define AX_WIN_MSGTHREAD  4
#define AX_WIN_MSGPROC    8
#define AX_WIN_MSGDELETE  16

//#define AX_WIN_EXEDEFAULT ( AX_WIN_BUFFERED | AX_WIN_MSGDELETE )
//#define AX_WIN_VSTDEFAULT ( AX_WIN_BUFFERED | AX_WIN_MSGTHREAD | AX_WIN_EMBEDDED)

// mouse buttons and mod keys
#define bu_None    0
#define bu_Left    1
#define bu_Right   2
#define bu_Middle  4
#define bu_Shift   8
#define bu_Ctrl    16
#define bu_Alt     32
//const ini bu_Double  = 1 << 6;
//const int bu_Hover   = 1 << 7;


class axWindowBase : public axWidget//axContainer
{
  protected:
    int         mWinFlags;
    axCanvas*   mCanvas;
    axSurface*  mSurface;

  //protected:
  public:

    axWindowBase(axBase* aBase, void* aParent, axRect aRect, int aWinFlags)
    : axWidget(NULL,/*-1,*/aRect,wa_Client)
      {
        mWinFlags = aWinFlags;
        mCanvas   = NULL;
        mSurface  = NULL;
      }

    virtual ~axWindowBase()
      {
      }

    //--------------------------------------------------

    virtual void doTimer(void)
      {
      }

    //--------------------------------------------------

    // canvas/surface

    virtual axCanvas*   createCanvas(void)  { return NULL; }
    virtual axSurface*  createSurface(int aWidth, int aHeight, int aDepth) { return NULL; }
    virtual axBitmap*   createBitmap(int aWidth, int aHeight, int aDepth) { return NULL; }
    virtual axCanvas*   getCanvas(void)     { return mCanvas; }
    virtual axSurface*  getSurface(void)    { return mSurface; }

    // low level

    virtual void    flush(void) {}
    virtual void    sync(void) {}
    virtual void    lock(void) {}
    virtual void    unlock(void) {}
    virtual void    show(void) {}
    virtual void    hide(void) {}

    virtual void    setPos(int aX, int aY) {}
    virtual void    setSize(int aW, int aH) {}
    virtual void    setParentSize(int aW, int aH) {}
    virtual void    setTitle(axString aTitle) {}
  //virtual void    reparent(int aParent) {}
    virtual void    reparent(void* aParent) {}

    virtual void    setCursor(int aCursor) {}
    virtual void    setCursorPos(int aX, int aY) {}
    virtual void    showCursor(void) {}
    virtual void    hideCursor(void) {}
    virtual void    grabCursor(void) {}
    virtual void    releaseCursor(void) {}

    // medium level

    virtual void    invalidate(int aX, int aY, int aW, int aH) {}
    virtual void    resizeBuffer(int aWidth, int aHeight) {}
    virtual void    beginPaint(void) {}
    virtual void    endPaint(void) {}

    virtual void    startTimer(int ms) {}
    virtual void    stopTimer(void) {}

    // events

    virtual void    sendEvent(unsigned int aValue=0) {}
    virtual void    eventLoop(void) {}

    // axWidgetListener

    virtual void onChange(axWidget* aWidget) { trace(":: axWindowBase.onChange(wdg)"); }
    virtual void onRedraw(axWidget* aWidget) { trace(":: axWindowBase.onRedraw(wdg)"); }

};

//----------------------------------------------------------------------
#endif
