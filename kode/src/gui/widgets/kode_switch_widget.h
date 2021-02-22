#ifndef kode_switch_widget_included
#define kode_switch_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_value_widget.h"

class KODE_SwitchWidget
: public KODE_ValueWidget {
  
//------------------------------
private:
//------------------------------

  const char* MOnText = "";
  const char* MOffText = "";
  
//------------------------------
public:
//------------------------------

  KODE_SwitchWidget(KODE_Rect ARect, float AValue=0, const char* AOnText="on", const char* AOffText="off")
  : KODE_ValueWidget(ARect,AValue) { 
    setDrawValue(false);
    setTextAlignment(KODE_TEXT_ALIGN_CENTER);
    MOnText   = AOnText;
    MOffText  = AOffText;
    if (AValue >= 0.5f) setText(MOnText);
    else setText(MOffText);
    //setFlag(KODE_WIDGET_SETCURSOR);
    MCursor = KODE_CURSOR_FINGER;
  }
  
  //----------
  
  virtual ~KODE_SwitchWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setText(const char* AOnText, const char* AOffText=KODE_NULL) {
    MOnText = AOnText;
    if (!AOffText) MOffText = MOnText;
    else MOffText = AOffText;
    const char* txt = KODE_NULL;
    if (getValue() >= 0.5f) txt = MOnText;
    else txt = MOffText;
    KODE_ValueWidget::setText(txt);
    //MText = AOnText;
  }
  
//------------------------------
public:
//------------------------------
  
  void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
    if (getValue() >= 0.5f) {
      setValue(0.0f);
      setText(MOffText);
    }
    else {
      setValue(1.0f);
      setText(MOnText);
    }
    do_update(this);
    do_redraw(this,MRect);
  }
  
};

//----------------------------------------------------------------------
#endif
