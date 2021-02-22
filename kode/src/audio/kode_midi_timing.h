#ifndef kode_midi_timing_included
#define kode_midi_timing_included
//----------------------------------------------------------------------

//#include "audio/kode_midi.h"
#include "common/kode_event.h"

//----------------------------------------------------------------------

#define KODE_MIDI_TIMING_MAX_EVENTS 256

//----------

/*
struct KMTEvent {
  uint32 ofs;
  uint32 msg1;
  uint32 msg2;
  uint32 msg3;
};
*/

//----------------------------------------------------------------------

class KODE_MidiTimingListener {
  public:
    virtual void on_miditiming_event(uint32_t msg, uint32_t chn, uint32_t idx, uint32_t val) {}
};

//----------------------------------------------------------------------

class KODE_MidiTiming {
  private:
    uint32_t                  MOffset;
    uint32_t                  MCurrEvent;
    uint32_t                  MNextEvent;
    uint32_t                  MNumEvents;
    KODE_MidiEvent            MEvents[KODE_MIDI_TIMING_MAX_EVENTS];
    KODE_MidiTimingListener*  MListener;

  public:

    KODE_MidiTiming(KODE_MidiTimingListener* AListener) {
      MListener   = AListener;
      MOffset     = 0;
      MCurrEvent  = 0;
      MNextEvent  = 999999;
      MNumEvents  = 0;
      KODE_Memset(MEvents,0,sizeof(MEvents));
    }

    virtual ~KODE_MidiTiming() {
    }

    //----------------------------------------
    //
    //----------------------------------------

    //virtual void on_event(uint32 msg, uint32 chn, uint32 idx, uint32 val) {
    //  STrace("event msg:%2x chn:%i idx:%i val:%i\n",msg,chn,idx,val);
    //}

    //----------------------------------------
    //
    //----------------------------------------

    void event( uint32_t ofs, uint8_t msg1, uint8_t msg2, uint8_t msg3) {
      if (MNumEvents<KODE_MIDI_TIMING_MAX_EVENTS) {
        MEvents[MNumEvents].offset  = ofs;
        MEvents[MNumEvents].msg1    = msg1;
        MEvents[MNumEvents].msg2    = msg2;
        MEvents[MNumEvents].msg3    = msg3;
        MNumEvents++;
      }
    }

    //----------

    // call this at the start of each process (audio block)

    //virtual
    void preProcess(void) {
      MOffset = 0;
      MCurrEvent = 0;
      if (MNumEvents>0) MNextEvent = MEvents[0].offset;
      else MNextEvent = 999999;
    }

    //----------

    /*
      call this for every sample
      todo/maybe-do:
      stuff event into struct (passed by ptr as arg) instead of calling
      virtual functions..
    */

    //----------

    //virtual
    uint32_t process(void) {
      // events
      uint32_t num = 0;
      while (MOffset==MNextEvent) {
        uint32_t msg = MEvents[MCurrEvent].msg1 & 0xf0;
        uint32_t chn = MEvents[MCurrEvent].msg1 & 0x0f;
        uint32_t idx = MEvents[MCurrEvent].msg2 & 0x7f;
        uint32_t val = MEvents[MCurrEvent].msg3 & 0x7f;
        MListener->on_miditiming_event(msg,chn,idx,val);
        MCurrEvent++;
        if (MCurrEvent<MNumEvents) MNextEvent = MEvents[MCurrEvent].offset;
        else MNextEvent = 999999;
        num += 1;
      }
      MOffset++;
      return num;
    }

    uint32_t process(uint32_t ACount) {
      // events
      uint32_t num = 0;
      for (uint32_t i=0; i<ACount; i++) {
        while (MOffset==MNextEvent) {
          uint32_t msg = MEvents[MCurrEvent].msg1 & 0xf0;
          uint32_t chn = MEvents[MCurrEvent].msg1 & 0x0f;
          uint32_t idx = MEvents[MCurrEvent].msg2 & 0x7f;
          uint32_t val = MEvents[MCurrEvent].msg3 & 0x7f;
          MListener->on_miditiming_event(msg,chn,idx,val);
          MCurrEvent++;
          if (MCurrEvent<MNumEvents) MNextEvent = MEvents[MCurrEvent].offset;
          else MNextEvent = 999999;
          num += 1;
        }
        MOffset++;
      }
      return num;
    }

    //----------

    // call this at the end of each process (audio block)
    // after all audio has been processed

    //virtual
    void postProcess(void) {
      MNumEvents = 0;
    }

};

//----------------------------------------------------------------------
#endif
