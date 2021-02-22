#ifndef s2_osc1_included
#define s2_osc1_included
//----------------------------------------------------------------------

//#if 0

#include "audio/waveforms/s2_waveform_polyblep.h"

class S2_Osc1 {

  //public:
  //  float     p1;
  //  float prev_t1;

  private:
    float MPulseWidth;
    float MSawSqu;
    float MSquTri;
    float MTriSin;
    float ph;
    float phadd;
    float z1;
    float MSampleRate;
    float MIrate;
    //float                   MSampleRate;
    //uint32                  MOverSample;
    //SWaveform_Saw_PolyBlep  MSaw1;
    //SWaveform_Saw_PolyBlep  MSaw2;

  public:

    S2_Osc1() {
      //MSampleRate = 44100.0f;
      //MOverSample = 1;
      MPulseWidth = 0.5f;
      MSawSqu     = 0.0f;
      MSquTri     = 0.0f;
      MTriSin     = 0.0f;
      z1          = 0;
      ph          = 0;
      phadd       = 0;
    }

  public:

    //float t(void)     { return ph; }
    //float dt(void)    { return phadd; }
    //void  t(float v)  { ph = v; }
    //void  dt(float v) { phadd = v; }

  public:

    void setSrate(float rate) {
      MSampleRate = rate;
      MIrate = 1.0f / MSampleRate;
    }

    void setFreq(float hz) {
      //phadd = hz / srate;
      phadd = hz * MIrate;
    }

  public:

    void setPhase(float a) {
      ph = a;
    }

    void setPhaseAdd(float a) {
      phadd = a;
    }

    void reset(void) {
      ph = 0;
      phadd = 0;
      z1 = 0;
    }

  public:

    void setPulseWidth(float pw) {
      MPulseWidth = pw;
    }

    void setSawSqu(float sawsqu) {
      MSawSqu = sawsqu;
    }

    void setSquTri(float squtri) {
      MSquTri = squtri;
    }

    void setTriSin(float trisin) {
      MTriSin = trisin;
    }

    float process() {
      ph += phadd;
      //if (ph >= 1.0f) ph -= 1.0f;
      ph = S2_Fract(ph);
      float t1 = ph + 0.5f;
      //if (t1 >= 1.0f) t1 -= 1.0f;
      t1 = S2_Fract(t1);
      float saw1 = 2.0f * t1 - 1.0f;
      saw1 -= S2_PolyBlep(t1,phadd);
      float t2 = t1 + MPulseWidth;
      //if (t2 >= 1.0f) t2 -= 1.0f;
      t2 = S2_Fract(t2);
      float saw2 = 2.0f * t2 - 1.0f;
      saw2 -= S2_PolyBlep(t2,phadd);
      float squ = saw1 - (saw2*MSawSqu);
      z1 = (phadd * squ) + ((1.0f - phadd) * z1);
      // squ-tri
      float tri = squ * (1-MSquTri) + (z1*4.0f) * MSquTri;
      float sine = tri * (1-MTriSin) + sin(ph*S2_PI2) * MTriSin;
      //float tr = fabsf(ph - 0.5f) * 2.0f - 0.5f;
      //float q = tr*tr*tr;
      //float s = 3.0f / 0.9379f;
      //float sine = (tr - q*2.0f + q * fabsf(tr)) * s;
      // crossfade tri-sin
      float out = tri * (1-MTriSin) + sine * MTriSin;
      return out;

    }

};

//#endif // 0

//----------------------------------------------------------------------
#endif
