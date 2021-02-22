#define AX_ALPHA
//#define AX_WIDGET_NOUPDATELIST

//#define AX_DEBUG_MEM
//#define AX_DEBUG_NEW
#define AX_DEBUG_CONSOLE  // define before axBase.h or from cmd line
#define AX_DEBUG_LOG      "tempodelay.log"

#include "base/axBase.h"
//#include "format/axFormat.h"
#include "gui/axEditor.h"
#include "skins/axSkinBasic.h"
#include "par/parFloat.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgKnob.h"
#include "skins/axSkinDef.h"
#include "gui/axBitmapLoader.h"
#include "../../ax/skins/img/skin1.h"
#include "../../ax/skins/img/knob1.h"

//----------------------------------------------------------------------

#define MAX_BEATS             4
#define MIN_BPM               30
#define MAX_SRATE             192000
#define MAX_SECONDS_PER_BEAT  60 / MIN_BPM
#define MAX_SAMPLES_PER_BEAT  MAX_SRATE * MAX_SECONDS_PER_BEAT
#define MAX_DELAY_LENGTH      MAX_BEATS * MAX_SAMPLES_PER_BEAT
//#define MAX_DELAY_LENGTH    1536000

//----------------------------------------------------------------------

char* str_params[] =
{
  (char*)"delay",
  (char*)"feedback",
  (char*)"dry",
  (char*)"wet"
};

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_tempodelay"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 4; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
    virtual bool          hasEditor(void)           { return true; }
    virtual axRect        getEditorRect(void)       { return axRect(0,0,120,163); }

};

//----------------------------------------------------------------------

//class myPlugin : public axFormat
class myInstance : public AX_INSTANCE
{
  private:
  // process
    SPL             m_Buffer[MAX_DELAY_LENGTH*2]; // totally 12mb (1.5m*sizeof(float)*2)
    int             m_Index;
    int             m_Size;
  //'internal' parameters
    float           m_Delay;
    float           m_Feedback;
    float           m_Dry;
    float           m_Wet;
  //vst parameters
    parFloat*       p_Delay;
    parFloat*       p_Feedback;
    parFloat*       p_Dry;
    parFloat*       p_Wet;
  //editor
    axEditor*       m_Editor;
    axSkinDef*      m_Skin;
    wdgPanel*       w_Panel;
    wdgKnob*        w_Delay;
    wdgKnob*        w_Feedback;
    wdgKnob*        w_Dry;
    wdgKnob*        w_Wet;
    bool            gui_initialized;
    axBitmapLoader* skinloader;
    axBitmapLoader* knobloader;

  public:

    //myPlugin(axContext* aContext, int aFlags)
    //: axFormat(aContext)
    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        if (!gui_initialized)
        {
          skinloader = new axBitmapLoader();
          skinloader->decodePng((unsigned char*)skin1,skin1_size);
          knobloader = new axBitmapLoader();
          knobloader->decodePng((unsigned char*)knob1,knob1_size);
          gui_initialized = true;
        }
        m_Index = 0;
        m_Size  = 0;
        //describe("fx_tempodelay","ccernn","axonlib example",2,AX_MAGIC+0x1001);
        //setupAudio(2,2,false);
        //setupEditor(100+20,(32*4)+15+20);
        appendParameter( p_Delay    = new parFloat(this,"delay",   "",0.75, 0.25, MAX_BEATS, 0.25 ) );
        appendParameter( p_Feedback = new parFloat(this,"feedback","",0.75 ) );
        appendParameter( p_Dry      = new parFloat(this,"dry",     "",0.75 ) );
        appendParameter( p_Wet      = new parFloat(this,"wet",     "",0.4  ) );
        setupParameters();
      }

    //----------

    ~myInstance()
      {
        if (gui_initialized)
        {
          delete skinloader;
          delete knobloader;
        }
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    //[internal]
    inline int calcSize(float delay, float tempo, float srate)
      {
        float sec_per_beat = 60 / tempo;
        float spl_per_beat = srate * sec_per_beat;
        return (int)(delay * spl_per_beat);
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual void doSetParameter(axParameter* aParameter)
      {
        int index = aParameter->getIndex();
        float val = aParameter->getValue();
        axString name = aParameter->getName();
        trace("doSetParameter(" << index << ") = " << val << "  [" <<  name.ptr() << "]" );
        switch(index)
        {
          // be careful with that index, eugene!
          case 0: m_Delay    = val; break;
          case 1: m_Feedback = val; break;
          case 2: m_Dry      = val; break;
          case 3: m_Wet      = val; break;
        }
      }

    //----------

    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        updateTimeInfo(); // get timing info from host
        m_Size = calcSize( m_Delay, getTempo(), getSampleRate() );
        while (m_Index>=m_Size) m_Index-=m_Size;
        return false; // we want doProcessSample
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        int i2 = m_Index*2;
        float in0 = *aInputs[0];
        float in1 = *aInputs[1];
        float dly0 = m_Buffer[i2  ];
        float dly1 = m_Buffer[i2+1];
        m_Buffer[i2  ] = in0 + dly0*m_Feedback;
        m_Buffer[i2+1] = in1 + dly1*m_Feedback;
        m_Index++;
        if (m_Index>=m_Size) m_Index = 0;
        *aOutputs[0] = in0*m_Dry + dly0*m_Wet;
        *aOutputs[1] = in1*m_Dry + dly1*m_Wet;
      }

    //--------------------------------------------------
    // editor
    //--------------------------------------------------

    virtual void* doOpenEditor(void* ptr)
      {
        //axEditor* editor = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);
        axEditor* editor = (axEditor*)mBase->getInterface()->createEditor(ptr,getEditorRect(),AX_WIN_DEFAULT);
        if (editor)
        {
          axCanvas* canvas = editor->getCanvas();
          m_Skin = new axSkinDef(canvas);
          m_Skin->loadSkinBitmap(editor,(char*)skinloader->getImage(),256,256,32);
          m_Skin->loadKnobBitmap(editor,(char*)knobloader->getImage(),32,(32*65),32);
          editor->applySkin(m_Skin);
          editor->appendWidget(  w_Panel = new wdgPanel(editor,NULL_RECT,wa_Client) );
          w_Panel->setBorders(10,10,5,5);
          w_Panel->appendWidget( w_Delay    = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"delay"    ) );
          w_Panel->appendWidget( w_Feedback = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"feedback" ) );
          w_Panel->appendWidget( w_Dry      = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"dry"      ) );
          w_Panel->appendWidget( w_Wet      = new wdgKnob(editor,axRect(100,32),wa_TopLeft,"wet"      ) );
          editor->connect(w_Delay,   p_Delay);
          editor->connect(w_Feedback,p_Feedback);
          editor->connect(w_Dry,     p_Dry);
          editor->connect(w_Wet,     p_Wet);
          editor->doRealign();
          //w_Delay->clearFlag(wf_Vertical);
          editor->show();
        }
        m_Editor = editor;
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        delete m_Skin;
        delete m_Editor;
        m_Editor = NULL;
      }

//    //TODO: this should be hidden (lower level..)
//    virtual void doIdleEditor(void)
//      {
//        //trace("doIdleEditor");
//        #ifndef AX_WIDGET_NOUPDATELIST
//          //trace("axFormat.doIdleEditor");
//          if (m_Editor && isEditorOpen() ) m_Editor->redrawUpdates();
//        #endif
//      }

};

//----------------------------------------------------------------------
//AX_ENTRYPOINT(myPlugin)
AX_MAIN(myDescriptor,myInstance)


