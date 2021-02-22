#ifndef kode_widget_pages_included
#define kode_widget_pages_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

//----------

class KODE_PagesWidget
: public KODE_Widget {

  private:

    uint32_t MActivePage = 0;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_PagesWidget(KODE_Rect ARect)
    : KODE_Widget(ARect) {
      MName       = "KODE_PagesWidget";
      MHint       = "pages";
    }

    //----------

    virtual ~KODE_PagesWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32_t appendPage(KODE_Widget* APage) {
      APage->clearFlag(KODE_WIDGET_ACTIVE);
      APage->clearFlag(KODE_WIDGET_VISIBLE);
      return appendWidget(APage)->getIndex();
    }

    //----------

    void setPage(uint32_t APage) {
      if (APage >= MChildren.size()) APage = MChildren.size() - 1;
      for (uint32_t i=0; i<MChildren.size(); i++) {
        KODE_Widget* wdg = MChildren[i];
        if (i == APage) {
          wdg->setFlag(KODE_WIDGET_ACTIVE);
          wdg->setFlag(KODE_WIDGET_VISIBLE);
        }
        else {
          wdg->clearFlag(KODE_WIDGET_ACTIVE);
          wdg->clearFlag(KODE_WIDGET_VISIBLE);
        }
      }
      MActivePage = APage;
    }

};

//----------------------------------------------------------------------
#endif
