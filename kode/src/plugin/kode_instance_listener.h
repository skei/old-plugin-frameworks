#ifndef kode_instance_listener_included
#define kode_instance_listener_included
//----------------------------------------------------------------------

// call back to format specifics..


/*
  inherited by
  - KODE_Vst3Instance, etc..
  
  plugin format:
  
  
*/

class KODE_InstanceListener {
public:
  virtual uint32_t  getPluginFormat() { return KODE_PLUGIN_FORMAT_NONE; }
  virtual float     getSamplerate() { return 0.0f; }
  virtual void      notifyHostUpdateParameter(uint32_t AIndex, float AValue) {}
  virtual void      notifyHostResizeWindow(uint32_t AWidth, uint32_t AHeight) {}
  virtual void      notifyHostMidiOutput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
};

//----------------------------------------------------------------------
#endif