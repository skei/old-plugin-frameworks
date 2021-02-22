#ifndef kode_painter_included
#define kode_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#ifdef KODE_GUI_CAIRO
  #include "gui/cairo/kode_cairo_painter.h"
  typedef KODE_CairoPainter KODE_ImplementedPainter;
#endif

//----------

#ifdef KODE_GUI_XCB
  #include "gui/xcb/kode_xcb_painter.h"
  typedef KODE_XcbPainter KODE_ImplementedPainter;
#endif

//----------------------------------------------------------------------

/*
  we want to accept both KODE_XcbPainter and KODE_CairoPainter in functions
  taking a KODE_Painter argument..
  this means we have to create the painter implicit/directly, and not using
  the KODE_Painter type (since this is empty)..
*/

//typedef KODE_ImplementedPainter KODE_Painter;
typedef KODE_BasePainter KODE_Painter;

//----------------------------------------------------------------------
#endif
