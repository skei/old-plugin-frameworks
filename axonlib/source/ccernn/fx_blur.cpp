#define AX_NOGUI
#include "base/axBase.h"
#include "par/parFloat.h"
#include "par/parInteger.h"

//----------------------------------------------------------------------
//TODO: fix this!

char* str_params[] = { (char*)"size", (char*)"decay", (char*)"xfade", (char*)"volume", (char*)"freeze" };

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_blur"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 5; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

//----------------------------------------------------------------------

char* str_onoff[] = { (char*)"off", (char*)"on" };

//----------

class myInstance : public AX_INSTANCE
{
  private:
    float*  BUF;
    float*  BUF_t;
    int     pos;
    int     bufsize;
    int     bufsize_t;
  public:
    float       decay, decay_t;
    float       decay2;
    float       vol;
    float       bs,d,d2,v,fr;
  public:
    parFloat*   p_Size;
    parFloat*   p_Decay;
    parFloat*   p_Xfade;
    parFloat*   p_Volume;
    parInteger* p_Freeze;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {

        BUF     = new float[0x40000];
        axMemset(BUF,0,0x30000*4);
        BUF_t   = BUF + 0x10000;
        pos     = 0;
        bufsize = 0;
        decay   = 0;
        //
        float srate = getSampleRate();
        bufsize_t = srate * (bs*bs);
        decay_t   = d*d;
        decay2    = d2*d2;
        vol       = v*v;

        //describe("fx_blur","ccernn","axonlib example",0,AX_MAGIC+0x1008);
        //setupAudio(2,2);
        //appendParameter( new axParameter(this,"gain","",0) );
        appendParameter( p_Size   = new parFloat(  this,"size",  "", 0.5,  0,1 ) );
        appendParameter( p_Decay  = new parFloat(  this,"decay", "", 0.2,  0,1 ) );
        appendParameter( p_Xfade  = new parFloat(  this,"xfade", "", 0.5,  0,1 ) );
        appendParameter( p_Volume = new parFloat(  this,"volume","", 0.7,  0,1 ) );
        appendParameter( p_Freeze = new parInteger(this,"freeze","", 0,    0,1, str_onoff)  );
        setupParameters();
      }

    virtual ~myInstance()
      {
        delete BUF;
      }

    //todo:
    // srate is not available until resume...

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float srate = getSampleRate(); // not available until is_Resume
        int id  = aParameter->getIndex();
        float f = aParameter->getValue();
        switch(id)
        {
          case 0:
            bs = f / 2;
            bufsize_t = srate * (bs*bs);
            break;
          case 1:
            d = f;
            decay_t = d*d;
            break;
          case 2:
            d2 = f / 10;
            decay2 = d2*d2;
            break;
          case 3:
            v = f * 2;
            vol = v*v;
            break;
          case 4:
            fr = f;
            break;
        }

      }

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0];
        float spl1 = *aInputs[1];

        bufsize += ( bufsize_t - bufsize ) * decay2;
        decay   += ( decay_t   - decay   ) * decay2;

        if( fr<0.5 ) BUF_t[pos] = spl0;
        BUF[pos] += ( BUF_t[pos] - BUF[pos] ) * decay;
        spl0 = BUF[pos] * vol;

        int po2 = pos + 0x20000;

        if( fr<0.5 ) BUF_t[po2] = spl1;
        BUF[po2] += ( BUF_t[po2] - BUF[po2] ) * decay;
        spl1 = BUF[po2] * vol;

        pos += 1;
        if( pos >= (int)bufsize ) pos=0;

        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_MAIN(myDescriptor,myInstance)
