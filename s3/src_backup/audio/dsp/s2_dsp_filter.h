#ifndef s2_dsp_filter_included
#define s2_dsp_filter_included
//----------------------------------------------------------------------

#include "core/utils/s2_math.h"
#include "audio/dsp/s2_dsp_delay.h"

//#include <algorithm>
//#include <math.h>
//#include "tables.h"
//#include "types.h"

//#define TANH(x) tanh_.linear(x)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  DC offset correction filter
*/

class S2_DCBlocker {
  private:
    float R, x1, y1;
  public:
    S2_DCBlocker() {
      x1 = 0.0f;
      y1 = 0.0f;
    }
    void setSamplerate(float r) {
      R = 1.0f - (S2_PI2 * 20.0f / r);
    }
    void clear(void) {
      x1 = y1 = 0.0f;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i=0; i<samples; i++) {
        // y(n) = x(n) - x(n - 1) + R * y(n - 1)
        float y = input[i] - x1 + R * y1;
        x1 = input[i];
        y1 = y;
        output[i] = y;
      }
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  one-pole filter class.
  This class implements a one-pole digital filter. A method is
  provided for setting the pole position along the real axis of the
  z-plane while maintaining a constant peak filter gain.
*/

class S2_OnePole {
  private:
    float b0_, a1_, last_;
  public:
    void clear() {
      last_ = 0.0;
    }
    float getLast() {
      return last_;
    }
    void setCoefficients(float b0, float a1) {
      b0_ = b0;
      a1_ = a1;
    }
    void setPole(double p) {
      b0_ = (p > 0.0) ? (1.0 - p) : (1.0 + p);
      a1_ = -p;
    }
    void setLowpass(double fc) {
      a1_ = exp(-2.0 * M_PI * fc);
      b0_ = 1.0 - a1_;
    }
    void setHighpass(double fc) {
      a1_ = -exp(-2.0 * M_PI * (0.5 - fc));
      b0_ = 1.0 + a1_;
    }
    float process(float input) {
      last_ = b0_ * input - a1_ * last_;
      return last_;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        last_ = b0_ * input[i] - a1_ * last_;
        output[i] = last_;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  one-zero filter class.
  This class implements a one-zero digital filter. A method is
  provided for setting the zero position along the real axis of the
  z-plane while maintaining a constant filter gain.
*/

class S2_OneZero {
  protected:
    float b0_, b1_, last_, prevIn_;
  public:
    void clear(void) {
      last_ = prevIn_ = 0.0;
    }
    void setCoefficients(float b0, float b1) {
      b0_ = b0;
      b1_ = b1;
    }
    void setZero(float z) {
      b0_ = (z > 0.0) ? (1.0 + z) : (1.0 - z);
      b1_ = -z * b0_;
    }
    float process(float input) {
      last_ = b0_ * input + b1_ * prevIn_;
      prevIn_ = input;
      return last_;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        last_ = b0_ * input[i] + b1_ * prevIn_;
        prevIn_ = input[i];
        output[i] = last_;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  one-pole, one-zero filter class.

  This class implements a one-pole, one-zero digital filter. A
  method is provided for creating an allpass filter with a given
  coefficient. Another method is provided to create a DC blocking
  filter.
*/

class S2_PoleZero {
  private:
    float a0_, a1_, b0_, b1_, last_, prevIn_;
  public:
    void clear(void) {
      last_ = prevIn_ = 0.0;
    }
    void setCoefficients(float b0, float b1, float a1) {
      b0_ = b0;
      b1_ = b1;
      a1_ = a1;
    }
    void setAllpass(float coefficient) {
      b0_ = coefficient;
      b1_ = 1.0;
      a0_ = 1.0; // just in case
      a1_ = coefficient;
    }
    void setBlockZero(float thePole = 0.99) {
      b0_ = 1.0;
      b1_ = -1.0;
      a0_ = 1.0; // just in case
      a1_ = -thePole;
    }
    float process(float input) {
      last_ = b0_ * input + b1_ * prevIn_ - a1_ * last_;
      prevIn_ = input;
      return last_;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        last_ = b0_ * input[i] + b1_ * prevIn_ - a1_ * last_;
        prevIn_ = input[i];
        output[i] = last_;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  two-pole filter class.

  This class implements a two-pole digital filter. A method is
  provided for creating a resonance in the frequency response while
  maintaining a nearly constant filter gain.
*/

class S2_TwoPole {
  private:
    float b0_, a1_, a2_, last_, last__;
  public:
    void clear(void) {
      last_ = last__ = 0.0;
    }
    void setCoefficients(float b0, float a1, float a2) {
      b0_ = b0;
      a1_ = a1;
      a2_ = a2;
    }
    float process(float input) {
      float temp = last_;
      last_ = b0_ * input - a1_ * last_ - a2_ * last__;
      last__ = temp;
      return last_;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        float temp = last_;
        last_ = b0_ * input[i] - a1_ * last_ - a2_ * last__;
        last__ = temp;
        output[i] = last_;
      }
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  two-zero filter class.

  This class implements a two-zero digital filter. A method is
  provided for creating a "notch" in the frequency response while
  maintaining a constant filter gain.
*/

class S2_TwoZero {
  private:
    float b0_, b1_, b2_, last_, prevIn_, prevIn__;
  public:
    void clear(void) {
      last_ = prevIn_ = prevIn__ = 0.0;
    }
    void setCoefficients(float b0, float b1, float b2) {
      b0_ = b0;
      b1_ = b1;
      b2_ = b2;
    }
    float process(float input) {
      last_ = b0_ * input + b1_ * prevIn_ + b2_ * prevIn__;
      prevIn__ = prevIn_;
      prevIn_ = input;
      return last_;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        last_ = b0_ * input[i] + b1_ * prevIn_ + b2_ * prevIn__;
        prevIn__ = prevIn_;
        prevIn_ = input[i];
        output[i] = last_;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  biquad (two-pole, two-zero) filter class.
  This class implements a two-pole, two-zero digital filter.
  Methods are provided for creating a resonance or notch in the
  frequency response while maintaining a constant filter gain.
*/

class S2_BiQuad {
  private:
    float b0_, b1_, b2_, a1_, a2_, last_, prevIn_, prevIn__, last__;
  public:
    void clear(void) {
      last_ = last__ = prevIn_ = prevIn__ = 0.0;
    }
    void setCoefficients(float b0, float b1, float b2, float a1, float a2) {
      b0_ = b0;
      b1_ = b1;
      b2_ = b2;
      a1_ = a1;
      a2_ = a2;
    }
    float process(float input) {
      float temp = last_;
      last_ = b0_ * input + b1_ * prevIn_ + b2_ * prevIn__;
      last_ -= a1_ * temp + a2_ * last__;
      prevIn__ = prevIn_;
      prevIn_ = input;
      last__ = temp;
      return last_;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        float temp = last_;
        last_ = b0_ * input[i] + b1_ * prevIn_ + b2_ * prevIn__;
        last_ -= a1_ * temp + a2_ * last__;
        prevIn__ = prevIn_;
        prevIn_ = input[i];
        last__ = temp;
        output[i] = last_;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S2_BiQuadDF2 {
  private:
    float b0_, b1_, b2_, a1_, a2_, z1_, z2_;
  public:
    void clear(void) {
      z1_ = z2_ = 0.0;
    }
    void setCoefficients(float b0, float b1, float b2, float a1, float a2) {
      b0_ = b0;
      b1_ = b1;
      b2_ = b2;
      a1_ = a1;
      a2_ = a2;
    }
    void setLowpass(float fc, float res) {
      float r = S2_Max(0.001, 2.0 * (1.0 - res)); //std::max(0.001, 2.0 * (1.0 - res));
      float k = tan(fc * M_PI);
      float k2 = k * k;
      float rk = r * k;
      float bh = 1.0 + rk + k2;
      float b0 = k2 / bh;
      float b1 = b0 * 2.0;
      float b2 = b0;
      float a1 = (2.0 * (k2 - 1.0)) / bh;
      float a2 = (1.0 - rk + k2) / bh;
      setCoefficients(b0, b1, b2, a1, a2);
    }
    void setHighpass(float fc, float res) {
      float r = S2_Max(0.001, 2.0 * (1.0 - res)); //std::max(0.001, 2.0 * (1.0 - res));
      float k = tan(fc * M_PI);
      float k2 = k * k;
      float rk = r * k;
      float bh = 1.0 + rk + k2;

      float b0 = 1.0 / bh;
      float b1 = -2.0 / bh;
      float b2 = b0;
      float a1 = (2.0 * (k2 - 1.0)) / bh;
      float a2 = (1.0 - rk + k2) / bh;
      setCoefficients(b0, b1, b2, a1, a2);
    }
    float process(float input) {
      float y  = (b0_ * input) + z1_;
      z1_ = z2_ + (b1_ * input) - (a1_ * y);
      z2_ = (b2_ * input) - (a2_ * y);
      return y;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
          float x = input[i];
          float y  = (b0_ * x) + z1_;
          z1_ = z2_ + (b1_ * x) - (a1_ * y);
          z2_ = (b2_ * x) - (a2_ * y);
          output[i] = y;
      }
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  AmSynth filter
  Copyright (c) 2001-2012 Nick Dowell
*/

class S2_AmSynthFilter {
    enum { LP24, HP24, BP24, LP12, HP12, BP12 };
  private:
    double d1, d2, d3, d4;
    float freq_, res_, sample_rate_;
    int type_;// = 0;
  public:
    S2_AmSynthFilter() {
      type_ = 0;
    }
    void clear(void) {
      d1 = 0.0;
      d2 = 0.0;
      d3 = 0.0;
      d4 = 0.0;
    }
    void setType(int t) {
      type_ = t;
    }
    void setSamplerate(float r) {
      sample_rate_ = r;
    }
    void setCoefficients(float f, float r) {
      //freq_ = std::min(f, float(sample_rate_ / 2.0) * 0.99f); // filter is unstable at PI
      //freq_ = std::max(freq_, 10.0f);
      freq_ = S2_Min(f, float(sample_rate_ / 2.0) * 0.99f); // filter is unstable at PI
      freq_ = S2_Max(freq_, 10.0f);
      res_ = r;
    }
    void process(float* input, float* output, int samples) {
      const double w = (freq_ / sample_rate_); // cutoff freq [ 0 <= w <= 0.5 ]
      //const double r = std::max(0.001, 2.0 * (1.0 - res_)); // r is 1/Q (sqrt(2) for a butterworth response)
      const double r = S2_Max(0.001, 2.0 * (1.0 - res_)); // r is 1/Q (sqrt(2) for a butterworth response)
      const double k = tan(w * M_PI);
      const double k2 = k * k;
      const double rk = r * k;
      const double bh = 1.0 + rk + k2;
      double a0 = 0;
      double a1 = 0;
      double a2 = 0;
      double b1 = 0;
      double b2 = 0;
      switch (type_ % 3) {
        case 0: // low
          a0 = k2 / bh;
          a1 = a0 * 2.0;
          a2 = a0;
          b1 = (2.0 * (k2 - 1.0)) / bh;
          b2 = (1.0 - rk + k2) / bh;
          break;
        case 1: // high
          a0 =  1.0 / bh;
          a1 = -2.0 / bh;
          a2 =  a0;
          b1 = (2.0 * (k2 - 1.0)) / bh;
          b2 = (1.0 - rk + k2) / bh;
          break;
        case 2: // band
          a0 =  rk / bh;
          a1 =  0.0;
          a2 = -rk / bh;
          b1 = (2.0 * (k2 - 1.0)) / bh;
          b2 = (1.0 - rk + k2) / bh;
          break;
      }
      if (type_ < 3) {
        for (int32 i = 0; i < samples; i++) {
          float y, x = input[i];
          y  =      (a0 * x) + d1;
          d1 = d2 + (a1 * x) - (b1 * y);
          d2 =      (a2 * x) - (b2 * y);
          x = y;
          y  =      (a0 * x) + d3;
          d3 = d4 + (a1 * x) - (b1 * y);
          d4 =      (a2 * x) - (b2 * y);
          output[i] = y;
        }
      } else {
        for (int32 i = 0; i < samples; i++) {
          float y, x = input[i];
          y  =      (a0 * x) + d1;
          d1 = d2 + (a1 * x) - (b1 * y);
          d2 =      (a2 * x) - (b2 * y);
          output[i] = y;
        }
      }
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  Moog 24 dB/oct resonant lowpass VCF
  References: CSound source code, Stilson/Smith CCRMA paper.
  Modified by paul.kellett@maxim.abel.co.uk July 2000
*/

class S2_MoogFilter {
  private:
    float f, pc, q;
    float bf0, bf1, bf2, bf3, bf4;
    float t1, t2;
    int   type;// = 0;
    float sample_rate;
  public:
    S2_MoogFilter() {
      type = 0;
    }
    void clear(void) {
      f = 0; pc = 0; q = 0;
      bf0 = 0; bf1 = 0; bf2 = 0; bf3 = 0; bf4 = 0;
      t1 = 0; t2 = 0;
    }
    void setType(int t) {
      type = t;
    }
    void setSamplerate(float r) {
      sample_rate = r;
    }
    void setCoefficients(float f_, float r_) {
      float frequency = f_ / (0.5 * sample_rate);
      float resonance = r_;
      if (frequency < 0) frequency = 0;
      if (frequency > 0.6) frequency = 0.6;
      q = 1.0f - frequency;
      pc = frequency + 0.8f * frequency * q;
      f = pc + pc - 1.0f;
      q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
    }
    void process(float* input, float* output, int samples) {
      for (int32 i = 0; i < samples; i++) {
        float in = input[i];
        in -= q * bf4; //feedback
        t1 = bf1;  bf1 = (in + bf0) * pc - bf1 * f;
        t2 = bf2;  bf2 = (bf1 + t1) * pc - bf2 * f;
        t1 = bf3;  bf3 = (bf2 + t2) * pc - bf3 * f;
        bf4 = (bf3 + t1) * pc - bf4 * f;
        bf4 = bf4 - bf4 * bf4 * bf4 * 0.166667f;    //clipping
        bf0 = in;
        // Lowpass  output:  bf4
        // Highpass output:  in - bf4;
        // Bandpass output:  3.0f * (bf3 - bf4);
        output[i] = bf4;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  Andrew Simper's State Variable Filter
  based on http://www.musicdsp.org/showone.php?id=92
*/

class S2_StateVariableFilter {
    enum {LP, HP, BP, NOTCH};
  private:
    float* out;
    float notch, low, high, band;
    float freq, damp;
    float drive; // internal distortion
    float sample_rate;
  public:
    void clear(void) {
      drive = 0.0;
      notch = low = high = band = 0.0;
      out = &low;
    }
    void setType(int type) {
      switch(type) {
        case LP:    out = &low;   break;
        case HP:    out = &high;  break;
        case BP:    out = &band;  break;
        case NOTCH: out = &notch; break;
      }
    }
    void setSamplerate(float rate) {
      sample_rate = rate;
    }
    void setCoefficients(float fc, float res) {
      //freq = 2.0 * sinf(M_PI * std::min(0.25, fc / (sample_rate * 2.0)));
      //damp = std::min(2.0*(1.0 - pow(res, 0.25)), std::min(2.0, 2.0 / freq - freq * 0.5));
      freq = 2.0 * sinf(M_PI * S2_Min(0.25, fc / (sample_rate * 2.0)));
      damp = S2_Min(2.0*(1.0 - pow(res, 0.25)), S2_Min(2.0, 2.0 / freq - freq * 0.5));
    }
    void setDistortion(float d) {
      drive = d;
    }
    void process(float* input, float* output, int samples) {
      for (int32 i = 0; i < samples; i++) {
        // TODO figure out better upsampling method
        const float in = input[i];
        notch = in - damp * band;
        low   = low + freq * band;
        high  = notch - low;
        band  = freq * high + band - drive * band * band * band;
        output[i] = 0.5 * *out;
        notch = in - damp * band;
        low   = low + freq * band;
        high  = notch - low;
        band  = freq * high + band - drive * band * band * band;
        output[i] += 0.5 * *out;
      }
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  Andrew Simper's State Variable Filter (improved version)

  based on Linear Trapezoidal Integrated State Variable Filter With Low Noise Optimisation
  (2011)
*/

class S2_StateVariableFilter2 {
    enum {LP, HP, BP, NOTCH};
  private:
    float sample_rate;
    int type;// = 0;
    float v0z, v1, v2;
    float k, g1, g2, g3, g4;
  public:
    S2_StateVariableFilter2() {
      type = 0;
    }
    void clear() {
      v0z = v1 = v2 = 0.0f;
    }
    void setType(int t) {
      type = t;
    }
    void setSamplerate(float rate) {
      sample_rate = rate;
    }
    void setCoefficients(float fc, float res) {
      float g = tan(M_PI * fc / sample_rate);
      //float damping = 1.0f / res;
      //k = damping;
      k = 1.0 - 0.99 * res;
      float ginv = g / (1.0f + g * (g + k));
      g1 = ginv;
      g2 = 2.0f * (g + k) * ginv;
      g3 = g * ginv;
      g4 = 2.0f * ginv;
    }
    #define SVF2_LOOP(x)                    \
      for (int32 i = 0; i < samples; i++) { \
        float v0 = input[i];                \
        float v1z = v1;                     \
        float v2z = v2;                     \
        float v3 = v0 + v0z - 2.0 * v2z;    \
        v1 += g1 * v3 - g2 * v1z;           \
        v2 += g3 * v3 + g4 * v1z;           \
        v0z = v0;                           \
        output[i] = x;                      \
      }

    void process(float* input, float* output, int32 samples) {
      switch (type) {
      case LP:    SVF2_LOOP(v2);                break;
      case BP:    SVF2_LOOP(v1);                break;
      case HP:    SVF2_LOOP(v0 - k * v1 - v2);  break;
      case NOTCH: SVF2_LOOP(v0 - k * v1);       break;
      }
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  Comb filter
*/

class S2_CombFilter {
  private:
    float     sample_rate, fc, amount;
    S2_DelayA delay;
  public:
    void clear(void) {
      delay.clear();
    }
    void setSamplerate(float rate) {
      sample_rate = rate;
    }
    void setCoefficients(float _fc, float _amount) {
      delay.setDelay((1.0 / _fc) * sample_rate);
      fc = _fc;
      amount = _amount;
    }
    void process(float* input, float* output, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        output[i] = input[i] + amount * delay.process(input[i]);
      }
    }

};

//----------------------------------------------------------------------
#endif

