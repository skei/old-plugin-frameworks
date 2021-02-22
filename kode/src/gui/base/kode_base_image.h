#ifndef kode_base_image_included
#define kode_base_image_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

class KODE_BaseImage : public KODE_Drawable {

public:

  KODE_BaseImage(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32) {}
  
  KODE_BaseImage(KODE_Drawable* AOwner, KODE_Bitmap* ABitmap) {}
  
  #ifndef KODE_IMAGE_NO_DECODE
  KODE_BaseImage(KODE_Drawable* AOwner, const uint8_t* buffer, uint32_t length) {}
    #ifndef KODE_IMAGE_NO_FILE
    KODE_BaseImage(KODE_Drawable* AOwner, const char* AFilename) {}
    #endif
  #endif
  
  virtual ~KODE_BaseImage() {}
  
public:

  virtual void upload() {}
  
  virtual KODE_Bitmap* getBitmap()  { return nullptr; }
  
};


//----------------------------------------------------------------------
#endif




