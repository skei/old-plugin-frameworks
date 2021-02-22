#ifndef kode_group_widget_included
#define kode_group_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"
#include "gui/widgets/kode_switch_widget.h"

//----------------------------------------------------------------------

class KODE_GroupWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_TextWidget*    MHeader     = KODE_NULL;
  KODE_BorderWidget*  MContainer  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_GroupWidget(KODE_Rect ARect, const char* ATitle)
  : KODE_Widget(ARect) {
    MName = "KODE_GroupBoxWidget";
    MHint = "groupbox";
    
    MHeader = KODE_New KODE_TextWidget( KODE_Rect(20), ATitle );
    //MHeader->setName("groupbox header");
    MHeader->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
    MHeader->setDrawBorder(false);
    MHeader->setBackgroundColor(KODE_DarkGrey);
    MHeader->setTextColor(KODE_White);
    
    MContainer = KODE_New KODE_BorderWidget(KODE_Rect(0));
    //MContainer->setName("groupbox container");
    MContainer->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
    MContainer->setBorderColor(KODE_DarkGrey);
    
    MContainer->setPadding(10);
    
    KODE_Widget::appendWidget( MHeader );
    KODE_Widget::appendWidget( MContainer );
  }
  
  //----------

  virtual ~KODE_GroupWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual KODE_TextWidget* getHeader(void) {
    return MHeader;
  }
  
  //----------

  virtual KODE_BorderWidget* getContainer(void) {
    return MContainer;
  }

  //----------

  //void setTitleText(const char* AOnText, const char* AOffText) {
  //  //MTitleBar->setText(AOnText,AOffText);
  //}

  //void setTitleText(const char* AText) {
  //  MTitleBar->setText(AText);
  //}

//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContainer->appendWidget(AWidget);
  }

//------------------------------
public:
//------------------------------

  /*
    todo:
    (optionally) after realignment, resize container to fit child widgets
    
  */
  
  //----------

  //void on_realign(bool ARecursive=true) override {
  //  KODE_TRACE;
  //  KODE_Widget::on_realign(ARecursive);
  //  KODE_Trace("MRect.h %.0f\n",MRect.h);
  //}

};

//----------------------------------------------------------------------
#endif
