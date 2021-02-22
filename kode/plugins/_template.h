
#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
//#include "plugin/kode_editor.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_Descriptor() {
    MName = "_template";
    MAuthor = "skei.audio";
    MNumInputs = 2;
    MNumOutputs = 2;
    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );
    //setFlag(KODE_PLUGIN_HAS_EDITOR);
    //setEditorSize(320,200);
  }
  
  //----------
  
  virtual ~myDescriptor() {
  }
  
};

//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {
  
//------------------------------
private:
//------------------------------

  float MLeft = 0.0f;
  float MRight = 0.0f;
  bool need_recalc = false;
  
//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }
  
  //----------
  
  virtual ~myInstance() {
  }

//------------------------------
public:
//------------------------------

  //void on_open() final {}
  //void on_close() final {}
  //void on_initialize() final {}
  //void on_terminate() final {}
  //void on_activate() final {}
  //void on_deactivate() final {}
  //void on_startUpdateAllParameters() final {}
  //void on_endUpdateAllParameters() final {}
  //void on_sampleRate(float ASampleRate) final {}
  
  //----------
  
  void on_parameterChange(uint32_t AIndex, float AValue) final {
    //switch(AIndex) {
    //  case 0:
    //    MLeft = AValue;
    //    break;
    //  case 1:
    //    MRight = AValue;
    //    break;
    //}
    //need_recalc = true;
  }
    
  //----------
  
  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
  //}
  
  //----------
  
  void on_process(KODE_ProcessContext* AContext) final {
    if (need_recalc) {
      need_recalc = false;
      //recalc(AContext->samplerate);
    }
    uint32_t len = AContext->numSamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * MLeft;
      *out1++ = *in1++ * MRight;
    }
  }
  
  //----------
  
  //KODE_Editor* on_openEditor(void* AParent) final {
  //  KODE_Editor* editor = KODE_New KODE_Editor(this,getDescriptor(),AParent);
  //  return editor;
  //}
  
  //----------
  
  //void on_closeEditor(KODE_Editor* AEditor) final {
  //  KODE_Delete AEditor;
  //}
  
  //----------
  
  //void on_updateEditor(KODE_Editor* AEditor) final {
  //}
  
};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
