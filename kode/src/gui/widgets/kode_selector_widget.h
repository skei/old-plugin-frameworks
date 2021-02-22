#ifndef kode_selector_widget_included
#define kode_selector_widget_included
//----------------------------------------------------------------------

#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class KODE_SelectorWidget
: public KODE_TextWidget
, public KODE_MenuListener {

  protected:

    KODE_MenuWidget*  MMenu =KODE_NULL;
    //uint32_t        MTextAlignment = kta_left;  // cppcheck: already defined in KWidget_Text

  //------------------------------
  //
  //------------------------------

  public:

    KODE_SelectorWidget(KODE_Rect ARect, KODE_MenuWidget* AMenu=nullptr)
    : KODE_TextWidget(ARect,"Text") {
      MName             = "KODE_SelectorWidget";
      MMenu             = AMenu;
      MCursor           = KODE_CURSOR_FINGER;
      MFillBackground   = true;
      MBackgroundColor  = KODE_DarkGrey;
      MTextColor        = KODE_White;
      MDrawBorder       = false;
      MDrawText         = true;
      MText             = "<select>";
    }

    //----------

    virtual ~KODE_SelectorWidget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual void setSelected(int32_t AIndex) {
      float v = indexToValue(AIndex);
      setValue(v);
      //do_update(this);
      //do_redraw(this,MRect);
    }
    
    //----------
    
    virtual int32_t getSelected(void) {
      int32_t i = valueToIndex( getValue() );
      return i;
    }
    
    //----------
    
    virtual float indexToValue(int32_t AIndex) {
      float v = 0.0f;
      if (MMenu) {
        uint32_t num = MMenu->getNumItems() - 1;
        if (num > 0) {
          v = (float)AIndex / (float)num;
        }
      }
      return v;
    }

    //----------
    
    virtual uint32_t valueToIndex(float AValue) {
      float i = 0;
      if (MMenu) {
        uint32_t num = MMenu->getNumItems() - 1;
        if (num > 0) {
          i = AValue * (float)num;
        }
      }
      return (uint32_t)i;
    }

  //------------------------------
  //
  //------------------------------

public:
  
    // todo: set MValue

    void on_menuEvent(int32_t AIndex) override {
      float v = indexToValue(AIndex);
      setValue(v);
      do_update(this);
      do_redraw(this,MRect);
    }

  //------------------------------
  // on_ (parent to children)
  //------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      if (MMenu) {
        float val = getValue();
        int32_t sel = valueToIndex(val);
        if (sel >= 0) {
          MText = MMenu->getItem(sel);
        }
        else {
          MText = "---";
        }
      }
      KODE_TextWidget::on_paint(APainter,ARect);
      APainter->setDrawColor(KODE_LightGrey);
      APainter->fillTriangle(
        MRect.x2() - 10,  MRect.y2() - 7,
        MRect.x2() - 3,   MRect.y2() - 7,
        MRect.x2() - 7,   MRect.y2() - 3
      );
    }

    //----------

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      KODE_TextWidget::on_mouseClick(AXpos,AYpos,AButton,AState); // Release ??
      if (MMenu) MMenu->open(this,AXpos,AYpos);
    }

};

//----------------------------------------------------------------------
#endif

