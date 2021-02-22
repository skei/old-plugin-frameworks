#ifndef s3_widget_buttonrow_included
#define s3_widget_buttonrow_included
//----------------------------------------------------------------------

#include "common/s3_convert.h"
#include "gui/widgets/s3_widget_grid.h"

//----------

#define S3_MAX_STATES 32

#define s3_bm_single  0
#define s3_bm_multi   1

//----------

class S3_Widget_ButtonRow
: public S3_Widget_Grid {

  protected:

    int32   MMode;
    bool    MStates[S3_MAX_STATES];
    char*   MLabels[S3_MAX_STATES];
    int32   MSelected;
    //SColor MValueColor;
    //SColor MBackColor;
    S3_Color MTextColor;
    S3_Color MActiveColor;

  public:

    //property    _mode : LongInt read FMode write FMode;
    int32 selected(void) { return MSelected; }
    bool  state(uint32 AIndex) { return MStates[AIndex]; }

  public:

    S3_Widget_ButtonRow(S3_Rect ARect, int32 ANum, const char** ATxt, int32 AMode=s3_bm_multi, uint32 AAlignment=s3_wa_none)
    //S3_Widget_ButtonRow(S3_Rect ARect, int32 ANum, int32 AMode=s3_bm_multi, uint32 AAlignment=s3_wa_none)
    : S3_Widget_Grid(ARect,ANum,1,AAlignment) {
      MName = "S3_Widget_ButtonRow";
      MHint = "buttonrow";
      setFlag(s3_wf_opaque);
      MMode = AMode;//s3_bm_multi;//s3_bm_single;
      //MWidth = ANum;//5;
      //MHeight = 1;
      //for (int32 i=0; i<S3_MAX_STATES; i++) {
      for (int32 i=0; i<ANum; i++) {
        MStates[i] = false;
        //MLabels[i] = S3_NULL;
        MLabels[i] = (char*)ATxt[i];
      }
      MSelected = 0;
      MStates[0] = true;
      MCursor = s3_mc_finger;
      MTextColor = S3_White;
      MActiveColor = S3_DarkGrey;
      MBackColor = S3_Grey;

      MDrawVertLines = false;

    }

    //void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
    //}

    //void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

  public:

    virtual // S3_Widget_Grid
    void on_gridClickCell(S3_Widget* AWidget, int32 AX, int32 AY, int32 AB) {
      select(AX);
      do_widgetUpdate(this);
      do_widgetRedraw(this,MRect,s3_pm_normal);
    }

    //----------

    virtual // S3_Widget_Grid
    void on_gridPaintCell(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, int32 AX, int32 AY) {
      char buf[256];
      if (MStates[AX]) APainter->setFillColor(MActiveColor);
      else APainter->setFillColor(MBackColor);
      APainter->fillRectangle( ARect );
      APainter->setTextColor(MTextColor);
      char* txt = MLabels[AX];
      if (txt) {
        APainter->drawText(ARect,txt,s3_ta_center);
      }
      else {
        S3_IntToString(buf,AX);
        APainter->drawText(ARect,buf,s3_ta_center);
      }
    }

    //virtual
    //void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
    //  //MValue = (float)MSelected / ((float)MWidth - 1.0f);
    //  S3_Widget_Grid::on_widgetPaint(AWidget,APainter,ARect,AMode);
    //}

    virtual // S3_Widget
    void value(float AValue) {
      S3_Widget::value(AValue);
      selectValue(AValue);
    }


    //----------

    bool getState(int32 i) {
      return MStates[i];
    }

    //----------

    void setState(int32 i, bool s) {
      MStates[i] = s;
    }

    //----------

    char* getName(int32 i) {
      return MLabels[i];
    }

    //----------

    void setName(int32 i, char* AName) {
      MLabels[i] = AName;
    }

    //----------

    void select(int32 index/*, bool ARedraw=false*/) {
      MSelected = index;
      //KTrace(['click cell: ',AX,',',AY,',',AB,KODE_CR]);
      if (MMode == s3_bm_single) {
        //for (int32 i=0; i<MWidth; i++) { // FHeight if vertical
        for (int32 i=0; i<MWidth; i++) { // FHeight if vertical
          if (i==MSelected) MStates[i] = true;
          else MStates[i] = false;
        }
        /*
        5 steps => 0,1,2,3,4 => 0, 0.25, 0.50, 0.75, 1.0
        selected/(steps-1)
        */
        MValue = (float)MSelected / ((float)MWidth - 1.0f);
        //KTrace(['KWidget_ButtonRow.select. FValue = ',FValue,KODE_CR]);
      }
      else {
        MStates[MSelected] = MStates[MSelected] ? false : true;
      }
      //if (ARedraw) do_redraw(this,MRect);
    }

    //----------

    //function KWidget_ButtonRow.getValue: Single;
    //begin
    //  result := FValue;
    //end;

    void selectValue(float AValue) {
      float num = AValue * MWidth; // 0 = 0, 1 = 4, 0.999 = 3
      num = S3_Min(num,MWidth-1);
      select( (int)num );
      //do_redraw(self,FRect);
      //do_update(self);
    }

};

#undef S3_MAX_STATES

//----------------------------------------------------------------------
#endif
