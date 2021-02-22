#ifndef kode_cairo_window_included
#define kode_cairo_window_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/cairo/kode_cairo.h"
#include "gui/cairo/kode_cairo_painter.h"
#include "gui/xcb/kode_xcb_window.h"

//----------------------------------------------------------------------

class KODE_CairoWindow
: public KODE_XcbWindow {

//------------------------------
private:
//------------------------------

  KODE_CairoPainter*  MCairoPainter = nullptr;

//------------------------------
public:
//------------------------------

  KODE_CairoWindow(uint32_t AWidth, uint32_t AHeight, void* AParent=nullptr)
  : KODE_XcbWindow(AWidth,AHeight,AParent) {
    MCairoPainter = KODE_New KODE_CairoPainter(this);
  }

  //----------

  virtual ~KODE_CairoWindow() {
    KODE_Delete MCairoPainter;
  }

//------------------------------
public:
//------------------------------

  void paint(KODE_Rect ARect) override {
    on_paint(MCairoPainter,ARect);
    //cairo_surface_flush(MCairoSurface->getSurface());
    //MCairoPainter->flush();
    
  }

  //----------

  void setSize(int32_t AWidth, int32_t AHeight) override {
    //KODE_Trace("%i %i\n",AWidth,AHeight);
    //KODE_XcbWindow::setSize(AWidth,AHeight);
    MCairoPainter->resize(AWidth,AHeight);
    KODE_XcbWindow::setSize(AWidth, AHeight);
  }

  //----------

  // If drawable is a Window, then the function cairo_xcb_surface_set_size()
  // must be called whenever the size of the window changes.


  void on_resize(float AWidth, float AHeight) override {
    //KODE_Trace("%i %i\n",AWidth,AHeight);
    //resizeBuffer(AWidth,AHeight);
    KODE_XcbWindow::on_resize(AWidth,AHeight);
    MCairoPainter->resize(AWidth,AHeight);
    //MWindowPainter->flush();
    //MCairoPainter->flush();
  }

};

//----------------------------------------------------------------------
#endif
