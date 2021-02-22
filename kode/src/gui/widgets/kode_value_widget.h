#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_convert.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_text_widget.h"

#ifdef KODE_PLUGIN
  #include "plugin/kode_parameter.h"
#endif

class KODE_ValueWidget
//: public KODE_Widget {
: public KODE_TextWidget {
  
//------------------------------
private:
//------------------------------

  bool        MDrawValue      = true;
  KODE_Color  MValueColor     = KODE_Black;
  uint32_t    MValueAlignment = KODE_TEXT_ALIGN_RIGHT;
  float       MValueOffset    = 3.0f;
  
//------------------------------
public:
//------------------------------

  KODE_ValueWidget(KODE_Rect ARect, float AValue=0.0f)
  //: KODE_Widget(ARect) { 
  : KODE_TextWidget(ARect) { 
    MDrawValue = true;
    setValue(AValue);
    setText("value");
    //MValueColor = KODE_Black;
    //MValueAlignment = KODE_TEXT_ALIGN_RIGHT;
  }
  
  //----------
  
  virtual ~KODE_ValueWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawValue(bool ADraw=true)          { MDrawValue = ADraw; }
  virtual void setValueColor(KODE_Color AColor)       { MValueColor = AColor; }
  virtual void setValueAlignment(uint32_t AAlignment) { MValueAlignment = AAlignment; }
  virtual void setValueOffset(float AOffset)          { MValueOffset = AOffset; }

//------------------------------
public:
//------------------------------
  
  virtual void drawValue(KODE_Painter* APainter, KODE_Rect ARect) {
    if (MDrawValue) {
      float value = getValue();
      char text[32];
      //#ifdef KODE_PLUGIN
        //KODE_Parameter* param = (KODE_Parameter*)MParameter;
        KODE_Parameter* param = (KODE_Parameter*)getParameter();
        if (param) param->getDisplayText(value,text);
        else KODE_FloatToString(text,value);
      //#else
      //  KODE_FloatToString(text,value);
      //#endif
      KODE_Rect r = MRect;
      switch (MValueAlignment) {
        case KODE_TEXT_ALIGN_LEFT:    r.x += MValueOffset; break;
        case KODE_TEXT_ALIGN_RIGHT:   r.w -= MValueOffset; break;
        case KODE_TEXT_ALIGN_TOP:     r.y += MValueOffset; break;
        case KODE_TEXT_ALIGN_BOTTOM:  r.h -= MValueOffset; break;
      }
      APainter->setTextColor(MValueColor);
      APainter->drawText(r,text,MValueAlignment);
    }
  }

    //------------------------------
public:
//------------------------------
  
  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    fillBackground(APainter,ARect);
    drawText(APainter,ARect);
    drawValue(APainter,ARect);
    paintChildren(APainter,ARect);
    drawBorder(APainter,ARect);
  }
  
};

//----------------------------------------------------------------------
#endif
