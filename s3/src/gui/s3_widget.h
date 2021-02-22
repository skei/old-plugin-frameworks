#ifndef s3_widget_included
#define s3_widget_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include "common/s3_array.h"
#include "common/s3_edges.h"
#include "common/s3_rect.h"
#include "gui/s3_painter.h"

//----------

class S3_Widget;
typedef S3_Array<S3_Widget*> S3_Widgets;

//----------

#define S3_WIDGET_CONSTRUCTOR(x) \
  x (S3_Rect ARect, uint32 AAlignment) : S3_Widget (ARect,AAlignment) {}

//----------------------------------------------------------------------

/*
  sizeLimit:
    x = minwidth
    y = minheight
    w = maxwidth
    h = maxheight
*/

struct s3_widget_layout {
  uint32  alignment;
  S3_Rect sizeLimit;
};

/*
  margins = around sub-widgets
  padding = between sub-widgets
*/

struct s3_container_layout {
  S3_Edges  margin;
  S3_Point  padding;
};

//----------------------------------------------------------------------

class S3_Widget {
  protected:
    const char*         MName;
    uint32              MFlags;
    int32               MIndex;
    int32               MCursor;
    const char*         MHint;
    float               MValue;
    S3_Widget*          MParent;
    S3_Widgets          MSubWidgets;
    S3_Rect             MRect;
    S3_Rect             MInitialRect;
    S3_Rect             MContentRect;     // calculated in alignWidgets()
    s3_widget_layout    MWidgetLayout;
    s3_container_layout MContainerLayout;
    int32               MStackedX;          // todo: MLayoutStackX?
    int32               MStackedY;          // todo: MLayoutStackXY
    void*               MParameter;
    S3_Widget*          MCapturedWidget;  // set by on_widget_mouseDown() & on_widget_mouseUp()
    S3_Widget*          MHoverWidget;     // set by on_widget_MouseMove()
    S3_Widget*          MModalWidget;     // set by S3_Window.do_widget_modal()
    S3_Widget*          MWantKeysWidget;  // set bu S3_Window.do_widget_wantKeys()
  //S3_Skin*            MSkin;
  public:
    S3_Widget(S3_Rect ARect, uint32 AAlignment=s3_wa_none);
    virtual ~S3_Widget();
  public:
    void        alignment(uint32 AAlignment)  { MWidgetLayout.alignment = AAlignment; }
    void        cursor(int32 ACursor)         { MCursor = ACursor; }
    void        flags(uint32 AFlags)          { MFlags = AFlags; }
    void        hint(const char* AHint)       { MHint = AHint; }
    void        index(int32 AIndex)           { MIndex = AIndex; }
    void        name(const char* AName)       { MName = AName; }
    void        sizeLimit(S3_Rect ALimit)     { MWidgetLayout.sizeLimit = ALimit; }
    virtual void value(float AValue)          { MValue = AValue; }
    S3_Rect     contentRect(void)             { return MContentRect; }
    uint32      flags(void)                   { return MFlags; }
    int32       index(void)                   { return MIndex; }
    const char* name(void)                    { return MName; }
    S3_Rect     rect(void)                    { return MRect; }
    float       value(void)                   { return MValue; }
    bool        hasFlag(uint32 AFlag)         { return (MFlags & AFlag); }
    void        clearFlag(uint32 AFlag)       { MFlags &= ~AFlag; }
    void        setFlag(uint32 AFlag)         { MFlags |= AFlag; }
    bool        hasFlags(uint32 AFlags)       { return ((MFlags & AFlags)==AFlags); }
    void        parameter(void* AParameter)   { MParameter = AParameter; }
    void*       parameter(void)               { return MParameter; }
  public: // container
    void        margins(S3_Edges AMargins)    { MContainerLayout.margin = AMargins; }
    void        padding(S3_Point APadding)    { MContainerLayout.padding = APadding; }
    uint32      numSubWidgets(void)           { return MSubWidgets.size(); }
    S3_Widgets* subWidgets(void)              { return &MSubWidgets; }
    S3_Widget*  parent(void)                  { return MParent; }
    S3_Widget*  subWidget(uint32 AIndex)      { return MSubWidgets[AIndex]; }
  public: // layout
    virtual void        margins(int32 ALeft, int32 ATop, int32 ARight, int32 ABottom);
    virtual void        margins(int32 AHoriz, int32 AVert);
    virtual void        margins(int32 AValue);
    virtual void        padding(int32 AHoriz, int32 AVert);
    virtual void        padding(int32 AValue);
  public:
    virtual int32       appendWidget(S3_Widget* AWidget);
    virtual S3_Widget*  appendWidgetW(S3_Widget* AWidget);
    virtual void        deleteWidgets(void);
    virtual S3_Widget*  findSubWidget(int32 AXpos, int32 AYpos);
    virtual bool        isChildOf(S3_Widget* AWidget);
    virtual S3_Widget*  getOwner(void);
    virtual void        alignWidgets(void);
  public: // parent -> children
    virtual void on_widget_reset(int32 AMode);
    virtual void on_widget_setPos(int32 AXpos, int32 AYpos);
    virtual void on_widget_setSize(int32 AWidth, int32 AHeight);
    virtual void on_widget_scroll(int32 ADeltaX, int32 ADeltaY);
    virtual void on_widget_align(void);
    virtual void on_widget_paint(S3_Painter* APainter, S3_Rect ARect, uint32 AMode);
    virtual void on_widget_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState);
    virtual void on_widget_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState);
    virtual void on_widget_mouseMove(int32 AXpos, int32 AYpos, int32 AState);
    virtual void on_widget_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState);
    virtual void on_widget_keyDown(int32 AKey, int32 AState);
    virtual void on_widget_keyUp(int32 AKey, int32 AState);
    virtual void on_widget_enter(int32 AXpos, int32 AYpos);
    virtual void on_widget_leave(int32 AXpos, int32 AYpos);
  public: // children -> parent
    virtual void do_widget_move(S3_Widget* AWidget, int32 AXpos, int32 AYpos);
    virtual void do_widget_resize(S3_Widget* AWidget, int32 AWidth, int32 AHeight);
    virtual void do_widget_sizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode);
    virtual void do_widget_redraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode);
    virtual void do_widget_update(S3_Widget* AWidget);
    virtual void do_widget_cursor(S3_Widget* AWidget, int32 ACursor);
    virtual void do_widget_cursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos);
    virtual void do_widget_hint(S3_Widget* AWidget, const char* AHint);
    virtual void do_widget_modal(S3_Widget* AWidget, int32 AMode=0);
    virtual void do_widget_wantKeys(S3_Widget* AWidget);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------

S3_Widget::S3_Widget(S3_Rect ARect, uint32 AAlignment) {
  //S3_Trace("S3_Widget\n");

  // base

  MName                     = "S3_Widget";
  MParent                   = S3_NULL;
  //MOwner                    = S3_NULL;
  MRect                     = ARect;
  MWidgetLayout.alignment   = AAlignment;
  //MWidgetLayout.sizeLimit   = S3_Rect(0,0,999999,999999);
  MWidgetLayout.sizeLimit   = S3_Rect(0,0,S3_INT32_MAX,S3_INT32_MAX);
  MValue                    = 0.0f;
  MFlags                    = s3_wf_active    // reacts to mouse/key events
                            + s3_wf_autoalign // auto-aligns its sub-widgets
                            + s3_wf_capture   // can capture the mouse
                            //+ s3_wf_clip    // bugs.. don't remember if it was xlib or gdi.. investigate..
                            //+ s3_wf_opaque  // initially the widget doesn't draw anything, so not opaque..
                            + s3_wf_visible;  // by default, the widget is visible
  MCursor                   = -1;//s3_mc_default;
  MHint                     = "S3_Widget";//S3_NULL;
  MIndex                    = -1;
  MInitialRect              = MRect;

  MParameter                = S3_NULL;

  // container

  MCapturedWidget           = S3_NULL;
  MHoverWidget              = S3_NULL;
  MModalWidget              = S3_NULL;
  MWantKeysWidget           = S3_NULL;
  MContainerLayout.margin   = S3_Edges(0);
  MContainerLayout.padding  = S3_Point(0);//SEdges(0);
  MContentRect              = S3_Rect(0);

  // layout

  MStackedX = 0;
  MStackedY = 0;

  //

  MRect.w = abs(MRect.w);
  MRect.h = abs(MRect.h);


  //MSkin = S3_NULL;

}

//----------

S3_Widget::~S3_Widget() {

  // container

  #ifndef S3_NO_AUTODELETE
  deleteWidgets();
  #endif

}

//------------------------------
//
//------------------------------



void S3_Widget::margins(int32 ALeft, int32 ATop, int32 ARight, int32 ABottom) {
  MContainerLayout.margin.left = ALeft;
  MContainerLayout.margin.top = ATop;
  MContainerLayout.margin.right = ARight;
  MContainerLayout.margin.bottom = ABottom;
}

void S3_Widget::margins(int32 AHoriz, int32 AVert) {
  MContainerLayout.margin.left = AHoriz;
  MContainerLayout.margin.top = AVert;
  MContainerLayout.margin.right = AHoriz;
  MContainerLayout.margin.bottom = AVert;
}

void S3_Widget::margins(int32 AValue) {
  MContainerLayout.margin.left = AValue;
  MContainerLayout.margin.top = AValue;
  MContainerLayout.margin.right = AValue;
  MContainerLayout.margin.bottom = AValue;
}

void S3_Widget::padding(int32 AHoriz, int32 AVert) {
  MContainerLayout.padding.x = AHoriz;
  MContainerLayout.padding.y = AVert;
}

void S3_Widget::padding(int32 AValue) {
  MContainerLayout.padding.x = AValue;
  MContainerLayout.padding.y = AValue;
}

//------------------------------
//
//------------------------------

int32 S3_Widget::appendWidget(S3_Widget* AWidget) {
  int32 index = MSubWidgets.size();
  AWidget->MParent = this;
  //AWidget->MOwner = MOwner;
  AWidget->MIndex = index;
  //if (MSkin && !AWidget->getSkin()) {
  //  AWidget->applySkin(MSkin,true);
  //}
  MSubWidgets.append(AWidget);
  return index;
}

//----------

S3_Widget* S3_Widget::appendWidgetW(S3_Widget* AWidget) {
  appendWidget(AWidget);
  return AWidget;
}

//----------

void S3_Widget::deleteWidgets(void) {
  for (uint32 i=0; i<MSubWidgets.size(); i++) {
    S3_Widget* w = MSubWidgets[i];
    MSubWidgets[i] = S3_NULL;
    //S3_Assert(w!=MHoverWidget);
    //S3_Assert(w!=MCapturedWidget);
    delete w;
  }
  MSubWidgets.clear();
}

//----------

//S3_Widget* subWidget(uint32 AIndex) {
//  return MSubWidgets[AIndex];
//}


//----------

/*
  find widget, or sub-widget, or sub-sub-..widget at X,Y
  (depth first traversal)
  scans backward in sub-widgets list
  (opposite of the painters algo we use for drawing)
  so we find the top-most widget, hopefully the one we clicked on
  if none found, returns self
*/

S3_Widget* S3_Widget::findSubWidget(int32 AXpos, int32 AYpos) {
  S3_Widget* wdg = this;
  //if (wdg->isChildOf(MModalWidget)) {
  //  DTrace("child of modal\n");
  //}
  int32 num = MSubWidgets.size();
  for (int32 i=num-1; i>=0; i--) {
    S3_Widget* w = MSubWidgets[i];

    //S3_Assert(w!=S3_NULL);

    //if (w->isChildOf(MModalWidget)) {

    //if (w->hasFlag(s3_wf_active+s3_wf_visible)) { // was swf_active only
    if (w->hasFlags(s3_wf_active+s3_wf_visible)) { // was swf_active only

      if (w->MRect.contains(AXpos,AYpos)) {
        wdg = w->findSubWidget(AXpos,AYpos);
        if (wdg != w) return wdg;
        else return w;
      } //contains
    } //active
    //} // ischildofhovewr
  } //for num
  //end; //num>0
  return wdg;
}

//----------

bool S3_Widget::isChildOf(S3_Widget* AWidget) {
  if (!AWidget) return false;
  if (AWidget==this) return false;
  S3_Widget* wdg = MParent;
  while (wdg) {
    if (wdg==AWidget) return true;
    wdg = wdg->MParent;
  }
  return false;
}

//----------

S3_Widget* S3_Widget::getOwner(void) {
  S3_Widget* parent = this;
  while (parent->MParent) parent = parent->MParent;
  return parent;
}

//----------

//void resetHover(bool ASub=true) {
//  if (ASub) {
//    for (uint32 i=0; i<MSubWidgets.size(); i++) MSubWidgets[i]->resetHover(ASub);
//  }
//  MHoverWidget = S3_NULL;
//}

//----------

//void resetCapture(bool ASub=true) {
//  if (ASub) {
//    for (uint32 i=0; i<MSubWidgets.size(); i++) MSubWidgets[i]->resetCapture(ASub);
//  }
//  MCapturedWidget = S3_NULL;
//}

//----------

//S3_Skin* getSkin(void) {
//  return MSkin;
//}

//----------

//void applySkin(S3_Skin* ASkin, bool AChildren=true) {
//  MSkin = ASkin;
//  if (AChildren) {
//    for (uint32 i=0; i<MSubWidgets.size(); i++) {
//      MSubWidgets[i]->applySkin(ASkin,AChildren);
//    }
//  }
//}

//------------------------------
//
//------------------------------

//TODO: /optimize
// should there be a way to detect, and signal 'no change'?
// so we can avoid unnecessary redraws..

/*
  align subwidgets (not itself)..
  keep track of rect encapsulating each widgets content (sub-widgets),
  so we can resize widgets depending on their content,
  or resize scrollbar size, etc..
*/

/*
  M = margin
  P = padding (between widgets when stacked)

   ________________________________________ _ _          _ ____
  |          ^                                                  |
  |          M                                                  |
  |      ____v___       ________       ____ _            _      |
  |     |        |     |        |     |                   |     |
  |<-M->|        |<-P->|        |<-P->|                   |<-M->|
  |     |________|     |________|     |______ _       _ __|     |
  |          ^                                                  :
  |          P
  |      ____v____      ___ _
  |     |         |    |
  |     :         .
  |

  TODO:
  - skip widget if not enough space for it?
    (null or negative space left)
  - break up this (too large) function into smaller pieces
    to make it easier to follow and see any overview...
  - sort widgets by x/y if necessary? (moving/resizing widgets)
*/

void S3_Widget::alignWidgets(void) {

  //KTrace("KWidget.align\n");
  if (hasFlag(s3_wf_autoalign)) {
    S3_Rect parent = MRect;
    parent.add(
        MContainerLayout.margin.left,
        MContainerLayout.margin.top,
      -(MContainerLayout.margin.left+MContainerLayout.margin.right),
      -(MContainerLayout.margin.top+MContainerLayout.margin.bottom)
    );
    S3_Rect client    = parent;
    MContentRect    = S3_Rect(MRect.x,MRect.y,0,0); //set(MRect.x,MRect.y,0,0);
    int32 stackx    = client.x;
    int32 stacky    = client.y;
    int32 largestw  = 0;
    int32 largesth  = 0;
    int32 remain    = 0;
    int32 diff      = 0;
    int32 prevx     = 0;
    int32 currx     = 0;
    int32 prevy     = 0;
    int32 curry     = 0;

    //if (MSubWidgets.size() > 0) {

      for (uint32 i=0; i<MSubWidgets.size(); i++) {

        S3_Widget* widget = MSubWidgets[i];

        int32 widgetwidth  = widget->MRect.w;  // current widget width
        int32 widgetheight = widget->MRect.h;  // height

        // if width or height is negative, treat is as percentage
        // of available client space..

        if (widget->MInitialRect.w < 0) {
          int32 w = abs(widget->MInitialRect.w);
          //widgetwidth  = client.w * w / 100;
          widgetwidth  = parent.w * w / 100;
          // on_widget_SetSize checks min/max sizes, and sets MRect.w/h
          widget->MRect.w = widgetwidth;
        }

        if (widget->MInitialRect.h < 0) {
          int32 h = abs(widget->MInitialRect.h);
          //widgetheight  = client.h * h / 100;
          widgetheight  = parent.h * h / 100;
          widget->MRect.h = widgetheight;
        }

        switch (widget->MWidgetLayout.alignment) {

          //  _____
          // |  _  |
          // | |X| |
          // |_____|
          //
          case s3_wa_none:
              /*
                should this one be called swa_relative or something?
                swa_parentRelative
              */
              widget->on_widget_setPos(abs(widget->MInitialRect.x)+parent.x, abs(widget->MInitialRect.y)+parent.y);
              break;

          //   _____
          //  |XXXXX|
          //  |XXXXX|
          //  |XXXXX|
          //
          case s3_wa_client: // overlay
            widget->on_widget_setPos(client.x,client.y);
            widget->on_widget_setSize(client.w,client.h);
            break;

          //   _____
          //  |XXXXX|
          //  |XXXXX|
          //  |XXXXX|
          //
          case s3_wa_fillClient:
            widget->on_widget_setPos(client.x,client.y);
            widget->on_widget_setSize(client.w,client.h);
            client.pos(parent.x,parent.y);
            client.size(0,0);
            break;

          //  ______
          // |X|
          // |X|
          // |X|____
          //
          case s3_wa_fillLeft:
            widget->on_widget_setPos(client.x,client.y);
            widget->on_widget_setSize(widgetwidth,client.h);
            client.x = client.x + (widgetwidth+MContainerLayout./*padding.left*/padding.x);
            client.w = client.w - (widgetwidth+MContainerLayout./*padding.left*/padding.x);
            stackx = client.x;
            stacky = client.y;
            largesth = 0;
            break;

          //  _____
          //     |X|
          //     |X|
          //  ___|X|
          //
          case s3_wa_fillRight:
            widget->on_widget_setPos(client.x2()-widgetwidth+1,client.y);
            widget->on_widget_setSize(widgetwidth,client.h);
            client.w = client.w - (widgetwidth+MContainerLayout.padding.x);
            break;

          //  _____
          // |XXXXX|
          // |     |
          // |     |
          //
          case s3_wa_fillTop:
            widget->on_widget_setPos(client.x,client.y);
            widget->on_widget_setSize(client.w,widgetheight);
            client.y = client.y + (widgetheight+MContainerLayout.padding.y);
            client.h = client.h - (widgetheight+MContainerLayout.padding.y);
            stackx = client.x;
            stacky = client.y;
            largestw = 0;
            break;

          //
          // |     |
          // |_____|
          // |XXXXX|
          //
          case s3_wa_fillBottom:
            widget->on_widget_setPos(client.x,client.y2()-widgetheight+1);
            widget->on_widget_setSize(client.w,widgetheight);
            client.h = client.h - (widgetheight+MContainerLayout.padding.y);
            break;

          //  _____
          // |_.
          // |X|
          // |_.___
          //


          case s3_wa_left:
            /*
              two widget->on_widget_SePos ???
            */
            //widget->on_widget_SetPos(client.x,client.y);
            widget->on_widget_setPos(client.x,abs(widget->MInitialRect.y)+parent.y); // setsize ???
            client.x = client.x + (widgetwidth+MContainerLayout.padding.x);
            client.w = client.w - (widgetwidth+MContainerLayout.padding.x);
            stackx = client.x;
            stacky = client.y;
            largestw = 0;
            break;

          //  _____
          // |X|
          // | .
          // |_.___
          //
          case s3_wa_leftTop:
            widget->on_widget_setPos(client.x,client.y);
            client.x = client.x + (widgetwidth+MContainerLayout.padding.x);
            client.w = client.w - (widgetwidth+MContainerLayout.padding.x);
            stackx = client.x;
            stacky = client.y;
            largestw = 0;
            break;

          //  _____
          // | .
          // |_.
          // |X|___
          //
          case s3_wa_leftBottom:
            widget->on_widget_setPos(  client.x, client.y2()-widgetheight+1 );
            client.x = client.x + (widgetwidth + MContainerLayout.padding.x);
            client.w = client.w - (widgetwidth + MContainerLayout.padding.x);
            stackx = client.x;
            stacky = client.y;
            break;

          //  _____
          //     ._|
          //     |X|
          //  ___._|
          //
          case s3_wa_right:
            widget->on_widget_setPos(  client.x2()-widgetwidth+1,abs(MInitialRect.y)+parent.y );
            client.w = client.w - (widgetwidth + MContainerLayout.padding.x);
            break;

          //  _____
          //     |X|
          //     . |
          //  ___._|
          //
          case s3_wa_rightTop:
            widget->on_widget_setPos(  client.x2()-widgetwidth+1, client.y );
            client.w = client.w - (widgetwidth + MContainerLayout.padding.x);
            break;

          //  _____
          //     . |
          //     ._|
          //  ___|X|
          //
          case s3_wa_rightBottom:
            widget->on_widget_setPos(  client.x2()-widgetwidth+1, client.y2()-widgetheight+1 );
            client.w = client.w - (widgetwidth + MContainerLayout.padding.x);
            break;

          //  _____
          // |.|X|.|
          // |     |
          // |     |
          //
          case s3_wa_top:
            widget->on_widget_setPos(abs(MInitialRect.x)+parent.x,client.y);
            client.y = client.y + (widgetheight+MContainerLayout.padding.y);
            client.h = client.h - (widgetheight+MContainerLayout.padding.y);
            stackx = client.x;
            stacky = client.y;
            break;

          //  _____
          // |X|.. |
          // |     |
          // |     |
          //
          case s3_wa_topLeft:
            widget->on_widget_setPos(client.x,client.y);
            client.y = client.y + (widgetheight+MContainerLayout.padding.y);
            client.h = client.h - (widgetheight+MContainerLayout.padding.y);
            stackx = client.x;
            stacky = client.y;
            break;

          //  _____
          // |.. |X|
          // |     |
          // |     |
          //
          case s3_wa_topRight:
            widget->on_widget_setPos(  client.x2()-widgetwidth+1, client.y );
            client.y = client.y + (widgetheight + MContainerLayout.padding.y);
            client.h = client.h - (widgetheight + MContainerLayout.padding.y);
            stackx = client.x;
            stacky = client.y;
            break;

          //
          // |     |
          // |. _..|
          // |_|X|_|
          //
          case s3_wa_bottom:
            widget->on_widget_setPos(  abs(MInitialRect.x)+parent.x, client.y2()-widgetheight+1 );
            //client.y += (wh + mPaddingY);
            client.h = client.h - (widgetheight + MContainerLayout.padding.y);
            break;

          //
          // |     |
          // |_ ...|
          // |X|___|
          //
          case s3_wa_bottomLeft:
            widget->on_widget_setPos(  client.x, client.y2()-widgetheight+1 );
            //client.y += (wh + mPaddingY);
            client.h = client.h - (widgetheight + MContainerLayout.padding.y);
            break;

          //
          // |     |
          // |... _|
          // |___|X|
          //
          case s3_wa_bottomRight:
            widget->on_widget_setPos(  client.x2()-widgetwidth+1, client.y2()-widgetheight+1 );
            //wdg->doSetSize( ww, client.h );
            client.h = client.h - (widgetheight + MContainerLayout.padding.y);
            break;

          //  __________________
          // |XXX_|XXX_|XX_|...... .
          // |
          // |
          //
          case s3_wa_stackedHoriz:
            remain = client.x2() - stackx + 1;
            if (remain >= widgetwidth) {
              // enough space
              widget->on_widget_setPos(stackx,stacky);
              stackx = stackx + (widgetwidth+MContainerLayout.padding.x);
              if (widgetheight > largesth) largesth = widgetheight;
              prevy = client.y;
              curry = stacky + (largesth+MContainerLayout.padding.y);
              diff = curry-prevy;
              client.y = curry;
              if (diff > 0) client.h = client.h - diff;
            }
            else {
              // not enougb space
              stackx = client.x;
              stacky = stacky + (largesth+MContainerLayout.padding.y);
              largesth = widgetheight;
              widget->on_widget_setPos(stackx,stacky);
              client.y = client.y + (largesth+MContainerLayout.padding.y);
              client.h = client.h - (largesth+MContainerLayout.padding.y);
              stackx = stackx + (widgetwidth+MContainerLayout.padding.x);
            }
            break;


          //  ________
          // |XX_|
          // |XX_|
          // |XX_|
          // |   |
          // |   :
          //     .
          case s3_wa_stackedVert:
            remain = client.y2() - stacky + 1;
            if (remain >= widgetheight) {
              // enough space
              widget->on_widget_setPos(stackx,stacky);
              stacky = stacky + (widgetheight+MContainerLayout.padding.y);
              if (widgetwidth > largestw) largestw = widgetwidth;
              prevx = client.x;
              currx = stackx + (largestw+MContainerLayout.padding.x);
              diff = currx-prevx;
              client.x = currx;
              if (diff > 0) client.w = client.w - diff;
            }
            else {
              // not enougb space
              stackx = stackx + (largestw+MContainerLayout.padding.x);
              stacky = client.y;
              largestw = widgetwidth;
              widget->on_widget_setPos(stackx,stacky);
              client.x = client.x + (largestw+MContainerLayout.padding.x);
              client.w = client.w - (largestw+MContainerLayout.padding.x);
              stacky = stacky + (widgetheight+MContainerLayout.padding.y);
            }
            break;
        } // case

        MContentRect.combine( widget->MRect ); // keep track of outer boundary
        widget->on_widget_align();
      } // for
      MContentRect.add(
        0,
        0,
        (MContainerLayout.margin.left),
        (MContainerLayout.margin.top)
      );

    //} //sub.size >0

  } // swf_autoalign
}

//------------------------------
// S3_WidgetListener
// (parent -> children)
//------------------------------

void S3_Widget::on_widget_reset(int32 AMode) {
  for (uint32 i=0; i<MSubWidgets.size(); i++) {
    MSubWidgets[i]->on_widget_reset(AMode);
  }
}

//----------

void S3_Widget::on_widget_setPos(int32 AXpos, int32 AYpos) {
  int32 dx = AXpos - MRect.x;
  int32 dy = AYpos - MRect.y;
  for (uint32 i=0; i<MSubWidgets.size(); i++) {
    S3_Widget* widget = MSubWidgets[i];
    int32 wx = widget->MRect.x;
    int32 wy = widget->MRect.y;
    widget->on_widget_setPos( wx+dx, wy+dy );
  }
  //S3_Widget::on_widget_SetPos(AXpos,AYpos);
  MRect.x = AXpos;
  MRect.y = AYpos;

}

//----------

void S3_Widget::on_widget_setSize(int32 AWidth, int32 AHeight) {
  if ((MWidgetLayout.sizeLimit.x>=0) && (AWidth  < MWidgetLayout.sizeLimit.x)) AWidth  = MWidgetLayout.sizeLimit.x;
  if ((MWidgetLayout.sizeLimit.w>=0) && (AWidth  > MWidgetLayout.sizeLimit.w)) AWidth  = MWidgetLayout.sizeLimit.w;
  if ((MWidgetLayout.sizeLimit.y>=0) && (AHeight < MWidgetLayout.sizeLimit.y)) AHeight = MWidgetLayout.sizeLimit.y;
  if ((MWidgetLayout.sizeLimit.h>=0) && (AHeight > MWidgetLayout.sizeLimit.h)) AHeight = MWidgetLayout.sizeLimit.h;
  MRect.w = AWidth;
  MRect.h = AHeight;
  // should sub-widgets be told to resize too???
}

//----------

void S3_Widget::on_widget_scroll(int32 ADeltaX, int32 ADeltaY) {
  for (uint32 i=0; i<MSubWidgets.size(); i++) {
    S3_Widget* wdg = MSubWidgets[i];
    int32 wx = wdg->MRect.x;
    int32 wy = wdg->MRect.y;
    wdg->on_widget_setPos( wx+ADeltaX, wy+ADeltaY );
  }
}

//----------

//virtual // S3_WidgetListener
//void S3_Widget::on_widget_Move(int32 AXdelta, int32 AYdelta) {
//}

//----------

//virtual // S3_WidgetListener
//void S3_Widget::on_widget_Resize(int32 AWidth, int32 AHeight) {
//}

//----------

void S3_Widget::on_widget_align(void) {
  alignWidgets();
}

//----------

void S3_Widget::on_widget_paint(S3_Painter* APainter, S3_Rect ARect, uint32 AMode) {

  if (ARect.isEmpty()) return;
  if (MRect.isEmpty()) return;
  // these has already been checked by parent widget (in on_widget_Paint)
  //if hasFlag(swf_visible) then
  //if FRect.intersect(ARect) then
  // clip
  if (MFlags&s3_wf_clip) APainter->pushClip(MRect);
  // paint children
  for (uint32 i=0; i<MSubWidgets.size(); i++) {
    S3_Widget* wdg = MSubWidgets[i];
    if (wdg->hasFlag(s3_wf_visible)) {
      //vis = ACanvas.visibleIntersection(wdg.FRect);
      //if not vis.empty then;
      if (wdg->MRect.isNotEmpty()) {
        if (wdg->MRect.intersects(ARect)) {
          wdg->on_widget_paint(APainter,ARect,AMode);
        } // subwdg.intersect
      } // not empty
    } // subwdg.visible
  } // for subwdg
  // un-clip
  if (MFlags&s3_wf_clip) APainter->popClip();
}

//----------

/*
  TODO:
    if s3_wf_sizeable and "can resize" flag..
    resize & realign
*/

void S3_Widget::on_widget_mouseDown(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
  if (MModalWidget) {
    MModalWidget->on_widget_mouseDown(AXpos,AYpos,AButton,AState);
  }
  else {
    if (MCapturedWidget) {
      MCapturedWidget->on_widget_mouseDown(AXpos,AYpos,AButton,AState);
    }
    else {
      //if zwf_active in FFlags then
      if (hasFlag(s3_wf_active)) {
        S3_Widget* hover = findSubWidget(AXpos,AYpos);
        if (hover) {
          if (hover != this) {
            //if zwf_capture in FFlags then FCapturedWidget = hover;
            if (MFlags&s3_wf_capture) {
              MCapturedWidget = hover;
            }
            hover->on_widget_mouseDown(AXpos,AYpos,AButton,AState);
          } // !hover
        }
      } // active
    } // !capture
  } // modal
}

//----------

/*
  TODO:
    if s3_wf_sizeable and "can resize" flag..
    end "can resize"
*/

void S3_Widget::on_widget_mouseUp(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
  if (MModalWidget) {
    MModalWidget->on_widget_mouseUp(AXpos,AYpos,AButton,AState);
  }
  else {
    if (MCapturedWidget) {
      MCapturedWidget->on_widget_mouseUp(AXpos,AYpos,AButton,AState);
      MCapturedWidget = S3_NULL;
    } //capture
    else {
      if (hasFlag(s3_wf_active)) {
        S3_Widget* hover = findSubWidget(AXpos,AYpos);
        if (hover) {
          if (hover != this) {
            hover->on_widget_mouseUp(AXpos,AYpos,AButton,AState);
          }
        }
      } //active
    }
  } // modal
}

//----------

/*
  TODO:
    if s3_wf_sizeable and close to edge, set cursor & flag to indicate
    "can resize" (left/right/top/bottom)..
*/

void S3_Widget::on_widget_mouseMove(int32 AXpos, int32 AYpos, int32 AState) {
  if (MModalWidget) MModalWidget->on_widget_mouseMove(AXpos,AYpos,AState);
  else {
    if (MCapturedWidget) {
      MCapturedWidget->on_widget_mouseMove(AXpos,AYpos,AState);
    }
    else {

      /*
        why is this commented away?
        we don't want mouse move events for inactive widgets, do we?
      */

      //if (hasFlag(swf_active) {
        S3_Widget* hover = findSubWidget(AXpos,AYpos);
        //S3_Assert(hover!=S3_NULL);
        //if (hover) {
          if (hover != MHoverWidget) {
            if (MHoverWidget) {
              MHoverWidget->on_widget_leave(AXpos,AYpos);
            }
            hover->on_widget_enter(AXpos,AYpos);
            MHoverWidget = hover;
          }
          // make sure we don't call ourselves recursively!
          if (hover != this) {
            hover->on_widget_mouseMove(AXpos,AYpos,AState);
          }
        //} // hover
      //} // active
    }
  } // modal
}

//----------

void S3_Widget::on_widget_mouseDoubleClick(int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
  /*
    we need time stamp for events to make this work..
    if we get another click event within a specific time frame
    and maybe if mouse hasn't moved (more than a specific amount?)
    we consider it a double click..
    which means, when we receive the first, initial click, we must wait and
    see if we get another one soon, before we know it is a single click,
    or the first one of a double clock..
    or should we just send both types of events, and make the widget
    responsible for handling the difference?
  */
}

//----------

void S3_Widget::on_widget_keyDown(int32 AKey, int32 AState) {
  if (MWantKeysWidget) MWantKeysWidget->on_widget_keyDown(AKey,AState);
  //if (MModalWidget) MModalWidget->on_widget_KeyDown(AWidget,AKey,AState);
  else if (MCapturedWidget) MCapturedWidget->on_widget_keyDown(AKey,AState);
}

//----------

void S3_Widget::on_widget_keyUp(int32 AKey, int32 AState) {
  if (MWantKeysWidget) MWantKeysWidget->on_widget_keyUp(AKey,AState);
  //if (MModalWidget) MModalWidget->on_widget_KeyUp(AWidget,AKey,AState);
  else if (MCapturedWidget) MCapturedWidget->on_widget_keyUp(AKey,AState);
}

//----------

void S3_Widget::on_widget_enter(int32 AXpos, int32 AYpos) {
  if (MParent) {
    if (MCursor >= 0) MParent->do_widget_cursor(this,MCursor);
    if (MHint) MParent->do_widget_hint(this,MHint);
    //if (hasFlag(s3_wf_redrawHover)) MParent->do_widget_Redraw(MRect,s3_pm_enter);
  }
}

void S3_Widget::on_widget_leave(int32 AXpos, int32 AYpos) {
  if (MParent) {
    MParent->do_widget_cursor(this,s3_mc_default);
    MParent->do_widget_hint(this,"");
    //if (hasFlag(s3_wf_redrawHover)) MParent->do_widget_Redraw(MRect,s3_pm_leave);
  }
}

//void S3_Widget::on_widget_Timer(void) {
//  for (int32 i=0; i<MSubWidgets.size(); i++) {
//    S3_Widget* w = MSubWidgets[i];
//    if (w->hasFlag(swf_timer)) w->on_widget_Timer();
//  }
//}

//----------

//void S3_Widget::on_widget_Idle(void) {
//}

//----------

/*
  this worked, but seems to be a bit tricky..
  we receive both normal mouse move/up/down messages, and the tablet
  messages.. there should be a way of separating the events,
  so we could optionally ignore the tablet when handling the mouse..
  and/or vice versa..
*/

//------------------------------
// S3_WidgetListener
// children -> parent
//------------------------------

void S3_Widget::do_widget_move(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
  if (MParent) MParent->do_widget_move(AWidget,AXpos,AYpos);
}

//----------

void S3_Widget::do_widget_resize(S3_Widget* AWidget, int32 AWidth, int32 AHeight) {
  if (MParent) MParent->do_widget_resize(AWidget,AWidth,AHeight);
}

//----------

void S3_Widget::do_widget_sizer(S3_Widget* AWidget, int32 ADeltaX, int32 ADeltaY, int32 AMode) {
  if (AWidget==this) {
    on_widget_setSize(MRect.w+ADeltaX, MRect.h+ADeltaY);
  }
  if (MParent) MParent->do_widget_sizer(AWidget,ADeltaX,ADeltaY,AMode);
}

//----------

/*
  if widget is not opaque (see-through), we must draw parent first..
  TODO: redraw parent before child, until arect is fully filled ???
*/

void S3_Widget::do_widget_redraw(S3_Widget* AWidget, S3_Rect ARect, uint32 AMode) {
  //if (MParent) {
  //  //TODO: check if fills entire rect?
  //  if (AWidget->hasFlag(s3_wf_opaque)) {
  //    // && MRect fills ARect ??
  //    MParent->do_widget_Redraw(AWidget,ARect,AMode);
  //  }
  //  else MParent->do_widget_Redraw(ARect,AMode);
  //}
  if (MParent) MParent->do_widget_redraw(AWidget,ARect,AMode);
}

//----------

void S3_Widget::do_widget_update(S3_Widget* AWidget) {
  if (MParent) MParent->do_widget_update(AWidget);
}

//----------

void S3_Widget::do_widget_cursor(S3_Widget* AWidget, int32 ACursor) {
  if (MParent) MParent->do_widget_cursor(AWidget,ACursor);
}

//----------

void S3_Widget::do_widget_cursorPos(S3_Widget* AWidget, int32 AXpos, int32 AYpos) {
  if (MParent) MParent->do_widget_cursorPos(AWidget,AXpos,AYpos);
}

//----------

void S3_Widget::do_widget_hint(S3_Widget* AWidget, const char* AHint) {
  if (MParent) MParent->do_widget_hint(AWidget,AHint);
}

//----------

void S3_Widget::do_widget_modal(S3_Widget* AWidget, int32 AMode) {
  if (MParent) MParent->do_widget_modal(AWidget,AMode);
}

//----------

void S3_Widget::do_widget_wantKeys(S3_Widget* AWidget) {
  if (MParent) MParent->do_widget_wantKeys(AWidget);
}

//----------------------------------------------------------------------
#endif
