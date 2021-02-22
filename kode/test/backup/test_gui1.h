
#define KODE_DEBUG
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
  KODE_Image*   image   = KODE_New KODE_Image(window,"img/skei.png");
  //KODE_Image*   image   = KODE_New KODE_Image(window,256,256);
  //bitmap->fill(0);
  KODE_Bitmap*  bitmap  = image->getBitmap();

  /*
  KODE_SoftwarePainter* painter = new KODE_SoftwarePainter(bitmap);
  painter->setDrawColor(KODE_White);
  painter->_line_br(  10,10, 50, 100 );
  painter->_lineAA_br(20,10, 60, 100 );
  painter->_lineAA_wu(30,10, 70, 100 );
  //painter->_lineAA_gs(40,10, 80, 100 );
  painter->_ellipseAA_wu(100,100,150,170);
  KODE_Delete painter;
  */
  
  //----------
  
  /*
  stbtt_vertex vertices[] = {
    { 64,  0,   0,0,0,0, STBTT_vmove, 0 },
    { 128,128,  0,0,0,0, STBTT_vline, 0 },
    { 0,  128,  0,0,0,0, STBTT_vline, 0 },
    { 64,  0,   0,0,0,0, STBTT_vline, 0 }
  };
  stbtt__bitmap result;// = {0};
  result.w      = bitmap->getWidth();
  result.h      = bitmap->getHeight();
  result.stride = bitmap->getStride();
  result.pixels = bitmap->getBuffer();
  void* userdata = KODE_NULL;
  stbtt_Rasterize(
    &result,
    1.0f,     //float flatness_in_pixels,
    vertices,
    4,        // num_verts,
    1.0f,     // float scale_x,
    1.0f,     // float scale_y,
    0.0f,     // float shift_x,
    0.0f,     // float shift_y,
    0,        // int x_off,
    0,        // int y_off,
    0,        // int invert,
    userdata
  );
  */
  
  //----------
  
  
  
  KODE_ImageWidget* image_widget  = KODE_New KODE_ImageWidget( KODE_Rect(10,10,256,256), image ) ;
  window->appendWidget( image_widget );

  //window->realignChildren();
  window->open();
  window->eventLoop();
  window->close();
  
  KODE_Delete image;
  KODE_Delete window;
  
}
