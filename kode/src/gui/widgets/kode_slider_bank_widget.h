#ifndef kode_sliderbank_widget_included
#define kode_sliderbank_widget_included
//----------------------------------------------------------------------

#include "base/kode_random.h"
#include "gui/kode_widget.h"

//----------

#define KODE_MAX_NUM_SLIDERS 256

//----------

class KODE_SliderBankWidget
: public KODE_Widget {

  private:

    int32_t     MHoverSlider;
    int32_t     MNumSliders;
    KODE_Color  MBarColor;
    KODE_Color  MHoverColor;
    float       MSliderValues[KODE_MAX_NUM_SLIDERS];


  public:

    KODE_SliderBankWidget(KODE_Rect ARect, int32_t ANum)
    : KODE_Widget(ARect) {
      MName = "KODE_SliderBankWidget";
      MHint = "sliderbank";
      MNumSliders = ANum;
      MHoverSlider = -1;
      MBarColor = KODE_DarkGrey;
      MHoverColor = KODE_LightGrey;

      setFlag(KODE_WIDGET_HOVER);

      //KMemset(MSliderValues,0,sizeof(MSliderValues));
      for (int32_t i=0; i<KODE_MAX_NUM_SLIDERS; i++) MSliderValues[i] = KODE_Random();
    }

    virtual ~KODE_SliderBankWidget() {
    }

  public:

    void numSliders(int32_t ANum)     { MNumSliders = ANum; }
    void barColor(KODE_Color AColor)  { MBarColor = AColor; }

  //public:
  //
  //  void drawBar(SPainter* APainter, int32 AIndex) {
  //    float w = ( (float)MRect.w / (float)MNumSliders );
  //    float x = (float)MRect.x + (w * AIndex);
  //    float y = MRect.y2() - floorf(MSliderValues[AIndex] * MRect.h);
  //    APainter->fillRect( floorf(x), y/*MRect.y*/, floorf(x+w)-2, MRect.y2() );
  //  }

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect/*, uint32 AMode=0*/) override {
      //STrace("painting %s\n",MName);
      if (MNumSliders > 1) {
        float w = ( (float)MRect.w / (float)MNumSliders );
        float x = (float)MRect.x /*+ sliderwidth - 1*/;
        for (int32_t i=0; i<MNumSliders; i++) {
          if (i==MHoverSlider) APainter->setFillColor( MHoverColor );
          else APainter->setFillColor( MBarColor );
          float y = MRect.y2() - floorf(MSliderValues[i] * MRect.h);
          APainter->fillRectangle( floorf(x), y, floorf(x+w)-2, MRect.y2() );
          x += w;
        }
      }
    }

    //----------

    void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {
      //SWidget::on_mouseMove(AXpos,AYpos,AState);
      float w = (float)(AXpos - MRect.x) / (float)MRect.w; // 0..1
      int32_t index = (w * (float)MNumSliders);
      if (index != MHoverSlider) {
        MHoverSlider = index;
        do_redraw(this,MRect);
      }
    }

    //----------

    void on_leave(float AXpos, float AYpos, KODE_Widget* AWidget=nullptr) override {
      //SWidget::on_leave(AWidget);
      if (MHoverSlider>=0) {
        MHoverSlider = -1;
        do_redraw(this,MRect);
      }
    }

};

//----------

#undef KODE_MAX_NUM_SLIDERS


//----------------------------------------------------------------------
#endif

