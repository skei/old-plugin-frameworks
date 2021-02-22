#ifndef kode_exe_instance_included
#define kode_exe_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_instance_listener.h"

class KODE_ExeInstance
: public KODE_InstanceListener {
  
public:

  KODE_ExeInstance(KODE_Instance* AInstance) {
  }
  
  virtual ~KODE_ExeInstance() {
  }
  
public: // instance listener

  uint32_t getPluginFormat() override {
    //KODE_Trace("EXE\n");
    return KODE_PLUGIN_FORMAT_EXE;
  }

  //void notifyHostUpdateParameter(uint32_t AIndex, float AValue) override {
  //  KODE_TRACE;
  //}
  
  //void notifyHostResizeWindow(uint32_t AWidth, uint32_t AHeight) override {
  //  KODE_TRACE;
  //}
  
  //void notifyHostMidiOutput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
  //  KODE_TRACE;
  //}
  
  
};

//----------------------------------------------------------------------
#endif