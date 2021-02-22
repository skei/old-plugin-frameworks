#ifndef s3_widget_agg_included
#define s3_widget_agg_included
//----------------------------------------------------------------------

#if 0

//#include "s3_agg_lite.h"
#include "s3_color.h"
#include "s3_widget.h"
#include "extern/agg2_lite/agg.h"
#include "extern/agg2_lite/agg.cpp.h"

#include "s3_painter_agg.h"
#include "s3_surface_agg.h"

//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>

//----------

class S3_Widget_Agg
: public S3_Widget {

  private:

    S3_Image* MImage;
    uint8*  MBuffer;
    int32   MWidth;
    int32   MHeight;
    int32   MStride;

    S3_Surface_Agg*  MSurface;
    S3_Painter_Agg*  MPainter;

    //agg::rendering_buffer*            rbuf;
    //agg::renderer<agg::span_bgra32>*  ren;
    //agg::rasterizer*                  ras;

  public:

    S3_Widget_Agg(S3_Rect ARect, S3_Image* AImage, uint32 AAlignment=swa_none)
    : S3_Widget(ARect,AAlignment) {
      MImage = AImage;
      MName = "SWidget_Agg";

      MBuffer = (uint8*)MImage->buffer();
      MWidth = MImage->width();
      MHeight = MImage->height();
      MStride = MImage->width() * 4;//sizeof(uint32);

      //rbuf = new agg::rendering_buffer(MBuffer,MWidth,MHeight,MStride);
      //ren = new agg::renderer<agg::span_bgra32>(rbuf);
      //ras = new agg::rasterizer();

      MSurface = new S3_AggSurface(MBuffer,MWidth,MHeight,MStride);
      MPainter = new S3_AggPainter(MSurface);
    }

    virtual ~SWidget_Agg() {
      //delete rbuf;
      //delete ren;
      //delete ras;
      delete MSurface;
      delete MPainter;
    }

  public:

    /*
    virtual void on_reset(int32 AMode=0) {}
    virtual void on_setPos(int32 AXpos, int32 AYpos) {}
    virtual void on_setSize(int32 AWidth, int32 AHeight) {}
    virtual void on_scroll(int32 ADeltaX, int32 ADeltaY) {}
    //virtual void on_move(int32 AXdelta, int32 AYdelta) {}
    //virtual void on_resize(int32 AWidth, int32 AHeight) {}
    virtual void on_align(void) {}
    */

    virtual void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Widget::on_widgetPaint(S3_Widget* AWidget, MPainter,ARect,AMode);
    }

    /*
    virtual void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {}
    virtual void on_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_keyDown(int32 AKey, int32 AState) {}
    virtual void on_keyUp(int32 AKey, int32 AState) {}
    virtual void on_enter(SWidget* AWidget) {}
    virtual void on_leave(SWidget* AWidget) {}
    //virtual void on_timer(void) {}
    //virtual void on_idle(void) {}
    */

  public:

    /*
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

#endif // 0

//----------------------------------------------------------------------
#endif

