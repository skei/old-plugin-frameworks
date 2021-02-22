#ifndef s3_xlib_included
#define s3_xlib_included
//----------------------------------------------------------------------

//#ifdef S3_XLIB

  #include <X11/X.h>
  #include <X11/Xlib.h>
  #include <X11/Xutil.h>
  #include <X11/cursorfont.h>

  bool s3_init_xlib(void) {
    //S3_Trace("calling XInitThreads\n");
    XInitThreads();
    return true;
  }

  bool s3_xlib_initialized = s3_init_xlib();

//#endif

//----------------------------------------------------------------------
#endif
