#ifndef s3_font_included
#define s3_font_included
//----------------------------------------------------------------------

#include "gui/base/s3_font_base.h"

//----------

// -I/usr/include/freetype2

#ifdef S3_LINUX
  #ifdef S3_GUI_XLIB
    #ifdef S3_XFT
      #include "gui/linux/s3_font_xft.h"
      typedef S3_Font_Xft S3_Font_Implementation;
    #else
      typedef S3_Font_Base S3_Font_Implementation;
    #endif
  #endif
#endif

//----------------------------------------------------------------------

#ifdef S3_WIN32
  #ifdef S3_GUI_GDI
    #include "gui/win32/s3_font_gdi.h"
    typedef S3_Font_Gdi S3_Font_Implementation;
  #endif
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef S3_Font_Implementation S3_Font;

//----------------------------------------------------------------------
#endif
