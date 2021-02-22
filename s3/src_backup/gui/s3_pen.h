#ifndef s3_pen_included
#define s3_pen_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #include "gui/linux/s3_pen_xlib.h"
    typedef S3_Pen_Xlib S3_Pen_Implementation;
#endif

//----------------------------------------------------------------------

#ifdef S3_WIN32
  #ifdef S3_GUI_GDI
    #include "gui/win32/s3_pen_gdi.h"
    typedef S3_Pen_Gdi S3_Pen_Implementation;
  #endif
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef S3_Pen_Implementation S3_Pen;

//----------------------------------------------------------------------
#endif
