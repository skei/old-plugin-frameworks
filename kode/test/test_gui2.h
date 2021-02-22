
#define KODE_GUI_CAIRO
//#define KODE_GUI_XCB


#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

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
    MName       = "test_gui2";
    MAuthor     = "skei.audio";
    MNumInputs  = 2;
    MNumOutputs = 2;
    setFlag(KODE_PLUGIN_HAS_EDITOR);
    setEditorSize(800,600);
    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {
  
//------------------------------
private:
//------------------------------

  KODE_Descriptor* MDescriptor = KODE_NULL;
  
//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    MDescriptor =ADescriptor;
  }

//------------------------------
public:
//------------------------------

  KODE_Editor* on_openEditor(void* AParent) final {
    KODE_Editor* editor = KODE_New KODE_Editor(this,MDescriptor,AParent);
    
    
    
    return editor;
  }
  
  //----------
  
  void on_closeEditor(KODE_Editor* AEditor) final {
    delete AEditor;
  }
  
};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
