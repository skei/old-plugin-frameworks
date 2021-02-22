
//#define KODE_GUI_CAIRO

#include "base/kode.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

int main(void) {
  KODE_Window* window = KODE_New KODE_Window(640,480);
  window->setTitle("KODE_SdlWindow");
  
  window->setPadding( 10,10,50,10 );
  window->setSpacing( 5,20 );
  
  KODE_Widget* w;
  
  // bottom
  
  w = window->appendWidget( KODE_New KODE_BorderWidget( KODE_Rect(100) ));
  w->setAlignment(KODE_WIDGET_ALIGN_BOTTOM_FILL);
  
  // left
  
  w = window->appendWidget( KODE_New KODE_BorderWidget( KODE_Rect(100) ));
  w->setAlignment(KODE_WIDGET_ALIGN_LEFT_FILL);
  
  // client
  
  w = window->appendWidget( KODE_New KODE_BorderWidget( KODE_Rect() ));
  {
    w->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
    w->setPadding(50,50,0,0);
    w->setSpacing(10,5);
    KODE_Widget* w2;
    for (uint32_t i=0; i<10; i++) {
      w2 = w->appendWidget( KODE_New KODE_ColorWidget( KODE_Rect(20), KODE_Red));
      w2->setAlignment(KODE_WIDGET_ALIGN_STACK_HORIZ);
    }
  }
  
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
}