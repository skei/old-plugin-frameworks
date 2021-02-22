#ifndef s3_window_linux_included
#define s3_window_linux_included
//----------------------------------------------------------------------

/*
  - if window is created with null listener, self is used..
  - buffer-events (on_xxxBuffer) are only called if S3_NO_WINDOW_BACKBUFFER is not defined
*/

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include "common/s3_rect.h"
#include "gui/base/s3_window_base.h"
#include "gui/s3_drawable.h"
#include "gui/s3_widget.h"
//#include "gui/s3_window_listener.h"

//----------

class S3_Window_Xlib
: public S3_Window_Base
//, public S3_WindowListener
, public S3_Widget
, public S3_Drawable {

  friend void* s3_xlib_threadproc(void* AData);
  friend void* s3_xlib_timerproc(void* AData);

  private:
    Display*            MDisplay;
    Window              MWindow;
    Window              MParentWindow;
    Window              MRootWindow;
    int32               MScreenNum;
    int32               MScreenDepth;
    Visual*             MVisual;
    Colormap            MColormap;
    Pixmap              MEmptyPixmap;
    XColor              MBlack;
    Atom                MDeleteAtom;
    Atom                MCustomAtom;
    XExposeEvent        MExposeEvent;   // invalidate
    XClientMessageEvent MSendEvent;     // sendEvent
    int32               MWindowCursor;
    pthread_t           MTimerThread;
    bool                MTimerThreadActive;
    int32               MTimerSleep;
    #ifdef S3_LIB
    pthread_t           MEventThread;
    bool                MEventThreadActive;
    #endif
    bool                MWindowExposed;
    bool                MWindowMapped;
  //protected:
  //  S3_WindowListener*  MListener;
  private:
    const char*         MTitle;
    void*               MParent;
    bool                MEmbedded;
    int32               MMouseXpos;
    int32               MMouseYpos;
    int32               MAdjustedWidth;
    int32               MAdjustedHeight;
    int32               MCurrentCursor;
    //S3_Painter*       MWindowPainter;

  public:
    S3_Window_Xlib(/*S3_WindowListener* AListener,*/ S3_Rect ARect, void* AParent=S3_NULL);
    virtual ~S3_Window_Xlib();
  public: // S3_Drawable
    bool      isWindow(void)  { return true; }
    Window    window(void)    { return MWindow; }
    int32     width(void)     { return MRect.w; }
    int32     height(void)    { return MRect.h; }
    int32     depth(void)     { return MScreenDepth; }
  //void*     buffer(void)    { return S3_NULL; }
    Display*  display(void)   { return MDisplay; }
    Drawable  drawable(void)  { return MWindow; }
  //#ifdef S3_XRENDER
  //Picture   picture(void)   { return MPicture; }
  //#endif
  private:
    void  eventHandler(XEvent* AEvent);
    void  removeDecoration(void);
    void  wantQuitEvent(void);
    int32 remapButton(int32 AButton);
    int32 remapKey(int32 AState);
    void  waitForMapNotify(void);
    #ifdef S3_LIB
    void  startEventThread(void);
    void  stopEventThread(void);
    #endif
    void  resetCursor(void);
  public:
    // set/get
    void  setPos(int32 AXpos, int32 AYpos) override;
    void  setSize(int32 AWidth, int32 AHeight) override;
    void  setTitle(const char* ATitle) override;
    void  open(void) override;
    void  close(void) override;
    void  startTimer(int32 ms) override;
    void  stopTimer(void) override;
  //void  flush(void) override;
  //void  sync(void) override;
  //void  lock(void) override;
  //void  unlock(void) override;
    void  reparent(void* AParent) override;
    void  sendEvent(uint32 AValue); // override;
    // event loop
    void  eventLoop(void) override;
    void  renderLoop(void) override;
    // painting
    void  invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) override;
    void  beginPaint(void) override;
    void  endPaint(void) override;
    // mouse
    void  setCursor(int32 ACursor) override;
    void  createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) override;
    void  destroyUserCursors(void) override;
    void  setCursorPos(int32 AXpos, int32 AYpos) override;
    //void getCursorPos(int32* AXpos, int32* AYpos) override;
    void  showCursor(void) override;
    void  hideCursor(void) override;
    void  grabCursor(void) override;
    void  releaseCursor(void) override;
    void  warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) override;
    void  unwarpCursor(void) override;
    // blit
    void  blit(int32 dst_x, int32 dst_y, S3_Painter* APainter, int32 src_x, int32 src_y, int32 src_w, int32 src_h) override;
};


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#include "common/s3_utils.h"

//----------

#ifdef S3_LIB
  void* s3_xlib_threadproc(void* AData);
#endif

void* s3_xlib_timerproc(void* AData);

//----------

struct s3_xlib_motif_hints {
  uint32  flags;
  uint32  functions;
  uint32  decorations;
  int32   inputMode;
  uint32  status;
};

//----------

// http://tronche.com/gui/x/xlib/appendix/b/

//static
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

//----------

//static
char s3_xlib_blank_cursor[] = { 0,0,0,0,0,0,0,0 };

//----------------------------------------------------------------------


/*
  By convention, the right and bottom edges of the rectangle are
  normally considered exclusive. In other words, the pixel whose
  coordinates are ( right, bottom ) lies immediately outside of the
  rectangle. For example, when RECT is passed to the FillRect function,
  the rectangle is filled up to, but not including, the right column and
  bottom row of pixels. This structure is identical to the RECTL
  structure.

    const char *display = ":0"; // Use NULL for default (environment variable)
    if (!(d_dpy = XOpenDisplay(display))) {
      fprintf(stderr, "Couldn't open X11 display\n");
      return 100;
    }

  ---

  we call XOpenDisplay with a ptr to swamask..
  does xlib copy this, or should we need to save it as a class member?

*/

S3_Window_Xlib::S3_Window_Xlib(/*S3_WindowListener* AListener,*/ S3_Rect ARect, void* AParent)
: S3_Window_Base(/*AListener,*/ARect,AParent)
, S3_Widget(ARect,s3_wa_fillClient) {

  //if (AListener) MListener = AListener;
  //else MListener = this;
  MName               = "S3_Window_Xlib";
  MTitle              = "S3_Window";
  MParent             = AParent;
  MMouseXpos          = -1;
  MMouseYpos          = -1;
  MWindowCursor       = -1;
  MTimerThread        = 0;
  MTimerThreadActive  = false;
  MTimerSleep         = 30;
  #ifdef S3_LIB
  MEventThread        = 0;
  MEventThreadActive  = false;
  #endif
//MWindowCreated      = false;
  MWindowExposed      = false;
  MWindowMapped       = false;
//MWidget             = S3_NULL;
  MDisplay            = XOpenDisplay(S3_NULL);
  MRootWindow         = DefaultRootWindow(MDisplay);
  //if (AParent) MParentWindow = (Window)AParent;
  //else MParentWindow = MRootWindow;
  #ifdef S3_LIB
  MParentWindow       = (Window)AParent;
  #else
  MParentWindow       = MRootWindow;
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
  swa.event_mask        = eventmask;  // http://tronche.com/gui/x/xlib/window/attributes/event-and-do-not-propagate.html
  swa.background_pixmap = None;       // http://tronche.com/gui/x/xlib/window/attributes/background.html
  swa.border_pixel      = 0;
  swa.colormap          = MColormap;  // http://tronche.com/gui/x/xlib/window/attributes/colormap.html
  uint32 swamask        = CWEventMask
                        | CWBackPixmap
                        | CWBorderPixel
                        | CWColormap;
  MWindow = XCreateWindow(
    MDisplay,
    MParentWindow,
    MRect.x,  // x
    MRect.y,  // y
    MRect.w,  //MWidth,
    MRect.h,  //MHeight,
    0,
    MScreenDepth,
    InputOutput,
    MVisual,
    swamask,
    &swa
  );
  #ifdef S3_LIB
  removeDecoration();
  #else
  wantQuitEvent();
  #endif
  MEmptyPixmap  = XCreateBitmapFromData(MDisplay, MWindow, s3_xlib_blank_cursor, 8, 8);
  MBlack.red    = 0;
  MBlack.green  = 0;
  MBlack.blue   = 0;
  MBlack.flags  = (DoRed | DoGreen | DoBlue);
  XAllocColor(MDisplay,XDefaultColormap(MDisplay,0),&MBlack);
  //MWindowPainter = new S3_Painter(this);
}

//----------

/*
  The pthread_join() function suspends execution of the calling thread
  until the target thread terminates, unless the target thread has
  already terminated.

  ---

  The XDestroyWindow() function destroys the specified window as well
  as all of its subwindows.If the window specified by the w argument is
  mapped, it is unmapped automatically.

  ---

  The XCloseDisplay() function closes the connection to the X server for the
  display specified in the Display structure and destroys all windows,
  resource IDs (Window, Font, Pixmap, Colormap, Cursor, and GContext), or
  other resources that the client has created on this display, unless the
  close-down mode of the resource has been changed (see XSetCloseDownMode()).
  Therefore, these windows, resource IDs, and other resources should never be
  referenced again or an error will be generated. Before exiting, you should
  call XCloseDisplay() explicitly so that any pending errors are reported as
  XCloseDisplay() performs a final XSync() operation.
*/

S3_Window_Xlib::~S3_Window_Xlib() {
  //delete MWindowPainter;
  if (MTimerThreadActive) stopTimer();
  #ifdef S3_LIB
  if (MEventThreadActive) stopEventThread();
  #endif
  XFreePixmap(MDisplay,MEmptyPixmap);
  if (MWindowCursor >= 0) XFreeCursor(MDisplay,MWindowCursor);
  XDestroyWindow(MDisplay,MWindow);
  //XFlush(MDisplay);
  XCloseDisplay(MDisplay);
}

//----------

void S3_Window_Xlib::eventHandler(XEvent* AEvent) {
  //int32 x, y, b, s, k; // w, h
  //int32 w,h;
  //SRect rec;

  switch(AEvent->type) {

    case ButtonPress: {
      //S3_Trace("click\n");
      int32 x = AEvent->xbutton.x;
      int32 y = AEvent->xbutton.y;
      int32 b = remapButton(AEvent->xbutton.button);
      int32 s = remapKey(AEvent->xbutton.state);
      //if (MListener) MListener->
      on_widgetMouseDown(this, x, y, b, s);
      if (MFlags&s3_wf_capture) grabCursor();
    }
    break;

    case ButtonRelease: {
      //S3_Trace("release\n");
      int32 x = AEvent->xbutton.x;
      int32 y = AEvent->xbutton.y;
      int32 b = remapButton(AEvent->xbutton.button);
      int32 s = remapKey(AEvent->xbutton.state);
      //if (MListener) MListener->
      on_widgetMouseUp(this, x, y, b, s);
      if (MFlags&s3_wf_capture) releaseCursor();
    }
    break;

    case ClientMessage: {
      int32 i = AEvent->xclient.data.l[0];
      switch(i) {
        case s3_ts_timer:
          //if (MWindowMapped && MWindowExposed)
          //if (MListener) MListener->
          on_windowTimer(this);
          break;
        case s3_ts_idle:
          if (MWindowMapped && MWindowExposed) {
            //if (MListener) MListener->
            on_windowIdle(this);
          }
          break;
        //case s3_ts_kill:
        //  break;
      }
    }
    break;

    case ConfigureNotify: {
      //int32 x = AEvent->xconfigure.x;
      //int32 y = AEvent->xconfigure.y;
      int32 w = AEvent->xconfigure.width;
      int32 h = AEvent->xconfigure.height;

      // see if there are more, and use last w/h
      // or should we use 'combine'???

      while (XCheckTypedWindowEvent(MDisplay, AEvent->xconfigure.window, ConfigureNotify, AEvent)) {
        w = AEvent->xconfigure.width;
        h = AEvent->xconfigure.height;
      }
      //if ((w != MRect.w) || (h != MRect.h)) {
      //  //FWinSurface.setSize(w,h); // TODO - cairo?
      //  on_bufferResize(this,w,h);
      //  //MWidth = w;
      //  //MHeight = h;
      //  //*if (MListener) MListener->
      //  on_widgetSetSize(this,w,h);
      //  if (hasFlag(s3_wf_autoalign)) on_widgetAlign(this);
      //  //paintBuffer();//redraw;
      //  on_bufferPaint(this,S3_NULL,s3_pm_normal);//redraw;
      //}

      if ( (w!=MRect.w) || (h!=MRect.h) ) {
        //if (MListener) {
          #ifndef S3_NO_WINDOW_BACKBUFFER
          /*MListener->*/on_bufferResize(this,w,h);
          #else
          MListener->on_windowResize(this,w,h);
          #endif
        //}
        on_widgetSetSize(this,w,h);
        if (MFlags & s3_wf_autoalign) on_widgetAlign(this);
        #ifndef S3_NO_WINDOW_BACKBUFFER
        //if (MListener) MListener->
        on_bufferPaint(this,S3_NULL,s3_pm_normal); //redraw;
        #endif
      }
    }
    break;

    // no drawing (to screen) is allowed until the first Expose event arrives.

    case Expose: {
      //STrace("--- Expose\n");
      MWindowExposed = true;
      S3_Rect rec;
      rec.x = AEvent->xexpose.x;
      rec.y = AEvent->xexpose.y;
      rec.w = AEvent->xexpose.width;
      rec.h = AEvent->xexpose.height;
      // combine all exposes, instead of doing them one by on..
      // flicker?
      while (XCheckTypedWindowEvent(MDisplay,AEvent->xexpose.window,Expose,AEvent)) {
        rec.combine(AEvent->xexpose.x,AEvent->xexpose.y,AEvent->xexpose.width,AEvent->xexpose.height);
      }

      //beginPaint();
      ////paintBuffer(self,rec);
      //on_bufferBlit(this,rec);
      //endPaint();

      beginPaint();
      //rc = S3_Rect( MWinPaintStruct.rcPaint.left,
      //              MWinPaintStruct.rcPaint.top,
      //              MWinPaintStruct.rcPaint.right -  MWinPaintStruct.rcPaint.left + 1,
      //              MWinPaintStruct.rcPaint.bottom - MWinPaintStruct.rcPaint.top + 1);
      if (MListener) {
        #ifndef S3_NO_WINDOW_BACKBUFFER
        MListener->on_bufferBlit(this,rec);
        #else
        MListener->on_windowPaint(this,rec);
        #endif
      }
      endPaint();
    }
    break;

    case KeyPress: {
      //int32 k = AEvent->xkey.keycode;
      int32 s = AEvent->xkey.state;
      char c[2];
      //int l;
      KeySym keysym; // = XID = unsigned long
      /*l =*/ XLookupString(&AEvent->xkey, c, 1, &keysym, S3_NULL);
      //if (l==0) c[0] = '\0';
      uint32 ks = remapKey(keysym);
      //STrace("key %i\n",ks);
      //DTrace("keycode:%i, state:%i, l:0x%02X = '%s', ks:0x%04X = %c\n",k,s,c[0],c,ks,ks);
      /*if (MListener) MListener->*/on_widgetKeyDown(this,/*k*/ks,s);
    }
    break;

    case KeyRelease: {
      //int32 k = AEvent->xkey.keycode;
      int32 s = AEvent->xkey.state;
      char c[2];
      //int l;
      KeySym keysym; // = XID = unsigned long
      /*l =*/ XLookupString(&AEvent->xkey, c, 1, &keysym, S3_NULL);
      //if (l==0) c[0] = '\0';
      uint32 ks = remapKey(keysym);
      //STrace("key %i\n",ks);
      //DTrace("keycode:%i, state:%i, l:0x%02X = '%s', ks:0x%04X = %c\n",k,s,c[0],c,ks,ks);
      /*if (MListener) MListener->*/on_widgetKeyUp(this,/*k*/ks, s);
    }
    break;

    case MapNotify: {
      MWindowMapped = true;
      //if (draw->expect_map_change) {
      //  draw->expect_map_change = false;
      //} else {
      //  draw->iconify_notify(true);
      //  draw->event_handlers->Iconify_Notify(draw, true);
      //}
    }
    break;

    case MotionNotify: {
      //S3_Trace("move\n");
      int32 x = AEvent->xbutton.x;
      int32 y = AEvent->xbutton.y;
      //int32 s = AEvent->xbutton.state;
      int32 s = remapKey(AEvent->xbutton.state);
      /*if (MListener) MListener->*/on_widgetMouseMove(this, x, y, s);
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

    //else KTrace('unhandled event: ',[AEvent->_type]);

  } // case

}


//----------

#define MWM_HINTS_DECORATIONS (1 << 1)
#define PROP_MOTIF_WM_HINTS_ELEMENTS 5

void S3_Window_Xlib::removeDecoration(void) {
  s3_xlib_motif_hints hints;
  //SMemset(&hints,0,sizeof(hints));   // shouldn't be necessary???
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
  //S3_Trace("remapKey 0x%X\n",AState);
  int32 result = s3_mb_none;
  if ((AState & 1  ) != 0) result |= s3_mk_shift;
  //if ((AState & 2  ) != 0) result |= s3_mk_caps;  // todo
  if ((AState & 4  ) != 0) result |= s3_mk_ctrl;
  if ((AState & 8  ) != 0) result |= s3_mk_alt;
  //if ((AState & 128) != 0) result |= s3_mk_altgr; // todo
  return result;
}

//----------

/*
  X is based upon a client-server architecture
  The map request is asynchronous, meaning that the time this instruction is
  executed doesn't tell us when the window is actually mapped.
  To be sure, we've to wait for the server to send us a MapNotify event

  When we have exited the loop, we have good confidence that the window appears
  on the screen. Actually, this may not be the case since, for example, the
  user may have iconified it using the window manager, but for now, we assume
  the window actually appears.

  ----------

  Another type of event you may wish to check for is the MapNotify event.
  This is raised when the window is mapped to the screen.
  So far we have assumed that the window was mapped as soon as we called
  XMapWindow or XMapRaised. In practise the window manager may have intervened
  and delayed the process. To verify that the window has been mapped (before we
  start drawing in it) you can ask for a MapNotify event via the
  StructureNotifyMask. An alternative is to just wait for the first Expose
  event before starting to draw on the window.

  ----------

  Note that you may not get events about your own windows getting mapped.
  This is because the WM is likely to reparent top-level windows to be children
  not of the root, but of intermediate decoration windows.
  There are two ways to cope with the situation:
  * Check if your window parent, the parent of the parent, ... etc
    is the mapped window of the event.
  * Add XSelectInput (d, yourwindow, StructureNotifyMask); to the mix.
  Note the first select has SubstructureNotifyMask and the second one
  StructureNotifyMask, a different mask.

  ----------


  1)

  for(;;) {
     XEvent e;
     XNextEvent(dpy, &e);
     if (e.type == MapNotify) break;
  }

  2)

  Waiting for MapNotify to inform that the window is mapped onto the screen
  for (XEvent e; e.type != MapNotify; XNextEvent(d_dpy, &e));

  #ifdef H_WAITFORNOTIFY
  static Bool h_WaitForNotify(Display* d, XEvent* e, char* arg)
  {
    return (e->type==MapNotify) & (e->xmap.window==(Window)arg);
  }
  #endif
  XEvent event;
  XIfEvent(m_Display,&event,h_WaitForNotify,(XPointer)m_Window);
*/

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

// [internal]
void S3_Window_Xlib::resetCursor(void) {
  XUndefineCursor(MDisplay,MWindow);
  XFreeCursor(MDisplay,MWindowCursor);
  MWindowCursor = -1;
}

//    #endif // 0

//------------------------------
// set/get
//------------------------------

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

/*
  The third and fourth items indicate the name which appears at the top of
  the window and the name of the minimized window respectively.
*/

void S3_Window_Xlib::setTitle(const char* ATitle) {
  XSetStandardProperties(MDisplay,MWindow,ATitle,ATitle,None,S3_NULL,0,S3_NULL);
}

//------------------------------
//
//------------------------------

/*
  http://tronche.com/gui/x/xlib-tutorial/
  - "Did you wait for a MapNotify before drawing your line ?"
    (more wizard doubletalk)
  - "No, why ?
  - X has a stateless drawing model, the content of the window may be lost when
    the window isn't on the screen." (overflow, why can't these wizard guys
    speak just like you and me ?) "You've to wait for a MapNotify before
    drawing."

    for(;;) {
      XEvent e;
      XNextEvent(dpy, &e);
      if (e.type == MapNotify) break;
    }

  ---

  * as a vst (in jost), i don't get MapNotify events for my own window (only parent?
  * freezes x in certain situations (in a dual monitor setup)

  ---

  http://tronche.com/gui/x/xlib/window/XMapWindow.html
  If the window is an InputOutput window, XMapWindow() generates Expose events
  on each InputOutput window that it causes to be displayed. If the client maps
  and paints the window and if the client {s processing events, the window
  is painted twice. To avoid this, first ask for Expose events and then map the
  window, so the client processes input events as usual. The event list will
  include Expose for each window that has appeared on the screen. The client's
  normal response to an Expose event should be to repaint the window. This
  method usually leads to simpler programs and to proper interaction with
  window managers.

  ---

  https://forum.winehq.org/viewtopic.php?f=2&t=23924
  // FIXME without this ugly delay, the VST window sometimes stays black.
  usleep(100000);
  XMapWindow(display, child);
  XSync(display, false);
  XCloseDisplay(display);

  ---

  // holos:
  virtual void show(void) {
    XMapWindow(m_Display,m_Window);
    XEvent event;
    XIfEvent(m_Display,&event,h_WaitForNotify,(char*)m_Window);
  }
}

//function _WaitForNotify(d:PDisplay; e:PXEvent; arg:TXPointer) : LongInt;
//{
//  if (e->_type = MapNotify) and (e->xmap.window = TWindow(arg) ) then result := 1
//  else result := 0;
//end;

---

  DPF: uses XMapRaised
*/

void S3_Window_Xlib::open(void) {
  /*
  ShowWindow(MWinHandle,SW_SHOW);
  #ifdef S3_WIN32_IDLE_TIMER
  SetTimer(MWinHandle,s3_ts_idle,S3_WIN32_IDLE_MS,S3_NULL);
  #endif
  */
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
  /*
  #ifdef S3_WIN32_IDLE_TIMER
  KillTimer(MWinHandle,s3_ts_idle);
  #endif
  ShowWindow(MWinHandle,SW_HIDE);
  */
  #ifdef S3_LIB
  stopEventThread();
  #endif
  XUnmapWindow(MDisplay,MWindow);
  XFlush(MDisplay);
  //MWindowExposed = false;
}

//----------

void S3_Window_Xlib::startTimer(int32 ms) {
  MTimerSleep = ms;
  MTimerThreadActive = true;
  pthread_create(&MTimerThread,S3_NULL,s3_xlib_timerproc,this);
}

//----------

// The KillTimer function does not remove WM_TIMER messages already posted
// to the message queue.

void S3_Window_Xlib::stopTimer(void) {
  void* ret;
  MTimerThreadActive = false;
  pthread_join(MTimerThread,&ret);
  S3_Trace("timer stopped..\n");
}

//----------

//void S3_Window_Xlib::flush(void) {
//  XFlush(MDisplay);
//}
//
////----------
//
//void S3_Window_Xlib::sync(void) {
//  XSync(MDisplay,false);
//}
//
////----------
//
//void S3_Window_Xlib::lock(void) {
//  XLockDisplay(MDisplay);
//}
//
////----------
//
//void S3_Window_Xlib::unlock(void) {
//  XUnlockDisplay(MDisplay);
//}

//----------

/*
  http://tronche.com/gui/x/xlib/window-and-session-manager/XReparentWindow.html
  A BadMatch error results if: The new parent window is not on the same
  screen as the old parent window.
*/

void S3_Window_Xlib::reparent(void* AParent) {
  MParentWindow= (Window)AParent;
  XReparentWindow(MDisplay,MWindow,MParentWindow,0,0);
  XFlush(MDisplay);
}

//----------

void S3_Window_Xlib::sendEvent(uint32 AValue) {
  //if (AValue==s3_ts_kill) {
  //  STrace("sending event: s3_ts_kill\n");
  //  STrace("  MDisplay %x\n",MDisplay);
  //  STrace("  MWindow  %i\n",MWindow);
  //}
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
  } // while
}

//----------

void S3_Window_Xlib::renderLoop(void) {
  // if (MListener) MListener->on_windowRenderFrame(this);
}

//----------

/*
  should this be static? what if we're sending off an event, and
  returning from this method.. the ev variable goes out of scope,
  and the event points to 'dangling pointer'???
*/

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

//----------

void S3_Window_Xlib::beginPaint(void) {
}

//----------

void S3_Window_Xlib::endPaint(void) {
}

//----------

void S3_Window_Xlib::setCursor(int32 ACursor) {
  //if (ACursor != MPrevCursor) MPrevCursor = ACursor;
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

/*
  https://tronche.com/gui/x/xlib/input/XWarpPointer.html

  If dest_w is None, XWarpPointer() moves the pointer by the offsets
  (dest_x, dest_y) relative to the current position of the pointer. If
  dest_w is a window, XWarpPointer() moves the pointer to the offsets
  (dest_x, dest_y) relative to the origin of dest_w. However, if src_w is a
  window, the move only takes place if the window src_w contains the
  pointer and if the specified rectangle of src_w contains the pointer.

  The src_x and src_y coordinates are relative to the origin of src_w. If src_height is zero, it is replaced with the current height of src_w minus src_y. If src_width is zero, it is replaced with the current width of src_w minus src_x.

  If you do use this function, it generates events just as if the user had
  instantaneously moved the pointer from one position to another. Note that
  you cannot use XWarpPointer() to move the pointer outside the confine_to
  window of an active pointer grab. An attempt to do so will only move the
  pointer as far as the closest edge of the confine_to window.
*/

// Moves the cursor to the specified screen coordinates
// will fire a mouse move event..

void S3_Window_Xlib::setCursorPos(int32 AXpos, int32 AYpos) {
  XWarpPointer(MDisplay,None,MWindow, 0, 0, 0, 0, AXpos,AYpos);
}

//----------

//virtual // S3_Window_Base
//void getCursorPos(int32* AXpos, int32* AYpos) {
//  *AXpos = 0;
//  *AYpos = 0;
//}

//----------

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

/*
  If owner_events is False, all generated pointer events are reported with
  respect to grab_window and are reported only if selected by event_mask.
  If owner_events is True and if a generated pointer event would normally
  be reported to this client, it is reported as usual. Otherwise, the event
  is reported with respect to the grab_window and is reported only if
  selected by event_mask. For either value of owner_events, unreported
  events are discarded.

  If the pointer_mode is GrabModeAsync, pointer event processing continues
  as usual. If the pointer is currently frozen by this client,
  the processing of events for the pointer is resumed. If the pointer_mode
  is GrabModeSync, the state of the pointer, as seen by client applications,
  appears to freeze, and the X server generates no further pointer events
  until the grabbing client calls XAllowEvents() or until the pointer grab
  is released. Actual pointer changes are not lost while the pointer is
  frozen; they are simply queued in the server for later processing.
*/

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

/*
  XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width, src_height, dest_x, dest_y)

    Display *display;
    Window src_w, dest_w;
    int src_x, src_y;
    unsigned int src_width, src_height;
    int dest_x, dest_y;

    display 	Specifies the connection to the X server.
    src_w 	Specifies the source window or None.
    dest_w 	Specifies the destination window or None.
    src_x
    src_y
    src_width
    src_height 	Specify a rectangle in the source window.
    dest_x
    dest_y
*/

/*
  generates events just as if the user had instantaneously moved the pointer from one position to another.
*/

void S3_Window_Xlib::warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {
}

//----------

void S3_Window_Xlib::unwarpCursor(void) {
}

//----------

/*
  XCopyArea(display, src, dest, gc, src_x, src_y, width, height,  dest_x, dest_y)
        Display *display;
        Drawable src, dest;
        GC gc;
        int src_x, src_y;
        unsigned int width, height;
        int dest_x, dest_y;

  display       Specifies the connection to the X server.
  src, dest     Specify the source and destination rectangles to be combined.
  gc            Specifies the GC.
  src_x, src_y 	Specify the x and y coordinates, which are relative to the origin of the source rectangle and specify its upper-left corner.
  width,height 	Specify the width and height, which are the dimensions of both the source and destination rectangles.
  dest_x,dest_y Specify the x and y coordinates, which are relative to the origin of the destination rectangle and specify its upper-left corner


  - The drawables must have the same root and depth, or a BadMatch error results.

*/

void S3_Window_Xlib::blit(int32 dst_x, int32 dst_y, S3_Painter* APainter, int32 src_x, int32 src_y, int32 src_w, int32 src_h) {
  bool same_depth = (MScreenDepth == APainter->depth());
  //if (MScreenDepth != APainter->depth()) { S3_Trace("depth doesn't match!\n!"); }
  if (same_depth) {
    XCopyArea(MDisplay,APainter->drawable(),MWindow,APainter->gc(),src_x,src_y,src_w,src_h,dst_x,dst_y);
  }
}

//----------------------------------------------------------------------
// thread proc
//----------------------------------------------------------------------

/*
  notes to self:
  - serialized (?).. we receive all events through this thread function,
    and we call the window's event handler for each of them, and process
    it there before we return here, and check for new events..
  - we're polling for events, not being called (possibly reentrant) like a
    callback, so while we're processing one event, we will not get another
    one interrupting us..
  - so, if ALL drawing originates from the window's event handler, it should
    be safe?
  - and we won't get interrupted by a Expose event when we're for example
    still processing a ConfigureNotify event?

  - how does effEditIdle fit into all of this?
    > we don't use effEditIdle in linux! the event thread calls on_idle
      at regular intervals..

  ---

  at the moment, this is used only in gui plugins
  window.open/show

  ---

  The pthread_exit() function terminates the calling thread and makes the value
  ptr available to any successful join with the terminating thread.

*/

//----------

/*
  todo:
  some kind of timerpool system, where widgets (or whatever) - STimerListener
  can register itself.. we set the S3_LINUX_THREADPROC_IDLESLEEP to something
  small, like 1 ms, and each timer can have a ticks variable.. each 1ms we check
  this list, and update counters, and possibly call on_timer() for the
  listeners
*/

//----------

#ifdef S3_LIB
void* s3_xlib_threadproc(void* AData) {
  S3_Window_Xlib* win = (S3_Window_Xlib*)AData;
  if (win) {

    //while (1) {
    //while (win->MEventThreadActive) {

    #ifndef S3_LINUX_EVENT_THREAD_SLEEP

    //while (1) {
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
     //win->sendEvent(sts_idle);
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

/*
  NOTE: could be called concurrent with threadproc!
  post message to xlib queue, to be sure the events are processed
  'in order' and in the right thread..
    win->sendEvent(sts_timer);
*/

void* s3_xlib_timerproc(void* AData) {
  S3_Window_Xlib* win = (S3_Window_Xlib*)AData;
  if (win) {
    while (win->MTimerThreadActive) {
      //if (MWindowMapped && MWindowExposed)
      win->on_windowTimer(win);
      //win->sendEvent(sts_timer);
      S3_Sleep(win->MTimerSleep); // hmm..
    }
  }
  return S3_NULL;
}

//----------------------------------------------------------------------
#endif
