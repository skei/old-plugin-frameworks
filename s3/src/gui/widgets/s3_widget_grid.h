#ifndef s3_widget_grid_included
#define s3_widget_grid_included
//----------------------------------------------------------------------

class S3_Widget_Grid
: public S3_Widget {

  protected:
    int32     MWidth;       // columns
    int32     MHeight;      // rows
    S3_Color  MBackColor;
    S3_Color  MGridColor;
    bool      MDrawHorizLines;
    bool      MDrawVertLines;
    bool      MDrawCells;
    bool      MClickCells;

  public:

    int32 width(void)                     { return MWidth; }
    int32 height(void)                    { return MHeight; }
    void  width(int32 AWidth)             { MWidth = AWidth; }
    void  height(int32 AHeight)           { MHeight = AHeight; }
    void  backColor(S3_Color AColor)      { MBackColor = AColor; }
    void  gridColor(S3_Color AColor)      { MGridColor = AColor; }
    void  drawHorizLines(bool ADraw=true) { MDrawHorizLines = ADraw; }
    void  drawVertLines(bool ADraw=true)  { MDrawVertLines = ADraw; }
    void  drawCells(bool ADraw=true)      { MDrawCells = ADraw; }
    void  clickCells(bool AClick=true)    { MClickCells = AClick; }

  public:

    S3_Widget_Grid(S3_Rect ARect, int32 AWidth, int32 AHeight, uint32 AAlignment=s3_wa_none)
    : S3_Widget(ARect,AAlignment) {
      MName           = "S3_Widget_Grid";
      MHint           = "grid";
      MWidth          = AWidth;//3;
      MHeight         = AHeight;//3;
      //MCursor         = kmc_Finger;
      MBackColor      = S3_LightGrey;
      MGridColor      = S3_DarkGrey;
      MDrawHorizLines = true;
      MDrawVertLines  = true;
      MDrawCells      = true;
      MClickCells     = true;

      /*
        ouch..
        initCells calls virtual methods, in the constructor..
        is this dangerous?
        todo: postpone.. (user calls initCells() if needed..
      */

      //initCells();

    }

    //----------

    virtual ~S3_Widget_Grid() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual void on_gridInitCell(S3_Widget* AWidget, S3_Rect ARect, int32 AX, int32 AY) {}
    virtual void on_gridClickCell(S3_Widget* AWidget, int32 AX, int32 AY, int32 AB) {}
    virtual void on_gridPaintCell(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, int32 AX, int32 AY) {}

  //------------------------------
  //
  //------------------------------

  public:

    //virtual
    void initCells(void) {
      if ((MWidth > 0) && (MHeight > 0)) {
        float xcell = ( (float)MRect.w / (float)MWidth );
        float ycell = ( (float)MRect.h / (float)MHeight );
        for (int32 yy=0; yy<MHeight; yy++) {
          for (int32 xx=0; xx<MWidth; xx++) {
            S3_Rect R = S3_Rect( MRect.x+floorf(xx*xcell), MRect.y+floorf(yy*ycell), floorf(xcell), floorf(ycell) );
            on_gridInitCell(this,R,xx,yy);
          }
        }
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    /*
      TODO
      - only draw visible cells..
      - cells intersecting with update-rect
    */

    virtual
    void on_widgetPaint(S3_Widget* AWidget, S3_Painter* APainter, S3_Rect ARect, uint32 AMode=0) {
      S3_Widget::on_widgetPaint(AWidget,APainter,ARect,AMode);

      if ((MWidth > 0) && (MHeight > 0)) {
        float xcell = ( (float)MRect.w / (float)MWidth );
        float ycell = ( (float)MRect.h / (float)MHeight );

        // background
        //APainter->setFillColor(MBackColor);
        //APainter->fillRect(MRect.x,MRect.y,MRect.x2-1,MRect.y2-1);
        // cells

        if (MDrawCells) {
          for (int32 xx=0; xx<MWidth; xx++) {
            for (int32 yy=0; yy<MHeight; yy++) {
              S3_Rect R = S3_Rect( MRect.x+floorf(xx*xcell), MRect.y+floorf(yy*ycell), floorf(xcell), floorf(ycell) );
              on_gridPaintCell(this,APainter,R,xx,yy);
            }
          }
        }
        // grid-lines
        float x = (float)MRect.x + xcell - 1;
        float y = (float)MRect.y + ycell - 1;
        APainter->setDrawColor( MGridColor );
        if (MDrawVertLines) {
          if (MWidth > 1) {
            for (int32 i=0; i<MWidth-1; i++) {
              APainter->drawLine( floorf(x), MRect.y, floorf(x), MRect.y2() );
              x += xcell;
            }
          }
        }
        if (MDrawHorizLines) {
          if (MHeight > 1) {
            for (int32 i=0; i<MHeight-1; i++) {
              APainter->drawLine( MRect.x, floorf(y), MRect.x2(), floorf(y) );
              y += ycell;
            }
          }
        }
      }
      //APainter->setDrawColor(MBorderColor);
      //APainter->drawRect(MRect.x,MRect.y,MRect.x2{-1},MRect.y2{-1});
    }

    //----------

    virtual
    void on_widgetMouseDown(S3_Widget* AWidget, int32 AXpos, int32 AYpos, int32 AButton, int32 AState) {
      S3_Widget::on_widgetMouseDown(AWidget,AXpos,AYpos,AButton,AState);
      if (MClickCells) {
        float xcell = ( (float)MRect.w / (float)MWidth );
        float ycell = ( (float)MRect.h / (float)MHeight );
        int32 x = floorf( (float)(AXpos-MRect.x) / xcell );
        int32 y = floorf( (float)(AYpos-MRect.y) / ycell );
        on_gridClickCell(this,x,y,AButton);
        //do_widgetUpdate(this); // ???
      }
    }

  //----------

};

//----------------------------------------------------------------------
#endif

