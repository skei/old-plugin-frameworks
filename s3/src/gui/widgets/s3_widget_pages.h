#ifndef s3_widget_pages_included
#define s3_widget_pages_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"

//----------

class S3_Widget_Pages
: public S3_Widget {

  private:

    int32 MActivePage;

  public:

    S3_Widget_Pages(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Pages";
      MHint = "pages";
      MActivePage = 0;
    }

    virtual ~S3_Widget_Pages() {
    }

  public:

    int32 appendPage(S3_Widget* APage) {
      APage->clearFlag(s3_wf_active);
      APage->clearFlag(s3_wf_visible);
      return appendWidget(APage);
    }

    void setPage(uint32 APage/*, bool ARedraw=false*/) {
      if ( APage >= MSubWidgets.size() ) APage = MSubWidgets.size() - 1;
      //if (APage!=MActivePage) {
        for (uint32 i=0; i<MSubWidgets.size(); i++) {
          S3_Widget* wdg = MSubWidgets[i];
          if (i == APage) {
            wdg->setFlag(s3_wf_active);
            wdg->setFlag(s3_wf_visible);
          }
          else {
            wdg->clearFlag(s3_wf_active);
            wdg->clearFlag(s3_wf_visible);
          }
        } //for
        //mPages->initMouseState();
        MActivePage = APage;
        //if (ARedraw) do_redraw(this,MRect);
      //} // !active page
    }

};

//----------------------------------------------------------------------
#endif

