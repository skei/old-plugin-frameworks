#ifndef s2_filter_cytomic_included
#define s2_filter_cytomic_included
//----------------------------------------------------------------------

// unfinished, untested..

/*

  http://www.cytomic.com/files/dsp/SvfLinearTrapOptimised2.pdf

  Algorithm for all responses

  "As has been demonstrated we can alter the cutoff, Q, and output mix to
  obtain all the various continuous bi-quadratic responses. This leads to a
  generic forumulation where, at the cost of additional computation, we can
  implement all the different responses with the same generic code, which can
  be useful for vector implementations.
  With the appropriate interpolation you could also smoothly 'œmorph' between
  all the responses, at each in between point are adjusting the mix of outputs
  as well as the cutoff and Q, so the resultant filter will remain stable and
  smooth even with audio rate modulation, although aliasing well result if the
  resultant modulation causes frequency or amplitude modulation sidebands to
  exceed the nyquist limit

*/

/*
  mode
  cutoff
  samplerate

  Q
  bellGainDb
*/

class S2_Filter_Cytomic {

  private:

    uint32  MMode;
    float   MCutoff;
    float   MQ;
    float   MBellGainDB;
    float   MSampleRate;

  private:

    float ic1eq;
    float ic2eq;
    float g, k, A;
    float a1, a2, a3;
    float m0, m1, m2;
    float v1, v2, v3;

  //------------------------------
  //
  //------------------------------

  public:

    void sampleRate(float ARate)  { MSampleRate = ARate; }
    void mode(float AMode)        { MMode = AMode; }
    void cutoff(float ACutoff)    { MCutoff = ACutoff; }
    void q(float AQ)              { MQ = AQ; }
    void bellGain(float ADb)      { MBellGainDB = ADb; }

  public:

    S2_Filter_Cytomic() {
      MMode = 0;
      MCutoff = 0;
      MQ = 0;
      MBellGainDB = 0;
      MSampleRate = 0;
      clear();
    }

    void clear(void) {
      ic1eq = 0;
      ic2eq = 0;
    }

    //void calcCoeffs(uint32 mode, float cutoff/*, float samplerate*/) {
    void calcCoefficients(void) {
      switch (MMode) {
        case 0: // low
          g = tan(S2_PI * MCutoff/MSampleRate);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 0;
          m1 = 0;
          m2 = 1;
          break;
        case 1: // band
          g = tan(S2_PI * MCutoff/MSampleRate);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 0;
          m1 = 1;
          m2 = 0;
          break;
        case 2: // high
          g = tan(S2_PI * MCutoff/MSampleRate);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 1;
          m1 = -k;
          m2 = -1;
          break;
        case 3: // notch
          A = pow(10, MBellGainDB/40);
          g = tan(S2_PI * MCutoff/MSampleRate);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 1;
          m1 = -k;
          m2 = -2;
          break;
        case 4: // peak
          g = tan(S2_PI * MCutoff/MSampleRate);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 1;
          m1 = -k;
          m2 = 0;
          break;      //(9*, 8+, 17 total ops for any output)

        case 5: // bell
          A = pow(10, MBellGainDB/40);
          g = tan(S2_PI * MCutoff/MSampleRate);
          k = 1/(MQ*A);
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 1;
          m1 = k*(A*A - 1);
          m2 = 0;
          break;
        case 6: // low shelf
          A = pow(10, MBellGainDB/40);
          g = tan(S2_PI * MCutoff/MSampleRate) / sqrt(A);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = 1;
          m1 = k*(A - 1);
          m2 = (A*A - 1);
          break;
        case 7: // high shelf
          A = pow(10, MBellGainDB/40);
          g = tan(S2_PI * MCutoff/MSampleRate) * sqrt(A);
          k = 1/MQ;
          a1 = 1/(1 + g*(g + k));
          a2 = g*a1;
          a3 = g*a2;
          m0 = A*A;
          m1 = k*(1 - A)*A;
          m2 = (1 - A*A);
          break;
      }
    }

    //(9*, 8+, 17 total ops for any output)

    float process(float v0) {
      v3      = v0 - ic2eq;
      v1      = a1*ic1eq + a2*v3;
      v2      = ic2eq + a2*ic1eq + a3*v3;
      ic1eq   = 2*v1 - ic1eq;
      ic2eq   = 2*v2 - ic2eq;
      return m0*v0 + m1*v1 + m2*v2;
    }

};

//----------------------------------------------------------------------
#endif
