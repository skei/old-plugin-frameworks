#ifndef fx_transient_included
#define fx_transient_included
//----------------------------------------------------------------------

/*
  based on:
    ?
    (C) 2007, Michael Gruhn
    http:// ?
    license: gpl
*/

//----------------------------------------------------------------------

// {$define log:=ln}

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

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
{

  private:

    float attack, sustain, vol;
    float b1Env1, a0Env1, b1Env2, a0Env2, b1Env3, a0Env3;
    float tmpEnv1, tmpEnv2, tmpEnv3;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_transient";
      MInfo.author     = "skeilib";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      appendParameter( new SParamFloat( "attack",  0, -100, 100, 1 ) );
      appendParameter( new SParamFloat( "sustain", 0, -100, 100, 1 ) );
      appendParameter( new SParamFloat( "output",  0, -24,  6     ) );
      tmpEnv1 = 0;
      tmpEnv2 = 0;
      tmpEnv3 = 0;

    }

    //----------

    //virtual
    void on_destroy(void) {
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    /*
      @init
      b1Env1 = -exp(-30 / srate );
      a0Env1 = 1.0 + b1Env1;
      b1Env2 = -exp(-1250 / srate );
      a0Env2 = 1.0 + b1Env2;
      b1Env3 = -exp(-3 / srate );
      a0Env3 = 1.0 + b1Env3;
    */

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_resume:
          b1Env1 = -exp(-30 / MState.sampleRate );
          a0Env1 = 1.0 + b1Env1;
          b1Env2 = -exp(-1250 / MState.sampleRate );
          a0Env2 = 1.0 + b1Env2;
          b1Env3 = -exp(-3 / MState.sampleRate );
          a0Env3 = 1.0 + b1Env3;
          break;
      }
    }

    //virtual
    void on_transportChange(uint32 ATransport) {
      /*
      if (ATransport & spt_changed) {}  // play, cycle or record state has changed
      if (ATransport & spt_play) {}     // Host sequencer is currently playing
      if (ATransport & spt_cycle) {}    // Host sequencer is in cycle mode
      if (ATransport & spt_record) {}   // Host sequencer is in record mode
      if (ATransport & spt_awrite) {}   // automation write mode active (record parameter changes)
      if (ATransport & spt_aread) {}    // automation read mode active (play parameter changes)
      */
    }

    /*
      @slider
      attack=slider1/100;
      sustain=slider2/50;
      vol = 2^(slider3/6);
    */

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: attack  = AValue/100;       break;
        case 1: sustain = AValue/50;        break;
        case 2: vol     = pow(2,AValue/6);  break;
      }
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    }

    //virtual
    void on_programChange(int32 AIndex) {
    }

    //virtual
    void on_preProgram(int32 AIndex) {
    }

    //virtual
    void on_postProgram(void) {
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    /*
      @sample
      maxSpls = max(abs(spl0),abs(spl1));
      env1 = sqrt(tmpEnv1 = a0Env1*maxSpls - b1Env1*tmpEnv1);
      env2 = sqrt(tmpEnv2 = a0Env2*maxSpls - b1Env2*tmpEnv2);
      env3 = sqrt(tmpEnv3 = a0Env3*maxSpls - b1Env3*tmpEnv3);
      gain = exp(log(max(env2/env1,1))*attack) * exp( log( max(env3/env1,1))*sustain);
      spl0 *= (gain *= vol);
      spl1 *= gain;
    */

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ANumSamples) {
      SSample* input0 = AInputs[0];
      SSample* input1 = AInputs[1];
      SSample* output0 = AOutputs[0];
      SSample* output1 = AOutputs[1];
      for (int i=0; i<ANumSamples; i++) {
        float spl0 = *input0++;
        float spl1 = *input1++;
        //...
        float maxSpls = SMax(abs(spl0),abs(spl1));
        tmpEnv1 = a0Env1*maxSpls - b1Env1*tmpEnv1;
        tmpEnv2 = a0Env2*maxSpls - b1Env2*tmpEnv2;
        tmpEnv3 = a0Env3*maxSpls - b1Env3*tmpEnv3;
        float env1 = /*S*/sqrt(tmpEnv1);
        float env2 = /*S*/sqrt(tmpEnv2);
        float env3 = /*S*/sqrt(tmpEnv3);
        float gain = /*S*/exp(log(SMax(env2/env1,1))*attack) * exp( log( SMax(env3/env1,1))*sustain);
        gain *= vol;
        spl0 *= gain;
        spl1 *= gain;
        //env1 := KKillDenorm(env1);
        //env2 := KKillDenorm(env2);
        //env3 := KKillDenorm(env3);
        //...
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


