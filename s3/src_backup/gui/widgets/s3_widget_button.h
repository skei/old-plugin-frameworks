#ifndef s3_widget_button_included
#define s3_widget_button_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"

//----------

class S3_Widget_Button
: public S3_Widget {

  protected:
    uint32      MTextAlign;
    S3_Color    MBackOnColor;
    S3_Color    MBackOffColor;
    S3_Color    MTextOnColor;
    S3_Color    MTextOffColor;
    const char* MText;
    const char* MTextOff;

  public:

    S3_Widget_Button(S3_Rect ARect, const char* ATextOn, const char* ATextOff, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName         = "S3_Widget_Button";
      MHint         = "button";
      MFlags |= s3_wf_opaque;
      MText         = ATextOn;
      MTextOff      = ATextOff;
      MValue        = 0;
      MTextAlign    = s3_ta_center;
      MCursor       = s3_mc_finger;
      MBackOnColor  = S3_LightGrey;
      MBackOffColor = S3_DarkGrey;
      MTextOnColor  = S3_DarkGrey;
      MTextOffColor = S3_LightGrey;
    }

    //----------

    virtual ~S3_Widget_Button() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void textAlign(uint32 AAlign)       { MTextAlign = AAlign; }
    void backOnColor(S3_Color AColor)   { MBackOnColor = AColor; }
    void backOffColor(S3_Color AColor)  { MBackOffColor = AColor; }
    void textOnColor(S3_Color AColor)   { MTextOnColor = AColor; }
    void textOffColor(S3_Color AColor)  { MTextOffColor = AColor; }
    void text(const char* AText)        { MText = AText; }
    void textOff(const char* AText)     { MTextOff = AText; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {

      if (MValue > 0.5) {
        APainter->setFillColor( MBackOnColor );
        APainter->fillRectangle(MRect);
        APainter->setTextColor(MTextOnColor);
        APainter->drawText(MRect,MText,MTextAlign);
      } else {
        APainter->setFillColor(MBackOffColor);
        APainter->fillRectangle(MRect);
        APainter->setTextColor(MTextOffColor);
        APainter->drawText(MRect,MTextOff,MTextAlign);
      }
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Widget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
      //if (MValue < 0.5) MValue = 1; else MValue = 0;
      if (AButton==s3_mb_left) {
        MValue = 1;
        do_widgetUpdate(this);
        do_widgetRedraw(this,MRect,s3_pm_interact);
      }
    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Widget::on_widgetMouseUp(AWidget,AXpos,AYpos,AButton,AState);
      //if (MValue < 0.5) MValue = 1; else MValue = 0;
      if (AButton==s3_mb_left) {
        MValue = 0;
        do_widgetUpdate(this);
        do_widgetRedraw(this,MRect,s3_pm_normal);
      }
    }

};

//----------------------------------------------------------------------
#endif
