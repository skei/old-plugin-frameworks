#ifndef kode_menu_widget_included
#define kode_menu_widget_included
//----------------------------------------------------------------------

/*
  TODO:
  - take menuitem's text x/y offset into account when calulating
    menu size?
*/

//----------------------------------------------------------------------

#include "base/kode_array.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widgets.h"
#include "gui/widgets/kode_menu_item_widget.h"

//typedef vector<char*> KODE_MenuItems;
typedef KODE_Array<char*> KODE_MenuItems;

//----------------------------------------------------------------------

class KODE_MenuListener {
  public:
    virtual void on_menuEvent(int32_t AIndex) {}
};

//----------------------------------------------------------------------

class KODE_MenuWidget
: public KODE_BorderWidget {

  protected:

    KODE_MenuListener*  MListener     = nullptr;
    KODE_MenuItems      MItems;
    uint32_t            MBorderSize   = 1;
    uint32_t            MItemWidth    = 96;
    uint32_t            MItemHeight   = 16;
    int32_t             MSelectedItem = -1;
    int32_t             MPrevSelected = -1;
    uint32_t            MItemsX       = 1;
    uint32_t            MItemsY       = 1;
    bool                MMirrorX      = false;
    bool                MMirrorY      = false;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_MenuWidget()
    : KODE_BorderWidget(KODE_Rect()) {
      clearFlag(KODE_WIDGET_ACTIVE);
      clearFlag(KODE_WIDGET_VISIBLE);
      MFillBackground   = true;
      MBackgroundColor  = KODE_DarkGrey;
      MDrawBorder       = true;
    }

    //----------

    virtual ~KODE_MenuWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual void        setItemWidth(int32_t AWidth)    { MItemWidth = AWidth; }
    virtual void        setItemHeight(int32_t  AHeight) { MItemHeight = AHeight; }
    virtual int32_t     getSelectedItem()               { return MSelectedItem; }
    virtual const char* getItem(uint32_t AIndex)        { return MItems[AIndex]; }
    virtual uint32_t    getNumItems()                   { return MItems.size(); }

    virtual void setMenuSize(int32_t x, int32_t y) {
      MItemsX = x;
      MItemsY = y;
    }

    virtual void setMenuMirror(bool x, bool y) {
      MMirrorX = x;
      MMirrorY = y;
    }

  //------------------------------
  //
  //------------------------------

  public:

    //virtual void appendMenuItem(const char* AItem) {
    //  MItems.append((char*)AItem);
    //}

    virtual void appendMenuItem(const char* AItem) {
      //MItems.append((char*)AItem);
      char* item = (char*)AItem;
      MItems.append(item);
    }

    //----------

    virtual void appendMenuItems(const char** AItems, uint32_t ANum) {
      for (uint32_t i=0; i<ANum; i++) {
        appendMenuItem(AItems[i]);
      }
    }

    //----------

    virtual void clearMenu(void) {
      //MItems.clear();
      deleteWidgets();
    }

    //----------

    virtual void open(KODE_MenuListener* AListener, int32_t AXpos, int32_t AYpos) {
      MListener = AListener;
      MPrevSelected = MSelectedItem;
      clearMenu();
      int32_t w = (MItemWidth  * MItemsX) + (MBorderSize * (MItemsX+1));
      int32_t h = (MItemHeight * MItemsY) + (MBorderSize * 2 /*(MItemsY+1)*/);
      // h = (MItems.size() * MItemHeight)  + (MBorderSize * 2);
      if (MMirrorX) AXpos -= w;
      if (MMirrorY) AYpos -= h;
      MRect = KODE_Rect(AXpos,AYpos,w,h);
      KODE_Rect R = KODE_Rect(AXpos+MBorderSize, AYpos+MBorderSize, MItemWidth, MItemHeight);
      uint32_t num = MItems.size();
      uint32_t i = 0;
      for (uint32_t x=0; x<MItemsX; x++) {
        for (uint32_t y=0; y<MItemsY; y++) {
          if (i < num) {
            const char* txt = MItems[i];
            //KODE_DTrace("%i %i %s : %i %i %i %i\n",x,y,txt,(int)R.x,(int)R.y,(int)R.w,(int)R.h);
            appendWidget( KODE_New KODE_MenuItemWidget( R, txt ));
            R.y += MItemHeight;
          }
          i++;
        }
        R.y = AYpos + MBorderSize;
        R.x += MItemWidth + MBorderSize;
      }
      //MIsActive = true;
      //MIsVisible = true;
      setFlag(KODE_WIDGET_ACTIVE);
      setFlag(KODE_WIDGET_VISIBLE);
      do_redraw(this,MRect); // redraw parent?
      do_setModal(this);
    }

    //----------

    virtual void close(void) {
      MListener = nullptr;
      //MIsActive = false;
      //MIsVisible = false;
      clearFlag(KODE_WIDGET_ACTIVE);
      clearFlag(KODE_WIDGET_VISIBLE);
      do_redraw(this,MRect);
      do_setModal(nullptr);
    }

  //------------------------------
  // on_ (parent to children)
  //------------------------------

  public:

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      KODE_BorderWidget::on_mouseClick(AXpos,AYpos,AButton,AState);
      if (AButton == KODE_BUTTON_LEFT) {
        if (!MRect.contains(AXpos,AYpos)) {
          // left-clicked outside of widget
          MSelectedItem = MPrevSelected;
          if (MListener) MListener->on_menuEvent(MSelectedItem);
          close();
        }
      }
      if (AButton == KODE_BUTTON_RIGHT) {
        // right-clicked
        MSelectedItem = MPrevSelected;
        if (MListener) MListener->on_menuEvent(MSelectedItem);
        close();
      }
    }

  //------------------------------
  // do_ (children to parent)
  //------------------------------

  public:

    // item selected

    void do_update(KODE_Widget* ASender) override {
      int32_t index = ASender->getIndex();
      MSelectedItem = index;
      if (MListener) MListener->on_menuEvent(MSelectedItem);
      close();
      KODE_BorderWidget::do_update(this);
    }

    //----------

    // (right button clicked)

    void do_notify(KODE_Widget* ASender, int32_t AMsg) override {
      if (AMsg==KODE_MENU_NOTIFY_CLOSE) {
        // right clicked on menuitem
        MSelectedItem = MPrevSelected;
        if (MListener) MListener->on_menuEvent(MSelectedItem);
        close();
      }
    }

};

//----------------------------------------------------------------------
#endif

