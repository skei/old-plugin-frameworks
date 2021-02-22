#ifndef s3_widget_scrollbar_included
#define s3_widget_scrollbar_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"

//----------

class S3_Widget_ScrollBar
: public S3_Widget {

  private:
    bool      MIsDragging;
    float     MPixelSize;
    int32     MClickX;
    int32     MClickY;
    float     MClickValue;

  protected:

    S3_Color  MBackColor;
    S3_Color  MThumbColor;
    float     MThumbSize;   // 0..1

  public:

    void thumbsize(float ASize) { MThumbSize = ASize; };

  public:

    S3_Widget_ScrollBar(S3_Rect ARect, float AValue, bool AVertical=false, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_ScrollBar";
      MHint = "scrollbar";
      MIsDragging = false;
      MThumbSize = 0.4;
      MValue = AValue;
      if (AVertical) {
        setFlag(s3_wf_vertical); // setFlag(MFlags,swf_vertical);
        MCursor = s3_mc_arrowUpDown;
      }
      else {
        clearFlag(s3_wf_vertical); // clearFlag(MFlags, swf_vertical);
        MCursor = s3_mc_arrowLeftRight;
      }
      MBackColor  = S3_Grey;
      MThumbColor = S3_DarkGrey;
    }

    virtual ~S3_Widget_ScrollBar() {
    }

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {

      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect);

      //APainter->setDrawColor(MBorderColor);
      //APainter->drawRect(MRect.x,MRect.y,MRect.x2,MRect.y2);
      if (hasFlag(s3_wf_vertical)) {
        float r = (float)MRect.h;
        //KTrace("draw vertical");
        int32 thumb = floorf( r * MThumbSize );
        int32 ipos  = floorf( (r-thumb) * MValue );
        int32 x = MRect.x;
        int32 y = MRect.y + ipos;
        int32 x2 = MRect.x2();
        int32 y2 = y + thumb;
        int32 w = (x2-x-0);
        int32 h = (y2-y-1);

        APainter->setFillColor(MThumbColor);
        APainter->fillRectangle(x,y,x+w,y+h);
        //APainter->setDrawColor(MBorderColor);
        //APainter->drawRect(x,y,x+w,y+h);
      }
      else {
        // horizontal
        float r = (float)MRect.w;
        int32 thumb = floorf( r * MThumbSize );
        int32 ipos  = floorf( (r-thumb) * MValue );
        int32 x = MRect.x + ipos;
        int32 y = MRect.y;
        int32 x2 = x + thumb;
        int32 y2 = MRect.y2();
        int32 w = (x2-x-1);
        int32 h = (y2-y+0);
        APainter->setFillColor(MThumbColor);
        APainter->fillRectangle(x,y,x+w,y+h);
        //APainter->setDrawColor(MBorderColor);
        //APainter->drawRect(x,y,x+w,y+h);
      }

    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == s3_mb_left) {
        MIsDragging = true;
        int32 thumbsize, numpixels;
        if (hasFlag(s3_wf_vertical)) {
          //STrace("click vertical\n");
          thumbsize = floorf( (float)MRect.h * MThumbSize );
          numpixels = MRect.h - thumbsize;
        }
        else {
          thumbsize = floorf( (float)MRect.w * MThumbSize );
          numpixels = MRect.w - thumbsize;
        }
        if (numpixels <= 0) numpixels = 1;  // HACK
        MPixelSize = 1.0 / (float)numpixels;
        MClickX = AXpos;
        MClickY = AYpos;
        MClickValue = MValue;
      }
    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (AButton == s3_mb_left) MIsDragging = false;
    }

    //----------

    /*
      TODO:
      - if x pixels away from scrollbar rect, set to previous value
    */

    virtual // S3_WidgetListener
    void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {
      if (MIsDragging) {

        if (hasFlag(s3_wf_vertical)) {
          //if ((AXpos >= MRect.x2()+20) || (AXpos <= MRect.x-20)) MValue = MClickValue;
          //else {
            if ((AYpos >= MRect.y) && (AYpos <= MRect.y2())) {
              int32 deltay = AYpos - MClickY;
              MValue += ( (float)deltay * MPixelSize );
            }
          //}
        }
        else {
          //if ((AYpos >= MRect.y2()+20) || (AYpos <= MRect.y-20)) MValue = MClickValue;
          //else {
            if ((AXpos >= MRect.x) && (AXpos <= MRect.x2())) {
              int32 deltax = AXpos - MClickX;
              MValue += ( (float)deltax * MPixelSize );
            }
          //}
        }
        MValue = S3_Clamp(MValue,0,1);

        do_widgetUpdate(this);
        do_widgetRedraw(this,MRect,s3_pm_interact);
        MClickX = AXpos;
        MClickY = AYpos;
      }
    }

    //----------

};

//----------------------------------------------------------------------
#endif

