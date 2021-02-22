#ifndef sonic_maximizer_included
#define sonic_maximizer_included
//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439
    license: ?
    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
    so i think it should be ok?
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define S3_DEBUG_MEM

#ifdef S3_PLUGIN_VST
  //#define S3_DEBUG_VST
  #define S3_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "s3.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 -S3_PI2 * freqHP
#define freqLP_p2 -S3_PI2 * freqLP


class myPlugin
: public S3_Plugin
{

  private:

    float slider1,slider2,slider3;
    float band1,band2,band3,amp;
    float tmplLP,tmprLP;
    float tmplHP,tmprHP;
    float xLP,xHP;
    float a0LP,a0HP;
    float b1LP,b1HP;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) final {
      MName       = "fx_sonic_maximizer";
      MAuthor     = "dominique guichaoua";
      MDescription    = MName;
      MUniqueId   = S3_MAGIC + s3_fx_sonic_maximizer_id;
      MNumInputs  = 2;
      MNumOutputs = 2;
      appendParameter( new S3_ParamFloat("Low Cont",  1,  0,  10, 0.1 ) );
      appendParameter( new S3_ParamFloat("Process",   1,  0,  10, 0.1 ) );
      appendParameter( new S3_ParamFloat("Outout",    1, -30, 0,  0.1 ) );
      slider1 = 0;
      slider2 = 0;
      slider3 = 0;
      band1   = 0;
      band2   = 1;
      band3   = 0;
      tmplLP  = 0;
      tmprLP  = 0;
      tmplHP  = 0;
      tmprHP  = 0;
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_stateChange(uint32 AState) final {
      switch(AState) {
        case s3_ps_resume:
          xHP  = expf(freqHP_p2/MSampleRate);
          a0HP = 1.0 - xHP;
          b1HP = -xHP;
          xLP  = expf(freqLP_p2/MSampleRate);
          a0LP = 1.0 - xLP;
          b1LP = -xLP;
          break;
      }
    }

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      switch (AIndex) {
        case 0:
          slider1 = AValue;
          band1 = expf( (slider1+3) / c_ampdB );
          break;
        case 1:
          slider2 = AValue;
          band3 = expf( (slider2+3) / c_ampdB );
          break;
        case 2:
          slider3 = AValue;
          amp = expf( slider3 / c_ampdB );
          break;
      }
      //band2 := 1; // exp(0/c_ampdB);
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (uint32 i=0; i<ANumSamples; i++) {
        float s0 = *input0++;
        float s1 = *input1++;
        tmplLP = a0LP*s0 - b1LP*tmplLP + cDenorm;
        tmprLP = a0LP*s1 - b1LP*tmprLP + cDenorm;
        float sp0 = tmplLP;
        float sp1 = tmprLP;
        tmplHP = a0HP*s0 - b1HP*tmplHP + cDenorm;
        tmprHP = a0HP*s1 - b1HP*tmprHP + cDenorm;
        float sp4 = s0 - tmplHP;
        float sp5 = s1 - tmprHP;
        float sp2 = s0 - sp0 - sp4;
        float sp3 = s1 - sp1 - sp5;
        *output0++ = (sp0 * band1 + sp2 /* * band2 */ + sp4 * band3) * amp;
        *output1++ = (sp1 * band1 + sp3 /* * band2 */ + sp5 * band3) * amp;
      }
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

