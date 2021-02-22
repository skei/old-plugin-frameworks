#ifndef kode_dsp_add_included
#define kode_dsp_add_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/kode_dsp_context.h"

class KODE_DspAdd {
  
private:

  KODE_DspContext*  MContext  = KODE_NULL;
  float*            MIn0Ptr   = KODE_NULL;
  float*            MIn1Ptr   = KODE_NULL;
  float*            MOutPtr   = KODE_NULL;
  
public:

  void initialize(KODE_DspContext* AContext) {
    MContext = AContext;
  }
  
  void connect(uint32_t AIndex, uint32_t ABuffer) {
    float* ptr = MContext->dspBuffer + (ABuffer * KODE_DSP_SIZE);
    switch(AIndex) {
      case 0: MIn0Ptr = ptr;  break;
      case 1: MIn1Ptr = ptr;  break;
      case 2: MOutPtr = ptr;  break;
    }
  }
  
  void parameter(uint32_t AIndex,float AValue) {
  }

  void process() {
    for (uint32_t i=0; i<KODE_DSP_SIZE; i++) {
      MOutPtr[i] = MIn0Ptr[i] + MIn1Ptr[i];
    }
  }
  
  void process(uint32_t ASize)  {
    for (uint32_t i=0; i<ASize; i++) {
      MOutPtr[i] = MIn0Ptr[i] + MIn1Ptr[i];
    }
  }
  
};


//----------------------------------------------------------------------
#endif