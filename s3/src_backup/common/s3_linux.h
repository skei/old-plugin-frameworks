#ifndef s3_linux_included
#define s3_linux_included
//----------------------------------------------------------------------

#ifdef S3_LINUX

/*
  freetype:
  libfreetype6-dev
  -I/usr/include/freetype2
*/

//----------------------------------------------------------------------

// -lpthread
#include <pthread.h>

//#ifdef S3_LINUX

#include "s3_xlib.h"
#include "s3_xrender.h"
#include "s3_xft.h"

//  #ifdef S3_XLIB
//    #include <X11/X.h>
//    #include <X11/Xlib.h>
//    #include <X11/Xutil.h>
//    #include <X11/cursorfont.h>
//  #endif // S3_XLIB

//  #ifdef S3_XRENDER
//    #include <X11/extensions/Xrender.h>
//  #endif // S3_XRENDER

//  #ifdef S3_XFT
//    #include <X11/Xft/Xft.h>
//  #endif // S3_XRENDER

  // XInitThreads..

//#endif // S3_LINUX

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include <time.h>

// usleep is deprecated

int S3_Sleep(long ms) {
  struct timespec req, rem;
  if (ms > 999) {
    req.tv_sec = (int)(ms / 1000);                            /* Must be Non-Negative */
    req.tv_nsec = (ms - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
  }
  else {
    req.tv_sec = 0;                         /* Must be Non-Negative */
    req.tv_nsec = ms * 1000000;    /* Must be in range of 0 to 999999999 */
  }
   return nanosleep(&req , &rem);
}



////----------------------------------------------------------------------
//// XInitThreads
////----------------------------------------------------------------------
//
//#ifdef S3_XLIB
//
///*
//  this is a bit hack-ish, isn't it?
//  maybe we should use a S3_Global?
//  note that this is called both when enumerating,
//  and when initializing the plugin
//*/
//
////static
//bool s3_need_xinitthreads_call = true;
//
////----------
//
////static __S3_UNUSED
//bool s3_init_xlib(void) {
//  //static bool need_xinitthreads_call = true;
//  if (s3_need_xinitthreads_call) {
//    S3_Trace("calling XInitThreads\n");
//    XInitThreads();
//    s3_need_xinitthreads_call = false;
//  }
//  return true;
//}
//
////----------
//
////static
//bool s3_xlib_initialized = s3_init_xlib();
//
//#endif // S3_XLIB
//
////----------------------------------------------------------------------
//// xlib / pixmap
////----------------------------------------------------------------------
//
//#ifdef S3_XLIB
//
///*
//  The server uses the specified drawable to determine on which screen
//  to create the pixmap. The pixmap can be used only on this screen and
//  only with other drawables of the same depth (see XCopyPlane for an
//  exception to this rule).
//*/
//
//Pixmap s3_xlib_create_pixmap(Display* ADisplay, Drawable ADrawable, int32 AWidth, int32 AHeight, int32 ADepth) {
//  return XCreatePixmap(ADisplay,ADrawable,AWidth,AHeight,ADepth);
//}
//
////----------
//
///*
//  The XFreePixmap() function first deletes the association between the pixmap ID and the pixmap.
//  Then, the X server frees the pixmap storage when there are no references to it.
//*/
//
//void s3_xlib_delete_pixmap(Display* ADisplay, Pixmap APixmap) {
//  XFreePixmap(ADisplay,APixmap);
//}
//
//#endif // S3_XLIB

//----------------------------------------------------------------------
// xrender / picture
//----------------------------------------------------------------------

//#ifdef S3_XRENDER
//
//static
//XRenderPictFormat s3_xrender_argb_templ = {
//  0,              // id
//  PictTypeDirect, // type
//  32,             // depth
//  {               // direct
//    16,           // direct.red
//    0xff,         // direct.redMask
//    8,            // direct.green
//    0xff,         // direct.greenMask
//    0,            // direct.blue
//    0xff,         // direct.blueMask
//    24,           // direct.alpha
//    0xff,         // direct.alphaMask
//  },
//  0,              // colormap
//};
//
////----------
//
//static
//uint32 s3_xrender_argb_templ_mask =
//  PictFormatType      |
//  PictFormatDepth     |
//  PictFormatRed       |
//  PictFormatRedMask   |
//  PictFormatGreen     |
//  PictFormatGreenMask |
//  PictFormatBlue      |
//  PictFormatBlueMask  |
//  PictFormatAlpha     |
//  PictFormatAlphaMask;
//
////----------
//
///*
//  http://www.winehq.org/pipermail/wine-patches/2005-August/020119.html
//  Avoid using XRenderFindStandardFormat as older libraries don't have it
//*/
//
//XRenderPictFormat* s3_xrender_find_format(Display* ADisplay, int32 ADepth) {
//  //case ADepth of
//  //  24: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB24);
//  //  32: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB32);
//  //end;
//  switch(ADepth) {
//    case 24: s3_xrender_argb_templ.depth = 24; break;
//    case 32: s3_xrender_argb_templ.depth = 32; break;
//  }
//  return XRenderFindFormat(ADisplay, s3_xrender_argb_templ_mask, &s3_xrender_argb_templ, 0);
//}
//
////----------
//
////void create_picture(Drawable ADrawable) {
//Picture s3_xrender_create_picture(Display* ADisplay, Drawable ADrawable, int32 ADepth) {
//  XRenderPictFormat*        fmt;
//  XRenderPictureAttributes  pict_attr;
//  int32                     pict_bits;
//  /*
//  if FDepth = 24 then  fmt := find_format(FDisplay,24{PictStandardRGB24})
//  else fmt := find_format(FDisplay,32{PictStandardARGB32});
//  */
//  if (ADepth == 24) fmt = XRenderFindStandardFormat(ADisplay,PictStandardRGB24);
//  else fmt = XRenderFindStandardFormat(ADisplay,PictStandardARGB32);
//  pict_attr.poly_edge = PolyEdgeSmooth;
//  pict_attr.poly_mode = PolyModeImprecise;
//  /*
//  pict_attr.component_alpha = true;
//  */
//  pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
//  return  XRenderCreatePicture(
//    ADisplay,
//    ADrawable,//MPixmap,
//    fmt,
//    pict_bits,
//    &pict_attr
//  );
//}
//
////----------
//
//void s3_xrender_delete_picture(Display* ADisplay, Picture APicture) {
//  XRenderFreePicture(ADisplay,APicture);
//}
//
//#endif // S3_XRENDER

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#endif // S3_LINUX

//----------------------------------------------------------------------
#endif



/*

  XInitThreads()

  http://www.x.org/archive/X11R6.8.1/doc/XInitThreads.3.html

  The XInitThreads function initializes Xlib support for concurrent threads.
  This function must be the first Xlib function a multi-threaded program calls,
  and it must complete before any other Xlib call is made.
  This function returns a nonzero status if initialization was successful;
  otherwise, it returns zero. On systems that do not support threads, this
  function always returns zero.
  It is only necessary to call this function if multiple threads might use Xlib
  concurrently. If all calls to Xlib functions are protected by some other
  access mechanism (for example, a mutual exclusion lock in a toolkit or
  through explicit client programming), Xlib thread initialization is not
  required. It is recommended that single-threaded programs not call this
  function.
  The XLockDisplay function locks out all other threads from using the
  specified display. Other threads attempting to use the display will block
  until the display is unlocked by this thread. Nested calls to XLockDisplay
  work correctly; the display will not actually be unlocked until
  XUnlockDisplay has been called the same number of times as XLockDisplay. This
  function has no effect unless Xlib was successfully initialized for threads
  using XInitThreads.
  The XUnlockDisplay function allows other threads to use the specified display
  again. Any threads that have blocked on the display are allowed to continue.
  Nested locking works correctly; if XLockDisplay has been called multiple
  times by a thread, then XUnlockDisplay must be called an equal number of
  times before the display is actually unlocked. This function has no effect
  unless Xlib was successfully initialized for threads using XInitThreads.

  http://www.remlab.net/op/xlib.shtml

  XInitThreads() initializes thread callbacks inside Xlib, and protects the
  global data within Xlib, such as internationalization stuff. Much like most
  libraries with a similar function, the function can be called multiple times,
  but it is not re-entrant: the first caller in the lifetime of the process
  must be the only concurrent caller. Effectively, the beginning of the main()
  function is the only really safe place to call XInitThreads().
  However, most real-life programs access Xlib through higher-level libraries,
  and the libraries do not initialize Xlib threading on their behalf. Today,
  most programs with multiple X11 connections and multiple threads are buggy.
  Consequences
  If XInitThreads() is not called anywhere in a multi-tasked program, the
  program will usually work. But weird crashes and other race conditions are
  bound to happen over time, as Xlib maintains some global data that is shared
  across all threads and all "displays".
  If XInitThreads() is called too late, then the process will crash, but only
  some time later in obscure ways. In my experience, the most common case is
  for the process to hit a segmentation fault within XCloseDisplay(), which
  usually happens at exit.

  https://mailman.videolan.org/pipermail/vlc-commits/2010-December/004628.html

  XInitThreads() can be called multiple times, but it is not reentrant,
  so we need this [a] global lock.

  http://forum.jogamp.org/SIGSEGV-when-closing-JOGL-applications-td895912.html

  We use 'XInitThreads()' to setup X11 multithreading support and it seems that
  the [J]AWT implementation in the JVM calls X11 functions before and after
  JOGL runs.
  The lack of calling 'XInitThreads()' in the JVM/JRE (when using AWT) is
  already identified as a huge pain and a workaround for this does not seem to
  be a good idea at all(*).
  The best solution would be to convince Oracle/OpenJDK to issue the
  XInitThread() call ASAP (before any other X11 call), and to revisit some
  _global_ AWT locks, since they are probably no more necessary. This would
  allow true inter-component multithreading with X11 without the need of a
  global AWT toolkit lock and hence a fluent X11 workflow where threads won't
  disturb each other anymore.
  BTW .. without XInitThreads() and with XLockDisplay
  it was not possible with the NV driver to manage lifecycle operations
  (create, makecurrent/release, destroy) in a reliable manner across threads.
  Eg:
    thread1: createContext, makeCurrent, some-rendering, release ..
    thread1: stops
    thread2: makeCurrent/release, destroyContext
             -> hangs somewhere, ie makeCurrent, swapBuffer or destroy
  So .. we have to think about this, and triage this in more detail.
  Multithreading in JOGL is not about using multiple rendering threads here,
  but to allow multiple threads to use the same X11 Display connection for
  rendering and event dispatching undisturbed, read: non blocking.

  http://www.kvraudio.com/forum/viewtopic.php?p=6114498#p6114498

  MusE does not call XInitThreads at the start of main. This leads to a crash
  as soon as you try to open Hive's native GUI.

  https://lists.freedesktop.org/archives/gstreamer-bugs/2014-June/126962.html

  XInitThreads can be called multiple times, only the first call has any effect.
  Calling it multiple times will still report success with every call.
*/
