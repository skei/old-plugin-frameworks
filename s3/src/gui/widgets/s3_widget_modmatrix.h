#ifndef s3_widget_modmatrix_included
#define s3_widget_modmatrix_included
//----------------------------------------------------------------------

/*
  todo:
  - tell S3_Widget_ModMatrix about the 'real' modmatrix..
  (constructor, or setModMatrix function)
  update this directly in do_widgetUpdate

*/

#include "gui/s3_widget.h"
#include "gui/widgets/s3_widget_dragvalue.h"
#include "gui/widgets/s3_widget_selector.h"
#include "audio/s3_modulation_matrix.h"

//----------------------------------------------------------------------

class S3_Widget_ModMatrixItem
: public S3_Widget {

  private:

    int32 MSubIndex;
    float MSubValue;

//    S3_ParamFloat* MAmountParam;
//    S3_ParamFloat* MScaleParam;
//    S3_ParamFloat* MOffsetParam;

  public:

    S3_Widget_ModMatrixItem(S3_Rect ARect, S3_Widget_Menu* ASrcMenu, S3_Widget_Menu* ADstMenu, uint32 ASrc, uint32 ADst, float AAmt, float AScale, float AOffset, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {

      MSubIndex = -1;
      MSubValue = 0;

      padding(1);
      int32 h = ARect.h;

      S3_Widget_Selector* src = new S3_Widget_Selector( S3_Rect(-27,h), 0, s3_wa_fillLeft);
      src->assignMenu(ASrcMenu);
      src->menuRightAlign();
      src->select( ASrc );
      appendWidget(src);

      S3_Widget_Selector* dst = new S3_Widget_Selector( S3_Rect(-27,h), 0, s3_wa_fillLeft);
      dst->assignMenu(ADstMenu);
      dst->menuRightAlign();
      dst->select( ADst );
      appendWidget(dst);

      S3_Widget_DragValue* amount = new S3_Widget_DragValue( S3_Rect(-15,h), AAmt, s3_wa_fillLeft );
      amount->backColor(S3_DarkGrey);
      amount->valueColor(S3_LightGrey);
      amount->drawName(false);
      amount->offValue(0);
      appendWidget(amount);

      S3_Widget_DragValue* scale = new S3_Widget_DragValue( S3_Rect(-15,h), AScale, s3_wa_fillLeft );
      scale->backColor(S3_DarkGrey);
      scale->valueColor(S3_LightGrey);
      scale->drawName(false);
      scale->offValue(1);
      appendWidget(scale);

      S3_Widget_DragValue* offset = new S3_Widget_DragValue( S3_Rect(-15,h), AOffset, s3_wa_fillClient );
      offset->backColor(S3_DarkGrey);
      offset->valueColor(S3_LightGrey);
      offset->drawName(false);
      offset->offValue(0);
      appendWidget(offset);

    }

    //----------

    virtual ~S3_Widget_ModMatrixItem() {

//      delete MAmountParam;

    }

  public:

    int32 subIndex(void) { return MSubIndex; }
    float subValue(void) { return MSubValue; }

    //void modSrc(int32 ASrc) {}
    //void modDst(int32 ASrc) {}
    //void modAmount(int32 ASrc) {}

  public:

    virtual void do_widgetUpdate(S3_Widget* AWidget) {
      MSubIndex = AWidget->index();
      MSubValue = AWidget->value();
      S3_Widget::do_widgetUpdate(this);
    }

};

//----------------------------------------------------------------------

class S3_Widget_ModMatrix
: public S3_Widget {

  private:

    S3_Widget_Menu* MSrcMenu;
    S3_Widget_Menu* MDstMenu;

    int32 MModIndex;
    int32 MSubIndex;
    float MSubValue;


  public:

    S3_Widget_ModMatrix(S3_Rect ARect, S3_Widget_Menu* ASrcMenu, S3_Widget_Menu* ADstMenu, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MSrcMenu = ASrcMenu;
      MDstMenu = ADstMenu;
      padding(1);
      MModIndex = -1;
      MSubIndex = -1;
      MSubValue = 0;
    }

    virtual ~S3_Widget_ModMatrix() {
    }

  public:

    int32 modIndex(void) { return MModIndex; }
    int32 subIndex(void) { return MSubIndex; }
    float subValue(void) { return MSubValue; }

    //----------

    S3_Widget* subWidget(int32 AModIndex, int32 AWdgIndex) {
      S3_Widget_ModMatrixItem* item = (S3_Widget_ModMatrixItem*)MSubWidgets[AModIndex];
      return item->subWidget(AWdgIndex);
    }

    //----------

  public:

    void appendModulation(uint32 src, uint32 dst, float amt, float scale, float offset  ) {
      appendWidget( new S3_Widget_ModMatrixItem(
        S3_Rect(MRect.w,16),
        MSrcMenu,
        MDstMenu,
        src,
        dst,
        amt,
        scale,
        offset,
        s3_wa_fillTop ));
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual void do_widgetUpdate(S3_Widget* AWidget) {
      S3_Widget_ModMatrixItem* wdg = (S3_Widget_ModMatrixItem*)AWidget;
      MModIndex = wdg->index();
      MSubIndex = wdg->subIndex();
      MSubValue = wdg->subValue();

      //S3_Trace("MModIndex %i MSubIndex %i MSubValue %.2f wdg.index %i\n",MModIndex,MSubIndex,MSubValue,wdg->index());

      S3_Widget::do_widgetUpdate( wdg->subWidget(MSubIndex) );

      //S3_Widget::do_widgetUpdate(this);
      //int32 idx = MModIndex * 5 + MSubIndex;
      //S3_Widget::do_widgetUpdate( subWidget(idx) );

      /*
      S3_Parameter* param = parameter( MMatrix.paramIndex()+ANum );
      float value = param->from01(AValue);
      switch(ANum) {
        case 0: MMatrix.modMatrix()->modSrc(AIndex,value); break;
        case 1: MMatrix.modMatrix()->modDst(AIndex,value); break;
        case 2: MMatrix.modMatrix()->modAmount(AIndex,value); break;
        case 3: MMatrix.modMatrix()->modScale(AIndex,value); break;
        case 4: MMatrix.modMatrix()->modOffset(AIndex,value); break;
      }
      */


    }

    //----------

};

//----------------------------------------------------------------------
#endif




#if 0

#include "gui/s3_painter.h"

class S3_WidgetListener {

  //------------------------------
  // parent -> subwidgets
  //------------------------------

  public:

    virtual void on_widgetReset(S3_Widget* AWidget, int32 AMode) {}
    virtual void on_widgetSetPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void on_widgetSetSize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void on_widgetScroll(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY) {}
    //virtual void on_widgetMove(S3_Widget* AWidget, int32 AXdelta, int32 AYdelta) {}
    //virtual void on_widgetResize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void on_widgetAlign(S3_Widget* AWidget) {}
    virtual void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode) {}
    virtual void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_widgetMouseUp(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_widgetMouseMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AState) {}
    virtual void on_widgetMouseDoubleClick(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {}
    virtual void on_widgetKeyDown(S3_Widget* AWidget, int32 AKey, int32 AState) {}
    virtual void on_widgetKeyUp(S3_Widget* AWidget, int32 AKey, int32 AState) {}
    virtual void on_widgetEnter(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void on_widgetLeave(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    //virtual void on_widgetTimer(S3_Widget* AWidget) {}
    //virtual void on_widgetIdle(S3_Widget* AWidget) {}

    // move to window listener?
    #ifdef S3_TABLET
    virtual void on_widgetTabletEvent(int32 AXpos, int32 AYpos, float APressure) {}
    #endif

  //------------------------------
  // subwidget -> parent
  //------------------------------

  public:

    virtual void do_widgetMove(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_widgetResize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {}
    virtual void do_widgetSizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {}
    virtual void do_widgetRedraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) {}
    virtual void do_widgetUpdate(S3_Widget* AWidget) {}
    virtual void do_widgetCursor(S3_Widget* AWidget, int32 ACursor) {}
    virtual void do_widgetCursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {}
    virtual void do_widgetHint(S3_Widget* AWidget, const char* AHint) {}
    virtual void do_widgetModal(S3_Widget* AWidget, int32 AMode=0) {}
    virtual void do_widgetWantKeys(S3_Widget* AWidget) {}

#endif // 0
