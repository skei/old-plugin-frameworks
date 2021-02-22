#ifndef s2_sinusoid_included
#define s2_sinusoid_included
//----------------------------------------------------------------------

// sinusoid (approximation)
// http://forum.cockos.com/showpost.php?p=348182&postcount=6

#include <math.h> // fabs

//----------

class S2_Sinusoid {

  private:

    float srate;

  public:
    float ph,phadd;

  public:

    S2_Sinusoid() {
      ph = 0.0;
      phadd = 0.0;
      srate = 0.0;
    }

    void setSampleRate(float ASampleRate) {
      srate = ASampleRate;
    }

    void setFreq(float AFreq) {
      phadd = (AFreq/srate) * 4.0;
    }

    void setPhase(float APhase) {
      ph = 0.0;
    }

    float process(void) {
      ph += phadd;
      if (ph > 2) ph -= 4.0;
      return ph * (2.0 - fabs(ph));
    }

};

//----------------------------------------------------------------------
#endif
