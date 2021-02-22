#ifndef kode_slider_widget_included
#define kode_slider_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_drag_value_widget.h"

//----------------------------------------------------------------------

class KODE_SliderWidget
: public KODE_DragValueWidget {
  
//------------------------------
private:
//------------------------------

  bool        MDrawSlider  = true;
  KODE_Color  MSliderColor = KODE_LightGrey;

//------------------------------
public:
//------------------------------

  KODE_SliderWidget(KODE_Rect ARect)
  : KODE_DragValueWidget(ARect) {
    setBackgroundColor(KODE_Color(0.55f));
    setDrawBorder(true);
    setBorderColor(KODE_Color(0.45f));
  }
  
  virtual ~KODE_SliderWidget() {
  }
  
//------------------------------
public:
//------------------------------

  virtual void setDrawSlider(bool ADraw=true)     { MDrawSlider = ADraw; }
  virtual void setSliderColor(KODE_Color AColor)  { MSliderColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void drawSlider(KODE_Painter* APainter, KODE_Rect ARect) {
    if (MDrawSlider) {
      float value = getValue();
      if (value > 0.0f) {
        //float mid = MRect.x + (MRect.w * 0.5);
        KODE_Rect r = MRect;
        if (hasFlag(KODE_WIDGET_VERTICAL)) {
          r.y = r.y2() - (MRect.h * value);
          r.h  = (MRect.h * value);
        }
        else {
          r.w = MRect.w * value;
        }
        if (MIsInteractive) APainter->setFillColor(MInteractiveColor);
        else APainter->setFillColor(MSliderColor);
        APainter->fillRectangle(r);
      }
    }
  }
    
    


//------------------------------
public:
//------------------------------

  //void on_move(float AXpos, float AYpos) {}
  //void on_resize(float AWidth, float AHeight) {}
  //void on_realign(bool ARecursive=true) {}
  
  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) {
    
    fillBackground(APainter,ARect);
    drawSlider(APainter,ARect);
    drawText(APainter,ARect);
    drawValue(APainter,ARect);
    paintChildren(APainter,ARect);
    drawBorder(APainter,ARect);
  }
  
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