#ifndef kode_button_widget_included
#define kode_button_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_value_widget.h"

class KODE_ButtonWidget
: public KODE_ValueWidget {
  
//------------------------------
private:
//------------------------------

  const char* MOnText = "";
  const char* MOffText = "";
  //KODE_Color  MTempColor = KODE_Black;
  
//------------------------------
public:
//------------------------------

  KODE_ButtonWidget(KODE_Rect ARect, float AValue=0, const char* AOnText="on", const char* AOffText="off")
  : KODE_ValueWidget(ARect,AValue) { 
    
    setDrawValue(false);
    setTextAlignment(KODE_TEXT_ALIGN_CENTER);
    MOnText   = AOnText;
    MOffText  = AOffText;
    if (AValue >= 0.5f) setText(MOnText);
    else setText(MOffText);
    //MTempColor = getBackgroundColor();
    //setFlag(KODE_WIDGET_SETCURSOR);
    MCursor = KODE_CURSOR_FINGER;
    
  }
  
  //----------
  
  virtual ~KODE_ButtonWidget() {
  }

//------------------------------
public:
//------------------------------
  
  void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
    setValue(1.0f);
    setText(MOnText);
    MIsInteractive = true;
    //MTempColor = getBackgroundColor();
    //setBackgroundColor(KODE_White);
    do_update(this);
    do_redraw(this,MRect);
    //do_setCursor(this,KODE_CURSOR_WAIT);
  }
  
  //----------
  
  void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
    setValue(0.0f);
    setText(MOffText);
    MIsInteractive = false;
    //setBackgroundColor(MTempColor);
    do_update(this);
    do_redraw(this,MRect);
    //do_setCursor(this,MCursor);
  }
  
  //----------
  
  //void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
  //  if (MIsInteractive) setBackgroundColor(KODE_White);
  //  else setBackgroundColor(LightGrey);
  //  KODE_ValueWidget::on_paint(APainter,ARect);
  //}
  
};

//----------------------------------------------------------------------
#endif
