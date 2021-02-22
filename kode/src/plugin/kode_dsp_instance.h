#ifndef kode_dsp_instance_included
#define kode_dsp_instance_included
//----------------------------------------------------------------------

// TEMP

#include "base/kode.h"
#include "base/kode_event.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "audio/kode_dsp_context.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

template<class AUDIO_DSP_PROCESSOR>
class KODE_DspInstance
: public KODE_Instance {
  
//------------------------------
private:
//------------------------------

  KODE_MidiEvent      MEvents[KODE_MAX_DSP_EVENTS]  = {0};
  uint32_t            MNumEvents                    = 0;
  uint32_t            MCurrEvent                    = 0;
  
//------------------------------
protected:
//------------------------------

  KODE_DspContext     MDspContext;
  AUDIO_DSP_PROCESSOR MDspHandler;

//------------------------------
public:
//------------------------------

  KODE_DspInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }
  
  //----------

  virtual ~KODE_DspInstance() {
  }
  
//------------------------------
public:
//------------------------------

  void dspInitialize(KODE_DspContext* AContext) {
    MDspHandler.initialize(AContext);
  }

  //----------

  void dspParameter(uint32_t AIndex, float AValue) {
    MDspHandler.parameter(AIndex,AValue);
  }
  
  //----------
  
  void dspMidi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
    if (MNumEvents < KODE_MAX_DSP_EVENTS) {
      MEvents[MNumEvents].offset  = AOffset;
      MEvents[MNumEvents].msg1    = AMsg1;
      MEvents[MNumEvents].msg2    = AMsg2;
      MEvents[MNumEvents].msg3    = AMsg3;
      MNumEvents++;
    }
  }
  
  //----------
  
  void dspProcess(float* AResult, uint32_t ABuffer) {
    uint32_t remaining = MDspContext.numSamples;
    float* dspbuffer = MDspContext.dspBuffer + (ABuffer * KODE_DSP_SIZE);
    preprocess_events();
    uint32_t offset = 0;
    while (remaining > 0) {
      if (remaining >= KODE_DSP_SIZE) {
        process_events(offset);
        MDspHandler.process();
        copy_buffer(AResult+offset,dspbuffer);
        remaining -= KODE_DSP_SIZE;
        offset += KODE_DSP_SIZE;
      }
      else {
        process_events(offset/*,remaining*/);
        MDspHandler.process(remaining);
        copy_buffer(AResult+offset,dspbuffer,remaining);
        remaining = 0;
      }
    }
    // catch last events..
    // also, if host is updating parameters and 0 samples buffer
    process_events(MDspContext.numSamples);
    postprocess_events();
  }
  
//------------------------------
private: // audio
//------------------------------

  void copy_buffer(float* ADst, float* ASrc) {
    for (uint32_t i=0; i<KODE_DSP_SIZE; i++) {
      *ADst++ = *ASrc++;
    }
    
  }
  
  //----------
  
  void copy_buffer(float* ADst, float* ASrc, uint32_t ASize) {
    for (uint32_t i=0; i<ASize; i++) {
      *ADst++ = *ASrc++;
    }
  }
  
  //----------
  
  //void add_buffer(float* ADst, float* ASrc) {
  //  for (uint32_t i=0; i<KODE_AUDIO_TICKSIZE; i++) *ADst++ += *ASrc++;
  //}
  
  //----------
  
  //void add_buffer(float* ADst, float* ASrc, uint32_t ASize) {
  //  for (uint32_t i=0; i<ASize; i++) *ADst++ += *ASrc++;
  //}

//------------------------------
private: // events
//------------------------------

  static
  int compare_event_offsets(const void* a, const void* b) {
    KODE_MidiEvent* ev1 = (KODE_MidiEvent*)a;
    KODE_MidiEvent* ev2 = (KODE_MidiEvent*)b;
    return (ev1->offset - ev2->offset);
  }
  
  //------------------------------

  void preprocess_events() {
    MCurrEvent = 0;
    if (MNumEvents > 0) {
      qsort(MEvents, MNumEvents, sizeof(KODE_MidiEvent), compare_event_offsets);
    }
  }
  
  //----------
  
  void postprocess_events() {
    MNumEvents = 0;
  }
  
  //----------
  
  void process_events(uint32_t AOffset) {
    if (MCurrEvent >= MNumEvents) return;
    uint32_t offset = MEvents[MCurrEvent].offset;
    while (offset <= AOffset) {
      
      uint32_t msg1 = MEvents[MCurrEvent].msg1;
      uint32_t msg2 = MEvents[MCurrEvent].msg2;
      uint32_t msg3 = MEvents[MCurrEvent].msg3;
      
      MDspHandler.midi(msg1,msg2,msg3);

      uint32_t chn    = msg1 & 0x0f;
      uint32_t msg    = msg1 & 0xf0;
      uint32_t note   = msg2;
      uint32_t vel    = msg3;
      uint32_t index  = note;
      uint32_t value  = vel;

      switch(msg) {
        case 0x80:
          handle_noteOff(chn,note,vel);
          break;
        case 0x90:
          if (vel==0) handle_noteOff(chn,note,vel);
          else handle_noteOn(chn,note,vel);
          break;
        case 0xA0:
          handle_polyAftertouch(chn,note,value);
          break;
        case 0xB0:
          handle_control(chn,index,value);
          break;
        case 0xC0:
          handle_programChange(chn,index);
          break;
        case 0xD0:
          handle_channelPressure(chn,index);
          break;
        case 0xE0:
          value = note + ( vel * 128 );
          handle_pitchBend(chn,value);
          break;
      }
      
      MCurrEvent++;
      if (MCurrEvent >= MNumEvents) return;
      offset = MEvents[MCurrEvent].offset;
    }
  }
  
//------------------------------
private: // handle events
//------------------------------

  void handle_noteOn(uint32_t AChan, uint32_t ANote, uint32_t AVel) {
    //float vel = (float)AVel * KODE_INV127F;
    //VOICE_* V = findFreeVoice();
    //if (V) {
    //  V->setNote(ANote);
    //  V->setVelocity(AVel);
    //  V->setChannel(AChan);
    //  MNoteMap[AChan*128 + ANote] = V;
    //  uint32_t state = V->on_noteOn(ANote,vel);
    //  V->setState(state);
    //}
  }

  //----------

  void handle_noteOff(uint32_t AChan, uint32_t ANote, uint32_t AVel) {
    //float vel = (float)AVel * KODE_INV127F;
    //VOICE_* V = MNoteMap[AChan*128 + ANote];
    //if (V) {
    //  uint32_t state = V->on_noteOff(ANote,vel);
    //  V->setState(state);
    //  MNoteMap[AChan*128 + ANote] = KODE_NULL;
    //}
  }

  //----------
  
  void handle_polyAftertouch(uint32_t chn, uint32_t note, uint32_t value) {
    //KODE_Trace("chn %i note %i value %i\n",chn,note,value);
    //float v = (float)value * KODE_INV127F;
    //for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //  if (MVoices[i].getChannel() == chn) {
    //    // if playing or releasing?
    //    MVoices[i].on_pressure(v);
    //  }
    //}
  }
  
  //----------

  void handle_programChange(uint32_t chn, uint32_t value) {
    //KODE_Trace("chn %i value %i\n",chn,value);
  }
  
  //----------

  void handle_channelPressure(uint32_t chn, uint32_t value) {
    //float v = (float)value * KODE_INV127F;
    //if (chn == 0) {
    //  for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //    MVoices[i].on_masterPressure(v);
    //  }
    //}
    //else {
    //  for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //    if (MVoices[i].getChannel() == chn) {
    //      // if playing or releasing?
    //      MVoices[i].on_pressure(v);
    //    }
    //  }
    //}
  }
  
  //----------

  /*
    TODO: don't send all controls (cc changes) to all voices..
    let the voices have a ptr to a struct with cc values or something..
    
    chan 0 = master
    
    nanokey2 sends 'mod' out on channel 1
  */

  void handle_control(uint32_t AChan, uint32_t AIndex, uint32_t AValue) {
    //float v = (float)AValue * KODE_INV127F;
    //if (AChan == 0) {
    //  for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //    if (AIndex == 74) MVoices[i].on_masterBrightness(v);
    //    else MVoices[i].on_masterControl(AIndex,v);
    //  }
    //}
    //else {
    //  for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //    if (MVoices[i].getChannel() == AChan) {
    //      // if playing or releasing?
    //      if (AIndex == 74) MVoices[i].on_brightness(v);
    //      else MVoices[i].on_control(AIndex,v);
    //    }
    //  }
    //}
  }

  //----------

  /*
    chan 0 = master channel
  */
  
  void handle_pitchBend(uint32_t AChan, uint32_t APitchBend) {
    //float pb = APitchBend;
    //pb -= 8192.0f;
    //pb *= KODE_INV8192F;
    //if (AChan == 0) {
    //  for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //    MVoices[i].on_masterBend(pb);
    //  }
    //}
    //else {
    //  for (uint32_t i=0; i<NUM_VOICES_; i++) {
    //    if (MVoices[i].getChannel() == AChan) {
    //      // if playing or releasing?
    //      MVoices[i].on_pitchBend(pb);
    //    }
    //  }
    //}
  }

};

//----------------------------------------------------------------------
#endif