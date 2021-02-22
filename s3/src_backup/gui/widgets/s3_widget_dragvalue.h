#ifndef s3_widget_dragvalue_included
#define s3_widget_dragvalue_included
//----------------------------------------------------------------------

//#include <math.h>

#include "gui/s3_widget.h"
#include "plugin/s3_parameter.h"

//----------

class S3_Widget_DragValue
: public S3_Widget {

  //private:
  protected:

    //S3_Window*  MWindow;
    bool        MDragging;
    int32       MStartX;
    int32       MStartY;
    int32       MPrevX;
    int32       MPrevY;
    bool        MHideCursor;
    bool        MDrawBackground;
    bool        MDrawName;
    bool        MDrawValue;

    float       MValueScale;
    float       MValueOffset;

    float       MOffValue;

  protected:
    S3_Color        MBackColor;
    S3_Color        MTextColor;
    S3_Color        MValueColor;
    S3_Color        MZeroColor;
    const char*     MText;
    char            MTextBuf[256];
    bool            MHorizontal;
    float           MSensitivity;
    float           MSensitivity2;

  public:

    S3_Widget_DragValue(S3_Rect ARect, float AValue, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName         = "S3_Widget_DragValue";
      MHint         = "value";
      MFlags        |= s3_wf_opaque;
      MValue        = AValue;
      MBackColor    = S3_Color(0.55,0.55,0.55); //S3_DarkGrey;
      MValueColor   = S3_White;
      MTextColor    = S3_Color(0.8,0.8,0.8);
      MZeroColor    = S3_Color(0.5,0.5,0.5);
      MText         = "value";
      MHorizontal   = false;
      MSensitivity  = 1.0f / 250.0f;
      MSensitivity2 = 1.0f / 15.0f; // MSensitivity multiplier when holding ctrl
      if (MHorizontal) MCursor = s3_mc_arrowLeftRight;
      else MCursor = s3_mc_arrowUpDown;
      MTextBuf[0]   = 0;
      // mouse drag
      MDragging   = false;
      MStartX     = 0;
      MStartY     = 0;
      MPrevX      = 0;
      MPrevY      = 0;
      MHideCursor = true;

      MDrawBackground = true;;
      MDrawName = true;
      MDrawValue = true;

      MValueScale = 1;
      MValueOffset = 0;

      MOffValue = 0.0;

    }

    //----------

    virtual ~S3_Widget_DragValue() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void backColor(S3_Color AColor)       { MBackColor = AColor; }
    void valueColor(S3_Color AColor)      { MValueColor = AColor; }
    void textColor(S3_Color AColor)       { MTextColor = AColor; }
    void zeroColor(S3_Color AColor)       { MZeroColor = AColor; }
    void text(const char* AText)          { MText = AText; }
    void drawBackground(bool ADraw=true)  { MDrawBackground = ADraw; }
    void drawName(bool ADraw=true)        { MDrawName = ADraw; }
    void drawValue(bool ADraw=true)       { MDrawValue = ADraw; }

    void offValue(float AValue)           { MOffValue = AValue; }

    void horizontal(bool AHoriz) {
      MHorizontal = AHoriz;
      if (MHorizontal) MCursor = s3_mc_arrowLeftRight;
      else MCursor = s3_mc_arrowUpDown;
    }

    void valueScale(float AScale) { MValueScale = AScale; }
    void valueOffset(float AOffset) { MValueOffset = AOffset; }

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      if (MDrawBackground) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect);
      }
      const char* vv;
      const char* vt;
      if (MParameter) {
        S3_Parameter* param = (S3_Parameter*)MParameter;

        vv = param->displayText( MValue );
        vt = param->name();
      }
      else {
        S3_FloatToString(MTextBuf, MValue*MValueScale+MValueOffset );
        vv = MTextBuf;
        vt = MText;//MName;
      }
      if (MDrawName) {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2(),MRect.y2(),vt, s3_ta_left);
      }

      if (MDrawValue) {
        //if (MValue>=0.01) APainter->setTextColor(S3_Black);
        //else
        if (MParameter) {
          S3_ParamFloat* param = (S3_ParamFloat*)MParameter;
          float v = param->from01(MValue);
          float av = fabs(v-MOffValue);
          if (av<=S3_TINY) APainter->setTextColor(MZeroColor);
          else APainter->setTextColor(MValueColor);
        }
        else {
          float v = MValue;
          float av = fabs(v-MOffValue);
          if (av<=S3_TINY) APainter->setTextColor(MZeroColor);
          else APainter->setTextColor(MValueColor);
        }
        if (!MDrawName) APainter->drawText(MRect.x,MRect.y,MRect.x2()-2,MRect.y2(),vv, s3_ta_center);
        else APainter->drawText(MRect.x,MRect.y,MRect.x2()-2,MRect.y2(),vv, s3_ta_right);
      }
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton==s3_mb_left) {
        if (AState & s3_mk_shift) {
          if (MParameter) {
            S3_Parameter* param = (S3_Parameter*)MParameter;
            float def = param->defaultValue();
            MValue = def;
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          }
        }
        else if (!MDragging) {
          MStartX = AXpos;
          MStartY = AYpos;
          MPrevX = AXpos;
          MPrevY = AYpos;
          MDragging = true;
          do_widgetCursor(this,s3_mc_hide);
          do_widgetRedraw(this,MRect,s3_pm_interact);
        }
      }

    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton==s3_mb_left) {
        if (MDragging) {
          MDragging = false;
          do_widgetCursorPos(this,MStartX,MStartY);
          do_widgetCursor(this,s3_mc_show);
          do_widgetRedraw(this,MRect,s3_pm_normal);
        }
      }
    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
      int32 prevx = MPrevX;
      int32 prevy = MPrevY;
      MPrevX = AXpos;
      MPrevY = AYpos;
      if (MDragging) {
        if ((AXpos != MStartX) || (AYpos != MStartY)) {
          int32 diffx = AXpos - prevx;
          int32 diffy = AYpos - prevy;
          if ((diffx != 0) || (diffy != 0)) {
            do_widgetCursorPos(this,MStartX,MStartY);
            float deltax = (float)diffx * MSensitivity;
            float deltay = (float)diffy * MSensitivity;
            if (AState & s3_mk_ctrl) {
              deltax *= MSensitivity2;
              deltay *= MSensitivity2;
            }
            if (MHorizontal) MValue += deltax;
            else MValue -= deltay;
            MValue = S3_Clamp(MValue,0,1);
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_interact);
          } // diff != 0
        } // pos != start
      } // dragging
    }

    //----------

};

//----------------------------------------------------------------------
#endif

