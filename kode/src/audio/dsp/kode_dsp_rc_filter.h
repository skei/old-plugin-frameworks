#ifndef kode_rc_filter_node_included
#define kode_rc_filter_node_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/kode_audio_node_context.h"
#include "audio/filters/kode_rc_filter.h"

class KODE_RcFilterNode {
  
private:

  KODE_AudioNodeContext*  MContext  = KODE_NULL;
  uint32_t                MBuffer   = 0;
  float*                  MInPtr    = KODE_NULL;
  float*                  MOutPtr   = KODE_NULL;
  
public:

  void initialize(KODE_AudioNodeContext* AContext) {
    MContext = AContext;
  }
  
  void connect(uint32_t AIndex, uint32_t ABuffer) {
    MBuffer = AIndex;
    switch(AIndex) {
      case 0: MInPtr  = MContext->nodeBuffer + (ABuffer * KODE_AUDIO_NODE_SIZE); break;
      case 1: MOutPtr = MContext->nodeBuffer + (ABuffer * KODE_AUDIO_NODE_SIZE); break;
    }
  }
  
  void parameter(uint32_t AIndex,float AValue) {
  }

  void process() {
    for (uint32_t i=0; i<KODE_AUDIO_NODE_SIZE; i++) {
      //MBufferPtr[i] = KODE_RandomSigned() * 0.1f;
    }
  }
  
  void process(uint32_t ASize)  {
    for (uint32_t i=0; i<ASize; i++) {
      //MBufferPtr[i] = KODE_RandomSigned() * 0.1f;
    }
  }
  
};


//----------------------------------------------------------------------
#endif