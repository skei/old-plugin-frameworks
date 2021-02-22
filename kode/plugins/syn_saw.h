
#define KODE_NO_GUI
#define KODE_DEBUG_TRACE_SOCKET

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_synth_instance.h"
#include "audio/kode_audio_math.h"
#include "audio/kode_voice.h"
#include "audio/modulation/kode_envelope.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class myDescriptor
: public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    MName = "syn_saw";
    MAuthor = "skei.audio";
    MNumInputs = 2;
    MNumOutputs = 2;
    setFlag(KODE_PLUGIN_IS_SYNTH);
    appendParameter( KODE_New KODE_FloatParameter( "Att",  0.1f, 0.0f, 4.0f ));
    appendParameter( KODE_New KODE_FloatParameter( "Dec",  0.1f, 0.0f, 4.0f ));
    appendParameter( KODE_New KODE_FloatParameter( "Sus",  0.5f, 0.0f, 1.0f ));
    appendParameter( KODE_New KODE_FloatParameter( "Rel",  0.1f, 0.0f, 4.0f ));
    appendParameter( KODE_New KODE_FloatParameter( "Gain", 0.5f, 0.0f, 1.0f ));
  }
  
};

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

class myVoice
: public KODE_Voice {

private:

  KODE_ExponentialAdsrEnvelope  env;
  KODE_VoiceContext*            MVoiceContext = KODE_NULL;
  float                         note          = 0.0f;
  float                         mbend         = 0.0f;
  float                         pbend         = 0.0f;
  float                         t             = 0.0f;
  float                         hz            = 0.0f;
  float                         gain          = 0.0f;

public:

  void on_initialize(KODE_VoiceContext* AContext) {
    MVoiceContext = AContext;
    env.setADSR(1.0f,0.0f,1.0f,3.0f);    
  }
    
  uint32_t on_getState(void) {
    return MState;
  }
  
  void on_setState(uint32_t AState) {
    MState = AState;
  }
  
  uint32_t on_noteOn(uint32_t ANote, float AVelocity) {
    note = ANote;
    //pbend = 0.0f;
    hz = KODE_NoteToHz(note + (mbend*2.0f) + (pbend*48.0f));
    env.noteOn();
    return KODE_SYNTH_VOICE_PLAYING;
  }
  
  uint32_t on_noteOff(uint32_t ANote, float AVelocity) {
    env.noteOff();
    return KODE_SYNTH_VOICE_RELEASED;
  }
  
  void on_pitchBend(float APitchBend) {
    pbend = APitchBend;
    hz = KODE_NoteToHz(note + (mbend*2.0f) + (pbend*48.0f));
  }
  
  void on_masterBend(float APitchBend) {
    mbend = APitchBend;
    hz = KODE_NoteToHz(note + (mbend*2.0f) + (pbend*48.0f));
  }

  void on_pressure(float AValue) {}
  void on_masterPressure(float AValue) {}
  void on_brightness(float AValue) {}
  void on_masterBrightness(float AValue) {}
  void on_control(uint32_t AIndex, float AValue) {}
  void on_masterControl(uint32_t AIndex, float AValue) {}
  
  void on_parameter(uint32_t AIndex, float AValue) {
    switch(AIndex) {
      case 0: env.setAttack(AValue);          break;
      case 1: env.setDecay(AValue);           break;
      case 2: env.setSustain(AValue*AValue);  break;
      case 3: env.setRelease(AValue);         break;
      case 4: gain = AValue*AValue;           break;
    }
  }
  
  void on_process() {
    float     dt    = hz / MVoiceContext->samplerate;
    uint32_t  len   = MVoiceContext->numsamples;
    float*    out0  = MVoiceContext->outputs[0];
    float*    out1  = MVoiceContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      t = KODE_Fract(t);
      float f = (t * 2.0f) - 1.0f;
      float env_value = env.process();
      if (env.getStage() == KODE_ENVELOPE_FINISHED) {
        MState = KODE_SYNTH_VOICE_FINISHED;
        // return, break
      }
      f *= env_value;
      f *= gain;
      *out0++ += f;
      *out1++ += f;
      t += dt;
    }
  }
  
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class myInstance
: public KODE_SynthInstance<myVoice,16> {
  
public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_SynthInstance(ADescriptor) {
  }
  
public:

  void on_sampleRate(float ASampleRate) final {
    MVoiceContext.samplerate = ASampleRate;
    initializeVoices(&MVoiceContext);
  }
  
  void on_parameterChange(uint32_t AIndex, float AValue) final {
    voiceSetParameter(AIndex,AValue);
  }
    
  void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
    voiceMidiEvent(AOffset,AMsg1,AMsg2,AMsg3);
  }
  
  void on_process(KODE_ProcessContext* AContext) final {
    processVoices(AContext,&MVoiceContext);
  }
  
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
