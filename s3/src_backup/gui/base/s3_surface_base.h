#ifndef s3_surface_base_included
#define s3_surface_base_included
//----------------------------------------------------------------------

//#include "gui/s3_paint_source.h"
//#include "gui/s3_paint_target.h"

class S3_Surface_Base {
  public:
    S3_Surface_Base(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight) {}
    S3_Surface_Base(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth) {}
    virtual ~S3_Surface_Base() {}
};

//----------------------------------------------------------------------
#endif
