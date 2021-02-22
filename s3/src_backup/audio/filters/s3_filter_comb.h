#ifndef s2_filter_comb_included
#define s2_filter_comb_included
//----------------------------------------------------------------------

// https://github.com/rjeschke/cetonesynths/blob/master/cetoneorg/reverb.h

//----------

/*
    20hz @ 192k, srate / hz
    192k/20 = 9.6k
*/

#define MAX_BUFFER (1024*16) // 16k

//----------

class S2_Filter_Comb {

  private:

    float _Buffer[MAX_BUFFER]; // was 8192
    int32 _Pos;
    float _History;
    int32 _Size;
    float _Damp1;
    float _Damp2;
    float _Feedback;

  //------------------------------
  //
  //------------------------------

  public:

    S2_Filter_Comb() {
      //_Size;
      //_Damp1;
      //_Damp2;
      //_Feedback;
      reset();
    }

    //----------

    ~S2_Filter_Comb() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void reset(void) {
      _History = 0.0f;
      S2_Memset(_Buffer, 0, MAX_BUFFER * sizeof(float));
    }

    //----------

    void size(int32 ASize) {
      _Size = ASize;
      _Pos %= _Size;
      //_Pos = 0;
      //reset();
    }

    //----------

    void feedback(float AValue) {
      _Feedback = AValue;
    }

    //----------

    void damp(float AValue) {
      _Damp1 = AValue;
      _Damp2 = 1.0f - AValue;
    }

  //------------------------------
  //
  //------------------------------

  public:

    float process(float input) {
      //S2_DTrace("%i ",_Size);
      float output = _Buffer[_Pos];
      _History = (output * _Damp2) + (_History * _Damp1);
      //S2_KillDenorm(_History);
      _Buffer[_Pos] = input + (_History * _Feedback);
      //S2_KillDenorm(_Buffer[_Pos]);
      _Pos = (_Pos + 1) % _Size;
      return output;
    }

};

#undef MAX_BUFFER

//----------------------------------------------------------------------
#endif
