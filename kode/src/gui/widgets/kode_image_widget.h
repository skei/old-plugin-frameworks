#ifndef kode_image_widget_included
#define kode_image_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_ImageWidget
: public KODE_Widget {

private:

  KODE_Drawable* MImage = nullptr;

//------------------------------
public:
//------------------------------

  KODE_ImageWidget(KODE_Rect ARect, KODE_Drawable* AImage)
  : KODE_Widget(ARect) {
    MName = "KODE_ImageWidget";
    MImage = AImage;
  }

  //----------

  virtual ~KODE_ImageWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    APainter->drawBitmap(MRect.x,MRect.y,MImage,0,0,MRect.w,MRect.h);
    //KODE_Widget::on_paint(APainter,ARect);
  }

};

//----------------------------------------------------------------------
#endif

