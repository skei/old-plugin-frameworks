#ifndef kode_voice_included
#define kode_voice_included
//----------------------------------------------------------------------

//#include "base/kode_list.h"
//#include "audio/kode_voice_context.h"
//#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

#define KODE_SYNTH_VOICE_FREE       0
#define KODE_SYNTH_VOICE_PLAYING    1
#define KODE_SYNTH_VOICE_RELEASED   2
#define KODE_SYNTH_VOICE_FINISHED   3

//----------------------------------------------------------------------

//class KODE_Voice
//: public KODE_ListNode {

class KODE_Voice {

protected:  
  
  //KODE_VoiceContext*  MVoiceContext = KODE_NULL;
  uint32_t            MState        = KODE_SYNTH_VOICE_FREE;
  int32_t             MNote         = -1;
  int32_t             MVelocity     = -1;
  int32_t             MChannel      = -1;

public:
  
  KODE_Voice() {}
  virtual ~KODE_Voice() {}
    
public:

  uint32_t  getState(void)                  { return MState; }
  void      setState(uint32_t AState)       { MState = AState; }

  int32_t   getChannel(void)                { return MChannel; }
  void      setChannel(int32_t AChannel)    { MChannel = AChannel; }

  int32_t   getNote(void)                   { return MNote; }
  void      setNote(int32_t ANote)          { MNote = ANote; }

  int32_t   getVelocity(void)               { return MVelocity; }
  void      setVelocity(int32_t AVelocity)  { MVelocity = AVelocity; }

public:

  /*
  void      on_initialize(KVoiceContext* AContext) { MVoiceContext = AContext; }
  uint32_t  on_getState(void) { return MState; }
  void      on_setState(uint32_t AState) { MState = AState; }
  void      on_noteOn(uint32_t ANote, float AVelocity) {}
  void      on_noteOff(uint32_t ANote, float AVelocity) {}
  void      on_pitchBend(float APitchBend) {}
  void      on_pressure(float AValue) {}
  void      on_brightness(float AValue) {}
  void      on_control(uint32_t AIndex, float AValue) {}
  void      on_masterBend(float APitchBend) {}
  void      on_masterPressure(float AValue) {}
  void      on_masterBrightness(float AValue) {}
  void      on_masterControl(uint32_t AIndex, float AValue) {}
  void      on_parameter(uint32_t AIndex, float AValue) {}
  void      on_process(KODE_ProcessContext* AContext) {}
  */
  
};

//----------------------------------------------------------------------
#endif
