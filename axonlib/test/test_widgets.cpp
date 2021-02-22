#define AX_DEBUG
#define AX_DEBUG_AUTO_STD

#include "axFormat.h"
#include "axEditor.h"

#include "gui/axSymbols.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgSizer.h"
#include "wdg/wdgLabel.h"
#include "wdg/wdgButton.h"
#include "wdg/wdgSlider.h"
#include "wdg/wdgValue.h"
#include "wdg/wdgImage.h"
#include "wdg/wdgBitmap.h"
#include "wdg/wdgKnob.h"
#include "wdg/wdgScrollBar.h"
#include "wdg/wdgScrollBox.h"
#include "wdg/wdgGroupBox.h"

//----------------------------------------------------------------------
//
// some symbols
//
//----------------------------------------------------------------------

// move to axStdSymbols.h ?

char* symbol0 = (char*) "........"
                        "........"
                        "........"
                        ".OOOOOO."
                        "..OOOO.."
                        "..oOOo.."
                        "...OO..."
                        "........";
char* symbol1 = (char*) "........"
                        "........"
                        "........"
                        "...OO..."
                        "..oOOo.."
                        "..OOOO.."
                        ".OOOOOO."
                        "........";
char* symbol2 = (char*) "........"
                        "....O..."
                        "..oOO..."
                        ".OOOO..."
                        ".OOOO..."
                        "..oOO..."
                        "....O..."
                        "........";
char* symbol3 = (char*) "........"
                        "...O...."
                        "...OOo.."
                        "...OOOO."
                        "...OOOO."
                        "...OOo.."
                        "...O...."
                        "........";
char* symbol4 = (char*) "O.O.O.O."
                        ".O.O.O.O"
                        "O.O.O.O."
                        ".O.O.O.O"
                        "O.O.O.O."
                        ".O.O.O.O"
                        "O.O.O.O."
                        ".O.O.O.O";
char* symbol5 = (char*) "........"
                        ".OOOOOO."
                        ".OOOOOO."
                        ".OOOOOO."
                        ".OOOOOO."
                        ".OOOOOO."
                        ".OOOOOO."
                        "........";
char* symbol6 = (char*) "...OO..."
                        "..OOOO.."
                        ".OOOOOO."
                        "........"
                        "........"
                        ".OOOOOO."
                        "..OOOO.."
                        "...OO...";
char* symbol7 = (char*) "OO...OO."
                        ".OO.OO.."
                        "..OOO..."
                        "..oOo..."
                        "..OOO..."
                        ".OO.OO.."
                        "OO...OO."
                        "........";
char* symbol8 = (char*) ".....oO."
                        ".....Oo."
                        "....oO.."
                        "Oo..Oo.."
                        "OO.oO..."
                        ".OOOo..."
                        "..OO...."
                        "........";

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  friend class myPlugin;
  private:
    int           mNumTimer;
    char          timerText[32];
    char          label_buf[256][8];
    axBitmap*     mBitmap;
    axBitmap*     mBitmap2;
    axSurface*    mImgSurf;
    axSurface*    mSymSurf;
    axSymbols*    mSymbols;
    wdgPanel*     wClient;
    wdgSizer*     wSizer;
    wdgPanel*     wLeft;
    wdgScrollBox* wCenter;

    wdgPanel*     wRight;
    wdgLabel*     wNumTimer;
    wdgLabel*     wNumIdle;
    wdgLabel*     wNumBlock;
  public: // HACK
    wdgKnob*      wKnob1;
    wdgValue*     wVal1;

  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
      {
        axCanvas*     can;
        axWidget*     wdg;
        wdgScrollBar* scr;
        wdgScrollBox* scb;

        //-----

        mNumTimer = 0;
        //setSkin(mDefaultSkin,true);

        // --- bitmap

        int x,y;
        char* buffer;
        mBitmap = createBitmap(100,100);
        buffer = mBitmap->createBuffer();
        for (y=0; y<100; y++)
        {
          for (x=0; x<100; x++)
          {
            *buffer++ = (int)((float)x * 2.56); // b
            *buffer++ = (int)((float)y * 2.56); // g
            *buffer++ = 0;                      // r
            *buffer++ = 0;
          }
        }

        // --- bitmap -> surface

        mBitmap->prepare();
        mImgSurf = createSurface(100,100);
        can = mImgSurf->getCanvas();
        can->drawBitmap(mBitmap,0,0,0,0,100,100);

        // --- bitmap -> bitmap2

        mBitmap2 = createBitmap(100,100);
        mBitmap2->createBuffer( mBitmap->getBuffer() );
        //delete mBitmap;
                   // in: b g r a   // out:
        mBitmap2->swizzle(0,0,1,0,  // b
                          0,1,0,0,  // g
                          1,0,0,0,  // r
                          0,0,0,0); // a
        mBitmap2->prepare();

        // --- symbols

        mSymSurf = createSurface(256,256);//new axSurface(aContext,256,256);
        can = mSymSurf->getCanvas();
          can->setBrushColor(can->getColor(AX_GREY));
          can->fillRect(0,0,255,255);
        axColor col1 = can->getColor(AX_WHITE);
        axColor col2 = can->getColor(AX_GREY_LIGHT);
        mSymbols = new axSymbols(mSymSurf);
          mSymbols->create(symbol0,axRect( 0,0,8,8), col1,                      col2 );
          mSymbols->create(symbol1,axRect( 8,0,8,8), can->getColor(AX_YELLOW ), col2 );
          mSymbols->create(symbol2,axRect(16,0,8,8), can->getColor(AX_GREEN  ), col2 );
          mSymbols->create(symbol3,axRect(24,0,8,8), can->getColor(AX_CYAN   ), col2 );
          mSymbols->create(symbol4,axRect(32,0,8,8), col1,                      col2 );
          mSymbols->create(symbol5,axRect(40,0,8,8), can->getColor(AX_MAGENTA), col2 );
          mSymbols->create(symbol6,axRect(48,0,8,8), can->getColor(AX_BLACK  ), can->getColor(AX_GREY_DARK ) );
          mSymbols->create(symbol7,axRect(56,0,8,8), can->getColor(AX_GREEN  ), col2 );
          mSymbols->create(symbol8,axRect(64,0,8,8), can->getColor(AX_RED    ), can->getColor(AX_RED       ) );

        //-----------------------------------

        //appendWidget( wClient = new wdgPanel(this,NULL_RECT,wa_Client) );
        //  wClient->setBorders(10,10,5,5);
        #define wClient this

          // --- left ---

          wClient->appendWidget( wLeft = new wdgPanel(this,axRect(0,0,260,0),wa_Left) );
            wLeft->setBorders(10,10,5,5);
            wLeft->setFlag(wf_Clip);
            wLeft->appendWidget(          new wdgLabel(     this,axRect(0,0,  0, 16), wa_Top,          "") );
            wLeft->appendWidget(          new wdgLabel(     this,axRect(0,0,  0, 16), wa_Top,          "label ",ta_Left) );
            wLeft->appendWidget(          new wdgLabel(     this,axRect(0,0,  0, 16), wa_Top,          "label ",ta_Right) );
            wLeft->appendWidget(          new wdgPanel(     this,axRect(0,0,  0, 20), wa_Top) );
            wLeft->appendWidget( wdg =    new wdgPanel(     this,axRect(0,0,  0, 32), wa_Top) );
              wdg->appendWidget( new wdgLabel(this,NULL_RECT,wa_Client,"panel"));

            wdgGroupBox* grp;
            wLeft->appendWidget( grp =    new wdgGroupBox(this,axRect(0,0,0,100), wa_Top) );
            //grp->setup("test",false,false);
            //grp->getContainer()->setBorders(5,5,5,5);
            //grp->setBorders(5,5,5,5);
            grp->getContainer()->setFlag(wf_Clip);

              wdgPanel* panel;
              grp->appendWidget( scb = new wdgScrollBox(this,NULL_RECT,wa_Client) );
                scb->appendWidget( panel = new wdgPanel(this,NULL_RECT,wa_Client) );
                panel->setBorders(5,5,5,5);
                for (int i=0; i<32; i++) panel->appendWidget( new wdgKnob(this,axRect(0,0,64,16),wa_StackedHoriz, "",0) );

            wLeft->appendWidget(          new wdgSlider(    this,axRect(0,0,  0, 20), wa_Top,          "slider") );
            wLeft->appendWidget( wdg =    new wdgSlider(    this,axRect(0,0, 20,  0 ),wa_Left,         "slider") );
              wdg->setFlag(wf_Vertical);
            wLeft->appendWidget( scr =    new wdgScrollBar( this,axRect(0,0, 20,0 ),wa_Left ) );
              scr->setFlag(wf_Vertical);
              scr->setThumbSize(0.4);
            wLeft->appendWidget(          new wdgButton(    this,axRect(0,0,  0, 20), wa_Top,          false, "switch OFF","switch ON") );
            wLeft->appendWidget(          new wdgButton(    this,axRect(0,0,  0, 20), wa_Top,          false, "button","release...",ta_Center,bm_Spring) );
            wLeft->appendWidget( wVal1 =  new wdgValue(     this,axRect(0,0, 80, 20), wa_Top,          "value", 0) );
            wLeft->appendWidget( wKnob1 = new wdgKnob(      this,axRect(0,0, 30, 30), wa_Top,          "knob", 0) );
            wLeft->appendWidget( scr =    new wdgScrollBar( this,axRect(0,0,  0, 20), wa_Top) );
              scr->setThumbSize(0.2);
            wLeft->appendWidget( wdg =    new wdgImage(     this,axRect(0,0,100,100), wa_LeftTop,      mImgSurf) );
              wdg->appendWidget( new wdgLabel(this,axRect(0,0,100,100),wa_Client,"image [surface]"));
            wLeft->appendWidget( wdg =    new wdgBitmap(    this,axRect(0,0,100,100), wa_RightBottom,  mBitmap2) );
              wdg->appendWidget( new wdgLabel(this,axRect(0,0,100,100),wa_Client,"bitmap"));

          // --- sizer ---

          wClient->appendWidget( wSizer = new wdgSizer(this,axRect(0,0,5,0),wa_Left) );
            wSizer->setTarget(wLeft);

          // --- right ---

          wClient->appendWidget( wRight = new wdgPanel(this,axRect(0,0,180,0),wa_Right) );
            wRight->setBorders(10,10,5,5);
            wRight->appendWidget( wNumTimer = new wdgLabel( this,axRect(0,0,0,16), wa_Top,"doTimer : 0",    ta_Left) );
            wRight->appendWidget( wNumIdle  = new wdgLabel( this,axRect(0,0,0,16), wa_Top,"effEditIdle : 0",ta_Left) );
            wRight->appendWidget( wNumBlock = new wdgLabel( this,axRect(0,0,0,16), wa_Top,"doProcessBlock : 0",ta_Left) );

          // --- center ---

          wClient->appendWidget( wCenter = new wdgScrollBox(this,NULL_RECT,wa_Client) );
            wCenter->getContainer()->setBorders(10,10,5,5);
            wCenter->setFlag(wf_Clip);
            for (int i=0; i<256; i++)
            {
              wdgLabel* la;
              wCenter->appendWidget( la = new wdgLabel(this,axRect(0,0,30,20),wa_StackedHoriz) );
                //sprintf(label_buf[i],"%i",i+1);
                axItoa(label_buf[i],i+1);
                la->setText(label_buf[i],ta_Center);
            }

          // ---

        #undef wClient

// check this!
// with the following line, everything looks right
// the skin should be automatically 'inherited' in axWidget.appendWidget?
// but that doesn't seem to work correctly?

applySkin(mDefaultSkin,true);

        doRealign();
        startTimer(100);
      }

    //----------

    virtual ~myEditor(void)
      {
        stopTimer();
        //delete mBitmap;
        delete mBitmap2;
        delete mImgSurf;
        delete mSymSurf;
        delete mSymbols;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void doTimer(void)
      {
        mNumTimer++;
        //sprintf(timerText,"doTimer : %i",mNumTimer);
        axItoa(timerText,mNumTimer);
        wNumTimer->setText(timerText,ta_Left);
        onRedraw(wNumTimer);
      }

    //----------

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        axEditor::doPaint(aCanvas,aRect);
        //
        mSymbols->draw(aCanvas, 20,20,0);
        mSymbols->draw(aCanvas, 30,20,1);
        mSymbols->draw(aCanvas, 40,20,2);
        mSymbols->draw(aCanvas, 50,20,3);
        mSymbols->draw(aCanvas, 60,20,4);
        mSymbols->draw(aCanvas, 70,20,5);
        mSymbols->draw(aCanvas, 80,20,6);
        mSymbols->draw(aCanvas, 90,20,7);
        mSymbols->draw(aCanvas,100,20,8);
      }

    //--------------------------------------------------
    //|
    //--------------------------------------------------

    virtual void onChange(axWidget* aWidget)
      {
        //wtrace("onChange " << aWidget->getValue() );
        axEditor::onChange(aWidget);
      }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat
{
  private:
    myEditor* mEditor;
    int       mNumIdle;
    int       mNumBlock;
    char      idleText[32];
    char      blockText[32];
    axParameter*  pValue1;
    axParameter*  pValue2;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, AX_PLUG_DEFAULT)
      {
        mNumIdle = 0;
        mNumBlock = 0;
        describe("test_widgets","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(800,600);
        appendParameter( pValue1 = new axParameter(this,"value1","") );
        appendParameter( pValue2 = new axParameter(this,"value2","") );
        setupParameters();
      }

    //----------

    virtual ~myPlugin()
      {
      }

    //---------- plugin ----------

    virtual void  doStateChange(int aState)
      {
        //wtrace(":: doStateChange " << aState);
      }

    virtual void  doTransportChange(int aState)
      {
        //wtrace(":: doTransportChange " << aState);
      }

    virtual void  doSetProgram(int aProgram)
      {
        //wtrace(":: doSetProgram " << aProgram);
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        //char buf[32];
        //aParameter->doGetDisplay(buf);
        //wtrace(":: doSetParameter " << aParameter->getName().ptr() << " = " << aParameter->getValue() << " (" << buf << ")");
      }

    //virtual bool  doProcessEvents(void)
    //  {
    //    /*wtrace("doProcessEvents");*/
    //    return false;
    //  }

    virtual void  doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        //wtrace(":: doProcessMidi "<<ofs<<" : "<<(int)msg1<<","<<(int)msg2<<","<<(int)msg3);
      }

    virtual bool  doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        mNumBlock++;
        return false;
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        // noise
        *aOutputs[0] = axRandomSigned();
        *aOutputs[1] = axRandomSigned();
      }

    //virtual void  doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize)
    //  {
    //  }

    //---------- editor ----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        //wtrace(":: doOpenEditor");
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor->connect(mEditor->wKnob1,pValue1);
        mEditor->connect(mEditor->wVal1,pValue2);
        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        //wtrace(":: doCloseEditor");
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

    //----------

    virtual void doIdleEditor()
      {
        mNumIdle++;
        sprintf(idleText,"doIdleEditor : %i",mNumIdle);
        mEditor->wNumIdle->setText(idleText,ta_Left);
        mEditor->onRedraw(mEditor->wNumIdle);
        sprintf(blockText,"doProcessBlock : %i",mNumBlock);
        mEditor->wNumBlock->setText(blockText,ta_Left);
        mEditor->onRedraw(mEditor->wNumBlock);

      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
