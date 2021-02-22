#ifndef kode_xcb_included
#define kode_xcb_included
//----------------------------------------------------------------------

// wait for mapnotify event after opening window
//#define KODE_XCB_WAIT_FOR_MAPNOTIFY

//----------------------------------------------------------------------

#include "base/kode.h"

#include <xcb/xcb.h>
#include <xcb/xcb_cursor.h>
#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_util.h>
#include <xcb/xproto.h>


//----------------------------------------------------------------------

#ifdef KODE_DEBUG_XCB
  #define XCB_Trace   KODE_Trace
  #define XCB_DTrace  KODE_DTrace
  #define XCB_TRACE   KODE_TRACE
#else
  #define XCB_Trace   KODE_NoTrace
  #define XCB_DTrace  KODE_NoTrace
  #define XCB_TRACE   {}
#endif

//----------------------------------------------------------------------
#endif
