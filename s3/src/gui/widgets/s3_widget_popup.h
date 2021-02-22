#ifndef s3_widget_popup_included
#define s3_widget_popup_included
//----------------------------------------------------------------------

// todo: EPopupClose: s3_pc_closeEsc, s3_pc_closeRightClick

#include "gui/widgets/s3_widget_color.h"

//----------

class S3_Widget_Popup
: public S3_Widget_Color {

  private:

    bool      MIsOpen;
    S3_Color  MBorderColor;
    bool      MDrawBorder;

  public:

    S3_Widget_Popup(S3_Rect ARect, uint32 AAlignment=s3_wa_none)
    : S3_Widget_Color(ARect,AAlignment/*s3_wa_none*/) {
      MName = "S3_Widget_Popup";
      MHint = "popup";
      clearFlag(s3_wf_active);
      clearFlag(s3_wf_visible);
      MIsOpen = false;
      MColor = S3_LightGrey;
      MBorderColor = S3_DarkGrey;
      MDrawBorder = true;
    }

    virtual ~S3_Widget_Popup() {
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void  borderColor(S3_Color AColor)  { MBorderColor = AColor; }
    void  drawColor(bool ADraw)         { MDrawBorder = ADraw; }
    bool  isOpen(void)                  { return MIsOpen; }

  public:

    void open(int32 AXpos, int32 AYpos) {
      MIsOpen = true;
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
      MHoverWidget = S3_NULL;
      /*
        same as S3_Widget_Menu)
        does the next two actually influence anything?
        MModalWidget is only set by S3_Window, in response to a
        'bubbled-up' do_widgetModal()..
        todo: prev_hoverWidget?
      */
      MCapturedWidget = S3_NULL;
      MModalWidget = S3_NULL;
      on_widgetReset(this,0); // FIsHovering = false, for all menuitems...
    }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      //STrace("painting %s\n",MName);
      S3_Widget_Color::on_widgetPaint(AWidget,APainter,ARect,AMode);
      if (MDrawBorder) {
        APainter->setDrawColor(MBorderColor);
        APainter->drawRectangle(MRect);
      }
    }

    //virtual
    //void do_update(SWidget* AWidget) {
    //  MSelected = AWidget->index();
    //  MSelectedWidget = AWidget;
    //  //KTrace(['KWidget_Menu.do_update: ',FSelected{Index},KODE_CR]);
    //  //close;
    //  SWidget_Color::do_update(this);
    //}

};

//----------------------------------------------------------------------
#endif
