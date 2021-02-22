
#define KODE_NO_GUI

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    MName       = "fx_gain";
    MAuthor     = "skei.audio";
    MNumInputs  = 2;
    MNumOutputs = 2;
    appendParameter( KODE_New KODE_FloatParameter("Gain", 1.0f, 0.0f, 2.0f) );
  }
  
  virtual ~myDescriptor() {
  }
  
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
  
private:

  float MGain = 0.0f;
  
public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }
  
  virtual ~myInstance() {
  }

  void on_parameterChange(uint32_t AIndex, float AValue) final {
    switch(AIndex) {
      case 0: MGain = AValue; break;
    }
  }
    
  void on_process(KODE_ProcessContext* AContext) final {
    float*    in0   = AContext->inputs[0];
    float*    in1   = AContext->inputs[1];
    float*    out0  = AContext->outputs[0];
    float*    out1  = AContext->outputs[1];
    uint32_t  len   = AContext->numSamples;
    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * MGain;
      *out1++ = *in1++ * MGain;
    }
  }
  
};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
