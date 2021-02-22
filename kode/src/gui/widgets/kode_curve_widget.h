#ifndef kode_curve_widget_included
#define kode_curve_widget_included
//----------------------------------------------------------------------

#include "base/kode_math.h"
//#include "gui/kode_widgets.h"
#include "gui/widgets/kode_drag_value_widget.h"

//----------

class KODE_CurveWidget
: public KODE_DragValueWidget {

  protected:

    KODE_Color  MCurveColor     = KODE_LightGrey;
    KODE_Color  MInteractColor  = KODE_White;
    bool        MDecay          = false;
    //KODE_Point  MCoords[1024]   = {0};
    float MXCoords[1024]   = {0};
    float MYCoords[1024]   = {0};

  //------------------------------
  //
  //------------------------------

  public:

    KODE_CurveWidget(KODE_Rect ARect, float AValue=0.5f, bool ADecay=true)
    : KODE_DragValueWidget(ARect,AValue) {
      MName             = "KODE_Widget_Curve";
      MHint             = "curve";
      MText             = "curve";
      MBackgroundColor  = KODE_Color(0.55f,0.55f,0.55f);
      MDecay            = ADecay;
    }

    //----------

    virtual ~KODE_CurveWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void curveColor(KODE_Color AColor)    { MCurveColor = AColor; }
    void interactColor(KODE_Color AColor) { MInteractColor = AColor; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {

      int32_t num = MRect.w;

      // calc coords

      if (num > 0) {
        float x,xadd;
        if (MDecay) {
          x = 1.0;
          xadd = -1.0f / num;
        }
        else {
          x = 0;
          xadd = 1.0f / num;
        }

        //float h = (float)MRect.h;
        //int32 y2 = MRect.y2() + 1;

        float value = getValue();
        for (int32_t i=0; i<num; i++) {
          float n = KODE_Curve(x,value);
          MXCoords[i] = MRect.x + i;
          MYCoords[i] = MRect.y2() + 1 - (n * (float)MRect.h);
          x += xadd;
        }
      }

      if (MDecay) {
        MXCoords[num] = MRect.x2();
        MYCoords[num] = MRect.y2() + 1;
        MXCoords[num+1] = MRect.x;
        MYCoords[num+1] = MRect.y2() + 1;
      }
      else {
        MXCoords[num] = MRect.x2();
        MYCoords[num] = MRect.y + 1;
        MXCoords[num+1] = MRect.x2();
        MYCoords[num+1] = MRect.y2() + 1;
      }

      // background

      APainter->setFillColor(MBackgroundColor);
      APainter->fillRectangle(MRect);

      // curve

      if (MIsInteractive) APainter->setDrawColor(MInteractColor);
      else APainter->setDrawColor(MCurveColor);

      APainter->setPenSize(2);
      //APainter->fillPolygon(num+2,MCoords);
      //APainter->drawLines(num+1,MCoords);
      for (int32_t i=0; i<num; i++) {
        APainter->drawLine(MXCoords[i],MYCoords[i],MXCoords[i+1],MYCoords[i+1]);
      }
      APainter->setPenSize(1);

    }

};

//----------------------------------------------------------------------
#endif
