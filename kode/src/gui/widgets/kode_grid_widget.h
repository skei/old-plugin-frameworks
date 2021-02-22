#ifndef kode_grid_widget_included
#define kode_grid_widget_included
//----------------------------------------------------------------------
/*

  todo:
  - selected x1,y1,x2,y2
    invert if necesary
    KODE_List<cells>
  - scroll?
  - cut, copy, paste
  - move/drag


*/
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_math.h"

class KODE_GridWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  int32_t     MWidth                = 0;       // columns
  int32_t     MHeight               = 0;      // rows
  KODE_Color  MBackgroundColor      = KODE_Grey;
  KODE_Color  MGridColor            = KODE_DarkGrey;
  bool        MDrawHorizontalLines  = true;
  bool        MDrawVerticalLines    = true;
  bool        MDrawCells            = true;
  bool        MClickCells           = true;
  bool        MSelectCell           = true;
  int32_t     MSelectedX            = -1;
  int32_t     MSelectedY            = -1;
  bool        MSelectCells          = true;
  bool        MIsDragging           = false;
  int32_t     MSelectedXcount       = 0;
  int32_t     MSelectedYcount       = 0;
  

//------------------------------
public:
//------------------------------

  int32_t     getWidth(void)                          { return MWidth; }
  int32_t     getHeight(void)                         { return MHeight; }
  void        setWidth(int32_t AWidth)                { MWidth = AWidth; }
  void        setHeight(int32_t AHeight)              { MHeight = AHeight; }
  void        setBackgroundColor(KODE_Color AColor)   { MBackgroundColor = AColor; }
  void        setGridColor(KODE_Color AColor)         { MGridColor = AColor; }
  void        setDrawHorizontalLines(bool ADraw=true) { MDrawHorizontalLines = ADraw; }
  void        setDrawVerticalLines(bool ADraw=true)   { MDrawVerticalLines = ADraw; }
  void        setDrawCells(bool ADraw=true)           { MDrawCells = ADraw; }
  void        setClickCells(bool AClick=true)         { MClickCells = AClick; }

//------------------------------
public:
//------------------------------

  KODE_GridWidget(KODE_Rect ARect, int32_t AWidth=4, int32_t AHeight=4)
  : KODE_Widget(ARect) {
    MName                 = "KODE_GridWidget";
    MHint                 = "grid";
    MWidth                = AWidth;
    MHeight               = AHeight;
    //MCursor               = KODE_CURSOR_FINGER;
    //MBackgroundColor      = KODE_LightGrey;
    //MGridColor            = KODE_DarkGrey;
    //MDrawHorizontalLines  = true;
    //MDrawVerticalLines    = true;
    //MDrawCells            = true;
    //MClickCells           = true;
    // initCells calls virtual methods, in the constructor..
    //initCells();
  }

  //----------

  virtual ~KODE_GridWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void on_initCell(KODE_Rect ARect, int32_t AX, int32_t AY) {}
  virtual void on_clickCell(int32_t AX, int32_t AY, int32_t AB) {}
  virtual void on_paintCell(KODE_Painter* APainter, KODE_Rect ARect, int32_t AX, int32_t AY) {}

//------------------------------
public:
//------------------------------

  virtual void initCells(void) {
    if ((MWidth > 0) && (MHeight > 0)) {
      float xcell = ( (float)MRect.w / (float)MWidth );
      float ycell = ( (float)MRect.h / (float)MHeight );
      for (int32_t yy=0; yy<MHeight; yy++) {
        for (int32_t xx=0; xx<MWidth; xx++) {
          KODE_Rect R = KODE_Rect(
            MRect.x+floorf(xx*xcell),
            MRect.y+floorf(yy*ycell),
                    floorf(   xcell),
                    floorf(   ycell)
          );
          on_initCell(/*this,*/R,xx,yy);
        }
      }
    }
  }
  
  //virtual void redrawCell(int32_t x, int32_t y) {
  //  float xcell = ( (float)MRect.w / (float)MWidth );
  //  float ycell = ( (float)MRect.h / (float)MHeight );
  //  MSelectedX = x;
  //  MSelectedY = y;
  //  //KODE_Trace("%.3f %.3f %.3f %.3f\n",x*xcell,y*ycell,xcell,ycell);
  //  do_redraw(this,KODE_Rect(x*xcell,y*ycell,xcell,ycell));
  //}

//------------------------------
public:
//------------------------------

  /*
    TODO
    - only draw visible cells..
    - cells intersecting with update-rect
  */

  void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
    KODE_Widget::on_paint(APainter,ARect);

    if ((MWidth > 0) && (MHeight > 0)) {
      float xcell = ( (float)MRect.w / (float)MWidth );
      float ycell = ( (float)MRect.h / (float)MHeight );

      // background
      //APainter->setFillColor(MBackgroundColor);
      //APainter->fillRectangle(MRect.x,MRect.y,MRect.x2()-1,MRect.y2()-1);

      // cells
      if (MDrawCells) {
        for (int32_t xx=0; xx<MWidth; xx++) {
          for (int32_t yy=0; yy<MHeight; yy++) {
            KODE_Rect R = KODE_Rect(
              MRect.x+floorf(xx*xcell),
              MRect.y+floorf(yy*ycell),
              floorf(xcell),
              floorf(ycell)
            );
            on_paintCell(/*this,*/APainter,R,xx,yy);
          }
        }
      }

      // grid-lines
      float x = (float)MRect.x + xcell - 1;
      float y = (float)MRect.y + ycell - 1;
      APainter->setDrawColor( MGridColor );
      if (MDrawVerticalLines) {
        if (MWidth > 1) {
          for (int32_t i=0; i<MWidth-1; i++) {
            //APainter->drawLine( floorf(x), MRect.y, floorf(x), MRect.y2() );
            APainter->drawVLine( floorf(x), MRect.y, MRect.y2() );
            x += xcell;
          }
        }
      }
      if (MDrawHorizontalLines) {
        if (MHeight > 1) {
          for (int32_t i=0; i<MHeight-1; i++) {
            APainter->drawHLine( MRect.x, floorf(y), MRect.x2() );
            y += ycell;
          }
        }
      }
      
      // selected cell(s)
      if ((MSelectedX >= 0) && (MSelectedY >= 0)) {
        x = xcell * MSelectedX;
        y = ycell * MSelectedY;
        float xn = xcell * MSelectedXcount;
        float yn = ycell * MSelectedYcount;
        float x2 = x + xn - 1;
        float y2 = y + yn - 1;
        //KODE_Trace("%.3f %.3f %.3f %.3f\n",x,y,xcell,ycell);
        x  += MRect.x;
        y  += MRect.y;
        x2 += MRect.x;
        y2 += MRect.y;
        APainter->setDrawColor(KODE_White);
        APainter->drawRectangle( x, y, x2, y2 );
      }
      
    }
  }

  //----------
  
  /*
    todo:
    * invert x/y - x2/y2 if needed
    * has selection -> move
    * clamp selection to grid and/or scroll grid
  */

  void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
    KODE_Widget::on_mouseClick(AXpos,AYpos,AButton,AState);
    
    if (MClickCells) {
      float xcell = ( (float)MRect.w / (float)MWidth );
      float ycell = ( (float)MRect.h / (float)MHeight );
      int32_t x = floorf( (float)(AXpos-MRect.x) / xcell );
      int32_t y = floorf( (float)(AYpos-MRect.y) / ycell );
      
      if (MSelectCell) {
        MSelectedX = x;
        MSelectedY = y;
        MSelectedXcount = 1;
        MSelectedYcount = 1;
        if (MSelectCells) {
          MIsDragging = true;
        }
        do_redraw(this,MRect);
      }
      
      on_clickCell(x,y,AButton);
      //do_widgetUpdate(this); // ???
    }
  }
  
  //----------

  void on_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
    KODE_Widget::on_mouseRelease(AXpos,AYpos,AButton,AState);
    MIsDragging = false;
  }

  //----------

  void on_mouseMove(float AXpos, float AYpos, uint32_t AState) override {
    KODE_Widget::on_mouseMove(AXpos,AYpos,AState);
    if (MIsDragging) {
      float xcell = ( (float)MRect.w / (float)MWidth );
      float ycell = ( (float)MRect.h / (float)MHeight );
      float x = floorf( (float)(AXpos-MRect.x) / xcell );
      float y = floorf( (float)(AYpos-MRect.y) / ycell );
      //KODE_Trace("%.3f %.3f %.3f %.3f\n",x,y,xcell,ycell);
      //KODE_Trace("selected: %.3f %.3f mouse: %.3f %.3f\n",(float)MSelectedX,(float)MSelectedY,x,y);
      MSelectedXcount = KODE_Max(1, (x - MSelectedX + 1));
      MSelectedYcount = KODE_Max(1, (y - MSelectedY + 1));
      //KODE_Trace("%.3f %.3f\n",(float)MSelectedXcount,(float)MSelectedYcount);
      do_redraw(this,MRect);
    }
  }
    
};

//----------------------------------------------------------------------
#endif
