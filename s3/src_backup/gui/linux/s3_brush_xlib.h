#ifndef s3_brush_xlib_included
#define s3_brush_xlib_included
//----------------------------------------------------------------------

#include "gui/base/s3_brush_base.h"
//#include "gui/s3_image.h"
//#include "gui/s3_surface.h"

//

class S3_Brush_Xlib
: public S3_Brush_Base{

  private:
    S3_Color  MColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Brush_Xlib()
    : S3_Brush_Base() {
      MColor = S3_Black;
    }

    //----------

    S3_Brush_Xlib(S3_Color AColor)
    : S3_Brush_Base(AColor) {
      MColor = AColor;
    }

    //----------

    S3_Brush_Xlib(S3_Surface* ASurface)
    : S3_Brush_Base(ASurface) {
    }

    //----------

    //S3_Brush_Xlib(S3_Image* AImage) {
    //}

    //----------

    virtual ~S3_Brush_Xlib() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void      color(S3_Color AColor)  { MColor = AColor; }
    S3_Color  color(void)             { return MColor; }

  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
#endif
