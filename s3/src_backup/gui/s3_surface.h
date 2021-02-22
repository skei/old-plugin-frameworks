#ifndef s3_surface_included
#define s3_surface_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #ifdef S3_GUI_XLIB
    #include "gui/linux/s3_surface_xlib.h"
    typedef S3_Surface_Xlib S3_Surface_Implementation;
    #endif
#endif

//----------------------------------------------------------------------

#ifdef S3_WIN32
  #ifdef S3_GUI_GDI
    #include "gui/win32/s3_surface_gdi.h"
    typedef S3_Surface_Gdi S3_Surface_Implementation;
  #endif
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef S3_Surface_Implementation S3_Surface;

//----------------------------------------------------------------------
#endif
