#ifndef s3_window_base_included
#define s3_window_base_included
//----------------------------------------------------------------------

#include "gui/s3_surface.h"
#include "gui/s3_widget.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Window_Base
: public S3_Widget {
  protected:
    //S3_Rect   MRect;
    //bool      MEmbedded;
  public:
    S3_Window_Base(S3_Rect ARect);
    virtual ~S3_Window_Base();
  public:
    virtual void  setPos(int32 AXpos, int32 AYpos) {}
    virtual void  setSize(int32 AWidth, int32 AHeight) {}
    virtual void  setTitle(const char* ATitle) {}
    virtual void  open(void) {}
    virtual void  close(void) {}
    virtual void  startTimer(int32 ms) {}
    virtual void  stopTimer(void) {}
    virtual void  flush(void) {}
    virtual void  sync(void) {}
    virtual void  lock(void) {}
    virtual void  unlock(void) {}
    virtual void  reparent(void* AParent) {}
    virtual void  sendEvent(uint32 AValue) {}
    virtual void  eventLoop(void) {}
    virtual void  renderLoop(void) {}
    virtual void  invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {}
    virtual void  invalidate(S3_Rect ARect) { invalidate(ARect.x,ARect.y,ARect.w,ARect.h);  }
    virtual void  beginPaint(void) {}
    virtual void  endPaint(void) {}
    virtual void  setCursor(int32 ACursor) {}
    virtual void  createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) {}
    virtual void  destroyUserCursors(void) {}
    virtual void  setCursorPos(int32 AXpos, int32 AYpos) {}
    virtual void  showCursor(void) {}
    virtual void  hideCursor(void) {}
    virtual void  grabCursor(void) {}
    virtual void  releaseCursor(void) {}
    virtual void  warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {}
    virtual void  unwarpCursor(void) {}
  //public:
  //  virtual void  redraw(void) { S3_TRACE; }
  //  virtual void  redraw(S3_Rect ARect) { S3_TRACE; }
  //  virtual void  redraw(S3_Widget* AWidget) { S3_TRACE; }
  public:
    virtual void  on_buffer_resize(uint32 AWidth, uint32 AHeight) {}
    virtual void  on_buffer_paint(S3_Rect ARect, uint32 AMode) {}
    virtual void  on_buffer_blit(S3_Rect ARect) {}
  public:
    virtual void  on_window_resize(uint32 AWidth, uint32 AHeight) {}
    virtual void  on_window_paint(S3_Rect ARect, uint32 AMode) {}
  public:
    virtual void  on_window_timer(void) {}
    virtual void  on_window_idle(void) {}


};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Window_Base::S3_Window_Base(S3_Rect ARect)
: S3_Widget(ARect) {
  //MRect     = ARect;
  //MEmbedded = false;
}

//----------

S3_Window_Base::~S3_Window_Base(void) {
}

//----------------------------------------------------------------------
#endif
