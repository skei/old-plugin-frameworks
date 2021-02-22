#ifndef kode_text_widget_included
#define kode_text_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_border_widget.h"

class KODE_TextWidget
//: public KODE_Widget {
: public KODE_BorderWidget {
  
//------------------------------
//private:
protected:
//------------------------------

  bool        MDrawText       = true;
  const char* MText           = "";
  KODE_Color  MTextColor      = KODE_DarkGrey;
  uint32_t    MTextAlignment  = KODE_TEXT_ALIGN_LEFT;
  float       MTextOffset     = 3.0f;
  float       MTextSize       = 14.0f;
  
//------------------------------
public:
//------------------------------

  KODE_TextWidget(KODE_Rect ARect, const char* AText="text", KODE_Color AColor=KODE_Black, uint32_t AAlignment=KODE_TEXT_ALIGN_LEFT)
  //: KODE_Widget(ARect) { 
  : KODE_BorderWidget(ARect) { 
    MDrawText = true;
    MText = AText;
    MTextColor = AColor;
    MTextAlignment = AAlignment;
  }
  
  //----------
  
  virtual ~KODE_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw=true)           { MDrawText = ADraw; }
  virtual void setText(const char* AText)             { MText = AText; }
  virtual void setTextColor(KODE_Color AColor)        { MTextColor = AColor; }
  virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  virtual void setTextOffset(float AOffset)           { MTextOffset = AOffset; }

//------------------------------
public:
//------------------------------
  
  virtual void drawText(KODE_Painter* APainter, KODE_Rect ARect) {
    if (MDrawText) {
      KODE_Rect r = MRect;
      switch (MTextAlignment) {
        case KODE_TEXT_ALIGN_LEFT:    r.x += MTextOffset; break;
        case KODE_TEXT_ALIGN_RIGHT:   r.w -= MTextOffset; break;
        case KODE_TEXT_ALIGN_TOP:     r.y += MTextOffset; break;
        case KODE_TEXT_ALIGN_BOTTOM:  r.h -= MTextOffset; break;
      }
      APainter->setTextColor(MTextColor);
      
      const char* text = MText;
      
      // messes up menu/selector..
      
      //KODE_Parameter* param = (KODE_Parameter*)getParameter();
      //if (param) text = param->getName();
      
      APainter->drawText(r,text,MTextAlignment);
    }
  }

    //------------------------------
public:
//------------------------------
  
  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    fillBackground(APainter,MRect);
    drawText(APainter,MRect);
    paintChildren(APainter,MRect);
    drawBorder(APainter,MRect);
    //KODE_BorderWidget::on_paint(APainter,ARect);
  }
  
};

//----------------------------------------------------------------------
#endif
