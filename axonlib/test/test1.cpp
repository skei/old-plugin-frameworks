#define AX_DEBUG
#include "core/axDebug.h"

// #define _WIN32_WINNT 0x0501

#include "platform/axContext.h"
#include "axFormat.h"
#include "axEditor.h"
#include "gui/axContainer.h"
#include "gui/axBitmap.h"

//----------------------------------------------------------------------

/*

todo/ideas/thoughts/whatever:

- streamline the #define AX_DEBUG + #include "core/axDebug.h" things?
  just defining AX_DEBUG could be enough?
  or we could find a platform-independent way to see if we're in a debug build,
  to get rid of the needed AX_DEBUG altogether...
  #ifdef __DEBUG__ or something?

- window size is set in two places...
  setupEditor vs doOpenEditor/axEditor constructor

- brush, pen etc attached to canvas, lost when resizing editor/window
  because the surface (and thus the canvas) have to be reconstructor

- change color of pen/brush without creating a new one?
  linux: easy, no real pen, just saving the color, and use it in selectPen() etc
  win32: can we re-color the pen/brush in some way?

- drawing pixels with rgb values directly (create temporary pen, draw, delete,
  or re-color an existing stock one, if possible)

*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class myWidget : public /*axContainer*/axWidget
{
  public:
    axSurface*  mSurface;
    axBitmap*   mBitmap;
    axColor     mBackColor;

  public:
    myWidget(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None)
    : axWidget(aListener,aRect,aAlignment)
    //: axContainer(aListener,aRect,aAlignment)
      {
        mSurface = NULL;
      }
    virtual ~myWidget() {}
    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        aCanvas->setBrushColor(mBackColor);
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());
        aCanvas->drawImage(mSurface, 10,10, 0,0,200,200);
        aCanvas->drawBitmap(mBitmap, 10,220,0,0,100,100);
      }
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class test1 : public axFormat
{
  private:
    axContext   ctx();
    axEditor*   mEditor;
    myWidget*   widget;
    axSurface*  surface;
    axBitmap*   bitmap;

  public:
    test1(axContext* aContext)
    : axFormat(aContext, AX_PLUG_DEFAULT)
      {
        describe("test1","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(220,330);
        //setupParameters();
      }
    virtual ~test1()
      {
      }

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axColor   dark,light;
        axColor   red,green,yellow,white;
        axCanvas* canvas;

        mEditor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT/*AX_WIN_MSGDELETE*/);

        // --- widget ---

        widget  = new myWidget(mEditor,axRect(0,0,220,330));
        canvas = mEditor->getCanvas();
        widget->mBackColor = canvas->getColor(128,144,128);

        // --- bitmap ---

        int x,y;
        char* buf;
        bitmap = mEditor->createBitmap(100,100);
        buf = bitmap->createBuffer();
        for (x=0; x<100; x++)
        {
          for (y=0; y<100; y++)
          {
            *buf++ = (int)((float)y * 2.55);
            *buf++ = 0;
            *buf++ = (int)((float)x * 2.55);
            *buf++ = 0;
          }
        }
        bitmap->prepare();

        // --- surface ---

        surface = mEditor->createSurface(200,200);
        canvas  = surface->getCanvas();

        dark    = canvas->getColor(AX_GREY_DARK);
        light   = canvas->getColor(AX_GREY_LIGHT);
          canvas->setBrushColor(dark);
          canvas->fillRect(0,0,199,199);

        red     = canvas->getColor(AX_RED);
        green   = canvas->getColor(AX_GREEN);
        yellow  = canvas->getColor(AX_YELLOW);
        white   = canvas->getColor(AX_WHITE);

        canvas->setPenColor(red);
          canvas->drawCircle( 0, 0,  29, 29);
        canvas->setBrushColor(green);
          canvas->fillCircle(30, 0,  59, 29);

        canvas->setPenColor(red);
          canvas->drawRect(  60, 0,  89, 29);
        canvas->setBrushColor(green);
          canvas->fillRect(  90, 0, 119, 29);
        canvas->setPenColor(red);
          canvas->drawArc(  120, 0, 149, 29, 0, 0.3);
        canvas->setBrushColor(green);
          canvas->fillArc(  150, 0, 179, 29, 0, 0.6);

        canvas->setPenColor(white);
          for (int i=0; i<50; i++)
            canvas->drawPoint(0+axRandomInt(29),30+axRandomInt(29));

        canvas->setPenColor(yellow);
        canvas->setPenWidth(5);
          canvas->drawLine(  32, 32,  57, 57);
        canvas->resetPen();
        canvas->setPenColor(white);
          canvas->drawLine(  67, 32,  82, 57);
        //canvas->setPenWidth(DEF_PENWIDTH);

        canvas->setTextColor(yellow);
          canvas->drawText(90,30,"text");

        for (x=0; x<64; x++)
        {
          for (y=0; y<64; y++)
          {
            axColor col = canvas->getColor(0,x*4,y*4);
            canvas->drawPoint(100+x,100+y,col);
          }
        }

        widget->mBitmap = bitmap;
        widget->mSurface = surface;
        mEditor->appendWidget(widget);

        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {

        mEditor->hide();
        delete mEditor;
        mEditor = NULL;

        delete surface;
        delete bitmap;
      }

    //----------

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(test1)


