#ifndef kode_window_included
#define kode_window_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#ifdef KODE_GUI_XCB
  #ifndef KODE_WINDOW_NOT_BUFFERED
    #include "gui/xcb/kode_xcb_buffered_window.h"
    typedef KODE_XcbBufferedWindow KODE_ImplementedWindow;
  #else
    #include "gui/xcb/kode_xcb_window.h"
    typedef KODE_XcbWindow KODE_ImplementedWindow;
  #endif
#endif

//----------

#ifdef KODE_GUI_CAIRO
  #ifndef KODE_WINDOW_NOT_BUFFERED
    #include "gui/cairo/kode_cairo_buffered_window.h"
    typedef KODE_CairoBufferedWindow KODE_ImplementedWindow;
  #else
    #include "gui/cairo/kode_cairo_window.h"
    typedef KODE_CairoWindow KODE_ImplementedWindow;
  #endif
#endif

//----------------------------------------------------------------------

class KODE_Window
: public KODE_ImplementedWindow {

//------------------------------
private:
//------------------------------

  KODE_Widget*  MHoverWidget      = KODE_NULL;
  KODE_Widget*  MActiveWidget     = KODE_NULL;
  KODE_Widget*  MModalWidget      = KODE_NULL;
  KODE_Widget*  MCapturedWidget   = KODE_NULL;
  KODE_Widget*  MKeyboardFocus    = KODE_NULL;
  bool          MFillBackground   = true;
  KODE_Color    MBackgroundColor  = KODE_Grey;
  float         MMouseX           = 0.0f;
  float         MMouseY           = 0.0f;
  int32_t       MCurrentCursor    = -1;
  
  //bool          MIsDragging       = false;
  //bool          MCanDrop          = false;
  //KODE_Widget*  MDragFrom         = KODE_NULL;
  //uint32_t      MDragType         = 0;
  //void*         MDragPtr          = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Window(uint32_t AWidth, uint32_t AHeight, void* AParent=KODE_NULL)
  : KODE_ImplementedWindow(AWidth,AHeight,AParent) {
    MName = "KODE_Window";
    //MRect = KODE_Rect(AWidth,AHeight);
    MRect.x = 0;
    MRect.y = 0;
  }

  //----------

  virtual ~KODE_Window() {
  }

//------------------------------
public:
//------------------------------

  virtual void setFillBackground(bool AFill=true)     { MFillBackground = AFill; }
  virtual void setBackgroundColor(KODE_Color AColor)  { MBackgroundColor = AColor; }

//------------------------------
private:
//------------------------------

  virtual void updateHoverWidget(float x, float y) {
    KODE_Widget* widget = KODE_NULL;
    if (MModalWidget) {
      widget = MModalWidget->findChild(x,y,true);
    }
    else if (MCapturedWidget) {
      widget = MCapturedWidget->findChild(x,y,true);
    }
    else {
      widget = findChild(x,y,true);
    }
    if (widget==this) { widget = KODE_NULL; }
    if (widget != MHoverWidget) {
      if (MHoverWidget) {
        if (MHoverWidget->hasFlag(KODE_WIDGET_SETCURSOR)) setCursor(KODE_CURSOR_DEFAULT);
        MHoverWidget->on_leave(x,y);
      }
      MHoverWidget = widget;
      if (MHoverWidget) {
        if (MHoverWidget->hasFlag(KODE_WIDGET_SETCURSOR)) setCursor(MHoverWidget->getCursor());
        MHoverWidget->on_enter(x,y);
      }
    }
  }

//------------------------------
public:
//------------------------------

  void open() override {
    KODE_ImplementedWindow::open();
    
//    MRect.x = 0;
//    MRect.y = 0;
    realignChildren();
    
    //MRect = KODE_Rect(MWindiwWidth,MWindowHeight);
    //if (hasFlag(KODE_WIDGET_REALIGN))
//      realignChildren();
  }
    
  //void setSize(int32_t AWidth, int32_t AHeight) override {
  //  KODE_Trace("%i %i\n",AWidth,AHeight);
  //  //KODE_ImplementedWindow::setSize(AWidth, AHeight);
  //}
  
  void fillBackground(KODE_Painter* APainter, KODE_Rect ARect) {
    if (MFillBackground) {
      APainter->setFillColor(MBackgroundColor);
      APainter->fillRectangle(ARect);
    }
}


//------------------------------
public:
//------------------------------

  //void on_move(float x, float y) {
  //  KODE_Trace("%.2f %.2f\n",x,y);
  //}

  //----------

  void on_resize(float AWidth, float AHeight) {
    
    //KODE_Trace("%.0f %.0f -> %.0f %.0f\n",MRect.w,MRect.h,AWidth,AHeight);
    
    KODE_ImplementedWindow::on_resize(AWidth, AHeight);
    MRect = KODE_Rect(AWidth,AHeight);
    //if (hasFlag(KODE_WIDGET_REALIGN))
    realignChildren();
    // -> KODE_CairoWindow
    //
    //#ifdef KODE_WINDOW_NOT_BUFFERED
    //  #ifdef KODE_CAIRO
    //    cairo_xcb_surface_set_size(MCairoSurface,w,h);
    //    MWindowPainter->flush();
    //  #endif
    //#endif

  }

  //----------

  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    //KODE_Trace("%.2f %.2f %.2f %.2f\n",ARect.x,ARect.y,ARect.w,ARect.h);
    //if (MFillBackground) {
    //  APainter->setFillColor(MBackgroundColor);
    //  APainter->fillRectangle(ARect);
    //}
    fillBackground(APainter,ARect);
    KODE_ImplementedWindow::on_paint(APainter,ARect);
  }

  //----------

  void on_keyPress(uint8_t c, uint32_t k, uint32_t s) override {
    //KODE_Trace("c %i k %i s %i\n",(int)c,k,s);
    if (MKeyboardFocus) {
      MKeyboardFocus->on_keyPress(c,k,s);
    }
  }

  //----------

  void on_keyRelease(uint8_t c, uint32_t k, uint32_t s) override {
    //KODE_Trace("%i %i %i\n",c,k,s);
    if (MKeyboardFocus) {
      MKeyboardFocus->on_keyRelease(c,k,s);
    }
  }

  //----------

  void on_mouseClick(float x, float y, uint32_t b, uint32_t s) override {
    //KODE_Trace("%.2f %.2f %i %i\n",x,y,b,s);
    if (MHoverWidget) {
      if (MHoverWidget->hasFlag(KODE_WIDGET_ACTIVE)) {
        MActiveWidget = MHoverWidget;
        MActiveWidget->on_mouseClick(x,y,b,s);
      }
      
      //TODO:if-drag-n-drop
      
      //MDragType = 0;
      //MDragPtr = KODE_NULL;
      //if (MHoverWidget->on_startDrag(&MDragType,&MDragPtr)) {
      //  MIsDragging = true;
      //  MDragFrom = MHoverWidget;
      //}
      
    }
  }

  //----------

  void on_mouseMove(float x, float y, uint32_t s) override {
    MMouseX = x;
    MMouseY = y;
    //KODE_Trace("%.2f %.2f %i %i\n",x,y,s);
    
    //if (MIsDragging) {
    //  updateHoverWidget(x,y);
    //  if (MHoverWidget) {
    //    if (MHoverWidget->on_canDrop(MDragType,MDragPtr)) MCanDrop = true;
    //    else MCanDrop = false;    //  }
    //}
    
    if (MActiveWidget) {
      MActiveWidget->on_mouseMove(x,y,s);
    }
    else {
      updateHoverWidget(x,y);
      if (MHoverWidget) {
        if (MHoverWidget->hasFlag(KODE_WIDGET_HOVER)) {
          MHoverWidget->on_mouseMove(x,y,0);
        }
      }
    }
  }

  //----------

  void on_mouseRelease(float x, float y, uint32_t b, uint32_t s) override {
    //KODE_Trace("%.2f %.2f %i %i\n",x,y,b,s);
    
    //if (MIsDragging) {
    //  if (MHoverWidget && MCanDrop) {
    //    MHoverWidget->on_endDrag(MDragFrom,MDragType,MDragPtr);
    //  }
    //  MIsDragging = false;
    //  MDragFrom = KODE_NULL;
    //  MDragType = 0;
    //  MDragPtr = KODE_NULL;
    //}
    
    if (MActiveWidget) {
      MActiveWidget->on_mouseRelease(x,y,b,s);
      MActiveWidget = KODE_NULL;
    }
    //updateHoverWidget(x,y);
  }

  //----------

  void on_enter(float x, float y, KODE_Widget* AFrom=KODE_NULL) override {
    //KODE_Trace("%.2f %.2f\n",x,y);
    //MHoverWidget = KODE_NULL;
    updateHoverWidget(x,y);
  }

  //----------

  void on_leave(float x, float y, KODE_Widget* AFrom=KODE_NULL) override {
    //KODE_Trace("%.2f %.2f\n",x,y);
    if (MHoverWidget) MHoverWidget->on_leave(x,y);
    MHoverWidget = KODE_NULL;

  }

  //----------

  //void on_timer() override {
  //  KODE_Trace("\n");
  //}

  //----------

  //void on_idle() override {
  //  KODE_Trace("\n");
  //}

  //----------

  //void on_clientMessage(uint32_t AData) override {
  //  KODE_Trace("%i\n",AData);
  //}

//------------------------------
public:
//------------------------------

  //void do_notify(KODE_Widget* ASender, int32_t AMsg) override {
  //}

  //----------

  //void do_update(KODE_Widget* ASender) override override {
  //}

  //----------

  void do_redraw(KODE_Widget* ASender, KODE_Rect ARect) override {
    //paint(ARect);
    invalidate(ARect);
  }

  //----------

  //void do_moved(KODE_Widget* ASender, float AXpos, float AYpos) override {
  //}

  //----------
  
  /*
    widget has been resized
    notify parent of widget, and realign/redraw
  */

  void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) override {
    KODE_Widget* parent = ASender->getParent();
    if (parent) {
      //KODE_Trace("ASender %s h %.0f parent %s h %.0f \n",ASender->getName(),ASender->getRect().h,parent->getName(),parent->getRect().h);
      //ASender->setSize(AWidth,AHeight);
      parent->on_realign(); // realignChildren();
      //KODE_Rect parent_rect = parent->getRect();
      //KODE_Trace("%s %.0f %.0f %.0f %.0f\n",parent->getName(),parent_rect.x,parent_rect.y,parent_rect.w,parent_rect.h);
      //do_redraw(parent,parent_rect);
      do_redraw(parent,parent->getRect());
      //paint(parent->getRect());
      
    }
    //ASender->on_realign(); //realignChildren();
    //KODE_Rect rect = ASender->getRect();
    //KODE_Trace("%s %.0f %.0f %.0f %.0f\n",ASender->getName(),rect.x,rect.y,rect.w,rect.h);
    //do_redraw(ASender,rect);
  }
  
  //----------

  void do_setCursor(KODE_Widget* ASender, int32_t ACursor) override {
    //KODE_Trace("%i\n",ACursor);
    if (ACursor == KODE_CURSOR_HIDE) hideCursor();
    else if (ACursor == KODE_CURSOR_SHOW) showCursor();
    else {
      if (ACursor != MCurrentCursor) {
        MCurrentCursor = ACursor;
        setCursor(ACursor);
      }
    }
  }

  //----------

  void do_setCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) override {
    setCursorPos(AXpos,AYpos);
  }


  //----------

  void do_hint(KODE_Widget* ASender, const char* AHint) override {
    if (AHint) { KODE_Trace("hint from '%s': '%s'\n",ASender->getName(),AHint); }
  }

  //----------

  void do_wantKeyEvents(KODE_Widget* ASender) override {
    MKeyboardFocus = ASender;
  }

  //----------

  void do_setModal(KODE_Widget* ASender) override {
    MModalWidget = ASender;
    if (ASender) {
    }
    else {
      updateHoverWidget(MMouseX,MMouseY);
    }
  }

  //----------

  //void do_startDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) override {
  //}

  //----------

  //void do_endDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) override {
  //}
  
  void do_sizer(KODE_Widget* ASender, float ADeltaX, float ADeltaY, uint32_t AMode) override {
    //KODE_TRACE;
    
    // resize window?
    
    /*
    KODE_Rect r = ASender->getRect();
    r.add(0,0,ADeltaX,ADeltaY);
    ASender->setRect(r);
    KODE_Widget* parent = ASender->getParent();
    if (parent) {
      KODE_Trace("%s\n",parent->getName());
      parent->on_realign();
      parent->do_redraw(parent,parent->getRect());
    }
    */
    //MRect.w += ADeltaX;
    //MRect.h += ADeltaY;
    //on_realign();
    //realignChildren();
    //do_redraw(this,MRect);
  }
  

};


//----------------------------------------------------------------------
#endif
