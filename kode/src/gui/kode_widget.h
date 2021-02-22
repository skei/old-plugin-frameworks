#ifndef kode_widget_included
#define kode_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_rect.h"
#include "gui/kode_painter.h"
#include "plugin/kode_parameter.h"

class   KODE_Widget;
typedef KODE_Array<KODE_Widget*> KODE_Widgets;

//----------------------------------------------------------------------

class KODE_Widget {

//------------------------------
private:
//------------------------------

  // private - 'force' using getValue(), so that we can override it in
  // containers..
  
  float         MValue          = 0.0f;

//------------------------------
protected:
//------------------------------

  const char*   MName           = "KODE_Widget";
  const char*   MLabel          = "";
  uint32_t      MFlags          = KODE_WIDGET_ACTIVE
                                | KODE_WIDGET_VISIBLE
                                | KODE_WIDGET_SETCURSOR
                                | KODE_WIDGET_CLIP
                                ;
  KODE_Rect     MRect           = {0};        // current (on-screen) rect
  KODE_Widget*  MParent         = KODE_NULL;  // parent
  KODE_Widgets  MChildren;                    // sub/child widgets
  int32_t       MIndex          = -1;         // index in parent
  
  //void*         MParameter      = KODE_NULL;  // parameter ptr (KODE_Paraneter)
  void*         MParameters[16] = {0};    
  
  const char*   MHint           = "";
  int32_t       MCursor         = KODE_CURSOR_DEFAULT;
  uint32_t      MAlignment      = KODE_WIDGET_ALIGN_PARENT; // _NONE;
  KODE_Rect     MContentRect    = {0};
  KODE_Rect     MInitialRect    = {0};
  KODE_Rect     MMargins        = {0}; // outside rect
  KODE_Rect     MPadding        = {0}; // inside rect
  KODE_Point    MSpacing        = {0}; // between child widgets
  KODE_Point    MOffset         = {0};
  KODE_Point    MMinSize        = {0};
  KODE_Point    MMaxSize        = {KODE_FLOAT_MAX};
  KODE_Point    MResized        = {0};
  bool          MIsInteractive  = false;

//------------------------------
public:
//------------------------------

  KODE_Widget(KODE_Rect ARect) {
    //KODE_Trace("%s %.0f %.0f %.0f %.0f\n",getName(),ARect.x,ARect.y,ARect.w,ARect.h);
    MRect         = ARect;
    MInitialRect  = ARect;
  }

  //----------

  virtual ~KODE_Widget() {
    #ifndef KODE_NO_AUTODELETE
    deleteWidgets();
    #endif
  }

//------------------------------
public:
//------------------------------

  virtual const char*   getName()         { return MName; }
  virtual const char*   getLabel()        { return MLabel; }
  virtual int32_t       getIndex()        { return MIndex; }
  
  //virtual void*         getParameter()    { return MParameter; }
  virtual void*         getParameter(uint32_t AIndex=0) { return MParameters[AIndex]; }
  
  virtual KODE_Rect     getRect()         { return MRect; }
  virtual KODE_Rect     getContentRect()  { return MContentRect; }
  virtual KODE_Rect     getInitialRect()  { return MInitialRect; }
  virtual KODE_Point    getResized()      { return MResized; }
  virtual float         getValue()        { return MValue; }
  virtual int32_t       getCursor()       { return MCursor; }
  virtual const char*   getHint()         { return MHint; }
  virtual uint32_t      getAlignment()    { return MAlignment; }
  virtual KODE_Widget*  getParent()       { return MParent; }
  virtual KODE_Rect     getMargins()      { return MMargins; }
  virtual KODE_Rect     getPadding()      { return MPadding; }
  virtual KODE_Point    getSpacing()      { return MSpacing; }
  virtual KODE_Point    getOffset()       { return MOffset; }
  virtual float         getXOffset()      { return MOffset.x; }
  virtual float         getYOffset()      { return MOffset.y; }
  virtual KODE_Point    getMinSize()      { return MMinSize; }
  virtual float         getMinWidth()     { return MMinSize.w; }
  virtual float         getMinHeight()    { return MMinSize.h; }
  virtual KODE_Point    getMaxSize()      { return MMaxSize; }
  virtual float         getMaxWidth()     { return MMaxSize.w; }
  virtual float         getMaxHeight()    { return MMaxSize.h; }

  //virtual void  setParameter(void* AParameter)  { MParameter = AParameter; }
  virtual void  setParameter(void* AParameter, uint32_t AIndex=0)  { MParameters[AIndex] = AParameter; }
  
  virtual void  setValue(float AValue)          { MValue = AValue; }
  virtual void  setIndex(int32_t AIndex )       { MIndex = AIndex; }
  virtual void  setCursor(int32_t ACursor)      { MCursor = ACursor; }
  virtual void  setName(const char* AName)      { MName = AName; }
  virtual void  setAlignment(uint32_t AAlign)   { MAlignment = AAlign; }
  virtual void  setRect(KODE_Rect ARect)        { MRect = ARect; }
  virtual void  setResized(KODE_Point APoint)   { MResized = APoint; }
  virtual void  setMargins(KODE_Rect ARect)     { MMargins = ARect; }
  virtual void  setPadding(KODE_Rect ARect)     { MPadding = ARect; }
  virtual void  setSpacing(KODE_Point APoint)   { MSpacing = APoint; }
  virtual void  setOffset(KODE_Point APoint)    { MOffset = APoint; }
  virtual void  setXOffset(float x)             { MOffset.x = x; }
  virtual void  setYOffset(float y)             { MOffset.y = y; }
  virtual void  setMinSize(KODE_Point APoint)   { MMinSize = APoint; }
  virtual void  setMinWidth(float x)            { MMinSize.x = x; }
  virtual void  setMinHeight(float y)           { MMinSize.y = y; }
  virtual void  setMaxSize(KODE_Point APoint)   { MMaxSize = APoint; }
  virtual void  setMaxWidth(float x)            { MMaxSize.x = x; }
  virtual void  setMaxHeight(float y)           { MMaxSize.y = y; }
  virtual void  setSpacing(float x, float y)    { MSpacing = KODE_Point(x,y); }
  virtual void  setOffset(float x, float y)     { MOffset = KODE_Point(x,y); }
  
  virtual void  setMargins(float l, float t, float r, float b)  { MMargins = KODE_Rect(l,t,r,b); }
  virtual void  setPadding(float l, float t, float r, float b)  { MPadding = KODE_Rect(l,t,r,b); }

  virtual void  setFlags(uint32_t AFlags)       { MFlags = AFlags; }
  virtual void  setFlag(uint32_t AFlag)         { MFlags |= AFlag; }
  virtual void  clearFlag(uint32_t AFlag)       { MFlags &= ~AFlag; }
  virtual bool  hasFlag(uint32_t AFlag)         { return MFlags & AFlag; }

//------------------------------
public:
//------------------------------

  /*
    clashes with KODE_BaseWIndoe.setPos/Size..
  */

  virtual void setPos(float x, float y) {
    MRect.x = x;
    MRect.y = y;
  }

  //----------

  virtual void setSize(float w, float h) {
    MRect.w = w;
    MRect.h = h;
    MResized.w = 0;
    MResized.h = 0;
  }

  //----------

  virtual void addPos(float x, float y) {
    MRect.x += x;
    MRect.y += y;
  }

  //----------

  virtual KODE_Widget* appendWidget(KODE_Widget* AWidget) {
    uint32_t index = MChildren.size();
    AWidget->MIndex = index;
    AWidget->MParent = this;
    //MChildren.push_back(AWidget);
    MChildren.append(AWidget);
    return AWidget;
  }

  //----------

  virtual void deleteWidgets() {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      delete MChildren[i];
      MChildren[i] = KODE_NULL;
    }
    MChildren.clear();
  }

  //----------

  virtual KODE_Widget* getChild(uint32_t AIndex) {
    return MChildren[AIndex];
  }

  //----------

  /*
    todo:
    - search from front, whice means backwards..

    returns:
    - null if outside of widget
    - child widget if inside
    - self (this) if not inside child widget
  */

  virtual KODE_Widget* findChild(float x, float y, bool children) {
    //if (!MRect.contains(x,y)) return KODE_NULL;
    
    //for (uint32_t i=MChildren.size()-1; i>=0; i--) {
    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Widget* widget = MChildren[i];
      
      if (widget->hasFlag(KODE_WIDGET_ACTIVE)) {
      
      KODE_Rect rect = widget->MRect;
      if (rect.contains(x,y)) {
        if (children) {
          KODE_Widget* child = widget->findChild(x,y,true);
          return child;
        }
        return widget;
      }
      
      }
      
    }
    return this;
  }

  //----------

  virtual KODE_Widget* findOpaqueParent() {
    if (hasFlag(KODE_WIDGET_OPAQUE)) return this;
    KODE_Widget* widget = MParent;
    while (widget) {
      if (widget->hasFlag(KODE_WIDGET_OPAQUE)) return widget;
      widget = widget->MParent;
    }
    return KODE_NULL;
  }

  //----------
  
  /*
  
     _____________________________________
    | v margins
    |    ______________
    |   |
    |   | padding
    |   | v  ___     ___
    |   |   |   |   |   |
    |   |   |___|   |___|
    |   |         ^
    |   |         spacing
  
  */
  
  virtual void realignChildren(bool ARecursive=true) {
    
    //if (!hasFlag(KODE_WIDGET_REALIGN)) return;

    MContentRect = KODE_Rect(MRect.x,MRect.y,0,0);
    MContentRect.pad(MPadding);

    //KODE_Rect initial_client_rect = MRect;
    
    KODE_Rect client_rect = MRect;
    
    //KODE_Trace("%s MRect %.0f %.0f %.0f %.0f\n",MName, MRect.x,MRect.y,MRect.w,MRect.h);
    
    client_rect.pad(MPadding);

    float stackx = 0;
    float stacky = 0;
    float stack_highest = 0;
    float stack_widest = 0;
    uint32_t prev_alignment = KODE_WIDGET_ALIGN_NONE;

    for (uint32_t i=0; i<MChildren.size(); i++) {
      KODE_Widget* child = MChildren[i];
      if (child->hasFlag(KODE_WIDGET_VISIBLE)) {
        //KODE_Rect new_client_rect = client_rect;
        KODE_Rect child_rect = child->MInitialRect;
        if (child_rect.x < 0) child_rect.x *= -MRect.w;
        if (child_rect.y < 0) child_rect.y *= -MRect.h;
        if (child_rect.w < 0) child_rect.w *= -MRect.w;
        if (child_rect.h < 0) child_rect.h *= -MRect.h;
        child_rect.x += client_rect.x;
        child_rect.y += client_rect.y;
        // resized is in pixels, and not scaled (negative factors)
        child_rect.w += child->MResized.w; // adjust for negative/percantages?)
        child_rect.h += child->MResized.h; // -"-
        //KODE_Trace("%s %.0f %.0f %.0f %.0f\n",child->getName(),child_rect.x,child_rect.y,child_rect.w,child_rect.h);
        uint32_t alignment = child->getAlignment();
        
//----------
        
        //  if we were stacking, but isn't now..
        
        if (prev_alignment == KODE_WIDGET_ALIGN_STACK_HORIZ) {
          if (alignment != KODE_WIDGET_ALIGN_STACK_HORIZ) {
            client_rect.y += (stacky + stack_highest + MSpacing.y);
          }
        }
        
        if (prev_alignment == KODE_WIDGET_ALIGN_STACK_VERT) {
          if (alignment != KODE_WIDGET_ALIGN_STACK_VERT) {
            //KODE_Trace("stackx %.0f stacky %.0f client_rect.x %.0f client_rect.y %.0f\n",stackx,stacky,client_rect.x,client_rect.y);
            client_rect.x += (stackx + stack_widest + MSpacing.x);
          }
        }
        
        prev_alignment = alignment;
        KODE_Rect new_client_rect = client_rect;

//----------
        
        switch (alignment) {

          case KODE_WIDGET_ALIGN_NONE:
            child_rect.x      -= client_rect.x;  // undo
            child_rect.y      -= client_rect.y;  // undo
            break;

          case KODE_WIDGET_ALIGN_PARENT:
            // already moved relative to parent
            //KODE_Trace("%.0f %.0f %.0f %.0f\n",MRect.x,MRect.y,MRect.w,MRect.h);
            //child_rect.x      -= client_rect.x;  // undo
            //child_rect.y      -= client_rect.y;  // undo
            break;

          case KODE_WIDGET_ALIGN_LEFT:
            child_rect.x       = client_rect.x;
            new_client_rect.x += child_rect.w + MSpacing.x;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_LEFT_FILL:
            child_rect.x       = client_rect.x;
            child_rect.y       = client_rect.y;
            child_rect.h       = client_rect.h;
            new_client_rect.x += child_rect.w + MSpacing.x;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_LEFT_TOP:
            child_rect.y       = client_rect.y;
            child_rect.x       = client_rect.x;
            new_client_rect.x += child_rect.w + MSpacing.x;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_LEFT_BOTTOM:
            child_rect.y       = client_rect.y + client_rect.h - child_rect.h;
            child_rect.x       = client_rect.x;
            new_client_rect.x += child_rect.w + MSpacing.x;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_RIGHT:
            child_rect.x       = client_rect.x + client_rect.w - child_rect.w;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_RIGHT_FILL:
            child_rect.x       = client_rect.x + client_rect.w - child_rect.w;
            child_rect.y       = client_rect.y;
            child_rect.h       = client_rect.h;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_RIGHT_TOP:
            child_rect.y       = client_rect.y;
            child_rect.x       = client_rect.x + client_rect.w - child_rect.w;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_RIGHT_BOTTOM:
            child_rect.y       = client_rect.y + client_rect.h - child_rect.h;
            child_rect.x       = client_rect.x + client_rect.w - child_rect.w;
            new_client_rect.w -= child_rect.w + MSpacing.x;
            break;

          case KODE_WIDGET_ALIGN_TOP:
            child_rect.y       = client_rect.y;
            new_client_rect.y += child_rect.h + MSpacing.y;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            break;

          case KODE_WIDGET_ALIGN_TOP_FILL:
            child_rect.y       = client_rect.y;
            child_rect.x       = client_rect.x;
            child_rect.w       = client_rect.w;
            new_client_rect.y += child_rect.h + MSpacing.y;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            break;

          case KODE_WIDGET_ALIGN_TOP_LEFT:
            child_rect.y       = client_rect.y;
            new_client_rect.y += child_rect.h + MSpacing.y;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            child_rect.x       = client_rect.x;
            break;

          case KODE_WIDGET_ALIGN_TOP_RIGHT:
            child_rect.y       = client_rect.y;
            new_client_rect.y += child_rect.h + MSpacing.y;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            child_rect.x       = client_rect.x + client_rect.w - child_rect.w;
            break;

          case KODE_WIDGET_ALIGN_BOTTOM:
            child_rect.y       = client_rect.y + client_rect.h - child_rect.h;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            break;

          case KODE_WIDGET_ALIGN_BOTTOM_FILL:
            child_rect.y       = client_rect.y + client_rect.h - child_rect.h;
            child_rect.x       = client_rect.x;
            child_rect.w       = client_rect.w;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            break;

          case KODE_WIDGET_ALIGN_BOTTOM_LEFT:
            child_rect.y       = client_rect.y + client_rect.h - child_rect.h;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            child_rect.x       = client_rect.x;
            break;

          case KODE_WIDGET_ALIGN_BOTTOM_RIGHT:
            child_rect.y       = client_rect.y + client_rect.h - child_rect.h;
            new_client_rect.h -= child_rect.h + MSpacing.y;
            child_rect.x       = client_rect.x + client_rect.w - child_rect.w;
            break;

          //case  KODE_WIDGET_ALIGN_STRETCH_HORIZONTAL:
          //  break;

          //case  KODE_WIDGET_ALIGN_STRETCH_VERTICAL:
          //  break;

          case KODE_WIDGET_ALIGN_CLIENT:
            child_rect         = client_rect;
            //new_client_rect.set(0);
            break;

          case KODE_WIDGET_ALIGN_CLIENT_FILL:
            child_rect         = client_rect;
            new_client_rect.set(0);
            break;
            
          case KODE_WIDGET_ALIGN_STACK_HORIZ:
            if ((stackx + child_rect.w /*+ MPadding.x*/) >= client_rect.w) {
              stackx           = 0;
              stacky          += stack_highest + MSpacing.y;
              stack_highest    = 0;
            }
            child_rect.x       = client_rect.x + stackx;
            child_rect.y       = client_rect.y + stacky;
            stackx            += child_rect.w + MSpacing.x;
            if (child_rect.h > stack_highest) stack_highest = child_rect.h;
            break;
            
          case KODE_WIDGET_ALIGN_STACK_VERT:
            if ((stacky + child_rect.h /*+ MPadding.y*/) >= client_rect.h) {
              stackx          += stack_widest + MSpacing.x;
              stacky           = 0;
              stack_widest     = 0;
            }
            child_rect.x       = client_rect.x + stackx;
            child_rect.y       = client_rect.y + stacky;
            stacky            += child_rect.h + MSpacing.y;
            if (child_rect.w > stack_widest) stack_widest = child_rect.w;
            break;
            
        }
        
        if (child_rect.w < child->getMinWidth()) child_rect.w = child->getMinWidth();
        if (child_rect.h < child->getMinHeight()) child_rect.h = child->getMinHeight();
        client_rect = new_client_rect;
        MContentRect.combine(child_rect);
        child_rect.pad(child->getMargins());
        child_rect.add(MOffset);
        //KODE_Trace("child_rect %s %.0f %.0f %.0f %.0f\n",child->getName(),child_rect.x,child_rect.y,child_rect.w,child_rect.h);
        child->setRect(child_rect);
        if (ARecursive) child->on_realign(ARecursive);
        //if (ARecursive) child->realignChildren(ARecursive);
        
      } // if visible
      //KODE_Trace("----------\n");
    } // for all children
    MContentRect.add(0,0,MPadding.right*2,MPadding.bottom*2);
    //KODE_Trace("MContentRect w %.2f h %.2f\n",MContentRect.w,MContentRect.h);
  }

  //----------

  virtual void paintChildren(KODE_Painter* APainter, KODE_Rect ARect) {
    //KODE_Trace("%s : %.0f %.0f %.0f %.0f\n",getName(),MRect.x,MRect.y,MRect.w,MRect.h);
    //KODE_Trace("%s : %.0f %.0f %.0f %.0f\n",getName(),APainter->getClipRect().x,APainter->getClipRect().y,APainter->getClipRect().w,APainter->getClipRect().h);
    if (MRect.isEmpty()) return;
    uint32_t num_children = MChildren.size();
    if (num_children == 0) return;
    if (hasFlag(KODE_WIDGET_CLIP)) APainter->pushClip(MRect);
    for (uint32_t i=0; i<num_children; i++) {
      KODE_Widget* child = MChildren[i];
      if (child->hasFlag(KODE_WIDGET_VISIBLE)) {
        if (ARect.intersects(child->MRect)) {
          //if (hasFlag(KODE_WIDGET_CLIP)) APainter->pushClip(MRect);
          child->on_paint(APainter,ARect);
          //if (hasFlag(KODE_WIDGET_CLIP)) APainter->popClip();
        }
      }
    }
    if (hasFlag(KODE_WIDGET_CLIP)) APainter->popClip();
  }

  //----------
  
  //virtual void offsetChildren(float AX, float AY, bool ARecursive=true) {
  //  for (uint32_t i=0; i<MChildren.size(); i++) {
  //    KODE_Widget* child = MChildren[i];
  //    child->setOffset(AX,AY);
  //    if (ARecursive) child->offsetChildren(AX,AY,ARecursive);
  //  }
  //}
  
  //----------

  virtual void redraw() {
    do_redraw(this,MRect);
  }

//------------------------------
public:
//------------------------------

  virtual void on_move(float AXpos, float AYpos) {
    setPos(AXpos,AYpos);
  }

  //----------

  virtual void on_resize(float AWidth, float AHeight) {
    //KODE_TRACE; // never printed.. ???
    setSize(AWidth,AHeight);
  }

  //----------

  virtual void on_realign(bool ARecursive=true) {
    //KODE_TRACE;
    //if (hasFlag(KODE_WIDGET_REALIGN))
    realignChildren(ARecursive);
  }

  //----------

  virtual void on_paint(KODE_Painter* APainter, KODE_Rect ARect) {
    paintChildren(APainter,ARect);
  }

  //----------

  virtual void on_keyPress(uint8_t AChar, uint32_t AKey, uint32_t AState) {
  }

  //----------

  virtual void on_keyRelease(uint8_t AChar, uint32_t AKey, uint32_t AState) {
  }

  //----------

  virtual void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
  }

  //----------

  virtual void on_mouseMove(float AXpos, float AYpos, uint32_t AState) {
  }

  //----------

  virtual void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) {
  }

  //----------

  virtual void on_enter(float AXpos, float AYpos, KODE_Widget* AFrom=KODE_NULL) {
  }

  //----------

  virtual void on_leave(float AXpos, float AYpos, KODE_Widget* AFrom=KODE_NULL) {
  }

  //----------

  virtual void on_timer() {
  }

  //----------

  virtual void on_idle() {
  }

  //----------

  virtual void on_clientMessage(uint32_t AData) {
  }

  //----------

  virtual void on_connect(KODE_Parameter* AParameter) {
    //MParameter = AParameter;
  }
  
  //----------
  
  virtual bool on_startDrag(uint32_t* AType, void** APtr) {
    *AType = 0;
    *APtr = KODE_NULL;
    return false;
  }
  
  //----------

  virtual void on_endDrag(KODE_Widget* AFrom, uint32_t AType, void* APtr) {
  }
  
  //----------

  virtual bool on_canDrop(uint32_t AType, void* APtr) {
    return false;
  }
  
  //----------

  //
  
  

//------------------------------
public:
//------------------------------

  virtual void do_notify(KODE_Widget* ASender, int32_t AMsg) {
    if (MParent) MParent->do_notify(ASender,AMsg);
  }

  //----------

  virtual void do_update(KODE_Widget* ASender) {
    if (MParent) MParent->do_update(ASender);
  }

  //----------

  virtual void do_redraw(KODE_Widget* ASender, KODE_Rect ARect) {
    if (MParent) MParent->do_redraw(ASender,ARect);
  }

  //----------

  virtual void do_moved(KODE_Widget* ASender, float AXpos, float AYpos) {
    if (MParent) MParent->do_moved(ASender,AXpos,AYpos);
  }

  //----------

  virtual void do_resized(KODE_Widget* ASender, float AWidth, float AHeight) {
    if (MParent) MParent->do_resized(ASender,AWidth,AHeight);
  }

  //----------

  virtual void do_setCursor(KODE_Widget* ASender, int32_t ACursor) {
    if (MParent) MParent->do_setCursor(ASender,ACursor);
  }

  //----------

  virtual void do_setCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) {
    if (MParent) MParent->do_setCursorPos(ASender,AXpos,AYpos);
  }

  //----------

  virtual void do_hint(KODE_Widget* ASender, const char* AHint) {
    if (MParent) MParent->do_hint(ASender,AHint);
  }

  //----------

  virtual void do_wantKeyEvents(KODE_Widget* ASender) {
    if (MParent) MParent->do_wantKeyEvents(ASender);
  }

  //----------

  virtual void do_setModal(KODE_Widget* ASender) {
    if (MParent) MParent->do_setModal(ASender);
  }

  //----------

  //virtual void do_startDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) {
  //  if (MParent) MParent->do_startDrag(ASender,AXpos,AYpos,AMode);
  //}

  //----------

  //virtual void do_endDrag(KODE_Widget* ASender, float AXpos, float AYpos, uint32_t AMode=0) {
  //  if (MParent) MParent->do_endDrag(ASender,AXpos,AYpos,AMode);
  //}  '
  
  //----------
  
  // ASender = sizer.MTarget

  virtual void do_sizer(KODE_Widget* ASender, float ADeltaX, float ADeltaY, uint32_t AMode) {
    
    //MResized.w += ADeltaX;
    //MResized.h += ADeltaY;
    
    float w = MInitialRect.w + MResized.w + ADeltaX;
    float h = MInitialRect.h + MResized.h + ADeltaY;
    //KODE_DTrace("MInitialRect.w %.0f MResized.w %.0f ADeltaX %.0f (MMinSize.w %.0f) -> w %.0f\n",MInitialRect.w,MResized.w,ADeltaX,MMinSize.w,w);
    if (w < MMinSize.w) MResized.w = MMinSize.w - MInitialRect.w;
    else if (w > MMaxSize.w) MResized.w = MMaxSize.w - MInitialRect.w;
    else MResized.w += ADeltaX;
    if (h < MMinSize.h) MResized.h = MMinSize.h - MInitialRect.h;
    else if (h > MMaxSize.h) MResized.h = MMaxSize.h - MInitialRect.h;
    else MResized.h += ADeltaY;
    
    do_resized( this, MInitialRect.w + MResized.w ,MInitialRect.h + MResized.h );
  }
  

};

//----------------------------------------------------------------------
#endif
