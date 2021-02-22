#ifndef s3_widget_symbol_included
#define s3_widget_symbol_included
//----------------------------------------------------------------------

//#include "gui/widgets/s3_widget_button.h"
//#include "gui/widgets/s3_widget_menu.h"
//#include "gui/widgets/s3_widget_menuitem.h"

//----------

class S3_Widget_Symbol
: public S3_Widget {

  private:

    int32     MSymbol;
    S3_Color  MColor;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Widget_Symbol(S3_Rect ARect, uint32 ASymbol, S3_Color AColor, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName   = "S3_Widget_Symbol";
      MHint   = "symbol";
      MSymbol = ASymbol;
      MColor  = AColor;
    }

    //----------

    virtual ~S3_Widget_Symbol() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void symbol(uint32 ASymbol) { MSymbol = ASymbol; }
    void color(S3_Color AColor) { MColor = AColor; }

  //------------------------------
  //
  //------------------------------

  public:

    virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode) {
      switch(MSymbol) {
        case 0:
          break;
        case 1: // rectangle
          APainter->setDrawColor(MColor);
          APainter->drawRectangle(MRect);
          break;
        case 2: // rectangle
          APainter->setFillColor(MColor);
          APainter->fillRectangle(MRect);
          break;
        case 3: // triangle (down)
          APainter->setDrawColor(MColor);
          APainter->drawTriangle( MRect.x, MRect.y, MRect.x2(), MRect.y, MRect.x + (MRect.w/2), MRect.y2() );
          break;
        case 4: // triangle (down)
          APainter->setFillColor(MColor);
          APainter->fillTriangle( MRect.x, MRect.y, MRect.x2(), MRect.y, MRect.x + (MRect.w/2), MRect.y2() );
          break;
      }
    }

    //----------

};

//----------------------------------------------------------------------
#endif
