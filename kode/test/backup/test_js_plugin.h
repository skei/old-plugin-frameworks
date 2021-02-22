
#define KODE_DEBUG

//#define KODE_PLUGIN_EXE
#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_js_plugin.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myDescriptor : public KODE_JSDescriptor {

//------------------------------
public:
//------------------------------

  myDescriptor()
  : KODE_JSDescriptor() {
    desc("js_loser_exciter");
    slider(0,"slider1",0.35f, 0.0f, 1.0f);
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myInstance : public KODE_JSInstance {

//------------------------------
public:
//------------------------------

  myInstance(KODE_Descriptor* ADescriptor) : KODE_JSInstance(ADescriptor) {
    ext_noinit = 1;
  }
  
  //----------
  
  virtual ~myInstance() {}
  
//------------------------------
public:
//------------------------------

  void on_init() final {
    KODE_TRACE;
  }
  
  //----------
  
  void on_slider() final {
    KODE_TRACE;
  }

  //----------
  
  void on_block() final {
    //KODE_TRACE;
  }

  //----------
  
  void on_sample() final {
    //KODE_TRACE;
  }

  //----------
  
  void on_gfx() final {
    KODE_TRACE;
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------









#if 0
#endif // 0
