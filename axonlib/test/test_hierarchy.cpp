#define AX_DEBUG
#define AX_DEBUG_AUTO_STD

#include "platform/axContext.h"
#include "axFormat.h"
#include "axEditor.h"
#include "gui/axWidget.h"

#include "wdg/wdgPanel.h"
#include "wdg/wdgScrollBox.h"
#include "wdg/wdgButtons.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel*     panel;
    wdgScrollBox* scroll;
    wdgButtons*   buttons;

  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      appendWidget( panel = new wdgPanel(this,NULL_RECT,wa_Client));
        panel->appendWidget( scroll = new wdgScrollBox(this,axRect(100,200,200,200),wa_None) );
          scroll->appendWidget( buttons = new wdgButtons(this,axRect(0,0,150,400),wa_None)  );
            for (int i=0; i<20; i++)
              buttons->appendButton( new wdgButton(buttons,axRect(0,20),wa_Top,false,"off","on"  ) );
      doRealign();
    }

    //----------

    //virtual ~myEditor()
    //  {
    //    //stopTimer();
    //  }

    //----------

    //virtual void doTimer()
    //  {
    //    //invalidate(mRect.x,mRect.y,mRect.w,mRect.h);
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

    //----------

    //virtual ~myPlugin() {}

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
