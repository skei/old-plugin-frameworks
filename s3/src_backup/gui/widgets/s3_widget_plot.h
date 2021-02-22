#ifndef s3_widget_plot_included
#define s3_widget_plot_included
//----------------------------------------------------------------------

#define MAX_WIDTH 4096

class S3_Widget_Plot
: public S3_Widget {

  private:

    S3_Color    MBackColor;
    S3_Color    MLineColor;
    bool        MDrawBackground;
    float       MValues[MAX_WIDTH];
    bool        MBipolar;

  public:

    S3_Widget_Plot(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName           = "S3_Widget_Plot";
      MHint           = "plot";
      MBackColor      = S3_DarkGrey;
      MLineColor      = S3_LightGrey;
      MDrawBackground = true;
      MBipolar        = true;
      S3_Memset(MValues,0,sizeof(MValues));
    }

    //----------

    virtual ~S3_Widget_Plot() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32 width(void)   { return MRect.w; }
    int32 height(void)  { return MRect.h; }

    void value(int32 AIndex, float AValue) {
      MValues[AIndex] = AValue;
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {

      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect);
      }

      float h2 = (float)MRect.h * 0.5f;
      APainter->setDrawColor( MLineColor );
      int32 yprev = MRect.y2() - 1;
      if (MBipolar) yprev -= h2;
      int32 x = MRect.x;
      for (int32 i=0; i<MRect.w; i++) {
        float v = MValues[i];
        if (MBipolar) v = (v+1) * 0.5;        // -1..1 -> 0..1
        //int32 y = MRect.y2() - (v*h2) - 1;
        int32 y = MRect.y2() - (v*MRect.h) - 1;
        APainter->drawLine(x,yprev,x+1,y);
        x++;
        yprev = y;
      }

      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

};

#undef MAX_WIDTH


//----------------------------------------------------------------------
#endif


#if 0

#include "gui/s3_painter.h"

class S3_Widget;

//----------

class S3_WidgetListener {

  //------------------------------
  // parent -> subwidgets
  //------------------------------

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

#endif // 0
