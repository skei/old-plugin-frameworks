#ifndef kode_border_widget_included
#define kode_border_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_color_widget.h"

class KODE_BorderWidget
//: public KODE_Widget {
: public KODE_ColorWidget {
  
//------------------------------
//private:
protected:
//------------------------------

  bool            MDrawBorder       = true;
  KODE_Color      MBorderColor      = KODE_LightGrey;
  uint32_t        MBorderThickness  = 1;
  
//------------------------------
public:
//------------------------------

  KODE_BorderWidget(KODE_Rect ARect, KODE_Color AColor=KODE_LightGrey, uint32_t AWidth=1)
  //: KODE_Widget(ARect) { 
  : KODE_ColorWidget(ARect) { 
    MDrawBorder = true;
    MBorderColor = AColor;
    MBorderThickness = 1;
  }
  
  //----------
  
  virtual ~KODE_BorderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawBorder(bool ADraw=true)           { MDrawBorder = ADraw; }
  virtual void setBorderColor(KODE_Color AColor)        { MBorderColor = AColor; }
  virtual void setBorderThickness(uint32_t AThickness)  { MBorderThickness = AThickness; }
  
//------------------------------
public:
//------------------------------
  
  virtual void drawBorder(KODE_Painter* APainter, KODE_Rect ARect) {
    if (MDrawBorder) {
      APainter->setDrawColor(MBorderColor);
      APainter->setPenSize(MBorderThickness);
      APainter->drawRectangle(MRect);
    }
  }

//------------------------------
public:
//------------------------------
  
  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    //KODE_ColorWidget::on_paint(APainter,ARect);
    fillBackground(APainter,ARect);
    paintChildren(APainter,ARect);
    drawBorder(APainter,ARect);
  }
  
};

//----------------------------------------------------------------------
#endif
