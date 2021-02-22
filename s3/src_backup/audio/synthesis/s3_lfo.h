#ifndef s2_lfo_included
#define s2_lfo_included
//----------------------------------------------------------------------

#include "audio/synthesis/s2_phase.h"
#include "audio/waveforms/s2_waveform_naive.h"

class S2_Lfo {
  private:
    uint32    MType;
    float     MSpeed; // hz
    float     MSampleRate;
    S2_Phase  MPhase;
    float     MValue;
    float     prev_ph;

  public:

    S2_Lfo() {
      MType = 0;
      MSpeed = 0;
      MSampleRate = 0;
      prev_ph = 1;
    }

    void sampleRate(float ARate) {
      MSampleRate = ARate;
      MPhase.setSampleRate(ARate);
    }

    void type(uint32 AType) {
      MType = AType;
    }

    void speed(float ASpeed) {
      MPhase.setFreq(ASpeed);
    }

    void reset(void) {
      MPhase.resetPhase();
      prev_ph = 1; // force calc new value
    }

    float process(void) {
      float ph = MPhase.process();
      //float v = 0;
      switch(MType) {
        case 0: break; //  Off,
        case 1: MValue = S2_Waveform_Sin_Naive(ph); break; //  Sine,
        case 2: MValue = S2_Waveform_Ramp_Naive(ph); break; //  Ramp,
        case 3: MValue = S2_Waveform_Saw_Naive(ph); break; //  Saw,
        case 4: MValue = S2_Waveform_Squ_Naive(ph); break; //  Squ,
        case 5: MValue = S2_Waveform_Tri_Naive(ph); break; //  Tri,
        case 6: if (ph < prev_ph) MValue = S2_RandomSigned(); prev_ph = ph; break; //  s&h / Noise
      }
      return MValue;
    }

    //float process(float t) {
    //  return 0;
    //}

    float processSteps(int32 ASteps) {
      if (ASteps==0) return MValue;
      float result = process();
      //if (ASteps>1) for (int32 i=1; i<ASteps; i++) {
      //  process();
        MPhase.skip(ASteps);
      //}
      return result;
    }

    //----------



};

//----------------------------------------------------------------------
#endif
