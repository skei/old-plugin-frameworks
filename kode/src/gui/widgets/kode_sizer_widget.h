#ifndef kode_widget_sizer_included
#define kode_widget_sizer_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

//----------

#define KODE_SIZER_NONE   0
#define KODE_SIZER_LEFT   1
#define KODE_SIZER_RIGHT  2
#define KODE_SIZER_TOP    3
#define KODE_SIZER_BOTTOM 4
#define KODE_SIZER_WINDOW 5

//----------

class KODE_SizerWidget
: public KODE_Widget {


  protected:
    float       prevx, prevy;
    bool          MIsDragging;
    KODE_Widget*  MTarget;
    //int32_t_    MSizeCursor;
    int32_t       MMode;
    KODE_Color    MFillColor;

  public:
    //property target : KWidget read FTarget write FTarget;
    //property cursor : longint read FSizeCursor write FSizeCursor;
    //property fillcolor : KColor read FFillColor write FFillColor;

  public:

    KODE_SizerWidget(KODE_Rect ARect, uint32_t AMode=KODE_SIZER_NONE, KODE_Widget* ATarget=KODE_NULL)
    : KODE_Widget(ARect) {
      MName       = "KWidget_Sizer";
      MHint       = "sizer";
      MTarget     = ATarget;
      MIsDragging = false;
      MMode       = AMode;
      switch(MMode) {
        case KODE_SIZER_NONE:     MCursor = KODE_CURSOR_DEFAULT;         break;
        case KODE_SIZER_LEFT:     MCursor = KODE_CURSOR_ARROWLEFTRIGHT;  break;
        case KODE_SIZER_RIGHT:    MCursor = KODE_CURSOR_ARROWLEFTRIGHT;  break;
        case KODE_SIZER_TOP:      MCursor = KODE_CURSOR_ARROWUPDOWN;     break;
        case KODE_SIZER_BOTTOM:   MCursor = KODE_CURSOR_ARROWUPDOWN;     break;
        case KODE_SIZER_WINDOW:   MCursor = KODE_CURSOR_ARROWDIAGLEFT;   break;
      }
      MFillColor = KODE_DarkGrey;
    }

    virtual ~KODE_SizerWidget() {
    }

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      APainter->setFillColor(MFillColor);
      if (MMode == KODE_SIZER_WINDOW) APainter->fillTriangle( MRect.x2()-1, MRect.y, MRect.x2()-1, MRect.y2()-1, MRect.x, MRect.y2()-1 );
      else APainter->fillRectangle(MRect);
    }

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      if (AButton == KODE_BUTTON_LEFT) {
        MIsDragging = true;
        prevx = AXpos;
        prevy = AYpos;
      }
    }

    void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      if (AButton == KODE_BUTTON_LEFT) MIsDragging = false;
    }

    void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {
      if (MIsDragging) {
        float deltax = AXpos - prevx;
        float deltay = AYpos - prevy;
        switch(MMode) {
          case KODE_SIZER_LEFT:
            deltay = 0;
            break;
          case KODE_SIZER_RIGHT:
            deltay = 0;
            deltax = -deltax;
            break;
          case KODE_SIZER_TOP:
            deltax = 0;
            break;
          case KODE_SIZER_BOTTOM:
            deltax = 0;
            deltay = -deltay;
            break;
          case KODE_SIZER_WINDOW:
            break;

        } // switch mode
        //if FMode = ksm_Horizontal then deltax := 0;
        //if FMode = ksm_Vertical then deltay := 0;
//        if (MMode == KODE_SIZER_WINDOW) {
//          if (MParent) MParent->do_sizer(this,deltax,deltay,MMode);
//        }
//        else {
//          //STrace("calling target\n");
          if (MTarget) MTarget->do_sizer(MTarget,deltax,deltay,MMode);
//          //else self.do_sizer(self,deltax,deltay,FMode);
//        }
        prevx = AXpos;
        prevy = AYpos;
      } // dragging
      //axWidget::doMouseMove(aXpos,aYpos,aButton);
    }

    void on_enter(float AXpos, float AYpos, KODE_Widget* AFrom) override {
      //int32_t cursor = kmc_default;
      //switch(MMode) {
      //  case ksm_left:    cursor = kmc_arrowLeftRight;   break;
      //  case ksm_right:   cursor = kmc_arrowLeftRight;   break;
      //  case ksm_top:     cursor = kmc_arrowUpDown;      break;
      //  case ksm_bottom:  cursor = kmc_arrowUpDown;      break;
      //  case ksm_window:  cursor = kmc_arrowDiagLeft;    break;
      //}
      do_setCursor( this, MCursor );
    }

    void on_leave(float AXpos, float AYpos, KODE_Widget* ATo) override {
      do_setCursor( this, KODE_CURSOR_DEFAULT );
    }

};

//----------------------------------------------------------------------
#endif

