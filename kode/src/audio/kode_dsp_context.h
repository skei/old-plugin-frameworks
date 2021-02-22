#ifndef kode_dsp_context_included
#define kode_dsp_context_included
//----------------------------------------------------------------------

#include "plugin/kode_process_context.h"

struct KODE_DspContext {
  float __KODE_ALIGNED(KODE_ALIGNMENT_CACHE) dspBuffer[KODE_DSP_SIZE * KODE_DSP_NUM_BUFFERS];
  //KODE_ProcessContext*  processContext  = KODE_NULL;
  float                 samplerate      = 0.0f;
  uint32_t              numSamples      = 0;
};

//----------------------------------------------------------------------
#endif