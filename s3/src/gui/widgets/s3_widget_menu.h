#ifndef s3_widget_menu_included
#define s3_widget_menu_included
//----------------------------------------------------------------------

#include "gui/widgets/s3_widget_menuitem.h"
#include "gui/widgets/s3_widget_color.h"

//----------

class S3_Widget_Menu
: public S3_Widget_Color {

  private:

    S3_Widget*    MOwner;

    int32         MSelected;
    S3_Widget*    MSelectedWidget;
    bool          MIsOpen;
    bool          MDrawBackground;
    bool          MDrawBorder;
    S3_Color      MBorderColor;

    uint32        MMenu_Num;
    const char**  MMenu_Text;


  //------------------------------
  //
  //------------------------------

  public:

    S3_Widget_Menu(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget_Color(ARect,AAlignment) {
      MName = "S3_Widget_Menu";
      MHint = "menu";
      clearFlag(s3_wf_active);
      clearFlag(s3_wf_visible);
      MSelected     = -1;
      MIsOpen       = false;
      MColor        = S3_Color(0.65,0.65,0.65);
      MDrawBorder   = true;
      MBorderColor  = S3_DarkGrey;
      MOwner        = S3_NULL;
      MMenu_Num     = 0;
      MMenu_Text    = S3_NULL;
    }

    //----------

    virtual ~S3_Widget_Menu() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32         selected(void)            { return MSelected; }
    S3_Widget*    selectedWidget(void)      { return MSelectedWidget; }
    bool          isOpen(void)              { return MIsOpen; }
    void          owner(S3_Widget* AOwner)  { MOwner = AOwner; }
    uint32        itemCount(void)           { return MMenu_Num; }
    const char**  itemText(void)            { return MMenu_Text; }
    const char*   itemText(uint32 AIndex)   { return MMenu_Text[AIndex]; }

  //------------------------------
  //
  //------------------------------

  public:

    #define MARGINS     5
    #define PADDING     0
    //#define ITEMHEIGHT  16

    void createMenuItems(uint32 AWidth, uint32 AHeight, uint32 ANum, const char** AText, uint32 numcol=1) {
      MMenu_Num   = ANum;
      MMenu_Text  = AText;
      int32 numrow = ANum / numcol;
      if ((ANum % numcol) > 0) numrow += 1;
      int32 w = AWidth  * numcol + ((numcol-1) * PADDING) + (2*MARGINS);
      int32 h = AHeight * numrow + ((numrow-1) * PADDING) + (2*MARGINS);
      on_widgetSetSize(this,w,h);
      margins(MARGINS);
      for (uint32 i=0; i<ANum; i++) {
        uint32 x = i / numrow;
        uint32 y = i % numrow;
        x *= AWidth;
        y *= AHeight;
        appendWidget( new S3_Widget_MenuItem( S3_Rect(x,y,AWidth,AHeight), AText[i], s3_wa_none ));
      }
    }

    #undef MARGINS
    #undef PADDING
    #undef ITEMHEIGHT

    //----------

    void deselect(void) {
      if ((MSelected >= 0) && (MSelected < (int32)MSubWidgets.size())) {
        S3_Widget_MenuItem* menuitem = (S3_Widget_MenuItem*)MSelectedWidget;
        menuitem->hovering(false);
      }
    }

    //----------

    /*
      todo:
      - center around mouse cursor, so that we initially point at
        item 1
      - make sure menu fits on screen/widget
    */

    void open(int32 AXpos, int32 AYpos) {
      MIsOpen = true;
      MSelected = -1;
      MSelectedWidget = S3_NULL;
      on_widgetSetPos(this,AXpos,AYpos);
      do_widgetCursor(this,s3_mc_default);
      setFlag(s3_wf_active);
      setFlag(s3_wf_visible);
      //on_setPos(AXpos,AYpos);
      //on_align;
      do_widgetRedraw(this,MRect,s3_pm_normal);
      do_widgetModal(this,0);
    }

    //----------

    void close(void) {
      MIsOpen = false;
      do_widgetModal(S3_NULL,0);
      clearFlag(s3_wf_active);
      clearFlag(s3_wf_visible);
      do_widgetRedraw(this,MRect,s3_pm_normal);
      // which of these three are needed.. todo: investigate
      MHoverWidget    = S3_NULL;
      /*
        todo: prev_hoverWidget?
      */
      MCapturedWidget = S3_NULL;
      MModalWidget    = S3_NULL;

      on_widgetReset(this,0); // FIsHovering = false, for all menuitems...
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      if (( (AButton==s3_mb_left) && (!MRect.contains(AXpos,AYpos)))
         || (AButton==s3_mb_right)) {
        close();
        MSelected = -1;
        MSelectedWidget = S3_NULL;
        if (MOwner) MOwner->do_widgetUpdate(this);
        else S3_Widget_Color::do_widgetUpdate(this);
      }
      S3_Widget_Color::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
    }

    //----------

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Widget_Color::on_widgetPaint(AWidget,APainter,ARect,AMode);
      if (MDrawBorder) {
        APainter->setDrawColor(MBorderColor);
        APainter->drawRectangle(MRect);
      }
    }

    //----------

    //virtual
    void do_widgetUpdate(S3_Widget* AWidget) {
      close();
      MSelected = AWidget->index();
      MSelectedWidget = AWidget;
      if (MOwner) MOwner->do_widgetUpdate(this);
      else S3_Widget_Color::do_widgetUpdate(this);
    }

};

//----------------------------------------------------------------------
#endif

