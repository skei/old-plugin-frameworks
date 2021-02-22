#ifndef s3_widget_circular_waveform_included
#define s3_widget_circular_waveform_included
//----------------------------------------------------------------------

#include "common/s3_integer.h"
#include "gui/s3_widget.h"

#define NUMSTEPS 1024


//----------

class S3_Widget_CircularWaveform
: public S3_Widget {

  private:

    float*    MBuffer;
    int32     MBufferSize;
    //float   MRadius;
    float     MOuterRadius;
    float     MInnerRadius;
    bool      MBipolar;
    S3_Color  MLineColor;

  public:

    S3_Widget_CircularWaveform(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName         = "S3_Widget_CircularWaveform";
      MHint         = "circular aveform";
      MBuffer       = S3_NULL;//(float*)SMalloc(NUMSTEPS*sizeof(float));
      MBufferSize   = 0;//1024;
      //MRadius       = 100.0f;
      MOuterRadius  = 1.0f;
      MInnerRadius  = 0.6f;
      MLineColor    = S3_LightGrey;
      MBipolar      = false;//true;
    }

    virtual ~S3_Widget_CircularWaveform() {
      //if (MBuffer) SFree(MBuffer);
    }

  public:

    float*  buffer(void)                        { return MBuffer; }
    float   buffer(int32 AIndex)                { return MBuffer[AIndex]; }
    void    buffer(int32 AIndex, float AValue)  { MBuffer[AIndex] = AValue; }
    void    buffer(float* ABuffer)              { MBuffer = ABuffer; }

    int32   buffersize(void)            { return MBufferSize; }
    void    buffersize(int32 ASize)     { MBufferSize = ASize; }

    //void    radius(float ARadius)       { MRadius = ARadius; }
    void    outerRadius(float ARadius)  { MOuterRadius = ARadius; }
    void    innerRadius(float ARadius)  { MInnerRadius = ARadius; }

    void    bipolar(bool ABipolar=true) { MBipolar = ABipolar; }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      int32 xcenter = MRect.x + (MRect.w / 2);
      int32 ycenter = MRect.y + (MRect.h / 2);

      float radius = S3_MinI(MRect.w,MRect.h);
      float r_range  = ( MOuterRadius - MInnerRadius );
      //float r_center = MInnerRadius + ( r_range / 2.0f );

      float angle = 0;
      float angleadd = 1.0f / NUMSTEPS;

      //float prevx = cosf(angle*S3_PI2) * r_center;
      //float prevy = sinf(angle*S3_PI2) * r_center;

      float vv;//  = 0.5f;
      float xx;//  = 1.0f;//cosf(0.0f);
      float yy;//  = 0.0f;//sinf(0.0f);
      float v   = MInnerRadius + (0.5f * r_range);
      float prevx = v * radius * 0.5f;
      float prevy = 0;

      APainter->setDrawColor(MLineColor);

      if (MBipolar) {
        for (int32 i=0; i<NUMSTEPS; i++) {
          vv = (MBuffer[i] + 1.0f) * 0.5f;      // 0..1
          v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
          xx = cosf(angle*S3_PI2);
          yy = sinf(angle*S3_PI2);
          float x = xx * v * radius * 0.5;
          float y = yy * v * radius * 0.5;
          APainter->drawLine(xcenter+prevx,ycenter+prevy,xcenter+x,ycenter+y);
          prevx = x;
          prevy = y;
          angle += angleadd;
        }
      }
      else {
        for (int32 i=0; i<NUMSTEPS; i++) {
          v  = MInnerRadius;
          xx = cosf(angle*S3_PI2);
          yy = sinf(angle*S3_PI2);
          float x1 = xx * v * radius * 0.5;
          float y1 = yy * v * radius * 0.5;
          vv = MBuffer[i];      // 0..1
          v  = MInnerRadius + (vv * r_range);   // 0.5 + (vv*0.5) = 0.5+0..0.5+0.5 = 0.5..1
          xx = cosf(angle*S3_PI2);
          yy = sinf(angle*S3_PI2);
          float x2 = xx * v * radius * 0.5;
          float y2 = yy * v * radius * 0.5;

          APainter->drawLine(xcenter+x1,ycenter+y1,xcenter+x2,ycenter+y2);

          //APainter->drawPoint(xcenter+x1,ycenter+y1);


          //prevx = x;
          //prevy = y;
          angle += angleadd;
        }
      }
    }


  /*

  public:

    virtual void on_reset(int32 AMode=0) {}
    virtual void on_setPos(int32 AXpos, int32 AYpos) {}
    virtual void on_setSize(int32 AWidth, int32 AHeight) {}
    virtual void on_scroll(int32 ADeltaX, int32 ADeltaY) {}
    //virtual void on_move(int32 AXdelta, int32 AYdelta) {}
    //virtual void on_resize(int32 AWidth, int32 AHeight) {}
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
    //virtual void on_timer(void) {}
    //virtual void on_idle(void) {}

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


#undef NUMSTEPS

//----------------------------------------------------------------------
#endif

