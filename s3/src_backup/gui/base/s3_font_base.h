#ifndef s3_font_base_included
#define s3_font_base_included
//----------------------------------------------------------------------

#include "gui/s3_drawable.h"

class S3_Font_Base {
  public:
    S3_Font_Base(S3_Drawable* ADrawable) {}
    S3_Font_Base(S3_Drawable* ADrawable, const char* AName, int32 ASize, S3_Color AColor) {}
    virtual ~S3_Font_Base() {}
};

//----------------------------------------------------------------------
#endif
