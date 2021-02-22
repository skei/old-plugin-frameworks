#ifndef kode_buffered_cairo_window_included
#define kode_buffered_cairo_window_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/cairo/kode_cairo.h"
#include "gui/cairo/kode_cairo_window.h"
#include "gui/xcb/kode_xcb_surface.h"

//----------------------------------------------------------------------

class KODE_CairoBufferedWindow
: public KODE_CairoWindow {

private:

  KODE_XcbSurface*    MBufferSurface = nullptr;
  KODE_CairoPainter*  MBufferPainter = nullptr;

public:

  KODE_CairoBufferedWindow(uint32_t AWidth, uint32_t AHeight, void* AParent=nullptr)
  : KODE_CairoWindow(AWidth,AHeight,AParent) {
    createBuffer(AWidth,AHeight);
  }

  //----------

  virtual ~KODE_CairoBufferedWindow() {
    destroyBuffer();
  }

//------------------------------
private:
//------------------------------

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    KODE_Assert(!MBufferSurface);
    KODE_Assert(!MBufferPainter);
    MBufferSurface = KODE_New KODE_XcbSurface(this,AWidth,AHeight);
    MBufferPainter = KODE_New KODE_CairoPainter(MBufferSurface);
    //KODE_Drawable* target = MBufferPainter->getTarget();
    //KODE_Rect cliprect = MBufferPainter->getClipRect();
    //KODE_Trace("x %.0f y %.0f w %.0f h %.0f\n",cliprect.x,cliprect.y,cliprect.w,cliprect.h);
  }

  //----------

  void destroyBuffer() {
    if (MBufferSurface) {
      KODE_Delete MBufferSurface;
      MBufferSurface = nullptr;
    }
    if (MBufferPainter) {
      KODE_Delete MBufferPainter;
      MBufferPainter = nullptr;
    }
  }

  //----------

  /*
    todo:
    - NextPowerOfTwo
    - only grow, never shrink
  */

  void resizeBuffer(uint32_t AWidth, uint32_t AHeight) {
    destroyBuffer();
    createBuffer(AWidth,AHeight);
  }

//------------------------------
public:
//------------------------------

  void paint(KODE_Rect ARect) override {
    on_paint(MBufferPainter,ARect);
    blit(ARect.x,ARect.y,MBufferSurface,ARect);
  }

  //----------

  void setSize(int32_t AWidth, int32_t AHeight) override {
    //KODE_Trace("%i %i\n",AWidth,AHeight);
    //KODE_CairoWindow::setSize(AWidth,AHeight);
    resizeBuffer(AWidth,AHeight);
    KODE_XcbWindow::setSize(AWidth, AHeight);
  }

  //----------

  void on_resize(float AWidth, float AHeight) override {
    //KODE_Trace("%i %i\n",AWidth,AHeight);
    resizeBuffer(AWidth,AHeight);
    //KODE_CairoWindow::on_resize(AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif
