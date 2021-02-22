#ifndef s3_widget_knob_included
#define s3_widget_knob_included
//----------------------------------------------------------------------

#include "gui/widgets/s3_widget_dragvalue.h"

//----------

class S3_Widget_Knob
: public S3_Widget_DragValue {

  protected:

    S3_Rect MLabelRect;
    S3_Rect MKnobRect;
    S3_Rect MValueRect;

    S3_Color MKnobBackColor;
    S3_Color MKnobValueColor;
    S3_Color MKnobCenterColor;

    S3_Color MCenterColor;

    bool is_inside;

  public:

    S3_Widget_Knob(S3_Rect ARect, float AValue, uint32 AAlignment=s3_wa_none)
    : S3_Widget_DragValue(ARect,AValue,AAlignment) {
      MName = "S3_Widget_Knob";
      MHint = "knob";
      MKnobBackColor    = S3_DarkGrey;
      MKnobValueColor   = S3_LightGrey;
      MKnobCenterColor  = S3_Grey;
      MTextColor        = S3_DarkGrey;
      MValueColor       = S3_LightGrey;
      MCursor = -1;
      is_inside = false;
    }

    virtual ~S3_Widget_Knob() {
    }

  public:

    void labelRect(S3_Rect ARect) { MLabelRect = ARect; }
    void knobRect(S3_Rect ARect)  { MKnobRect = ARect; }
    void valueRect(S3_Rect ARect) { MValueRect = ARect; }
    void knobCenterColor(S3_Color AColor) { MKnobCenterColor = AColor; }

  public:

    void recalcRects(S3_Rect ARect) {
      int32 w = ARect.w;
      int32 h = ARect.h;

      if (w==h) {
        //MDrawName   = false;
        //MDrawValue  = false;
        MLabelRect  = S3_Rect(0);
        MKnobRect   = ARect;
        MValueRect  = S3_Rect(0);
      }
      else if (w>h) {
        int32 kw = h;
        int32 kh = h;
        int32 th = 16;
        //MDrawName   = true;
        //MDrawValue  = true;
        MKnobRect   = S3_Rect( 0,    0,    kw,   kh );
        MLabelRect  = S3_Rect( 0+kw, 0,    w-kw, th );
        MValueRect  = S3_Rect( 0+kw, 0+th, w-kw, th );
      }
      else { // w<h
        int32 kw = w;
        int32 kh = w;
        int32 th = 16;
        //MDrawName   = true;
        //MDrawValue  = true;
        MLabelRect  = S3_Rect( 0, 0,       kw, th );
        MKnobRect   = S3_Rect( 0, 0+th,    kw, kh );
        MValueRect  = S3_Rect( 0, 0+th+kh, kw, th );
      }
    }

  public:

    void on_widgetAlign(S3_Widget* AWidget) {
      S3_Widget_DragValue::on_widgetAlign(AWidget);
      recalcRects(MRect);
    }

    //----------

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Rect R;
      // background
      //APainter->setFillColor(MBackColor);
      //APainter->fillRect(MRect);
      //APainter->setDrawColor(FBorderColor);
      //APainter->drawRect(FRect.x,FRect.y,FRect.x2,FRect.y2);
      //
      const char* tl;
      const char* tv;
      if (MParameter) {
        S3_Parameter* param = (S3_Parameter*)MParameter;
        tl = param->name();
        tv = param->displayText( MValue );
      }
      else {
        S3_FloatToString(MTextBuf,MValue);
        tl = MText;//MName;
        tv = MTextBuf;
      }
      // label
      if (MDrawName) {
        R = MLabelRect;
        R.add(MRect.x,MRect.y);
        APainter->setTextColor(MTextColor);
        APainter->drawText(R,tl,s3_ta_center);
      }
      // value
      if (MDrawValue==true) {
        R = MValueRect;
        R.add(MRect.x,MRect.y);
        APainter->setTextColor(MValueColor);
        APainter->drawText(R,tv,s3_ta_center);
      }
      //--- knob ---
      R = MKnobRect;
      R.add(MRect.x,MRect.y);
      // background arc
      APainter->setFillColor(MKnobBackColor);
      APainter->fillArc(R.x+3,R.y+3,R.x2()-3,R.y2()-3,-0.4,0.8);
      // value arc
      if ((AWidget==this) && (AMode==s3_pm_interact)) APainter->setFillColor(S3_White);
      else APainter->setFillColor(MKnobValueColor);
      APainter->fillArc(R.x+3,R.y+3,R.x2()-3,R.y2()-3,-0.4,MValue*0.8);
      // clear center
      int32 s = R.h / 5; //(3*2);
      APainter->setFillColor(MKnobCenterColor);
      APainter->fillArc(R.x+3+s,R.y+3+s,R.x2()-3-s,R.y2()-3-s,0,1);
    }

    //----------

    virtual
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
      S3_Rect R = MKnobRect;
      R.add( MRect.x, MRect.y );
      if (R.contains(AXpos,AYpos)) {
        if (is_inside) {}
        else {
          is_inside = true;
          do_widgetCursor(this,s3_mc_arrowUpDown);
        }
      }
      else {
        if (is_inside) {
          is_inside = false;
          do_widgetCursor(this,s3_mc_default);
        }
        else {}
      }
      S3_Widget_DragValue::on_widgetMouseMove(AWidget,AXpos,AYpos,AState);
    }


    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Rect R = MKnobRect;
      R.add( MRect.x, MRect.y );
      if (R.contains(AXpos,AYpos)) {
        S3_Widget_DragValue::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton, AState);
      }
    }

    //----------

    virtual
    void on_widgetEnter(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
      S3_Rect R = MKnobRect;
      R.add( MRect.x, MRect.y );
      if (R.contains(AXpos,AYpos)) {
        is_inside = true;
        do_widgetCursor(this,s3_mc_arrowUpDown);
      }
      S3_Widget_DragValue::on_widgetEnter(AWidget,AXpos,AYpos);
    }

    //virtual
    //void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
    //}


};

//----------------------------------------------------------------------
#endif

