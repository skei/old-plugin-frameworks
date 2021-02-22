#define AX_NOGUI

#include "base/axBase.h"
#include "par/parFloat.h"
#include "par/parInteger.h"

#include "audio/axVoices.h"
#include "dsp/dsp_Osc.h"
#include "dsp/dspRC.h"

//----------------------------------------------------------------------

//char* str_filter[] =
//{
//  (char*)"off",
//  (char*)"lowpass",
//  (char*)"highpass",
//  (char*)"bandpass",
//  (char*)"notch"
//};

#define MAX_VOICES 16

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice : public axVoice
{
  private:
    float   gain, attack,release, tuneoct, tunesemi, tunecent, vel;
    dspRC   att1,rel1;
    dspOsc0 osc1,osc2;
  public:

    myVoice() : axVoice()
      {
        osc1.setup(osc_Ramp,0,0);
        osc2.setup(osc_Squ,0,0);
        att1.setup(0,1,0);
        rel1.setup(0,0,0);
      }

    virtual ~myVoice() {}

    virtual void  setSampleRate(float aRate)
      {
        osc1.setSampleRate(aRate);
        osc2.setSampleRate(aRate);
        axVoice::setSampleRate(aRate);
      }

    virtual void noteOn(int aNote, int aVel)
      {
        vel = (float)aVel * inv127;
        att1.setValue(0);         // start from 0
        att1.setWeight(attack);   // fade up
        rel1.setValue(1);         // initially, set to full on
        rel1.setWeight(0);        // fade speed = 0 (until note off)
        float detune = (tuneoct*12) + tunesemi + tunecent;
        float fr1 = 440 * axPowf(2.0,(aNote-69.0) / 12);
        float fr2 = 440 * axPowf(2.0,(aNote-69.0+detune) / 12);
        osc1.setFreq(fr1);
        osc2.setFreq(fr2);
      }

    virtual void noteOff(int aNote, int aVel)
      {
        rel1.setValue( att1.getValue() ); // start from current amplitude
        rel1.setWeight(release);          // and let it fade down
      }

    //virtual void  ctrlChange(int aCtrl, int aVal) {}

    virtual void control(int aIndex, float aValue)
      {
        switch(aIndex)
        {
          case 0: gain     = aValue; break;
          case 1: attack   = aValue; break;
          case 2: release  = aValue; break;
          case 3: tuneoct  = aValue; break;
          case 4: tunesemi = aValue; break;
          case 5: tunecent = aValue; break;
        }
      }

    virtual void process(float* outs)
      {
        float out1 = osc1.process();
        float out2 = osc2.process();
        float a = att1.process();
        float r = rel1.process();
        if (r<EPSILON) mState=vs_Off;
        outs[0] = out1*vel*a*r * gain;
        outs[1] = out2*vel*a*r * gain;
      }

};

//----------------------------------------------------------------------
//
// synth
//
//----------------------------------------------------------------------

char* str_params[] = { (char*)"gain", (char*)"att", (char*)"rel", (char*)"oct", (char*)"semi", (char*)"cent" };

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"syn_poly"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 6; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
    virtual bool          isSynth(void)             { return true; }

};

//----------------------------------------------------------------------

class myInstance : public AX_INSTANCE
{
  private:
    axVoiceManager* VM;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {

        axRand(19);
        VM = new axVoiceManager();
        for (int i=0; i<MAX_VOICES; i++) VM->appendVoice( new myVoice() );
        //describe("syn_poly","ccernn","axonlib example",0,AX_MAGIC+0x1007);
        //setupAudio(2,2,true);
        appendParameter( new parFloat(  this,"gain","db",1,  0, 1  ));
        appendParameter( new parFloat(  this,"att", "",  3,  1, 50 ));
        appendParameter( new parFloat(  this,"rel", "",  20, 1, 50 ));
        appendParameter( new parInteger(this,"oct", "",  0, -4, 4  ));
        appendParameter( new parInteger(this,"semi","",  0, -12,12 ));
        appendParameter( new parInteger(this,"cent","",  0, -50,50 ));
        setupParameters();
      }

    virtual ~myInstance()
      {
        delete VM;
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch( aParameter->getIndex() )
        {
          case 0: VM->control(0, val*val*val);      break;
          case 1: VM->control(1, 1/(val*val*val) ); break;
          case 2: VM->control(2, 1/(val*val*val) ); break;
          case 3: VM->control(3, val);              break;
          case 4: VM->control(4, val);              break;
          case 5: VM->control(5, val/100);          break;
        }
      }

    virtual void doProcessMidi(int ofs, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
        VM->midi(ofs,msg1,msg2,msg3);
      }


    virtual bool doProcessBlock(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        VM->setSampleRate( getSampleRate() );
        VM->preProcess();
        return false;
      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        SPL outs[2];
        VM->process( outs );
        *aOutputs[0] = outs[0];
        *aOutputs[1] = outs[1];
      }

    virtual void doPostProcess(SPL** aInputs, SPL** aOutputs, int aSize)
      {
        VM->postProcess();
      }

};

AX_MAIN(myDescriptor,myInstance)
