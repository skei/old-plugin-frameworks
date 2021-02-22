#ifndef s3_widget_groupbox_included
#define s3_widget_groupbox_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"
#include "gui/widgets/s3_widget_switch.h"

//----------

class S3_Widget_GroupBox
: public S3_Widget {

  private:

    S3_Widget_Switch* MTitleBar;
    S3_Widget*        MContainer;
    bool            MClosed;
    bool            MClosable;
    int32           MPrevSize;

  public:

    //property    _container : KWidget read FContainer;
    //property    _titlebar : KWidget_Switch{Button} read FTitleBar;

  public:

    S3_Widget_GroupBox(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_GroupBox";
      MHint = "groupbox";
      MTitleBar = new S3_Widget_Switch(S3_Rect(20),"groupbox","groupbox",s3_wa_fillTop );
      MContainer = new S3_Widget(S3_Rect(0),s3_wa_fillClient);
      MTitleBar->name("groupbox titlebar");
      MContainer->name("groupbox container");

      //FContainer.setFlag(kwf_clip);
      MContainer->setFlag(s3_wf_clip);

      S3_Widget::appendWidget( MTitleBar );
      S3_Widget::appendWidget( MContainer );
      MClosed = false;
      MClosable = true;
      MPrevSize = 0;
      //FTitleBar.clearFlag(kwf_active);

      close();
    }

    virtual ~S3_Widget_GroupBox() {
    }

  public:

    S3_Widget* titlebar(void) { return MTitleBar; }
    S3_Widget* container(void) { return MContainer; }


  public:

    virtual
    int32 appendWidget(S3_Widget* AWidget) {
      return MContainer->appendWidget(AWidget);
    }

    void open(void) {
      if (MClosed) {
        MClosed = false;
        MContainer->setFlag(s3_wf_active);
        MContainer->setFlag(s3_wf_visible);
        MRect.h = MPrevSize;
      }
    }

    void close(void) {
      if (!MClosed) {
        MClosed = true;
        MContainer->clearFlag(s3_wf_active);
        MContainer->clearFlag(s3_wf_visible);
        MPrevSize = MRect.h;
        MRect.h = MTitleBar->rect().h;
      }
    }

    void toggle(void) {
      if (MClosed) {
        MClosed = false;
        MContainer->setFlag(s3_wf_active);
        MContainer->setFlag(s3_wf_visible);
        MRect.h = MPrevSize;
      }
      else {
        MClosed = true;
        MContainer->clearFlag(s3_wf_active);
        MContainer->clearFlag(s3_wf_visible);
        MPrevSize = MRect.h;
        MRect.h = MTitleBar->rect().h;
      }
    }

  public:

    virtual void do_widgetUpdate(S3_Widget* AWidget) {
      if (AWidget == MTitleBar) {
        if (MClosable) {
          if (MTitleBar->value() > 0.5) open();
          else close();
          do_widgetResize(MContainer,0,MContainer->rect().h);
          do_widgetRedraw(MContainer,MContainer->rect(),s3_pm_normal);
        }
      }
      //else
      S3_Widget::do_widgetUpdate(AWidget);
    }

};

//----------------------------------------------------------------------
#endif

