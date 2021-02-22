#ifndef s3_window_included
#define s3_window_included
//----------------------------------------------------------------------

/*
  handles:
  - double buffering
  - events from widgets (do_widget..)
*/

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #include "gui/linux/s3_window_xlib.h"
  typedef S3_Window_Xlib S3_Window_Implementation;
#endif

//----------

#ifdef S3_WIN32
  #include "gui/win32/s3_window_win32.h"
  typedef S3_Window_Win32 S3_Window_Implementation;
#endif

//------------------------------

class S3_Window
: public S3_Window_Implementation {
  private:
    #ifndef S3_NO_WINDOW_BACKBUFFER
    uint32      MBufferWidth;
    uint32      MBufferHeight;
    S3_Painter* MBufferPainter;
    #ifdef S3_WINDOW_BACKBUFFER_IMAGE
    S3_Image*   MBufferImage;
    #else
    S3_Surface* MBufferSurface;
    #endif
    #endif
  public:
    S3_Window(S3_WindowListener* AListener, S3_Rect ARect, void* AParent=S3_NULL);
    virtual ~S3_Window();
  public: // back buffer
    #ifndef S3_NO_WINDOW_BACKBUFFER
      S3_Painter* bufferPainter(void) { return MBufferPainter; }
      #ifdef S3_WINDOW_BACKBUFFER_IMAGE
        S3_Image*   bufferImage(void)   { return MBufferImage; }
      #else
        S3_Surface* bufferSurface(void) { return MBufferSurface; }
      #endif
    #endif
  // none of these uses the AWindow argument..
  public: // S3_WindowListener
    #ifndef S3_NO_WINDOW_BACKBUFFER
      void on_bufferCreate(S3_Window_Base* AWindow, uint32 AWidth, uint32 AHeight) override;
      void on_bufferDestroy(S3_Window_Base* AWindow) override;
      void on_bufferResize(S3_Window_Base* AWindow, uint32 AWidth, uint32 AHeight) override;
      void on_bufferPaint(S3_Window_Base* AWindow, S3_Widget* AWidget, uint32 AMode) override;
      void on_bufferBlit(S3_Window_Base* AWindow, S3_Rect ARect) override;
    #endif
  public: // S3_WidgetListener
    void do_widgetMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos) override;
    void do_widgetResize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) override;
    void do_widgetSizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) override;
    void do_widgetRedraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) override;
    void do_widgetUpdate(S3_Widget* AWidget) override;
    void do_widgetCursor(S3_Widget* AWidget, int32 ACursor) override;
    void do_widgetCursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) override;
    void do_widgetHint(S3_Widget* AWidget, const char* AHint) override;
    void do_widgetModal(S3_Widget* AWidget, int32 AMode=0) override;
    void do_widgetWantKeys(S3_Widget* AWidget) override;
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Window::S3_Window(S3_WindowListener* AListener, S3_Rect ARect, void* AParent)
: S3_Window_Implementation(AListener,ARect,AParent) {
  MName = "S3_Window";
  #ifndef S3_NO_WINDOW_BACKBUFFER
  MBufferWidth    = 0;
  MBufferHeight   = 0;
  #ifdef S3_WINDOW_BACKBUFFER_IMAGE
  MBufferImage    = S3_NULL;
  #else
  MBufferSurface  = S3_NULL;
  #endif
  MBufferPainter  = S3_NULL;
  // MListener is set to argument or self in impl..
  // calling virtual methods in constructor?
  /*if (MListener)*/ MListener->on_bufferCreate(this,MRect.w,MRect.h);
  #endif
}

//----------

S3_Window::~S3_Window() {
  #ifndef S3_NO_WINDOW_BACKBUFFER
  if (MListener) MListener->on_bufferDestroy(this);
  #endif
}

//------------------------------
// S3_WindowListener
//------------------------------

// none of these uses the AWindow argument..

#ifndef S3_NO_WINDOW_BACKBUFFER

void S3_Window::on_bufferCreate(S3_Window_Base* AWindow, uint32 AWidth, uint32 AHeight) {
  MBufferWidth    = AWidth;
  MBufferHeight   = AHeight;
  #ifdef S3_WINDOW_BACKBUFFER_IMAGE
  MBufferImage    = new S3_Image(AWidth,AHeight);
  MBufferPainter  = new S3_Painter(MBufferImage);
  #else
  MBufferSurface  = new S3_Surface(this,AWidth,AHeight);
  MBufferPainter  = new S3_Painter(MBufferSurface);
  #endif
}

//----------

void S3_Window::on_bufferDestroy(S3_Window_Base* AWindow) {
  if (MBufferPainter) delete MBufferPainter;
  MBufferPainter  = S3_NULL;
  MBufferWidth    = 0;
  MBufferHeight   = 0;
  #ifdef S3_WINDOW_BACKBUFFER_IMAGE
  if (MBufferImage)   delete MBufferImage;
  MBufferImage    = S3_NULL;
  #else
  if (MBufferSurface) delete MBufferSurface;
  MBufferSurface  = S3_NULL;
  #endif
}

//----------

void S3_Window::on_bufferResize(S3_Window_Base* AWindow, uint32 AWidth, uint32 AHeight) {
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
    on_bufferDestroy(this);
    on_bufferCreate(this,w,h);
    //redraw;
  }
}

//----------

void S3_Window::on_bufferPaint(S3_Window_Base* AWindow, S3_Widget* AWidget, uint32 AMode) {
  S3_Rect rect;
  S3_Widget* widget;
  if (AWidget) {
    widget = AWidget;
    rect = AWidget->rect();
  }
  else {
    //S3_Trace("AWidget == S3_NULL\n");
    widget = this;
    rect = MRect;
  }
  //S3_Rect rect = AWidget->rect();
  if (MBufferPainter) {
    MBufferPainter->pushClip(rect);

    /*if (MModalWidget) MModalWidget->on_widgetPaint(widget,MBufferPainter,rect,s3_pm_normal);
    else*/ on_widgetPaint(widget,MBufferPainter,rect,AMode);

    //else if (AWidget) { AWidget->on_paint(AWidget,MBufferPainter,rect); }
    //else { this->on_paint(AWidget,MBufferPainter,rect); }
    MBufferPainter->popClip();
    invalidate( rect.x, rect.y, rect.w, rect.h ) ;
  }
}

//----------

void S3_Window::on_bufferBlit(S3_Window_Base* AWindow, S3_Rect ARect) {
  blit(ARect.x,ARect.y,MBufferPainter,ARect.x,ARect.y,ARect.w,ARect.h);
  //#ifdef S3_WINDOW_BACKBUFFER_IMAGE
  //  blit(ARect.x,ARect.y,MBufferImage,ARect.x,ARect.y,ARect.w,ARect.h);
  //#else
  //  blit(ARect.x,ARect.y,MBufferSurface,ARect.x,ARect.y,ARect.w,ARect.h);
  //#endif
}

#endif // S3_NO_WINDOW_BACKBUFFER

//------------------------------
// S3_WidgetListener
//------------------------------

// widget has been resized

void S3_Window::do_widgetMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
  S3_Widget* parent = AWidget->parent();
  if (parent) {
    if (parent->hasFlag(s3_wf_autoalign)) parent->on_widgetAlign(AWidget);
    #ifndef S3_NO_WINDOW_BACKBUFFER
    if (MListener) MListener->on_bufferPaint(this,parent,s3_pm_normal);
    #endif
  }
}

//----------

// widget has been resized

void S3_Window::do_widgetResize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {
  S3_Widget* parent = AWidget->parent();
  if (parent) {
    if (parent->hasFlag(s3_wf_autoalign)) parent->on_widgetAlign(AWidget);
    #ifndef S3_NO_WINDOW_BACKBUFFER
    if (MListener) MListener->on_bufferPaint(this,parent,s3_pm_normal);
    #endif
  }
  else {
    #ifndef S3_NO_WINDOW_BACKBUFFER
    if (MListener) MListener->on_bufferPaint(this,AWidget,s3_pm_normal);
    #endif
  }
}

//----------

/*
  sizer widget is being dragged
  resize window (if mode==ssm_window), else resize widget
*/

void S3_Window::do_widgetSizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {
  // we don't get resize messages from the host, so we 'simulate' it ???
  #ifdef S3_LIB
    // hmm.. do we receive size messages when/if the host resizes the editor?
    // if not, who resizes the backbuffer?
    if (hasFlag(s3_wf_autoalign)) on_widgetAlign(AWidget);
    if (MListener) MListener->on_bufferPaint(this,S3_NULL,s3_pm_normal);
  #else
    if (AMode==s3_sm_window) {
      int32 w = MRect.w + ADeltaX;
      int32 h = MRect.h + ADeltaY;
      // resize (?) crashes with 0 or negative sizes..
      if (w <= 32) w = 32;
      if (h <= 32) h = 32;
      setSize(w,h);
      //if ( (w!=MBufferWidth) || (h!=MBufferHeight) )
      #ifndef S3_NO_WINDOW_BACKBUFFER
      if (MListener) MListener->on_bufferResize(this,w,h);
      #endif
    }
    if (hasFlag(s3_wf_autoalign)) on_widgetAlign(AWidget);
    #ifndef S3_NO_WINDOW_BACKBUFFER
    if (MListener) MListener->on_bufferPaint(this,S3_NULL,s3_pm_normal);
    #endif
  #endif
}

//----------

void S3_Window::do_widgetRedraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) {
  #ifndef S3_NO_WINDOW_BACKBUFFER
  if (MListener) MListener->on_bufferPaint(this,AWidget,AMode);
  #else
    //TODO
  #endif
}

//----------

void S3_Window::do_widgetUpdate(S3_Widget* AWidget) {
}

//----------

void S3_Window::do_widgetCursor(S3_Widget* AWidget, int32 ACursor) {
  //S3_Trace("cursor %i\n",ACursor);
  if (ACursor==s3_mc_hide) hideCursor();
  else if (ACursor==s3_mc_show) showCursor();
  else setCursor(ACursor);
}

//----------

void S3_Window::do_widgetCursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
  setCursorPos(AXpos,AYpos);
}

//----------

void S3_Window::do_widgetHint(S3_Widget* AWidget, const char* AHint) {
}

//----------

void S3_Window::do_widgetModal(S3_Widget* AWidget, int32 AMode) {
  //if AMode = 0 then FModalWidget := nil
  //else
  MModalWidget = AWidget;
}

//----------

void S3_Window::do_widgetWantKeys(S3_Widget* AWidget) {
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
