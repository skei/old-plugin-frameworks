//#define AX_NO_MALLOC
//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG "test_skin.log"

//----------------------------------------------------------------------

#define AX_ALPHA

#include "format/axFormat.h"
#include "gui/axEditor.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"

#include "skins/axSkinBasic.h"
#include "skins/axSkinDef.h"
#include "gui/axBitmapLoader.h"
#include "../ax/skins/img/skin1.h"
#include "../ax/skins/img/knob1.h"
#include "../ax/skins/img/knob32.h"

class myPlugin : public axFormat
{
  private:
    axEditor*       m_Editor;
    wdgPanel*       w_Panel;

    bool            skin_initialized;
    axBitmapLoader* skinloader1;
    axBitmapLoader* knobloader1;
    axBitmapLoader* knobloader2;
    axSkinDef*      m_Skin1;
    axSkinDef*      m_Skin2;

  public:

    myPlugin(axContext* aContext, int aFlags)
    : axFormat(aContext)
      {
        describe("test_skin","ccernn","axonlib example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(320,240);
        skin_initialized = false;
      }

    virtual ~myPlugin()
      {
        if (skin_initialized)
        {
          delete skinloader1;
          delete knobloader1;
          delete knobloader2;
        }
      }

    //----------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        if (!skin_initialized)
        {
          //
          // so we're sure png decoding is only done once
          // this should be in axFormat.doStateChange, resume or open..
          // to avoid needing to decode the bitmaps when a host is
          // enumerating, and also to avoid having to (re-)decode the
          // png's every time you open the editor
          //
          skinloader1 = new axBitmapLoader();
          skinloader1->decodePng((unsigned char*)skin1,skin1_size);
          knobloader1 = new axBitmapLoader();
          knobloader1->decodePng((unsigned char*)knob1,knob1_size);
          knobloader2 = new axBitmapLoader();
          knobloader2->decodePng((unsigned char*)knob32,knob32_size);
          skin_initialized = true;

        }

        //---

        axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axCanvas* canvas = editor->getCanvas();
        editor->appendWidget( w_Panel = new wdgPanel(editor,NULL_RECT,wa_Client) );   // background
        w_Panel->setBorders(10,10,5,5);

        //---

        // skin 1
        m_Skin1 = new axSkinDef(canvas);
        m_Skin1->loadSkinBitmap(editor,(char*)skinloader1->getImage(),256,256,32);
        m_Skin1->loadKnobBitmap(editor,(char*)knobloader1->getImage(),32,(32*65),32);
        //editor->applySkin(m_Skin1);
        // skin 2
        //m_Skin2 = new axSkinDef(canvas);
        //m_Skin2->copyFrom(m_Skin1);
        m_Skin2 = new axSkinDef(m_Skin1);
        m_Skin2->loadKnobBitmap(editor,(char*)knobloader2->getImage(),32,(32*65),32);

        axWidget* wdg;

        w_Panel->appendWidget( wdg = new wdgKnob( editor,axRect(100,32),wa_StackedVert,"knob1",0.75) );
        wdg->applySkin(editor->defaultSkin());

        w_Panel->appendWidget( wdg = new wdgKnob( editor,axRect(100,32),wa_StackedVert,"knob2",0.75) );
        wdg->applySkin(m_Skin1);

        w_Panel->appendWidget( wdg = new wdgKnob( editor,axRect(100,32),wa_StackedVert,"knob3",0.75) );
        wdg->applySkin(m_Skin2);

        //---

        editor->doRealign();
        editor->show();
        m_Editor = editor;
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        axEditor* editor = m_Editor;
        m_Editor->hide();
        m_Editor = NULL;
        delete editor;
        delete m_Skin1;
        delete m_Skin2;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)
