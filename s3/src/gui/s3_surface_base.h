#ifndef s3_surface_base_included
#define s3_surface_base_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include "gui/s3_drawable.h"

class S3_Surface_Base {
  public:
    S3_Surface_Base(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=0);
    virtual ~S3_Surface_Base();
  public:
    virtual void setSize(uint32 AX, uint32 AY) {}
    virtual void flush(void) {}
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Surface_Base::S3_Surface_Base(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth) {
}

S3_Surface_Base::~S3_Surface_Base(void) {
}

//----------------------------------------------------------------------
#endif
