#ifndef s3_widget_panel_included
#define s3_widget_panel_included
//----------------------------------------------------------------------

#include "common/s3_color.h"
#include "common/s3_rect.h"
#include "gui/s3_painter.h"
#include "gui/s3_widget.h"

//----------

class S3_Widget_Panel
: public S3_Widget {

  protected:

    S3_Color  MLightColor;
    S3_Color  MDarkColor;
    S3_Color  MFillColor;

  public:

    void lightcolor(S3_Color AColor) { MLightColor = AColor; }
    void darkcolor(S3_Color AColor) { MDarkColor = AColor; }
    void fillcolor(S3_Color AColor) { MFillColor = AColor; }

  public:

    S3_Widget_Panel(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Panel";
      MHint = "panel";
      MFlags |= s3_wf_opaque;
      MLightColor = S3_LightGrey;
      MDarkColor = S3_DarkGrey;
      MFillColor = S3_Grey;
    }

    //----------

    virtual ~S3_Widget_Panel() {
    }

  //----------------------------------------
  // widget listener
  //----------------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      APainter->setFillColor( MFillColor );
      APainter->fillRectangle( MRect);
      APainter->setDrawColor( MLightColor );
      APainter->drawLine( MRect.x, MRect.y, MRect.x2()-1, MRect.y     );
      APainter->drawLine( MRect.x, MRect.y, MRect.x,      MRect.y2()-1 );
      APainter->setDrawColor( MDarkColor );
      APainter->drawLine( MRect.x+1,  MRect.y2(), MRect.x2(), MRect.y2() );
      APainter->drawLine( MRect.x2(), MRect.y+1,  MRect.x2(), MRect.y2() );
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

};
//----------------------------------------------------------------------
#endif
