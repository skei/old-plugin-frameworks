//#define AX_DEBUG_AUTO_STD
//#define AX_DEBUG_MEM
//#define AX_DEBUG_PNG
//#define AX_DEBUG_NEW
//#define AX_DEBUG_LOG "test.log"

#define AX_NOGUI
//#define AX_WIDGET_NOUPDATELIST
#define AX_DEBUG_CONSOLE
#define AX_DEBUG_LOG "test.log"

//----------

#include "base/axBase.h"
//#include "core/axCpu.h"
//#include "gui/axWindow.h"
//#include "gui/axEditor.h"
//#include "skins/axSkinBasic.h"
//#include "wdg/wdgPanel.h"
//#include "wdg/wdgButton.h"
//#include "wdg/wdgKnob.h"
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
    //virtual bool    hasEditor(void)     { return true; }
    //virtual axRect  getEditorRect(void) { return axRect(0,0,320,240); }
    virtual char*   getName(void)             { return (char*)"gain"; }
    virtual char*   getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*   getProduct(void)          { return (char*)"gain :: axonlib example plugin"; }
    virtual int     getNumParams(void)        { return 1; }
    virtual char*   getParamName(int aIndex)  { if (aIndex==0) return (char*)"gain !!!"; return (char*)""; }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance : public AX_INSTANCE
{
  private:
    axBase*   mBase;
    parFloat* pGain;
    float     mGain;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        mBase = aBase;
        mGain = 0.5; // !!
        appendParameter( pGain = new parFloat(this,"gain","",0.5) );
        setupParameters();
      }

    //----------

    //virtual ~myInstance()
    //  {
    //  }

    //--------------------------------------------------

    //virtual void doStateChange(int aState)
    //  {
    //    trace("doStateChange: " << aState);
    //  }

    virtual void doSetParameter(axParameter* aParameter)
      {
        int   index = aParameter->getIndex();
        float value = aParameter->getValue();
        trace("doSetParameter(" << index << ") = " << value);
        if (index==0) mGain = value;
      }

    //virtual bool doProcessBlock(float** aInputs, float** aOutputs, int Length)
    //  {
    //    trace("block");
    //    return false;
    //  }

    virtual void doProcessSample(float** aInputs, float** aOutputs)
      {
        *aOutputs[0] = *aInputs[0] * mGain;
        *aOutputs[1] = *aInputs[1] * mGain;
        //trace(".");
      }

};

//----------------------------------------------------------------------

// in axFormat/Base?
//#define AX_MAIN(d,i) AX_ENTRYPOINT(AX_PLATFORM, AX_INTERFACE,AX_FORMAT,d,i)

AX_MAIN(myDescriptor,myInstance)

