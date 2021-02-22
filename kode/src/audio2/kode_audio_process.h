#ifndef kode_audio_graph_included
#define kode_audio_graph_included
//----------------------------------------------------------------------

#define KODE_TICKSIZE 16

//----------------------------------------------------------------------

//struct KODE_AudioModule {
//  void setBuffer(uint32_t AIndex, uint32_t ABuffer) {}
//  void process() {}
//};

//----------------------------------------------------------------------

struct KODE_TickContext {
  float*    MBuffers    = KODE_NULL;
};

//----------------------------------------------------------------------

class KODE_PhasorModule {
  
private:

  KODE_TickContext* MContext    = KODE_NULL;
  float             MSampleRate = 0.0f;
  float             MHz         = 0.0f;
  float             MPhase      = 0.0f;
  float             MPhaseAdd   = 0.0f;
  float*            MBuffer     = KODE_NULL;
  
public:

  KODE_PhasorModule(KODE_TickContext* AContext) {
    MContext = AContext;
  }

public:

  void setSamplerate(float sr) {
    MSampleRate = sr;
  }
  
  void setFrequency(float hz) {
    KODE_Assert(MSampleRate > 0.0f);
    MHz = hz;
    //MPhaseLength = MSampleRate / hz;
    MPhaseAdd = hz / MSampleRate;
  }

  void setPhase(float ph) {
    MPhase = ph;
  }
  
  void setPhaseAdd(float ph) {
    MPhaseAdd = ph;
  }
  
  void setBuffer(uint32_t AIndex, uint32_t ABuffer) {
    switch(AIndex) {
      case 0:
        MBuffer = &MContext->MBuffers[ABuffer * KODE_TICKSIZE];
        break;
    }
  }
  
  void process() {
    float* ptr = MBuffer;
    for (uint32_t i=0; i<KODE_TICKSIZE; i++) {
      // phase
      MPhase += MPhaseAdd;
      MPhase -= (long)MPhase; // fract(t)
      *ptr++ = MPhase;
    }
  }
  
};

//----------------------------------------------------------------------

class KODE_AudioProcess {

  private:

    KODE_PhasorModule MPhasor;

public:

    void setup() {
      MPhasor.setBuffer(0,0);
    }
  
    void process() {
      MPhasor.process();
    }

};

//----------------------------------------------------------------------
#endif

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
