#ifndef s2_dsp_oscillator_included
#define s2_dsp_oscillator_included
//----------------------------------------------------------------------

#include "audio/dsp/s2_dsp_filter.h"
#include "audio/dsp/s2_dsp_phase.h"
#include "audio/dsp/s2_dsp_polyblep.h"
#include "audio/dsp/s2_dsp_tables.h"

#define SEMITONE      1.05946f
#define MAX_ADDITIVE  64

//----------------------------------------------------------------------

#define SIN(x) sin_.linear(x)

#define COS(x) cos_.linear(x)

#define CASE(a,b) case a: b(output, out_sync, samples); break;

//----------------------------------------------------------------------
// sync  >= 0.0  samples after reset
//       > -1.0  samples before reset
//       -2.0    no reset

#define INC_PHASE()                     \
  phase_ = phase;                       \
  phase += inc;                         \
  if (phase >= 1.0f) {                  \
    phase -= 1.0f;                      \
    out_sync[i] = phase / inc;          \
  } else if (phase > (1.0 - inc)) {     \
    out_sync[i] = -(1.0 - phase) / inc; \
  } else {                              \
    out_sync[i] = -2.0;                 \
  }


#define INC_PHASE_SYNC()                \
  phase_ = phase;                       \
  phase += inc;                         \
  if (input_sync[i] >= 0.0) {           \
    phase = input_sync[i] * inc;        \
    out_sync[i] = phase / inc;          \
  }  else if (phase >= 1.0f) {          \
    phase -= 1.0f;                      \
    out_sync[i] = phase / inc;          \
  } else if (phase > (1.0 - inc)) {     \
    out_sync[i] = -(1.0 - phase) / inc; \
  } else {                              \
    out_sync[i] = input_sync[i];        \
  }


//----------------------------------------------------------------------
// phase loop

#define PHASE_LOOP(calc)                                      \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE()                                               \
    calc                                                      \
    inc += inc_step;                                          \
  }

#define PHASE_LOOP_SYNC(calc)                                 \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE_SYNC()                                          \
    calc                                                      \
    inc += inc_step;                                          \
  }

#define PHASE_LOOP_PM(calc)                                   \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE()                                               \
    float phase = pmod(this->phase, i);                       \
    calc                                                      \
    inc += inc_step;                                          \
  }

#define PHASE_LOOP_PM_SYNC(calc)                              \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE_SYNC()                                          \
    float phase = pmod(this->phase, i);                       \
    calc                                                      \
    inc += inc_step;                                          \
  }

#define PHASE_LOOP_BOTH(calc)   \
  if (pm > 0.0f) {              \
    if (sync) {                 \
      PHASE_LOOP_PM_SYNC(calc)  \
    } else {                    \
      PHASE_LOOP_PM(calc)       \
    }                           \
  } else if (sync) {            \
    PHASE_LOOP_SYNC(calc)       \
  } else {                      \
    PHASE_LOOP(calc)            \
  }

//----------------------------------------------------------------------
// pwidth loop

#define PWIDTH_LOOP(calc)                                         \
  float inc = ff / sample_rate;                                   \
  float inc_step = (ft / sample_rate - inc) / (float)samples;     \
  float width = norm_width(wf, inc);                              \
  float w_step = (norm_width(wt, inc) - width) / (float)samples;  \
  for (int32 i = 0; i < samples; i++) {                           \
    INC_PHASE()                                                   \
    calc                                                          \
    width += w_step;                                              \
    inc += inc_step;                                              \
  }

#define PWIDTH_LOOP_PM(calc)                                      \
  float inc = ff / sample_rate;                                   \
  float inc_step = (ft / sample_rate - inc) / (float)samples;     \
  float width = norm_width(wf, inc);                              \
  float w_step = (norm_width(wt, inc) - width) / (float)samples;  \
  for (int32 i = 0; i < samples; i++) {                           \
    INC_PHASE()                                                   \
    float phase = pmod(this->phase, i);                           \
    calc                                                          \
    width += w_step;                                              \
    inc += inc_step;                                              \
  }

#define PWIDTH_LOOP_SYNC(calc)                                    \
  float inc = ff / sample_rate;                                   \
  float inc_step = (ft / sample_rate - inc) / (float)samples;     \
  float width = norm_width(wf, inc);                              \
  float w_step = (norm_width(wt, inc) - width) / (float)samples;  \
  for (int32 i = 0; i < samples; i++) {                           \
    INC_PHASE_SYNC()                                              \
    calc                                                          \
    width += w_step;                                              \
    inc += inc_step;                                              \
  }

#define PWIDTH_LOOP_PM_SYNC(calc)                                 \
  float inc = ff / sample_rate;                                   \
  float inc_step = (ft / sample_rate - inc) / (float)samples;     \
  float width = norm_width(wf, inc);                              \
  float w_step = (norm_width(wt, inc) - width) / (float)samples;  \
  for (int32 i = 0; i < samples; i++) {                           \
    INC_PHASE_SYNC()                                              \
    float phase = pmod(this->phase, i);                           \
    calc                                                          \
    width += w_step;                                              \
    inc += inc_step;                                              \
  }

#define PWIDTH_LOOP_BOTH(calc)  \
  if (pm > 0.0f) {              \
    if (sync) {                 \
      PWIDTH_LOOP_PM_SYNC(calc) \
    } else {                    \
      PWIDTH_LOOP_PM(calc)      \
    }                           \
   } else if (sync) {           \
    PWIDTH_LOOP_SYNC(calc)      \
  } else {                      \
    PWIDTH_LOOP(calc)           \
  }

//----------------------------------------------------------------------
// pmod loop

#define PMOD_LOOP(calc)                                       \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE()                                               \
    calc                                                      \
    mod += m_step;                                            \
    inc += inc_step;                                          \
  }

#define PMOD_LOOP_PM(calc)                                    \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE()                                               \
    float phase = pmod(this->phase, i);                       \
    calc                                                      \
    mod += m_step;                                            \
    inc += inc_step;                                          \
  }

#define PMOD_LOOP_SYNC(calc)                                  \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE_SYNC()                                          \
    calc                                                      \
    mod += m_step;                                            \
    inc += inc_step;                                          \
  }

#define PMOD_LOOP_PM_SYNC(calc)                               \
  float inc = ff / sample_rate;                               \
  float inc_step = (ft / sample_rate - inc) / (float)samples; \
  for (int32 i = 0; i < samples; i++) {                       \
    INC_PHASE_SYNC()                                          \
    float phase = pmod(this->phase, i);                       \
    calc                                                      \
    mod += m_step;                                            \
    inc += inc_step;                                          \
  }

#define PMOD_LOOP_BOTH(calc)  \
  if (pm > 0.0f) {            \
    if (sync) {               \
      PMOD_LOOP_PM_SYNC(calc) \
    } else {                  \
      PMOD_LOOP_PM(calc)      \
    }                         \
  } else if (sync) {          \
    PMOD_LOOP_SYNC(calc)      \
  } else {                    \
    PMOD_LOOP(calc)           \
  }

//----------------------------------------------------------------------

// phase distortion
float pd(float x, float w) {
  if (x < w) return 0.5f * x / w;
  else return 0.5f + 0.5f * (x - w) / (1.0f - w);
}

//----------

float norm_width(float w, float inc) {
  if (w < inc) return inc;
  else if (w > (1.0 - inc)) return 1.0 - inc;
  else return w;
}

//----------

// copied from lmms triple oscillator
float lmms_exp(float phase) {
  if (phase > 0.5f) return -1.0 + 8.0 * (1.0 - phase) * (1.0 - phase);
  else return -1.0 + 8.0 * phase * phase;
}

//----------

float double_saw(float phase, float width) {
  if (phase < width) return phase / width;
  else return (phase - width) / (1.0f - width);
}

//----------

float pulse_saw(float phase, float width) {
  if (phase < width) return phase / width;
  else return (width - phase) / (1.0f - width);
}

//----------

float bandlimit_fm678(float y, float phase, float inc) {
  if (phase < 0.25f) {} // do nothing;
  else if (phase < (0.25 + inc)) { y *= ((0.25 + inc) - phase) / inc; } // fade out
  else if (phase < (0.5 - inc)) { y = 0.0f; }
  else if (phase < 0.5) { y *= (phase - (0.5 - inc)) / inc; }      // fade in
  else if (phase < 0.75f) {} // do nothing
  else if (phase < (0.75f + inc)) { y *= ((0.75 + inc) - phase) / inc; } // fade out
  else { y = 0.0f; }
  return y;
}

//----------------------------------------------------------------------

/*
  Virtual Analog    - Vintage waveform models
  Phase Distortion  - Casio CZ sounds
  Electronic        - various
  FM                - OPL FM sounds
*/

class S2_Oscillator_Analogue {

  public:

    enum {
      VA_SAW,
      VA_TRI_SAW,
      VA_PULSE,
      PD_SAW,
      PD_SQUARE,
      PD_PULSE,
      PD_DOUBLE_SINE,
      PD_SAW_PULSE,
      PD_RES1,
      PD_RES2,
      PD_RES3,
      PD_HALF_SINE,
      EL_SAW,
      EL_DOUBLE_SAW,
      EL_TRI,
      EL_PULSE,
      EL_PULSE_SAW,
      EL_SLOPE,
      EL_ALPHA1,
      EL_ALPHA2,
      EL_EXP,
      FM1,
      FM2,
      FM3,
      FM4,
      FM5,
      FM6,
      FM7,
      FM8
    };

  private:

    float             ff, ft, sample_rate, phase, phase_;
    float             start, wf, wt;
    int               type;
    float*            input;
    float*            input_sync;
    float             pm;
    bool              sync;
    float             prev;;
    S2_AmSynthFilter  filter;

  //------------------------------
  //
  //------------------------------

  public:

    S2_Oscillator_Analogue() {
      ff      = 440.0f;
      ft      = 440.0f;
      phase   = 0.0f;
      phase_  = 0.0f;
      start   = 0.0f;
      wf      = 0.5f;
      wt      = 0.5f;
      type    = 0;
      pm      = 0.0f;
      sync    = false;
      prev    = 0.0f;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setType(int t) {
      type = t;
    }

    //----------

    void setFreq(float _ff, float _ft) {
      ff = _ff;
      ft = _ft;
    }

    //----------

    float pmod(float phase, int i) {
      float phase2 = phase + pm * input[i];
      if (phase2 < 0.0f) phase2 = 1.0f - fmod(fabs(phase2), 1.0f);
      else if (phase2 >= 1.0f) phase2 = fmod(phase2, 1.0f);
      return phase2;
    }

    //----------

    void setModulation(float* _input, float* _input_s, float _pm, bool _sync) {
      input = _input;
      input_sync = _input_s;
      pm = _pm;
      sync = _sync;
    }

    //----------

    void setStart(float _s) {
      start = _s;
    }

    //----------

    void setWidth(float _wf, float _wt) {
      wf = _wf;
      wt = _wt;
    }

    //----------

    void setSamplerate(float r) {
      sample_rate = r;
      filter.setSamplerate(r);
    }

    //----------

    void clear() {
      phase = start;
      phase_ = start;
      ff = ft = 440.0;
      type = 0;
      wf = wt = 0.5;
      filter.clear();
    }

    //----------

    void reset() {
      phase = start;
      phase_ = start;
      filter.clear();
    }

  //------------------------------
  // VA
  //------------------------------

  public:

    void va_saw(float* output, float* out_sync, int samples) {
      el_saw(output, out_sync, samples);
      filter.setType(4);
      filter.setCoefficients(50, 0);
      filter.process(output, output, samples);
      for (int32 i = 0; i < samples; i++) {
          output[i] = 0.8 * output[i];
      }
    }

    //----------

    void va_tri_saw(float* output, float* out_sync, int samples) {
      el_tri(output, out_sync, samples);
      filter.setType(4);
      filter.setCoefficients(50, 0);
      filter.process(output, output, samples);
      for (int32 i = 0; i < samples; i++) {
          output[i] = 0.8 * output[i];
      }
    }

    //----------

    void va_pulse(float* output, float* out_sync, int samples) {
      el_pulse(output, out_sync, samples);
      filter.setType(4);
      filter.setCoefficients(50, 0);
      filter.process(output, output, samples);
      for (int32 i = 0; i < samples; i++) {
          output[i] = 0.8 * output[i];
      }
    }

  //------------------------------
  // PD
  //------------------------------

  public:

    void pd_saw(float* output, float* out_sync, int samples) {
      float mod = 0.5f - wf * 0.5;
      float m_step = 0.5f * (wf - wt) / (float)samples;
      PMOD_LOOP_BOTH(
        output[i] = COS(pd(phase, mod));
      )
    }

    //----------

    void pd_square(float* output, float* out_sync, int samples) {
      float mod = 0.5f - wf * 0.5;
      float m_step = 0.5f * (wf - wt) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2;
        if (phase < mod) p2 = phase * 0.5f / mod;
        else if (phase < 0.5f) p2 = 0.5f;
        else if (phase < 0.5f + mod) p2 = (phase - 0.5f) * 0.5f / mod + 0.5f;
        else p2 = 1.0f;
        output[i] = COS(p2);
      )
    }

    //----------

    void pd_pulse(float* output, float* out_sync, int samples) {
      float mod = 1.0f - wf;
      float m_step = (wf - wt) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2 = phase < mod ? phase / mod : 1.0f;
        output[i] = COS(p2);
      )
    }

    //----------

    void pd_double_sine(float* output, float* out_sync, int samples) {
      float mod = 1.0f - wf;
      float m_step = (wf - wt) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2 = 0;
        if (phase < 0.5f) p2 = 2.0f * phase;
        else {
          p2 = 1.0f - (phase - 0.5f) / (0.5f * mod);
          if (p2 < 0) p2 = 0;
        }
        output[i] = COS(p2);
      )
    }

    //----------

    void pd_saw_pulse(float* output, float* out_sync, int samples) {
      float mod = 1.0f - wf;
      float m_step = (wf - wt) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2 = 0.0f;
        if (phase < 0.5f) p2 = phase;
        else {
          p2 = 0.5f - (phase - 0.5f) / mod;
          if (p2 < 0) p2 = 0;
        }
        output[i] = COS(p2);
      )
    }

    //----------

    void pd_res1(float* output, float* out_sync, int samples) {
      float mod = expf(wf * 6.0f * (float)M_LN2);
      float modt = expf(wt * 6.0f * (float)M_LN2);
      float m_step = (modt - mod) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2 = fmod(mod * phase, 1.0f);
        float window = 1.0f - phase;
        output[i] = 1.0f - window * (1.0 - COS(p2));
      )
    }

    //----------

    void pd_res2(float* output, float* out_sync, int samples) {
      float mod = expf(wf * 6.0f * (float)M_LN2);
      float modt = expf(wt * 6.0f * (float)M_LN2);
      float m_step = (modt - mod) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2 = fmod(mod * phase, 1.0f);
        float window = phase < 0.5f ? 2.0f * phase : 2.0f * (1.0f - phase);
        output[i] = 1.0f - window * (1.0 - COS(p2));
      )
    }

    //----------

    void pd_res3(float* output, float* out_sync, int samples) {
      float mod = expf(wf * 6.0f * (float)M_LN2);
      float modt = expf(wt * 6.0f * (float)M_LN2);
      float m_step = (modt - mod) / (float)samples;
      PMOD_LOOP_BOTH(
        float p2 = fmod(mod * phase, 1.0f);
        float window = phase < 0.5f ? 1.0f : 2.0f * (1.0f - phase);
        output[i] = 1.0f - window * (1.0 - COS(p2));
      )
    }

    //----------

    void pd_half_sine(float* output, float* out_sync, int samples) {
      float mod = 0.5f + wf * 0.5;
      float m_step = 0.5f * (wt - wf) / (float)samples;
      PMOD_LOOP_BOTH(
        output[i] = gb(SIN(0.5f * pd(phase, mod)));
      )
    }

  //------------------------------
  // EL
  //------------------------------

  public:

    void el_saw(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            output[i] = gb(phase);
          )
        } else {
          PHASE_LOOP_PM( output[i] = gb(phase); )
        }
      } else if (sync) {
        // bandlimited
        PHASE_LOOP_SYNC(
          float mod = saw_sync(phase, phase_, inc, input_sync[i]);
          output[i] = gb(phase - mod);
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          float mod = saw_polyblep(phase, inc);
          output[i] = gb(phase - mod);
        )
      }
    }

    //----------

    void el_double_saw(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PWIDTH_LOOP_PM_SYNC(
            output[i] = gb(double_saw(phase, width));
          )
        } else {
          PWIDTH_LOOP_PM(
            output[i] = gb(double_saw(phase, width));
          )
        }
      } else if (sync) {
        // bandlimited
        PWIDTH_LOOP_SYNC(
          float p2 = double_saw(phase, width);
          float s = input_sync[i];
          float mod = 0.0f;
          if (s >= 0.0f) { // sync start
            mod = double_saw(phase_, width) * polyblep(s);
          } else if (s > -1.0f) { // sync end
            mod = double_saw(phase, width) * polyblep(s);
          } else if (phase < inc) { // start
            mod = polyblep(phase / inc);
          } else if (phase > (1.0f - inc)) { // end
            mod = polyblep( (phase - 1.0f) / inc);
          } else if (phase < width && phase > (width - inc)) { // mid end
            mod = polyblep( (phase - width) / inc);
          } else if (phase > width && phase < (width + inc)) { // mid start
            mod = polyblep((phase - width) / inc);
          }
          output[i] = gb(p2 - mod);
        )
      } else {
        // bandlimited
        PWIDTH_LOOP(
          float p2 = double_saw(phase, width);
          float mod = 0.0f;
          if (phase < inc) { // start
            mod = polyblep(phase / inc);
          } else if (phase > (1.0f - inc)) { // end
            mod = polyblep( (phase - 1.0f) / inc);
          } else if (phase < width && phase > (width - inc)) { // mid end
            mod = polyblep( (phase - width) / inc);
          } else if (phase > width && phase < (width + inc)) { // mid start
            mod = polyblep((phase - width) / inc);
          }
          output[i] = gb(p2 - mod);
        )
      }
    }

    //----------

    void el_tri(float* output, float* out_sync, int samples) {
      if (pm > 0.0f) {
        if (sync) {
          PWIDTH_LOOP_PM_SYNC(
            output[i] = gb(gtri(phase, width));
          )
        } else {
          PWIDTH_LOOP_PM(
            output[i] = gb(gtri(phase, width));
          )
        }
      } else if (sync) {
        // bandlimited
        PWIDTH_LOOP_SYNC(
          float s = input_sync[i];
          float mod = 0.0f;
          if (s >= 0.0f) { // start
            mod = gtri(phase_ + inc - phase, width) * polyblep(s);
          } else if (s > -1.0f) { // end
            mod = gtri(phase + sync * inc, width) * polyblep(s);
          }
          output[i] = gb(gtri(phase, width) - mod);
        )
      } else {
        PWIDTH_LOOP(
          output[i] = gb(gtri(phase, width));
        )
      }
    }

    //----------

    void el_pulse(float* output, float* out_sync, int samples) {
      if (pm > 0.0f) {
        if (sync) {
          PWIDTH_LOOP_PM_SYNC(
            output[i] = phase < width ? -1.0f : 1.0f;
          )
        } else {
          PWIDTH_LOOP_PM(
            output[i] = phase < width ? -1.0f : 1.0f;
          )
        }
      } else if (sync) {
        // FIXME
        PWIDTH_LOOP_SYNC(
          float mod = pulse_sync(phase, phase_, width, inc, input_sync[i]);
          output[i] = (phase < width ? -1.0f : 1.0f) - 2.0f * mod;
        )
      } else {
        // bandlimited
        PWIDTH_LOOP(
          float mod = pulse_polyblep(phase, width, inc);
          output[i] = (phase < width ? -1.0f : 1.0f) - 2.0f * mod;
        )
      }
    }

    //----------

    void el_pulse_saw(float* output, float* out_sync, int samples) {
      if (pm > 0.0f) {
        if (sync) {
          PWIDTH_LOOP_PM_SYNC(
            output[i] = pulse_saw(phase, width);
          )
        } else {
          PWIDTH_LOOP_PM(
            output[i] = pulse_saw(phase, width);
          )
        }
      } else if (sync) {
        // bandlimited
        PWIDTH_LOOP_SYNC(
          float s = input_sync[i];
          float p2 = pulse_saw(phase, width);
          float mod = 0.0f;
          if (s >= 0.0f) { // sync start
            mod = pulse_saw(phase_, width) * polyblep(s);
          } else if (phase < inc) { // start
            mod = -polyblep(phase / inc);
          } else if (phase > (1.0f - inc)) { // end
            mod = -polyblep( (phase - 1.0f) / inc);
          } else if (phase < width && phase > (width - inc)) { // mid end
            mod = polyblep( (phase - width) / inc);
          } else if (phase > width && phase < (width + inc)) { // mid start
            mod = polyblep((phase - width) / inc);
          } else if (s > -1.0f) { // sync end
            mod = pulse_saw(phase, width) * polyblep(s);
          }
          output[i] = p2 - mod;
        )
      } else {
        // bandlimited
        PWIDTH_LOOP(
          float p2 = pulse_saw(phase, width);
          float mod = 0.0f;
          if (phase < inc) { // start
            mod = -polyblep(phase / inc);
          } else if (phase > (1.0f - inc)) { // end
            mod = -polyblep( (phase - 1.0f) / inc);
          } else if (phase < width && phase > (width - inc)) { // mid end
            mod = polyblep( (phase - width) / inc);
          } else if (phase > width && phase < (width + inc)) { // mid start
            mod = polyblep((phase - width) / inc);
          }
          output[i] = p2 - mod;
        )
      }
    }

    //----------

    void el_slope(float* output, float* out_sync, int samples) {
      if (pm > 0.0f) {
        if (sync) {
          PWIDTH_LOOP_PM_SYNC(
            output[i] = gb(gvslope(phase, width));
          )
        } else {
          PWIDTH_LOOP_PM(
            output[i] = gb(gvslope(phase, width));
          )
        }
      } else if (sync) {
        // bandlimited
        PWIDTH_LOOP_SYNC(
          float s = input_sync[i];
          float p2 = gvslope(phase, width);
          float mod = 0.0f;
          if (s >= 0.0f) { // sync start
            mod = gvslope(phase_, width) * polyblep(s);
          } else if (s > -1.0f) { // sync end
            mod = gvslope(phase, width) * polyblep(s);
          } else if (phase < inc) { // start
            mod = polyblep(phase / inc);
          } else if (phase > (1.0f - inc)) { // end
            mod = polyblep( (phase - 1.0f) / inc);
          } else if (phase < width && phase > (width - inc)) { // mid end
            mod = width * polyblep( (phase - width) / inc);
          } else if (phase > width && phase < (width + inc)) { // mid start
            mod = width * polyblep((phase - width) / inc);
          }
          output[i] = gb(p2 - mod);
        )
      } else {
        // bandlimited
        PWIDTH_LOOP(
          float p2 = gvslope(phase, width);
          float mod = 0.0f;
          if (phase < inc) { // start
            mod = polyblep(phase / inc);
          } else if (phase > (1.0f - inc)) { // end
            mod = polyblep( (phase - 1.0f) / inc);
          } else if (phase < width && phase > (width - inc)) { // mid end
            mod = width * polyblep( (phase - width) / inc);
          } else if (phase > width && phase < (width + inc)) { // mid start
            mod = width * polyblep((phase - width) / inc);
          }
          output[i] = gb(p2 - mod);
        )
      }
    }

    //----------

    void el_alpha1(float* output, float* out_sync, int samples) {
      // pulse
      float ff_old = ff;
      float ft_old = ft;
      float p = phase;
      float p_ = phase_;
      ff = 2.0f * ff;
      ft = 2.0f * ft;
      phase = fmod(2.0f * phase, 1.0);
      phase_ = fmod(2.0f * phase_, 1.0);
      el_pulse(output, out_sync, samples);
      // saw
      phase = p;
      phase_ = p_;
      ff = ff_old;
      ft = ft_old;
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            output[i] = phase * (output[i] + 1.0) - 1.0f;
          )
        } else {
          PHASE_LOOP_PM(
            output[i] = phase * (output[i] + 1.0) - 1.0f;
          )
        }
      } else if (sync) {
        // bandlimited
        PHASE_LOOP_SYNC(
          float mod = saw_sync(phase, phase_, inc, input_sync[i]);
          output[i] = (phase - mod) * (output[i] + 1.0) - 1.0f;
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          float mod = saw_polyblep(phase, inc);
          output[i] = (phase - mod) * (output[i] + 1.0) - 1.0f;
        )
      }
    }

    //----------

    void el_alpha2(float* output, float* out_sync, int samples) {
      // pulse
      float ff_old = ff;
      float ft_old = ft;
      float p = phase;
      float p_ = phase_;
      ff = 4.0f * ff;
      ft = 4.0f * ft;
      phase = fmod(4.0f * phase, 1.0);
      phase_ = fmod(4.0f * phase_, 1.0);
      el_pulse(output, out_sync, samples);
      // saw
      phase = p;
      phase_ = p_;
      ff = ff_old;
      ft = ft_old;
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            output[i] = phase * (output[i] + 1.0) - 1.0f;
          )
        } else {
          PHASE_LOOP_PM(
            output[i] = phase * (output[i] + 1.0) - 1.0f;
          )
        }
      } else if (sync) {
        // bandlimited
        PHASE_LOOP_SYNC(
          float mod = saw_sync(phase, phase_, inc, input_sync[i]);
          output[i] = (phase - mod) * (output[i] + 1.0) - 1.0f;
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          float mod = saw_polyblep(phase, inc);
          output[i] = (phase - mod) * (output[i] + 1.0) - 1.0f;
        )
      }
    }
    void el_exp(float* output, float* out_sync, int samples) {
      if (pm > 0.0f) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            output[i] = lmms_exp(phase);
          )
        } else {
          PHASE_LOOP_PM(
            output[i] = lmms_exp(phase);
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          // TODO
          output[i] = lmms_exp(phase);
        )
      } else {
        PHASE_LOOP(
          output[i] = lmms_exp(phase);
        )
      }
    }

  //------------------------------
  // FM
  //------------------------------

  public:

    void fm1(float* output, float* out_sync, int samples) {
      PHASE_LOOP_BOTH(
        output[i] = SIN(phase);
      )
    }

    //----------

    void fm2(float* output, float* out_sync, int samples) {
      PHASE_LOOP_BOTH(
        output[i] = SIN(0.5f * phase);
      )
    }

    //----------

    void fm3(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            float y = SIN(phase);
            output[i] = (phase > 0.25 && phase < 0.75) ? -y : y;
          )
        } else {
          PHASE_LOOP_PM(
            float y = SIN(phase);
            output[i] = (phase > 0.25 && phase < 0.75) ? -y : y;
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          float y = SIN(phase);
          output[i] = (phase > 0.25 && phase < 0.75) ? -y : y;
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          float y = SIN(phase);
          if (phase < (0.25 - inc)) {
            // do nothing
          } else if (phase < 0.25) {
            // fade out
            y *= (0.25 - phase) / inc;
          } else if (phase < (0.25 + inc)) {
            // fade in (inverted)
            y *= (0.25 - phase) / inc;
          } else if (phase < (0.75 - inc)) {
            y *= -1.0;
          } else if (phase < 0.75) {
            // fade out
            y *= (phase - 0.75) / inc;
          } else if (phase < (0.75 + inc)) {
            // fade in
            y *= (phase - 0.75) / inc;
          }
          output[i] = y;
        )
      }
    }

    //----------

    void fm4(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            output[i] = phase < 0.5f ? SIN(2.0f * phase) : 0.0f;
          )
        } else {
          PHASE_LOOP_PM(
            output[i] = phase < 0.5f ? SIN(2.0f * phase) : 0.0f;
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          output[i] = phase < 0.5f ? SIN(2.0f * phase) : 0.0f;
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          if (phase < 0.5) {
            output[i] = SIN(2.0f * phase);
          } else if (phase < (0.5 + inc)) {
            // fade out
            output[i] = SIN(2.0f * phase - 1.0f) * ((0.5 + inc) - phase) / inc;
          } else {
            output[i] = 0.0;
          }
        )
      }
    }

    //----------

    void fm5(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            output[i] = phase < 0.5f ? SIN(phase) : 0.0f;
          )
        } else {
          PHASE_LOOP_PM(
            output[i] = phase < 0.5f ? SIN(phase) : 0.0f;
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          output[i] = phase < 0.5f ? SIN(phase) : 0.0f;
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          if (phase < 0.5) {
            output[i] = SIN(phase);
          } else if (phase < (0.5 + inc)) {
            // fade out
            output[i] = SIN(phase) * ((0.5 + inc) - phase) / inc;
          } else {
            output[i] = 0.0;
          }
        )
      }
    }

    //----------

    void fm6(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            if (phase < 0.25f) {
               output[i] = SIN(2.0 * phase);
             } else if (phase > 0.5f && phase < 0.75f) {
               output[i] = SIN(2.0 * (phase - 0.25));
             } else {
               output[i] = 0.0f;
             }
          )
        } else {
          PHASE_LOOP_PM(
            if (phase < 0.25f) {
               output[i] = SIN(2.0 * phase);
             } else if (phase > 0.5f && phase < 0.75f) {
               output[i] = SIN(2.0 * (phase - 0.25));
             } else {
               output[i] = 0.0f;
             }
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          if (phase < 0.25f) {
             output[i] = SIN(2.0 * phase);
           } else if (phase > 0.5f && phase < 0.75f) {
             output[i] = SIN(2.0 * (phase - 0.25));
           } else {
             output[i] = 0.0f;
           }
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          float y = 0.0f;
          if (phase < 0.25f) {
            y = SIN(2.0 * phase);
          } else if (phase > 0.5f && phase < 0.75f) {
            y = SIN(2.0 * (phase - 0.25));
          }
          output[i] = bandlimit_fm678(y, phase, inc);
        )
      }
    }

    //----------

    void fm7(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            if (phase < 0.25 || (phase > 0.5 && phase < 0.75)) {
              output[i] = SIN(phase);
            } else {
              output[i] = 0.0f;
            }
          )
        } else {
          PHASE_LOOP_PM(
            if (phase < 0.25 || (phase > 0.5 && phase < 0.75)) {
              output[i] = SIN(phase);
            } else {
              output[i] = 0.0f;
            }
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          if (phase < 0.25 || (phase > 0.5 && phase < 0.75)) {
            output[i] = SIN(phase);
          } else {
            output[i] = 0.0f;
          }
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          const float y = SIN(phase);
          output[i] = bandlimit_fm678(y, phase, inc);
        )
      }
    }

    //----------

    void fm8(float* output, float* out_sync, int samples) {
      if (pm > 0.0) {
        if (sync) {
          PHASE_LOOP_PM_SYNC(
            if (phase < 0.25 || (phase > 0.5 && phase < 0.75)) {
              output[i] = SIN(fmod(phase, 0.25f));
            } else {
              output[i] = 0.0f;
            }
          )
        } else {
          PHASE_LOOP_PM(
            if (phase < 0.25 || (phase > 0.5 && phase < 0.75)) {
              output[i] = SIN(fmod(phase, 0.25f));
            } else {
              output[i] = 0.0f;
            }
          )
        }
      } else if (sync) {
        PHASE_LOOP_SYNC(
          if (phase < 0.25 || (phase > 0.5 && phase < 0.75)) {
            output[i] = SIN(fmod(phase, 0.25f));
          } else {
            output[i] = 0.0f;
          }
        )
      } else {
        // bandlimited
        PHASE_LOOP(
          const float y = fabs(SIN(fmod(phase, 0.5f)));
          output[i] = bandlimit_fm678(y, phase, inc);
        )
      }
    }

  //------------------------------
  // process
  //------------------------------

  public:

    void process(float* output, float* out_sync, int samples) {
      switch (type) {
        // va
        CASE(VA_SAW, va_saw)
        CASE(VA_TRI_SAW, va_tri_saw)
        CASE(VA_PULSE, va_pulse)
        // pd
        CASE(PD_SAW, pd_saw)
        CASE(PD_SQUARE, pd_square)
        CASE(PD_PULSE, pd_pulse)
        CASE(PD_DOUBLE_SINE, pd_double_sine)
        CASE(PD_SAW_PULSE, pd_saw_pulse)
        CASE(PD_RES1, pd_res1)
        CASE(PD_RES2, pd_res2)
        CASE(PD_RES3, pd_res3)
        CASE(PD_HALF_SINE, pd_half_sine)
        // el
        CASE(EL_SAW, el_saw)
        CASE(EL_DOUBLE_SAW, el_double_saw)
        CASE(EL_TRI, el_tri)
        CASE(EL_PULSE, el_pulse)
        CASE(EL_PULSE_SAW, el_pulse_saw)
        CASE(EL_SLOPE, el_slope)
        CASE(EL_ALPHA1, el_alpha1)
        CASE(EL_ALPHA2, el_alpha2)
        CASE(EL_EXP, el_exp)
        // fm
        CASE(FM1, fm1)
        CASE(FM2, fm2)
        CASE(FM3, fm3)
        CASE(FM4, fm4)
        CASE(FM5, fm5)
        CASE(FM6, fm6)
        CASE(FM7, fm7)
        CASE(FM8, fm8)
      }
    }
};

//----------------------------------------------------------------------

// TODO JP8000 tri mod

//----------------------------------------------------------------------

/*
  Additive Synthesis
*/

class S2_Oscillator_Additive {

  public:

    enum { SAW, SQUARE, TRIANGLE };

  private:

    float   ff, ft, sample_rate, phase, phase_;
    float   start, wf, wt;
    int     type;
    float*  input;
    float*  input_sync;
    float   pm;
    bool    sync;
    float   phases[MAX_ADDITIVE];

  //------------------------------
  //
  //------------------------------

  public:

    S2_Oscillator_Additive() {
      ff      = 440.0f;
      ft      = 440.0f;
      phase   = 0.0f;
      phase_  = 0.0f;
      start   = 0.0f;
      wf      = 0.5f;
      wt      = 0.5f;
      type    = 0;
      pm      = 0.0f;
      sync    = false;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setType(int t) {
      type = t;
    }

    //----------

    void setFreq(float _ff, float _ft) {
      ff = _ff;
      ft = _ft;
    }

    //----------

    float pmod(float phase, int i) {
      float phase2 = phase + pm * input[i];
      if (phase2 < 0.0f) phase2 = 1.0f - fmod(fabs(phase2), 1.0f);
      else if (phase2 >= 1.0f) phase2 = fmod(phase2, 1.0f);
      return phase2;
    }

    //----------

    void setModulation(float* _input, float* _input_s, float _pm, bool _sync) {
      input = _input;
      input_sync = _input_s;
      pm = _pm;
      sync = _sync;
    }

    //----------

    void setStart(float _s) {
      start = _s;
    }

    //----------

    void setWidth(float _wf, float _wt) {
      wf = _wf;
      wt = _wt;
    }

    //----------

    void setSamplerate(float r) {
      sample_rate = r;
    }

    //----------

    void clear() {
      phase = start;
      phase_ = start;
      ff = ft = 440.0;
      type = 0;
      wf = wt = 0.5;
      for (int32 i = 0; i < MAX_ADDITIVE; i++) {
        phases[i] = start * rand() / (RAND_MAX + 1.0f);
      }
    }

    //----------

    void reset() {
      phase = start;
      phase_ = start;
      for (uint32 i = 0; i < MAX_ADDITIVE; i++) {
        phases[i] = start * rand() / (RAND_MAX + 1.0f);
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    void saw(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      int max = S2_Min((float)MAX_ADDITIVE * wt, sample_rate / 2.0f / ff);
      for (int32 i = 0; i < samples; i++) output[i] = 0;
      if (pm > 0.0) {
        for (int32 j = 0; j < max; j++) {
          float inc2 = (j + 1) * inc;
          float scale = 1.0/ (j + 1);
          for (int32 i = 0; i < samples; i++) {
            phases[j] += inc2;
            if (phases[j] >= 1.0f) phases[j] -= 1.0f;
            output[i] += scale * SIN(pmod(phases[j], i));
          }
        }
      } else {
        for (int32 j = 0; j < max; j++) {
          float inc2 = (j + 1) * inc;
          float scale = 1.0/ (j + 1);
          for (int32 i = 0; i < samples; i++) {
            phases[j] += inc2;
            if (phases[j] >= 1.0f) phases[j] -= 1.0f;
            output[i] += scale * SIN(phases[j]);
          }
        }
      }
      // normalize
      for (int32 i = 0; i < samples; i++) {
        output[i] *= -2.0f/M_PI;
      }
    }

    //----------

    void square(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      int max = S2_Min(40.0f * wt, sample_rate / 2.0f / ff);
      for (int32 i = 0; i < samples; i++) output[i] = 0;
      if (pm > 0.0) {
        for (int32 j = 0; j < max; j += 2) {
          uint32 jj = j / 2;
          float inc2 = (j + 1) * inc;
          float scale = 1.0/ (j + 1);
          for (int32 i = 0; i < samples; i++) {
            phases[jj] += inc2;
            if (phases[jj] >= 1.0f) phases[jj] -= 1.0f;
            output[i] += scale * SIN(pmod(phases[jj], i));
          }
        }
      } else {
        for (int32 j = 0; j < max; j += 2) {
          uint32 jj = j / 2;
          float inc2 = (j + 1) * inc;
          float scale = 1.0/ (j + 1);
          for (int32 i = 0; i < samples; i++) {
            phases[jj] += inc2;
            if (phases[jj] >= 1.0f) phases[jj] -= 1.0f;
            output[i] += scale * SIN(phases[jj]);
          }
        }
      }
      // normalize
      for (int32 i = 0; i < samples; i++) {
        output[i] *= -4.0f/M_PI;
      }
    }

    //----------

    void triangle(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      int max = S2_Min(40.0f * wt, sample_rate / 2.0f / ff);
      for (int32 i = 0; i < samples; i++) output[i] = 0;
      if (pm > 0.0) {
        float inv = 1.0f;
        for (int32 j = 0; j < max; j += 2) {
          uint32 jj = j / 2;
          float inc2 = (j + 1) * inc;
          float scale = inv / ((j + 1) * (j + 1));
          for (int32 i = 0; i < samples; i++) {
            phases[jj] += inc2;
            if (phases[jj] >= 1.0f) phases[jj] -= 1.0f;
            output[i] += scale * SIN(pmod(phases[jj], i));
          }
          inv *= -1.0f;
        }
      } else {
        float inv = 1.0f;
          for (int32 j = 0; j < max; j += 2) {
          uint32 jj = j / 2;
          float inc2 = (j + 1) * inc;
          float scale = inv / ((j + 1) * (j + 1));
          for (int32 i = 0; i < samples; i++) {
            phases[jj] += inc2;
            if (phases[jj] >= 1.0f) phases[jj] -= 1.0f;
            output[i] += scale * SIN(phases[jj]);
          }
          inv *= -1.0f;
        }
      }
      // normalize
      for (int32 i = 0; i < samples; i++) {
        output[i] *= -8.0/(M_PI * M_PI);
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    void process(float* output, float* out_sync, int samples) {
      switch (type) {
        CASE(SAW, saw)
        CASE(SQUARE, square)
        CASE(TRIANGLE, triangle)
      }
    }
};

//----------------------------------------------------------------------

/*
  Aliasing Supersaw and Supersquare
  7 oscillators, width controls detune amount (max: +/-1 semitone)
  Uses ideas and values from How to Emulate the Supersaw, 2010 Adam Szabo
*/

class S2_Oscillator_SuperWave {

  public:

    enum { SAW, SQUARE, SAW2, SQUARE2 };

  private:

    float             ff, ft, sample_rate, phase, phase_;
    float             start, wf, wt;
    int               type;
    float*            input;
    float*            input_sync;
    float             pm;
    bool              sync;
    float             phases[7];
    S2_AmSynthFilter  filter;

  //------------------------------
  //
  //------------------------------

  public:

    S2_Oscillator_SuperWave() {
      // S2_Oscillator
      ff      = 440.0f;
      ft      = 440.0f;
      phase   = 0.0f;
      phase_  = 0.0f;
      start   = 0.0f;
      wf      = 0.5f;
      wt      = 0.5f;
      type    = 0;
      pm      = 0.0f;
      sync    = false;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setType(int t) {
      type = t;
    }

    //----------

    void setFreq(float _ff, float _ft) {
      ff = _ff;
      ft = _ft;
    }

    //----------

    float pmod(float phase, int i) {
      float phase2 = phase + pm * input[i];
      if (phase2 < 0.0f) phase2 = 1.0f - fmod(fabs(phase2), 1.0f);
      else if (phase2 >= 1.0f) phase2 = fmod(phase2, 1.0f);
      return phase2;
    }

    //----------

    void setModulation(float* _input, float* _input_s, float _pm, bool _sync) {
      input = _input;
      input_sync = _input_s;
      pm = _pm;
      sync = _sync;
    }

    //----------

    void setStart(float _s) {
      start = _s;
    }

    //----------

    void setWidth(float _wf, float _wt) {
      wf = _wf;
      wt = _wt;
    }

    //----------

    void setSamplerate(float r) {
      sample_rate = r;
      filter.setSamplerate(r);
      filter.setType(1);
    }

    //----------

    void clear() {
      phase = start;
      phase_ = start;
      ff = ft = 440.0;
      type = 0;
      wf = wt = 0.5;
      filter.clear();
      for (uint32 i = 0; i < 7; i++) {
        phases[i] = start * rand() / (RAND_MAX + 1.0f);
      }
    }

    //----------

    void reset() {
      phase = start;
      phase_ = start;
      filter.clear();
      for (uint32 i = 0; i < 7; i++) {
        phases[i] = start * rand() / (RAND_MAX + 1.0f);
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    void saw(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      float incs[7];
      float off = -0.2;
      float w = wf * wf;
      for (uint32 j = 0; j < 7; j++) {
        incs[j] = inc * powf(SEMITONE, w * off);
        off += 0.2/3.0;
      }
      for (int32 i = 0; i < samples; i++) {
        float out = 0;
        for (uint32 j = 0; j < 7; j++) {
          phases[j] += incs[j];
          if (phases[j] >= 1.0f) phases[j] -= 1.0f;
          out += gb(phases[j]);
        }
        output[i] = 0.8 * out;
      }
      filter.setCoefficients(ff, 0.3);
      filter.process(output, output, samples);
    }

    //----------

    void square(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      float incs[7];
      float off = -0.2;
      float w = wf * wf;
      for (uint32 j = 0; j < 7; j++) {
        incs[j] = inc * powf(SEMITONE, w * off);
        off += 0.2/3.0;
      }
      for (int32 i = 0; i < samples; i++) {
        float out = 0;
        for (uint32 j = 0; j < 7; j++) {
          phases[j] += incs[j];
          if (phases[j] >= 1.0f) phases[j] -= 1.0f;
          out += phases[j] < 0.5 ? -1.0 : 1.0;
        }
        output[i] = 0.8 * out;
      }
      filter.setCoefficients(ff, 0.3);
      filter.process(output, output, samples);
    }

    //----------

    void saw2(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      float incs[7];
      float off = -0.2;
      float w = wf * wf;
      for (uint32 j = 0; j < 7; j++) {
        incs[j] = inc * powf(SEMITONE, w * off);
        off += 0.2/3.0;
      }
      for (int32 i = 0; i < samples; i++) {
        float out = 0;
        for (uint32 j = 0; j < 7; j++) {
          phases[j] += incs[j];
          if (phases[j] >= 1.0f) phases[j] -= 1.0f;
          float mod = saw_polyblep(phases[j], incs[j]);
          out += gb(phases[j] - mod);
        }
        output[i] = 0.2 * out;
      }
    }

    //----------

    void square2(float* output, float* sync, int samples) {
      // TODO interpolation between ff - ft
      float inc = ff / sample_rate;
      float incs[7];
      float off = -0.2;
      float w = wf * wf;
      for (uint32 j = 0; j < 7; j++) {
        incs[j] = inc * powf(SEMITONE, w * off);
        off += 0.2/3.0;
      }
      for (int32 i = 0; i < samples; i++) {
        float out = 0;
        for (uint32 j = 0; j < 7; j++) {
          phases[j] += incs[j];
          if (phases[j] >= 1.0f) phases[j] -= 1.0f;
          float mod = pulse_polyblep(phases[j], 0.5, incs[j]);
          out += (phases[j] < 0.5 ? -1.0 : 1.0) - (2.0 * mod);
        }
        output[i] = 0.2 *  out;
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    void process(float* output, float* out_sync, int samples) {
      switch (type) {
        CASE(SAW, saw)
        CASE(SQUARE, square)
        CASE(SAW2, saw2)
        CASE(SQUARE2, square2)
      }
    }
};


//----------------------------------------------------------------------

/*
  Noise
*/

class S2_Oscillator_Noise {

  public:

    enum { WHITE, PINK, LP, BP };

  private:

    int                     type;
    float                   ff;//, ft;
    float                   wf;//, wt;
    S2_StateVariableFilter2 filter;
    float                   b0, b1, b2;

  //------------------------------
  //
  //------------------------------

  public:

    S2_Oscillator_Noise() {
      type    = 0;
      ff      = 440.0f;
      //ft      = 440.0f;
      wf      = 0.5f;
      //wt      = 0.5f;
      b0      = 0.0f;
      b1      = 0.0f;
      b2      = 0.0f;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setType(int t) {
      type = t;
    }

    //----------

    void setFreq(float _ff, float _ft) {
      ff = _ff;
      //ft = _ft;
    }

    //----------

    void setWidth(float _wf, float _wt) {
      wf = _wf;
      //wt = _wt;
    }

    //----------

    void clear() {
      type = 0;
      //ff = ft = 440.0;
      //wf = wt = 0.5;
      ff = 440.0;
      wf = 0.5;
    }

    //----------

    //void reset() {
    //}

    //----------

    void setSamplerate(float r) {
      filter.setSamplerate(r);
    }

  //------------------------------
  //
  //------------------------------

  public:

    void process(float* output, float* out_sync, int32 samples) {
      for (int32 i = 0; i < samples; i++) {
        output[i] =  (2.0f * rand() / (RAND_MAX + 1.0f) - 1.0f);
        out_sync[i] = -2.0;
      }
      if (type == PINK) {
        // Paul Kellet's pink noise
        // http://musicdsp.org/files/pink.txt
        for (int32 i = 0; i < samples; i++) {
          float white = output[i];
          b0 = 0.99765 * b0 + white * 0.0990460;
          b1 = 0.96300 * b1 + white * 0.2965164;
          b2 = 0.57000 * b2 + white * 1.0526913;
          output[i] = b0 + b1 + b2 + white * 0.1848;
        }
      } else if (type == LP) {
        filter.setType(0);
        filter.setCoefficients(ff, wf);
        filter.process(output, output, samples);
      } else if (type == BP) {
        filter.setType(2);
        filter.setCoefficients(ff, wf);
        filter.process(output, output, samples);
      }
    }

};

//----------------------------------------------------------------------

#undef  SEMITONE
#undef  MAX_ADDITIVE

//----------------------------------------------------------------------
#endif
