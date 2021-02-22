
//#define KODE_DEBUG
//#define KODE_PLUGIN

//#define KODE_GUI_CAIRO

#include "base/kode.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_image.h"
#include "gui/kode_window.h"
#include "gui/kode_widgets.h"

//#include "gui/ctx/kode_ctx_painter.h"
//#include "gui/software/kode_software_painter.h"

#include "gfx/kode_rasterizer.h"

//----------------------------------------------------------------------

int main(void) {
  KODE_Window*  window  = KODE_New KODE_Window(640,480);
  window->open();
  window->eventLoop();
  window->close();
  KODE_Delete window;
}
