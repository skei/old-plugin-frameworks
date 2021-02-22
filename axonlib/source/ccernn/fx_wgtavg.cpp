#define AX_NOGUI

#include "base/axBase.h"
#include "par/parFloat.h"
#include "par/parInteger.h"
#include "dsp/dspRC.h"

//----------------------------------------------------------------------

char* str_params[] = { (char*)"mode", (char*)"weight" };

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_wgtavg"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 2; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

//----------------------------------------------------------------------

char* str_mode[] = { (char*)"lowpass", (char*)"highpass" };

//----------

class myInstance : public AX_INSTANCE
{
  private:
    int     m_Mode;
    dspRC   m_RC0,m_RC1;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        //describe("fx_wgtavg","ccernn","axonlib example",0,AX_MAGIC+0x1005);
        //setupAudio(2,2);
        appendParameter( new parInteger( this,"mode",  "", 0,   0,1,   str_mode) );
        appendParameter( new parFloatPow(this,"weight","", 0.5, 0,1,0, 3) );
        setupParameters();
      }

    virtual ~myInstance()
      {
        //delete rc1;
        //delete rc2;
      }

    virtual void doStateChange(int aState)
      {
        float s;
        switch (aState)
        {
          case is_Resume:
            s = getSampleRate();
            //trace("samplerate = " << s);
            m_RC0.setSRate(s);
            m_RC1.setSRate(s);
            break;
        }
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch( aParameter->getIndex() )
        {
          case 0:
            m_Mode = (int)val;
            break;
          case 1:
            m_RC0.setWeight(val);
            m_RC1.setWeight(val);
            break;
        }
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        SPL spl0 = *aInputs[0];
        SPL spl1 = *aInputs[1];
        switch (m_Mode)
        {
          case 0: // lowpass
            spl0 = m_RC0.process(spl0);
            spl1 = m_RC1.process(spl1);
            break;
          case 1: // highpass
            spl0 -= m_RC0.process(spl0);
            spl1 -= m_RC1.process(spl1);
            break;
        }
        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_MAIN(myDescriptor,myInstance)
