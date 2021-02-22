#ifndef kode_base_surface_included
#define kode_base_surface_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_drawable.h"

//----------------------------------------------------------------------

class KODE_BaseSurface : public KODE_Drawable {
public:
  KODE_BaseSurface(KODE_Drawable* AOwner) {}
  KODE_BaseSurface(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0) {}
  virtual ~KODE_BaseSurface() {}
};

//----------------------------------------------------------------------
#endif

