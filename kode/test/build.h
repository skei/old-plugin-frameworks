
//#define KODE_DEBUG
//#define KODE_DEBUG_VST2

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_plugin.h"

#include "base/kode_inifile.h"

//----------------------------------------------------------------------

int main() {
  KODE_IniFile ini;
  bool result;
  result = ini.load();
  if (result) {
    int32_t i = ini.getInt("section1","value32");
    KODE_Trace("%i\n",i);
  }
  //ini.save();
  return 0;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#if 0

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    KODE_TRACE;
    MName = "test plugin";
    MAuthor = "skei.audio";
    MNumInputs = 2;
    MNumOutputs = 2;
    appendParameter( KODE_New KODE_Parameter("param1", 0.5f) );
  }
  
  virtual ~myDescriptor() {
    KODE_TRACE;
  }
  
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
  
public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    KODE_TRACE;
  }
  
  virtual ~myInstance() {
    KODE_TRACE;
  }
  
};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_MAIN(myDescriptor,myInstance)

#endif // 0
