// i586-mingw32msvc-g++ -Wall -O3 -mwindows -lmsimg32 -Wno-unused -Wno-long-long -DAX_FORMAT_EXE -I../ -c test.cpp -o test.exe

//#define AX_NOGUI
//#define AX_MULTICHANNEL
//#define AX_WIDGET_NOUPDATELIST

#define AX_DEBUG_MEM
#define AX_DEBUG_NEW
//#define AX_DEBUG_PNG
//#define AX_DEBUG_CONSOLE
#define AX_DEBUG_LOG "test.log"

//----------

#include "base/axBase.h"
#include "core/axCpu.h"
#include "gui/axWindow.h"
#include "gui/axEditor.h"
#include "skins/axSkinBasic.h"
#include "wdg/wdgPanel.h"
#include "wdg/wdgButton.h"
#include "wdg/wdgKnob.h"
#include "par/parFloat.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) {}
    //virtual ~myDescriptor() {}
    virtual bool    hasEditor(void)     { return true; }
    virtual axRect  getEditorRect(void) { return axRect(0,0,320,240); }
    virtual int     getNumParams(void) { return 1; }
    virtual char*   getParamName(int aIndex)  { if (aIndex==0) return (char*)"gain.descr"; return (char*)""; }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance : public AX_INSTANCE
{
  private:
    axBase*       mBase;
    axRect        mEditorRect;
    axEditor*     edit;
    axSkinBasic*  skin;
    axRand        rnd;
    parFloat*     gain;
    wdgButton*    button;
    wdgKnob*      knob;
    float         m_gain;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        trace("myInstance: " << this);
        //trc("myInstance");
        mBase = aBase;
        edit  = NULL;
        skin  = NULL;
        //test();
        m_gain = 1;
        gain = new parFloat(this,"gain","",0.5);
        appendParameter( gain );
        setupParameters();
        //printf("myInstance ok\n");
      }

    //----------

    //virtual ~myInstance()
    //  {
    //  }

    //--------------------------------------------------

//    virtual void onChange(axParameter* aParameter)
//      {
//        trace("onChange par");
//      }


    //--------------------------------------------------

    void test(void)
      {
        // pragma message (compile time)
        pmsg("some note here")

        trace(AX_AXONLIB_TEXT);
        char temp[256];
        //trace("basepath:        '" << axGetBasePath(temp) << "'");
        trace("platform:        '" << mBase->getPlatform()->getPlatformName() << "'");
        trace("format:          '" << mBase->getFormat()->getFormatName() << "'");
        trace("w32 class name   '" << mBase->getInterface()->getName() << "'");
        axCpu cpu;
        //cpu.axCpuId();
        unsigned long start_time, end_time, diff;
        start_time = cpu.rdtsc();
        trace("---------------"); // some code to measure
        end_time   = cpu.rdtsc();
        diff       = end_time - start_time;
        trace("axCpuCaps:        " << cpu.axCpuCaps() );
        trace("cpu caps          " << cpu.axCpuCaps() << " '" << cpu.axCpuCapsString() << "'" );
        trace("rdtsc:            " << diff << " (may not be correct for multi-core)");
      }

    //--------------------------------------------------

    //virtual void doStateChange(int aState)
    //  {
    //    trace("doStateChange: " << aState);
    //  }

    virtual void doSetParameter(axParameter* aParameter)
      {
        int   index = aParameter->getIndex();
        float value = aParameter->getValue();
        //trace("doSetParameter(" << index << ") = " << value);
        if (index==0) m_gain = value;
      }

    //virtual bool doProcessBlock(float** aInputs, float** aOutputs, int Length)
    //  {
    //    trace("block");
    //    return false;
    //  }

    virtual void doProcessSample(float** aInputs, float** aOutputs)
      {
        *aOutputs[0] = rnd.randSigned() * m_gain;
        *aOutputs[1] = rnd.randSigned() * m_gain;
        //trace(".");
      }

    //--------------------------------------------------

    virtual void* doOpenEditor(void* ptr)
      {
        edit = (axEditor*)mBase->getInterface()->createEditor(ptr,getEditorRect(),AX_WIN_DEFAULT);
        if (edit)
        {
          edit->show();
          //win->show();
          axCanvas* canvas = edit->getCanvas();
          skin = new axSkinBasic(canvas);
          edit->applySkin(skin);
          edit->setInstance(this);
          wdgPanel* panel;
          edit->appendWidget( panel = new wdgPanel(edit,NULL_RECT,wa_Client));
            panel->appendWidget( button = new wdgButton(edit,axRect(10,10,100,30),wa_None));
            panel->appendWidget( knob   = new wdgKnob(  edit,axRect(10,50,100,30),wa_None));
          edit->doRealign();
          edit->connect(knob,gain);
        }
        return edit;
      }

    //----------

    virtual void  doCloseEditor(void)
      {
        if (skin) { delete skin; skin=NULL; }
        if (edit) { delete edit; edit=NULL; }
      }

    //

    //TODO: this should be hidden (lower level..)
    virtual void doIdleEditor(void)
      {
        //trace("doIdleEditor");
        #ifndef AX_WIDGET_NOUPDATELIST
          //trace("axFormat.doIdleEditor");
          if (edit) edit->redrawUpdates();
        #endif
      }

};

//----------------------------------------------------------------------

#ifdef AX_MAIN
  #undef AX_MAIN
#endif
//AX_ENTRYPOINT(axDebug,AX_INTERFACE,AX_FORMAT,myDescriptor,myInstance)
#define AX_MAIN(d,i) AX_ENTRYPOINT(AX_PLATFORM, AX_INTERFACE,AX_FORMAT,d,i)
AX_MAIN(myDescriptor,myInstance)

