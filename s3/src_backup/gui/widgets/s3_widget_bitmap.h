#ifndef s3_widget_bitmap_included
#define s3_widget_bitmap_included
//----------------------------------------------------------------------

#include "gui/s3_drawable.h"
#include "gui/s3_widget.h"

//----------

//enum EBitmapMode {
//  s3_bm_draw    = 0,
//  s3_bm_blend   = 1,
//  s3_bm_stretch = 2
//};

//----------

class S3_Widget_Bitmap
: public S3_Widget {

  private:
    S3_Drawable* MBitmap;
    uint32          MMode;

  public:

    S3_Widget_Bitmap(S3_Rect ARect, S3_Drawable* ABitmap, uint32 AMode=s3_bm_draw, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_Bitmap";
      MHint = "bitmap";
      setFlag(s3_wf_opaque);
      MBitmap = ABitmap;
      MMode = AMode;
    }

    virtual ~S3_Widget_Bitmap() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void bitmap(S3_Drawable* ABitmap) { MBitmap = ABitmap; }
    void mode(uint32 AMode) { MMode = AMode; }

  //------------------------------
  //
  //------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      if (MBitmap) {
        switch(MMode) {
          case s3_bm_draw:
            APainter->drawBitmap(MRect.x,MRect.y,MBitmap,0,0,MRect.w,MRect.h );
            break;
          case s3_bm_blend:
            APainter->blendBitmap(MRect.x,MRect.y,MBitmap,0,0,MRect.w,MRect.h );
            break;
          case s3_bm_stretch:
            APainter->stretchBitmap(MRect.x,MRect.y,MRect.w,MRect.h,MBitmap,0,0,MBitmap->width(),MBitmap->height() );
            break;
        } // switch
      }
    }

    //----------

};

//----------------------------------------------------------------------
#endif
