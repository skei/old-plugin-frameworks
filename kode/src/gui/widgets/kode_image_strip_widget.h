#ifndef kode_widget_imagestrip_included
#define kode_widget_imagestrip_included
//----------------------------------------------------------------------

//#include "base/kode_integer.h"
#include "gui/kode_tilemap.h"
#include "gui/widgets/kode_drag_value_widget.h"

//----------

class KODE_ImageStripWidget
: public KODE_DragValueWidget {

  protected:
    //SColor    MBarColor;
    //SColor    MClearColor;
    //SSurface* MSurface;
    //uint32    MCount;
    //uint32    MOrientation;
    KODE_TileMap*   MTileMap;

  public:

    KODE_ImageStripWidget(KODE_Rect ARect, KODE_TileMap* ATileMap, float AValue=0.0f)
    : KODE_DragValueWidget(ARect,AValue) {
      MName = "KWidget_ImageStrip";
      MHint = "ImageStrip";
      MCursor = KODE_CURSOR_ARROWUPDOWN;
      MTileMap = ATileMap;
    }

    virtual ~KODE_ImageStripWidget() {
    }

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      //APainter->drawTile(MRect.x,MRect.y,MValue,MSurface,MCount,MOrientation);
      uint32_t num = MTileMap->getNumTiles();
      float idx = (float)getValue() * (float)num;
      uint32_t index = KODE_MinI( num-1, floorf(idx) );
      //KODE_Point pos = MRect.pos();
      KODE_Drawable* drawable = MTileMap->getDrawable();
      KODE_Rect rect = MTileMap->getTileRect(index);
      APainter->drawBitmap(MRect.x,MRect.y,drawable,rect);
    }

};

//----------------------------------------------------------------------
#endif


