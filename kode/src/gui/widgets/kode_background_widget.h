#ifndef kode_background_widget_included
#define kode_background_widget_included
//----------------------------------------------------------------------

#include "base/kode_color.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_widget.h"

#define KODE_BACKGROUND_NONE    0
#define KODE_BACKGROUND_COLOR   1
#define KODE_BACKGROUND_BITMAP  2

class KODE_BackgroundWidget
: public KODE_Widget {

  protected:

    uint32_t      MMode   = KODE_BACKGROUND_NONE;
    KODE_Color    MColor  = KODE_Grey;
    KODE_Bitmap*  MBitmap = nullptr;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_BackgroundWidget(KODE_Rect ARect, KODE_Color AColor)
    : KODE_Widget(ARect) {
      MMode = KODE_BACKGROUND_COLOR;
      MColor = AColor;
    }

    KODE_BackgroundWidget(KODE_Rect ARect, KODE_Bitmap* ABitmap)
    : KODE_Widget(ARect) {
      MMode = KODE_BACKGROUND_BITMAP;
      MBitmap = ABitmap;
    }

    //----------

    virtual ~KODE_BackgroundWidget() {
    }

  //------------------------------
  // get, set
  //------------------------------

  public:

    virtual void          setMode(uint32_t AMode)         { MMode = AMode; }
    virtual void          setColor(KODE_Color AColor)     { MColor = AColor; }
    virtual void          setBitmap(KODE_Bitmap* ABitmap) { MBitmap = ABitmap; }

    virtual uint32_t      getMode(void)                   { return MMode; }
    virtual KODE_Color    getColor(void)                  { return MColor; }
    virtual KODE_Bitmap*  getBitmap(void)                 { return MBitmap; }

  //------------------------------
  // parent -> children
  //------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      switch(MMode) {
        case KODE_BACKGROUND_NONE:
          break;
        case KODE_BACKGROUND_COLOR:
          APainter->setFillColor( MColor );
          APainter->fillRectangle( ARect );
          break;
        case KODE_BACKGROUND_BITMAP:
          break;
      }
      paintChildren(APainter,ARect);
    }

};

//----------------------------------------------------------------------
#endif
