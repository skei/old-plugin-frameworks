#ifndef kode_dsp_param_lerp_included
#define kode_dsp_param_lerp_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/kode_dsp_context.h"

class KODE_DspParamLerp {
  
private:

  KODE_DspContext*  MContext  = KODE_NULL;
  float*            MOutPtr   = KODE_NULL;
  
  float             MCurrentValue = 0.0f;
  float             MTargetValue  = 0.0f;
  
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
    switch(AIndex) {
      case 0: MTargetValue = AValue;  break;
    }
  }

  void process() {
    float diff = (MTargetValue - MCurrentValue);
    float adder = diff * KODE_INV16F;
    for (uint32_t i=0; i<KODE_DSP_SIZE; i++) {
      MOutPtr[i] = MCurrentValue;
      MCurrentValue += adder;
    }
  }
  
  void process(uint32_t ASize)  {
    float diff = (MTargetValue - MCurrentValue);
    float adder = diff * KODE_INV16F;
    for (uint32_t i=0; i<ASize; i++) {
      MOutPtr[i] = MCurrentValue;
      MCurrentValue += adder;
    }
  }
  
};


//----------------------------------------------------------------------
#endif