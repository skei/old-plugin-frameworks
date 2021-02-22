#ifndef s3_widget_color_included
#define s3_widget_color_included
//----------------------------------------------------------------------

#include "common/s3_color.h"
#include "common/s3_rect.h"
#include "gui/s3_painter.h"
#include "gui/s3_widget.h"

//----------

class S3_Widget_Color
: public S3_Widget {

  protected:

    S3_Color  MColor;

  public:

    S3_Widget_Color(S3_Rect ARect, S3_Color AColor, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MColor = AColor;
      MName = "S3_Widget_Color";
      MHint = "color";
      MFlags |= s3_wf_opaque;
    }

    //----------

    virtual ~S3_Widget_Color() {
    }

  //----------------------------------------

  public:

    void on_widget_paint(S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) override {
      APainter->setFillColor(MColor);
      APainter->fillRectangle(MRect);
      S3_Widget::on_widget_paint(APainter,ARect,AMode);
    }

};

//----------------------------------------------------------------------
#endif
