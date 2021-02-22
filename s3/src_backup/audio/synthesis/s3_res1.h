#ifndef s2_res1_included
#define s2_res1_included
//----------------------------------------------------------------------

#include "audio/synthesis/s2_waveguide.h"
//#include "audio/waveforms/s2_waveform_polyblep.h"
#include "audio/filters/s2_filter_dc.h"
#include "audio/filters/s2_filter_rc.h"

#include "common/s2_interpolation.h"

//----------

// resonator type

#define s2_rt_off        0
#define s2_rt_resonator  1
#define s2_rt_pluck      2

//----------

#define CLAMP(x,a,b) x = S2_Clamp(x,a,b)

//----------------------------------------------------------------------


class S2_Res1 {

  private:

    S2_Filter_RC  nshape;
    S2_Filter_DC  dc;
    S2_WaveGuide  wg1;
    //S2_WaveGuide  wg2;
    float         MSampleRate;
    float         MInvRate;

    float w1,w2;

    float         _hz1;
    uint32        _os;
    uint32        _type;      // s2_rt_off, s2_rt_resonator, s2_rt_pluck
    float         _noise;     // 0..1
    float         _nshape;    // 0..1
    float         _fb;        // 0..1
    float         _damp;      // 0..1
    float         _tune;      // 0.01..2
    float         _mix;       // 0..1

    float         _tune_oct;
    float         _tune_semi;
    float         _tune_fine;

  public:

    S2_Res1() {
      _hz1    = 0.0f;
      _os     = 1;
      _type   = s2_rt_off;
      _noise  = 0.0f;
      _nshape = 0.0f;
      _fb     = 0.0f;
      _damp   = 0.0f;
      _tune   = 0.0f; // 1.0f
      _mix    = 0.0f;
    }

  public:


  public:

    void reset(void) {
    }

    void noteOn(void) {
      wg1.clear();
      //wg2.clear();
    }

    void setSampleRate(float srate) {
      MSampleRate = srate;
      MInvRate = 1.0f / MSampleRate;
      nshape.sampleRate(srate);
      wg1.sampleRate(srate);
      //wg2.sampleRate(srate);
    }

    void setFreq(float hz)        { _hz1    = hz; }
    void setOversample(float os)  { _os     = os; }
    void setType(uint32 type)     { _type   = type; }
    void setNoise(float n)        { _noise  = n; }//S2_Clamp(n,   0,    1); }
    void setNShape(float ns)      { _nshape = ns; }//S2_Clamp(ns,  0,    1); }
    void setFeedback(float fb)    { _fb     = fb; }//S2_Clamp(fb,  0,    1); }

    void setDamping(float d) {
      _damp = d;//S2_Clamp(d,   0,    1);
      wg1.damp_lo(_damp);
      //wg2.damp_lo(_damp);
    }

    void setTune(float tune)      { _tune   = tune; }////S2_Clamp(tune,0.01, 2); }
    void setMix(float m)          { _mix    = m; }////S2_Clamp(m,   0,    1); }

    void setImpulse(bool i) {
      wg1.impulse(i);
      //wg2.impulse(i);
    }

    //----------

    void setTuneOct(float v)  { _tune_oct   = v; }
    void setTuneSemi(float v) { _tune_semi  = v; }
    void setTuneFine(float v) { _tune_fine  = v; }

    //float _nshplo = res_nshplo + MModMatrix->dstValue(s2_md_shape);
    //float _noise = res_noise + MModMatrix->dstValue(s2_md_noise);
    //float _damplo = res_damplo + + MModMatrix->dstValue(s2_md_damp);
    //float _feedback = res_feedback + MModMatrix->dstValue(s2_md_fb);
    //float _tune = res_tune + MModMatrix->dstValue(s2_md_tune);
    //float _mix = res_mix + MModMatrix->dstValue(s2_md_mix);

    //CLAMP(_nshplo,0,1);
    //CLAMP(_noise,0,1);
    //CLAMP(_damplo,0,1);
    //CLAMP(_feedback,0,1);
    //CLAMP(_tune,0.01,2);      // !!!
    //CLAMP(_mix,0,1);

    //----------

    float process(float AInput) {

      float v = AInput;
      if (_type != s2_rt_off) {

      float len,fb;
      float w;
      //float v1,v2;
      float rnd = S2_RandomSigned();
      nshape.weight( 1 - S2_Curve(_nshape,0.9) );
      rnd = nshape.process(rnd);
      float nv = v + rnd * _noise;
      wg1.damp_lo(1-_damp);
      //wg2.damp_lo(1-_damp);

//      switch(_type) {
//        case s2_rt_resonator:
//          len = MSampleRate / (_hz1*(S2_Max(0.01f,_tune)));
//          len *= _os;
//          fb = 1 - _fb;
//          fb = (fb*fb*fb);
//          fb = 1 - fb;
//          //w1 = wg1.feed(nv+w2,fb,len); // 1-sample delay
//          //w2 = wg2.feed(w1,fb,len);
//          //v1 = wg1.tap(len,0.1);
//          //v2 = wg2.tap(len,0.9);
//          //w = v1 + v2;
//          w = wg1.feed(nv,fb,len);
//          //v = S2_Interpolate_Linear( _mix, nv, w );
//          v = S2_Interpolate_Linear( _mix, v, w );
//          v = dc.process(v);  // ???
//          break;
//        case s2_rt_pluck:

          //float note = (_tune_oct * 12) + _tune_semi + _tune_fine;// + _tune;
          //float hz = S2_NoteToHz(note);
          //len = MSampleRate / hz;

          //len = MSampleRate / (_hz1*(S2_Max(0.01f,_tune)));

          float h = S2_Clamp(_hz1,20,MSampleRate*0.5);

          len = MSampleRate / h;//(_hz1*(S2_Max(0.01f,_tune)));

          len *= _os;
          fb = 1 - _fb;
          fb = (fb*fb*fb);
          fb = 1 - fb;
          w = wg1.feed(nv,fb,len);
          v = S2_Interpolate_Linear( _mix, v, w );
          v = dc.process(v);  // ???
//          break;
//      }

      } // !off

      return v;
    }

};

//----------------------------------------------------------------------
#endif

