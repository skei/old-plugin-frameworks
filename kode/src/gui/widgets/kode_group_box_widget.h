#ifndef kode_group_box_widget_included
#define kode_group_box_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"
#include "gui/widgets/kode_switch_widget.h"

//----------------------------------------------------------------------

class KODE_GroupBoxWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  KODE_SwitchWidget*  MTitleBar   = KODE_NULL;
  KODE_BorderWidget*  MContainer  = KODE_NULL;
  bool                MClosed     = false;
  bool                MClosable   = true;
  float               MOpenSize   = 0.0f;
  float               MClosedSize = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_GroupBoxWidget(KODE_Rect ARect)
  : KODE_Widget(ARect) {
    MName = "KODE_GroupBoxWidget";
    MHint = "groupbox";
    MTitleBar = KODE_New KODE_SwitchWidget( KODE_Rect(20) /*header_rect*/, 1, "groupbox open","groupbox closed" );
    MTitleBar->setName("groupbox titlebar");
    MTitleBar->setAlignment(KODE_WIDGET_ALIGN_TOP_FILL);
    MContainer = KODE_New KODE_BorderWidget(KODE_Rect(0));
    MContainer->setName("groupbox container");
    MContainer->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
    KODE_Widget::appendWidget( MTitleBar );
    KODE_Widget::appendWidget( MContainer );
    MOpenSize   = ARect.h;
    MClosedSize = MTitleBar->getRect().h;
    MClosed     = false;
    MClosable   = true;
    //close();
  }
  
  //----------

  virtual ~KODE_GroupBoxWidget() {
  }

//------------------------------
public:
//------------------------------

  KODE_SwitchWidget* getTitleBar(void) {
    return MTitleBar;
  }
  
  //----------

  KODE_BorderWidget* getContainer(void) {
    return MContainer;
  }

  //----------

  //void setTitleText(const char* AOnText, const char* AOffText) {
  //  //MTitleBar->setText(AOnText,AOffText);
  //}

  //void setTitleText(const char* AText) {
  //  MTitleBar->setText(AText);
  //}
  
  virtual void setClosable(bool AClosable=true) { MClosable = AClosable; }
  //virtual void setOpenSize(float ASize)         { MOpenSize = ASize; }
  //virtual void setClosedSize(float ASize)       { MClosedSize = ASize; }
  
//------------------------------
public:
//------------------------------

  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContainer->appendWidget(AWidget);
  }

  //----------

  void open(void) {
    if (MClosed) {
      MClosed = false;
      MContainer->setFlag(KODE_WIDGET_ACTIVE);
      MContainer->setFlag(KODE_WIDGET_VISIBLE);
      setResized( KODE_Point(0,0) );
      do_resized(this,MRect.w,MOpenSize);
    }
  }

  //----------

  void close(void) {
    if (!MClosed) {
      MClosed = true;
      MContainer->clearFlag(KODE_WIDGET_ACTIVE);
      MContainer->clearFlag(KODE_WIDGET_VISIBLE);
      float rs = MClosedSize - getInitialRect().h;
      //KODE_Trace("rs %.0f\n",rs);
      setResized( KODE_Point(0,rs) );
      do_resized(this,MRect.w,MClosedSize);
    }
  }

  //----------

  void toggle(void) {
    if (MClosed) open();
    else close();
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
  
  //----------
  
  // TODO: togglewidget

  void do_update(KODE_Widget* AWidget) override {
    if (AWidget == MTitleBar) {
      if (MClosable) {
        if (MTitleBar->getValue() > 0.5f) open();
        else close();
      }
    }
    KODE_Widget::do_update(AWidget);
  }

};

//----------------------------------------------------------------------
#endif

