#ifndef s3_tilemap_included
#define s3_tilemap_included
//----------------------------------------------------------------------

#include "common/s3_rect.h"
#include "gui/s3_drawable.h"

//----------

class S3_TileMap {

  private:

    S3_Drawable*  MDrawable;
    S3_Rect       MRect;
    uint32        MXcount;
    uint32        MYcount;
    uint32        MTileWidth;
    uint32        MTileHeight;
    uint32        MNumTiles;

  public:

    S3_TileMap(S3_Drawable* ADrawable, S3_Rect ARect, uint32 AXcount, uint32 AYcount) {
      MDrawable   = ADrawable;
      MRect       = ARect;
      MXcount     = AXcount;
      MYcount     = AYcount;
      MTileWidth  = ARect.w / AXcount;
      MTileHeight = ARect.h / AYcount;
      MNumTiles   = MXcount * MYcount;
      //STrace("tilewidth %i tileheight %i\n",MTileWidth,MTileHeight);
    }

    //----------

    virtual ~S3_TileMap() {
    }

    //--------------------------------------------------

    S3_Drawable*  drawable(void)  { return MDrawable; }
    uint32        numTiles(void)  { return MNumTiles; }

    //--------------------------------------------------

    S3_Rect getTileRect(uint32 AIndex) {
      uint32 x = MRect.x + ( (AIndex % MXcount) * MTileWidth );
      uint32 y = MRect.y + ( (AIndex / MXcount) * MTileHeight );
      uint32 w = (MTileWidth-1);
      uint32 h = (MTileHeight-1);
      //STrace("> getTileRect: %i, %i, %i, %i\n",x,y,w,h);
      return S3_Rect(x,y,w,h);
    }


      /*
      float s;
      int32 x,y,w,h,tilecount,num;
      switch(MOrientatATileOrientation) {
        case sto_vertical:
          tilecount = ACount;//ASurface->height() / ASurface->width();
          s = (float)tilecount * AValue;
          num = SMinI( tilecount-1, floorf(s) );
          //STrace("num %i\n",num);
          w = ASurface->width();
          h = ASurface->height() / ACount; // ASurface->width();
          //STrace("h %i\n",h);
          x = 0;
          y = h*num;
          break;
        case sto_horizontal:
          tilecount = ACount;//ASurface->width() / ASurface->height();
          s = (float)tilecount * AValue;
          num = SMinI( tilecount-1, floorf(s) );
          //STrace("num %i\n",num);
          w = ASurface->width() / ACount;
          h = ASurface->height();
          x = w*num;
          y = 0;
          break;
      } // switch
      //blendSurface(ADstX,ADstY,ASurface,x,y,x+w-1,y+h-1);
      blendSurface(ADstX,ADstY,ASurface,x,y,w,h);
      */

};

//----------------------------------------------------------------------
#endif

