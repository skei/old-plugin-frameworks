#define AX_FORMAT_EXE

#define AX_DEBUG
#include "core/axDebug.h"

#include "platform/axContext.h"
#include "axFormat.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel* main_panel;

  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      //axCanvas* canvas = getCanvas();
      appendWidget( main_panel = new wdgPanel(this,NULL_RECT,wa_Client) );
      setSkin(mDefaultSkin/*,true*/);
      doRealign();
      //startTimer(250);
    }

    //----------

    virtual ~myEditor()
      {
        //stopTimer();
      }

    //----------

    //virtual void doTimer()
    //  {
    //    wtrace("doTimer...");
    //    //invalidate(mRect.x,mRect.y,mRect.w,mRect.h); // redraw everything
    //  }

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
        describe("test_","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
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
    //    wtrace("...doIdleEditor");
    //  }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)

