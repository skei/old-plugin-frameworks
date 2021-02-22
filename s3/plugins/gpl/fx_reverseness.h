#ifndef reverseness_included
#define reverseness_included
//----------------------------------------------------------------------

//#define SKEI_DEBUG_MEM
#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

#ifdef SKEI_PLUGIN_HAS_EDITOR
  #include "skei_editor.h"
#endif

//----------------------------------------------------------------------

// 64mb
#define BUFFER_SIZE (1024*1024*64)

class myPlugin
: public SPlugin
{

  private:
    float   buffer[BUFFER_SIZE];
    bool    init;
    uint32  pos;
    float   len,wet,dry,fadelen,halflen;
    float   slider1,slider2,slider3,slider4;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "reverseness";
      MInfo.author     = "skeilib";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + SKEI_ID_NONE;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;

      /*
      //slider1:4<0,128,1>length (beats)
      slider1:500<0,4000,10>length (ms)
      slider2:0<-120,6,1>wet mix (dB)
      slider3:-120<-120,6,1>dry mix (dB)
      slider4:0.1<0,0.5>edge overlap (0..0.5)
      */

      appendParameter( new SParamFloat( "length",   500,  0,   4000 ));
      appendParameter( new SParamFloat( "wet mix",  0,   -120, 6    ));
      appendParameter( new SParamFloat( "dry mix", -120, -120, 6    ));
      appendParameter( new SParamFloat( "overlap",  0.1,  0,   0.5  ));

      init = true;
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
      /*
      switch(AState) {
        case sps_sampleRate: break;
        case sps_blockSize: break;
        case sps_resume: break;
      }
      */
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: slider1 = AValue; break;
        case 1: slider2 = AValue; break;
        case 2: slider3 = AValue; break;
        case 3: slider4 = AValue; break;
      }
      init = true;
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ANumSamples) {
      if (init) {
        //len = srate * (60 / tempo * slider1);
        len = MState.sampleRate * slider1/1000;
        wet = pow( 2, (slider2/6) );
        dry = pow( 2, (slider3/6) );
        fadelen = len * slider4 * 0.5;
        //freembuf(len * 2);
        halflen = len * 0.5;
        init = false;
      }
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (int32 i=0; i<ANumSamples; i++) {
        float spl0 = *input0++;
        float spl1 = *input1++;

        if (pos>=len) pos = 0;

        uint32 p2 = ((len - pos) * 2);
        //s0 = ((len - pos) * 2)[0];
        //s1 = ((len - pos) * 2)[1];
        //(pos * 2)[0] = spl0;
        //(pos * 2)[1] = spl1;
        float s0 = buffer[p2];
        float s1 = buffer[p2+1];
        buffer[p2] = spl0;
        buffer[p2+1] = spl1;

        float pp = len - pos;

        //float w2 = pp < fadelen ? (pp/fadelen) : ( pp > len-fadelen ? (len-pp) / fadelen : 1   );
        float w2;
        if (pp < fadelen) w2 = (pp/fadelen);
        else if (pp > len-fadelen) w2 = (len-pp) / fadelen;
        else w2 = 1.0;

        if (w2 >= 1.0) {
          pp = abs(halflen - pos);
          w2 = pp < fadelen ? pp / fadelen : 1;
        }
        spl0 = s0 * wet * w2 + spl0 * (dry + 1 - w2);
        spl1 = s1 * wet * w2 + spl1 * (dry + 1 - w2);
        //(pos += 1) >= len ? pos = 0;
        pos += 1;

//        if (pos>=len) pos = 0;

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





#if 0

// This effect Copyright (C) 2004 and later Cockos Incorporated
// License: GPL - http://www.gnu.org/licenses/gpl.html

desc: reverseness X sync (reverses chunks)
slider1:4<0,128,1>length (beats)
slider2:0<-120,6,1>wet mix (dB)
slider3:-120<-120,6,1>dry mix (dB)
slider4:0.1<0,0.5>edge overlap (0..0.5)

@slider
  init = 0;

@block
!init ?
(
  len = srate * (60 / tempo * slider1);
  wet = 2 ^ (slider2 / 6);
  dry = 2 ^ (slider3 / 6);
  fadelen = len * slider4 * 0.5;
  freembuf(len * 2);
  halflen = len * 0.5;
  init = 1;
);

@sample

s0 = ((len - pos) * 2)[0];
s1 = ((len - pos) * 2)[1];
(pos * 2)[0] = spl0;
(pos * 2)[1] = spl1;

pp = len - pos;
w2 = pp < fadelen ? pp / fadelen : (pp > len - fadelen ? (len - pp) / fadelen : 1);
w2 == 1 ?
(
  pp = abs(halflen - pos);
  w2 = pp < fadelen ? pp / fadelen : 1;
);

spl0 = s0 * wet * w2 + spl0 * (dry + 1 - w2);
spl1 = s1 * wet * w2 + spl1 * (dry + 1 - w2);

(pos += 1) >= len ? pos = 0;

#endif // 0

