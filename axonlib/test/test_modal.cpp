#if 0




#define AX_DEBUG
#define AX_DEBUG_AUTO_STD

#include "platform/axContext.h"
#include "axFormat.h"
#include "axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgButton.h"

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class myEditor : public axEditor
{
  private:
    wdgPanel*   panel;
    wdgPanel*   popup;
    wdgButton*  button;

  public:

    myEditor(axFormat* aPlugin, axContext* aContext, axRect aRect, int aWinFlags)
    : axEditor(aPlugin,aContext,aRect,aWinFlags)
    {
      appendWidget( panel = new wdgPanel(this,NULL_RECT,wa_Client) );
        panel->appendWidget( button = new wdgButton(this,axRect(10,10,100,20),wa_None,false,"off","on",ta_Center,bm_Spring) );
      doRealign();

      popup = new wdgPanel( this,axRect(0,0,200,100),wa_None );
      popup->clearFlag(wf_Visible);
      popup->clearFlag(wf_Active);
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

    //----------

    // if we right-click, and we're in a modal state, end modal
    virtual void doMouseDown(int aXpos, int aYpos, int aButton)
      {
        if (mModalWidget && (aButton==bu_Right)) unModal();
        else axEditor::doMouseDown(aXpos,aYpos,aButton);
      }

    virtual void doKeyDown(int aKeyCode, int aState)
      {
        if (mModalWidget) unModal();
        else axEditor::doKeyDown(aKeyCode,aState);
      }

    // button clicked -> create popup & go modal
    virtual void onChange(axWidget* aWidget)
      {
        axWidget* wdg;
        axEditor::onChange(aWidget);

        if (aWidget==button)
        {
          wdg = new wdgPanel(this,axRect(20,20,100,200),wa_None);
            wdg->setBorders(5,5,0,0);
            wdg->appendWidget( new wdgButton(this,axRect(0,20),wa_Top) );
            wdg->appendWidget( new wdgButton(this,axRect(0,20),wa_Top) );
            wdg->appendWidget( new wdgButton(this,axRect(0,20),wa_Top) );
            wdg->appendWidget( new wdgButton(this,axRect(0,20),wa_Top) );
            wdg->appendWidget( new wdgButton(this,axRect(0,20),wa_Top) );
            wdg->doRealign();
            //onCursor(DEF_CURSOR);
            setCursor(DEF_CURSOR);
          goModal( wdg );
        }
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
        describe("test_modal","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
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






#endif
