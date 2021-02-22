#ifndef kode_base_window_included
#define kode_base_window_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_rect.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

class KODE_BaseWindow
: public KODE_Drawable {
public:
  KODE_BaseWindow(uint32_t AWidth, uint32_t AHeight) {}
  virtual ~KODE_BaseWindow() {}
public:
  virtual void setPos(int32_t AXpos, int32_t AYpos) {}
  virtual void setSize(int32_t AWidth, int32_t AHeight) {}
  virtual void setTitle(const char* ATitle) {}
  virtual void open(void) {}
  virtual void close(void) {}
  virtual void flush(void) {}
  virtual void sync(void) {}
  virtual void reparent(void* AParent) {}
  virtual void startTimer(uint32_t ms) {}
  virtual void stopTimer(void) {}
  virtual void setCursor(uint32_t ACursor) {}
  virtual void setCursorPos(int32_t AXpos, int32_t AYpos) {}
  virtual void hideCursor(void) {}
  virtual void showCursor(void) {}
  virtual void grabCursor(void) {}
  virtual void releaseCursor(void) {}
  virtual void beginPaint(void) {}
  virtual void paint(KODE_Rect ARect) {}
  virtual void endPaint(void) {}
  virtual void blit(int32_t dst_x, int32_t dst_y, KODE_Drawable* ADrawable, int32_t src_x, int32_t src_y, int32_t src_w, int32_t src_h) {}
  virtual void blit(int32_t dst_x, int32_t dst_y, KODE_Drawable* ADrawable, KODE_Rect src) {}
  virtual void invalidate(KODE_Rect ARect) {}
  virtual void sendEvent(uint32_t AData, uint32_t AType) {}
  virtual void eventLoop(void) {}
  //virtual void renderLoop(float ATime, KODE_RenderCallback) {}
  //virtual void renderLoop(KODE_Drawable* ADrawable) {
};

//----------------------------------------------------------------------
#endif

