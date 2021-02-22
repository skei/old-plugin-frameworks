#ifndef s3_widget_tabs_included
#define s3_widget_tabs_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"
#include "gui/widgets/s3_widget_buttonrow.h"
#include "gui/widgets/s3_widget_pages.h"

//----------

class S3_Widget_Tabs
: public S3_Widget {

  protected:

    S3_Widget_ButtonRow*  MHeader;
    S3_Widget_Pages*      MPages;
    int32                 MNumPages;

  public:

    S3_Widget*  header(void)    { return MHeader; }
    S3_Widget*  pages(void)     { return MPages; }
    int32       numpages(void)  { return MNumPages; }

  public:

    S3_Widget_Tabs(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Tabs";
      MHint = "tabs";
      MHeader = new S3_Widget_ButtonRow(S3_Rect(20),0,s3_wa_fillTop);
      MPages = new S3_Widget_Pages( S3_Rect(0), s3_wa_fillClient);
      MNumPages = 0;
      MHeader->width(0);
      appendWidget(MHeader);
      appendWidget(MPages);
    }

    virtual ~S3_Widget_Tabs() {
    }

  public:

    int32 appendPage(S3_Widget* AWidget, char* ATitle) {
      int32 result = MPages->appendPage(AWidget);
      MHeader->setName(MNumPages,ATitle);
      MNumPages += 1;
      MHeader->width(MNumPages);
      return result;
    }

    void selectTab(int32 AIndex/*, bool ARedraw=false*/) {
      if (AIndex < MNumPages) {
        MHeader->select(AIndex/*,ARedraw*/);
        MPages->setPage(AIndex/*,ARedraw*/);
      }
    }

  public:

    //virtual
    void do_widgetUpdate(S3_Widget* AWidget) {
      if (AWidget == MHeader) {
        int32 sel = MHeader->selected();
        MPages->setPage(sel/*,true*/);
        do_widgetRedraw(MPages,MPages->rect(),s3_pm_normal);
      }
      S3_Widget::do_widgetUpdate(AWidget);
    }

};

//----------------------------------------------------------------------
#endif
