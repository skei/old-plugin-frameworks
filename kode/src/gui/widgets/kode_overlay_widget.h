#ifndef kode_overlay_widget_included
#define kode_overlay_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

//----------

class KODE_OverlayWidget
: public KODE_Widget {

  private:

    //KODE_Widget*  MContainer;
    bool        MClosed;
    //int32       MPrevWidth;
    //int32       MPrevHeight;

  public:

    KODE_OverlayWidget(void)
    : KODE_Widget(KODE_Rect()) {
      MName = "KODE_OverlayWidget";
      MHint = "overlay";
      //MContainer = KODE_New KODE_Widget(KRect(0),kwa_fillClient);
      //MContainer->name("overlay container");
      //MContainer->setFlag(kwf_clip);
      //KODE_Widget::appendWidget(MContainer);
      MClosed = false;
      //MPrevWidth = 0;
      //MPrevHeight = 0;
      close();
    }

    virtual ~KODE_OverlayWidget() {
    }

  public:

    //KODE_Widget* container(void) { return MContainer; }

  public:

    //virtual
    //int32 appendWidget(KODE_Widget* AWidget) {
    //  return MContainer->appendWidget(AWidget);
    //}

    void open(void) {
      if (MClosed) {
        MClosed = false;
        setFlag(KODE_WIDGET_VISIBLE);
        setFlag(KODE_WIDGET_ACTIVE);
        //MRect.w = MPrevWidth;
        //MRect.h = MPrevHeight;
      }
    }

    void close(void) {
      if (!MClosed) {
        MClosed = true;
        clearFlag(KODE_WIDGET_VISIBLE);
        clearFlag(KODE_WIDGET_ACTIVE);
        //MPrevWidth = MRect.w;
        //MPrevHeight = MRect.h;
        //MRect.w = 0;
        //MRect.h = 0;
      }
    }

    void toggle(void) {
      if (MClosed) {
        MClosed = false;
        setFlag(KODE_WIDGET_VISIBLE);
        setFlag(KODE_WIDGET_ACTIVE);
        //MRect.w = MPrevWidth;
        //MRect.h = MPrevHeight;
      }
      else {
        MClosed = true;
        clearFlag(KODE_WIDGET_VISIBLE);
        clearFlag(KODE_WIDGET_ACTIVE);
        //MPrevWidth = MRect.w;
        //MPrevHeight = MRect.h;
        //MRect.h = w;
        //MRect.h = 0;
      }
    }

  public:

    //virtual void do_widgetUpdate(KODE_Widget* AWidget) {
    //  if (AWidget == MTitleBar) {
    //    if (MClosable) {
    //      if (MTitleBar->value() > 0.5) open();
    //      else close();
    //      do_widgetResize(MContainer,0,MContainer->rect().h);
    //      do_widgetRedraw(MContainer,MContainer->rect(),kpm_normal);
    //    }
    //  }
    //  //else
    //  KODE_Widget::do_widgetUpdate(AWidget);
    //}

};

//----------------------------------------------------------------------
#endif



