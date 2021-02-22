#ifndef kode_range_slider__widgetincluded
#define kode_range_slider__widgetincluded
//----------------------------------------------------------------------

/*
  TODO:
  - drag middle section to modify both values..
  - use drag_value instead of mouse x pos ?
*/

//----------------------------------------------------------------------

#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_RangeSliderWidget
: public KODE_DragValueWidget {

  protected:
    KODE_Color  MBarColor         = KODE_LightGrey;
    KODE_Color  MEdgeColor        = KODE_DarkGrey;
    KODE_Color  MIEdgeColor       = KODE_White;
    uint32_t    MEdgeWidth        = 3;
    float       MWidgetValue2     = 0.0f;
    bool        MDrawRangeSlider  = true;
    int32_t     MHoverEdge        = -1;
    bool        MIsDragging1      = false;
    bool        MIsDragging2      = false;
    uint32_t    MEdgeDistance     = 3;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_RangeSliderWidget(KODE_Rect ARect, float AValue=0.0f, float AValue2=1.0f)
    : KODE_DragValueWidget(ARect,AValue) {
      MName = "KODE_RangeSliderWidget";
      setValue2(AValue2);
      //MDrawValueBar   = false;
      MDrawText       = false;
      MFillBackground = false;
      MDrawBorder     = true;//false;
      //clearFlag(kwf_setCursor);
      clearFlag(KODE_WIDGET_HIDECURSOR);
      MCursor = KODE_CURSOR_DEFAULT;

      setFlag(KODE_WIDGET_HOVER);

    }

    //----------

    virtual ~KODE_RangeSliderWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual float getValue2(void)         { return MWidgetValue2; }
    virtual void  setValue2(float AValue) { MWidgetValue2 = AValue; }

  //------------------------------
  //
  //------------------------------

  public:

    virtual void findHoverEdge(float AXpos) {
      float v = (float)(AXpos - MRect.x) / (float)MRect.w;
      float d1 = fabs(getValue() - v);
      float d2 = fabs(getValue2() - v);
      float d;
      int32_t prev = MHoverEdge;
      if (d1 <= d2) {
        d = d1 * (float)MRect.w;
        if (d <= MEdgeDistance) MHoverEdge = 0;
        else MHoverEdge = -1;
      }
      else {
        d = d2 * (float)MRect.w;
        if (d <= MEdgeDistance) MHoverEdge = 1;
        else MHoverEdge = -1;
      }
      if (MHoverEdge != prev) {
        do_redraw(this,MRect);
        if (MHoverEdge >= 0) {
          do_setCursor(this,KODE_CURSOR_ARROWLEFTRIGHT);
        }
        else {
          do_setCursor(this,KODE_CURSOR_DEFAULT);
          //MHoverEdge = -1;
        }
      }
    }

    //----------

    virtual void drawRangeSlider(KODE_Painter* APainter, KODE_Rect ARect) {
      float x1 = MRect.x + ((float)MRect.w * getValue());
      float x2 = MRect.x + ((float)MRect.w * getValue2()) - 1;
      float w  = (x2 - x1 + 1);
      if (w > 0) {
        APainter->setFillColor(MBarColor);
        APainter->fillRectangle( x1, MRect.y, x2, MRect.y2() );
      }
      if (MIsInteractive) APainter->setFillColor(MIEdgeColor);
      else APainter->setFillColor(MEdgeColor);

      if (MHoverEdge == 0) {
        APainter->fillRectangle( x1, MRect.y, x1+MEdgeWidth, MRect.y2() );
      }
      else if (MHoverEdge == 1) {
        APainter->fillRectangle( x2-MEdgeWidth, MRect.y, x2, MRect.y2() );
      }
    }

  //------------------------------
  // on_ (parent to children)
  //------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      if (MFillBackground) fillBackground(APainter,ARect);
      if (MDrawRangeSlider) drawRangeSlider(APainter,ARect);
      if (MDrawBorder) drawBorder(APainter,ARect);
    }

    //----------

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      if (MHoverEdge==0) MIsDragging1 = true;
      else if (MHoverEdge==1) MIsDragging2 = true;
      MIsInteractive = true;
      if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_HIDE);
      do_redraw(this,MRect);
    }

    //----------

    void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      MIsDragging1 = false;
      MIsDragging2 = false;
      if (MRect.contains(AXpos,AYpos)) findHoverEdge(AXpos);
      else {
        MHoverEdge = -1;
        do_setCursor(this,KODE_CURSOR_DEFAULT);
      }
      MIsInteractive = false;
      if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_SHOW);

      do_redraw(this,MRect);
    }

    //----------

    void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {
      float v = (float)(AXpos - MRect.x) / (float)MRect.w;
      v = KODE_Clamp(v,0,1);
      float value1 = getValue();
      float value2 = getValue2();
      //KODE_TRACE;
      if (MIsDragging1) {
        //KODE_TRACE;
        value1 = v;
        if (value1 > value2) {
          setValue(value2);
          setValue2(value1);
          //KODE_Swap(MIsDragging1,MIsDragging2);
          {
            bool temp = MIsDragging1;
            MIsDragging1 = MIsDragging2;
            MIsDragging2 = temp;
          }
          MHoverEdge = 1;
        }
        else {
          setValue(value1);
          setValue2(value2);
        }
        do_redraw(this,MRect);
      }
      else if (MIsDragging2) {
        //KODE_TRACE;
        value2 = v;
        if (value1 > value2) {
          setValue(value2);
          setValue2(value1);
          //KODE_Swap(MIsDragging1,MIsDragging2);
          {
            bool temp = MIsDragging1;
            MIsDragging1 = MIsDragging2;
            MIsDragging2 = temp;
          }
          
          MHoverEdge = 0;
        }
        else {
          setValue(value1);
          setValue2(value2);
        }
        do_redraw(this,MRect);
      }
      else {
        //KODE_TRACE;
        findHoverEdge(AXpos);
      }
    }

    //----------

    void on_enter(float AXpos, float AYpos, KODE_Widget* AFrom=nullptr) override {
      //if (MIsDragging1 || MIsDragging2) {
      //}
      //else {
        findHoverEdge(AXpos);
      //}
    }

    //----------

    void on_leave(float AXpos, float AYpos, KODE_Widget* ATo) override {
      if (MHoverEdge >= 0) {
        MHoverEdge = -1;
        do_redraw(this,MRect);
      }
      //if (MIsDragging1 || MIsDragging2) {
      //}
      //else {
      //  do_setCursor(this,kmc_default);
      //  //findHoverEdge(AXpos);
      //  do_redraw(this,MRect);
      //}
    }

};

//----------------------------------------------------------------------
#endif

