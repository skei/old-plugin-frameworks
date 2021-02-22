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

#ifndef axWindowLinux_included
#define axWindowLinux_included
//----------------------------------------------------------------------

// valgrind memory leak report..
// XGetDefault, XrmGetStringDatabase,
//   _XrmInitParseInfo, _XOpenLC,, _XlcDefaultLoader, _XlcCreateLC, .. realloc
// my guess it's something to do with the fonts...


#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/cursorfont.h>

//#ifdef AX_XRENDER
//#include <X11/extensions/Xrender.h>
//#endif

#include <pthread.h>
#include <unistd.h>   // sleep

//#include "base/axContext.h"
#include "core/axDefines.h"
#include "core/axRect.h"
#include "core/axString.h"
//#include "base/axWindowBase.h"
#include "gui/axWindowBase.h"
#include "gui/axWidget.h"
#include "gui/axCanvas.h"
#include "gui/axSurface.h"

//----------------------------------------------------------------------

// http://tronche.com/gui/x/xlib/appendix/b/
// mouse cursor shapes
#define cu_Arrow           2
#define cu_ArrowUp         114
#define cu_ArrowDown       106
#define cu_ArrowLeft       110
#define cu_ArrowRight      112
#define cu_ArrowUpDown     116
#define cu_ArrowLeftRight  108
#define cu_ArrowDiagLeft   12
#define cu_ArrowDiagRight  14
#define cu_Move            52
#define cu_Wait            150
#define cu_ArrowWait       26
#define cu_Hand            58
#define cu_Finger          60
#define cu_Cross           0
#define cu_Pencil          86
#define cu_Plus            90
#define cu_Question        99
#define cu_Ibeam           152

//#define DEF_CURSOR    cu_Arrow
#define DEF_CURSOR      -1

//----------

// thread signals
#define ts_Kill   666
#define ts_Timer  667

//----------------------------------------------------------------------

void* threadProc(void* data);
void* timerProc(void* data);
void  eventProc(XEvent* ev);

// empty/blank mouse cursor
static char noData[] = { 0,0,0,0,0,0,0,0 };

    char* x11_event_names[] = {
      (char*)"0",
      (char*)"1",
      (char*)"KeyPress",
      (char*)"KeyRelease",
      (char*)"ButtonPress",
      (char*)"ButtonRelease",
      (char*)"MotionNotify",
      (char*)"EnterNotify",
      (char*)"LeaveNotify",
      (char*)"FocusIn",
      (char*)"FocusOut",
      (char*)"KeymapNotify",
      (char*)"Expose",
      (char*)"GraphicsExpose",
      (char*)"NoExpose",
      (char*)"VisibilityNotify",
      (char*)"CreateNotify",
      (char*)"DestroyNotify",
      (char*)"UnmapNotify",
      (char*)"MapNotify",
      (char*)"MapRequest",
      (char*)"ReparentNotify",
      (char*)"ConfigureNotify",
      (char*)"ConfigureRequest",
      (char*)"GravityNotify",
      (char*)"ResizeRequest",
      (char*)"CirculateNotify",
      (char*)"CirculateRequest",
      (char*)"PropertyNotify",
      (char*)"SelectionClear",
      (char*)"SelectionRequest",
      (char*)"SelectionNotify",
      (char*)"ColormapNotify",
      (char*)"ClientMessage",
      (char*)"MappingNotify",
      (char*)"GenericEvent"
    };

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class axWindowLinux : public axWindowBase
{
  friend void* threadProc(void* data);
  friend void* timerProc(void* data);
  friend class axFormatVst;

  private:
    //axFormat*   mFormat;
    //axInterface*   mInterface;
    axBase*     mBase;
    Display*    mDisplay;
    Window      mParent;
    long        mEventMask;
    Atom        mDeleteWindowAtom;
    pthread_t   mEventThread;
    Window      mWindow;
    Atom        mCustomEventAtom;
    Cursor      mInvisibleCursor;
    Pixmap      mBitmapNoData;
    XColor      mBlack;
    //axEventThread mEventThread;
    pthread_t   mTimerThread;
    bool        mTimerRunning;
    int         mTimerSleep;

    //#ifdef AX_XRENDER
    #ifdef AX_ALPHA
    //bool          mXRender;
      Picture       mPicture;
    #endif
    //XRenderColor rendercolor;

  protected:
    // ptr to external axContext (used when creating surfaces)
    //axContext*  mContext;
    int         mWinCursor;

    //Cursor    invisibleCursor;
    //Pixmap    bitmapNoData;
    //int       mWinCursor;
    //XColor    mBlack;


  public:

    //----------------------------------------
    //
    //----------------------------------------

    axWindowLinux(axBase* aBase, void* aParent,axRect aRect, int aWinFlags)
    : axWindowBase(aBase,aParent,aRect,aWinFlags)
      {
        //trace("axWindowLinux.constructor");
        //mFormat = aFormat;
        //mInterface = aInterface;
        mBase     = aBase;
        mDisplay  = (Display*)aBase->getInterface()->getHandle();
        mParent   = *(Window*)aParent;
        //trace("  mBase:    " << mBase);
        //trace("  mDisplay: " << mDisplay);
        //trace("  mParent:  " << mParent);

        mTimerRunning = false;
        mTimerSleep = 30; // 30 ms between each timer signal

        mEventMask  = ExposureMask
                    | ButtonPressMask
                    | ButtonReleaseMask
                    | PointerMotionMask
                    | KeyPressMask
                    | KeyReleaseMask
                    | StructureNotifyMask
                    | PropertyChangeMask
                    | ClientMessage;

        XSetWindowAttributes    swa;
        swa.override_redirect = true;       // http://tronche.com/gui/x/xlib/window/attributes/override-redirect.html
        swa.background_pixmap = None;       // http://tronche.com/gui/x/xlib/window/attributes/background.html
        swa.colormap          = 0;          // http://tronche.com/gui/x/xlib/window/attributes/colormap.html
        swa.event_mask        = mEventMask; // http://tronche.com/gui/x/xlib/window/attributes/event-and-do-not-propagate.html

        // --- create window ---

        mWindow = XCreateWindow(
          mDisplay,
          mParent,
          aRect.x,aRect.y,aRect.w,aRect.h,
          CopyFromParent,
          CopyFromParent,
          InputOutput,
          CopyFromParent,
          CWBackPixmap|CWEventMask,
          &swa
        );

        //trace("window created...");
        //trace("  mWindow:   " << mWindow);
        //trace("  mWinFlags: " << mWinFlags);

        // --- WM_DELETE_WINDOW ClientMessage ---

        if (mWinFlags&AX_WIN_MSGDELETE)
        {
          mDeleteWindowAtom = XInternAtom(mDisplay,"WM_DELETE_WINDOW",True);
          XSetWMProtocols(mDisplay,mWindow,&mDeleteWindowAtom,1);
        }

        // --- remove title-bar, borders ---

        if (mWinFlags&AX_WIN_EMBEDDED)
        {
          #define MWM_HINTS_DECORATIONS (1L << 1)
          #define PROP_MOTIF_WM_HINTS_ELEMENTS  5
          typedef struct
          {
            unsigned long flags;
            unsigned long functions;
            unsigned long decorations;
            long          inputMode;
            unsigned long status;
          } PropMotifWmHints;
          PropMotifWmHints motif_hints;
          motif_hints.flags = MWM_HINTS_DECORATIONS;
          motif_hints.decorations = 0;
          Atom prop = XInternAtom(mDisplay,"_MOTIF_WM_HINTS",True);
          XChangeProperty(mDisplay,mWindow,prop,prop,32,PropModeReplace,(unsigned char *)&motif_hints,PROP_MOTIF_WM_HINTS_ELEMENTS);
          #undef MWM_HINTS_DECORATIONS
          #undef PROP_MOTIF_WM_HINTS_ELEMENTS
        }

        // --- eventProc ---

        if (mWinFlags&AX_WIN_MSGPROC)
        {
          Atom atom;
          void* data;
          // --- eventproc ---
          data = (void*)(size_t)&eventProc;  // the (int) to make the compiler shut up its complaining!
          // ^ lii: loses precision with (int) on 64bit
          // use 'size_t' temp..
          atom = XInternAtom(mDisplay,"_XEventProc",false);
          XChangeProperty(mDisplay,mWindow,atom,atom,32,PropModeReplace,(unsigned char*)&data,1);
          // --- this ---
          data = this;
          atom = XInternAtom(mDisplay, "_this", false);
          XChangeProperty(mDisplay,mWindow,atom,atom,32,PropModeReplace,(unsigned char*)&data,1);
        }

        mCanvas = createCanvas(); // window canvas

        if (mWinFlags&AX_WIN_BUFFERED)
        {
          XWindowAttributes attr;
          XGetWindowAttributes(mDisplay,mWindow,&attr );
          //XGetWindowAttributes(mDisplay,mParent,&attr );

          //trace("x,y: " << attr.x << "," << attr.y); // location of window
          //trace("w,h: " << attr.width << "," << attr.height); // width and height of window
          //trace("border width: " << attr.border_width); /* border width of window */
          //trace("depth: " << attr.depth); /* depth of window */
          //trace("visual: " << attr.visual);    /* theassociated visual structure */
          //trace("root: " << attr.root);    /* root of screen containing window */
          ////trace("class: " << attr.class);    /* InputOutput, InputOnly*/
          //trace("bit_gravity: " << attr.bit_gravity);    /* one of the bit gravity values */
          //trace("win_gravity: " << attr.win_gravity);    /* one of the window gravity values */
          //trace("backing_store: " << attr.backing_store);    /* NotUseful, WhenMapped, Always */
          //trace("backing_planes: " << attr.backing_planes);/* planes to be preserved if possible */
          //trace("backing_pixel: " << attr.backing_pixel);/* value to be used when restoring planes */
          //trace("save_under: " << attr.save_under); /* boolean, should bits under be saved? */
          //trace("colormap: " << attr.colormap); /* color map to be associated with window */
          //trace("map_installed: " << attr.map_installed); /* boolean, is color map currently installed*/
          //trace("map_state: " << attr.map_state); /* IsUnmapped, IsUnviewable, IsViewable */
          //trace("all_event_masks: " << attr.all_event_masks); /* set of events all people have interest in*/
          //trace("your_event_mask: " << attr.your_event_mask); /* my event mask */
          //trace("do_not_propagate_mask: " << attr.do_not_propagate_mask);/* set of events that should not propagate */
          //trace("override_redirect: " << attr.override_redirect); /* boolean value for override-redirect */
          //trace("screen: " << attr.screen); /* back pointer to correct screen */

          int depth = attr.depth;//DefaultDepth(mDisplay,DefaultScreen(mDisplay));

          mSurface = createSurface(mRect.w,mRect.h,depth); // see also: resizeBuffer

          //#ifdef AX_XRENDER
          #ifdef AX_ALPHA
          //if (mXRender)
          //{
            //XRenderPictFormat* format = XRenderFindStandardFormat(mDisplay,PictStandardRGB24);
            //XRenderPictFormat* format = XRenderFindStandardFormat(mDisplay,PictStandardARGB32);
            XRenderPictFormat *format = XRenderFindVisualFormat(mDisplay,attr.visual);
            XRenderPictureAttributes pict_attr;
            mPicture = XRenderCreatePicture(mDisplay,mWindow,format,None,&pict_attr);
            //mPicture = XRenderCreatePicture(mDisplay,mParent,format,None,&pict_attr);
            mCanvas->setPicture( mPicture );
          //}
          bool hasAlpha  = ( format->type == PictTypeDirect && format->direct.alphaMask );
          #endif
        }

        // --- (invisible) mouse cursor ---

        mBitmapNoData = XCreateBitmapFromData(mDisplay,mWindow,noData,8,8);
        mWinCursor    = -1;
        mBlack.red    = 0;
        mBlack.green  = 0;
        mBlack.blue   = 0;
        mBlack.flags  = (DoRed or DoGreen or DoBlue);
        XAllocColor(mDisplay,XDefaultColormap(mDisplay,0),&mBlack);

        // --- event handler thread ---

        if (mWinFlags&AX_WIN_MSGTHREAD)
        {
          pthread_create(&mEventThread,NULL,&threadProc,this);
        }

      }

    //----------------------------------------

    ~axWindowLinux()
      {
        if (mWinFlags&AX_WIN_MSGTHREAD)
        {
          sendEvent(ts_Kill);
          void* ret;
          pthread_join(mEventThread,&ret);
        }
        if (mTimerRunning)
        {
          //wtrace("killing timer");
          mTimerRunning = false;
          void* ret;
          pthread_join(mTimerThread,&ret);
        }

        #ifdef AX_ALPHA
        XRenderFreePicture(mDisplay,mPicture);
        #endif

        if (mCanvas) delete mCanvas;
        if (mSurface) delete mSurface;
        XFreePixmap(mDisplay, mBitmapNoData);
        if (mWinCursor>=0) XFreeCursor(mDisplay,mWinCursor);
        XDestroyWindow(mDisplay,mWindow);
        XFlush(mDisplay);
      }

    //----------------------------------------

    //virtual int getHandle(void) { return (int)mWindow; }
    //Display* getDisplay(void) { return mDisplay; }

    //----------------------------------------

    virtual axCanvas* createCanvas(void)
      {
        //axContext ctx(mDisplay,mWindow);
        axCanvas* canvas = new axCanvas(mBase,&mWindow);
        return canvas;
      }

    virtual axSurface* createSurface(int aWidth, int aHeight, int aDepth)
      {
        //axSurface* surface = new axSurface(mInterface,&mParent,aWidth,aHeight,aDepth);
        axSurface* surface = new axSurface(mBase,&mWindow,aWidth,aHeight,aDepth);
        return surface;
      }

    virtual axBitmap* createBitmap(int aWidth, int aHeight, int aDepth)
      {
        return new axBitmap(mBase,aWidth,aHeight,aDepth);
      }

    //----------------------------------------
    // low level
    //----------------------------------------

    virtual void flush(void)
      {
        XFlush(mDisplay);
      }

    //----------

    virtual void sync(void)
      {
        XSync(mDisplay,false);
      }

    //----------

    virtual void lock(void)
      {
        XLockDisplay(mDisplay);
      }

    //----------

    virtual void unlock(void)
      {
        XUnlockDisplay(mDisplay);
      }

    //----------

    virtual void show(void)
      {
        XMapWindow(mDisplay,mWindow);
        XFlush(mDisplay);
      }

    //----------

    virtual void hide(void)
      {
        XUnmapWindow(mDisplay,mWindow);
        XFlush(mDisplay);
      }

    //----------

    virtual void setPos(int aX, int aY)
      {
        XWindowChanges attr;
        attr.x = aX;
        attr.y = aY;
        XConfigureWindow(mDisplay,mWindow,CWX|CWY,&attr);
        XFlush(mDisplay);
      }

    //----------

    virtual void setSize(int aW, int aH)
      {
        XResizeWindow(mDisplay,mWindow,aW,aH);
        XFlush(mDisplay);
      }

    //----------

    ////TODO
    //virtual void setParentSize(int aWidth, int aHeight)
    //  {
    //    //if (mParent!=0)
    //    //  XResizeWindow(gDP, mParent, aWidth, aHeight);
    //    setSize(aWidth,aHeight);
    //  }

    //----------

    // valgrind reports memory leak here ('definitely lost')
    // XStringListToTextProperty, malloc

    // lii:
    // there could be malloc() in XStringListToTextProperty(),
    // so 'free(window_title);' might be needed after XFlush(..); ?

    virtual void setTitle(axString aTitle)
      {
        //XTextProperty window_title_property;
        //char* window_title = aTitle.ptr();
        //XStringListToTextProperty(&window_title,1,&window_title_property);
        //XSetWMName(mDisplay,mWindow,&window_title_property);
        //XFlush(mDisplay);
      }

    //----------

    //virtual void reparent(int aParent)
    virtual void reparent(void* aParent)
      {
        mParent = *(Window*)aParent;
        XReparentWindow(mDisplay,mWindow,mParent,0,0);
        XFlush(mDisplay);
      }

    //----------

    virtual void resetCursor(void)
      {
        XUndefineCursor(mDisplay,mWindow);
        XFreeCursor(mDisplay,mWinCursor);
        mWinCursor=-1;
      }

    //----------

    virtual void setCursor(int aCursor)
      {
        //if( aCursor<0 ) aCursor = DEF_CURSOR;
        if (mWinCursor>=0) resetCursor();
        //mWinCursor = XCreatePixmapCursor( gDP,bitmapNoData,bitmapNoData,&mBlack,&mBlack,0,0 );
        if (aCursor>=0)
        {
          mWinCursor = XCreateFontCursor(mDisplay, aCursor);
          XDefineCursor(mDisplay, mWindow, mWinCursor);
        }
      }

    //----------

    //virtual void setCursorPos(int aX, int aY)
    //  {
    //  }

    //----------

    virtual void showCursor(void)
      {
        setCursor(DEF_CURSOR);
      }

    //----------

    virtual void hideCursor(void)
      {
        if (mWinCursor>=0) resetCursor();
        mWinCursor = XCreatePixmapCursor( mDisplay,mBitmapNoData,mBitmapNoData,&mBlack,&mBlack,0,0 );
        XDefineCursor(mDisplay,mWindow,mWinCursor);
      }

    //----------

    virtual void grabCursor(void)
      {
        int which = ButtonPressMask|ButtonReleaseMask|PointerMotionMask;
        XGrabPointer(mDisplay,mWindow,false,which,GrabModeSync,GrabModeAsync,mWindow,/*cursor*/None,CurrentTime);
      }

    //----------

    virtual void releaseCursor(void)
      {
        XUngrabPointer(mDisplay, CurrentTime);
      }

    //----------------------------------------
    // medium level
    //----------------------------------------

    virtual void invalidate(int aX, int aY, int aW, int aH)
      {
        static XExposeEvent ev;
        ev.type     = Expose;
        ev.display  = mDisplay;
        ev.window   = mWindow; // mParent;
        ev.x        = aX;
        ev.y        = aY;
        ev.width    = aW;
        ev.height   = aH;
        ev.count    = 0;
        XSendEvent(mDisplay,mWindow,false,ExposureMask,(XEvent*)&ev);
        XFlush(mDisplay);
      }

    //----------

    virtual void resizeBuffer(int aWidth, int aHeight)
      {
        //wtrace("axWindowLinux.resizeBuffer");
        if (mWinFlags&AX_WIN_BUFFERED)
        {
          //if (aWidth!=mSurface->getWidth() || aHeight!=mSurface->getHeight())
          //{
            //mSurfaceMutex.lock();
            axSurface* srf;
            if (mSurface)
            {
              //TODO: copy brushes & pens?
              srf = mSurface;
              mSurface = NULL;
              delete srf;
            }
            int depth = DefaultDepth(mDisplay,DefaultScreen(mDisplay));
            srf = createSurface(aWidth,aHeight,depth);

            srf->getCanvas()->setPicture( srf->getPicture() );

            //mCanvas->setPicture( mSurface->getPicture() );
            mSurface = srf;
            //mSurfaceMutex.unlock();
          //} //size
        } //buffered
        //mRect.w = aWidth;
        //mRect.h = aHeight;
        //doSetSize(aWidth,aHeight);
      }

    //----------

    virtual void beginPaint(void)
      {
      }

    //----------

    virtual void endPaint(void)
      {
        XFlush(mDisplay);
      }



    //----------------------------------------
    //
    //----------------------------------------

    virtual void startTimer(int ms)
      {
        mTimerSleep = ms;
        mTimerRunning = true;
        /*int ret = */pthread_create( &mTimerThread, NULL, &timerProc, this );
      }

    //----------

    virtual void stopTimer(void)
      {
        void* ret;
        mTimerRunning = false;
        /*int ret = */pthread_join(mTimerThread, &ret);
      }

    //----------------------------------------
    // events
    //----------------------------------------

    virtual void sendEvent(unsigned int aValue=0)
    {
      static XClientMessageEvent event;
      //memset(&event,0,sizeof(event));
      event.type          = ClientMessage;
      event.message_type  = mCustomEventAtom;
      event.display       = mDisplay;
      event.window        = mWindow;
      event.format        = 32;
      event.data.l[0]     = aValue;
      XSendEvent(mDisplay,mWindow,False,NoEventMask,(XEvent*)&event);
      XFlush(mDisplay);
    }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void eventLoop(void)
      {
        XEvent ev;
        while (1)
        {
          XNextEvent(mDisplay,&ev);
          unsigned int data = ev.xclient.data.l[0];
          if (ev.type==ClientMessage && data==mDeleteWindowAtom)
          {
            //tracetrace("exit signal");
            break;
          }
          else eventHandler(&ev);
        }
      }

    //----------------------------------------
    //
    //----------------------------------------

    //#define bu_None    0
    //#define bu_Left    1
    //#define bu_Right   2
    //#define bu_Middle  4
    //#define bu_Shift   8
    //#define bu_Ctrl    16
    //#define bu_Alt     32

    int remapButton(int aButton)
      {
        int ret = bu_None;
        switch (aButton)
        {
          case 1: ret = bu_Left;   break;
          case 2: ret = bu_Middle; break;
          case 3: ret = bu_Right;  break;
        }
        return ret;
      }

    //----------

    int remapKey(int aKey)
      {
        int ret = 0;
        if (aKey & 1) ret |= bu_Shift;
        // 2 CapsLock
        if (aKey & 4) ret |= bu_Ctrl;
        if (aKey & 8) ret |= bu_Alt;
        // 128 Alt Gr
        return ret;
      }

    //----------------------------------------
    // event handler
    //----------------------------------------

    void eventHandler(XEvent* ev)
      {
        //trace("axWindowX11.eventHandler: " << ev->type << " : " << x11_event_names[ev->type]);
        axRect rc;
        int but,key,val;
        int w,h;
        //int x,y;
        switch (ev->type)
        {
          case ConfigureNotify:
            w = ev->xconfigure.width;
            h = ev->xconfigure.height;
            while (XCheckTypedWindowEvent(mDisplay, ev->xconfigure.window, ConfigureNotify, ev))
            {
              w = ev->xconfigure.width;
              h = ev->xconfigure.height;
            }
            //wtrace(":: ConfigureNotify " << w << "," << h);
            resizeBuffer(w,h);
            doSetSize(w,h);
            break;
          case Expose:
            beginPaint();
            rc = axRect(ev->xexpose.x,
                        ev->xexpose.y,
                        ev->xexpose.width,
                        ev->xexpose.height);
            while (XCheckTypedWindowEvent(mDisplay, ev->xexpose.window, Expose, ev))
            {
              rc.combine( ev->xexpose.x, ev->xexpose.y, ev->xexpose.width, ev->xexpose.height );
            }
            //trace("Expose " << rc.x << "," << rc.y << "," << rc.w << "," << rc.h);
            //mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            if ((mWinFlags&AX_WIN_BUFFERED) && mSurface )
            {
              //trace("- double buffered");
              axCanvas* can = mSurface->getCanvas();
              //trace("  can: " << can);
              can->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(can,rc);

              //TODO:
              //investigate this. should the window buffer be blitter w/transparency?
              //make it similar to how it works in windows...

              /*
              //#ifdef AX_XRENDER
              #ifdef AX_ALPHA
              mCanvas->renderImage(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              #else
              */

              //mCanvas->drawImage(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              mCanvas->drawSurface(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);

              /*
              #endif
              */

              //mCanvas->renderPicture(mPicture,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);

              can->clearClipRect();
            }
            else
            {
              trace("- not double buffered");
              mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(mCanvas,rc);
              mCanvas->clearClipRect();
              //XFlush(mDisplay);
            }
            //mCanvas->clearClipRect();
            endPaint();
            break;
          case ClientMessage:
            val = ev->xclient.data.l[0];
            //wtrace("ClientMessage " << val);
            if (val==ts_Timer) { /*wtrace("timer");*/ doTimer(); }
            break;
          case ButtonPress:
            but = remapButton(ev->xbutton.button);
            key = remapKey(ev->xbutton.state);
            //trace("axWindowX11.eventHandler :: ButtonPress " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseDown(ev->xbutton.x, ev->xbutton.y, but|key );
            break;
          case ButtonRelease:
            but = remapButton(ev->xbutton.button);
            key = remapKey(ev->xbutton.state);
            //trace("axWindowX11.eventHandler :: ButtonRelease " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseUp(ev->xbutton.x, ev->xbutton.y, but|key);
            break;
          case MotionNotify:
            but = 0;//remapButton(ev->xbutton.button);
            key = remapKey(ev->xbutton.state);
            //trace("axWindowX11.eventHandler :: MotionNotify " << ev->xbutton.x << "," << ev->xbutton.y << ", " << (but|key));
            doMouseMove(ev->xbutton.x, ev->xbutton.y, but|key);
            break;
          case KeyPress:
            //trace("axWindowX11.eventHandler :: KeyPress " << ev->xkey.keycode << "," << ev->xkey.state);
            doKeyDown(ev->xkey.keycode, ev->xkey.state);
            break;
          case KeyRelease:
            //trace("axWindowX11.eventHandler :: KeyRelease " << ev->xkey.keycode << "," << ev->xkey.state);
            doKeyUp(ev->xkey.keycode, ev->xkey.state);
            break;
//          default:
//            trace("axWindowX11.eventHandler :: unhandled event :: " << ev->type << " " << x11_event_names[ev->type]);
//            break;
        }

      }

};

//----------------------------------------------------------------------
//
// thread proc
//
//----------------------------------------------------------------------

void* threadProc(void* data)
  {
    axWindowLinux* win = (axWindowLinux*)data;
    if (win)
    {
      XEvent ev;
      while (1)
      {
        XNextEvent(win->mDisplay,&ev);
        if (ev.type==ClientMessage)
        {
          //wtrace("client message");
          XClientMessageEvent *cev = (XClientMessageEvent *)&ev;
          unsigned int data = ev.xclient.data.l[0];
          if (cev->message_type==win->mCustomEventAtom)
          {
            if (data==ts_Kill) { /*wtrace("ts_Kill");*/ pthread_exit(NULL); }
            else win->eventHandler(&ev);
          }
        } //ClientMessage
        else win->eventHandler(&ev);
      }
    }
    return NULL;
  }

//----------------------------------------------------------------------
//
// timer proc
//
//----------------------------------------------------------------------

void* timerProc(void* data)
  {
    axWindowLinux* win = (axWindowLinux*)data;
    if (win)
    {
      while (win->mTimerRunning)
      {
        win->sendEvent(ts_Timer);
        usleep(win->mTimerSleep*1000); //ms*1000;
      }
    }
    return NULL;
  }

//----------------------------------------------------------------------
//
// event proc
//
//----------------------------------------------------------------------

//[internal]
bool xerror;
int errorHandler(Display *dp, XErrorEvent *e)
{
  xerror = true;
  return 0;
}
//----------
//[internal]
int getProperty(Window win, Display* disp, Atom atom)
{
  int result = 0, userSize;
  unsigned long bytes, userCount;
  unsigned char *data;
  Atom userType;
  xerror = false;
  XErrorHandler olderrorhandler = XSetErrorHandler(errorHandler);
  XGetWindowProperty(
    disp,             // display
    win,              // window
    atom,             // property
    0, 1,             // offset, length
    false,            // delete?
    AnyPropertyType,  // req type
    &userType,        // actual type return
    &userSize,        // actual format return
    &userCount,       // n items return
    &bytes,           // bytes after return
    &data             // prop return
  );
  if (xerror==false && userCount==1) result = *(int*)data;
  XSetErrorHandler(olderrorhandler);
  return result;
}

//--------------------------------------------------

//typedef struct
//{
//	int type;
//	unsigned long serial; /* # of last request processed by server */
//	Bool send_event;      /* true if this came from a SendEvent request */
//	Display *display;     /* Display the event was read from */
//	Window window;
//} XAnyEvent;

void eventProc(XEvent* ev)
{
  //trace("eventProc");
  Window xw   = ev->xany.window;
  Display* xd = ev->xany.display;
  axWindowLinux* win = (axWindowLinux*)getProperty(xw,xd,XInternAtom(xd,"_this",false));
  if (win==0) return;
  win->eventHandler(ev);
}

//----------------------------------------------------------------------

typedef axWindowLinux axWindowImpl;

//----------------------------------------------------------------------
#endif
