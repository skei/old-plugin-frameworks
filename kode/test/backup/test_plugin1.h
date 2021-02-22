
//#define KODE_DEBUG
//#define KODE_DEBUG_VST2

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

#define KODE_GUI_CAIRO

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

#include "gui/kode_widgets.h"

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    KODE_TRACE;
    MName = "plugin";
    MAuthor = "skei.audio";
    MNumInputs = 2;
    MNumOutputs = 2;
    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  1.0f, 0.0f, 2.0f) );
    MEditorDefaultWidth = 640;
    MEditorDefaultHeight = 480;
    setFlag(KODE_PLUGIN_HAS_EDITOR);
  }
  
  //----------
  
  virtual ~myDescriptor() {
    KODE_TRACE;
  }
  
};

//----------------------------------------------------------------------

class myEditor : public KODE_Editor {
  
public:

 myEditor(KODE_EditorListener* AListener, KODE_Descriptor* ADescriptor, void* AParent)
 : KODE_Editor(AListener,ADescriptor,AParent) {
   KODE_TRACE;
   appendWidget( KODE_New KODE_Knob2Widget(   KODE_Rect(10,10,40,72), 0.25f, false));
   appendWidget( KODE_New KODE_Knob2Widget(   KODE_Rect(60,10,40,72), 0.75f, false));
   appendWidget( KODE_New KODE_WaveformWidget(KODE_Rect(10,90,100,50) ));
   connect( getChild(0), 0 );
   connect( getChild(1), 1 );
 }
 
 virtual ~myEditor() {
   KODE_TRACE;
 }
  
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
  
private:

  float         MLeft   = 0.0f;
  float         MRight  = 0.0f;
  KODE_Editor*  MEditor = KODE_NULL;
  
public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    KODE_TRACE;
  }
  
  virtual ~myInstance() {
    KODE_TRACE;
  }
  
public:

  //void on_open()        override { KODE_TRACE; }
  //void on_close()       override { KODE_TRACE; }
  //void on_initialize()  override { KODE_TRACE; }
  //void on_terminate()   override { KODE_TRACE; }
  //void on_activate()    override { KODE_TRACE; }
  //void on_deactivate()  override { KODE_TRACE; }
  //void on_startPreset() override { KODE_TRACE; }
  //void on_endPreset()   override { KODE_TRACE; }
  
  void on_parameterChange(uint32_t AIndex, float AValue) override {
    KODE_Trace("%i %.3f\n",AIndex,AValue);
    switch(AIndex) {
      case 0: MLeft   = AValue; break;
      case 1: MRight  = AValue; break;
    }
  }
    
  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {}
  
  void on_process(KODE_ProcessContext* AContext) override {
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
  
  KODE_Editor* on_openEditor(void* AParent) override {
    MEditor = KODE_New myEditor(this,getDescriptor(),AParent);
    return MEditor;
  }
  
  void on_closeEditor(KODE_Editor* AEditor) override {
    KODE_Delete MEditor;
  }
  
  void on_updateEditor(KODE_Editor* AEditor) override {
  }
  
};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_MAIN(myDescriptor,myInstance)

