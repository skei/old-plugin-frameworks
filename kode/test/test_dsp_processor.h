#ifndef test_dsp_processor_included
#define test_dsp_processor_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "audio/kode_dsp.h"
#include "audio/kode_dsp_context.h"

//----------------------------------------------------------------------

//struct KODE_DspVoiceContext {
//  KODE_DspContext*  dspContext  = KODE_NULL;
//};

//----------------------------------------------------------------------

//class myVoiceProcessor {
//public:
//  void initialize(KODE_DspVoiceContext* AContext) {}
//  void process() {}
//  void process(uint32_t ASize) {}
//};

//----------------------------------------------------------------------

class myDspProcessor {

//------------------------------
private:
//------------------------------

  //KODE_DspVoiceContext  MVoiceContext;
  //myVoiceProcessor      MVoices;

  KODE_DspParam         MDspParam;
  KODE_DspParamLerp     MDspParamLerp;
  KODE_DspRandom        MDspRandom;
  KODE_DspMul           MDspMul;
  
//------------------------------
public:
//------------------------------

  void initialize(KODE_DspContext* AContext) {
    
    //MVoiceContext.dspContext = AContext;
    //MVoices.initialize(&MVoiceContext);

    MDspParam.initialize(AContext);
    MDspParam.parameter(0,0.1f);
    MDspParam.connect(0,1);             // out param

    MDspParamLerp.initialize(AContext);
    MDspParamLerp.parameter(0,0.1f);
    MDspParamLerp.connect(0,2);         // out lerp

    MDspRandom.initialize(AContext);
    MDspRandom.connect(0,3);            // out random

    MDspMul.initialize(AContext);
    MDspMul.connect(0,2);               // in mul.a
    MDspMul.connect(1,3);               // in mul.b
    MDspMul.connect(2,OUTPUT);          // out mul
    
    
  }
  
  //----------
  
  void parameter(uint32_t AIndex, float AValue) {
    switch(AIndex) {
      case 0: MDspParam.parameter(0,AValue);      break;
      case 1: MDspParamLerp.parameter(0,AValue);  break;
    }
  }
  
  //----------
  
  void midi(uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
  }
  
  //----------
  
  void process() {
    //MVoices.process();
    MDspParam.process();
    MDspParamLerp.process();
    MDspRandom.process();
    MDspMul.process();
    
    
  }
  
  //----------
  
  void process(uint32_t ASize) {
    //MVoices.process();
    MDspParam.process(ASize);
    MDspParamLerp.process(ASize);
    MDspRandom.process(ASize);
    MDspMul.process(ASize);
  }
  
};

//----------------------------------------------------------------------
#endif
