#ifndef kode_drawable_included
#define kode_drawable_included
//----------------------------------------------------------------------

#ifdef KODE_XCB
  #include "gui/xcb/kode_xcb.h"
#endif

#ifdef KODE_CAIRO
  #include "gui/cairo/kode_cairo.h"
#endif

#ifdef KODE_SDL
  #include "gui/sdl/kode_sdl.h"
#endif

//#include "gfx/kode_bitmap.h"

//----------------------------------------------------------------------

class KODE_Drawable {
public:
  virtual bool isWindow()   { return false; }
  virtual bool isSurface()  { return false; }
  virtual bool isImage()    { return false; }
  virtual bool isBitmap()   { return false; }
  virtual bool isCairo()    { return false; }
  virtual bool isSdl()      { return false; }
public:
  virtual uint32_t          getWidth()        { return 0; }
  virtual uint32_t          getHeight()       { return 0; }
  virtual uint32_t          getDepth()        { return 0; }
  #ifdef KODE_XCB
  virtual xcb_drawable_t    getDrawable()     { return XCB_NONE; }
  virtual xcb_image_t*      getImage()        { return nullptr; }
  virtual xcb_pixmap_t      getPixmap()       { return XCB_NONE; }
  virtual xcb_window_t      getWindow()       { return XCB_NONE; }
  virtual xcb_connection_t* getConnection()   { return nullptr; }
  virtual xcb_visualid_t    getVisual()       { return XCB_NONE; }
  #endif
  #ifdef KODE_CAIRO
  virtual cairo_surface_t*  getCairoSurface() { return nullptr; }
  #endif
  #ifdef KODE_SDL
  virtual SDL_Window*       getSdlWindow()    { return nullptr; }
  virtual SDL_Renderer*     getSdlRenderer()  { return nullptr; }
  virtual SDL_Texture*      getSdlTexture()   { return nullptr; }
  #endif
};

//----------------------------------------------------------------------
#endif
