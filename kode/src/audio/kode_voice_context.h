#ifndef kode_voice_context_included
#define kode_voice_context_included
//----------------------------------------------------------------------

//#include "audio/kode_modulation_matrix.h"

//----------

struct KODE_VoiceContext {
  float                   samplerate        = 0.0f;
  uint32_t                numsamples        = 0;
  float*                  inputs[8]         = {0};
  float*                  outputs[8]        = {0};
  //KODE_ModulationMatrix*  mod_matrix        = KODE_NULL;
  float                   master_bend       = 0.0f;
  float                   master_pressure   = 0.0f;
  float                   master_brightness = 0.0f;
};

//----------------------------------------------------------------------
#endif
