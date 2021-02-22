#ifndef s3_widget_slider_included
#define s3_widget_slider_included
//----------------------------------------------------------------------

#include "gui/widgets/s3_widget_dragvalue.h"

//----------

class S3_Widget_Slider
: public S3_Widget_DragValue {

  protected:

    S3_Color  MBarColor;
    //S3_Color  MHoverColor;
    S3_Color  MInteractColor;
    bool      MDrawValueName;
    bool      MDrawValueText;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Widget_Slider(S3_Rect ARect, float AValue, uint32 AAlignment=s3_wa_none)
    : S3_Widget_DragValue(ARect,AValue,AAlignment) {
      MName  = "S3_Widget_Slider";
      MHint = "slider";
      MText = "slider";
      MFlags |= s3_wf_opaque;
      MBackColor      = S3_Color(0.55,0.55,0.55); //S3_DarkGrey;
      MBarColor       = S3_Color(0.65,0.65,0.65); //S3_LightGrey;
      MInteractColor  = S3_Color(0.75,0.75,0.75); //S3_Black;
      MValueColor     = S3_Black;
      MTextColor      = S3_Color(0.90,0.90,0.90); //S3_White;
      MDrawValueName = true;
      MDrawValueText = true;
    }

    //----------

    virtual ~S3_Widget_Slider() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void drawValueName(bool ADraw)  { MDrawValueName = ADraw; }
    void drawValueText(bool ADraw)  { MDrawValueText = ADraw; }
    void barColor(S3_Color AColor)  { MBarColor = AColor; }
    void interactColor(S3_Color AColor)  { MInteractColor = AColor; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      float fw = (float)MRect.w * MValue;
      int32 w = (int)fw;
      const char* nt;
      const char* vt;
      if (MParameter) {
        S3_Parameter* param = (S3_Parameter*)MParameter;
        nt = param->name();
        vt = param->displayText(MValue);
      }
      else {
        nt = MText;//MName;
        S3_FloatToString(MTextBuf,MValue);
        //KFloatToString(txt,FValue);
        vt = MTextBuf;//txt;
      }

      // bar

      if (w > 0) {
        if ((AMode==s3_pm_interact) && (AWidget==this)) APainter->setFillColor(MInteractColor);
        else APainter->setFillColor(MBarColor);
        APainter->fillRectangle(MRect.x,MRect.y,MRect.x+w-1,MRect.y2());

        // border
        //ACanvas.setDrawColor(FBorderColor);
        //ACanvas.drawRect(FRect.x,FRect.y,FRect.x+w-1,FRect.y2);
      }

      // background

      if (w < MRect.w) {
        APainter->setFillColor(MBackColor);
        APainter->fillRectangle(MRect.x+w,MRect.y,MRect.x2(),MRect.y2());

      }

      // border
      //ACanvas.setDrawColor(FBorderColor);
      //ACanvas.drawRect(FRect.x,FRect.y,FRect.x2,FRect.y2);

      // name

      if (MDrawValueName) {
        //nt = MName;
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+4,MRect.y,MRect.x2(),MRect.y2(),nt/*FName*/, s3_ta_left);
      }

      // value

      if (MDrawValueText) {
        APainter->setTextColor(MValueColor);
        APainter->drawText(MRect.x,MRect.y,MRect.x2()-4,MRect.y2(),vt/*FTextBuf*/, s3_ta_right/*center*/);
      }

    }

};

//----------------------------------------------------------------------
#endif
