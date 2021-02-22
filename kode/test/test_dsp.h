
#define KODE_DEBUG_TRACE_SOCKET

//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_memory.h"
#include "base/kode_random.h"
#include "base/kode_event.h"

#include "audio/kode_audio_utils.h"

#include "plugin/kode_descriptor.h"
#include "plugin/kode_dsp_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define OUTPUT 16

#include "test_dsp_processor.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_Descriptor() {
    MName       = "test_dsp";
    MAuthor     = "skei.audio";
    MNumInputs  = 2;
    MNumOutputs = 2;
    appendParameter( KODE_New KODE_FloatParameter("param", 0.1f, 0.0f, 1.0f) );
    appendParameter( KODE_New KODE_FloatParameter("lerp", 0.1f, 0.0f, 1.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );
    //setFlag(KODE_PLUGIN_HAS_EDITOR);
    //setEditorSize(320,200);
  }
  
  //----------
  
  virtual ~myDescriptor() {
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myInstance
: public KODE_DspInstance<myDspProcessor> {
  
//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_DspInstance(ADescriptor) {
  }
  
  //----------
  
  virtual ~myInstance() {
  }
  
//------------------------------
public:
//------------------------------

  void on_sampleRate(float ASampleRate) override {
    MDspContext.samplerate = ASampleRate;
    dspInitialize(&MDspContext);
  }
  
  //----------
  
  void on_parameterChange(uint32_t AIndex, float AValue) override {
    dspParameter(AIndex,AValue);
  }
    
  //----------
  
  void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
    dspMidi(AOffset,AMsg1,AMsg2,AMsg3);
  }
  
  //----------
  
  void on_process(KODE_ProcessContext* AContext) override {
    //MDspContext.processContext = AContext;
    MDspContext.numSamples = AContext->numSamples;
    dspProcess( AContext->outputs[0], OUTPUT );
    KODE_CopyMonoBuffer(AContext->outputs[1],AContext->outputs[0],AContext->numSamples);
  }

};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
