#ifndef s3_widget_sliderbank_included
#define s3_widget_sliderbank_included
//----------------------------------------------------------------------

#include "common/s3_random.h"
#include "gui/s3_widget.h"

//----------

#define S3_MAX_NUM_SLIDERS 256

//----------

class S3_Widget_SliderBank
: public S3_Widget {

  private:

    int32     MHoverSlider;
    int32     MNumSliders;
    S3_Color  MBarColor;
    S3_Color  MHoverColor;
    float     MSliderValues[S3_MAX_NUM_SLIDERS];


  public:

    S3_Widget_SliderBank(S3_Rect ARect, int32 ANum, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_SliderBank";
      MHint = "sliderbank";
      MNumSliders = ANum;
      MHoverSlider = -1;
      MBarColor = S3_DarkGrey;
      MHoverColor = S3_LightGrey;
      //S3_Memset(MSliderValues,0,sizeof(MSliderValues));
      for (int32 i=0; i<S3_MAX_NUM_SLIDERS; i++) MSliderValues[i] = S3_Random();
    }

    virtual ~S3_Widget_SliderBank() {
    }

  public:

    void numSliders(int32 ANum)     { MNumSliders = ANum; }
    void barColor(S3_Color AColor)  { MBarColor = AColor; }

  //public:
  //
  //  void drawBar(SPainter* APainter, int32 AIndex) {
  //    float w = ( (float)MRect.w / (float)MNumSliders );
  //    float x = (float)MRect.x + (w * AIndex);
  //    float y = MRect.y2() - floorf(MSliderValues[AIndex] * MRect.h);
  //    APainter->fillRect( floorf(x), y/*MRect.y*/, floorf(x+w)-2, MRect.y2() );
  //  }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      //STrace("painting %s\n",MName);
      if (MNumSliders > 1) {
        float w = ( (float)MRect.w / (float)MNumSliders );
        float x = (float)MRect.x /*+ sliderwidth - 1*/;
        for (int32 i=0; i<MNumSliders; i++) {
          if (i==MHoverSlider) APainter->setFillColor( MHoverColor );
          else APainter->setFillColor( MBarColor );
          float y = MRect.y2() - floorf(MSliderValues[i] * MRect.h);
          APainter->fillRectangle( floorf(x), y, floorf(x+w)-2, MRect.y2() );
          x += w;
        }
      }
    }

    //----------

    //virtual
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);
      float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      int32 index = (w * (float)MNumSliders);
      MHoverSlider = index;
      do_widgetRedraw(this,MRect,s3_pm_hover);
    }

    //----------

    //virtual
    void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
      //SWidget::on_leave(AWidget);
      if (MHoverSlider>=0) {
        MHoverSlider = -1;
        do_widgetRedraw(this,MRect,s3_pm_normal);
      }
    }

  /*

  public:

    virtual void on_reset(int32 AMode=0) {}
    virtual void on_setPos(int32 AXpos, int32 AYpos) {}
    virtual void on_setSize(int32 AWidth, int32 AHeight) {}
    virtual void on_scroll(int32 ADeltaX, int32 ADeltaY) {}
    virtual void on_align(void) {}
    virtual void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {}
    virtual void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {}
    virtual void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_keyDown(int32 AKey, int32 AState) {}
    virtual void on_keyUp(int32 AKey, int32 AState) {}
    virtual void on_enter(SWidget* AWidget) {}
    virtual void on_leave(SWidget* AWidget) {}

  public:

    virtual void do_move(SWidget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_sizer(SWidget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {}
    virtual void do_resize(SWidget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void do_redraw(SWidget* AWidget, SRect ARect) {}
    virtual void do_update(SWidget* AWidget) {}
    virtual void do_cursor(SWidget* AWidget, int32 ACursor) {}
    virtual void do_hint(SWidget* AWidget, const char* AHint) {}
    virtual void do_modal(SWidget* AWidget, int32 AMode=0) {}
    virtual void do_wantkeys(SWidget* AWidget) {}

  */

};

//----------

#undef S3_MAX_NUM_SLIDERS


//----------------------------------------------------------------------
#endif

