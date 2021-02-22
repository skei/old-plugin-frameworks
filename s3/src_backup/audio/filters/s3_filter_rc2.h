#ifndef s2_filter_rc2_included
#define s2_filter_rc2_included
//----------------------------------------------------------------------

// http://musicdsp.org/showArchiveComment.php?ArchiveID=185


class S2_Filter_RC2 {

  private:
    float v0,v1;
    float c,r;

  public:

    S2_Filter_RC2() {
      v0 = 0;
      v1 = 0;
    }

    //----------

    ~S2_Filter_RC2() {
    }

    //----------

     // 0..1

    void setCutoff(float ACutoff) {
      float cutoff = ACutoff * 127;
      c = pow(0.5, (128-cutoff) / 16.0);
    }

    //----------

     // 0..4

    void setResonance(float AResonance) {
      float resonance = AResonance * 127;
      r = pow(0.5, (resonance+24) / 16.0);
    }

    //----------

    void setFreq(float AFreq, float ARate) {
      c = 2 * sin( S2_PI * AFreq / ARate );
      //You can approximate this (tuning error towards nyquist):
      //c := KODE_PI2*freq/samplerate;
    }

    //----------

    void setQ(float AQ) {
      r = 1 / AQ;
    }

    //----------

    float process(float AInput) {
      /*
      v0 := (1-r*c)*v0 - (c)*v1 + (c)*AInput;
      v1 := (1-r*c)*v1 + (c)*v0;
      result := v1;
      */
      // This filter has stability issues for high r values. State variable filter
      // stability limits seem to work fine here. It can also be oversampled for
      // better stability and wider frequency range (use 0.5*original frequency):
      v0 = (1-r*c)*v0 - c*v1 + c*AInput;
      v1 = (1-r*c)*v1 + c*v0;
      float tmp = v1;
      v0 = (1-r*c)*v0 - c*v1 + c*AInput;
      v1 = (1-r*c)*v1 + c*v0;
      return (tmp+v1)*0.5;
    }

};


//----------------------------------------------------------------------
#endif
