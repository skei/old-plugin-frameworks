#ifndef s3_window_xlib_included
#define s3_window_xlib_included
//----------------------------------------------------------------------

#include "common/s3_rect.h"
//#include "common/s3_xlib.h"
#include "gui/s3_drawable.h"
#include "gui/s3_widget.h"
#include "gui/s3_window_base.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Window_Xlib
: public S3_Window_Base
, public S3_Drawable {

  friend void* s3_xlib_threadproc(void* AData);
  friend void* s3_xlib_timerproc(void* AData);

  private: // create
    Display*            MDisplay;
    Window              MRootWindow;
    Window              MParentWindow;
    int32               MScreenNum;
    Visual*             MVisual;
    Colormap            MColormap;
    int32               MScreenDepth;
  private: // events
    Atom                MDeleteAtom;
    Atom                MCustomAtom;
    XExposeEvent        MExposeEvent;   // invalidate
    XClientMessageEvent MSendEvent;     // sendEvent
  private: // cursor
    Pixmap              MEmptyPixmap;
    XColor              MBlack;
    int32               MWindowCursor;
    int32               MCurrentCursor;
    int32               MMouseXpos;
    int32               MMouseYpos;
  private: // window
    Window              MWindow;
    bool                MWindowExposed;
    bool                MWindowMapped;
    bool                MWindowNeedSize;
  private: // timer thread
    pthread_t           MTimerThread;
    bool                MTimerThreadActive;
    int32               MTimerSleep;
  private: // event thread
    #ifdef S3_LIB
    pthread_t           MEventThread;
    bool                MEventThreadActive;
    #endif
    #ifdef S3_XRENDER
    Picture             MPicture;
    #endif
  public:
    S3_Window_Xlib(S3_Rect ARect, void* AParent=S3_NULL);
    virtual ~S3_Window_Xlib();
  public: // drawable
    bool      isWindow(void)  override  { return true; }
    int32     width(void)     override  { return MRect.w; }
    int32     height(void)    override  { return MRect.h; }
    int32     depth(void)     override  { return MScreenDepth; }
    Display*  display(void)   override  { return MDisplay; }
    Visual*   visual(void)    override  { return MVisual; }
    Drawable  drawable(void)  override  { return MWindow; }
    #ifdef S3_XRENDER
    Picture   picture(void)   override  { return MPicture; }
    #endif

  //public:
  //  Window    window(void) { return MWindow; }
  private:
    void  removeDecoration(void);
    void  wantQuitEvent(void);
    void  waitForMapNotify(void);
    void  startEventThread(void);
    void  stopEventThread(void);
    void  resetCursor(void);
    int32 remapButton(int32 AButton);
    int32 remapKey(int32 AState);
    void  eventHandler(XEvent* AEvent);
  //public:
  //  virtual void on_window_mapped(void) {}
  public:
    void  setPos(int32 AXpos, int32 AYpos) override;
    void  setSize(int32 AWidth, int32 AHeight) override;
    void  setTitle(const char* ATitle) override;
    void  open(void) override;
    void  close(void) override;
    void  startTimer(int32 ms) override;
    void  stopTimer(void) override;
    void  flush(void) override;
    void  sync(void) override;
    void  lock(void) override;
    void  unlock(void) override;
    void  reparent(void* AParent) override;
    void  sendEvent(uint32 AValue) override;
    void  eventLoop(void) override;
    void  renderLoop(void) override;
    void  invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) override;
    void  beginPaint(void) override;
    void  endPaint(void) override;
    void  setCursor(int32 ACursor) override;
    void  createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) override;
    void  destroyUserCursors(void) override;
    void  setCursorPos(int32 AXpos, int32 AYpos) override;
    void  showCursor(void) override;
    void  hideCursor(void) override;
    void  grabCursor(void) override;
    void  releaseCursor(void) override;
    void  warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) override;
    void  unwarpCursor(void) override;
    //void  blit(int32 dst_x, int32 dst_y, S3_Surface* ASurface, int32 src_x, int32 src_y, int32 src_w, int32 src_h) override;
    //----------
    void  invalidate(S3_Rect ARect) { invalidate(ARect.x,ARect.y,ARect.w,ARect.h); }
};

//----------

void* s3_xlib_threadproc(void* AData);
void* s3_xlib_timerproc(void* AData);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

char s3_xlib_blank_cursor[] = { 0,0,0,0,0,0,0,0 };

uint32 s3_xlib_cursors[20] = {
  XC_arrow,             // smc_default
  XC_arrow,             // smc_Arrow
  XC_center_ptr,        // smc_ArrowUp
  XC_sb_down_arrow,     // smc_ArrowDown
  XC_sb_left_arrow,     // smc_ArrowLeft
  XC_sb_right_arrow,    // smc_ArrowRight
  XC_sb_v_double_arrow, // smc_ArrowUpDown
  XC_sb_h_double_arrow, // smc_ArrowLeftRight
  XC_top_left_corner,   // smc_ArrowDiagLeft
  XC_top_right_corner,  // smc_ArrowDiagRight
  XC_fleur,             // smc_Move
  XC_watch,             // smc_Wait
  XC_clock,             // smc_ArrowWait
  XC_hand1,             // smc_Hand
  XC_hand2,             // smc_Finger
  XC_crosshair,         // smc_Cross
  XC_pencil,            // smc_Pencil
  XC_plus,              // smc_Plus
  XC_question_arrow,    // smc_Question
  XC_xterm              // smc_Ibeam
};

//----------------------------------------------------------------------

S3_Window_Xlib::S3_Window_Xlib(S3_Rect ARect, void* AParent)
: S3_Window_Base(ARect) {

  // xlib

  MWindowExposed      = false;
  MWindowMapped       = false;
  MWindowNeedSize     = true;
  MDisplay            = XOpenDisplay(S3_NULL);
  MRootWindow         = DefaultRootWindow(MDisplay);
  #ifdef S3_LIB
    MParentWindow     = (Window)AParent;
  #else
    MParentWindow     = MRootWindow;
  #endif
  MScreenNum          = XDefaultScreen(MDisplay);
  MVisual             = XDefaultVisual(MDisplay, MScreenNum);
  MColormap           = XCreateColormap(MDisplay, MParentWindow, MVisual, AllocNone); // -> XFreeColors
  MScreenDepth        = XDefaultDepth(MDisplay, MScreenNum);

  uint32 eventmask    =
    ExposureMask        |
    StructureNotifyMask |
    PropertyChangeMask  |
    ButtonPressMask     |
    ButtonReleaseMask   |
    PointerMotionMask   |
    KeyPressMask        |
    KeyReleaseMask      |
    ClientMessage;
  XSetWindowAttributes swa;
  swa.event_mask        = eventmask;
  swa.background_pixmap = None;
  swa.border_pixel      = 0;
  swa.colormap          = MColormap;
  uint32 swamask        = CWEventMask
                        | CWBackPixmap
                        | CWBorderPixel
                        | CWColormap;

  // create window

  MWindow = XCreateWindow(
    MDisplay,
    MParentWindow,
    MRect.x,
    MRect.y,
    MRect.w,
    MRect.h,
    0,
    MScreenDepth,
    InputOutput,
    MVisual,
    swamask,
    &swa
  );

  //

  #ifdef S3_LIB
    removeDecoration();
  #else
    wantQuitEvent();
  #endif

  // mouse cursor

  MEmptyPixmap  = XCreateBitmapFromData(MDisplay, MWindow, s3_xlib_blank_cursor, 8, 8);
  MBlack.red    = 0;
  MBlack.green  = 0;
  MBlack.blue   = 0;
  MBlack.flags  = (DoRed | DoGreen | DoBlue);
  XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&MBlack);
  MMouseXpos          = -1;
  MMouseYpos          = -1;
  MWindowCursor       = -1;

  // timer thread

  MTimerThread        = 0;
  MTimerThreadActive  = false;
  MTimerSleep         = 30;

  // event thread

  #ifdef S3_LIB
    MEventThread        = 0;
    MEventThreadActive  = false;
  #endif

  // xrender

  #ifdef S3_XRENDER
    MPicture = s3_xrender_create_picture(MDisplay,MWindow,MScreenDepth);
  #endif

}

//----------

S3_Window_Xlib::~S3_Window_Xlib(void) {
  if (MTimerThreadActive) stopTimer();
  #ifdef S3_LIB
    if (MEventThreadActive) stopEventThread();
  #endif
  #ifdef S3_XRENDER
    s3_xrender_delete_picture(MDisplay,MPicture);
  #endif
  XFreePixmap(MDisplay,MEmptyPixmap);
  if (MWindowCursor >= 0) XFreeCursor(MDisplay,MWindowCursor);
  XDestroyWindow(MDisplay,MWindow);
  XCloseDisplay(MDisplay);

}

//----------------------------------------
//
//----------------------------------------

struct s3_xlib_motif_hints {
  uint32  flags;
  uint32  functions;
  uint32  decorations;
  int32   inputMode;
  uint32  status;
};

//----------

#define MWM_HINTS_DECORATIONS (1 << 1)
#define PROP_MOTIF_WM_HINTS_ELEMENTS 5

void S3_Window_Xlib::removeDecoration(void) {
  s3_xlib_motif_hints hints;
  hints.flags = MWM_HINTS_DECORATIONS;
  hints.decorations = 0;
  Atom prop = XInternAtom(MDisplay, "_MOTIF_WM_HINTS", true);
  if (prop != None) {
    XChangeProperty(MDisplay, MWindow, prop, prop, 32, PropModeReplace, (unsigned char*)&hints, PROP_MOTIF_WM_HINTS_ELEMENTS);
  }
}

#undef MWM_HINTS_DECORATIONS
#undef PROP_MOTIF_WM_HINTS_ELEMENTS

//----------

// want WM_DELETE_WINDOW ClientMessage

void S3_Window_Xlib::wantQuitEvent(void) {
  MDeleteAtom = XInternAtom(MDisplay, "WM_DELETE_WINDOW", true);
  if (MDeleteAtom != None) {
    XSetWMProtocols(MDisplay, MWindow, &MDeleteAtom, 1);
  }
}

//----------

void S3_Window_Xlib::waitForMapNotify(void) {
  XFlush(MDisplay);
  while (1) {
    XEvent e;
    XNextEvent(MDisplay,&e);
    if (e.type == MapNotify) break;
  }
}

//----------

#ifdef S3_LIB
void S3_Window_Xlib::startEventThread(void) {
  MEventThreadActive = true;
  pthread_create(&MEventThread,S3_NULL,s3_xlib_threadproc,this);
}
#endif

//----------

#ifdef S3_LIB
void S3_Window_Xlib::stopEventThread(void) {
  void* ret;
  MEventThreadActive = false;
  sendEvent(s3_ts_kill);
  pthread_join(MEventThread,&ret);
}
#endif

//----------

void S3_Window_Xlib::resetCursor(void) {
  XUndefineCursor(MDisplay,MWindow);
  XFreeCursor(MDisplay,MWindowCursor);
  MWindowCursor = -1;
}

//----------

int32 S3_Window_Xlib::remapButton(int32 AButton) {
  int32 result = s3_mb_none;
  switch(AButton) {
    case 1: result = s3_mb_left; break;
    case 2: result = s3_mb_middle; break;
    case 3: result = s3_mb_right; break;
  }
  return result;
}

//----------

int32 S3_Window_Xlib::remapKey(int32 AState) {
  int32 result = s3_mb_none;
  if ((AState & 1  ) != 0) result |= s3_mk_shift;
  //if ((AState & 2  ) != 0) result |= s3_mk_caps;  // todo
  if ((AState & 4  ) != 0) result |= s3_mk_ctrl;
  if ((AState & 8  ) != 0) result |= s3_mk_alt;
  //if ((AState & 128) != 0) result |= s3_mk_altgr; // todo
  return result;
}

//----------------------------------------
// event handler
//----------------------------------------

void S3_Window_Xlib::eventHandler(XEvent* AEvent) {

  switch(AEvent->type) {

    case ButtonPress: {
      int32 x = AEvent->xbutton.x;
      int32 y = AEvent->xbutton.y;
      int32 b = remapButton(AEvent->xbutton.button);
      int32 s = remapKey(AEvent->xbutton.state);
      on_widget_mouseDown(x,y,b,s);
      if (MFlags&s3_wf_capture) grabCursor();
    }
    break;

    case ButtonRelease: {
      int32 x = AEvent->xbutton.x;
      int32 y = AEvent->xbutton.y;
      int32 b = remapButton(AEvent->xbutton.button);
      int32 s = remapKey(AEvent->xbutton.state);
      on_widget_mouseUp(x,y,b,s);
      if (MFlags&s3_wf_capture) releaseCursor();
    }
    break;

    case ClientMessage: {
      int32 i = AEvent->xclient.data.l[0];
      switch(i) {
        case s3_ts_timer:
          on_window_timer();
          break;
        case s3_ts_idle:
          if (MWindowMapped && MWindowExposed) {
            on_window_idle();
          }
          break;
      }
    }
    break;

    case ConfigureNotify: {
      //S3_Trace("ConfigureNotify\n");
      int32 w = AEvent->xconfigure.width;
      int32 h = AEvent->xconfigure.height;
      while (XCheckTypedWindowEvent(MDisplay, AEvent->xconfigure.window, ConfigureNotify, AEvent)) {
        w = AEvent->xconfigure.width;
        h = AEvent->xconfigure.height;
      }
      //S3_Trace("ConfigureNotify %i %i MRect %i %i %i %i\n",w,h,MRect.x,MRect.y,MRect.w,MRect.h);
      if ((w != MRect.w) || (h != MRect.h)) {
        #ifndef S3_NO_WINDOW_BACKBUFFER
          on_buffer_resize(w,h);
        #else
          on_window_resize(w,h);
        #endif
        on_widget_setSize(w,h);
        if (MFlags & s3_wf_autoalign) on_widget_align();
        #ifndef S3_NO_WINDOW_BACKBUFFER
          on_buffer_paint(MRect,s3_pm_normal);
        #endif
      }

    }
    break;

    case Expose: {
      //S3_Trace("Expose\n");
      MWindowExposed = true;
      S3_Rect rec;
      rec.x = AEvent->xexpose.x;
      rec.y = AEvent->xexpose.y;
      rec.w = AEvent->xexpose.width;
      rec.h = AEvent->xexpose.height;
      while (XCheckTypedWindowEvent(MDisplay,AEvent->xexpose.window,Expose,AEvent)) {
        rec.combine(AEvent->xexpose.x,AEvent->xexpose.y,AEvent->xexpose.width,AEvent->xexpose.height);
      }
      //S3_Trace("Expose %i %i %i %i\n",rec.x,rec.y,rec.w,rec.h);
      beginPaint();
      #ifndef S3_NO_WINDOW_BACKBUFFER
        on_buffer_blit(rec);
      #else
        on_window_paint(rec,s3_pm_normal);
      #endif
      endPaint();
    }
    break;

    case KeyPress: {
      int32 s = AEvent->xkey.state;
      char c[2];
      KeySym keysym; // = XID = unsigned long
      XLookupString(&AEvent->xkey, c, 1, &keysym, S3_NULL);
      uint32 ks = remapKey(keysym);
      on_widget_keyDown(ks,s);
    }
    break;

    case KeyRelease: {
      int32 s = AEvent->xkey.state;
      char c[2];
      KeySym keysym; // = XID = unsigned long
      XLookupString(&AEvent->xkey, c, 1, &keysym, S3_NULL);
      uint32 ks = remapKey(keysym);
      on_widget_keyUp(ks, s);
    }
    break;

    case MapNotify: {
      MWindowMapped = true;
      //on_window_mapped();
    }
    break;

    case MotionNotify: {
      int32 x = AEvent->xbutton.x;
      int32 y = AEvent->xbutton.y;
      int32 s = remapKey(AEvent->xbutton.state);
      on_widget_mouseMove(x,y,s);
      MMouseXpos = x;
      MMouseYpos = y;
    }
    break;

    //case PropertyNotify: {
    //  atomname := XGetAtomName(MDisplay,AEvent->xproperty.atom);
    //}

    case UnmapNotify: {
      MWindowMapped = false;
    }

    //else S3_Trace("unhandled event: %i\n",AEvent->type);

  } // case

}

//------------------------------
// set/get
//------------------------------

// window must be open

void S3_Window_Xlib::setPos(int32 AXpos, int32 AYpos) {
  XWindowChanges attr;
  attr.x = AXpos;
  attr.y = AYpos;
  XConfigureWindow(MDisplay,MWindow,CWX | CWY,&attr);
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::setSize(int32 AWidth, int32 AHeight) {
  MRect.w = AWidth;
  MRect.h = AHeight;
  XResizeWindow(MDisplay,MWindow,MRect.w,MRect.h);
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::setTitle(const char* ATitle) {
  XSetStandardProperties(MDisplay,MWindow,ATitle,ATitle,None,S3_NULL,0,S3_NULL);
}

//----------------------------------------
//
//----------------------------------------

void S3_Window_Xlib::open(void) {
  XMapRaised(MDisplay,MWindow);
  #ifdef S3_LINUX_WAIT_FOR_MAPNOTIFY
  waitForMapNotify();
  #endif
  #ifdef S3_LIB
  startEventThread();
  #endif
}

//----------

void S3_Window_Xlib::close(void) {
  #ifdef S3_LIB
  stopEventThread();
  #endif
  XUnmapWindow(MDisplay,MWindow);
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::startTimer(int32 ms) {
  MTimerSleep = ms;
  MTimerThreadActive = true;
  pthread_create(&MTimerThread,S3_NULL,s3_xlib_timerproc,this);
}

//----------

void S3_Window_Xlib::stopTimer(void) {
  void* ret;
  MTimerThreadActive = false;
  pthread_join(MTimerThread,&ret);
}

//----------

void S3_Window_Xlib::flush(void) {
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::sync(void) {
  XSync(MDisplay,false);
}

//----------

void S3_Window_Xlib::lock(void) {
  XLockDisplay(MDisplay);
}

//----------

void S3_Window_Xlib::unlock(void) {
  XUnlockDisplay(MDisplay);
}


//----------------------------------------
//
//----------------------------------------

void S3_Window_Xlib::reparent(void* AParent) {
  MParentWindow = (Window)AParent;
  XReparentWindow(MDisplay,MWindow,MParentWindow,0,0);
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::sendEvent(uint32 AValue) {
  MSendEvent.type         = ClientMessage;
  MSendEvent.message_type = MCustomAtom;
  MSendEvent.display      = MDisplay;
  MSendEvent.window       = MWindow;
  MSendEvent.format       = 32;
  MSendEvent.data.l[0]    = AValue;
  XSendEvent(MDisplay,MWindow,False,NoEventMask,(XEvent*)&MSendEvent);
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::eventLoop(void) {
  while (1) {
    XEvent event;
    XNextEvent(MDisplay, &event);
    uint32 data = event.xclient.data.l[0];
    if ((event.type==ClientMessage) && (data==MDeleteAtom)) break;
    else eventHandler(&event);
  }
}

void S3_Window_Xlib::renderLoop(void) {
  //on_render();
}

void S3_Window_Xlib::invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
  MExposeEvent.type     = Expose;
  MExposeEvent.display  = MDisplay;
  MExposeEvent.window   = MWindow; // mParent;
  MExposeEvent.x        = AXpos;
  MExposeEvent.y        = AYpos;
  MExposeEvent.width    = AWidth;
  MExposeEvent.height   = AHeight;
  MExposeEvent.count    = 0;
  XSendEvent(MDisplay,MWindow,False,ExposureMask,(XEvent*)&MExposeEvent);
  XFlush(MDisplay);
}

//void S3_Window_Xlib::invalidate(S3_Rect ARect) {
//  invalidate(ARect.x,ARect.y,ARect.w,ARect.h) {
//}

//----------

void S3_Window_Xlib::beginPaint(void) {
}

//----------

void S3_Window_Xlib::endPaint(void) {
}

//----------

void S3_Window_Xlib::setCursor(int32 ACursor) {
  if (ACursor < 0) ACursor = 0;
  if (MWindowCursor >= 0) resetCursor();
  MWindowCursor = XCreateFontCursor(MDisplay, s3_xlib_cursors[ACursor]);
  XDefineCursor(MDisplay,MWindow,MWindowCursor);
}

//----------

void S3_Window_Xlib::createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) {
}

//----------

void S3_Window_Xlib::destroyUserCursors(void) {
}

//----------

void S3_Window_Xlib::setCursorPos(int32 AXpos, int32 AYpos) {
  XWarpPointer(MDisplay,None,MWindow, 0, 0, 0, 0, AXpos,AYpos);
}

void S3_Window_Xlib::showCursor(void) {
  setCursor(s3_mc_default);
}

//----------

void S3_Window_Xlib::hideCursor(void) {
  if (MWindowCursor >= 0) resetCursor();
  MWindowCursor = XCreatePixmapCursor(MDisplay, MEmptyPixmap, MEmptyPixmap, &MBlack, &MBlack, 0, 0);
  XDefineCursor(MDisplay, MWindow, MWindowCursor);
}

//----------

void S3_Window_Xlib::grabCursor(void) {
  int32 which = ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
  which |= FocusChangeMask | EnterWindowMask | LeaveWindowMask,
  XGrabPointer(
    MDisplay,
    MWindow,        // the grab window
    False,          // whether the pointer events are to be reported as usual or reported with respect to the grab window if selected by the event mask.
    which,          // pointer event mask bits
    //GrabModeSync, // further processing of pointer events
    GrabModeAsync,
    GrabModeAsync,  // further processing of keyboard events
    //MWindow,      // the window to confine the pointer in or None.
    None,
    None,           // the cursor that is to be displayed during the grab or None.
    CurrentTime     // timestamp or CurrentTime.
  );
}

//----------

void S3_Window_Xlib::releaseCursor(void) {
  XUngrabPointer(MDisplay,CurrentTime);
}

//----------

void S3_Window_Xlib::warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
}

//----------

void S3_Window_Xlib::unwarpCursor(void) {
}

//----------

/*
  The drawables must have the same root and depth, or a BadMatch error results.
  TODO:
    MWindowPainter->gc() instead of APainter->gc()
    if it works, we can have S3_Drawable argument instead of S3_Painter
*/

//void S3_Window_Xlib::blit(int32 dst_x, int32 dst_y, S3_Surface* ASurface, int32 src_x, int32 src_y, int32 src_w, int32 src_h) {
//  //bool same_depth = (MScreenDepth == APainter->depth());
//  S3_Assert( /*same_depth*/ MScreenDepth == APainter->depth() );
//  //if (same_depth) {
//  XCopyArea(MDisplay,APainter->drawable(),MWindow,APainter->gc(),src_x,src_y,src_w,src_h,dst_x,dst_y);
//  //}
//}

//----------------------------------------------------------------------
// thread proc
//----------------------------------------------------------------------

#ifdef S3_LIB
  void* s3_xlib_threadproc(void* AData) {
    S3_Window_Xlib* win = (S3_Window_Xlib*)AData;
    if (win) {
      #ifndef S3_LINUX_EVENT_THREAD_SLEEP
      while (win->MEventThreadActive) {
        XEvent ev;
        XNextEvent(win->MDisplay, &ev);
        if (ev.type == ClientMessage) {
          XClientMessageEvent* cev = (XClientMessageEvent*)&ev;
          uint32 dta = ev.xclient.data.l[0];
          if (cev->message_type == win->MCustomAtom) {
            if (dta == s3_ts_kill) pthread_exit(S3_NULL);
            else win->eventHandler(&ev);
          } // custom atom
        } //ClientMessage
        else
          win->eventHandler(&ev);
      } // while ..
      #else
      while (XPending(win->MDisplay) > 0) {
        XEvent ev;
        XNextEvent(win->MDisplay, &ev);
        if (ev.type == ClientMessage) {
          XClientMessageEvent* cev = (XClientMessageEvent*)&ev;
          uint32 dta = ev.xclient.data.l[0];
          if (cev->message_type == win->MCustomAtom) {
            if (dta == s3_ts_kill) pthread_exit(S3_NULL);
            else win->eventHandler(&ev);
          } // custom atom
        } //ClientMessage
        else
          win->eventHandler(&ev);
       } // pending
       if (win->MWindowMapped && win->MWindowExposed) win->on_idle();
        SSleep(S3_LINUX_THREADPROC_IDLESLEEP);
      } // while ..
     #endif // S3_LINUX_EVENT_THREAD_SLEEP
    } // win
    return S3_NULL;
  }
#endif

//----------------------------------------------------------------------
// timer proc
//----------------------------------------------------------------------

void* s3_xlib_timerproc(void* AData) {
  S3_Window_Xlib* win = (S3_Window_Xlib*)AData;
  if (win) {
    while (win->MTimerThreadActive) {
      win->on_window_timer();
      S3_Sleep(win->MTimerSleep); // hmm..
    }
  }
  return S3_NULL;
}

//----------------------------------------------------------------------
#endif
