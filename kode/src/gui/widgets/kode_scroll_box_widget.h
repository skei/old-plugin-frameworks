#ifndef kode_scroll_box_widget_included
#define kode_scroll_box_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/widgets/kode_border_widget.h"
#include "gui/widgets/kode_scroll_bar_widget.h"

//----------

class KODE_ScrollBoxWidget
: public KODE_Widget {
  
//------------------------------
private:
//------------------------------

  KODE_ScrollBarWidget* MVerticalScrollBar    = nullptr;
  KODE_ScrollBarWidget* MHorizontalScrollBar  = nullptr;
  KODE_BorderWidget*    MContainer            = nullptr;
  
  //float MVerticalOffset = 0.0f;
  //float MHorizontalOffset = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_ScrollBoxWidget(KODE_Rect ARect)
  : KODE_Widget(ARect) {

    MName = "KODE_ScrollBoxWidget";
    MHint = "scrollbox";
    //setFlag(KODE_WIDGET_CLIP);
    
    //float x1  = ARect.x;
    //float y1  = ARect.y;
    //float w   = ARect.w;
    //float h   = ARect.h;
    //float x2  = ARect.x2();
    //float y2  = ARect.y2();
    //MVerticalScrollBar    = KODE_New KODE_ScrollBarWidget(KODE_Rect(x2-10, y1,    10,   h-10  ), true );
    //MHorizontalScrollBar  = KODE_New KODE_ScrollBarWidget(KODE_Rect( x1,    y2-10, w-10, 10    ), false );
    //MContainer            = KODE_New KODE_BorderWidget(   KODE_Rect( x1,    y1,    w-10, h-10  ) );

    MVerticalScrollBar = KODE_New KODE_ScrollBarWidget(KODE_Rect(10), true );
    MVerticalScrollBar->setAlignment(KODE_WIDGET_ALIGN_RIGHT_FILL);
    MVerticalScrollBar->setMargins(0,0,0,10);
    
    MHorizontalScrollBar = KODE_New KODE_ScrollBarWidget(KODE_Rect(10), false );
    MHorizontalScrollBar->setAlignment(KODE_WIDGET_ALIGN_BOTTOM_FILL);
    MHorizontalScrollBar->setMargins(0,0,0,0);

    MContainer = KODE_New KODE_BorderWidget(KODE_Rect(0) );
    MContainer->setAlignment(KODE_WIDGET_ALIGN_CLIENT_FILL);
    MContainer->setPadding(10);
    MContainer->setSpacing(5);
    //MContainer->setFlag(KODE_WIDGET_CLIP);
    
    //MContainer->setDrawBorder();
    //MContainer->setBorderColor(KODE_Yellow);
    MContainer->setFillBackground(false);
    //MContainer->setBackgroundColor(KODE_Grey);
    
    
    KODE_Widget::appendWidget(MVerticalScrollBar);
    KODE_Widget::appendWidget(MHorizontalScrollBar);
    KODE_Widget::appendWidget(MContainer);
    
  }

  //----------

  virtual ~KODE_ScrollBoxWidget() {
  }
  
//------------------------------
public:
//------------------------------

  //void setPos(float x, float y) override {
  //  KODE_Widget::setPos(x,y);
  //}
  
  //void setSize(float w, float h) override {
  //  KODE_Widget::setSize(w,h);
  //}
  
  KODE_Widget* appendWidget(KODE_Widget* AWidget) override {
    return MContainer->appendWidget(AWidget);
  }
  
  //void deleteWidgets() override {
  // KODE_Widget::deleteWidgets();
  //}
  
  //KODE_Widget* getChild(uint32_t AIndex) override {
  //  return MContainer->getChild(AIndex);
  //}
  
  //KODE_Widget* findChild(float x, float y, bool children) override {
  //  return MContainer->findChild(x,y,children);
  //}
  
  //----------
  
  void scroll() {
  
    KODE_Rect r = MContainer->getRect();
    KODE_Rect c = MContainer->getContentRect();
    //KODE_Trace("c.w %.2f c.h %.2f\n",c.w,c.h);
    if ((c.w > 0) && (c.h > 0)) {
      // how much of the content is not visible?
      // ie. how much can we scroll?
      float x = 1.0f - (r.w / c.w);
      float y = 1.0f - (r.h / c.h);
      // how many pixels can we scroll?
      x *= c.w;
      y *= c.h;
      // how many pixels should we scroll?
      x *= MHorizontalScrollBar->getThumbPos();
      y *= MVerticalScrollBar->getThumbPos();
      //KODE_Trace("x %.2f y %.2f\n",x,y);
      MContainer->setOffset(-x,-y);
      MContainer->on_realign();
      //do_redraw(MContainer,MContainer->getRect());
      //MContainer->offsetChildren(-x,-y);
    }
  }
  
  //----------
  
  void recalc_thumb_size() {
    
    KODE_Rect cr = MContainer->getContentRect();
    
    if ((cr.w <= 0) || (cr.h <= 0)) {
      MHorizontalScrollBar->setThumbSize(1);
      MVerticalScrollBar->setThumbSize(1);
      MContainer->setOffset(0,0);
      //do_redraw(MHorizontalScrollBar,MHorizontalScrollBar->getRect());
      //do_redraw(MVerticalScrollBar,MVerticalScrollBar->getRect());
      return;
    }

    KODE_Rect  r = MContainer->getRect();
    float size;
    
    // horizontal
    
    size = r.w / cr.w;
    if (size >= 1.0f) {
      size = 1.0f;
      MHorizontalScrollBar->setThumbPos(0,false);
      MContainer->setXOffset(0);
    }
    MHorizontalScrollBar->setThumbSize(size);
    //do_redraw(MHorizontalScrollBar,MHorizontalScrollBar->getRect());

    // vertical
    
    size = r.h / cr.h;
    if (size >= 1.0f) {
      size = 1.0f;
      MVerticalScrollBar->setThumbPos(0,false);
      MContainer->setYOffset(0);
    }
    MVerticalScrollBar->setThumbSize(size);
    //do_redraw(MVerticalScrollBar,MVerticalScrollBar->getRect());

  }
  
//------------------------------
public:
//------------------------------

  void on_realign(bool ARecursive=true) override {
    //KODE_TRACE;
    KODE_Widget::on_realign(ARecursive);
    recalc_thumb_size();
  }

//------------------------------
public:
//------------------------------

  void do_update(KODE_Widget* ASender) override {
    //KODE_TRACE;
    if ((ASender == MHorizontalScrollBar) || (ASender == MVerticalScrollBar)) {
      scroll();
      do_redraw(MContainer,MContainer->getRect());
    }
    else {
      KODE_Widget::do_update(ASender);
    }
  }
  
  //void do_moved(KODE_Widget* ASender, float AXpos, float AYpos) override {
  //  KODE_Widget::do_moved(ASender,AXpos,AYpos);
  //}
  
  //void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) override {
  //  KODE_Widget::do_resized(ASender,AWidth,AHeight);
  //}

};

//----------------------------------------------------------------------
#endif
