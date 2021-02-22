#ifndef s3_widget_scrollbox_included
#define s3_widget_scrollbox_included
//----------------------------------------------------------------------

/*
  this is a bit clumsy/hack-ish..
  that un_scroll and re_scroll things..

  one minor issue:
  when resizing the window (not using scrollbar), in do_update,
  scrollbox is one pixel off (top)
*/


#include "gui/s3_widget.h"
#include "gui/widgets/s3_widget_scrollbar.h"

//----------

class S3_Widget_ScrollBox
: public S3_Widget {

  protected:

    S3_Widget_ScrollBar*  MScrollBar;
    S3_Widget*            MContainer;   // has background
    int32                 MScrolledX;   // how much we've already scrolled
    int32                 MScrolledY;   // -_"--
    float                 MVisible;     // how much of the content is visible (calculated in doRealign)

  public:

    S3_Widget* container(void) { return MContainer; }
    S3_Widget* scrollbar(void) { return MScrollBar; }

  public:

    S3_Widget_ScrollBox(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_ScrollBox";
      MHint = "scrollbox";
      //KClearFlag(FFlags,kwf_opaque);
      MScrollBar = new S3_Widget_ScrollBar(S3_Rect(10),0,true,s3_wa_fillRight);
      MContainer = new S3_Widget(S3_Rect(0), s3_wa_fillClient);
      MScrollBar->setFlag(s3_wf_vertical);
      MScrollBar->thumbsize(1);
      MContainer->setFlag(s3_wf_clip);
      S3_Widget::appendWidget( MScrollBar );
      S3_Widget::appendWidget( MContainer );
      MScrolledX = 0;
      MScrolledY = 0;
      MVisible = 1;
    }

    //----------

    virtual ~S3_Widget_ScrollBox() {
    }

  public:

    /*
      append sub-widgets to FContainer, not self
    */


    virtual
    int32 appendWidget(S3_Widget* AWidget) {
      int32 result = MContainer->appendWidget(AWidget);
      return result;
    }

    //----------

    //SWidget* subWidget(uint32 AIndex) {
    //  return MContainer->MSubWidgets[AIndex];
    //}

    //----------

    /*
      called from on_align

      calculates the size of the thumb-bar (scrollbar), based on FContent
      (calculated in on_align), and how much of the content that is visible
    */

    void calc_thumbsize(void) {
      float height  = (float)MContainer->rect().h; // size of container
      float content = (float)MContainer->contentRect().h;    // size of content
      if (content > height) {
        MVisible = height / content;
        MScrollBar->thumbsize(MVisible);
      }
      else {
        MVisible = 1;
        MScrollBar->thumbsize(1);
      }
    }

    //----------

    // called from on_align

    void un_scroll(void) {
      //do_scroll(-mScrolledX,-mScrolledY);
      MContainer->on_widgetScroll(this,MScrolledX,MScrolledY);
      MScrolledX = 0;
      MScrolledY = 0;
      MScrollBar->value(0);
    }

    //----------

    // called from on_align
    // x/y = 0..1

    void re_scroll(float x, float y) {
      float scrollable = (float)MContainer->contentRect().h * (1-MVisible);
      int32 yscroll = floorf( scrollable*y );
      int32 dy = yscroll - MScrolledY;
      MContainer->on_widgetScroll(this,0,-dy);
      MScrolledX += 0;
      MScrolledY += dy;
    }

  public:

    /*
      on_align resets/modifies x/y pos and sizes, so we remember the current
      y position, realigns, and then resets the position..

      perhaps we could have an offset field in each widget instead?
      FRect being the 'unscrolled' position, that is used in on_align, etc,
      and then a FOffsetRect that is set
    */

    //----------

    //virtual
    void on_widgetAlign(S3_Widget* AWidget) {
      float y = MScrollBar->value();
      un_scroll();
      //inherited;// on_align;
      S3_Widget::on_widgetAlign(this);
      calc_thumbsize();
      re_scroll(0,y);
      MScrollBar->value(y);
    }

    //----------

    //virtual
    void do_widgetUpdate(S3_Widget* AWidget) {
      if (AWidget == MScrollBar) {
        float val = MScrollBar->value();
        float scrollable = (float)MContainer->contentRect().h * (1-MVisible);
        int32 i = floorf(scrollable*val);
        int32 dy = i - MScrolledY;
        MContainer->on_widgetScroll(this,0,-dy);
        MScrolledX += 0;
        MScrolledY += dy;
        do_widgetRedraw(MContainer,MContainer->rect(),s3_pm_normal); // self
      }
      else
      S3_Widget::do_widgetUpdate(AWidget);
    }

    //----------

};

//----------------------------------------------------------------------
#endif
