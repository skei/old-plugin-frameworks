#ifndef kode_widget_windiw_included
#define kode_widget_windiw_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/base/kode_base_window.h"
#include "gui/kode_widget.h"

class KODE_WidgetWindow
: public KODE_BaseWindow
//, public KODE_Drawable
, public KODE_Widget {

public:

  KODE_WidgetWindow(uint32_t AWidth, uint32_t AHeight, void* AParent=nullptr)
  : KODE_BaseWindow(AWidth,AHeight)
  , KODE_Widget(KODE_Rect(AWidth,AHeight)) {
    
    //KODE_Trace("%s %.0f %.0f %.0f %.0f\n",getName(),MRect.x,MRect.y,MRect.w,MRect.h);
    MName = "KODE_WidgetWindow";
    clearFlag(KODE_WIDGET_CLIP);
    MRect.x = 0;
    MRect.y = 0;
    
  }
  
  virtual ~KODE_WidgetWindow() {
  }
  
public:

  //void setPos(float x, float y) override {
  //  MRect.x = 0;
  //  MRect.y = 0;
  //}


public:

  void on_move(float AXpos, float AYpos) override {
    KODE_Widget::on_move(AXpos,AYpos);
    MRect.x = 0;
    MRect.y = 0;
    
  }
  
  void on_resize(float AWidth, float AHeight) override {
    KODE_Widget::on_resize(AWidth,AHeight);
    MRect.x = 0;
    MRect.y = 0;
    //MRect.w = AWidth;
    //MRect.h = AHeight;
  }
  
  //void on_realign(bool ARecursive=true) {}
  //void on_paint(KODE_Painter* APainter, KODE_Rect ARect) {}
  //void on_keyPress(uint8_t AChar, uint32_t AKey, uint32_t AState) {}
  //void on_keyRelease(uint8_t AChar, uint32_t AKey, uint32_t AState) {}
  //void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {}
  //void on_mouseMove(float AXpos, float AYpos, uint32_t AState) {}
  //void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {}
  //void on_enter(float AXpos, float AYpos, KODE_Widget* AFrom=nullptr) {}
  //void on_leave(float AXpos, float AYpos, KODE_Widget* AFrom=nullptr) {}
  //void on_timer() {}
  //void on_idle() {}
  //void on_clientMessage(uint32_t AData) {}
  //void on_connect(KODE_Parameter* AParameter) {}

public:
  
  // ???

  /*
  void do_notify(KODE_Widget* ASender, int32_t AMsg) override {}
  void do_update(KODE_Widget* ASender) override {}
  void do_redraw(KODE_Widget* ASender, KODE_Rect ARect) override {}
  void do_moved(KODE_Widget* ASender, float AXpos, float AYpos) override {}
  void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) override {}
  void do_setCursor(KODE_Widget* ASender, int32_t ACursor) override {}
  void do_setCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {}
  void do_hint(KODE_Widget* ASender, const char* AHint) override {}
  void do_wantKeyEvents(KODE_Widget* ASender) override {}
  void do_setModal(KODE_Widget* ASender) override {}
  //void do_startDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) override {}
  //void do_endDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) override {}
  */

};


//----------------------------------------------------------------------
#endif