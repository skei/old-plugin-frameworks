#ifndef s3_surface_included
#define s3_surface_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #ifdef S3_GUI_CAIRO
    #include "gui/s3_surface_cairo.h"
    typedef S3_Surface_Cairo S3_Surface_Implementation;
  #else
    #include "gui/s3_surface_xlib.h"
    typedef S3_Surface_Xlib S3_Surface_Implementation;
  #endif
#endif

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Surface
: public S3_Surface_Implementation {
  public:
    S3_Surface(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=0);
    virtual ~S3_Surface();
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Surface::S3_Surface(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
: S3_Surface_Implementation(ADrawable,AWidth,AHeight,ADepth)
{
}

S3_Surface::~S3_Surface() {
}

//----------------------------------------------------------------------
#endif
