#ifndef kode_drag_value_widget_included
#define kode_drag_value_widget_included
//----------------------------------------------------------------------
/*
  todo:
  
  - if snap/quantizemode = 1 (shift key), set new value when pressing or
    releasing shift key
*/

//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_color.h"
#include "base/kode_math.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_value_widget.h"

class KODE_DragValueWidget
: public KODE_ValueWidget {
  
//------------------------------
//private:
protected:
//------------------------------

  bool        MShiftPressed   = false;
  float       MClickedX       = 0.0f;
  float       MClickedY       = 0.0f;
  float       MClickedValue   = 0.0f;
  float       MPrevMouseX     = 0.0f;
  float       MPrevMouseY     = 0.0f;
  float       MDragValue      = 0.0f;
  
  float       MSensitivity    = 0.01f;
  float       MSensitivity2   = 0.05f;
  
  bool        MSnap           = false;
  float       MSnapPos        = 0.5;
  float       MSnapDist       = 0.1;
  uint32_t    MSnapMode       = 1; // 0: always snap, 1: shift disables snapping

  bool        MQuantize       = false;
  uint32_t    MQuantizeSteps  = 0;
  uint32_t    MQuantizeMode   = 1;
  
  uint32_t    MShiftKey       = KODE_KEY_SHIFT;
  
  KODE_Color  MInteractiveColor = KODE_White;
  
//------------------------------
public:
//------------------------------

  KODE_DragValueWidget(KODE_Rect ARect, float AValue=0)
  : KODE_ValueWidget(ARect,AValue) { 
    //MTempColor = getBackgroundColor();
    //MCursor = KODE_CURSOR_ARROWUPDOWN;
    setFlag(KODE_WIDGET_HIDECURSOR);
    setFlag(KODE_WIDGET_GRABCURSOR);
    //setFlag(KODE_WIDGET_SETCURSOR);
    MCursor = KODE_CURSOR_ARROWUPDOWN;
  }
  
  //----------
  
  virtual ~KODE_DragValueWidget() {
  }
  
//------------------------------
public:
//------------------------------

  virtual void setSensitivity(float ASens)        { MSensitivity = ASens; }
  virtual void setSensitivity2(float ASens)       { MSensitivity2 = ASens; }

  virtual void setSnap(bool ASnap=true)           { MSnap = ASnap; }
  virtual void setSnapPos(float APos)             { MSnapPos = APos; }
  virtual void setSnapDist(float ADist)           { MSnapDist = ADist; }
  virtual void setSnapMode(uint32_t AMode)        { MSnapMode = AMode; }
  
  virtual void setQuantize(bool AQuantize=true)   { MQuantize = AQuantize; }
  virtual void setQuantizeSteps(uint32_t ASteps)  { MQuantizeSteps = ASteps; }
  virtual void setQuantizeMode(uint32_t AMode)    { MQuantizeMode = AMode; }
  
  virtual void setShiftKey(uint32_t AKey)         { MShiftKey = AKey; }
  
  virtual void setInteractiveColor(KODE_Color AColor) { MInteractiveColor = AColor; }
  
//------------------------------
protected:
//------------------------------

  virtual bool  getShiftPressed() { return MShiftPressed; }
  virtual float getClickedX()     { return MClickedX; }
  virtual float getClickedY()     { return MClickedY; }
  virtual float getClickedValue() { return MClickedValue; }
  virtual float getPrevMouseX()   { return MPrevMouseX; }
  virtual float getPrevMouseY()   { return MPrevMouseY; }
  virtual float getDragValue()    { return MDragValue; }

//------------------------------
private:
//------------------------------

  virtual float snapValue(float AValue) {
    float v = AValue;
    float s = 1.0f;
    float dist = fabs( AValue - MSnapPos );
    if (dist < MSnapDist) {
      v = MSnapPos;
    }
    else {
      // scale left
      if (AValue < MSnapPos) {
        float sp_sd = MSnapPos - MSnapDist;
        if (sp_sd > 0) s = MSnapPos / sp_sd;
        v = AValue * s;
      }
      // scale right
      else if (AValue > MSnapPos) {
        float iv = 1.0f - AValue;
        float isp = (1.0f - MSnapPos);
        float isp_sd = isp - MSnapDist;
        if (isp_sd > 0) s = isp / isp_sd;
        v = iv * s;
        v = 1.0f - v;
      }
    }
    v = KODE_Clamp(v,0.0f,1.0f);
    return v;
  }
  
//------------------------------
public:
//------------------------------

  /*
    TODO:
    - want keys?
    - intercept shift, etc..
  */
  
  void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
    MShiftPressed = (AState & MShiftKey);
    MClickedX = AXpos;
    MClickedY = AYpos;
    MClickedValue = getValue();
    MDragValue = getValue();
    MIsInteractive = true;
    //MTempColor = getBackgroundColor();
    //setBackgroundColor(KODE_White);
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
    if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_HIDE);
    //do_update(this);
    do_redraw(this,MRect);
    
    
  }
  
  //----------
  
  void on_mouseMove(float AXpos, float AYpos, uint32_t AState) {
    if ((AXpos == MClickedX) && (AYpos == MClickedY)) {
      MPrevMouseX = AXpos;
      MPrevMouseY = AYpos;
      return;
    }
    bool shift_pressed = (AState & MShiftKey);
    if ((shift_pressed == true) && (MShiftPressed == false)) {
      //TODO whwn shift is pressed
    }
    if ((shift_pressed == false) && (MShiftPressed == true)) {
      //TODO whwn shift is released
    }
    MShiftPressed = shift_pressed;
    if (MIsInteractive) {

      float value;
      if (MShiftPressed) value = getValue();
      else value = MDragValue;
      
      float diff;
      diff = MPrevMouseY - AYpos;
      float change = diff * MSensitivity;
      if (AState & KODE_KEY_CTRL) change *= MSensitivity2;
      float v = KODE_Clamp((value + change),0.0f,1.0f);
      MDragValue = v;
      if (MSnap) {
        switch (MSnapMode) {
          case 0:
            v = snapValue(v);
            break;
          case 1:
            if (!(AState & MShiftKey)) v = snapValue(v);
            break;
        }
      } // snap
      if (MQuantize) {
        switch (MSnapMode) {
          case 0:
            v = KODE_Quantize(v,MQuantizeSteps);
            break;
          case 1:
            if (!(AState & MShiftKey)) v = KODE_Quantize(v,MQuantizeSteps);
            break;
        }
      } // quantize
      setValue(v);
      if (hasFlag(KODE_WIDGET_GRABCURSOR)) do_setCursorPos(this,MClickedX,MClickedY);
      do_update(this);
      do_redraw(this,MRect);
    }
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
  }
  
  //----------
  
  void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
    MIsInteractive = false;
    //setBackgroundColor(MTempColor);
    if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_SHOW);
    //do_update(this);
    do_redraw(this,MRect);
  }
  
  //----------
  
  //void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
  //  if (MIsInteractive) setBackgroundColor(KODE_White);
  //  else setBackgroundColor(LightGrey);
  //  KODE_ValueWidget::on_paint(APainter,ARect);
  //}
 
};

//----------------------------------------------------------------------
#endif
