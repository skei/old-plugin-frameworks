#ifndef s3_brush_included
#define s3_brush_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #ifdef S3_GUI_XLIB
  #include "gui/linux/s3_brush_xlib.h"
    typedef S3_Brush_Xlib S3_Brush_Implementation;
  #endif
#endif

//----------------------------------------------------------------------

#ifdef S3_WIN32
  #ifdef S3_GUI_GDI
    #include "gui/win32/s3_brush_gdi.h"
    typedef S3_Brush_Gdi S3_Brush_Implementation;
  #endif
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef S3_Brush_Implementation S3_Brush;

//----------------------------------------------------------------------
#endif
