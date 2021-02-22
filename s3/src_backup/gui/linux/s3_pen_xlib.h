#ifndef s3_pen_xlib_included
#define s3_pen_xlib_included
//----------------------------------------------------------------------

#include "gui/base/s3_pen_base.h"

//----------

class S3_Pen_Xlib
: public S3_Pen_Base {

  private:
    S3_Color  MColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Pen_Xlib()
    : S3_Pen_Base() {
      MColor    = S3_Black;
    }

    //----------

    S3_Pen_Xlib(S3_Color AColor, int32 AWidth=S3_PAINTER_DEFAULT_PEN_WIDTH)
    : S3_Pen_Base(AColor,AWidth) {
      MColor    = AColor;
    }

    //----------

    virtual ~S3_Pen_Xlib() {
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
