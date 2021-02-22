#ifndef s3_widget_imagestrip_included
#define s3_widget_imagestrip_included
//----------------------------------------------------------------------

#include "common/s3_integer.h"
#include "gui/s3_tilemap.h"
#include "gui/widgets/s3_widget_dragvalue.h"

//----------

class S3_Widget_ImageStrip
: public S3_Widget_DragValue {

  protected:
    //SColor    MBarColor;
    //SColor    MClearColor;
    //SSurface* MSurface;
    //uint32    MCount;
    //uint32    MOrientation;
    S3_TileMap*   MTileMap;

  public:

    S3_Widget_ImageStrip(S3_Rect ARect, float AValue, S3_TileMap* ATileMap, /*SSurface* ASurface, uint32 ACount, uint32 AOrientation=sto_vertical,*/ uint32 AAlignment=s3_wa_none)
    : S3_Widget_DragValue(ARect,AValue,AAlignment) {
      MName = "S3_Widget_ImageStrip";
      MHint = "ImageStrip";
      MCursor = s3_mc_arrowUpDown;
      MTileMap = ATileMap;
    }

    virtual ~S3_Widget_ImageStrip() {
    }

  public:

    virtual // S3_WidgetListewner
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      //APainter->drawTile(MRect.x,MRect.y,MValue,MSurface,MCount,MOrientation);
      uint32 num = MTileMap->numTiles();
      float idx = (float)MValue * (float)num;
      uint32 index = S3_MinI( num-1, floorf(idx) );
      S3_Point pos = MRect.pos();
      S3_Drawable* drawable = MTileMap->drawable();
      S3_Rect rect = MTileMap->getTileRect(index);
      APainter->blendBitmap(pos,drawable,rect);
    }

};

//----------------------------------------------------------------------
#endif


