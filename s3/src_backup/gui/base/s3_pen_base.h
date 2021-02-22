#ifndef s3_pen_base_included
#define s3_pen_base_included
//----------------------------------------------------------------------

#include "common/s3_color.h"

class S3_Pen_Base {
  public:
    S3_Pen_Base() {}
    S3_Pen_Base(S3_Color AColor, int32 AWidth=S3_PAINTER_DEFAULT_PEN_WIDTH) {}
    virtual ~S3_Pen_Base() {}
};

//----------------------------------------------------------------------
#endif
