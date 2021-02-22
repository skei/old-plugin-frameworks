#ifndef s3_window_base_included
#define s3_window_base_included
//----------------------------------------------------------------------

//#include "common/s3_rect.h"
//#include "gui/s3_window_listener.h"
#include "gui/s3_painter.h"

//class S3_Painter;
//class S3_WindowListener;

//----------------------------------------------------------------------

class S3_Window_Base {
  public:
    S3_Window_Base(/*S3_WindowListener* AListener,*/ S3_Rect ARect, void* AParent=S3_NULL) {}
    virtual ~S3_Window_Base() {}
  public:
    virtual void setPos(int32 AXpos, int32 AYpos) {}
    virtual void setSize(int32 AWidth, int32 AHeight) {}
    virtual void setTitle(const char* ATitle) {}
    virtual void open(void) {}
    virtual void close(void) {}
    virtual void startTimer(int32 ms) {}
    virtual void stopTimer(void) {}
    virtual void reparent(void* AParent) {}
    virtual void eventLoop(void) {}
    virtual void renderLoop(void) {}
    virtual void invalidate(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {}
    virtual void beginPaint(void) {}
    virtual void endPaint(void) {}
    virtual void setCursor(int32 ACursor) {}
    virtual void createUserCursor(uint32 AIndex, int32 AWidth, int32 AHeight, const uint8* AAndMask, const uint8* AXorMask, int32 AXhotspot, int32 AYhotspot) {}
    virtual void destroyUserCursors(void) {}
    virtual void setCursorPos(int32 AXpos, int32 AYpos) {}
    //virtual void getCursorPos(int32* AXpos, int32* AYpos) {}
    virtual void showCursor(void) {}
    virtual void hideCursor(void) {}
    virtual void grabCursor(void) {}
    virtual void releaseCursor(void) {}
    //virtual void resetCursor(void) {}
    virtual void warpCursor(int32 AXpos, int32 AYpos, int32 AWidth, int32 AHeight) {}
    virtual void unwarpCursor(void) {}
    virtual void blit(int32 dst_x, int32 dst_y, S3_Painter* APainter, int32 src_x, int32 src_y, int32 src_w, int32 src_h) {}
  public:
    virtual void on_windowIdle(S3_Window_Base* AWindow) {}
    virtual void on_windowTimer(S3_Window_Base* AWindow) {}
    virtual void on_windowRenderFrame(S3_Window_Base* AWindow) {}
    virtual void on_windowResize(S3_Window_Base* AWindow, int32 AWidth, int32 AHeight) {}
    virtual void on_windowPaint(S3_Window_Base* AWindow, S3_Rect ARect) {}
  //public:
  //  virtual void on_bufferCreate(S3_Window_Base* AWindow, uint32 AWidth, uint32 AHeight) {}
  //  virtual void on_bufferDestroy(S3_Window_Base* AWindow) {}
  //  virtual void on_bufferResize(S3_Window_Base* AWindow, uint32 AWidth, uint32 AHeight) {}
  //  virtual void on_bufferPaint(S3_Window_Base* AWindow, S3_Widget* AWidget, uint32 AMode) {}
  //  virtual void on_bufferBlit(S3_Window_Base* AWindow, S3_Rect ARect) {}
};

//----------------------------------------------------------------------
#endif
