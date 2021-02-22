#ifndef kode_color_widget_included
#define kode_color_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

class KODE_ColorWidget
: public KODE_Widget {
  
//private:
protected:

  bool        MFillBackground   = true;
  KODE_Color  MBackgroundColor  = KODE_Grey;
  
//------------------------------
public:
//------------------------------

  KODE_ColorWidget(KODE_Rect ARect, KODE_Color AColor=KODE_Grey)
  : KODE_Widget(ARect) { 
    MFillBackground = true;
    MBackgroundColor = AColor;
  }
  
  //----------
  
  virtual ~KODE_ColorWidget() {
  }

//------------------------------
public:
//------------------------------
  
  
  virtual void setFillBackground(bool ADraw=true)    { MFillBackground = ADraw; }
  virtual void setBackgroundColor(KODE_Color AColor) { MBackgroundColor = AColor; }
  
  virtual KODE_Color getBackgroundColor() { return MBackgroundColor; }

//------------------------------
public:
//------------------------------
  
  virtual void fillBackground(KODE_Painter* APainter, KODE_Rect ARect) {
    if (MFillBackground) {
      APainter->setFillColor(MBackgroundColor);
      APainter->fillRectangle(MRect);
    }
  }
    
//------------------------------
public:
//------------------------------
  
  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    fillBackground(APainter,ARect);
    //KODE_Widget::on_paint(APainter,ARect);
    paintChildren(APainter,ARect);
  }
  
};

//----------------------------------------------------------------------
#endif
