#ifndef s3_widget_textedit_included
#define s3_widget_textedit_included
//----------------------------------------------------------------------

#include "gui/s3_widget.h"

/*
enum EKeyChar {
  s3_kc_enter     = 13,
  s3_kc_esc       = 27,
  s3_kc_home      = 36,
  s3_kc_end       = 35,
  s3_kc_left      = 37,
  s3_kc_right     = 39,
  s3_kc_delete    = 46,
  s3_kc_backspace = 8
};
*/

//----------

class S3_Widget_TextEdit
: public S3_Widget {

  private:

    char      MText[256];
    S3_Color  MTextColor;
    S3_Color  MBackColor;
    S3_Color  MCaretColor;
    bool      MEditing;
    int32     MCaretPos;

  public:

    S3_Widget_TextEdit(S3_Rect ARect, const char* AText, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName = "S3_Widget_TextEdit";
      MHint = "textedit";
      S3_Strncpy(MText,AText,255);
      MTextColor  = S3_Black;
      MBackColor  = S3_LightGrey;
      MCaretColor = S3_BrightRed;
      MEditing    = false;
      MCaretPos   = 0;
      MCursor = s3_mc_ibeam;
    }

    virtual ~S3_Widget_TextEdit() {
    }

  public:

    //virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect);
      if (MEditing) {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-2,MRect.y2(),MText,s3_ta_left);

        char c = MText[MCaretPos];
        MText[MCaretPos] = 0;
        int32 txtwidth = APainter->getTextWidth(MText);
        MText[MCaretPos] = c;
        int32 x = MRect.x + 2 + txtwidth;

        APainter->setDrawColor(MCaretColor);
        APainter->drawLine(x,MRect.y,x,MRect.y2());

      } // editing
      else {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-4,MRect.y2(),MText,s3_ta_left);
      } // not editing
    }

    ////----------------------------------------
    //
    //{
    //  assume APos is valid
    //}
    //
    //// pos = 2
    //// len = 6
    //// 01234567
    //// ..::::0
    //
    //procedure insertChar(ABuffer:PChar; APos:LongInt; AChar:Char);
    //var
    //  i,len : longint;
    //begin
    //  if not Assigned(ABuffer) then exit;
    //  len := KStrlen(ABuffer);
    //  for i := (len+1) downto (APos+1) do ABuffer[i] := ABuffer[i-1];
    //  ABuffer[Apos] := AChar;
    //end;
    //
    ////----------
    //
    //procedure deleteChar(ABuffer:PChar; APos:LongInt);
    //var
    //  i,len : longint;
    //begin
    //  if not Assigned(ABuffer) then exit;
    //  len := KStrlen(ABuffer);
    //  for i := Apos to len do ABuffer[i] := ABuffer[i+1];
    //end;

    //----------------------------------------

    // AKey = key code, not ascii..

    //virtual
    void on_widgetKeyDown(S3_Widget* AWidget, int32 AKey, int32 AState) {

      //S3_Trace("AKey %i AState %i\n",AKey,AState);

    //var
    //  c : char;
    //  len : longint;
    ////  changed : boolean;

      int32 len;
      char  c;

      S3_Widget::on_widgetKeyDown(AWidget,AKey,AState);

      //STrace("key: %i, skift: %i\n",AKey,AState);

      //changed := false;
      switch(AKey) {
        case s3_kc_enter:
          MEditing = false;
          do_widgetWantKeys(S3_NULL);
          do_widgetUpdate(this);
          do_widgetRedraw(this,MRect,s3_pm_normal);
          //changed := true;
          //KTrace(['"',MText,'"',KODE_CR])
          break;
        case s3_kc_esc:
          MEditing = false;
          do_widgetWantKeys(S3_NULL);
          do_widgetUpdate(this);
          do_widgetRedraw(this,MRect,s3_pm_normal);
          //changed = true;
          break;
        case s3_kc_home:
          MCaretPos = 0;
          //changed = true;
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          break;
        case s3_kc_end:
          len = S3_Strlen(MText);
          MCaretPos = len;
          //changed = true;
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          break;
        case s3_kc_left:
          MCaretPos -= 1;
          if (MCaretPos < 0) MCaretPos = 0;
          //changed = true;
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          break;
        case s3_kc_right:
          len = S3_Strlen(MText);
          MCaretPos += 1;
          if (MCaretPos > len) MCaretPos = len;
          //changed = true;
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          break;
        case s3_kc_delete:
          len = S3_Strlen(MText);
          if ((uint32)MCaretPos < S3_Strlen(MText)) {
            S3_DeleteChar(MText,MCaretPos);
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          }
          break;
        case s3_kc_backspace:
          if (MCaretPos > 0) {
            MCaretPos -= 1;
            S3_DeleteChar(MText,MCaretPos);
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
          }
          break;
        default:
          if ((AKey >= 32) && (AKey <= 127)) {
            c = AKey & 0xff;
            S3_InsertChar(MText,MCaretPos,c);
            MCaretPos += 1;
            //if FCaretPos > len then FCaretPos = len;
            do_widgetUpdate(this);
            do_widgetRedraw(this,MRect,s3_pm_normal);
            //changed = true;
          }
          break;
      } // switch
      //if changed then
      //begin
      //  do_update(this);
      //  do_redraw(this,MRect);
      //end;
    }

    //virtual
    //void on_keyUp(int32 AKey, int32 AState) {
    //}

    //virtual
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Widget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
      MEditing = true;
      MCaretPos = S3_Strlen(MText);
      //do_update(this);
      do_widgetRedraw(this,MRect,s3_pm_normal);
      //do_modal(this);
      do_widgetWantKeys(this);
    }

};

//----------------------------------------------------------------------
#endif

