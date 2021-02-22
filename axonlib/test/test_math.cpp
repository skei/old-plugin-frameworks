//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#include "format/axFormat.h"
#include "par/parInteger.h"
#include "par/parFloat.h"
#include "gui/axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgSlider.h"

#include <math.h>

//---------------------------------------------------------------------

#define NUMTYPES 26

char* str_type[] =
{
  (char*)"n",
  (char*)"sinf",
  (char*)"axSin",
  (char*)"axSinf",
  (char*)"cosf",
  (char*)"axCos",
  (char*)"axCosf",
  (char*)"tanf",
  (char*)"axTan",
  (char*)"axTanf",
  (char*)"expf",
  (char*)"axExp",
  (char*)"axExpf",
  (char*)"logf",
  (char*)"axLog",
  (char*)"axLogf",
  (char*)"floorf",
  (char*)"axFloor",
  (char*)"ceilf",
  (char*)"axCeil",
  (char*)"roundf",
  (char*)"axRound",
  (char*)"abs",
  (char*)"axAbs",
  (char*)"signbit",
  (char*)"axSignBit"
};


//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPainter : public axWidget
{
  public:
    int type1,type2;
    float minx,miny,maxx,maxy;
    float nval;

  public:

    myPainter(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
      {
      }

    //----------

float calc(int type, float n)
  {
    switch (type)
    {
      case 0:   return nval;
      case 1:   return sinf(n);
      case 2:   return axSin(n);
      case 3:   return axSinf(n);
      case 4:   return cosf(n);
      case 5:   return axCos(n);
      case 6:   return axCosf(n);
      case 7:   return tanf(n);
      case 8:   return axTan(n);
      case 9:   return axTanf(n);
      case 10:  return expf(n);
      case 11:  return axExp(n);
      case 12:  return axExpf(n);
      case 13:  return logf(n);
      case 14:  return axLog(n);
      case 15:  return axLogf(n);
      case 16:  return floorf(n);
      case 17:  return axFloor(n);
      case 18:  return ceilf(n);
      case 19:  return axCeil(n);
      case 20:  return roundf(n);
      case 21:  return axRound(n);
      case 22:  return fabs(n);
      case 23:  return axAbs(n);
      case 24:  return signbit(n);
      case 25:  return axSignBit(n);
    }
    return 0;
  }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        // background

        aCanvas->setBrushColor( aCanvas->getColor(AX_GREY_DARK) );
        aCanvas->fillRect( mRect.x, mRect.y, mRect.x2(), mRect.y2() );

        aCanvas->setClipRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());

        float rangex = maxx-minx;
        float rangey = maxy-miny;
        float scalex = 0;
        float scaley = 0;
        float x0 = 0;
        float y0 = 0;

        // x axis

        aCanvas->setPenColor( aCanvas->getColor(AX_GREY) );
        if ((minx<0) && (rangex>0))
        {
          scalex = mRect.w / rangex;
          x0 = mRect.x + axAbs(minx) * scalex;
          aCanvas->drawLine(x0,mRect.y,x0,mRect.y2());
        }

        // y axis

        if ((miny<0) && (rangey>0))
        {
          scaley = mRect.h / rangey;
          y0 = mRect.y + axAbs(miny) * scaley;
          aCanvas->drawLine(mRect.x,y0,mRect.x2(),y0);
        }

        // math func
        int   i;
        float x,n1,n2;
        float px,cx;
        float py1,py2;
        float cy1,cy2;
        float stepx = rangex / mRect.w;

        x = minx;
        px = x;
        py1 = y0;
        py2 = y0;
        axColor red = aCanvas->getColor(AX_RED);
        axColor green = aCanvas->getColor(AX_GREEN);
        axColor yellow = aCanvas->getColor(AX_YELLOW);
        for (i=0; i<mRect.w; i++)
        {
          n1 = calc(type1,x);//axSinf(x);
          n2 = calc(type2,x);//axSinf(x);
          cx = mRect.x + i;
          cy1 = (int)(y0 - (n1*scaley));
          cy2 = (int)(y0 - (n2*scaley));
          //aCanvas->drawPoint( mRect.x+i, y0-(n*scaley) );
          aCanvas->setPenColor(red);
          aCanvas->drawLine( px,py1,cx,cy1 );
          //aCanvas->drawLine( px,y0,cx,cy1 );
          aCanvas->setPenColor(green);
          aCanvas->drawLine( px,py2,cx,cy2 );
          //aCanvas->drawLine( px,y0,cx,cy2 );
          px = cx;
          py1 = cy1;
          py2 = cy2;
          x += stepx;
        }
        aCanvas->clearClipRect();

      }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{

  private:
    axEditor*   w_Editor;
    myPainter*  w_Painter;

  public:

    myPlugin(axContext* aContext, int aFlags)
    : axFormat(aContext)
      {
        w_Editor = NULL;
        w_Painter = NULL;
        describe("test_gain_gui","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2);
        setupEditor(640,480);
        appendParameter( new parInteger(this,"type1","", 0,   0, NUMTYPES-1, str_type ) );
        appendParameter( new parInteger(this,"type2","", 0,   0, NUMTYPES-1, str_type ) );
        appendParameter( new parFloat(  this,"min x","",-10, -10,10 ) );
        appendParameter( new parFloat(  this,"max x","", 10, -10,10 ) );
        appendParameter( new parFloat(  this,"min y","",-10, -10,10 ) );
        appendParameter( new parFloat(  this,"max y","", 10, -10,10 ) );
        appendParameter( new parFloat(  this,"n",    "", 0,  -10,10  ) );
        //setupParameters();
        prepareParameters();
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        int index = aParameter->getIndex();
        float value = aParameter->getValue();
        if (w_Painter)
        {
          switch (index)
          {
            case 0: w_Painter->type1 = (int)value; break;
            case 1: w_Painter->type2 = (int)value; break;
            case 2: w_Painter->minx = value; break;
            case 3: w_Painter->maxx = value; break;
            case 4: w_Painter->miny = value; break;
            case 5: w_Painter->maxy = value; break;
            case 6: w_Painter->nval = value; break;
          }
          if (w_Editor) w_Editor->redrawAll();
        }
      }

    //----------------------------------------
    // gui
    //----------------------------------------

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"type1 - red"   ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"type2 - green" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"min x" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"max x" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"min y" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"max y" ) );
        editor->appendWidget( new wdgSlider( editor, axRect(128,20),wa_Top,"n"     ) );
        editor->connect( editor->getWidget(0), mParameters[0] );
        editor->connect( editor->getWidget(1), mParameters[1] );
        editor->connect( editor->getWidget(2), mParameters[2] );
        editor->connect( editor->getWidget(3), mParameters[3] );
        editor->connect( editor->getWidget(4), mParameters[4] );
        editor->connect( editor->getWidget(5), mParameters[5] );
        editor->connect( editor->getWidget(6), mParameters[6] );
        editor->appendWidget( w_Painter = new myPainter(editor,NULL_RECT,wa_Client) );
        editor->doRealign();

        transferParameters();

        editor->show();
        w_Editor = editor;
        return editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = w_Editor;
        w_Editor->hide();
        w_Editor = NULL;
        delete editor;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
