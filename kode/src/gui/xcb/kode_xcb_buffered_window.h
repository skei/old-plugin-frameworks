#ifndef kode_xcb_buffered_window_included
#define kode_xcb_buffered_window_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/xcb/kode_xcb_painter.h"
#include "gui/xcb/kode_xcb_surface.h"
#include "gui/xcb/kode_xcb_window.h"

//----------------------------------------------------------------------

class KODE_XcbBufferedWindow
: public KODE_XcbWindow {

//------------------------------
private:
//------------------------------

  KODE_XcbSurface*  MBufferSurface = nullptr;
  KODE_XcbPainter*  MBufferPainter = nullptr;

//------------------------------
public:
//------------------------------

  KODE_XcbBufferedWindow(uint32_t AWidth, uint32_t AHeight, void* AParent=nullptr)
  : KODE_XcbWindow(AWidth,AHeight,AParent) {
    createBuffer(AWidth,AHeight);
  }

  //----------

  virtual ~KODE_XcbBufferedWindow() {
    destroyBuffer();
  }

//------------------------------
private:
//------------------------------

  void createBuffer(uint32_t AWidth, uint32_t AHeight) {
    MBufferSurface = KODE_New KODE_XcbSurface(this,AWidth,AHeight);
    MBufferPainter = KODE_New KODE_XcbPainter(MBufferSurface);
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
    //KODE_XcbWindow::setSize(AWidth,AHeight);
    resizeBuffer(AWidth,AHeight);
  }

  //----------

  void on_resize(float AWidth, float AHeight) override {
    //KODE_Trace("%i %i\n",AWidth,AHeight);
    resizeBuffer(AWidth,AHeight);
  }

};

//----------------------------------------------------------------------
#endif
