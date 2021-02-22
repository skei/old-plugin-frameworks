#ifndef s2_midi_timing_included
#define s2_midi_timing_included
//----------------------------------------------------------------------

//U#include "s2_memory.h"

//----------------------------------------------------------------------

#define S2_MIDI_TIMING_MAX_EVENTS 256

//----------

struct S2_MTEvent {
  uint32 ofs;
  uint32 msg1;
  uint32 msg2;
  uint32 msg3;
};

//----------------------------------------------------------------------

class S2_MidiTiming_Listener {
  public:
    virtual void on_miditiming_event(uint32 msg, uint32 chn, uint32 idx, uint32 val) {}
};

//----------------------------------------------------------------------

class S2_MidiTiming {
  private:
    uint32    MOffset;
    uint32    MCurrEvent;
    uint32    MNextEvent;
    uint32    MNumEvents;
    S2_MTEvent  MEvents[S2_MIDI_TIMING_MAX_EVENTS];
    S2_MidiTiming_Listener* MListener;

  public:

    S2_MidiTiming(S2_MidiTiming_Listener* AListener) {
      MListener   = AListener;
      MOffset     = 0;
      MCurrEvent  = 0;
      MNextEvent  = 999999;
      MNumEvents  = 0;
      S2_Memset(MEvents,0,sizeof(MEvents));
    }

    virtual ~S2_MidiTiming() {
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

    void event( uint32 ofs, uint8 msg1, uint8 msg2, uint8 msg3) {
      if (MNumEvents<S2_MIDI_TIMING_MAX_EVENTS) {
        MEvents[MNumEvents].ofs  = ofs;
        MEvents[MNumEvents].msg1 = msg1;
        MEvents[MNumEvents].msg2 = msg2;
        MEvents[MNumEvents].msg3 = msg3;
        MNumEvents++;
      }
    }

    //----------

    // call this at the start of each process (audio block)

    //virtual
    void preProcess(void) {
      MOffset = 0;
      MCurrEvent = 0;
      if (MNumEvents>0) MNextEvent = MEvents[0].ofs;
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
    uint32 process(void) {
      // events
      uint32 num = 0;
      while (MOffset==MNextEvent) {
        uint32 msg = MEvents[MCurrEvent].msg1 & 0xf0;
        uint32 chn = MEvents[MCurrEvent].msg1 & 0x0f;
        uint32 idx = MEvents[MCurrEvent].msg2 & 0x7f;
        uint32 val = MEvents[MCurrEvent].msg3 & 0x7f;
        MListener->on_miditiming_event(msg,chn,idx,val);
        MCurrEvent++;
        if (MCurrEvent<MNumEvents) MNextEvent = MEvents[MCurrEvent].ofs;
        else MNextEvent = 999999;
        num += 1;
      }
      MOffset++;
      return num;
    }

    uint32 process(uint32 ACount) {
      // events
      uint32 num = 0;
      for (uint32 i=0; i<ACount; i++) {
        while (MOffset==MNextEvent) {
          uint32 msg = MEvents[MCurrEvent].msg1 & 0xf0;
          uint32 chn = MEvents[MCurrEvent].msg1 & 0x0f;
          uint32 idx = MEvents[MCurrEvent].msg2 & 0x7f;
          uint32 val = MEvents[MCurrEvent].msg3 & 0x7f;
          MListener->on_miditiming_event(msg,chn,idx,val);
          MCurrEvent++;
          if (MCurrEvent<MNumEvents) MNextEvent = MEvents[MCurrEvent].ofs;
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
