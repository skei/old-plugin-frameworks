#ifndef s3_widget_text_included
#define s3_widget_text_included
//----------------------------------------------------------------------

#include "gui/s3_font.h"
#include "gui/s3_widget.h"

//----------

class S3_Widget_Text
: public S3_Widget {

  protected:
    const char* MText;
    S3_Color    MTextColor;
    uint32      MTextAlignment;
    S3_Color    MBackColor;
    bool        MDrawBackground;
    S3_Font*    MFont;

  public:

    S3_Widget_Text(S3_Rect ARect, const char* AText, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Text";
      MHint = "text";
      MText           = AText;
      MBackColor      = S3_LightGrey;
      MTextColor      = S3_Black;
      MDrawBackground = false;
      MTextAlignment  = s3_ta_left;
      MFont           = S3_NULL;
    }

    //----------

    virtual ~S3_Widget_Text() {
    }

  //----------------------------------------

  public:

    void text(const char* AText)      { MText = AText; }
    void font(S3_Font* AFont)         { MFont = AFont; }
    void textColor(S3_Color AColor)   { MTextColor = AColor; }
    void textAlignment(uint32 AAlign) { MTextAlignment = AAlign; }
    void backColor(S3_Color AColor)   { MBackColor = AColor; }
    void drawBackground(bool ADraw)   { MDrawBackground = ADraw; }

    const char* text(void)            { return MText; }
    S3_Color    textColor(void)       { return MTextColor; }
    uint32      textAlignment(void)   { return MTextAlignment; }
    S3_Color    backColor(void)       { return MBackColor; }
    bool        drawBackground(void)  { return MDrawBackground; }

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      //S3_TRACE;
      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect);
      }
      if (MFont) APainter->selectFont(MFont);
      APainter->setTextColor(MTextColor);
      APainter->drawText(MRect,MText,MTextAlignment);
      if (MFont) APainter->resetFont();
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

};

//----------------------------------------------------------------------
#endif

