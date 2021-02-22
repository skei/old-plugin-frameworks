#ifndef s3_brush_base_included
#define s3_brush_base_included
//----------------------------------------------------------------------

#include "gui/s3_image.h"
#include "gui/s3_surface.h"

class S3_Brush_Base {
  public:
    S3_Brush_Base() {}
    S3_Brush_Base(S3_Color AColor) {}
    S3_Brush_Base(S3_Surface* ASurface) {}
    //S3_Brush_Base(S3_Image* AImage) {}
    virtual ~S3_Brush_Base() {}
};

//----------------------------------------------------------------------
#endif
