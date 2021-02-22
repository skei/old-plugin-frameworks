#ifndef s2_envelope_included
#define s2_envelope_included
//----------------------------------------------------------------------

/*

coeff = exp(-2 * pi * fc/fs)
coeff = exp(-1 / (tau * fs))
..where "tau" is the decay length in seconds

----------

delta = input - buffer
buffer = buffer + scale * delta

expr(n,r) = exp(ln(ln(r))-ln(ln(n)))
nroot(n,r) = n^(1/r)
pos(spd,smp) = (1-spd)^smp
spd(pos,smp) = 1-nroot(pos,smp)
smp(spd,pos) = expr(1-spd,pos)

expr()  = anti-power
nroot() = nth root
pos()   = position arrived at speed in samples
spd()   = speed required to arrive at position in samples
smp()   = samples required to arrive at position with speed

->

delta         = input - buffer
buffer        = buffer + scale * delta
pos(spd,smp)  = (1-spd)^smp
pos()         = position arrived at speed in samples

*/

//#define env_numstages 5

#include <math.h>

#define s2_env_rate_scale  30.0f
#define s2_env_threshold   S2_TINY

//----------------------------------------------------------------------

struct S2_EnvStage {
  float target;
  float rate;
};

//----------------------------------------------------------------------

// We calculate the coefficients using the time-constant equation:
// g = e ^ ( -1 / (time * sample rate) ),
// out = in + g * (out – in),

//----------------------------------------------------------------------

class S2_Envelope_ADSR {

  private:

    float       MSampleRate;
    float       MValue;
    int32       MStage;
    S2_EnvStage MStages[5]; // -,a,d,s,r

  public:

    S2_Envelope_ADSR() {
      //MScale = 50.0f;//6.0f;
      MStage = s2_es_off; // EEnvelopeStage
      MValue = 0.0f;
    }

    //----------

    void sampleRate(float ARate) { MSampleRate = ARate; }
    float value(void) { return MValue; }
    int32 stage(void) { return MStage; }

  public:

    /*
      we cheat a little, and multiply the ms value by 5..
      ..easier to make log/exp ish curves (x*x*x)..
    */

    float calcRate(float ms) {
      //return expf(-1 / (ms * MSampleRate));
      ms = (ms*ms); // a little hack to make the knob more 'sensitive' to short durations..
      //ms *= 2.0f;
      return 1.0f - expf(-1.0f / (ms*MSampleRate));
    }

    //----------

    void setAttack(float AValue) {
      //float r1 = AValue * s2_env_rate_scale;
      //float r2 = (r1*r1*r1) + 1;
      MStages[s2_es_attack].target = 1.0f;
      //MStages[s2_es_attack].rate   = 1.0f / r2;
      MStages[s2_es_attack].rate   = calcRate(AValue);
    }

    void setDecay(float AValue) {
      //float r1 = AValue * s2_env_rate_scale;
      //float r2 = (r1*r1*r1) + 1;
      //MStages[s2_es_decay].target = ATarget; // set in setSustain
      //MStages[s2_es_decay].rate = 1.0f / r2;
      MStages[s2_es_decay].rate = calcRate(AValue);
    }

    void setSustain(float AValue) {
      //float r1 = AValue;                  // * env_rate_scale;
      //float r2 = (r1*r1*r1);              // + 1;
      //MStages[s2_es_decay].target = r2;   // set in setSustain
      //MStages[s2_es_sustain].target = r2; // set in setSustain
      MStages[s2_es_decay].target = AValue;
      MStages[s2_es_sustain].target = AValue;
      MStages[s2_es_sustain].rate = 1.0f;
    }

    void setRelease(float AValue) {
      //float r1 = AValue * s2_env_rate_scale;
      //float r2 = (r1*r1*r1) + 1;
      MStages[s2_es_release].target = 0.0f;
      //MStages[s2_es_release].rate   = 1.0f / r2;
      MStages[s2_es_release].rate   = calcRate(AValue);
    }

    //----------

    //void setStage(int32 AStage, float ATarget, float ARate) {
    //  float r1 = ARate * env_rate_scale;
    //  float r2 = (r1*r1*r1) + 1;
    //  //float r2 = r1*r1 + 1;
    //  STrace("r2: %f\n",r2);
    //  MStages[AStage].target = ATarget;
    //  MStages[AStage].rate   = 1.0f / r2;
    //}

    //----------

    void setADSR(float a, float d, float s, float r) {
      //setStage(s2_es_attack,1.0f,a);
      //setStage(s2_es_decay,s,d);
      //setStage(s2_es_sustain,s,1,1);
      ////MStages[s2_es_sustain].target = s*s*s;
      ////MStages[s2_es_sustain].rate   = 1.0f;
      //setStage(s2_es_release,0.0f,r);
      setAttack(a);
      setDecay(d);
      setSustain(s);
      setRelease(r);
    }

    //----------

    void noteOn(void) {
      MStage = s2_es_attack;
      MValue = 0.0f;

    }

    //----------

    void noteOff(void) {
      MStage = s2_es_release;
    }

    //----------

    float process(void) {
      if (MStage == s2_es_off) return 0.0f;
      if (MStage == s2_es_finished) return 0.0f;
      if (MStage == s2_es_sustain) return MValue;
      float target = MStages[MStage].target;
      float rate   = MStages[MStage].rate;
      MValue += ( (target-MValue) * rate );
      if (fabs(target-MValue) <= s2_env_threshold) {
        MStage += 1;
      }
      return MValue;
    }

    //----------

    // http://www.kvraudio.com/forum/viewtopic.php?p=6515525#p6515525

    float processSteps(int32 ASteps) {
      if (ASteps==0) return MValue;

      float result = process();

      //if (ASteps>1) for (int32 i=1; i<ASteps; i++) process();

      float target = MStages[MStage].target;
      float rate   = MStages[MStage].rate;
      MValue += (target - MValue) * (1 - pow(1 - rate, ASteps));

      //if (fabs(target-MValue) <= s2_env_threshold) {
      //  MStage += 1;
      //}

      return result;
    }

    //----------

};

//----------------------------------------------------------------------
#endif
