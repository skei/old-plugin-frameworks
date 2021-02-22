#ifndef s3_widget_group_included
#define s3_widget_group_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"

//----------

class S3_Widget_Group
: public S3_Widget {

  private:

    S3_Widget*  MContainer;
    bool        MClosed;
    int32       MPrevWidth;
    int32       MPrevHeight;

  public:

    S3_Widget_Group(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Group";
      MHint = "group";
      MContainer = new S3_Widget(S3_Rect(0),s3_wa_fillClient);
      MContainer->name("groupbox container");
      MContainer->setFlag(s3_wf_clip);
      S3_Widget::appendWidget(MContainer);
      MClosed = false;
      MPrevWidth = 0;
      MPrevHeight = 0;
      close();
    }

    virtual ~S3_Widget_Group() {
    }

  public:

    S3_Widget* container(void) { return MContainer; }

  public:

    virtual
    int32 appendWidget(S3_Widget* AWidget) {
      return MContainer->appendWidget(AWidget);
    }

    void open(void) {
      if (MClosed) {
        MClosed = false;
        setFlag(s3_wf_visible);
        setFlag(s3_wf_active);
        MRect.w = MPrevWidth;
        MRect.h = MPrevHeight;
      }
    }

    void close(void) {
      if (!MClosed) {
        MClosed = true;
        clearFlag(s3_wf_visible);
        clearFlag(s3_wf_active);
        MPrevWidth = MRect.w;
        MPrevHeight = MRect.h;
        MRect.w = 0;
        MRect.h = 0;
      }
    }

    void toggle(void) {
      if (MClosed) {
        MClosed = false;
        setFlag(s3_wf_visible);
        setFlag(s3_wf_active);
        MRect.w = MPrevWidth;
        MRect.h = MPrevHeight;
      }
      else {
        MClosed = true;
        clearFlag(s3_wf_visible);
        clearFlag(s3_wf_active);
        MPrevWidth = MRect.w;
        MPrevHeight = MRect.h;
        MRect.h = 0;
        MRect.h = 0;
      }
    }

  public:

    //virtual void do_widgetUpdate(S3_Widget* AWidget) {
    //  if (AWidget == MTitleBar) {
    //    if (MClosable) {
    //      if (MTitleBar->value() > 0.5) open();
    //      else close();
    //      do_widgetResize(MContainer,0,MContainer->rect().h);
    //      do_widgetRedraw(MContainer,MContainer->rect(),s3_pm_normal);
    //    }
    //  }
    //  //else
    //  S3_Widget::do_widgetUpdate(AWidget);
    //}

};

//----------------------------------------------------------------------
#endif


