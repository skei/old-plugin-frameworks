#ifndef s3_widget_selector_included
#define s3_widget_selector_included
//----------------------------------------------------------------------

#include "common/s3_integer.h"

#include "gui/widgets/s3_widget_button.h"
#include "gui/widgets/s3_widget_menu.h"
#include "gui/widgets/s3_widget_menuitem.h"
#include "gui/widgets/s3_widget_symbol.h"

//----------

class S3_Widget_Selector
: public S3_Widget {

  protected:

    S3_Color          MBackColor;
    S3_Color          MTextColor;
    S3_Color          MArrowColor;
    uint32            MMenu_Num;
    const char**      MMenu_Text;
    S3_Widget_Menu*   MMenu;
    uint32            MTextAlign;
    //uint32            MSelected;
    bool              MMenuRightAlign;

  //------------------------------
  //
  //------------------------------

  public:

    S3_Widget_Selector(S3_Rect ARect, uint32 AIndex, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName       = "S3_Widget_Selector";
      MHint       = "selector";
      MFlags     |= s3_wf_opaque;
      MCursor     = s3_mc_finger;
      MBackColor  = S3_DarkGrey;
      MTextColor  = S3_LightGrey;
      MArrowColor = S3_LightGrey;
      MTextAlign  = s3_ta_left;
      MMenu       = S3_NULL;
      MMenu_Num   = 0;
      MMenu_Text  = S3_NULL;
      MMenuRightAlign = false;
      select(0);
    }

    //----------

    virtual ~S3_Widget_Selector() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void  backColor(S3_Color AColor)        { MBackColor = AColor; }
    void  textColor(S3_Color AColor)        { MTextColor = AColor; }
    void  arrowColor(S3_Color AColor)       { MArrowColor = AColor; }
    void  textAlign(uint32 AAlign)          { MTextAlign = AAlign; }
    void  menuRightAlign(bool AAlign=true)  { MMenuRightAlign = AAlign; }

  //------------------------------
  //
  //------------------------------

  public:

    void assignMenu(S3_Widget_Menu* AMenu) {
      MMenu = AMenu;
      MMenu_Num = AMenu->itemCount();
      MMenu_Text = AMenu->itemText();
      //MMenu->owner(this);
    }

    //----------

    void select(int32 AIndex) {
      if (AIndex>=0) {
        //MSelected = AIndex;
        MValue = (float)AIndex / (float)(MMenu_Num-1);
      }
    }

    //----------

    uint32 selected(void) {
      float index = MValue * (float)MMenu_Num;
      return  S3_MinI( (int)index, MMenu_Num-1 );
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual // S3_WidgetListener
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode) {
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect);
      APainter->setFillColor(MArrowColor);
      APainter->fillTriangle(
        MRect.x2()-10,  MRect.y+5,
        MRect.x2()-4,   MRect.y+5,
        MRect.x2()-7,   MRect.y+9 );
      APainter->setTextColor(MTextColor);
      uint32 index = selected();
      S3_Rect r = MRect;
      r.add(4,0,0,0);
      const char* txt = MMenu_Text[index];
      APainter->drawText(r,txt,MTextAlign);
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
    }

    //----------

    virtual // S3_WidgetListener
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Widget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
      if (MMenu) {
        if (AButton==s3_mb_left) {
          int32 x,y;
          if (MMenuRightAlign) {
            x = MRect.x2() - MMenu->rect().w;
            y = MRect.y2() + 1;
          }
          else {
            x = MRect.x;
            y = MRect.y2() + 1;
          }
          MMenu->owner(this);
          MMenu->open(x,y);
        }
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual
    void do_widgetUpdate(S3_Widget* AWidget) {
      if (AWidget==MMenu) {
        int32 index = MMenu->selected();
        select(index);
        S3_Widget::do_widgetUpdate(this);
        do_widgetRedraw(this,MRect,s3_pm_normal);
      }
      else S3_Widget::do_widgetUpdate(AWidget);
    }

    //----------

};

//----------------------------------------------------------------------
#endif

