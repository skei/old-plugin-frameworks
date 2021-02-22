#ifndef fx_frac_delay_included
#define fx_frac_delay_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define S3_DEBUG_MEM
//#define S3_DEBUG_VST
#define S3_DEBUG_SOCKET

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "s3.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

// 20 hz = 44100/20 = 2205
#define S3_MAX_SAMPLES 4096

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public S3_Plugin
{

  private:

    float buf[S3_MAX_SAMPLES*2*2];
    int32 pos;
    float size;
    float frac;
    float z0;
    float z00;
    float z01;
    //
    float dly;
    float fbk;
    float wet;
    float dry;

  public:

    //virtual
    void on_create(void) final {
      MName         = "fx_frac_delay";
      MAuthor       = "tor.helge.skei";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_fx_frac_delay_id;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      MFlags       |= s3_pf_receiveMidi;
      appendParameter( new S3_ParamFloat( "delay",     100, 0, S3_MAX_SAMPLES ));
      appendParameter( new S3_ParamFloat( "feedback",  0.9, 0, 0.99 ));
      appendParameter( new S3_ParamFloat( "wet",      -6, -60, 0 ));
      appendParameter( new S3_ParamFloat( "dry",      -6, -60, 0 ));
      pos  = 0;
      size = 0;
      frac = 0;
      z00  = 0;
      z01  = 0;
    }

    //----------

    //virtual
    void on_destroy(void) final {
    }

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      switch(AIndex) {
        case 0: dly = AValue; size = floorf(dly); frac = dly-size; break;
        case 1: fbk = AValue;
        case 2: wet = powf(2,AValue/6);
        case 3: dry = powf(2,AValue/6);
      }
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) final {
      uint32 m = AMsg1 & 0xF0/*240*/;
      if (m == 0x90/*(9*16)*/) {
        uint32 note = AMsg2 & 127;
        float freq = 440.0 * powf(2,(note-69.0)/12.0);
        //uint32 vel = AMsg3 & 127;
        //wet = vel / 127;
        dly = MSampleRate / freq;
        //slider1 := dly;
        //slider3 := wet;
        //sliderchange(slider1+slider3);
        size = floorf(dly);
        frac = dly - size;
        pos  = 0;
        z0   = 0;
      }
    }

    //----------

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {

      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];

      for (uint32 i=0; i<ANumSamples; i++) {

        float spl0 = *input0++;//*AInputs[0];
        float spl1 = *input1++;//*AInputs[1];
        int32 pos2 = pos*2;
        float in0 = spl0;
        float in1 = spl1;
        float d00 = buf[pos2];
        float d01 = buf[pos2+1];
        float i00,i01;
        if (dly<1) {
          i00 = (in0*(1-frac)) + (d00*frac);
          i01 = (in1*(1-frac)) + (d01*frac);
        } else {
          i00 = (d00*(1-frac)) + (z00*frac);
          i01 = (d01*(1-frac)) + (z01*frac);
        }
        buf[pos2]   = in0 + (fbk*i00);
        buf[pos2+1] = in1 + (fbk*i01);
        z00 = d00;
        z01 = d01;
        pos += 1;
        if (pos>=size) pos = 0; //-=size
        spl0 = (in0*dry) + (i00*wet);
        spl1 = (in1*dry) + (i01*wet);
        /**AOutputs[0]*/ *output0++ = spl0;
        /**AOutputs[1]*/ *output1++ = spl1;
      }

    }

};

//----------

#undef S3_MAX_SAMPLES

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

