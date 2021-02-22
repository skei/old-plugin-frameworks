#ifndef s3_image_cairo_included
#define s3_image_cairo_included
//----------------------------------------------------------------------

#include "gui/s3_image_base.h"
#include "gfx/s3_bitmap.h"
#include "gui/s3_drawable.h"

//----------

class S3_Image_Cairo
: public S3_Image_Base
, public S3_Drawable {

  private:
    //Display*          MDisplay; // from drawable
    //XImage*           MImage;
    cairo_surface_t*  MCairoSurface;

  private:
    int32       MWidth;
    int32       MHeight;
    int32       MDepth;
    //int32     MStride;
    uint32*     MBuffer;


  //------------------------------
  //
  //------------------------------

  public:

    S3_Image_Cairo(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=32)
    : S3_Image_Base(ADrawable,AWidth,AHeight,ADepth) {
      //MDisplay = ADrawable->display();
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = ADepth;
      cairo_format_t format;
      switch (MDepth) {
        case 1:   format = CAIRO_FORMAT_A1;         break;
        case 8:   format = CAIRO_FORMAT_A8;         break;
        case 16:  format = CAIRO_FORMAT_RGB16_565;  break;
        case 24:  format = CAIRO_FORMAT_RGB24;      break;
        case 32:  format = CAIRO_FORMAT_ARGB32;     break;
        default:  format = CAIRO_FORMAT_ARGB32;     break;
      }
      MCairoSurface = cairo_image_surface_create(format,AWidth,AHeight);
      MBuffer = (uint32*)cairo_image_surface_get_data(MCairoSurface);
    }

    //----------

    virtual ~S3_Image_Cairo() {
      cairo_surface_destroy(MCairoSurface);
    }

  //------------------------------
  // S3_Drawable
  //------------------------------

  public:

    //bool              isImage(void) override      { return true; }
    //XImage*           image(void)   override      { return MImage; }
    int32             width(void)   override      { return MWidth; }
    int32             height(void)  override      { return MHeight; }
    int32             depth(void)   override      { return MDepth; }
    void*             buffer(void)  override      { return MBuffer; }
    #ifdef S3_CAIRO
    bool              isCairo(void) override      { return true; }
    cairo_surface_t*  cairoSurface(void)          { return MCairoSurface; }
    #endif

};

//----------------------------------------------------------------------
#endif

