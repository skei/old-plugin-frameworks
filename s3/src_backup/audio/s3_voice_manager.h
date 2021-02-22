#ifndef s2_voice_manager_included
#define s2_voice_manager_included
//----------------------------------------------------------------------

/*
  MPE
  - chn 1     = master
  - chn 2..15 = zone
  chn = index

*/

//----------------------------------------------------------------------

#define TICKSIZE        16
#define MAX_OVERSAMPLE  8
#define BUFFERSIZE      (TICKSIZE * MAX_OVERSAMPLE)
#define MAX_EVENTS      1024

//----------------------------------------------------------------------

#include "common/s2_memory_stdlib.h"

//----------------------------------------------------------------------
//
// voice
//
//----------------------------------------------------------------------

/*
template <class VOICE, int NUMVOICES> class S2_VoiceManager;
class myVoice;
typedef S2_VoiceManager<myVoice,16> myVoiceManager;

//----------

class myVoice {
  private:
    uint32            MState;
    myVoiceManager*   MVoiceManager;
  private:
    float             MSampleRate;
    uint32            MBlockSize;
    float             MNote;
    float             MVelocity;
    int32             MChannel;
    float             MPitchBend;
  public:
    myVoice() {
      MState          = s2_vs_off;
      MVoiceManager   = S2_NULL;
      MSampleRate     = 0;
      MBlockSize      = 0;
      MNote           = 0;
      MVelocity       = 0;
      MChannel        = -1;
      MPitchBend      = 0;
    }
    ~myVoice() {
    }
  public:
    void            state(uint32 AState)                        { MState = AState; }
    uint32          state(void)                                 { return MState; }
    void            voiceManager(myVoiceManager* AVoiceManager) { MVoiceManager = AVoiceManager; }
    myVoiceManager* voiceManager(void)                          { return MVoiceManager; }
    void            sampleRate(float ARate)                     { MSampleRate = ARate; }
    void            blockSize(uint32 ASize)                     { MBlockSize = ASize; }
    int32           channel(void)                               { return MChannel; }
  public:
    void noteOn(int32 AIndex, int32 AVelocity, int32 AChannel) {
      MNote     = AIndex;
      MVelocity = AVelocity * S2_INV127F;
      MChannel  = AChannel;
    }
    void noteOff(int32 AIndex, int32 AVelocity, int32 AChannel) {}
    void control(int32 AIndex, int32 AValue, int32 AChannel) {}
    void parameter(int32 AIndex, float AValue) {}
    void pitchBend(int32 ABend, int32 AChannel) {}
    //void resetPitchBend(int32 AChannel) { MPitchBend = 0; }
    void process(S2_Sample* ABuffer) {}
    void process(S2_Sample* ABuffer, uint32 ASize) {}
};

*/

//----------------------------------------------------------------------
//
// voice manager
//
//----------------------------------------------------------------------

struct s2_voice_event {
  int32   ofs;            // 4 bytes
  uint8   msg1,msg2,msg3; // 3 bytes
  uint8   padding;        // 1 byte
};                        // = 8 bytes

//----------------------------------------------------------------------

template <class VOICE, int NUMVOICES>
class S2_VoiceManager {

  protected:

    VOICE           MVoices[NUMVOICES];
    VOICE*          MNoteToVoice[128];
    VOICE*          MChannelToVoice[16];      // MPE
    uint32          MNoteToChannel[128];      // MPE
    float           MVoiceBuffer[BUFFERSIZE]; // todo: alignment (S2_ALIGNMENT_CACHE/_SIMD)
    float           MTickBuffer[BUFFERSIZE];  // todo: alignment --"--
    uint32          MOverSample;
    float           MPitchBendRange;

  protected: // events

    int32           MNumEvents;
    int32           MNextEvent;
    int32           MCurrEvent;
    s2_voice_event  MEvents[MAX_EVENTS]; // todo: alignment (S2_ALIGNMENT_CACHE/_SIMD)

  //------------------------------
  //
  //------------------------------

  public:

    S2_VoiceManager() {

      S2_Memset(MNoteToVoice,0,sizeof(MNoteToVoice));
      S2_Memset(MNoteToChannel,0,sizeof(MNoteToChannel));
      S2_Memset(MChannelToVoice,0,sizeof(MChannelToVoice));
      S2_Memset(MEvents,0,sizeof(MEvents));
      MOverSample     = 1;
      MPitchBendRange = 2;
      MNumEvents      = 0;
      MNextEvent      = S2_INT32_MAX;
      MCurrEvent      = 0;
    }

  //------------------------------
  //
  //------------------------------

  public:

    int32   numVoices(void)               { return NUMVOICES; }
    VOICE*  voice(uint32 AIndex)          { return &MVoices[AIndex]; }
    float   pitchBendRange(void)          { return MPitchBendRange; }
    void    pitchBendRange(float ARange)  { MPitchBendRange = ARange; }

    //void    initialize(void)              { for (uint32 i=0; i<NUMVOICES; i++) MVoices[i].initialize(this,i); }
    void    sampleRate(float ARate)       { for (uint32 i=0; i<NUMVOICES; i++) MVoices[i].sampleRate(ARate); }
    void    blockSize(uint32 ASize)       { for (uint32 i=0; i<NUMVOICES; i++) MVoices[i].blockSize(ASize); }

    //----------

    VOICE* findOffVoice(void) {
      for (uint32 i=0; i<NUMVOICES; i++) {
        if (MVoices[i].state() == s2_vs_off) return &MVoices[i];
      }
      return S2_NULL;
    }

    //----------

    VOICE* findReleasedVoice(void) {
      for (uint32 i=0; i<NUMVOICES; i++) {
        if (MVoices[i].state() == s2_vs_released) return &MVoices[i];
      }
      return S2_NULL;
    }

    //----------

    VOICE* findVoice(void) {
      VOICE* v = findOffVoice();
      if (!v) v = findReleasedVoice();
      return v;
    }

  //------------------------------
  // events
  //------------------------------

  private:

    void handleNoteOn(int32 ANote, int32 AVelocity, int32 AChannel) {
      VOICE* v = findVoice();
      if (v) {
        v->noteOn(ANote,AVelocity,AChannel);
        v->state(s2_vs_playing);
        MNoteToVoice[ANote] = v;
        MNoteToChannel[ANote] = AChannel; // MPE
        MChannelToVoice[AChannel] = v;    // MPE
      }
    }

    //----------

    void handleNoteOff(int32 ANote, int32 AVelocity, int32 AChannel) {
      VOICE* v = MNoteToVoice[ANote];
      if (v) {

        //v->state(s2_vs_off);
        v->noteOff(ANote,AVelocity,AChannel);
        v->state(s2_vs_released);

        MNoteToVoice[ANote] = S2_NULL;
        MNoteToChannel[ANote] = S2_NULL;      // MPE
        MChannelToVoice[AChannel] = S2_NULL;  // MPE
      }
    }

    //----------

    void handleControl(int32 AIndex, int32 AValue, int32 AChannel) {
      //S2_Trace("control %i %i %i\n",AIndex,AValue,AChannel);
//      #ifdef S2_PLUGIN_MPE
//      VOICE* v = MChannelToVoice[AChannel];
//      if (v) v->control(AIndex,AValue,AChannel);
//      #else
//      #endif
    }

    //----------

    void handlePitchBend(int32 ABend, int32 AChannel) {
//      #ifdef S2_PLUGIN_MPE
//      VOICE* v = MChannelToVoice[AChannel];
//      if (v) v->pitchBend(ABend,AChannel);
//      #else
      for (uint32 i=0; i<NUMVOICES; i++) {
        MVoices[i].pitchBend(ABend,AChannel);
      }
//      #endif
    }

  //------------------------------
  // buffer
  //------------------------------

  private:

    void clearBuffer(float* ABuffer, uint32 ASize) {
      S2_Memset(ABuffer,0,ASize*sizeof(float));
    }

    //----------

    void copyBuffer(float* ABuffer, float* ABuffer2, uint32 ASize) {
      S2_Memcpy(ABuffer,ABuffer2,ASize*sizeof(float));
    }

    //----------

    void addBuffer(float* ABuffer, float* ABuffer2, uint32 ASize) {
      for (uint32 i=0; i<ASize; i++) {
        ABuffer[i] += ABuffer2[i];
      }
    }

    //----------

    //TODO

    //----------

    void clearBuffer(float* ABuffer) {
      clearBuffer(ABuffer,TICKSIZE);
    }

    //----------

    void copyBuffer(float* ABuffer, float* ABuffer2) {
      copyBuffer(ABuffer,ABuffer2,TICKSIZE);
    }

    //----------

    void addBuffer(float* ABuffer, float* ABuffer2) {
      addBuffer(ABuffer,ABuffer2,TICKSIZE);
    }

  //------------------------------
  // process
  //------------------------------

  private:

    void preProcessEvents(void) {
      MCurrEvent = 0;
      if (MNumEvents>0) MNextEvent = MEvents[0].ofs;
      else MNextEvent = S2_INT32_MAX;
    }

    //----------

    void postProcessEvents(void) {
      MNumEvents = 0;
    }

    //----------

    /*
      returns number of samples until next event,
      or S2_INT32_MAX if no more events
    */

    int32 processEvents(int32 AOffset, int32 ASize) {
      while (MNextEvent<(AOffset+ASize)) {
        int32 chn   = MEvents[MCurrEvent].msg1 & 0x0f;
        int32 msg   = MEvents[MCurrEvent].msg1 >> 4; // & 0xf0;
        int32 index = MEvents[MCurrEvent].msg2;
        int32 value = MEvents[MCurrEvent].msg3;
        switch(msg) {
          case 0x9:
            if (value==0) handleNoteOff(index,value,chn);
            else handleNoteOn(index,value,chn);
            break;
          case 0x8:
            handleNoteOff(index,value,chn);
            break;
          case 0xB:
            handleControl(index,value,chn);
            break;
          case 0xE:
            handlePitchBend(index+(value*128),chn);
            break;
        }
        MCurrEvent++;
        if (MCurrEvent<MNumEvents) MNextEvent = MEvents[MCurrEvent].ofs;
        else MNextEvent = S2_INT32_MAX;
      }
      int32 res = MNextEvent - AOffset;
      return res;
    }

    //----------

    // TODO

    int32 processEvents(int32 AOffset) {
      return processEvents(AOffset,TICKSIZE);
    }

    //----------

    void processTick(int32 ASize) {
      clearBuffer(MTickBuffer,ASize);
      for (int32 i=0; i<NUMVOICES; i++) {
        uint32 state = MVoices[i].state();
        if (state != s2_vs_off) {
          MVoices[i].process(MVoiceBuffer,ASize);
          addBuffer(MTickBuffer,MVoiceBuffer,ASize);
        }
      }
      // post-process per voice effects here..
    }

    //----------

    // TODO

    void processTick(void) {
      processTick(TICKSIZE);
    }

  //------------------------------
  //
  //------------------------------

  public:

    void parameter(int32 AIndex, float AValue) {
      //S2_Trace("parameter %i = %.2f\n",AIndex,AValue);
      for (int32 i=0; i<NUMVOICES; i++) {
        MVoices[i].parameter(AIndex,AValue);
      }
    }

    //----------

    /*
      - assumption: we will get midi events inbetween process() calls..
        (the offsets are relative to the start of the next block, so it
        wouldn't make sense if we received midi events async, i guess?)
      - TODO: should the offsets be 'scaled' by the oversample amount?
      - should not be called concurrently with:
        - handleEvents
        - processBlock

    */

    void midiInput(int32 ofs, uint8 msg1, uint8 msg2, uint8 msg3) {
      if (MNumEvents<MAX_EVENTS) {
        MEvents[MNumEvents].ofs  = ofs;
        MEvents[MNumEvents].msg1 = msg1;
        MEvents[MNumEvents].msg2 = msg2;
        MEvents[MNumEvents].msg3 = msg3;
        MNumEvents++;
      }
    }

    //----------

    /*
      this could just as well be used for effects too, i guess?
    */

    void processBlock(S2_Sample** AInputs, S2_Sample** AOutputs, uint32 ALength) {
      float* out0 = AOutputs[0];
      float* out1 = AOutputs[1];
      int32 offset = 0;
      int32 remaining = ALength;
      preProcessEvents();
      while (remaining>0) {
        if (remaining>=TICKSIZE) {
          processEvents(offset);
          processTick();
          copyBuffer(out0+offset,MTickBuffer);
          copyBuffer(out1+offset,MTickBuffer);
          remaining -= TICKSIZE;
          offset += TICKSIZE;
        }
        else {
          processEvents(offset,remaining);
          processTick(remaining);
          copyBuffer(out0+offset,MTickBuffer,remaining);
          copyBuffer(out1+offset,MTickBuffer,remaining);
          remaining = 0;
        }
      }
      // post-process global effects here..
      postProcessEvents();
    }

    //----------

};

//----------------------------------------------------------------------

#undef NUMVOICES
#undef TICKSIZE
#undef MAX_OVERSAMPLE
#undef BUFFERSIZE
#undef MAX_EVENTS


//----------------------------------------------------------------------
#endif
