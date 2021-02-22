#ifndef fx_filter_ms20_included
#define fx_filter_ms20_included
//----------------------------------------------------------------------

// nc -U -l -k /tmp/skeilib.socket

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- plugins -----

//#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
//#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----- format -----

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #define SKEI_XLIB
  #define SKEI_XRENDER
  #define SKEI_XFT
#endif

//----- debug -----

//#define SKEI_DEBUG_MEM

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #include "skei_editor.h"
#endif

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

const char* txt_type[2] = { "lowpass", "highpass" };

//----------

class myPlugin
: public SPlugin
{

  private:
    float   z1, z2;
    uint32  p_type;
    float   p_freq, p_reso, p_clip;
    float   p_pre, p_post;
    //float dt; // 1/srate

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_filter_ms20";
      MInfo.author     = "skeilib";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + SKEI_ID_NONE;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;

      appendParameter( new SParamText(  "type", 0,    2,    txt_type  ));
      appendParameter( new SParamFloat( "freq", 1.0                   ));
      appendParameter( new SParamFloat( "reso", 0.0,  0.0,  1.1       ));
      appendParameter( new SParamFloat( "clip", 0.5,  0.01, 4         ));
      appendParameter( new SParamFloat( "pre",  1.0,  0,    2         ));
      appendParameter( new SParamFloat( "post", 1.0,  0,    2         ));

      z1 = 0;
      z2 = 0;

      //dt = 0;

    }

    //----------

    //virtual
    void on_destroy(void) {
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_sampleRate:
          //dt = 1.0f / MState.sampleRate;
          break;
        //case sps_resume: break;
      }
    }


    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      float v3 = AValue*AValue*AValue;
      float v5 = AValue*AValue*AValue*AValue*AValue;
      switch(AIndex) {
        case 0: p_type  = AValue; break;
        case 1: p_freq  = v5;     break;
        case 2: p_reso  = AValue; break;
        case 3: p_clip  = AValue; break;
        case 4: p_pre   = v3;     break;
        case 5: p_post  = v3;     break;
      }
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // arbitrarily made up, asymmetric to introduce even harmonics

    float ms20_filter_shape_gain(float x) {
      return 1 / sqrt(1.0 + 0.66 * x + x * x);
    }

    //----------

    float ms20_filter_shape(float clip_level, float x) {
      return x * ms20_filter_shape_gain(x / clip_level);
    }

    //----------

    float ms20_filter_tick(float freq, float reso, float clip_level, float lp_input, float hp_input) {
      float k = fmin(1.0, freq*SKEI_PI2);
      float z2_prime = z2 + hp_input;
      float z1_prime = z1 + ms20_filter_shape(clip_level, 2 * z2_prime * reso);
      z1 += (lp_input - z1_prime) * k;
      z1_prime += (lp_input - z1_prime) * k;
      z2 += (z1_prime - z2_prime) * k;
      z2_prime += (z1_prime - z2_prime) * k;
      return z2_prime;
    }

    //----------

    float ms20_filter_tick_lp(float freq, float reso, float clip_level, float input) {
      return ms20_filter_tick(freq, reso, clip_level, input, 0.0);
    }

    //----------

    float ms20_filter_tick_hp(float freq, float reso, float clip_level, float input) {
      return ms20_filter_tick(freq, reso, clip_level, 0.0, input);
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ANumSamples) {
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];


      for (uint32 i=0; i<ANumSamples; i++) {
        float spl0 = *input0++;
        float spl1 = *input1++;

        // todo: move switch outside of loop?
        switch (p_type) {
          case 0: // lowpass
            spl0 = ms20_filter_tick_lp(p_freq*0.5,p_reso,p_clip,spl0*p_pre) * p_post;
            break;
          case 1: // highpass
            spl0 = ms20_filter_tick_hp(p_freq*0.5,p_reso,p_clip,spl0*p_pre) * p_post;
            break;
        }

        spl1 = spl0;
        *output0++ = spl0;
        *output1++ = spl1;
      }
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif


