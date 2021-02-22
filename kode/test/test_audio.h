

#include "base/kode.h"
#include "audio2/kode_audio_process.h"

int main() {
  return 0;
}












//----------------------------------------------------------------


#if 0


#include <stdio.h> // printf

#define ALIGNMENT_CACHE 64
#define TICKSIZE 16
#define __ALIGNED(x) __attribute__ ((aligned (x)))
typedef float __ALIGNED(ALIGNMENT_CACHE) CacheAlignedBuffer[TICKSIZE];

//----------------------------------------------------------------
/*

  buffer:
  - clear, set
  - mul, add
  - integrate
  - differentiate
  - interpolate
  
  generate:
  - calcPhase
  - calcEnvelope
  - calcLfo

  process:
  - calcSaw, Squ, ..
  - antialias (polyblep, dsf, ..)
  - filter
  
  env/lfo: precalc buffer when changing settings (adsr, speed, etc,) only?

*/
//----------------------------------------------------------------

template <int NUM>
class KODE_Oscillator {

public:

  float MPhase[NUM];
  float MPhaseAdd[NUM];
  float MPrevSum[NUM];

public:  

  void update(float* AOutput) {
    for (int i=0; i<NUM; i++) {
      
      // phase
      float dt = MPhaseAdd[i];
      float t = MPhase[i];
      t += dt;
      t -= (long)t; // fract(t)
      MPhase[i] = t;
      
      // dpw
      float res = 0.0f;
      if (dt > 0.0f) {
        float sum = t;
        sum *= t;
        sum -= t;
        res = sum;
        res -= MPrevSum[i];
        res /= dt;
        MPrevSum[i] = sum;
      }
      AOutput[i] = res;
    }
  }
  
};

//----------------------------------------------------------------

template <int NUM>
class KODE_Envelope {
  
  float MAtt[NUM];
  float MDec[NUM];
  float MSus[NUM];
  float MRel[NUM];
  
public:

  void update(float* AOutput) {}
};

//----------------------------------------------------------------

template <int NUM>
class KODE_Lfo {
public:
  void update(float* AOutput) {}
};

//----------------------------------------------------------------

template <int NUM>
class KODE_Filter {
public:
  void update(float* AInput, float* AOutput) {}
};

//----------------------------------------------------------------

//float16_t osc_buffer;
//float16_t env_buffer;
//float16_t lfo_buffer;
//float16_t flt_buffer;

//----------------------------------------------------------------

template <int NUM>
class KODE_Voice {
  
public:

  //float osc_buffer[NUM];
  CacheAlignedBuffer osc_buffer;
  CacheAlignedBuffer env_buffer;
  CacheAlignedBuffer lfo_buffer;
  CacheAlignedBuffer flt_buffer;

  KODE_Oscillator<NUM>  MOscillator;
  KODE_Envelope<NUM>    MEnvelope;
  KODE_Lfo<NUM>         MLfo;
  KODE_Filter<NUM>      MFilter;
  
public:
  
  void update() {
    MOscillator.update(osc_buffer);
    MEnvelope.update(env_buffer);
    MLfo.update(lfo_buffer);
    MFilter.update(osc_buffer,flt_buffer);
  }
  
};


//----------------------------------------------------------------------

int main(void) {
  KODE_Voice<16> v;
  v.update();
  return 0;
}

#endif



//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------


//#define ALIGNMENT_CACHE 64
//#define TICKSIZE 16
//#define __ALIGNED(x) __attribute__ ((aligned (x)))
//
//typedef float __ALIGNED(ALIGNMENT_CACHE) CacheAlignedBuffer[TICKSIZE];
//
////----------------------------------------------------------------------
//
//template<float* in0, float* out0>
//class Filter {
//public:
//  void process() { for (int i=0; i<TICKSIZE; i++) out0[i] = in0[i]; }
//  void process(int size)  { for (int i=0; i<size; i++) out0[i] = in0[i]; }
//};
//
////----------------------------------------------------------------------
//
//class AudioBlockProcessor {
//  
//private:
//
//  // todo
//  void preprocess_events() {}
//  void postprocess_events() {}
//  void process_events(int offset) {}
//  void process_events(int offset, int size) {}
//  
//  void copy_buffer(float* dst, float* src) {
//    for (int i=0; i<TICKSIZE; i++) *dst++ = *src++;
//  }
//  
//  void copy_buffer(float* dst, float* src, int size) {
//    for (int i=0; i<size; i++) *dst++ = *src++;
//  }
//
//private:
//
//  //PROC MProcess;
//  
//public:
//
//  void process(int ANumSamples, float* AResult) {
//    int remaining = ANumSamples;
//    int offset = 0;
//    preprocess_events();
//    while (remaining > 0) {
//      if (remaining >= TICKSIZE) {
//        process_events(offset);
//        //MProcess.process();
//        //copy_buffer(AResult + offset,MProcess.getBuffer());
//        remaining -= TICKSIZE;
//        offset += TICKSIZE;
//      }
//      else {
//        process_events(offset,remaining);
//        //MProcess.process(remaining);
//        //copy_buffer(AResult + offset,MProcess.getBuffer(),remaining);
//        remaining = 0;
//      }
//    }
//    postprocess_events();
//  }
//
//};
//
//
////----------------------------------------------------------------------
//
//int main() {
//  float buffer[256] = {0};
//  AudioBlockProcessor proc;
//  proc.process(256,buffer);
//  return 0;
//}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------
//----------------------------------------------------------------------

/*
template<float* BUFFER>
class Effect1 {
public:
  float* getBuffer() { return BUFFER; }
  void process() { for (int i=0; i<TICKSIZE; i++) BUFFER[i] = i; }
  void process(int size)  { for (int i=0; i<size; i++) BUFFER[i] = i; }
};

template<float* BUFFER>
class Effect2 {
public:
  float* getBuffer() { return BUFFER; }
  void process() { for (int i=0; i<TICKSIZE; i++) BUFFER[i] *= -1; }
  void process(int size)  { for (int i=0; i<size; i++) BUFFER[i] *= -1; }
};

//----------------------------------------------------------------------

CacheAlignedBuffer TickBuffer1;

typedef Effect1<TickBuffer1> myEffect1;
typedef Effect2<TickBuffer1> myEffect2;

//

template<float* BUFFER>
class myAudioTickProcessor {
private:
  Effect1<BUFFER> myEffect1;
  Effect2<BUFFER> myEffect2;
  float* getBuffer() { return BUFFER; }
  void process() { for (int i=0; i<TICKSIZE; i++) BUFFER[i] *= -1; }
  void process(int size)  { for (int i=0; i<size; i++) BUFFER[i] *= -1; }
};

//----------------------------------------------------------------------

template<class PROC>
class AudioBlockProcessor {
  
private:

  // todo
  void preprocess_events() {}
  void postprocess_events() {}
  void process_events(int offset) {}
  void process_events(int offset, int size) {}
  
  void copy_buffer(float* dst, float* src) {
    for (int i=0; i<TICKSIZE; i++) *dst++ = *src++;
  }
  
  void copy_buffer(float* dst, float* src, int size) {
    for (int i=0; i<size; i++) *dst++ = *src++;
  }

private:

  PROC MProcess;
  
public:

  void process(int ANumSamples, float* AResult) {
    int remaining = ANumSamples;
    int offset = 0;
    preprocess_events();
    while (remaining > 0) {
      if (remaining >= TICKSIZE) {
        process_events(offset);
        MProcess.process();
        copy_buffer(AResult + offset,MProcess.getBuffer());
        remaining -= TICKSIZE;
        offset += TICKSIZE;
      }
      else {
        process_events(offset,remaining);
        MProcess.process(remaining);
        copy_buffer(AResult + offset,MProcess.getBuffer(),remaining);
        remaining = 0;
      }
    }
    postprocess_events();
  }

};

//----------------------------------------------------------------------


//----------

int main() {
  float finalbuffer[256] = {0};
  AudioBlockProcessor<myAudioTickProcessor> myAudioBlockProcessor;
  myAudioBlockProcessor.process(256,finalbuffer);
  for (int i=0; i<256; i++) printf("%i. %.2f\n",i,finalbuffer[i]);
  return 0;
}
*/