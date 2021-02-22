#ifndef s3_widget_imgknob_included
#define s3_widget_imgknob_included
//----------------------------------------------------------------------

#include "common/s3_integer.h"
#include "gui/widgets/s3_widget_knob.h"
#include "gui/s3_tilemap.h"

//----------

class S3_Widget_ImgKnob
: public S3_Widget_Knob {

  private:

    S3_TileMap*   MTileMap;

  public:

    S3_Widget_ImgKnob(S3_Rect ARect, S3_TileMap* ATileMap, float AValue=0, uint32 AAlignment=s3_wa_none)
    : S3_Widget_Knob(ARect,AValue,AAlignment) {
      MName = "S3_Widget_ImgKnob";
      MHint = "imgknob";
      MTileMap = ATileMap;
      MCursor = s3_mc_arrowUpDown;
    }

    virtual ~S3_Widget_ImgKnob() {
    }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Rect R;
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

      /*

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

      */

      uint32 num = MTileMap->numTiles();
      float idx = (float)MValue * (float)num;
      uint32 index = S3_MinI( num-1, floorf(idx) );
      S3_Point pos = R.pos();// MRect.pos();
      S3_Drawable* drawable = MTileMap->drawable();
      S3_Rect rect = MTileMap->getTileRect(index);
      APainter->blendBitmap(pos,drawable,rect);

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
      S3_Widget_Knob::on_widgetMouseMove(AWidget,AXpos,AYpos,AState);
    }


    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Rect R = MKnobRect;
      R.add( MRect.x, MRect.y );
      if (R.contains(AXpos,AYpos)) {
        S3_Widget_Knob::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton, AState);
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
      S3_Widget_Knob::on_widgetEnter(AWidget,AXpos,AYpos);
    }

    //virtual
    //void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
    //}


};

//----------------------------------------------------------------------
#endif


