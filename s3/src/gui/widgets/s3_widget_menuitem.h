#ifndef s3_widget_menuitem_included
#define s3_widget_menuitem_included
//----------------------------------------------------------------------

#include "gui/widgets/s3_widget_text.h"

//----------

class S3_Widget_MenuItem
: public S3_Widget_Text {

  private:

    bool      MIsHovering;
    S3_Color  MHoverTextColor;
    S3_Color  MHoverBackColor;

  public:

    S3_Widget_MenuItem(S3_Rect ARect, const char* AText, uint32 AAlignment=s3_wa_fillTop)
    : S3_Widget_Text(ARect,AText,AAlignment) {
      MName = "S3_Widget_MenuItem";
      MHint = "menuitem";
      //setFlag(swf_active);
      MDrawBackground = true;
      MIsHovering = false;
      MCursor = s3_mc_finger;
      MTextColor = S3_Black;//SDarkGrey;
      MBackColor = S3_Color(0.65,0.65,0.65);//S3_LightGrey;
      MHoverTextColor = S3_White;
      MHoverBackColor = S3_DarkGrey;//SBlack;
    }

    virtual ~S3_Widget_MenuItem() {
    }

  public:

    bool hovering(void)         { return MIsHovering; }
    void hovering(bool AHover)  { MIsHovering = AHover; }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Color temptext,tempback;
      //STrace("painting %s\n",MName);
      if (MIsHovering) {
        temptext = MTextColor;
        tempback = MBackColor;
        MTextColor = MHoverTextColor;//SLightGrey;
        MBackColor = MHoverBackColor;//SDarkGrey;
        S3_Widget_Text::on_widgetPaint(this,APainter,ARect,AMode);
        MTextColor = temptext;
        MBackColor = tempback;
      }
      else {
        //MTextColor = SDarkGrey;
        //MBackColor = SLightGrey;
        S3_Widget_Text::on_widgetPaint(this,APainter,ARect,AMode);
      }
    }

    //virtual
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      switch(AButton) {
        case s3_mb_left:
          MValue = 1;
          do_widgetUpdate(this);
          break;
        //smb_right:
        //  FValue := 0;
        //  do_update(Self);
        //  break;
      } // swith
    }

    //virtual
    void on_widgetEnter(S3_Widget* AWidget, int32 AXpos, int32 AYpos) { // from
      //DTrace("enter %i (%s')\n",MIndex,MName);
      MIsHovering = true;
      do_widgetRedraw(this,MRect,s3_pm_hover);
      S3_Widget_Text::on_widgetEnter(AWidget,AXpos,AYpos); // cursor, hint
    }

    //virtual
    void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) { // to
      //DTrace("leave %i (%s')\n",MIndex,MName);
      MIsHovering = false;
      do_widgetRedraw(this,MRect,s3_pm_normal);
      S3_Widget_Text::on_widgetLeave(AWidget,AXpos,AYpos);
    }

    //virtual
    void on_widgetReset(S3_Widget* AWidget, int32 AMode=0) {
      MIsHovering = false;
      S3_Widget_Text::on_widgetReset(AWidget,AMode); // children
    }

};

//----------------------------------------------------------------------
#endif

