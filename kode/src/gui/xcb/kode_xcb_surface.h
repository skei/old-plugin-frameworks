#ifndef kode_xcb_surface_included
#define kode_xcb_surface_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_surface.h"
#include "gui/xcb/kode_xcb.h"

#ifdef KODE_CAIRO
#include "gui/cairo/kode_cairo.h"
#endif

//----------------------------------------------------------------------

class KODE_XcbSurface
: public KODE_BaseSurface {

//------------------------------
private:
//------------------------------

  xcb_connection_t* MConnection   = nullptr;
  xcb_visualid_t    MVisual       = XCB_NONE;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  xcb_pixmap_t      MPixmap       = XCB_NONE;
  int32_t           MWidth        = 0;
  int32_t           MHeight       = 0;
  int32_t           MDepth        = 0;
  bool              MIsWindow     = false;
  xcb_window_t      MWindow       = XCB_NONE;

  #ifdef KODE_CAIRO
  cairo_surface_t*  MCairoSurface = nullptr;
  #endif

//------------------------------
public:
//------------------------------

  KODE_XcbSurface(KODE_Drawable* AOwner)
  : KODE_BaseSurface(AOwner) {
    MConnection = AOwner->getConnection();
    MDrawable   = AOwner->getDrawable();
    MVisual     = AOwner->getVisual();
    MWidth      = AOwner->getWidth();
    MHeight     = AOwner->getHeight();
    MDepth      = AOwner->getDepth();
    MWindow     = AOwner->getWindow();
    MIsWindow   = true;
    xcb_flush(MConnection);

    #ifdef KODE_CAIRO
    MCairoSurface = createCairoSurface();
    //KODE_Trace("%s\n",cairo_status_to_string(cairo_surface_status(MCairoSurface)));
    #endif
  }

  //----------

  KODE_XcbSurface(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : KODE_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
    MConnection = AOwner->getConnection();
    MDrawable   = AOwner->getDrawable();
    MVisual     = AOwner->getVisual();
    MWidth      = AWidth;
    MHeight     = AHeight;
    if (ADepth  == 0) MDepth  = AOwner->getDepth();
    else MDepth  = ADepth;
    //if (AOwner->isWindow()) {
    //  MIsWindow = true;
    //  MWindow = AOwner->getWindow();
    //}
    //else {
      MPixmap = xcb_generate_id(MConnection);
      xcb_create_pixmap(
        MConnection,
        MDepth,
        MPixmap,
        MDrawable,
        AWidth,
        AHeight
      );
    //}
    xcb_flush(MConnection);

    #ifdef KODE_CAIRO
    MCairoSurface = createCairoSurface();
    #endif

  }

  //----------

  virtual ~KODE_XcbSurface() {
    #ifdef KODE_CAIRO
    //KODE_Trace("%s\n",cairo_status_to_string(cairo_surface_status(MCairoSurface)));
    //cairo_surface_finish(MCairoSurface);
    cairo_surface_destroy(MCairoSurface);
    #endif
    if (!MIsWindow) {
      xcb_free_pixmap(MConnection,MPixmap);
    }
  }

//------------------------------
public: // drawable
//------------------------------

  bool              isSurface()           override { return true; }
  xcb_connection_t* getConnection()       override { return MConnection; }
  xcb_visualid_t    getVisual()           override { return MVisual; }
  xcb_drawable_t    getDrawable()         override { return MPixmap; /*MDrawable;*/ }
  xcb_pixmap_t      getPixmap()           override { return MPixmap; }
  
  uint32_t          getWidth()            override { return MWidth; }
  uint32_t          getHeight()           override { return MHeight; }  
  
  #ifdef KODE_CAIRO
  bool              isCairo()             override { return true; }
  cairo_surface_t*  getCairoSurface(void) override { return MCairoSurface; }
  #endif

//------------------------------
//
//------------------------------

  /*
    Creates an XCB surface that draws to the given drawable. The way that
    colors are represented in the drawable is specified by the provided visual.

    Note:
    If drawable is a Window, then the function cairo_xcb_surface_set_size()
    must be called whenever the size of the window changes.

    When drawable is a Window containing child windows then drawing to the
    created surface will be clipped by those child windows. When the created
    surface is used as a source, the contents of the children will be included.

    This function always returns a valid pointer, but it will return a pointer
    to a "nil" surface if an error such as out of memory occurs. You can use
    cairo_surface_status() to check for this.

  */

  #ifdef KODE_CAIRO
  cairo_surface_t* createCairoSurface() {
    cairo_surface_t* surface = cairo_xcb_surface_create(
      MConnection,
      MIsWindow ? MWindow : MPixmap,//MDrawable,//getDrawable(),
      kode_xcb_find_visual(MConnection,MVisual),
      MWidth,
      MHeight
    );
    //check_cairo_surface_errors(surface);
    return surface;
  }
  #endif

};

//----------------------------------------------------------------------
#endif
