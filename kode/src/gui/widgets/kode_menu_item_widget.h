#ifndef kode_menu_item_widget_included
#define kode_menu_item_widget_included
//----------------------------------------------------------------------

#include "base/kode_color.h"
#include "gui/kode_widget.h"

#define KODE_MENU_NOTIFY_CLOSE 1

//----------------------------------------------------------------------

class KODE_MenuItemWidget
: public KODE_TextWidget {

  protected:

    //int32   MXoffset  = 0;
    //int32   MYoffset  = 0;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_MenuItemWidget(KODE_Rect ARect, const char* AText="menuitem"/*, KODE_Color AColor=KODE_DarkGrey, uint32_t AAlign=kta_left*/)
    : KODE_TextWidget(ARect,AText) {
      MName                 = "KODE_Widget_MenuItem";
      //MTextXoffset          = 2;
      MDrawText             = true;
      MFillBackground       = true;
      MDrawBorder           = false;
      MBackgroundColor      = KODE_DarkGrey;
      MTextColor            = KODE_LightGrey;
      
      MCursor               = KODE_CURSOR_FINGER;
      //setFlag(KODE_WIDGET_SETCURSOR);
    }

    //----------

    virtual ~KODE_MenuItemWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    //virtual void setOffset(KODE_i32 AXoffset, KODE_i32 AYoffset) {
    //  MXoffset = AXoffset;
    //  MYoffset = AYoffset;
    //}

  //------------------------------
  //
  //------------------------------

  public:

  //------------------------------
  // on_ (parent to children)
  //------------------------------

  public:

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      if (AButton == KODE_BUTTON_LEFT) {
        do_update(this);
      }
      if (AButton == KODE_BUTTON_RIGHT) {
        do_notify(this,KODE_MENU_NOTIFY_CLOSE);
      }
    }

    //----------

    void on_enter(float AXpos, float AYpos, KODE_Widget* AFrom=nullptr) override {
      //MIsHovering       = true;
      MBackgroundColor  = KODE_LightGrey;
      MTextColor        = KODE_DarkGrey;
      do_redraw(this,MRect);
    }

    //----------

    void on_leave(float AXpos, float AYpos, KODE_Widget* ATo=nullptr) override {
      //MIsHovering       = false;
      MBackgroundColor  = KODE_DarkGrey;
      MTextColor        = KODE_LightGrey;
      do_redraw(this,MRect);
    }

};

//----------------------------------------------------------------------
#endif

