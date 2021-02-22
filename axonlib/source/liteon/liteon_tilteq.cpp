#define AX_NOGUI

#include "base/axBase.h"
#include "par/parInteger.h"
#include "par/parFloat.h"

// is there some 'zipper noise' in there?
// or some variables needs to be re-initialized when tweaking sliders?

char* str_processing[] = { (char*)"stereo",(char*)"mono" };
char* str_params[] = { (char*)"process", (char*)"center", (char*)"tilt", (char*)"output" };

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"biquad_rbj"; }
    virtual char*         getAuthor(void)           { return (char*)"liteon"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 4; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};


class myInstance : public AX_INSTANCE
{
  private:
    float amp, pi, sr3;
    float outgain, lgain, hgain;
    int   mono;
    float input,  output,  lp_out;
    float input_r,output_r,lp_out_r;
    float a0, b1;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        //float srate = 44100;
        //sr3 = 3*srate;
        amp = 6/axLogf(2);//log(2);
        a0=b1 = 0;
        lp_out = lp_out_r = 0;
        appendParameter( new parInteger(this, "processing",       "",       0,   0,  1,   str_processing) );
        appendParameter( new parFloat(  this, "center frequency", "scale",  50,  0,  100, 0.05   ) );
        appendParameter( new parFloat(  this, "tilt (low/high)",  "db",     0,  -6,  6,   0.05   ) );
        appendParameter( new parFloat(  this, "output gain",      "db",     0,  -25, 25,  0.05   ) );
        // prepare for vst use, but don't transfer variables yet
        // wait until resume, so we have the samplew rate...
        //setupParameters();
        prepareParameters();
        //transferParameters();
      }

    virtual void doStateChange(int aState)
      {
        switch(aState)
        {
          case is_Resume:
            sr3 = 3*getSampleRate();
            transferParameters();
            break;
        }
      }

    virtual void  doSetParameter(axParameter* aParameter)
      {
        int  id = aParameter->getIndex();
        float f = aParameter->getValue();
        switch(id)
        {
          case 0: // slider1
            mono = (int)f;
            break;
          case 1: // slider2
          {
            float sx = 16+f*1.20103;
            //float f0 = floor(exp(sx*log(1.059))*8.17742);
            float f0 = axFloor(axExpf(sx*axLogf(1.059))*8.17742);
            float omega = 2*PI*f0;
            float n = 1/(sr3 + omega); // sr3 = 0 in constructor...
            a0 = 2*omega*n;
            b1 = (sr3 - omega)*n;
            break;
          }
          case 2: // slider3
          {
            float gain = f;
            float gfactor = 4;
            float g1, g2;
            if (gain>0)
            {
              g1 = -gfactor*gain;
              g2 = gain;
            }
            else
            {
              g1 = -gain;
              g2 = gfactor*gain;
            }
            //lgain = exp(g1/amp)-1;
            //hgain = exp(g2/amp)-1;
            lgain = axExpf(g1/amp)-1;
            hgain = axExpf(g2/amp)-1;
            break;
          }
          case 3: // slider4
            //outgain = exp(f/amp);
            outgain = axExpf(f/amp);
            break;
        }

      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0] + DENORM;
        float spl1 = *aInputs[1] + DENORM;
        if (mono==1)
        {
          //process mono
          float input = (spl0+spl1)*0.5;
          lp_out = a0*input + b1*lp_out;
          float output = input + lgain*lp_out + hgain*(input - lp_out);
          spl0=spl1=output*outgain;
        }
        else
        {
          //process stereo
          float input = spl0;
          lp_out = a0*input + b1*lp_out;
          float output = input + lgain*lp_out + hgain*(input - lp_out);
          spl0 = output*outgain;

          float input_r = spl1;
          lp_out_r = a0*input_r + b1*lp_out_r;
          float output_r = input_r + lgain*lp_out_r + hgain*(input_r - lp_out_r);
          spl1 = output_r*outgain;
        }
        // ---------
        *aOutputs[0] = spl0 - DENORM;
        *aOutputs[1] = spl1 - DENORM;
      }

};

AX_MAIN(myDescriptor,myInstance)
