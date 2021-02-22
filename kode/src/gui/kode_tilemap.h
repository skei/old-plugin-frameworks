#ifndef kode_tilemap_included
#define kode_tilemap_included
//----------------------------------------------------------------------

#include "base/kode_rect.h"
#include "gui/kode_drawable.h"

//----------

class KODE_TileMap {

  private:

    KODE_Drawable*  MDrawable   = nullptr;
    KODE_Rect       MRect       = {0};
    uint32_t        MXcount     = 0;
    uint32_t        MYcount     = 0;
    uint32_t        MTileWidth  = 0;
    uint32_t        MTileHeight = 0;
    uint32_t        MNumTiles   = 0;

  public:

    KODE_TileMap(KODE_Drawable* ADrawable, KODE_Rect ARect, uint32_t AXcount, uint32_t AYcount) {
      MDrawable   = ADrawable;
      MRect       = ARect;
      MXcount     = AXcount;
      MYcount     = AYcount;
      MTileWidth  = ARect.w / AXcount;
      MTileHeight = ARect.h / AYcount;
      MNumTiles   = MXcount * MYcount;
      //KODE_Trace("tilewidth %i tileheight %i\n",MTileWidth,MTileHeight);
    }

    //----------

    virtual ~KODE_TileMap() {
    }

    //--------------------------------------------------

    KODE_Drawable*  getDrawable(void)  { return MDrawable; }
    uint32_t        getNumTiles(void)  { return MNumTiles; }

    //--------------------------------------------------

    KODE_Rect getTileRect(uint32_t AIndex) {
      uint32_t x = MRect.x + ( (AIndex % MXcount) * MTileWidth );
      uint32_t y = MRect.y + ( (AIndex / MXcount) * MTileHeight );
      uint32_t w = (MTileWidth-1);
      uint32_t h = (MTileHeight-1);
      //KODE_Trace("> getTileRect: %i, %i, %i, %i\n",x,y,w,h);
      return KODE_Rect(x,y,w,h);
    }

};

//----------------------------------------------------------------------
#endif

