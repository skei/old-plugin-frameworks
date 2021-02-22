#ifndef kode_xcb_window_included
#define kode_xcb_window_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_time.h"
#include "gui/kode_drawable.h"
#include "gui/kode_widget.h"
//#include "gui/kode_window.h"
#include "gui/kode_widget_window.h"
#include "gui/xcb/kode_xcb.h"
#include "gui/xcb/kode_xcb_painter.h"

#ifdef KODE_CAIRO
#include "gui/cairo/kode_cairo.h"
#endif

//----------------------------------------------------------------------

void* xcb_event_thread_proc(void* AWindow);
void* xcb_timer_thread_proc(void* AWindow);

//----------

// https://tronche.com/gui/x/xlib/appendix/b/
#include <X11/cursorfont.h>

uint32_t xcb_cursors[20] = {
  XC_left_ptr,          // smc_default
  XC_left_ptr,          // smc_Arrow
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
  XC_hand2,             // smc_Hand
  XC_hand1,             // smc_Finger
  XC_crosshair,         // smc_Cross
  XC_pencil,            // smc_Pencil
  XC_plus,              // smc_Plus
  XC_question_arrow,    // smc_Question
  XC_xterm              // smc_Ibeam
};

const char* xcb_wm_cursors[20] = {
  "left_ptr",          // 0 KODE_CURSOR_DEFAULT
  "left_ptr",          // KODE_CURSOR_ARROW
  "sb_up_arrow",        // KODE_CURSOR_ARROWUP
  "sb_down_arrow",     // KODE_CURSOR_ARROWDOWN
  "sb_left_arrow",     // KODE_CURSOR_ARROWLEFT
  "sb_right_arrow",    // KODE_CURSOR_ARROWRIGHT
  "sb_v_double_arrow", // KODE_CURSOR_ARROWUPDOWN
  "sb_h_double_arrow", // KODE_CURSOR_ARROWLEFTRIGHT
  "top_left_corner",   // KODE_CURSOR_ARROWDIAGLEFT
  "top_right_corner",  // KODE_CURSOR_ARROWDIAGRIGHT
  "fleur",             // KODE_CURSOR_MOVE
  "watch",             // KODE_CURSOR_WAIT
  "clock",             // KODE_CURSOR_ARROWWAIT
  "hand2",             // KODE_CURSOR_HAND
  "hand1",             // KODE_CURSOR_FINGER
  "crosshair",         // KODE_CURSOR_CROSS
  "pencil",            // KODE_CURSOR_PENCIL
  "plus",              // KODE_CURSOR_PLUS
  "question_arrow",    // KODE_CURSOR_QUESTION
  "xterm"              // KODE_CURSOR_IBEAM
};

/*
  "left_ptr",   "left_side",    "left_tee"
  "right_ptr",  "right_side",   "right_tee"
  "top_ptr",    "top_side",     "top_tee"
  "bottom_ptr", "bottom_side",  "bottom_tee"
  "top_left_corner", "top_right_corner", "bottom_left_corner", "bottom_right_corner"
  "ul_angle", "ur_angle", "ll_angle", "lr_angle"
  "sb_left_arrow", "sb_right_arrow", "sb_up_arrow", "sb_down_arrow"
  "sb_h_double_arrow", "sb_v_double_arrow"
  "circle" (arrow w/forbidden sign)
  "cross"
  "crosshair"
  "double_arrow"
  "fleur" (move)
  "hand" (finger)
  "icon", "target"
  "pencil"
  "pirate" (x)
  "plus"
  "question_arrow"
  "watch"
  "xterm"
*/

//----------------------------------------------------------------------


class KODE_XcbWindow
//: public KODE_BaseWindow {
: public KODE_WidgetWindow {
//: public KODE_Window {

friend void* xcb_event_thread_proc(void* AWindow);
friend void* xcb_timer_thread_proc(void* AWindow);

//------------------------------
private:
//------------------------------

  xcb_connection_t*           MConnection                   = nullptr;
  xcb_window_t                MWindow                       = XCB_NONE;
  xcb_window_t                MRootWindow                   = XCB_NONE;
  xcb_window_t                MParentWindow                 = XCB_NONE;
  xcb_screen_t*               MScreen                       = nullptr;
  uint32_t                    MScreenNum                    = 0;
  uint32_t                    MScreenWidth                  = 0;
  uint32_t                    MScreenHeight                 = 0;
  uint32_t                    MScreenDepth                  = 0;
  //int32_t                     MScreenNumber                 = 0;
  xcb_window_t                MScreenRootWindow             = XCB_NONE;
  xcb_gcontext_t              MScreenGC                     = XCB_NONE;
  xcb_visualid_t              MScreenVisual                 = XCB_NONE;
  xcb_colormap_t              MScreenColormap               = XCB_NONE;
  xcb_atom_t                  MWMProtocolsAtom              = XCB_NONE; // KWantQuitEvents
  xcb_atom_t                  MWMDeleteWindowAtom           = XCB_NONE; // KWantQuitEvents
//xcb_atom_t                  MCustomAtom                   = XCB_NONE; // sendEvent
  xcb_pixmap_t                MEmptyPixmap                  = XCB_NONE;
  xcb_cursor_t                MHiddenCursor                 = XCB_NONE;
  xcb_cursor_t                MWindowCursor                 = XCB_NONE;
  bool                        MCursorHidden                 = false;
  pthread_t                   MTimerThread                  = 0;
  bool                        MTimerThreadActive            = false;
  int32_t                     MTimerSleep                   = 20; // 20 ms = 50 hz
  pthread_t                   MEventThread                  = 0;
  bool                        MEventThreadActive            = false;
  //KODE_Rect                   MWindowRect                   = {0};
  int32_t                     MWindowXpos                   = 0;
  int32_t                     MWindowYpos                   = 0;
  int32_t                     MWindowWidth                  = 0;
  int32_t                     MWindowHeight                 = 0;
  bool                        MWindowMapped                 = false;
  bool                        MWindowExposed                = false;
  bool                        MQuitEventLoop                = false;
  // Every X11 event is 32 bytes long. Therefore, XCB will copy 32 bytes.
  // In order to properly initialize these bytes, we allocate 32 bytes even
  // though we only need less for an xcb_configure_notify_event_t
  char*                       MExposeEventBuffer[32]        = {0};
  char*                       MClientMessageEventBuffer[32] = {0};
  xcb_expose_event_t*         MExposeEvent                  = (xcb_expose_event_t*)MExposeEventBuffer;
  xcb_client_message_event_t* MClientMessageEvent           = (xcb_client_message_event_t*)MClientMessageEventBuffer;

  bool                        MUseEventThread               = false;

  xcb_key_symbols_t*          MKeySyms                      = KODE_NULL;

//------------------------------
protected:
//------------------------------

  #ifdef KODE_CAIRO
  cairo_surface_t*            MCairoSurface                 = nullptr;
  #endif

  KODE_XcbPainter*            MWindowPainter                = nullptr;
  //KODE_Painter*            MWindowPainter                = nullptr;

//------------------------------
public:
//------------------------------

  KODE_XcbWindow(uint32_t AWidth, uint32_t AHeight, void* AParent=nullptr)
  //: KODE_BaseWindow(AWidth,AHeight,AParent) {
  : KODE_WidgetWindow(AWidth,AHeight) {
  //: KODE_Window(AWidth,AHeight,AParent) {
    
    XCB_Trace("xcb: KODE_XcbWindow AWidth %i AHeight %i AParent %p\n",AWidth,AHeight,AParent);

    if (AParent) MUseEventThread = true;
    else MUseEventThread = false;

    int screen_num;
    MConnection = xcb_connect(nullptr,&screen_num);
    xcb_screen_iterator_t iter;
    iter = xcb_setup_roots_iterator(xcb_get_setup(MConnection));
    for (; iter.rem; --screen_num, xcb_screen_next(&iter)) {
      if (screen_num == 0) {
        MScreen = iter.data;
        break;
      }
    }

    MScreenNum        = screen_num;
    MScreenWidth      = MScreen->width_in_pixels;
    MScreenHeight     = MScreen->height_in_pixels;
    MScreenDepth      = MScreen->root_depth;
    MScreenRootWindow = MScreen->root;
    MScreenColormap   = MScreen->default_colormap;
    MScreenVisual     = MScreen->root_visual;
    MScreenGC = xcb_generate_id(MConnection);
    xcb_drawable_t draw = MScreen->root;
    uint32_t mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    uint32_t values[2];
    values[0] = MScreen->black_pixel;
    values[1] = MScreen->white_pixel;
    xcb_create_gc(MConnection, MScreenGC, draw, mask, values);
    if (AParent) MParentWindow = (xcb_window_t)(intptr_t)AParent;
    else MParentWindow = MScreen->root;
    uint32_t event_mask =
      XCB_EVENT_MASK_KEY_PRESS      |
      XCB_EVENT_MASK_KEY_RELEASE    |
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW   |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_EXPOSURE       |
      XCB_EVENT_MASK_STRUCTURE_NOTIFY;
    uint32_t window_mask =
      XCB_CW_BACK_PIXMAP  |
      XCB_CW_BORDER_PIXEL |
      XCB_CW_EVENT_MASK   |
      XCB_CW_COLORMAP;
    uint32_t window_mask_values[4] = {
      XCB_NONE,
      0,
      event_mask,
      MScreen->default_colormap
    };
    MWindow = xcb_generate_id(MConnection);
    xcb_create_window(
      MConnection,                    // connection
      XCB_COPY_FROM_PARENT,           // depth (same as root)
      MWindow,                        // window Id
      MParentWindow,                  // parent window
      0, 0,                           // x, y
      AWidth, AHeight,                // width, height
      0,                              // border_width
      XCB_WINDOW_CLASS_INPUT_OUTPUT,  // class
      MScreen->root_visual,           // visual
      window_mask,
      window_mask_values
    );
    //MWindowRect = KODE_Rect(AWidth,AHeight);
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    if (AParent) {
      //reparent(AParent);
      removeDecorations();
    }
    else {
      wantDeleteWindowEvent();
    }
    MEmptyPixmap = xcb_generate_id(MConnection);
    MHiddenCursor = xcb_generate_id(MConnection);
    xcb_create_pixmap(
      MConnection,
      1,
      MEmptyPixmap,
      MWindow,
      1,
      1
    );
    xcb_create_cursor(
      MConnection,
      MHiddenCursor,
      MEmptyPixmap,
      MEmptyPixmap,
      0, 0, 0,  // fore color
      0, 0, 0,  // back color
      0, 0      // x,y
    );
    MTimerThread        = 0;
    MTimerThreadActive  = false;
    MTimerSleep         = 30;
    //#ifndef KODE_XCB_NO_EVENT_THREAD
    MEventThread        = 0;
    MEventThreadActive  = false;
    //#endif
    xcb_flush(MConnection);

    #ifdef KODE_CAIRO
    MCairoSurface = createCairoSurface();
    #endif

    MWindowPainter = KODE_New KODE_XcbPainter(this);
    setCursor(KODE_CURSOR_DEFAULT);

    MKeySyms = xcb_key_symbols_alloc(MConnection);

  }

  //----------

  virtual ~KODE_XcbWindow() {
    
    XCB_Trace("xcb: ~KODE_XcbWindow\n");
    if (MTimerThreadActive) stopTimer();
    //#ifndef KODE_XCB_NO_EVENT_THREAD
    if (MEventThreadActive) stopEventThread();
    //#endif

    if (MWindowPainter) KODE_Delete MWindowPainter;

    #ifdef KODE_CAIRO
    if (MCairoSurface)  {
      cairo_device_t* device = cairo_device_reference( cairo_surface_get_device(MCairoSurface));
      cairo_device_finish(device);
      cairo_device_destroy(device);
      //cairo_surface_finish(MCairoSurface);
      cairo_surface_destroy(MCairoSurface);
    }
    #endif

    xcb_key_symbols_free(MKeySyms);

    xcb_free_pixmap(MConnection, MEmptyPixmap);
    xcb_free_cursor(MConnection,MHiddenCursor);
    if (MWindowCursor != XCB_NONE) xcb_free_cursor(MConnection,MWindowCursor);
    xcb_destroy_window(MConnection,MWindow);
    xcb_free_gc(MConnection,MScreenGC);
    xcb_disconnect(MConnection);

  }

//------------------------------
public: // drawable
//------------------------------

  bool              isWindow()            override { return true; }
  xcb_connection_t* getConnection()       override { return MConnection; }
  xcb_visualid_t    getVisual()           override { return MScreenVisual; }
  xcb_drawable_t    getDrawable()         override { return MWindow; }
  xcb_window_t      getWindow()           override { return MWindow; }
  uint32_t          getWidth()            override { return MWindowWidth; }
  uint32_t          getHeight()           override { return MWindowHeight; }
  uint32_t          getDepth()            override { return MScreenDepth; }

  #ifdef KODE_CAIRO
  bool              isCairo()             override { return true; }
  cairo_surface_t*  getCairoSurface(void) override { return MCairoSurface; }
  #endif

//------------------------------
public:
//------------------------------

  uint32_t getScreenDepth() {
    return MScreenDepth;
  }

  //----------

  /*
    Note:
    If drawable is a Window, then the function cairo_xcb_surface_set_size() must
    be called whenever the size of the window changes.
  */

  #ifdef KODE_CAIRO
  cairo_surface_t* createCairoSurface() {
    XCB_Trace("xcb: createCairoSurface\n");
    cairo_surface_t* surface = cairo_xcb_surface_create(
      MConnection,
      MWindow,
      kode_xcb_find_visual(MConnection,MScreenVisual),
      MWindowWidth,
      MWindowHeight
    );
    //check_cairo_surface_errors(surface);
    //cairo_xcb_surface_set_size(surface,MWindowRect.w,MWindowRect.h);
    return surface;
  }
  #endif

//------------------------------
private:
//------------------------------

  void quitEventLoop() {
    //XCB_Trace("xcb: quitEventLoop\n");
    MQuitEventLoop = true;
  }

  //----------

  static const unsigned long MWM_HINTS_FUNCTIONS   = 1 << 0;
  static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;
  //static const unsigned long MWM_DECOR_ALL         = 1 << 0;
  static const unsigned long MWM_DECOR_BORDER      = 1 << 1;
  static const unsigned long MWM_DECOR_RESIZEH     = 1 << 2;
  static const unsigned long MWM_DECOR_TITLE       = 1 << 3;
  static const unsigned long MWM_DECOR_MENU        = 1 << 4;
  static const unsigned long MWM_DECOR_MINIMIZE    = 1 << 5;
  static const unsigned long MWM_DECOR_MAXIMIZE    = 1 << 6;
  //static const unsigned long MWM_FUNC_ALL          = 1 << 0;
  static const unsigned long MWM_FUNC_RESIZE       = 1 << 1;
  static const unsigned long MWM_FUNC_MOVE         = 1 << 2;
  static const unsigned long MWM_FUNC_MINIMIZE     = 1 << 3;
  static const unsigned long MWM_FUNC_MAXIMIZE     = 1 << 4;
  static const unsigned long MWM_FUNC_CLOSE        = 1 << 5;

  struct WMHints {
    uint32_t flags;
    uint32_t functions;
    uint32_t decorations;
    int32_t  inputMode;
    uint32_t state;
  };

  //----------

  void removeDecorations() {
    XCB_Trace("xcb: removeDecorations\n");
    xcb_atom_t prop = kode_xcb_get_intern_atom(MConnection,"_MOTIF_WM_HINTS");
    if (prop) {
      WMHints hints;
      hints.flags = MWM_HINTS_DECORATIONS;
      hints.decorations = 0;
      const unsigned char* ptr = (const unsigned char*)(&hints);
      xcb_change_property(
        MConnection,
        XCB_PROP_MODE_REPLACE,
        MWindow,
        prop,     // hintsAtomReply->atom,
        prop,     // XCB_ATOM_WM_HINTS,
        32,
        5,        // PROP_MOTIF_WM_HINTS_ELEMENTS
        ptr
      );
    }
  }

  //----------

  void wantDeleteWindowEvent() {
    XCB_Trace("xcb: wantDeleteWindowEvent\n");
    xcb_intern_atom_cookie_t  protocol_cookie = xcb_intern_atom_unchecked(MConnection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_cookie_t  close_cookie    = xcb_intern_atom_unchecked(MConnection, 0, 16, "WM_DELETE_WINDOW");
    xcb_intern_atom_reply_t*  protocol_reply  = xcb_intern_atom_reply(MConnection, protocol_cookie, 0);
    xcb_intern_atom_reply_t*  close_reply     = xcb_intern_atom_reply(MConnection, close_cookie, 0);
    MWMProtocolsAtom    = protocol_reply->atom;
    MWMDeleteWindowAtom = close_reply->atom;
    //xcb_change_property(MConnection, XCB_PROP_MODE_REPLACE, MWindow, protocol_reply->atom, 4, 32, 1, &MDeleteWindowAtom);
    xcb_change_property(MConnection, XCB_PROP_MODE_REPLACE, MWindow, MWMProtocolsAtom, 4, 32, 1, &MWMDeleteWindowAtom);
    //xcb_flush(MConnection);
    free(protocol_reply); // note KODE_Malloc'ed ??
    free(close_reply);
  }

  //----------

  /*
    xcb_wait_for_event() doesn't flush
    have we flushed already?
    when we create the window?
    when we open it?
  */

  void waitForMapNotify() {
    XCB_Trace("xcb: waitForMapNotify\n");
    xcb_flush(MConnection);
    while (1) {
      xcb_generic_event_t* event;
      event = xcb_wait_for_event(MConnection);
      uint8_t e = event->response_type & ~0x80;
      free(event); // not KODE_Malloc'ed
      if (e == XCB_MAP_NOTIFY) break;
    }
  }

  //----------

  void startEventThread() {
    XCB_Trace("xcb: startEventThread\n");
    //#ifndef KODE_XCB_NO_EVENT_THREAD
    MEventThreadActive = true;
    pthread_create(&MEventThread,nullptr,xcb_event_thread_proc,this);
    //#endif
  }

  //----------

  void stopEventThread() {
    XCB_Trace("xcb: stopEventThread\n");
    //#ifndef KODE_XCB_NO_EVENT_THREAD
    void* ret;
    MEventThreadActive = false;
    sendEvent(KODE_THREAD_KILL,0);
    pthread_join(MEventThread,&ret);
    //#endif
  }

  //----------

  uint32_t remapButton(uint32_t AButton, uint32_t AState) {
    //XCB_Trace("xcb: remapButton AButton %i AState %i\n",AButton,AState);
    //KODE_Trace("remapButton AButton %i AState %i\n",AButton,AState);
    uint32_t b = AButton;
    return b;
  }

  //----------

  // https://github.com/etale-cohomology/xcb/blob/master/loop.c

  uint32_t remapKey(uint32_t AKey, uint32_t AState) {
    //XCB_Trace("xcb: remapKey AKey %i AState %i\n",AKey,AState);
    //uint32_t k = AKey;
    int col = 0;
    xcb_keysym_t keysym = xcb_key_symbols_get_keysym(MKeySyms,AKey,col);
    //xcb_keycode_t* keycode = xcb_key_symbols_get_keycode(MKeySyms,keysym);
    //KODE_Trace("AKey %i AState %i keysym %i keycode %i\n",AKey,AState,keysym,keycode[0]);
    //KODE_Free(keycode);
    return keysym;
  }

  //----------

  uint32_t remapState(uint32_t AState) {
    //XCB_Trace("xcb: remapState AState %i\n",AState);
    uint32_t s = KODE_KEY_NONE;
    if (AState & XCB_MOD_MASK_SHIFT)    s += KODE_KEY_SHIFT;
    if (AState & XCB_MOD_MASK_LOCK)     s += KODE_KEY_CAPS;
    if (AState & XCB_MOD_MASK_CONTROL)  s += KODE_KEY_CTRL;
    if (AState & XCB_MOD_MASK_1)        s += KODE_KEY_ALT;
    if (AState & XCB_MOD_MASK_5)        s += KODE_KEY_ALTGR;

    //if (AState & XCB_MOD_MASK_1) KODE_Trace("1\n");
    //if (AState & XCB_MOD_MASK_2) KODE_Trace("2\n");
    //if (AState & XCB_MOD_MASK_3) KODE_Trace("3\n");
    //if (AState & XCB_MOD_MASK_4) KODE_Trace("4\n");
    //if (AState & XCB_MOD_MASK_5) KODE_Trace("5\n");

    return s;
  }

  //----------

  void freeXcbCursor(void) {
    XCB_Trace("xcb: freeXcbCursor\n");
    if (MWindowCursor != XCB_NONE) {
      xcb_free_cursor(MConnection,MWindowCursor);
      MWindowCursor = XCB_NONE;
      //xcb_flush(MConnection);
    }
  }

  //----------

  void setXcbCursor(xcb_cursor_t ACursor) {
    XCB_Trace("xcb: setXcbCursor ACursor %i\n",ACursor);
    uint32_t mask;
    uint32_t values;
    mask   = XCB_CW_CURSOR;
    values = ACursor;
    xcb_change_window_attributes(MConnection,MWindow,mask,&values);
    // without xcb_flush, the mouse cursor wasn't updating properly (standalone)..
    xcb_flush(MConnection);
  }

  //----------

  void setWMCursor(uint32_t ACursor) {
    XCB_Trace("xcb: setWMCursor ACursor %i\n",ACursor);
    xcb_cursor_context_t *ctx;
    if (xcb_cursor_context_new(MConnection, MScreen, &ctx) >= 0) {
      const char* name = xcb_wm_cursors[ACursor];
      xcb_cursor_t cursor = xcb_cursor_load_cursor(ctx, name);
      if (cursor != XCB_CURSOR_NONE) {
        xcb_change_window_attributes(MConnection, MWindow, XCB_CW_CURSOR, &cursor);
      }
    }
    xcb_cursor_context_free(ctx);
    xcb_flush(MConnection);
  }

//------------------------------
public:
//------------------------------

  void setPos(int32_t AXpos, int32_t AYpos) override {
    XCB_Trace("xcb: setPos AXpos %i AYpos %i\n",AXpos,AYpos);
    MWindowXpos = AXpos;
    MWindowYpos = AYpos;
    static uint32_t values[] = {
      (uint32_t)AXpos,
      (uint32_t)AYpos
    };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_X|XCB_CONFIG_WINDOW_Y,values);
    xcb_flush(MConnection);
  }

  //----------

  /*
    cairo_xcb_surface_set_size:
    Informs cairo of the new size of the XCB drawable underlying the surface.
    For a surface created for a window (rather than a pixmap), this function
    must be called each time the size of the window changes. (For a subwindow,
    you are normally resizing the window yourself, but for a toplevel window,
    it is necessary to listen for ConfigureNotify events.)
    A pixmap can never change size, so it is never necessary to call this
    function on a surface created for a pixmap.
    If cairo_surface_flush() wasn't called, some pending operations might be
    discarded.
  */

  void setSize(int32_t AWidth, int32_t AHeight) override {
    KODE_Trace("%i %i\n",AWidth,AHeight);
    XCB_Trace("xcb: setSize AWidth %i AHeight %i\n",AWidth,AHeight);
    MWindowWidth = AWidth;
    MWindowHeight = AHeight;
    static uint32_t values[] = {
      (uint32_t)AWidth,
      (uint32_t)AHeight
    };
    xcb_configure_window(MConnection,MWindow,XCB_CONFIG_WINDOW_WIDTH|XCB_CONFIG_WINDOW_HEIGHT,values);
    xcb_flush(MConnection);
  }

  //----------

  void setTitle(const char* ATitle) override {
    XCB_Trace("xcb: setTitle ATitle %s\n",ATitle);
    xcb_change_property(
      MConnection,
      XCB_PROP_MODE_REPLACE,
      MWindow,
      XCB_ATOM_WM_NAME,
      XCB_ATOM_STRING,
      8,
      KODE_Strlen(ATitle),
      ATitle
    );
    xcb_flush(MConnection);
  }

  //----------

  void open(void) override {
    XCB_Trace("xcb: open\n");
    xcb_map_window(MConnection,MWindow);

    #ifdef KODE_XCB_WAIT_FOR_MAPNOTIFY
    waitForMapNotify();
    #endif

    //#ifdef KODE_CAIRO
    //MCairoSurface = createCairoSurface();
    //#endif

    // use the event thread only in plugins
    //#ifdef KODE_LIBRARY

//    #ifndef KODE_PLUGIN_EXE
//    //#ifndef KODE_XCB_NO_EVENT_THREAD
//    startEventThread();
//    //#endif
//    #endif

    if (MUseEventThread) startEventThread();

    //#endif
    //paint?
    xcb_flush(MConnection);
  }

  //----------

  void close(void) override {
    XCB_Trace("xcb: close\n");
    //xcb_flush(MConnection);

//    //#ifndef KODE_XCB_NO_EVENT_THREAD
//    stopEventThread();
//    //#endif

    if (MUseEventThread) stopEventThread();

    //#ifdef KODE_CAIRO
    //if (MCairoSurface) {
    //  cairo_surface_finish(MCairoSurface);
    //  cairo_surface_destroy(MCairoSurface);
    //}
    //#endif

    xcb_unmap_window(MConnection,MWindow);
    xcb_flush(MConnection);
  }

  //----------

  void flush(void) override {
    XCB_Trace("xcb: flush\n");
    xcb_flush(MConnection);
  }

  //----------

  void sync(void) override {
    XCB_Trace("xcb: sync\n");
    xcb_aux_sync(MConnection);
  }

  //----------

  void reparent(void* AParent) override {
    XCB_Trace("xcb: reparent AParent %p\n",AParent);
    MParentWindow = (intptr_t)AParent;
    xcb_reparent_window(MConnection,MWindow,MParentWindow,0,0);
    xcb_flush(MConnection);
  }

  //----------

  void startTimer(uint32_t ms) override {
    XCB_Trace("xcb: startTimer %i\n",ms);
    MTimerSleep = ms;
    MTimerThreadActive = true;
    pthread_create(&MTimerThread,nullptr,xcb_timer_thread_proc,this);
  }

  //----------

  void stopTimer(void) override {
    XCB_Trace("xcb: stopTimer\n");
    void* ret;
    MTimerThreadActive = false;
    pthread_join(MTimerThread,&ret);
  }

  //----------

  //MWindowCursor = XCreateFontCursor(MDisplay, kode_xlib_cursors[ACursor]);
  //XDefineCursor(MDisplay,MWindow,MWindowCursor);

  void setCursor(uint32_t ACursor) override {
    XCB_Trace("xcb: setCursor ACursor %i\n",ACursor);

    //if (!MCursorHidden) {
    //  freeXcbCursor();
    //  MWindowCursor = kode_xcb_create_font_cursor(MConnection, xcb_cursors[ACursor] );
    //  setXcbCursor(MWindowCursor);
    //}

    setWMCursor(ACursor);


  }

  //----------

  void setCursorPos(int32_t AXpos, int32_t AYpos) override {
    XCB_Trace("xcb: setCursorPos AXpos %i AYpos %i\n",AXpos,AYpos);
    xcb_warp_pointer(MConnection,XCB_NONE,MWindow,0,0,0,0,AXpos,AYpos);
    xcb_flush(MConnection);
  }

  //----------

  void hideCursor(void) override {
    XCB_Trace("xcb: hideCursor\n");
    if (!MCursorHidden) {
      setXcbCursor(MHiddenCursor);
      MCursorHidden = true;
    }
  }

  //----------

  void showCursor(void) override {
    XCB_Trace("xcb: showCursor\n");
    if (MCursorHidden) {
      setXcbCursor(MWindowCursor);
      MCursorHidden = false;
    }
  }

  //----------

  void grabCursor(void) override {
    XCB_Trace("xcb: grabCursor\n");
    int32_t event_mask =
      XCB_EVENT_MASK_BUTTON_PRESS   |
      XCB_EVENT_MASK_BUTTON_RELEASE |
      XCB_EVENT_MASK_POINTER_MOTION |
      XCB_EVENT_MASK_FOCUS_CHANGE   |
      XCB_EVENT_MASK_ENTER_WINDOW   |
      XCB_EVENT_MASK_LEAVE_WINDOW;
    xcb_grab_pointer(
      MConnection,
      0,                                      // owner_events
      MWindow, /*rootWindow,*/                // grab_window
      event_mask,                             // event_mask
      XCB_GRAB_MODE_ASYNC,                    // pointer_mode
      XCB_GRAB_MODE_ASYNC,                    // keyboard_mode
      XCB_NONE, /*rootWindow,*/               // confine_to
      XCB_NONE,                               // cursor
      XCB_CURRENT_TIME                        // timestamp
    );
    xcb_flush(MConnection);
  }

  //----------

  void releaseCursor(void) override {
    XCB_Trace("xcb: releaseCursor\n");
    xcb_ungrab_pointer(MConnection,XCB_CURRENT_TIME);
    xcb_flush(MConnection);
  }

  //----------

  void beginPaint(void) override {
    XCB_Trace("xcb: beginPaint\n");
  }

  //----------

  void paint(KODE_Rect ARect) override {
    XCB_Trace("xcb: paint\n");
//    MWindowPainter->resetClip();
    on_paint(MWindowPainter,ARect);
  }

  //----------

  void endPaint(void) override {
    XCB_Trace("xcb: endPaint\n");
    xcb_flush(MConnection);
    //#ifdef KODE_GUI_CAIRO
    //cairo_surface_flush(MCairoSurface);
    //#endif
  }

  //----------

  void blit(int32_t dst_x, int32_t dst_y, KODE_Drawable* ADrawable, int32_t src_x, int32_t src_y, int32_t src_w, int32_t src_h) override {
    XCB_Trace("xcb: blit dst_x %i dst_y %i scr_x %i src_y %i src_w %i src_h %i\n",dst_x,dst_y,src_x,src_y,src_w,src_h);
    if (ADrawable) {
      if (ADrawable->isImage()) {
        //KODE_Trace("isImage\n");
        /*
        xcb_put_image(
          MConnection,                        // connection
          XCB_IMAGE_FORMAT_Z_PIXMAP,          // format
          MWindow,//ADrawable->getDrawable(), // drawable
          MScreenGC,                          // gc
          src_w,                              // width
          src_h,                              // height
          dst_x,                              // dst x
          dst_y,                              // dst y
          0,                                  // left_pat
          ADrawable->getDepth(), //32,        // depth
          ADrawable->getStride() * src_h,     // data_len
          (uint8_t *)ADrawable->getBuffer()   // data
        );
        */
        //KODE_Trace("xcb_image_put: dst_x %i dst_y %i scr_x %i src_y %i src_w %i src_h %i\n",dst_x,dst_y,src_x,src_y,src_w,src_h);
        xcb_image_put (
          MConnection,            // xcb_connection_t *  conn,
          MWindow,                // xcb_drawable_t      draw,
          MScreenGC,              // xcb_gcontext_t      gc,
          ADrawable->getImage(),  // xcb_image_t *       image,
          dst_x,                  // int16_t             x,
          dst_y,                  // int16_t             y,
          0                       // uint8_t             left_pad
        );
        xcb_flush(MConnection);
        //#ifdef KODE_CAIRO
        //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
        //#endif

      }
      else if (ADrawable->isSurface()) {

        //#ifdef KODE_CAIRO
        //cairo_surface_flush(MCairoSurface);
        //#endif

        //KODE_Trace("isSurface\n");
        //KODE_Trace("xcb_copy_area: dst_x %i dst_y %i scr_x %i src_y %i src_w %i src_h %i\n",dst_x,dst_y,src_x,src_y,src_w,src_h);
        xcb_copy_area(
          MConnection,              // Pointer to the xcb_connection_t structure
          ADrawable->getDrawable(), // The Drawable we want to paste
          MWindow,                  // The Drawable on which we copy the previous Drawable
          MScreenGC,                // A Graphic Context
          src_x,                    // Top left x coordinate of the region we want to copy
          src_y,                    // Top left y coordinate of the region we want to copy
          dst_x,                    // Top left x coordinate of the region where we want to copy
          dst_y,                    // Top left y coordinate of the region where we want to copy
          src_w,                    // Width                 of the region we want to copy
          src_h                     // Height of the region we want to copy
        );
        xcb_flush(MConnection);
        //#ifdef KODE_CAIRO
        //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
        //#endif
      }

      else {
        KODE_Trace("unknown ADrawable for blit()\n");
      }

    }
  }

  void blit(int32_t dst_x, int32_t dst_y, KODE_Drawable* ADrawable, KODE_Rect src) override {
    blit(dst_x,dst_y,ADrawable,src.x,src.y,src.w,src.h);
  }

  //----------

  void invalidate(KODE_Rect ARect) override {
    XCB_Trace("xcb: invalidate ARect %.0f %.0f %.0f %.0f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    KODE_Memset(MExposeEventBuffer,0,sizeof(MExposeEventBuffer));
    MExposeEvent->window        = MWindow;
    MExposeEvent->response_type = XCB_EXPOSE;
    MExposeEvent->x             = ARect.x;
    MExposeEvent->y             = ARect.y;
    MExposeEvent->width         = ARect.w;
    MExposeEvent->height        = ARect.h;
    xcb_send_event(
      MConnection,
      false,
      MWindow,
      XCB_EVENT_MASK_EXPOSURE,
      (char*)MExposeEvent
    );
    xcb_flush(MConnection);
  }

  //----------
  /*
    https://www.x.org/releases/X11R7.7/doc/man/man3/xcb_send_event.3.xhtml#heading8
    https://stackoverflow.com/questions/40533318/xcb-custom-message-to-event-loop
  */

  void sendEvent(uint32_t AData, uint32_t AType) override {
    XCB_Trace("xcb: sendEvent AData %i AType %i\n",AData,AType);
    //KODE_TRACE;
    KODE_Memset(MClientMessageEventBuffer,0,sizeof(MClientMessageEventBuffer));
    MClientMessageEvent->window         = MWindow;
    MClientMessageEvent->response_type  = XCB_CLIENT_MESSAGE;
    MClientMessageEvent->format         = 32; // didn't work without this
    MClientMessageEvent->type           = AType;
    MClientMessageEvent->data.data32[0] = AData;
    xcb_send_event(
      MConnection,
      false,
      MWindow,
      XCB_EVENT_MASK_NO_EVENT,
      (char*)MClientMessageEvent
    );
    xcb_flush(MConnection);
  }

  //----------

  void eventLoop(void) override {
    XCB_Trace("xcb: eventLoop\n");
    MQuitEventLoop = false;
    xcb_flush(MConnection);

    xcb_generic_event_t* event;// = xcb_wait_for_event(MConnection);
    //while (event) {
    while ((event = xcb_wait_for_event(MConnection))) {
      uint32_t e = event->response_type & ~0x80;

      if (e == XCB_CLIENT_MESSAGE) {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
        xcb_atom_t  type = client_message->type;
        uint32_t      data = client_message->data.data32[0];
        if (type == MWMProtocolsAtom) {
          if (data == MWMDeleteWindowAtom) {
            free(event); // not KODE_Malloc'ed
            //MQuitEventLoop = true;
            break;
          }
        }
      }
      eventHandler(event);
      free(event); // not KODE_Malloc'ed
      if (MQuitEventLoop) break;
      //event = xcb_wait_for_event(MConnection);

    }
  }

  //----------

  /*
  void renderLoop(float AFPS) override {
    //unsigned long lastRepaint = 0;
    //XEvent event;
    while (true) {
      //if (XPending(xinfo.display) > 0) {
      //  XNextEvent( xinfo.display, &event );
      //  switch( event.type ) {
      //    case MotionNotify:
      //      handleMotion(xinfo, event);
      //      break;
      //    ...
      //  }
      //}
      unsigned long end = now();
      if ((end - lastRepaint) > (1000000 / AFPS)) {
        //handleAnimation(xinfo);
        //repaint(xinfo);
        lastRepaint = now();
      }
      //if (XPending(xinfo.display) == 0) {
        usleep(1000000 / AFPS - (end - lastRepaint));
      //}
    }
  }
  */

//------------------------------
private:
//------------------------------

  void eventHandler(xcb_generic_event_t* AEvent) {

    switch (AEvent->response_type & ~0x80) {

      //----------

      case XCB_MAP_NOTIFY: {
        XCB_Trace("xcb: XCB_MAP_NOTIFY\n");
        //xcb_map_notify_event_t* map_notify = (xcb_map_notify_event_t*)AEvent;
        MWindowMapped = true;
        //on_window_open();
        break;
      }

      //----------

      case XCB_UNMAP_NOTIFY: {
        XCB_Trace("xcb: XCB_UNMAP_NOTIFY\n");
        //xcb_unmap_notify_event_t* unmap_notify = (xcb_unmap_notify_event_t*)AEvent;
        MWindowMapped = false;
        //on_window_close();
        break;
      }

      //----------

      case XCB_CONFIGURE_NOTIFY: {
        xcb_configure_notify_event_t* configure_notify = (xcb_configure_notify_event_t*)AEvent;
        int32_t x = configure_notify->x;
        int32_t y = configure_notify->y;
        int32_t w = configure_notify->width;
        int32_t h = configure_notify->height;
        XCB_Trace("xcb: XCB_CONFIGURE_NOTIFY x %i y %i w %i h %i\n",x,y,w,h);
        if ((x != MWindowXpos) || (y != MWindowYpos)) {
          MWindowXpos = x;
          MWindowYpos = y;
          on_move(x,y);
        }
        if ((w != MWindowWidth) || (h != MWindowHeight)) {
          MWindowWidth = w;
          MWindowHeight = h;
          on_resize(w,h);

          //#ifdef KODE_CAIRO
          //cairo_xcb_surface_set_size(MCairoSurface,w,h);
          //#endif

        }
        break;
      }

      //----------


      case XCB_EXPOSE: {
        MWindowExposed = true;
        KODE_Rect RECT;
        xcb_expose_event_t* expose = (xcb_expose_event_t *)AEvent;
        int32_t x = expose->x;
        int32_t y = expose->y;
        int32_t w = expose->width;
        int32_t h = expose->height;
        RECT = KODE_Rect(x,y,w,h);

        // https://cairographics.org/cookbook/xcbsurface.c/
        // Avoid extra redraws by checking if this is the last expose event in the sequence
        //if (expose->count != 0) break;

        //while(expose->count != 0) {
        //  xcb_generic_event_t* e2 = xcb_wait_for_event(MConnection);
        //  xcb_expose_event_t* ex2 = (xcb_expose_event_t *)e2;
        //  RECT.combine( KODE_Rect( ex2->x, ex2->y, ex2->width, ex2->height ) );
        //}

        XCB_Trace("xcb: XCB_EXPOSE x %i y %i w %i h %i\n",x,y,w,h);
        
        //KODE_Trace("xcb: XCB_EXPOSE x %i y %i w %i h %i\n",x,y,w,h);
        if ((w < 1) || (h < 1)) {
          //KODE_Trace("w || h < 1\n");
        }
        else {
          beginPaint();
          paint(RECT);
          //on_paint(nullptr,RECT);
          endPaint();
        }
        break;
      }

      //----------

      //s = AEvent->xkey.state;
      //XLookupString(&AEvent->xkey, c, 1, &keysym, NULL);
      //k = remapKey(keysym);

      /*
      // wrapper to get xcb keysymbol from keycode
      static xcb_keysym_t xcb_get_keysym(xcb_keycode_t keycode) {
        xcb_key_symbols_t *keysyms;
        if (!(keysyms = xcb_key_symbols_alloc(conn))) return 0;
        xcb_keysym_t keysym = xcb_key_symbols_get_keysym(keysyms, keycode, 0);
        xcb_key_symbols_free(keysyms);
        return keysym;
      }
      */

      case XCB_KEY_PRESS: {
        xcb_key_press_event_t* key_press = (xcb_key_press_event_t*)AEvent;
        uint32_t  k = remapKey(   key_press->detail, key_press->state );
        uint32_t  s = remapState( key_press->state );
        uint32_t ts = key_press->time;
        //int32_t   x = key_press->event_x;
        //int32_t   y = key_press->event_y;
        XCB_Trace("xcb: XCB_KEY_PRESS k %i s %i (ts %i)\n",k,s,ts);
        on_keyPress(0,k,s);
        break;
      }

      //----------

      case XCB_KEY_RELEASE: {
        xcb_key_release_event_t* key_release = (xcb_key_release_event_t*)AEvent;
        uint32_t  k = remapKey( key_release->detail, key_release->state );
        uint32_t  s = remapState( key_release->state );
        uint32_t ts = key_release->time;
        //int32_t   x = key_release->event_x;
        //int32_t   y = key_release->event_y;
        XCB_Trace("xcb: XCB_KEY_RELEASE k %i s %i (ts %i)\n",k,s,ts);
        on_keyRelease(0,k,s);
        break;
      }

      //----------

      case XCB_BUTTON_PRESS: {
        xcb_button_press_event_t* button_press = (xcb_button_press_event_t*)AEvent;
        uint32_t  b = remapButton( button_press->detail, button_press->state );
        uint32_t  s = remapState( button_press->state );
        int32_t   x = button_press->event_x;
        int32_t   y = button_press->event_y;
        uint32_t ts = button_press->time;
        XCB_Trace("xcb: XCB_BUTTON_PRESS x %i y %i b %i s %i (ts %i)\n",x,y,b,s,ts);
        on_mouseClick(x,y,b,s);
        break;
      }

      //----------

      case XCB_BUTTON_RELEASE: {
        xcb_button_release_event_t* button_release = (xcb_button_release_event_t*)AEvent;
        uint32_t  b = remapButton( button_release->detail, button_release->state );
        uint32_t  s = remapState( button_release->state );
        int32_t   x = button_release->event_x;
        int32_t   y = button_release->event_y;
        uint32_t ts = button_release->time;
        XCB_Trace("xcb: XCB_BUTTON_RELEASE x %i y %i b %i s %i (ts %i)\n",x,y,b,s,ts);
        on_mouseRelease(x,y,b,s);
        break;
      }

      //----------

      case XCB_MOTION_NOTIFY: {
        xcb_motion_notify_event_t* motion_notify = (xcb_motion_notify_event_t*)AEvent;
        //uint32_t  b = motion_notify->detail;
        uint32_t  s = remapState( motion_notify->state );
        int32_t   x = motion_notify->event_x;
        int32_t   y = motion_notify->event_y;
        uint32_t ts = motion_notify->time;
        XCB_Trace("xcb: XCB_MOTION_NOTIFY x %i y %i s %i (ts %i)\n",x,y,s,ts);
        on_mouseMove(x,y,s);
        break;
      }

      //----------

      case XCB_ENTER_NOTIFY: {
        //#ifdef KODE_DEBUG_XCB
        xcb_enter_notify_event_t* enter_notify = (xcb_enter_notify_event_t*)AEvent;
        //uint32_t  t = enter_notify->time;
        uint32_t  m = enter_notify->mode;
        uint32_t  s = enter_notify->state;
        int32_t   x = enter_notify->event_x;
        int32_t   y = enter_notify->event_y;
        uint32_t ts = enter_notify->time;
        XCB_Trace("xcb: XCB_ENTER_NOTIFY x %i y %i m %i s %i (ts %i)\n",x,y,m,s,ts);
        //#endif
        on_enter(x,y,nullptr);
        break;
      }

      case XCB_LEAVE_NOTIFY: {
        //#ifdef KODE_DEBUG_XCB
        xcb_leave_notify_event_t* leave_notify = (xcb_leave_notify_event_t*)AEvent;
        //uint32_t  t = leave_notify->time;
        uint32_t  m = leave_notify->mode;
        uint32_t  s = leave_notify->state;
        int32_t   x = leave_notify->event_x;
        int32_t   y = leave_notify->event_y;
        uint32_t ts = leave_notify->time;
        XCB_Trace("xcb: XCB_LEAVE_NOTIFY x %i y %i m %i s %i (ts %i)\n",x,y,m,s,ts);
        //#endif
        on_leave(x,y,nullptr);
        break;
      }

      //----------

      case XCB_CLIENT_MESSAGE: {
        xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)AEvent;
        xcb_atom_t type = client_message->type;
        uint32_t data = client_message->data.data32[0];
        XCB_Trace("xcb: XCB_CLIENT_MESSAGE type %i data %i\n",type,data);
        switch(data) {
          case KODE_THREAD_TIMER:
            //KTrace("ClientMessage: data = kts_timer\n");
            on_timer();
            break;
          case KODE_THREAD_IDLE:
            //KTrace("ClientMessage: data = kts_idle\n");
            if (MWindowMapped && MWindowExposed) {
              //KODE_TRACE;
              on_idle();
            }
            break;
          default:
            on_clientMessage(/*type,*/data);
            break;
        }
        break;
      }

      //----------

      //case XCB_PROPERTY_NOTIFY: {
      //  xcb_property_notify_event_t* property_notify = (xcb_property_notify_event_t*)AEvent;
      //  xcb_atom_t      a = property_notify->atom;
      ////xcb_timestamp_t t = property_notify->time;
      ////uint32_t          s = property_notify->state;
      //  char  buffer1[256];
      //  char* name = buffer1;
      //  bool  ok1  = getAtomName(a,&name);
      //  char buffer2[256];
      //  char* property = buffer2;
      //  xcb_atom_t type = XCB_ATOM;
      //  switch (a) {
      //    case XCB_ATOM_WM_NAME:
      //      type = XCB_ATOM_STRING;
      //      break;
      //  }
      //  bool ok2 = getAtomProperty(a,type,&property);
      //  XCB_Trace("XCB_PROPERTY_NOTIFY (%i) %s %s\n",a,ok1 ? name : "---", ok2 ? property : "---" );
      //  break;
      //}

      //----------

      //case XCB_FOCUS_IN: {
      //  xcb_focus_in_event_t* focus_in = (xcb_focus_in_event_t*)AEvent;
      //  XCB_Trace("XCB_FOCUS_IN\n");
      //  //on_window_focus(true);
      //  break;
      //}

      //case XCB_FOCUS_OUT: {
      //  xcb_focus_out_event_t* focus_out = (xcb_focus_out_event_t*)AEvent;
      //  XCB_Trace("XCB_FOCUS_OUT\n");
      //  //on_window_focus(false);
      //  break;
      //}

      //case XCB_KEYMAP_NOTIFY: {
      //  xcb_keymap_notify_event_t* keymap_notify = (xcb_keymap_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_KEYMAP_NOTIFY\n");
      //  break;
      //}

      //case XCB_GRAPHICS_EXPOSURE: {
      //  xcb_graphics_exposure_event_t *graphics_exposure = (xcb_graphics_exposure_event_t *)AEvent;
      //  XCB_Trace("XCB_GRAPHICS_EXPOSURE\n");
      //  break;
      //}

      //case XCB_NO_EXPOSURE: {
      //  xcb_no_exposure_event_t *no_exposure = (xcb_no_exposure_event_t *)AEvent;
      //  XCB_Trace("XCB_NO_EXPOSURE\n");
      //  break;
      //}

      //case XCB_VISIBILITY_NOTIFY: {
      //  xcb_visibility_notify_event_t *visibility_notify = (xcb_visibility_notify_event_t *)AEvent;
      //  XCB_Trace("XCB_VISIBILITY_NOTIFY\n");
      //  break;
      //}

      //case XCB_CREATE_NOTIFY: {
      //  xcb_create_notify_event_t* create_notify = (xcb_create_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_CREATE_NOTIFY\n");
      //  break;
      //}

      //case XCB_DESTROY_NOTIFY: {
      //  xcb_destroy_notify_event_t* destroy_notify = (xcb_destroy_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_DESTROY_NOTIFY\n");
      //  break;
      //}

      //case XCB_MAP_REQUEST: {
      //  xcb_map_request_event_t* map_request = (xcb_map_request_event_t*)AEvent;
      //  XCB_Trace("XCB_MAP_REQUEST\n");
      //  break;
      //}

      //case XCB_REPARENT_NOTIFY: {
      //  //xcb_reparent_notify_event_t* reparent_notify = (xcb_reparent_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_REPARENT_NOTIFY\n");
      //  break;
      //}

      //case XCB_CONFIGURE_REQUEST: {
      //  xcb_configure_request_event_t* configure_request = (xcb_configure_request_event_t*)AEvent;
      //  XCB_Trace("XCB_CONFIGURE_REQUEST\n");
      //  break;
      //}

      //case XCB_GRAVITY_NOTIFY: {
      //  xcb_gravity_notify_event_t* gravity_notify = (xcb_gravity_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_GRAVITY_NOTIFY\n");
      //  break;
      //}

      //case XCB_RESIZE_REQUEST: {
      //  xcb_resize_request_event_t* resize_request = (xcb_resize_request_event_t*)AEvent;
      //  XCB_Trace("XCB_RESIZE_REQUEST\n");
      //  break;
      //}

      //case XCB_CIRCULATE_NOTIFY: {
      //  xcb_circulate_notify_event_t* circulate_notify = (xcb_circulate_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_CIRCULATE_NOTIFY\n");
      //  break;
      //}

      //case XCB_CIRCULATE_REQUEST: {
      //  xcb_circulate_request_event_t* circulate_request = (xcb_circulate_request_event_t*)AEvent;
      //  XCB_Trace("XCB_CIRCULATE_REQUEST\n");
      //  break;
      //}

      //case XCB_SELECTION_CLEAR: {
      //  xcb_selection_clear_event_t* selection_clear = (xcb_selection_clear_event_t*)AEvent;
      //  XCB_Trace("XCB_SELECTION_CLEAR\n");
      //  break;
      //}

      //case XCB_SELECTION_REQUEST: {
      //  xcb_selection_request_event_t* selection_request = (xcb_selection_request_event_t*)AEvent;
      //  XCB_Trace("XCB_SELECTION_REQUEST\n");
      //  break;
      //}

      //case XCB_SELECTION_NOTIFY: {
      //  xcb_selection_notify_event_t* selection_notify = (xcb_selection_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_SELECTION_NOTIFY\n");
      //  break;
      //}

      //case XCB_COLORMAP_NOTIFY: {
      //  xcb_colormap_notify_event_t* colormap_notify = (xcb_colormap_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_COLORMAP_NOTIFY\n");
      //  break;
      //}

      //case XCB_MAPPING_NOTIFY: {
      //  xcb_mapping_notify_event_t* mapping_notify = (xcb_mapping_notify_event_t*)AEvent;
      //  XCB_Trace("XCB_MAPPING_NOTIFY\n");
      //  break;
      //}

      //default: {
      //  XCB_Trace("unhandled xcb event: %i\n",event->response_type);
      //  break;
      //}

    } // switch event type

  }

};

//----------------------------------------------------------------------
// event thread
//----------------------------------------------------------------------

void* xcb_event_thread_proc(void* AWindow) {
  XCB_Trace("xcb: kode_xcb_event_thread_proc AWindow %p\n",AWindow);
  KODE_XcbWindow* window = (KODE_XcbWindow*)AWindow;
  if (window) {
    xcb_connection_t* connection = window->MConnection;
    xcb_flush(connection);
    while (window->MEventThreadActive) {
      xcb_generic_event_t* event = xcb_wait_for_event(connection);
      if (event) {
        //uint32_t e = event->response_type & ~0x80;
        //if (e == XCB_CLIENT_MESSAGE) {
        //  xcb_client_message_event_t* client_message = (xcb_client_message_event_t*)event;
        //  xcb_atom_t  type = client_message->type;
        //  uint32_t      data = client_message->data.data32[0];
        //  KTrace("XCB_CLIENT_MESSAGE: type %i data %i\n",type,data);
        //}
        window->eventHandler(event);
        free(event); // not MALLOC'ed
      }
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------
// timer thread
//----------------------------------------------------------------------

void* xcb_timer_thread_proc(void* AWindow) {
  XCB_Trace("xcb: kode_xcb_timer_thread_proc AWindow %p\n",AWindow);
  KODE_XcbWindow* window = (KODE_XcbWindow*)AWindow;
  if (window) {
    xcb_connection_t* connection = window->MConnection;
    xcb_flush(connection); // so we're sure all messages have been sent before entering loop
    while (window->MTimerThreadActive) {
      //window->on_window_timer();
      window->sendEvent(KODE_THREAD_TIMER,0);
      KODE_Sleep(window->MTimerSleep); // ???
    }
  }
  return nullptr;
}

//----------------------------------------------------------------------
#endif
