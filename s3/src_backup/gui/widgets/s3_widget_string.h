#ifndef s3_widget_string_included
#define s3_widget_string_included
//----------------------------------------------------------------------

#include "common/s3_string.h"
#include "gui/s3_widget.h"

//----------

class S3_Widget_String
: public S3_Widget {

  protected:
    //S3_String*  MString;

    S3_String   MString;

    S3_Color    MTextColor;
    uint32      MTextAlignment;
    S3_Color    MBackColor;
    bool        MDrawBackground;

  public:

    //void text(const char* AText) { MText = AText; }

  public:

    S3_Widget_String(S3_Rect ARect, const char* AText, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName           = "S3_Widget_String";
      MHint           = "string";
      MString         = AText;
      MBackColor      = S3_LightGrey;
      MTextColor      = S3_Black;
      MDrawBackground = false;
      MTextAlignment  = s3_ta_left;
    }

    //----------

    S3_Widget_String(S3_Rect ARect, S3_String& AString, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName           = "S3_Widget_String";
      MHint           = "string";
      MString         = AString.ptr();
      MBackColor      = S3_LightGrey;
      MTextColor      = S3_Black;
      MDrawBackground = false;
      MTextAlignment  = s3_ta_left;
    }

    //----------

    virtual ~S3_Widget_String() {
    }

  //----------------------------------------

  public:

    void        string(S3_String* AString) { MString = *AString; }
    const char* text(void) { return MString.ptr(); }

    //SColor      MTextColor;
    //uint32      MTextAlignment;
    //SColor      MBackColor;
    //bool        MDrawBackground;



  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect);
      }
      APainter->setTextColor(MTextColor);
      APainter->drawText(MRect,MString.ptr(),MTextAlignment);
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    /*
    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    }

    virtual // S3_WidgetListener
    void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    }

    virtual // S3_WidgetListener
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
    }
    */

};

//----------------------------------------------------------------------
#endif

