#ifndef kode_button_row_widget_included
#define kode_button_row_widget_included
//----------------------------------------------------------------------

//#include "base/kode_convert.h"
#include "gui/kode_widgets.h"
#include "gui/widgets/kode_grid_widget.h"

//----------

#define KODE_MAX_STATES 32

#define KODE_BUTTON_ROW_SINGLE  0
#define KODE_BUTTON_ROW_MULTI   1

//----------

class KODE_ButtonRowWidget
: public KODE_GridWidget {

  protected:

    int32_t     MMode;
    bool        MStates[KODE_MAX_STATES];
    char*       MLabels[KODE_MAX_STATES];
    int32_t     MSelected;
    //KODE_Color  MValueColor;
    //KODE_Color  MBackColor;

    KODE_Color  MTextColor;
    KODE_Color  MActiveColor;
    KODE_Color  MBorderColor;

  public:

    //property    _mode : LongInt read FMode write FMode;
    int32_t getSelected(void)         { return MSelected; }
    bool    getState(uint32_t AIndex) { return MStates[AIndex]; }

  public:

    KODE_ButtonRowWidget(KODE_Rect ARect, int32_t ANum, const char** ATxt, int32_t AMode=KODE_BUTTON_ROW_SINGLE)
    //KWidget_ButtonRow(KRect ARect, int32 ANum, int32 AMode=KODE_BUTTON_ROW_MULTI, uint32 AAlignment=kwa_none)
    : KODE_GridWidget(ARect,ANum,1) {
      MName = "KODE_ButtonRow_Widget";
      MHint = "buttonrow";
      //setFlag(kwf_opaque);
      MMode = AMode;
      //MWidth = ANum;//5;
      //MHeight = 1;
      //for (int32 i=0; i<KODE_MAX_STATES; i++) {
      for (int32_t i=0; i<ANum; i++) {
        MStates[i] = false;
        //MLabels[i] = KODE_NULL;
        MLabels[i] = (char*)ATxt[i];
      }
      MSelected = 0;
      MStates[0] = true;
      MCursor = KODE_CURSOR_FINGER;
      MTextColor = KODE_White;
      MActiveColor = KODE_DarkGrey;
      MBackgroundColor = KODE_Grey;
      MDrawVerticalLines = false;
    }

    //void on_paint(SPainter* APainter, SRect ARect, uint32 AMode=0) {
    //}

    //void on_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
    //}

  public:

    void on_clickCell(/*KWidget* AWidget,*/ int32_t AX, int32_t AY, int32_t AB) override {
      select(AX);
      do_update(this);
      do_redraw(this,MRect);
    }

    //----------

    void on_paintCell(/*KWidget* AWidget,*/ KODE_Painter* APainter, KODE_Rect ARect, int32_t AX, int32_t AY) override {
      char buf[256];
      APainter->setPenSize(1);

      if (MStates[AX]) APainter->setFillColor( MActiveColor );
      else APainter->setFillColor( MBackgroundColor );
      APainter->fillRectangle(ARect);

      //if ((AX > 0) /*&& (AX < (MWidth-1))*/  ) {
      //  APainter->setDrawColor( MBorderColor );
      //  APainter->drawVLine(ARect.x,ARect.y,ARect.y2());
      //}
      APainter->setTextColor(MTextColor);
      char* txt = MLabels[AX];
      if (txt) {
        APainter->drawText(ARect,txt,KODE_TEXT_ALIGN_CENTER);
      }
      else {
        //KODE_IntToString(buf,AX);
        sprintf(buf,"%i",AX);
        APainter->drawText(ARect,buf,KODE_TEXT_ALIGN_CENTER);
      }

    }

    //virtual
    //void on_widgetPaint(KWidget* AWidget, KPainter* APainter, KRect ARect, uint32 AMode=0) {
    //  //MValue = (float)MSelected / ((float)MWidth - 1.0f);
    //  KWidget_Grid::on_widgetPaint(AWidget,APainter,ARect,AMode);
    //}

    void setValue(float AValue) override {
      KODE_Widget::setValue(AValue);
      selectValue(AValue);
    }


    //----------

    bool getState(int32_t i) {
      return MStates[i];
    }

    //----------

    void setState(int32_t i, bool s) {
      MStates[i] = s;
    }

    //----------

    char* getName(int32_t i) {
      return MLabels[i];
    }

    //----------

    void setName(int32_t i, const char* AName) {
      MLabels[i] = (char*)AName;
    }

    //----------

    void select(int32_t index/*, bool ARedraw=false*/) {
      MSelected = index;
      //KTrace(['click cell: ',AX,',',AY,',',AB,KODE_CR]);
      if (MMode == KODE_BUTTON_ROW_SINGLE) {
        //for (int32 i=0; i<MWidth; i++) { // FHeight if vertical

        for (int32_t i=0; i<MWidth; i++) { // FHeight if vertical
          if (i==MSelected) MStates[i] = true;
          else MStates[i] = false;
        }

        /*
        5 steps => 0,1,2,3,4 => 0, 0.25, 0.50, 0.75, 1.0
        selected/(steps-1)
        */
        float v = (float)MSelected / ((float)MWidth - 1.0f);
        KODE_Widget::setValue(v);
        //KODE_Trace("%0.3f\n",v);
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
      num = KODE_Min(num,float(MWidth-1));
      select( (int)num );
      //do_redraw(self,FRect);
      //do_update(self);
    }

};

#undef KODE_MAX_STATES

//----------------------------------------------------------------------
#endif
