
#define AX_DEBUG
//#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axFormat.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgOctave.h"


//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel*     main_panel;
    wdgOctave*  octave1;

  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      axCanvas* aCanvas = getCanvas();
      appendWidget( main_panel = new wdgPanel(this,NULL_RECT,wa_Client) );
        main_panel->appendWidget( octave1 = new wdgOctave(this,axRect(0,0,200,60),wa_Client) );
          octave1->setDist(2);
          //octave1->activate(0);
          //octave1->activate(3);
          //octave1->activate(7);
          octave1->setColors( aCanvas->getColor(255,255,255),
                              aCanvas->getColor(  0,  0,  0),
                              aCanvas->getColor(192,192,192),
                              aCanvas->getColor( 96, 96, 96)
          );
      setSkin(mDefaultSkin,true);
      doRealign();
      octave1->recalc();
      startTimer(1000);
    }

    //----------

    virtual ~myEditor()
      {
        stopTimer();
      }

    //----------

    virtual void doTimer()
      {
        int num = axRandomInt(11); // 0..11
        octave1->activate(num);
        num = axRandomInt(11);
        octave1->deactivate(num);
        //wtrace("doTimer...");
        invalidate(mRect.x,mRect.y,mRect.w,mRect.h); // redraw everything
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

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, AX_PLUG_DEFAULT)
      {
        axRandomize(418);
        describe("test_keyboard","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(640,480);
        //setupParameters();
      }

    virtual ~myPlugin()
      {
      }

    //----------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        mEditor = new myEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        mEditor->show();
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        mEditor->hide();
        delete mEditor;
        mEditor = NULL;
      }

    //----------

    //virtual void doIdleEditor()
    //  {
    //    trace("...doIdleEditor");
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
