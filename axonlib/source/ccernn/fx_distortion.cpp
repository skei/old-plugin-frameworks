#define AX_NOGUI
#include "base/axBase.h"
#include "par/parInteger.h"
#include "par/parFloat.h"
#include "dsp/dspRC.h"

//----------------------------------------------------------------------

char* str_params[] = { (char*)"type", (char*)"thresh", (char*)"pregain", (char*)"postgain", (char*)"filter", (char*)"volume" };

//----------

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"fx_ditortion"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 6; }
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};


//----------------------------------------------------------------------

char* str_type[] =
{
  (char*)"bypass",
  (char*)"clip",
  (char*)"fold",
  (char*)"wrap+",
  (char*)"wrap",
  (char*)"wrap-"
};

//----------------------------------------------------------------------

class myInstance : public AX_INSTANCE
{
  private:
  // process
    dspRC       flt0,flt1;
  //'internal' parameters
    int         m_Type;
    float       m_Thr;
    float       m_Pre;
    float       m_Post;
    float       m_Flt;
    float       m_Vol;
  //vst parameters
    parInteger* p_Type;
    parFloat*   p_Thr;
    parFloat*   p_Pre;
    parFloat*   p_Post;
    parFloat*   p_Flt;
    parFloat*   p_Vol;

  public:

    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        //describe("fx_distortion","ccernn","axonlib example",2,AX_MAGIC+0x1003);
        //setupAudio(2,2,false);
        appendParameter( p_Type = new parInteger( this,"type",      "", 0, 0,5, str_type) );
        appendParameter( p_Thr  = new parFloatPow(this,"threshold", "", 1, 0,1,0, 3 ) );
        appendParameter( p_Pre  = new parFloatPow(this,"pre gain",  "", 1, 1,2,0, 3 ) );
        appendParameter( p_Post = new parFloatPow(this,"post gain", "", 1, 0,2,0, 3 ) );
        appendParameter( p_Flt  = new parFloatPow(this,"filter",    "", 1, 0,1,0, 3 ) );
        appendParameter( p_Vol  = new parFloatPow(this,"volume",    "", 1, 0,1,0, 3 ) );
        setupParameters();
      }

    //----------

    virtual void doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch ( aParameter->getIndex() )
        {
          case 0: m_Type = (int)val;  break;
          case 1: m_Thr  =      val;  break;;
          case 2: m_Pre  =      val;  break;
          case 3: m_Post =      val;  break;
          case 4: m_Flt  =      val;
                  flt0.setWeight(m_Flt);
                  flt1.setWeight(m_Flt);
                                      break;
          case 5: m_Vol  =      val;  break;
        }
      }

    //----------

    virtual void  doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float in0 = *aInputs[0];
        float in1 = *aInputs[1];
        float out0 = in0 * m_Pre;
        float out1 = in1 * m_Pre;
        float over,under;
        switch (m_Type)
        {
          case 1: // clip
            out0 = axMin(axMax(out0,-m_Thr),m_Thr);
            out1 = axMin(axMax(out1,-m_Thr),m_Thr);
            break;
          case 2: // fold
            if( in0> m_Thr ) { over =in0-m_Thr; out0= m_Thr-over;  }  // l+
            if( in0<-m_Thr ) { under=in0+m_Thr; out0=-m_Thr-under; }  // l+
            if( in1> m_Thr ) { over =in1-m_Thr; out1= m_Thr-over;  }  // r+
            if( in1<-m_Thr ) { under=in1+m_Thr; out1=-m_Thr-under; }  // r-
            break;
          case 3: // wrap+
            if( in0> m_Thr ) { over =in0-m_Thr; out0=-m_Thr+over;  }
            if( in0<-m_Thr ) { under=in0+m_Thr; out0= m_Thr+under; }
            if( in1> m_Thr ) { over =in1-m_Thr; out1=-m_Thr+over;  }
            if( in1<-m_Thr ) { under=in1+m_Thr; out1= m_Thr+under; }
            break;
          case 4: // wrap (flat)
            if( in0> m_Thr ) { out0=-m_Thr; }
            if( in0<-m_Thr ) { out0= m_Thr; }
            if( in1> m_Thr ) { out1=-m_Thr; }
            if( in1<-m_Thr ) { out1= m_Thr; }
            break;
          case 5: // wrap-
            if( in0> m_Thr ) { over =in0-m_Thr; out0=-m_Thr-over;  }
            if( in0<-m_Thr ) { under=in0+m_Thr; out0= m_Thr-under; }
            if( in1> m_Thr ) { over =in1-m_Thr; out1=-m_Thr-over;  }
            if( in1<-m_Thr ) { under=in1+m_Thr; out1= m_Thr-under; }
            break;
        }
        flt0.setTarget(out0);
        flt1.setTarget(out1);
        out0 = flt0.process(out0);
        out1 = flt1.process(out1);
        out0 *= m_Post;
        out1 *= m_Post;
        // filter?
        *aOutputs[0] = out0*m_Vol;
        *aOutputs[1] = out1*m_Vol;
      }

};

//----------------------------------------------------------------------
AX_MAIN(myDescriptor,myInstance)
