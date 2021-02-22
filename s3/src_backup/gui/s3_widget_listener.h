#ifndef s3_widget_listener_included
#define s3_widget_listener_included
//----------------------------------------------------------------------

#include "gui/s3_painter.h"

class S3_Widget;

class S3_WidgetListener {

  //------------------------------
  // parent -> subwidgets
  //------------------------------

  // protected:
  public:

    virtual void on_widgetReset(S3_Widget* AWidget, int32 AMode) {}
    virtual void on_widgetSetPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void on_widgetSetSize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void on_widgetScroll(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY) {}
    //virtual void on_widgetMove(S3_Widget* AWidget, int32 AXdelta, int32 AYdelta) {}
    //virtual void on_widgetResize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void on_widgetAlign(S3_Widget* AWidget) {}
    virtual void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode) {}
    virtual void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {}
    virtual void on_widgetMouseDoubleClick(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_widgetKeyDown(S3_Widget* AWidget, int32 AKey, int32 AState) {}
    virtual void on_widgetKeyUp(S3_Widget* AWidget, int32 AKey, int32 AState) {}
    virtual void on_widgetEnter(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    //virtual void on_widgetTimer(S3_Widget* AWidget) {}
    //virtual void on_widgetIdle(S3_Widget* AWidget) {}

    // move to window listener?
    #ifdef S3_TABLET
    virtual void on_widgetTabletEvent(int32 AXpos, int32 AYpos, float APressure) {}
    #endif

  //------------------------------
  // subwidget -> parent
  //------------------------------

  // protected:
  public:

    virtual void do_widgetMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_widgetResize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void do_widgetSizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {}
    virtual void do_widgetRedraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) {}
    virtual void do_widgetUpdate(S3_Widget* AWidget) {}
    virtual void do_widgetCursor(S3_Widget* AWidget, int32 ACursor) {}
    virtual void do_widgetCursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_widgetHint(S3_Widget* AWidget, const char* AHint) {}
    virtual void do_widgetModal(S3_Widget* AWidget, int32 AMode=0) {}
    virtual void do_widgetWantKeys(S3_Widget* AWidget) {}

};

//----------------------------------------------------------------------
#endif
