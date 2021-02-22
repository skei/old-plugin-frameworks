#ifndef kode_dsp_random_included
#define kode_dsp_random_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/kode_dsp_context.h"

class KODE_DspRandom {
  
private:

  KODE_DspContext*  MContext  = KODE_NULL;
  float*            MOutPtr   = KODE_NULL;
  
public:

  void initialize(KODE_DspContext* AContext) {
    MContext = AContext;
  }
  
  void connect(uint32_t AIndex, uint32_t ABuffer) {
    float* ptr = MContext->dspBuffer + (ABuffer * KODE_DSP_SIZE);
    switch(AIndex) {
      case 0: MOutPtr = ptr;  break;
    }
  }
  
  void parameter(uint32_t AIndex,float AValue) {
  }

  void process() {
    for (uint32_t i=0; i<KODE_DSP_SIZE; i++) {
      MOutPtr[i] = KODE_RandomSigned();
    }
  }
  
  void process(uint32_t ASize)  {
    for (uint32_t i=0; i<ASize; i++) {
      MOutPtr[i] = KODE_RandomSigned();
    }
  }
  
};


//----------------------------------------------------------------------
#endif