#ifndef s2_waveguide_included
#define s2_waveguide_included
//----------------------------------------------------------------------

// something fishy with this..

/*
  based on: http://musicdsp.org/archive.php?classid=4#98
*/

/*
  Class for waveguide/delay effects
  Type : IIR filter
  References : Posted by arguru[AT]smartelectronix.com
  Notes :
  Flexible-time, non-sample quantized delay , can be used for stuff like waveguide synthesis or time-based (chorus/flanger) fx.
  MAX_WG_DELAY is a constant determining MAX buffer size (in samples)
*/

//template<int MAX_WG_DELAY>

//----------------------------------------------------------------------

// srate 48khz, 20hz = 2400 samples,, 192k = 9600

//#define MAX_WG_DELAY 65536
#define MAX_WG_DELAY 10240

#include "audio/filters/s2_filter_rc.h"

//----------------------------------------------------------------------

class S2_WaveGuide {

  private:

    float         MBuffer[MAX_WG_DELAY];
    //float         MBuffer2[MAX_WG_DELAY];
    int           MCounter;
    bool          MImpulse;
    bool          MWrapped;
    S2_Filter_RC  MFilter_LP;
    //S2_Filter_RC  MFilter_HP;

  public:

    bool wrapped(void)          { return MWrapped; }
    bool impulse(void)          { return MImpulse; }
    void impulse(bool imp=true) { MImpulse = imp; }
    void sampleRate(float rate) { MFilter_LP.sampleRate(rate); /*MFilter_HP.sampleRate(rate);*/ }
    void damp_lo(float damp)    { MFilter_LP.weight(damp); }
    //void damp_hi(float damp)    { MFilter_HP.weight(damp); }

  public:

    S2_WaveGuide() {
      clear();
      MImpulse = false;
    }

    //----------

    virtual ~S2_WaveGuide() {
    }

    //----------

    void clear(void) {
      MCounter = 0;
      MWrapped = false;
      /*
        this created a spike in cu each time a note was started..
        do we need to clear the buffer?
      */
      //for (int i=0; i<MAX_WG_DELAY; i++) MBuffer[i] = 0;
      S2_Memset(MBuffer,0,MAX_WG_DELAY*sizeof(float));
      //S2_Memset(MBuffer2,0,MAX_WG_DELAY*sizeof(float));
    }

    //----------

    /*
    float softclip(float x) {
      if (x <= -1) return (-2 / 3);
      else if (x >= 1) return (2 / 3);
      else return x- ((x*x*x) / 3);
    }
    */

    //----------

    /*
      - separate feed & tap..
      - dual delay lines, feed the output from one into the other
        with either filtering/damping, and/or inversion
    */

    //----------

    float feed(float AInput, float AFeedback, float ADelay) {
      if (MImpulse && MWrapped) AInput = 0;
      // calculate delay offset
      float back = (float)MCounter - ADelay;
      if (back < 0.0) back = MAX_WG_DELAY + back;
      int index0 = (int)back;
      int index_1 = index0-1;
      int index1 = index0+1;
      int index2 = index0+2;
      if (index_1 < 0) index_1 = MAX_WG_DELAY - 1;
      if (index1 >= MAX_WG_DELAY) index1 = 0;
      if (index2 >= MAX_WG_DELAY) index2 = 0;
      // interpolate
      float y_1= MBuffer [index_1];
      float y0 = MBuffer [index0];
      float y1 = MBuffer [index1];
      float y2 = MBuffer [index2];
      float x = (float)back - (float)index0;
      float c0 = y0;
      float c1 = 0.5f*(y1-y_1);
      float c2 = y_1 - 2.5f*y0 + 2.0f*y1 - 0.5f*y2;
      float c3 = 0.5f*(y2-y_1) + 1.5f*(y0-y1);
      float output = ((c3*x+c2)*x+c1)*x+c0;
      // feedback/damping

      float fb = MFilter_LP.process(AInput + output * AFeedback); // was +
      //fb = S2_Clamp(fb,-1,1);
      //fb = softclip(fb);
      fb = atan(fb);

      MBuffer[MCounter] = fb;
      MCounter++;
      if (MCounter >= ADelay) { MWrapped = true; }
      if (MCounter >= MAX_WG_DELAY) { MCounter = 0; /*MWrapped = true;*/ }
      return output;
    }
    //----------

    // tap = 0..1 (delay multiplier)
    // delay length

    /*

    float tap(float ADelay, float ATap) {
      // calculate delay offset
      float back = (float)MCounter - (ADelay*ATap);
      if (back < 0.0) back = MAX_WG_DELAY + back;
      int index0 = (int)back;
      int index_1 = index0-1;
      int index1 = index0+1;
      int index2 = index0+2;
      if (index_1 < 0) index_1 = MAX_WG_DELAY - 1;
      if (index1 >= MAX_WG_DELAY) index1 = 0;
      if (index2 >= MAX_WG_DELAY) index2 = 0;
      // interpolate
      float y_1= MBuffer [index_1];
      float y0 = MBuffer [index0];
      float y1 = MBuffer [index1];
      float y2 = MBuffer [index2];
      float x = (float)back - (float)index0;
      float c0 = y0;
      float c1 = 0.5f*(y1-y_1);
      float c2 = y_1 - 2.5f*y0 + 2.0f*y1 - 0.5f*y2;
      float c3 = 0.5f*(y2-y_1) + 1.5f*(y0-y1);
      float output = ((c3*x+c2)*x+c1)*x+c0;
      // feedback/damping
//      float fb = MFilter_LP.process(AInput + output * AFeedback); // was +
//      fb = S2_Clamp(fb,-1,1);
//      MBuffer[MCounter] = fb;
//      MCounter++;
//      if (MCounter >= ADelay) { MWrapped = true; }
//      if (MCounter >= MAX_WG_DELAY) { MCounter = 0; } //MWrapped = true; }
      return output;
    }

    */

};

//----------------------------------------------------------------------



//----------------------------------------------------------------------

#undef MAX_WG_DELAY

//----------------------------------------------------------------------
#endif
