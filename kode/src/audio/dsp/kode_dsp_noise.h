#ifndef kode_dsp_noise_included
#define kode_dsp_noise_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/kode_dsp_context.h"

class KODE_DspNoise {
  
private:

  KODE_DspContext*  MContext    = KODE_NULL;
  uint32_t          MBuffer     = 0;
  float*            MBufferPtr  = KODE_NULL;
  
public:

  void initialize(KODE_DspContext* AContext) {
    MContext = AContext;
  }
  
  void connect(uint32_t AIndex, uint32_t ABuffer) {
     if (AIndex==0) {
       MBuffer    = ABuffer;
       MBufferPtr = MContext->dspBuffer + (ABuffer * KODE_DSP_SIZE);
     }
  }
  
  void parameter(uint32_t AIndex,float AValue) {
  }

  void process() {
    for (uint32_t i=0; i<KODE_DSP_SIZE; i++) {
      MBufferPtr[i] = KODE_RandomSigned();// * 0.1f;
    }
  }
  
  void process(uint32_t ASize)  {
    for (uint32_t i=0; i<ASize; i++) {
      MBufferPtr[i] = KODE_RandomSigned();// * 0.1f;
    }
  }
  
};


//----------------------------------------------------------------------
#endif