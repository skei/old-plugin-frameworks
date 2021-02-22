#ifndef kode_voice_manager_included
#define kode_voice_manager_included
//----------------------------------------------------------------------

///*
//  TODO / MAYBE-DO:
//
//  * voices should process blocks to internal buffer first, then these
//    should be added to the final output buffer..
//
//  * uint32_t numParameterUpdates..
//    setParameter increases it.
//    check in start of process, if > 0, loop through parameters, check
//    for changes.. end of process, set back to 0..
//    (assumes setParameter is not called concurrently with process)
//
//  * multi timbral
//
//*/
//
////----------------------------------------------------------------------
//
//#define KODE_VOICE_MANAGER_MAX_EVENTS 256
//
//#define KODE_VOICE_FREE     0
//#define KODE_VOICE_PLAYING  1
//#define KODE_VOICE_RELEASED 2
//#define KODE_VOICE_FINISHED 3
//
////----------------------------------------------------------------------
//
////#include "audio/kode_midi.h"
//
//#include "base/kode_event.h"
//#include "base/kode_list.h"
//#include "base/kode_math_int.h"
//#include "audio/kode_audio_utils.h"
//#include "audio/kode_voice_context.h"
//#include "plugin/kode_process_context.h"
//
////----------------------------------------------------------------------
//// voice manager
////----------------------------------------------------------------------
//
//template<class VOICE_, int NUM_VOICES_>
//class KODE_VoiceManager {
//
//  private: // events
//
//    uint32_t              MNextEvent    = 0;
//    uint32_t              MCurrEvent    = 0;
//    uint32_t              MNumEvents    = 0;
//    uint32_t              MNumPlaying   = 0;
//    uint32_t              MNumReleased  = 0;
//    KODE_MidiEvent        MEvents[KODE_VOICE_MANAGER_MAX_EVENTS] = {0};
//    
//  protected: // voices
//
//    VOICE_                MVoices[NUM_VOICES_];     // all voices
//    VOICE_*               MNoteMap[128*16] = {0};   // currently playing notes
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//    KODE_VoiceManager() {
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        MVoices[i].setState(KODE_VOICE_FREE);
//      }
//      KODE_Memset(MNoteMap,0,sizeof(MNoteMap));
//      MCurrEvent  = 0;
//      MNextEvent  = KODE_INT32_MAX;
//      MNumEvents  = 0;
//      KODE_Memset(MEvents,0,sizeof(MEvents));
//      
//    }
//
//    //----------
//
//    virtual ~KODE_VoiceManager() {
//    }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//    int32_t getNumPlayingVoices(void)   { return MNumPlaying; }
//    int32_t getNumReleasedVoices(void)  { return MNumReleased; }
//    
//    VOICE_* getVoice(uint32_t i) {
//      return &( MVoices[i] );
//    }
//
//    //----------
//    
//    void initialize(KODE_VoiceContext* AVoiceContext) {
//      //AVoiceContext->owner = this;
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        MVoices[i].on_initialize(AVoiceContext);
//      }
//    }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  private:
//
//    // ...
//    // voices are apended to tail, so head is oldest
//    // we could add some kind of 'level' assigned to each voice,
//    // and select the weakest one..
//
//    VOICE_* findFreeVoice() {
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        if (MVoices[i].getState() == KODE_VOICE_FREE) {
//          return &MVoices[i];
//        }
//      }
//      // fouind no free voices.. try released voices..
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        if (MVoices[i].getState() == KODE_VOICE_RELEASED) {
//          return &MVoices[i];
//        }
//      }
//      return KODE_NULL;
//    }
//
//  //----------------------------------------
//  // incoming events
//  //----------------------------------------
//
//  public:
//
//    /*
//      assumption:
//      - we will get midi events inbetween process() calls..
//      - the offsets are relative to the start of the next block..
//      - wouldn't make sense if we received midi events async, would it?..
//    */
//
//    //TODO: offset should probably be scaled if oversampling?
//
//    void midiEvent(int32_t ofs, uint8_t msg1, uint8_t msg2, uint8_t msg3) {
//      if (MNumEvents < KODE_VOICE_MANAGER_MAX_EVENTS) {
//        MEvents[MNumEvents].offset  = ofs;
//        MEvents[MNumEvents].msg1    = msg1;
//        MEvents[MNumEvents].msg2    = msg2;
//        MEvents[MNumEvents].msg3    = msg3;
//        MNumEvents++;
//      }
//    }
//
//    //----------
//
//    void setParameter(int32_t AIndex, float AValue) {
//      handle_parameter(AIndex,AValue);
//    }
//
//  //----------------------------------------
//  // handle events
//  //----------------------------------------
//
//  private:
//
//    void handle_noteOn(uint32_t AChan, uint32_t ANote, uint32_t AVel) {
//      float vel = (float)AVel * KODE_INV127F;
//      VOICE_* V = findFreeVoice();
//      if (V) {
//        V->setNote(ANote);
//        V->setVelocity(AVel);
//        V->setChannel(AChan);
//        MNoteMap[AChan*128 + ANote] = V;
//        uint32_t state = V->on_noteOn(ANote,vel);
//        V->setState(state);
//      }
//    }
//
//    //----------
//
//    void handle_noteOff(uint32_t AChan, uint32_t ANote, uint32_t AVel) {
//      float vel = (float)AVel * KODE_INV127F;
//      VOICE_* V = MNoteMap[AChan*128 + ANote];
//      if (V) {
//        uint32_t state = V->on_noteOff(ANote,vel);
//        V->setState(state);
////        V->setNote(-1);
////        V->setChannel(-1);
//        MNoteMap[AChan*128 + ANote] = KODE_NULL;
//      }
//    }
//
//    //----------
//    
//    void handle_polyAftertouch(uint32_t chn, uint32_t note, uint32_t value) {
//      //KODE_Trace("chn %i note %i value %i\n",chn,note,value);
//      //float v = (float)value * KODE_INV127F;
//      //for (uint32_t i=0; i<NUM_VOICES_; i++) {
//      //  if (MVoices[i].getChannel() == chn) {
//      //    // if playing or releasing?
//      //    MVoices[i].on_pressure(v);
//      //  }
//      //}
//    }
//    
//    //----------
//
//    void handle_programChange(uint32_t chn, uint32_t value) {
//      //KODE_Trace("chn %i value %i\n",chn,value);
//    }
//    
//    //----------
//
//    void handle_channelPressure(uint32_t chn, uint32_t value) {
//      float v = (float)value * KODE_INV127F;
//      //KODE_Trace("chn %i value %i\n",chn,value);
//      if (chn == 0) {
//        for (uint32_t i=0; i<NUM_VOICES_; i++) {
//          MVoices[i].on_masterPressure(v);
//        }
//      }
//      else {
//        for (uint32_t i=0; i<NUM_VOICES_; i++) {
//          if (MVoices[i].getChannel() == chn) {
//            // if playing or releasing?
//            MVoices[i].on_pressure(v);
//          }
//        }
//      }
//    }
//    
//    //----------
//
//    /*
//      TODO: don't send all controls (cc changes) to all voices..
//      let the voices have a ptr to a struct with cc values or something..
//      
//      chan 0 = master
//      
//      nanokey2 sends 'mod' out on channel 1
//    */
//
//    void handle_control(uint32_t AChan, uint32_t AIndex, uint32_t AValue) {
//      //KODE_Trace("AChan %i AIndex %i AValue %i\n",AChan,AIndex,AValue);
//      float v = (float)AValue * KODE_INV127F;
//      if (AChan == 0) {
//        for (uint32_t i=0; i<NUM_VOICES_; i++) {
//          if (AIndex == 74) MVoices[i].on_masterBrightness(v);
//          else MVoices[i].on_masterControl(AIndex,v);
//        }
//      }
//      else {
//        for (uint32_t i=0; i<NUM_VOICES_; i++) {
//          if (MVoices[i].getChannel() == AChan) {
//            // if playing or releasing?
//            if (AIndex == 74) MVoices[i].on_brightness(v);
//            else MVoices[i].on_control(AIndex,v);
//          }
//        }
//      }
//    }
//
//    //----------
//
//    /*
//      chan 0 = master channel
//    */
//    
//    void handle_pitchBend(uint32_t AChan, uint32_t APitchBend) {
//      float pb = APitchBend;
//      pb -= 8192.0f;
//      pb *= KODE_INV8192F;
//      if (AChan == 0) {
//        for (uint32_t i=0; i<NUM_VOICES_; i++) {
//          MVoices[i].on_masterBend(pb);
//        }
//      }
//      else {
//        for (uint32_t i=0; i<NUM_VOICES_; i++) {
//          if (MVoices[i].getChannel() == AChan) {
//            // if playing or releasing?
//            MVoices[i].on_pitchBend(pb);
//          }
//        }
//      }
//    }
//
//    //----------
//    //----------
//
//    /*
//      TODO: don't send all parameters to all voices..
//      -> KODE_VoiceContext.parameters
//    */
//
//    void handle_parameter(uint32_t AIndex, float AValue) {
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        MVoices[i].on_parameter(AIndex,AValue);
//      }
//    }
//
//  //----------------------------------------
//  // per sample
//  //----------------------------------------
//
//  private:
//
//    /*
//      returns number of samples until next event,
//      or KODE_INT32_MAX if no more events
//    */
//
//    int32_t process_events(uint32_t AOffset) {
//      while (MNextEvent == AOffset) {
//        uint32_t chn    = MEvents[MCurrEvent].msg1 & 0x0f;
//        uint32_t msg    = MEvents[MCurrEvent].msg1 & 0xf0;
//        uint32_t note   = MEvents[MCurrEvent].msg2;
//        uint32_t vel    = MEvents[MCurrEvent].msg3;
//        uint32_t index  = note;
//        uint32_t value  = vel;
//        switch(msg) {
//          case 0x80:
//            handle_noteOff(chn,note,vel);
//            break;
//          case 0x90:
//            if (vel==0) handle_noteOff(chn,note,vel);
//            else handle_noteOn(chn,note,vel);
//            break;
//          case 0xA0:
//            // 1010nnnn 0kkkkkkk 0vvvvvvv
//            // Polyphonic Key Pressure (Aftertouch).
//            // This message is most often sent by pressing down on the key
//            // after it "bottoms out".
//            // (kkkkkkk) is the key (note) number.
//            // (vvvvvvv) is the pressure valu            
//            handle_polyAftertouch(chn,note,value);
//            break;
//          case 0xB0:
//            handle_control(chn,index,value);
//            break;
//          case 0xC0:
//            // 1100nnnn 0ppppppp
//            // Program Change.
//            // This message sent when the patch number changes.
//            // (ppppppp) is the new program number.
//            handle_programChange(chn,index);
//            break;
//          case 0xD0:
//            // 1101nnnn 0vvvvvvv
//            // Channel Pressure (After-touch).
//            // This message is most often sent by pressing down on the key
//            // after it "bottoms out". This message is different from
//            // polyphonic after-touch. Use this message to send the single
//            // greatest pressure value (of all the current depressed keys).
//            // (vvvvvvv) is the pressure value.          
//            // handle_channelPressure()
//            handle_channelPressure(chn,index);
//            break;
//          case 0xE0:
//            value = note + ( vel * 128 );
//            handle_pitchBend(chn,value);
//            break;
//        } // switch msg
//        MCurrEvent++;
//        if (MCurrEvent < MNumEvents) MNextEvent = MEvents[MCurrEvent].offset;
//        else MNextEvent = KODE_INT32_MAX;
//      } // next event = offset
//      return (MNextEvent - AOffset);
//    }
//
//    //----------
//
//    void process_playingVoices() {
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        if (MVoices[i].getState() == KODE_VOICE_PLAYING) {
//          MVoices[i].on_process();
//        }
//      }
//    }
//
//    //----------
//
//    void process_releasedVoices() {
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        if (MVoices[i].getState() == KODE_VOICE_RELEASED) {
//          MVoices[i].on_process();
//        }
//      }
//    }
//
//    //----------
//    
//    //void process_modulation(KODE_VoiceContext* AContext) {
//    //}
//
//  //----------------------------------------
//  // per block
//  //----------------------------------------
//  
//
////------------------------------
//private:
////------------------------------
//  
//    static
//    int compare_event_offsets(const void* a, const void* b) {
//      KODE_MidiEvent* ev1 = (KODE_MidiEvent*)a;
//      KODE_MidiEvent* ev2 = (KODE_MidiEvent*)b;
//      return (ev1->offset - ev2->offset);
//    }
//    
//    //----------
//    
//    /*
//      in bitwig, it looks like events are sorted first by channel, then offset.
//      and so, might create havoc with mpe..
//      so we qsort events..
//    */
//
//    void pre_process(void) {
//      MCurrEvent = 0;
//      if (MNumEvents > 0) {
//        qsort(MEvents, MNumEvents, sizeof(KODE_MidiEvent), compare_event_offsets);
//        MNextEvent = MEvents[0].offset;
//      }
//      else {
//        MNextEvent = KODE_INT32_MAX;
//      }
//    }
//
//    //----------
//
//    void post_process(void) {
//      MNumPlaying   = 0;
//      MNumReleased  = 0;
//      for (uint32_t i=0; i<NUM_VOICES_; i++) {
//        switch( MVoices[i].getState() ) {
//          case KODE_VOICE_PLAYING:
//            MNumPlaying++;
//            break;
//          case KODE_VOICE_RELEASED:
//            MNumReleased++;
//            break;
//          case KODE_VOICE_FINISHED:
//            MVoices[i].setState(KODE_VOICE_FREE);
//            break;
//        }
//      }
//      MNumEvents = 0;
//    }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  /*
//    TODO:
//      voices process into a buffer first, so that we can post-process
//      them with effects, etc..
//      and then add them to the 'main' buffers..
//  */
//
//  public:
//
//    void process(KODE_ProcessContext* AProcessContext, KODE_VoiceContext* AVoiceContext) {
//      pre_process();
//      AVoiceContext->samplerate = AProcessContext->samplerate;
//      AVoiceContext->inputs[0]  = AProcessContext->inputs[0];
//      AVoiceContext->inputs[1]  = AProcessContext->inputs[1];
//      AVoiceContext->outputs[0] = AProcessContext->outputs[0];
//      AVoiceContext->outputs[1] = AProcessContext->outputs[1];
//      uint32_t offset     = 0;
//      uint32_t remaining  = AProcessContext->numSamples;
//      KODE_ClearStereoBuffer( AVoiceContext->outputs, AProcessContext->numSamples );
//      while (offset < AProcessContext->numSamples) {
//        uint32_t samples_until_next_event = process_events(offset);
//        uint32_t samples_to_process = KODE_MinI(remaining,samples_until_next_event);
//        AVoiceContext->numsamples  = samples_to_process;
//        
//        //process_modulation(AVoiceContext);
//        
//        process_playingVoices();
//        process_releasedVoices();
//        offset += samples_to_process;
//        AVoiceContext->inputs[0]  += samples_to_process;
//        AVoiceContext->inputs[1]  += samples_to_process;
//        AVoiceContext->outputs[0] += samples_to_process;
//        AVoiceContext->outputs[1] += samples_to_process;
//        remaining -= samples_to_process;
//      }
//      post_process();
//    }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//};

//----------------------------------------------------------------------
#endif
