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

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      //S3_Trace("color paint: %i %i %i %i\n",ARect.x,ARect.y,ARect.x2(),ARect.y2());
      APainter->setFillColor(MColor);

      //APainter->fillRectangle(ARect);
      APainter->fillRectangle(MRect);

      //S3_Rect r = ARect;
      //r.intersection(MRect);
      //APainter->setFillColor(S3_Magenta);
      //APainter->fillRectangle(r);
      //S3_Trace("intersection %i %i %i %i\n",r.x,r.y,r.w,r.h);

      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

};

//----------------------------------------------------------------------
#endif
