/*
  ******************************************************************************
  buquad filter band (rbj)
  (c) 2010, lubomir i. ivanov (neolit123 at gmail)
  using axonlib 0.0.x:
  <http://axonlib.googlecode.com/>
  ******************************************************************************
*/

    // it doesn't seem loike we have the sample-rate available in the plugin constructor
    // the host might be enumerating plugins, of turned off, or something...
    // so the above setupParameters won't work correctly for parameters that depend on samplerate
    // in doStateChange(fs_Resume), the s.rate has been set, it seems
    // so we force a recalc there

#define AX_NOGUI

#include "base/axBase.h"
#include "par/parInteger.h"
#include "par/parFloat.h"
#include "dsp/dspRBJ.h"   // add filter

// string arrays
char* str_processing[] =
{
  (char*)"stereo",
  (char*)"mono"
};
char* ftype_ar[] =
{
  (char*)"off",
  (char*)"LP",
  (char*)"HP",
  (char*)"BP",
  (char*)"BR",
  (char*)"PK",
  (char*)"LS",
  (char*)"HS"
};
char* str_params[] =
{
  (char*)"mode",
  (char*)"filter",
  (char*)"freq",
  (char*)"bw",
  (char*)"gain"
};

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"biquad_rbj"; }
    virtual char*         getAuthor(void)           { return (char*)"liteon"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 5; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

// class
class myInstance : public AX_INSTANCE
{

private:
  unsigned int mono, ftype;
  //float srate;
  float q, freq, gain;
  // init 2 fiters (left / right)
  dspRBJ f0,f1;

public:
  // constructor
  myInstance(axBase* aBase) : AX_INSTANCE(aBase)
  {
    // set init values
    mono = 0;
    freq = q = gain = 0.f;
    // add params
    appendParameter( new parInteger(this,"mode",  "",     0,     0,   1,     str_processing) );
    appendParameter( new parInteger(this,"filter","",     1,     0,   7,     ftype_ar) );
    appendParameter( new parFloat(  this,"freq",  "scale",0.5f,  0,   1.0135,0.0001) );
    appendParameter( new parFloat(  this,"BW",    "oct",  2.543f,0.01,4,     0.001) );
    appendParameter( new parFloat(  this,"gain",  "dB",   0.f,   -24, 24,    0.001) );
    // first process params
    setupParameters();
  }

  //----------

  inline void setup_filters(int mo, int ft, float fr, float  q, float ga/*, bool intrp*/)
  {
    float sr = getSampleRate();
             f0.setup(ft,sr,fr,q,ga,true);
    if (!mo) f1.setup(ft,sr,fr,q,ga,true);
  }

  //----------

//  // process state
  virtual void doStateChange(int aState)
  {
    switch (aState)
    {
      //case is_Open:
      //  trace("open. srate=" << getSampleRate() );
      //  break;
      //case is_Close:
      //  trace("close.");
      //  break;
      //case is_Suspend:
      //  trace("suspend: srate=" << getSampleRate() );
      //  break;
      case is_Resume:
        //trace("resume.");
        setup_filters(mono,ftype,freq,q,gain/*, bool intrp*/);
        break;
    }
  }

  //----------

  // process parameters
  virtual void doSetParameter(axParameter* aParameter)
  {
    // param id, value
    int  id = aParameter->getIndex();
    float f = aParameter->getValue();

    switch (id)
    {
      // mono / stereo
    case 0:
      mono = (int)f;
      break;
      // filter type
    case 1:
      ftype = (int)f;
      break;
      // frequency
    case 2:
      freq = axExpf(f*6.90775f + 2.99573f); // [0 - 1.0135] to [20hz - ~22000hz]
      //trace("freq = " << freq);
      break;
      // q factor
    case 3:
      q = axOctaves2Q(f);
      break;
      // gain
    case 4:
      gain = f;
      break;
    }
    //setup filters f0, f1
    setup_filters(mono,ftype,freq,q,gain/*,true*/);
    //float srate = getSampleRate();
    //f0.setup(ftype, srate, freq, q, gain, true);
    //if (!mono) f1.setup(ftype, srate, freq, q, gain, true);
  }

  //----------

  // interpolate filter coefficients
  virtual bool doProcessBlock(float** inputs, float** outputs, int sampleFrames)
  {
    f0.interpolate(sampleFrames);
    f1.interpolate(sampleFrames);
    return false;
  }

  //----------

  // per sample
  virtual void doProcessSample(float** ins, float** outs)
  {
    float spl0 = *ins[0];
    float spl1 = *ins[1];
    // mono
    if (mono)
    {
      spl0 = spl1 = f0.process(spl0);
    }
    // stereo
    else
    {
      spl0 = f0.process(spl0);
      spl1 = f1.process(spl1);
    }
    *outs[0] = spl0;
    *outs[1] = spl1;
  }
};

// -----------------
AX_MAIN(myDescriptor,myInstance)
