//#define AX_DEBUG
//#include "core/axDebug.h"

#include "axFormat.h"
#include "axEditor.h"
#include "gui/axSkinDefault.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"
#include "wdg/wdgButtons.h"

//----------------------------------------------------------------------

class myPlugin : public axFormat
{

    //----------------------------------------
    // plugin
    //----------------------------------------

  private:
    axParameter*  p_Gain;
    float         m_Gain;

  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, pf_None/*HasEditor*/)
      {
        m_Gain = 0;
        describe("test_buttons","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(320,240);
        appendParameter( p_Gain = new axParameter(this,"gain","") );
        setupParameters();
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        if (aParameter==p_Gain) m_Gain = aParameter->getValue();
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * m_Gain;
        *aOutputs[1] = *aInputs[1] * m_Gain;
      }

    //----------------------------------------
    // gui
    //----------------------------------------

  private:
    axEditor* mEditor;
    wdgPanel* wPanel;
    wdgKnob*  w_Gain;
    wdgButtons* btns;

  public:

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        editor->appendWidget( wPanel = new wdgPanel(  editor,NULL_RECT,wa_Client) );
          wPanel->setBorders(10,10,5,5);
          wPanel->appendWidget( w_Gain = new wdgKnob( editor,axRect(10,10,128, 32),wa_TopLeft,"gain",0.75) );
          wPanel->appendWidget( btns = new wdgButtons(editor,axRect(10,50,200,100),wa_Client ) );
            btns->setBorders(0,0,2,2);
            for (int i=0; i<32; i++) btns->appendButton( new wdgButton( btns,axRect(0,0,32,32),wa_StackedHoriz ) );
            btns->setMode(bm_Single);
        editor->connect(w_Gain,p_Gain);
        editor->doRealign();
        editor->show();
        mEditor = editor;
        return mEditor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = mEditor;
        mEditor->hide();
        mEditor = NULL;
        delete editor;
      }

    //----------

    //virtual void doIdleEditor() {}

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
