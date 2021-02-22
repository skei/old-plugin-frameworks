#define AX_NOGUI

//#include "format/axFormat.h"
#include "base/axBase.h"
#include "par/parFloat.h"
#include "par/parInteger.h"
#include "dsp/dspSVF.h"

//----------------------------------------------------------------------

#define AX_SCRIPT_STDLIB
#include "script/axScript.h"
//#include "script/axScriptStdlib.h"

char* source = (char*)
  ": proc1 "
//  "  0 "
  "$ label1 "
  "  DUP . "
  "  1+ "
  "  DUP 10 < IF label1 ENDIF "
  "; "
  "$ main "
  "  5 "
  "  proc1 "
  "  2 "
  "  proc1 "
  ;

// char* source = (char*)": testproc 0 $ label1 1+ DUP 100 < IF label1 ENDIF ; $ main testproc ";
//
// uses 16% cpu on my laptop when called for every sample...
// 48.000 * 100 = 4.800.000 'rounds' through the mainloop = 7-8 tokens
// estimated tokens per sec = 30m ? :-)

//----------------------------------------------------------------------

char* str_params[] = { (char*)"mode", (char*)"freq", (char*)"bw" };
char* str_filter[] = { (char*)"off", (char*)"lowpass", (char*)"highpass", (char*)"bandpass", (char*)"notch" };

//----------------------------------------------------------------------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"test_script"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 3; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

//----------------------------------------------------------------------

//class myPlugin : public axFormat
class myInstance : public AX_INSTANCE
{
  private:
    dspSVF svf1,svf2;
    axScript* script;
    int main_pos;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        appendParameter( new parInteger(  this,"_mode_", "", 0, 0,4, str_filter ) );
        appendParameter( new parFloatPow( this,"_freq_", "", 1, 0,1,0, 2 ) );
        appendParameter( new parFloatPow( this,"_bw_",   "", 1, 0,1,0, 2 ) );
        setupParameters();
        svf1.setup(0,1,1);
        svf2.setup(0,1,1);

        script = new axScript();
        script->compile(source);
        script->dumpCode();
        main_pos = script->labelPos( script->findLabel((char*)"main") );
        trace("main_pos = " << main_pos);

        script->execute(main_pos);

      }

    virtual ~myInstance()
      {
        delete script;
      }


    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch( aParameter->getIndex() )
        {
          case 0: // mode
            svf1.mMode = val;
            svf2.mMode = val;
            break;
          case 1: // freq
            svf1.mFreq = val;
            svf2.mFreq = val;
            break;
          case 2: // bw
            svf1.mBW = val;
            svf2.mBW = val;
            break;
        }
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        script->execute(main_pos);
        *aOutputs[0] = svf1.process(aInputs,aOutputs);
        *aOutputs[1] = svf2.process(aInputs,aOutputs);
      }

};

//----------------------------------------------------------------------
AX_MAIN(myDescriptor,myInstance)
