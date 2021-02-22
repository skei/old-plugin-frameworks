#ifndef kode_image_included
#define kode_image_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#ifdef KODE_GUI_CAIRO
  #include "gui/xcb/kode_xcb_image.h"
  typedef KODE_XcbImage KODE_ImplementedImage;
  //#include "gui/cairo/kode_cairo_image.h"
  //typedef KODE_CairoImage KODE_ImplementedImage;
#endif

//----------

#ifdef KODE_GUI_XCB
  #include "gui/xcb/kode_xcb_image.h"
  typedef KODE_XcbImage KODE_ImplementedImage;
#endif

//----------------------------------------------------------------------

typedef KODE_ImplementedImage KODE_Image;

//----------------------------------------------------------------------
#endif
