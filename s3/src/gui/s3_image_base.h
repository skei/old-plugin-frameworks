#ifndef s3_image_base_included
#define s3_image_base_included
//----------------------------------------------------------------------

#include "gui/s3_drawable.h"

class S3_Image_Base {
  public:
    S3_Image_Base(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=32) {}
    virtual ~S3_Image_Base() {}
};

//----------------------------------------------------------------------
#endif
