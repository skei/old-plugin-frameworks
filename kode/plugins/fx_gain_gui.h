
#define KODE_GUI_CAIRO

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"
#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    MName                 = "fx_gain_gui";
    MAuthor               = "skei.audio";
    MNumInputs            = 2;
    MNumOutputs           = 2;
    MEditorDefaultWidth   = 256;
    MEditorDefaultHeight  = 128;
    setFlag(KODE_PLUGIN_HAS_EDITOR);
    appendParameter( KODE_New KODE_FloatParameter("Gain", 1.0f, 0.0f, 2.0f) );
  }
  
  virtual ~myDescriptor() {
  }
  
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
  
private:

  float         MGain   = 0.0f;
  KODE_Editor*  MEditor = KODE_NULL;
  
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
  
  KODE_Editor* on_openEditor(void* AParent) override {
    MEditor = KODE_New KODE_Editor(this,getDescriptor(),AParent);
    KODE_Widget* knob = KODE_New KODE_Knob2Widget( KODE_Rect( 10,10, 40,72 ));
    MEditor->appendWidget(knob);
    MEditor->connect(knob,0);
    return MEditor;
  }
  
  void on_closeEditor(KODE_Editor* AEditor) override {
    KODE_Delete MEditor;
    MEditor = KODE_NULL;
  }
  
};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
