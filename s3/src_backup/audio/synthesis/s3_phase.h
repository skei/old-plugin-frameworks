#ifndef s2_phase_included
#define s2_phase_included
//----------------------------------------------------------------------

// 0..1

//----------

class S2_Phase {

  private:
    float srate;
    float irate;

  public:

    float t,dt;

  public:

    S2_Phase() {
      t = 0;
      dt = 0;
      srate = 0;
    }

    void setSampleRate(float ASampleRate) {
      //S2_Trace("samplerate: %f\n",ASampleRate);
      srate = ASampleRate;
      irate = 1.0f / ASampleRate;
    }

    // AFrreq = hz

    void setFreq(float AFreq/*, float ASampleRate*/) {
      //dt = AFreq / ASampleRate;
      dt = AFreq * irate;
    }

    void addFreq(float AFreq, float ASampleRate) {
      //dt += AFreq / ASampleRate;
      dt += AFreq * irate;
    }

    void setPhase(float APhase) {
      t = APhase;
    }

    void resetPhase(void) {
      t = 0;
    }

    /*
      todo: negative frequency (t%1)
    */

    float process(void) {
      float result = t;
      t += dt;
      // mod 1

      //if (t >= 1) t -= 1;
      t = S2_Fract(t);

      //t -= floor(t);
      //while (p>=1) p -= 1;
      //while (p<=0) p += 1;
      return result;
    }

    /*
      todo: negative frequency (t%1)
    */

    float process(float AMod) {
      float result = t;
      float a = dt + AMod;
      t += a;//dt;
      //if (t >= 1) t -= 1;
      t = S2_Fract(t);
      //t -= floor(t);
      //while (p>=1) p -= 1;
      //while (p<=0) p += 1;
      return result;
    }

    //----------

    void skip(int32 ASteps) {
      t += (dt*ASteps);
      t = S2_Fract(t);
    }

};

//----------------------------------------------------------------------
#endif
