#ifndef s3_image_included
#define s3_image_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #ifdef S3_GUI_CAIRO
    #include "gui/s3_image_cairo.h"
    typedef S3_Image_Cairo S3_Image_Implementation;
  #else
    #include "gui/s3_image_xlib.h"
    typedef S3_Image_Xlib S3_Image_Implementation;
  #endif
#endif

//#ifdef S3_WIN32
//  #ifdef S3_GUI_GDI
//    #include "gui/win32/s3_image_gdi.h"
//    typedef S3_Image_Gdi S3_Image_Implementation;
//  #endif
//#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef S3_Image_Implementation S3_Image;

//----------------------------------------------------------------------
#endif
