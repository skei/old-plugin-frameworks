#ifndef kode_tabs_widget_included
#define kode_tabs_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"

//----------

class KODE_TabsWidget
: public KODE_Widget {

  protected:

    KODE_ButtonRowWidget* MHeader;
    KODE_PagesWidget*     MPages;
    int32_t               MNumPages;

  public:

    KODE_Widget*  header(void)    { return MHeader; }
    KODE_Widget*  pages(void)     { return MPages; }
    int32_t       numpages(void)  { return MNumPages; }

  public:

    KODE_TabsWidget(KODE_Rect ARect/*, const char** ATxt*/)
    : KODE_Widget(ARect) {
      MName = "KODE_TabsWidget";
      MHint = "tabs";
      
      MHeader = KODE_New KODE_ButtonRowWidget(KODE_Rect(20),0,KODE_NULL/*ATxt*/);
      MHeader->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
      
      MPages = KODE_New KODE_PagesWidget(KODE_Rect());
      MPages->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
      
      MNumPages = 0;
      MHeader->setWidth(0);
      appendWidget(MHeader);
      appendWidget(MPages);
    }

    virtual ~KODE_TabsWidget() {
    }

  public:

    int32_t appendPage(KODE_Widget* AWidget, const char* ATitle) {
      int32_t result = MPages->appendPage(AWidget);
      MHeader->setName(MNumPages,ATitle);
      MNumPages += 1;
      MHeader->setWidth(MNumPages);
      return result;
    }

    void selectPage(int32_t AIndex/*, bool ARedraw=false*/) {
      if (AIndex < MNumPages) {
        MHeader->select(AIndex/*,ARedraw*/);
        MPages->setPage(AIndex/*,ARedraw*/);
      }
    }

  public:

    void do_update(KODE_Widget* AWidget) override {
      if (AWidget == MHeader) {
        int32_t sel = MHeader->getSelected();
        MPages->setPage(sel/*,true*/);
        MPages->on_realign();
        do_redraw(MPages,MPages->getRect());
      }
      KODE_Widget::do_update(AWidget);
    }

};

//----------------------------------------------------------------------
#endif
