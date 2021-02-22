#ifndef kode_xcb_image_included
#define kode_xcb_image_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_drawable.h"
//#include "gui/kode_image.h"
#include "gui/base/kode_base_image.h"
#include "gui/xcb/kode_xcb.h"

#ifdef KODE_CAIRO
#include "gui/cairo/kode_cairo.h"
#endif

//----------------------------------------------------------------------

class KODE_XcbImage
: public KODE_BaseImage {

//------------------------------
private:
//------------------------------

  xcb_connection_t* MConnection   = nullptr;
  xcb_visualid_t    MVisual       = XCB_NONE;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  xcb_image_t*      MImage        = nullptr;
  KODE_Bitmap*      MBitmap       = nullptr;
  bool              MAllocated    = false;

  #ifdef KODE_CAIRO
  cairo_surface_t*  MCairoSurface = nullptr;
  #endif
  
  uint32_t MWidth = 0;
  uint32_t MHeight = 0;

//------------------------------
private:
//------------------------------

  /*
    //http://tronche.com/gui/x/xlib/utilities/XCreateImage.html
    does not allocate space for the image itself
    initializes the structure byte-order, bit-order, and bitmap-unit values from the display

    //http://www.sbin.org/doc/Xlib/chapt_06.html
    XCreateImage()
    Allocates memory for an XImage structure and sets various members.
    Note that it uses the server's data format, which is often not appropriate.
    The byte- and bit-order fields should usually be changed directly to the client-native format.
    However, then the call _XInitImageFuncPtrs(image) should be issued to reset the mapping
    to the appropriate versions of the functions for manipulating the image.
    This call is supposed to be private to Xlib and, therefore, should be watched for changes
    in later releases, but this is currently the accepted method.

    also called by KODE_CairoImage
  */

  void _create_xcb_image(KODE_Drawable* AOwner) {
    MConnection = AOwner->getConnection();
    MVisual     = AOwner->getVisual();
    MDrawable   = AOwner->getDrawable();
    
    MWidth = MBitmap->getWidth();
    MHeight = MBitmap->getHeight();
    
    MImage      = xcb_image_create(
      MWidth,//MBitmap->getWidth(),            // width      width in pixels.
      MHeight,//MBitmap->getHeight(),           // height     height in pixels.
      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
      32,                             // xpad       scanline pad (8,16,32)
      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
      32,                             // unit       unit of image representation, in bits (8,16,32)
      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
      MBitmap->getBuffer(),           // base       The base address of malloced image data
      MBitmap->getBufferSize(),       // bytes      The size in bytes of the storage pointed to by base.
                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
      (uint8_t*)MBitmap->getBuffer()  // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                      //            by mallocing sufficient storage and filling in base.
    );
    xcb_flush(MConnection);
  }

  //----------

  /*
    //http://www.sbin.org/doc/Xlib/chapt_06.html
    XDestroyImage()
    Frees the data field in an image structure if the image structure was
    allocated in the application. If the image was created using
    XCreateImage(), XGetImage(), or XGetSubImage(), XDestroyImage() frees
    both the data and the image structure. Note that if the image data is
    stored in static memory in the application, it cannot be freed.
    To free an image created with XCreateImage() that has statically
    allocated data, you must set NULL into the data field before calling
    XDestroyImage().
  */

  void _destroy_xcb_image() {
    //MImage->data = nullptr; // crash
    MImage->base = nullptr;
    xcb_image_destroy(MImage);
  }

//------------------------------
public:
//------------------------------

  KODE_XcbImage(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32)
  : KODE_BaseImage(AOwner, AWidth,AHeight,ADepth) {
    MBitmap = KODE_New KODE_Bitmap(AWidth,AHeight);
    MAllocated = true;
    _create_xcb_image(AOwner);
    #ifdef KODE_CAIRO
    MCairoSurface = createCairoSurface();
    #endif
  }

  //----------

  KODE_XcbImage(KODE_Drawable* AOwner, KODE_Bitmap* ABitmap)
  : KODE_BaseImage(AOwner,ABitmap) {
    MBitmap     = ABitmap;
    MAllocated  = false;
    _create_xcb_image(AOwner);
    #ifdef KODE_CAIRO
    MCairoSurface = createCairoSurface();
    #endif
  }

  //----------

  #ifndef KODE_IMAGE_NO_DECODE

    KODE_XcbImage(KODE_Drawable* AOwner, const uint8_t* buffer, uint32_t length)
    : KODE_BaseImage(AOwner,buffer,length) {
      MBitmap     = KODE_New KODE_Bitmap(buffer,length);
      //MBitmap->convertRgbaBgra();
      MAllocated  = true;
      _create_xcb_image(AOwner);
      #ifdef KODE_CAIRO
      MCairoSurface = createCairoSurface();
      #endif
    }

  #endif

  //----------

  #ifndef KODE_IMAGE_NO_DECODE
    #ifndef KODE_IMAGE_NO_FILE

      KODE_XcbImage(KODE_Drawable* AOwner, const char* AFilename)
      : KODE_BaseImage(AOwner,AFilename) {
        MBitmap     = KODE_New KODE_Bitmap(AFilename);
        MAllocated  = true;
        _create_xcb_image(AOwner);
        #ifdef KODE_CAIRO
        MCairoSurface = createCairoSurface();
        #endif
      }

    #endif
  #endif

  //----------
  //----------

  virtual ~KODE_XcbImage() {
    #ifdef KODE_CAIRO
    cairo_surface_destroy(MCairoSurface);
    #endif
    _destroy_xcb_image();
    if (MAllocated && MBitmap) KODE_Delete MBitmap;
  }

//------------------------------
public: // drawable
//------------------------------

  bool              isImage()             override { return true; }
  xcb_image_t*      getImage()            override { return MImage; }
  uint32_t          getWidth()            override { return MWidth; }
  uint32_t          getHeight()           override { return MHeight; }
  #ifdef KODE_CAIRO
  bool              isCairo()             override { return true; }
  cairo_surface_t*  getCairoSurface(void) override { return MCairoSurface; }
  #endif

//------------------------------
public:
//------------------------------

  /*
    Creates an image surface for the provided pixel data. The output buffer
    must be kept around until the cairo_surface_t is destroyed or
    cairo_surface_finish() is called on the surface. The initial contents of
    data will be used as the initial image contents; you must explicitly clear
    the buffer, using, for example, cairo_rectangle() and cairo_fill() if you
    want it cleared.

    Note that the stride may be larger than width*bytes_per_pixel to provide
    proper alignment for each pixel and row. This alignment is required to
    allow high-performance rendering within cairo. The correct way to obtain a
    legal stride value is to call cairo_format_stride_for_width() with the
    desired format and maximum image width value, and then use the resulting
    stride value to allocate the data and to create the image surface. See
    cairo_format_stride_for_width() for example code.
  */

  #ifdef KODE_CAIRO
  cairo_surface_t* createCairoSurface(void) {
    cairo_surface_t* surface = cairo_image_surface_create_for_data(
      (unsigned char*)MBitmap->getBuffer(), // unsigned char *data,
      CAIRO_FORMAT_ARGB32,                  // cairo_format_t format,
      MBitmap->getWidth(),                  // int width,
      MBitmap->getHeight(),                 // int height,
      MBitmap->getStride()                  // int stride);
    );
    //check_cairo_surface_errors(surface);
    return surface;
  }
  #endif

  //----------

  /*
    xcb_image_annotate:
    Update the cached data of an image. An image's size and stride, among
    other things, are cached in its structure. This function recomputes those
    cached values for the given image.
  */

  //void update(void) override {
  //  xcb_image_annotate(MImage);
  //}

  //----------

  virtual void upload() {
  }

  KODE_Bitmap* getBitmap()  override {
    return MBitmap;
  }

};


//----------------------------------------------------------------------
#endif























/*
xcb_image_t* xcb_image_subimage(
  xcb_image_t*  image,
  ui32_t        x,
  ui32_t        y,
  ui32_t        width,
  ui32_t        height,
  void *        base,
  ui32_t        bytes,
  uint8_t*      data
);
*/

/*
https://stackoverflow.com/questions/39773763/xcb-shm-get-image-equivalent-in-xlib
https://stackoverflow.com/questions/27745131/how-to-use-shm-pixmap-with-xcb

//----------

https://github.com/etale-cohomology/xcb/blob/master/img.c

  #define W 256
  #define H 256
  #define C 4

  const xcb_setup_t* setup = xcb_get_setup(connection);
  uint8 buffer[W*H*C];  // Can be bigger than actual size, but not smaller!

  xcb_image_format_t  image_format  = XCB_IMAGE_FORMAT_Z_PIXMAP;
  uint8               spad          = 32;     // Valids: 8,16,32
  uint8               depth         = 24;     // Valids: 1,4,8,16,24    zpixmap, 1 xybitmap, anything xypixmap
  uint8               bpp           = 32;     // Valids: 1,4,8,16,24,32 zpixmap, 1 xybitmap, anything xypixmap
  uint8               unit          = 32;     // Valids: 8,16,32
  xcb_image_order_t   byte_order    = setup->image_byte_order;
  xcb_image_order_t   bit_order     = XCB_IMAGE_ORDER_LSB_FIRST;
  uint                size          = W*H*C;  // In bytes! Can be bigger than actual size, but not smaller!

  xcb_image_t* image = xcb_image_create(
    W,
    H,
    image_format,
    spad,
    depth,
    bpp,
    unit,
    byte_order,
    bit_order,
    buffer,
    size,
    buffer
  );

//----------

// https://github.com/nxsy/xcb_handmade/blob/master/src/xcb_handmade.cpp

xcb_image_t* hhxcb_create_image(ui32_t              pitch,
                                ui32_t              width,
                                ui32_t              height,
                                xcb_format_t*       fmt,
                                const xcb_setup_t*  setup) {
  size_t image_size = pitch * height;
  uint8_t *image_data = (uint8_t *)calloc(1, image_size);
  //uint8_t *image_data = (uint8_t *)KODE_Malloc(image_size);
  return xcb_image_create(
    width,
    height,
    XCB_IMAGE_FORMAT_Z_PIXMAP,
    fmt->scanline_pad,
    fmt->depth,
    fmt->bits_per_pixel,
    0,
    (xcb_image_order_t)setup->image_byte_order,
    XCB_IMAGE_ORDER_LSB_FIRST,
    image_data,
    image_size,
    image_data
  );
}

//----------

// http://vincentsanders.blogspot.no/2010/04/xcb-programming-is-hard.html

*/

/*
  xcb_shm_get_image
  // see also: xcb_image_create_native
*/
