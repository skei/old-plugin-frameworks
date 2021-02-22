#ifndef kode_template_widget_included
#define kode_template_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_TemplateWidget
: public KODE_Widget {
  
//------------------------------
public:
//------------------------------

  KODE_TemplateWidget(KODE_Rect ARect)
  : KODE_Widget(ARect) {
  }
  
  virtual ~KODE_TemplateWidget() {
  }
  
//------------------------------
public:
//------------------------------

  //void on_move(float AXpos, float AYpos) {}
  //void on_resize(float AWidth, float AHeight) {}
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

//------------------------------
public:
//------------------------------

  //void do_notify(KODE_Widget* ASender, int32_t AMsg) {}
  //void do_update(KODE_Widget* ASender) {}
  //void do_redraw(KODE_Widget* ASender, KODE_Rect ARect) {}
  //void do_moved(KODE_Widget* ASender, float AXpos, float AYpos) {}
  //void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) {}
  //void do_setCursor(KODE_Widget* ASender, int32_t ACursor) {}
  //void do_setCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) {}
  //void do_hint(KODE_Widget* ASender, const char* AHint) {}
  //void do_wantKeyEvents(KODE_Widget* ASender) {}
  //void do_setModal(KODE_Widget* ASender) {}

};

//----------------------------------------------------------------------
#endif