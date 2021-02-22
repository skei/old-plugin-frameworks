#ifndef s3_window_included
#define s3_window_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #include "gui/s3_window_xlib.h"
  typedef S3_Window_Xlib S3_Window_Implementation;
#endif

#include "gui/s3_painter.h"
#include "gui/s3_surface.h"
#include "gui/s3_widget.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Window
: public S3_Window_Implementation {
  protected:
    S3_Surface* MWindowSurface;
    S3_Painter* MWindowPainter;
    #ifndef S3_NO_WINDOW_BACKBUFFER
    uint32      MBufferWidth;
    uint32      MBufferHeight;
    S3_Surface* MBufferSurface;
    S3_Painter* MBufferPainter;
    #endif
  public:
    S3_Window(S3_Rect ARect, void* AParent=S3_NULL);
    virtual ~S3_Window();
  //public:
  //  void  open(void) override;
  //  void  close(void) override;
  #ifndef S3_NO_WINDOW_BACKBUFFER
  private:
    void  buffer_create(uint32 AWidth, uint32 AHeight);
    void  buffer_destroy(void);
  public:
    void  on_buffer_resize(uint32 AWidth, uint32 AHeight) override;
    void  on_buffer_paint(S3_Rect ARect, uint32 AMode) override;
    void  on_buffer_blit(S3_Rect ARect) override;
  #endif
  public:
    //void  on_window_mapped(void) override;
    void  on_window_resize(uint32 AWidth, uint32 AHeight) override;
    void  on_window_paint(S3_Rect ARect, uint32 AMode) override;
  public:
    void do_widget_move(S3_Widget* AWidget, int32 AXpos, int32 AYpos) override;
    void do_widget_resize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) override;
    void do_widget_sizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) override;
    void do_widget_redraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) override;
    void do_widget_update(S3_Widget* AWidget) override;
    void do_widget_cursor(S3_Widget* AWidget, int32 ACursor) override;
    void do_widget_cursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) override;
    void do_widget_hint(S3_Widget* AWidget, const char* AHint) override;
    void do_widget_modal(S3_Widget* AWidget, int32 AMode=0) override;
    void do_widget_wantKeys(S3_Widget* AWidget) override;
};

//#endif

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//#ifndef S3_NO_WINDOW_BACKBUFFER

S3_Window::S3_Window(S3_Rect ARect, void* AParent)
: S3_Window_Implementation(ARect,AParent) {

  #ifdef S3_CAIRO
    MWindowSurface = new S3_Surface(this,MRect.w,MRect.h);
    MWindowPainter = new S3_Painter(MWindowSurface);
  #else
    MWindowSurface = S3_NULL;//S3_Surface(this,MRect.w,MRect.h);
    MWindowPainter = new S3_Painter(this);
  #endif

  #ifndef S3_NO_WINDOW_BACKBUFFER
    buffer_create(MRect.w,MRect.h);
    //#ifdef S3_CAIRO
    //  if (MWindowSurface) MWindowSurface->setSize(MRect.w,MRect.h);
    //#endif
  #endif
}

//----------

S3_Window::~S3_Window(void) {
  #ifndef S3_NO_WINDOW_BACKBUFFER
    buffer_destroy();
  #endif
  delete MWindowPainter;
  //#ifdef S3_CAIRO
  if (MWindowSurface) delete MWindowSurface;
  //#endif
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//void S3_Window::open(void) {
//  S3_Window_Implementation::open();
//  //on_buffer_paint(MRect,s3_pm_normal);
//  //on_buffer_blit(MRect);
//  //invalidate(MRect);
//}
//
//void S3_Window::close(void) {
//  S3_Window_Implementation::close();
//}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifndef S3_NO_WINDOW_BACKBUFFER

// called from constructor
// don't call virtual methods..

void S3_Window::buffer_create(uint32 AWidth, uint32 AHeight) {
  S3_TRACE;
  MBufferWidth    = AWidth;
  MBufferHeight   = AHeight;
  #ifdef S3_WINDOW_BACKBUFFER_IMAGE
    MBufferImage    = new S3_Image(AWidth,AHeight);
    MBufferPainter  = new S3_Painter(MBufferImage);
  #else
    MBufferSurface = new S3_Surface(this,AWidth,AHeight);
    MBufferPainter = new S3_Painter(MBufferSurface);
  #endif
}

//----------

void S3_Window::buffer_destroy(void) {
  S3_TRACE;
  if (MBufferPainter) delete MBufferPainter;
  MBufferPainter  = S3_NULL;
  MBufferWidth    = 0;
  MBufferHeight   = 0;
  #ifdef S3_WINDOW_BACKBUFFER_IMAGE
    if (MBufferImage) delete MBufferImage;
    MBufferImage = S3_NULL;
  #else
    if (MBufferSurface) delete MBufferSurface;
    MBufferSurface = S3_NULL;
  #endif
}

//----------------------------------------------------------------------
// on_buffer_
//----------------------------------------------------------------------

void S3_Window::on_buffer_resize(uint32 AWidth, uint32 AHeight) {
  S3_TRACE;
  uint32 w = MBufferWidth;
  uint32 h = MBufferHeight;
  if ((w==0) || (h==0)) {
    w = AWidth;
    h = AHeight;
  }
  while (AWidth  > w) w *= 2;
  while (AHeight > h) h *= 2;
  #ifndef S3_NO_SHRINK_BACKBUFFER
    while (AWidth  < (w >> 1)) w >>= 1;
    while (AHeight < (h >> 1)) h >>= 1;
  #endif
  if ((w != MBufferWidth) || (h != MBufferHeight)) {
    buffer_destroy();
    buffer_create(w,h);
  }
  #ifdef S3_CAIRO
    /*if (MWindowSurface)*/ MWindowSurface->setSize(AWidth,AHeight);
  #endif
}

//----------

void S3_Window::on_buffer_paint(S3_Rect ARect, uint32 AMode) {
  S3_TRACE;
  MBufferPainter->pushClip(ARect);
  on_widget_paint(MBufferPainter,ARect,AMode);
  MBufferPainter->popClip();
  //invalidate(ARect.x,ARect.y,ARect.w,ARect.h);
  //MBufferPainter->flush();
}

//----------

void S3_Window::on_buffer_blit(S3_Rect ARect) {
  S3_TRACE;
  //MBufferSurface->flush();
  MWindowPainter->drawBitmap(ARect.x,ARect.y,MBufferSurface,ARect.x,ARect.y,ARect.w,ARect.h);
}

#endif // S3_NO_WINDOW_BACKBUFFER

//----------------------------------------------------------------------
// on_window_
//----------------------------------------------------------------------

//void S3_Window::on_window_mapped(void) {
//  S3_TRACE;
//  #ifdef S3_CAIRO
//    MWindowSurface = new S3_Surface(this,MRect.w,MRect.h);
//    MWindowPainter = new S3_Painter(MWindowSurface);
//    //MWindowSurface->setSize(MRect.w,MRect.h);
//  #else
//    MWindowSurface = S3_NULL;//S3_Surface(this,MRect.w,MRect.h);
//    MWindowPainter = new S3_Painter(this);
//  #endif
//}

//----------

void S3_Window::on_window_resize(uint32 AWidth, uint32 AHeight) {
  S3_TRACE;
  #ifdef S3_CAIRO
    if (MWindowSurface) MWindowSurface->setSize(AWidth,AHeight);
  #endif
}

//----------

void S3_Window::on_window_paint(S3_Rect ARect, uint32 AMode) {
  S3_TRACE;
  MWindowPainter->pushClip(ARect);
  on_widget_paint(MWindowPainter,ARect,AMode);
  MWindowPainter->popClip();
}

//----------------------------------------------------------------------
// do_
//----------------------------------------------------------------------

void S3_Window::do_widget_move(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
  S3_Widget* parent = AWidget->parent();
  if (parent) {
    if (parent->hasFlag(s3_wf_autoalign)) parent->on_widget_align();
    #ifndef S3_NO_WINDOW_BACKBUFFER
      on_buffer_paint(MRect,s3_pm_normal);
    #endif
  }
}

//----------

void S3_Window::do_widget_resize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {
  S3_Widget* parent = AWidget->parent();
  if (parent) {
    if (parent->hasFlag(s3_wf_autoalign)) parent->on_widget_align();
    #ifndef S3_NO_WINDOW_BACKBUFFER
      on_buffer_paint(MRect,s3_pm_normal);
    #endif
  }
  else {
    #ifndef S3_NO_WINDOW_BACKBUFFER
      on_buffer_paint(MRect,s3_pm_normal);
    #endif
  }
}

//----------

void S3_Window::do_widget_sizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {
  // we don't get resize messages from the host, so we 'simulate' it ???
  #ifdef S3_LIB
    // hmm.. do we receive size messages when/if the host resizes the editor?
    // if not, who resizes the backbuffer?
    if (hasFlag(s3_wf_autoalign)) on_align(AWidget);
    on_buffer_paint(MRect,s3_pm_normal);
  #else
    if (AMode == s3_sm_window) {
      int32 w = MRect.w + ADeltaX;
      int32 h = MRect.h + ADeltaY;
      // resize (?) crashes with 0 or negative sizes..
      if (w < 32) w = 32;
      if (h < 32) h = 32;
      setSize(w,h);
      #ifndef S3_NO_WINDOW_BACKBUFFER
        on_buffer_resize(w,h);
      #endif
    }
    if (hasFlag(s3_wf_autoalign)) on_widget_align();
    #ifndef S3_NO_WINDOW_BACKBUFFER
      on_buffer_paint(MRect,s3_pm_normal);
    #endif
  #endif
}

//----------

void S3_Window::do_widget_redraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) {
  #ifndef S3_NO_WINDOW_BACKBUFFER
    //on_buffer_paint(AWidget->rect(),AMode);
    on_buffer_paint(ARect,AMode);
    invalidate(ARect);
    //invalidate?
  #else
    //on_window_paint(AWidget->rect(),AMode);
    on_window_paint(ARect,AMode);
    //TODO
  #endif
}

//----------

void S3_Window::do_widget_update(S3_Widget* AWidget) {
}

//----------

void S3_Window::do_widget_cursor(S3_Widget* AWidget, int32 ACursor) {
  if (ACursor == s3_mc_hide) hideCursor();
  else if (ACursor == s3_mc_show) showCursor();
  else setCursor(ACursor);
}

//----------

void S3_Window::do_widget_cursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
  setCursorPos(AXpos,AYpos);
}

//----------

void S3_Window::do_widget_hint(S3_Widget* AWidget, const char* AHint) {
  //S3_Trace("hint: %s\n",AHint);
}

//----------

void S3_Window::do_widget_modal(S3_Widget* AWidget, int32 AMode) {
  //if AMode = 0 then FModalWidget := nil
  //else
  MModalWidget = AWidget;
}

//----------

void S3_Window::do_widget_wantKeys(S3_Widget* AWidget) {
  if (AWidget) {
    //grabKeyboard();
    MWantKeysWidget = AWidget;
  }
  else {
    //releaseKeyboard();
    MWantKeysWidget = S3_NULL;
  }
}

//----------------------------------------------------------------------
#endif
