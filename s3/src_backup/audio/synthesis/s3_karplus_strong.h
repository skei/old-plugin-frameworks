#ifndef s2_karplus_strong_included
#define s2_karplus_strong_included
//----------------------------------------------------------------------

#include "s2_waveguide.h"

//----------

#define s2_ks_off
#define s2_km_exciting
#define s2_km_resonating
//#define s2_km_decaying

//----------

class S2_KarplusStrong {

  private:

    S2_WaveGuide  MWaveGuide;
    uint32        MMode;

    float         MDelay;
    float         MFeedback;

  public:

    void size(float ASize) {
    }

    void excite(float AInput) {
      MExcitation = MWaveGuide.feed(AInput,MFeedback,MDelay);
    }

    void resonate(void) {
    }

    float output(void) {
      return 0;
    }

}

//----------------------------------------------------------------------
#endif
