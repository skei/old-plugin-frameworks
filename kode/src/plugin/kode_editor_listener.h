#ifndef kode_editor_listener_included
#define kode_editor_listener_included
//----------------------------------------------------------------------

// instance

class KODE_EditorListener {
public:
  virtual void updateParameterFromEditor(uint32_t AIndex, float AValue) {}
  virtual void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) {}
  //virtual void sendMidiFromEditor(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
};

//----------------------------------------------------------------------
#endif
