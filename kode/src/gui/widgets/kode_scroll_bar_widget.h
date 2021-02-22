#ifndef kode_scroll_bar_widget_included
#define kode_scroll_bar_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
//#include "common/kode_math.h"

class KODE_ScrollBarWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  KODE_Color  MBackgroundColor  = KODE_DarkGrey;
  KODE_Color  MThumbColor       = KODE_LightGrey;
  KODE_Color  MInteractiveColor = KODE_White;
  float       MThumbPos         = 0.0f;
  float       MThumbSize        = 0.25f;
  float       MPageSize         = 0.1f;
  
  KODE_Rect   MThumbRect        = {0};
  bool        MIsDragging       = false;
  float       MClickedX         = 0.0f;
  float       MClickedY         = 0.0f;
  float       MClickedPos       = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_ScrollBarWidget(KODE_Rect ARect, bool AVertical=false)
  : KODE_Widget(ARect) {
    MName = "KODE_ScrollBarWidget";
    MHint = "scrollbar";
    if (AVertical) setFlag(KODE_WIDGET_VERTICAL);
    //recalcThumbRect();
    
  }

  //----------

  virtual ~KODE_ScrollBarWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual float getThumbPos() {
    return MThumbPos;
  }
  
  //virtual void getThumbSize() { return MThumbSize; }

  virtual void setBackgroundColor(KODE_Color AColor) {
    MBackgroundColor = AColor;
  }
  
  //----------

  virtual void setThumbColor(KODE_Color AColor) {
    MThumbColor = AColor;
  }
  
  //----------

  virtual void setInteractiveColor(KODE_Color AColor) {
    MInteractiveColor = AColor;
  }
  
  //----------

  virtual void setThumbPos(float APos, bool ARedraw=true) {
    MThumbPos = APos;
    MThumbPos = KODE_Clamp(MThumbPos,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_redraw(this,MRect);
  }
  
  //----------

  virtual void setThumbSize(float ASize, bool ARedraw=true) {
    MThumbSize = ASize;
    if (MThumbSize >= 1.0f) MThumbPos = 0.0f;
    MThumbSize = KODE_Clamp(MThumbSize,0.0f,1.0f);
    //recalcThumbRect();
    if (ARedraw) do_redraw(this,MRect);
  }
  
  //----------
  
  virtual void setPageSize(float ASize) {
    MPageSize = ASize;
  }
  
  //----------

  virtual void recalcThumbRect() {
    if (hasFlag(KODE_WIDGET_VERTICAL)) {
      float thumb = MRect.h * MThumbSize;
      float available = MRect.h - thumb;
      MThumbRect.x = MRect.x;
      MThumbRect.y = MRect.y + (MThumbPos * available);
      MThumbRect.w = MRect.w;
      MThumbRect.h = thumb;
    }
    else {
      float thumb = MRect.w * MThumbSize;
      float available = MRect.w - thumb;
      MThumbRect.x = MRect.x + (MThumbPos * available);
      MThumbRect.y = MRect.y;
      MThumbRect.w = thumb;
      MThumbRect.h = MRect.h;
    }
    //KODE_Trace("ThumbRect %.2f %.2f %.2f %.2f\n",MThumbRect.x,MThumbRect.y,MThumbRect.w,MThumbRect.h);
  }

//------------------------------
public:
//------------------------------

  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    KODE_Widget::on_paint(APainter,ARect);
    recalcThumbRect();
    APainter->setFillColor(MBackgroundColor);
    APainter->fillRectangle(MRect);
    if (MIsInteractive) APainter->setFillColor(MInteractiveColor);
    else APainter->setFillColor(MThumbColor);
    APainter->fillRectangle(MThumbRect);
  }

  //----------
  
  void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
    KODE_Widget::on_mouseClick(AXpos,AYpos,AButton,AState);
    if (AButton == KODE_BUTTON_LEFT) {
      if (MThumbRect.contains(AXpos,AYpos)) {
        MClickedX = AXpos;
        MClickedY = AYpos;
        MClickedPos = MThumbPos;
        MIsDragging = true;
        MIsInteractive  = true;
        do_redraw(this,MRect);
      }
      else {
        if (hasFlag(KODE_WIDGET_VERTICAL)) {
          if (AYpos < MThumbRect.y) {
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_update(this);
            do_redraw(this,MRect);
          }
          else if (AYpos > MThumbRect.y2()) {
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_update(this);
            do_redraw(this,MRect);
          }
        }
        else {
          if (AXpos < MThumbRect.x) {
            MThumbPos -= MPageSize;
            //recalcThumbRect();
            do_update(this);
            do_redraw(this,MRect);
          }
          else if (AXpos > MThumbRect.x2()) {
            MThumbPos += MPageSize;
            //recalcThumbRect();
            do_update(this);
            do_redraw(this,MRect);
          }
        } // !vertical
      } // !contains
    } // left button
  }
  
  //----------

  void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
    KODE_Widget::on_mouseRelease(AXpos,AYpos,AButton,AState);
    if (AButton == KODE_BUTTON_LEFT) {
      if (MIsDragging) {
        MIsDragging = false;
        MIsInteractive = false;
        do_redraw(this,MRect);
      }
    }
  }

  //----------

  void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {
    KODE_Widget::on_mouseMove(AXpos,AYpos,AState);
    if (MIsDragging) {
      
      float available = 0.0f;
      float dist = 0.0f;
      float thumb_size = 0.0f;
      if (hasFlag(KODE_WIDGET_VERTICAL)) {
        dist = AYpos - MClickedY;
        thumb_size = MRect.h * MThumbSize;
        available = MRect.h - thumb_size;
      }
      else {
        dist = AXpos - MClickedX;
        thumb_size = MRect.w * MThumbSize;
        available = MRect.w - thumb_size;
      } 
      if (available > 0.0f) {
        float v = dist / available;
        MThumbPos = MClickedPos + v;
        MThumbPos = KODE_Clamp(MThumbPos,0.0f,1.0f);
        //recalcThumbRect();
        do_update(this);
        do_redraw(this,MRect);
      }
    }
  }
  
};

//----------------------------------------------------------------------
#endif
