/*
  xlib,  no buffer  = ok
  xlib,  buffer     = ok
  cairo, no buffer  = ok
  cairo, buffer     = no initial buffer painting

  cairo + backbuffer
    no initial blit :-/
    works after window resize

  cairo uses window as surface.. can we create the surface/painter
  for a window that hasn't been opened yet?
*/


//----------------------------------------------------------------------

#define S3_DEBUG_MEM
//#define S3_DEBUG_XLIB
//#define S3_DEBUG_CALLSTACK
//#define S3_DEBUG_CRASHHANDLER

//#define S3_GUI_XLIB
//#define S3_XRENDER
#define S3_GUI_CAIRO
//#define S3_NO_WINDOW_BACKBUFFER

//----------------------------------------------------------------------

#include "s3.h"
#include "gfx/s3_bitmap.h"
#include "gui/s3_image.h"
#include "gui/s3_surface.h"
#include "gui/s3_painter.h"
#include "gui/s3_window.h"

#include "gui/widgets/s3_widgets.h"
//#include "gui/widgets/s3_widget_color.h"

//----------------------------------------------------------------------
// widget
//----------------------------------------------------------------------

class myWidget : public S3_Widget {

  friend int main(void);

  private:
    S3_Bitmap*  MBitmap;
    S3_Image*   MImage;

  public:

    S3_WIDGET_CONSTRUCTOR(myWidget)

    //myWidget(S3_Rect ARect, uint32 AAlignment) : S3_Widget (ARect,AAlignment) {
    //}
    //
    //virtual ~myWidget() {
    //  delete MImage;
    //}

  public:

    void on_widget_paint(S3_Painter* APainter, S3_Rect ARect, uint32 AMode) final {
      APainter->setPenSize(1);
      //APainter->setFillColor(S3_DarkGreen);
      //APainter->fillRectangle(MRect);
      APainter->setDrawColor(S3_Black);
      APainter->drawLine(MRect);
      APainter->setFillColor(S3_Red);
      APainter->fillArc(MRect,0.6,0.8);
      MRect.shrink(20);
      APainter->setDrawColor(S3_White);
      APainter->drawArc(MRect,0.58,0.84);
      APainter->drawBitmap(50,50,MImage,0,0,128,128);
      MRect.scale(0.5, 0.9);
      APainter->setDrawColor(S3_BrightYellow);
      APainter->drawArc(MRect,0.58,0.84);
      APainter->setDrawColor(S3_White);
      APainter->drawText(MRect,"hello world!",s3_ta_center);
      APainter->setDrawColor(S3_Black);
      APainter->drawRoundedRectangle(S3_Rect(10,10,300,75),20);
      APainter->setFillColor( S3_Color(1,1,1,0.5 ) );
      APainter->fillRectangle( S3_Rect(200,200, 100,100) );
      //APainter->setDrawColor( S3_Color(1,0,0,1 ) );
      APainter->fillGradient( S3_Rect(200,300, 100,100), S3_Black, S3_White );
      //APainter->setFillColor( S3_Color(0,1,1,0.7 ) );
      APainter->setFillColor( S3_Color(0,1,1,0.5) );
      APainter->fillTriangle(200,10,390,110,310,200);
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

int main(void) {

  S3_Window* window = new S3_Window( S3_Rect(640,480) );
  window->setTitle("hello world!");

    //myWidget* widget = new myWidget( S3_Rect(10,10,100,50), s3_wa_fillClient );
    S3_Widget* panel = window->appendWidgetW( new S3_Widget_Color(S3_Rect(10,10,100,50), S3_Grey, s3_wa_fillClient) );

    S3_Image* image = new S3_Image(window,128,128,window->depth());
    uint8* buffer = (uint8*)image->buffer();
    //uint32* buffer = (uint32*)image->buffer();
    for (uint32 y=0; y<128; y++) {
      for (uint32 x=0; x<128; x++) {
        *buffer++ = x*2; // b
        *buffer++ = y*2; // g
        *buffer++ = 0; // r
        *buffer++ = 128; // a
        // *buffer++ = S3_BGR(x,y,0,0);
        // *buffer++ = (uint32)( x + (y<<8) );
      }
    }

    myWidget* widget = new myWidget( S3_Rect(10,10,100,50), s3_wa_fillClient );
    widget->MImage = image;
    panel->appendWidget(widget);

    panel->appendWidget( new S3_Widget_DragValue( S3_Rect(0,0,100,20), 0.5 ));

  window->on_widget_align();
  #ifndef S3_NO_WINDOW_BACKBUFFER
    window->on_buffer_paint( window->rect(), s3_pm_normal);
  #endif

  window->open();
  window->setPos(300,200);
  window->eventLoop();
  window->close();

    delete image;

  delete window;
  return 0;
}
