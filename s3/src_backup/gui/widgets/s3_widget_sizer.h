#ifndef s3_widget_sizer_included
#define s3_widget_sizer_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"

//----------

/*enum ESizerMode {
  s3_sm_left    = 0,
  s3_sm_right   = 1,
  s3_sm_top     = 2,
  s3_sm_bottom  = 3
};*/

//----------

class S3_Widget_Sizer
: public S3_Widget {


  protected:
    int32       prevx,prevy;
    bool        MIsDragging;
    S3_Widget*  MTarget;
    //int32     MSizeCursor;
    int32       MMode;
    S3_Color      MFillColor;

  public:
    //property target : KWidget read FTarget write FTarget;
    //property cursor : longint read FSizeCursor write FSizeCursor;
    //property fillcolor : KColor read FFillColor write FFillColor;

  public:

    S3_Widget_Sizer(S3_Rect ARect, uint32 AMode=s3_sm_none, S3_Widget* ATarget=S3_NULL, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName       = "S3_Widget_Sizer";
      MHint       = "sizer";
      MTarget     = ATarget;
      MIsDragging = false;
      MMode       = AMode;
      switch(MMode) {
        case s3_sm_none:     MCursor = s3_mc_default; break;
        case s3_sm_left:     MCursor = s3_mc_arrowLeftRight; break;
        case s3_sm_right:    MCursor = s3_mc_arrowLeftRight; break;
        case s3_sm_top:      MCursor = s3_mc_arrowUpDown; break;
        case s3_sm_bottom:   MCursor = s3_mc_arrowUpDown; break;
        case s3_sm_window:   MCursor = s3_mc_arrowDiagLeft; break;
      }
      MFillColor = S3_DarkRed;
    }

    virtual ~S3_Widget_Sizer() {
    }

  public:

    virtual void on_widgetPaint(S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      APainter->setFillColor(MFillColor);
      if (MMode == s3_sm_window) APainter->fillTriangle( MRect.x2()-1, MRect.y, MRect.x2()-1, MRect.y2()-1, MRect.x, MRect.y2()-1 );
      else APainter->fillRectangle(MRect);
    }

    virtual void on_widgetMouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == s3_mb_left) {
        MIsDragging = true;
        prevx = AXpos;
        prevy = AYpos;
      }
    }

    virtual void on_widgetMouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == s3_mb_left) MIsDragging = false;
    }

    virtual void on_widgetMouseMove(int32 AXpos, int32 AYpos, int32 AState) {
      if (MIsDragging) {
        int32 deltax = AXpos - prevx;
        int32 deltay = AYpos - prevy;
        switch(MMode) {
          case s3_sm_left:
            deltay = 0;
            break;
          case s3_sm_right:
            deltay = 0;
            deltax = -deltax;
            break;
          case s3_sm_top:
            deltax = 0;
            break;
          case s3_sm_bottom:
            deltax = 0;
            deltay = -deltay;
            break;
          //ksm_Window:
          //  begin
          //  end;
          //  break;

        } // switch mode
        //if FMode = ksm_Horizontal then deltax := 0;
        //if FMode = ksm_Vertical then deltay := 0;
        if (MMode == s3_sm_window) {
          if (MParent) MParent->do_widgetSizer(this,deltax,deltay,MMode);
        }
        else {
          //STrace("calling target\n");
          if (MTarget) MTarget->do_widgetSizer(/*this*/MTarget,deltax,deltay,MMode);
          //else self.do_sizer(self,deltax,deltay,FMode);
        }
        prevx = AXpos;
        prevy = AYpos;
      } // dragging
      //axWidget::doMouseMove(aXpos,aYpos,aButton);
    }

};

//----------------------------------------------------------------------
#endif

