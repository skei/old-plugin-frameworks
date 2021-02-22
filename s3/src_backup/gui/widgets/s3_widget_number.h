#ifndef s3_widget_number_included
#define s3_widget_number_included
//----------------------------------------------------------------------

#include "common/s3_convert.h"
#include "gui/s3_widget.h"

//----------

class S3_Widget_Number
: public S3_Widget {

  protected:
    //float     MValue;
    S3_Color  MValueColor;
    S3_Color  MBackColor;
    uint32    MValueAlignment;
    bool      MDrawBackground;
    bool      MDrawInteger;
    char      MTextBuf[256];

  public:

    S3_Widget_Number(S3_Rect ARect, float AValue, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Nodes";
      MHint = "number";
      //MText           = AText;
      MValue          = AValue;
      MValueColor     = S3_Black;
      MValueAlignment = s3_ta_right;
      MBackColor      = S3_DarkGrey;
      MDrawBackground = false;
    }

    //----------

    virtual ~S3_Widget_Number() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void valueColor(S3_Color AColor) { MValueColor = AColor; }
    void backColor(S3_Color AColor) { MBackColor = AColor; }
    void valueAlignment(uint32 AAlignment) { MValueAlignment = AAlignment; }
    void drawBackground(bool ADraw=true) { MDrawBackground = ADraw; }
    void drawInteger(bool AInt=true) { MDrawInteger = AInt; }

  //------------------------------
  //
  //------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect);
      }
      APainter->setTextColor(MValueColor);
      if (MDrawInteger) S3_IntToString(MTextBuf,MValue);
      else S3_FloatToString(MTextBuf,MValue);
      APainter->drawText(MRect,MTextBuf,MValueAlignment);
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

};

//----------------------------------------------------------------------
#endif


