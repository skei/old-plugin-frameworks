#ifndef s2_module_included
#define s2_module_included
//----------------------------------------------------------------------

#define S2_MODULE_TICKSIZE 16

//#define TICKSIZE        16

#define S2_MODULE_MAX_OVERSAMPLE  8
#define S2_MODULE_BUFFERSIZE      (S2_MODULE_TICKSIZE * S2_MODULE_MAX_OVERSAMPLE)
#define S2_MODULE_MAX_EVENTS      1024

//----------------------------------------------------------------------

template <class T>
class S2_Module {
  public:
    //virtual ~S2_Module() {}
    //static void static_func(void) { T::static_sub_func(); }
    void noteOn(uint32 ANote, uint32 AVelocity)   { static_cast<T*>(this)->on_noteOn(ANote,AVelocity);    }
    void noteOff(uint32 ANote, uint32 AVelocity)  { static_cast<T*>(this)->on_noteOff(ANote,AVelocity);   }
    void pitchBend(uint32 ABend)                  { static_cast<T*>(this)->on_pitchBend(ABend);           }
    void control(uint32 AIndex, uint32 AValue)    { static_cast<T*>(this)->on_control(AIndex,AValue);     }
    void parameter(uint32 AIndex, float AValue)   { static_cast<T*>(this)->on_parameter(AIndex,AValue);   }
    void process(float* ABuffer, uint32 ALength)  { static_cast<T*>(this)->on_process(ABuffer,ALength);   }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S2_Module_Noop
: public S2_Module<S2_Module_Noop> {
  public:
    S2_Module_Noop() {}
    void on_noteOn(uint32 ANote, uint32 AVelocity)      {}
    void on_noteOff(uint32 ANote, uint32 AVelocity)     {}
    void on_pitchBend(uint32 ABend)                     {}
    void on_control(uint32 AIndex, uint32 AValue)       {}
    void on_parameter(uint32 AIndex, float AValue)      {}
    void on_on_process(float* ABuffer, uint32 ALength)  {}
};


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class S2_Module_Osc
: public S2_Module<S2_Module_Osc> {
  //static void static_sub_func();

  private:

    float t;
    float dt;
    float v;

  public:

    S2_Module_Osc() {
      t = 0;
      dt = 0;
      v = 0;
    }

    void on_noteOn(uint32 ANote, uint32 AVelocity) {
      dt = S2_NoteToHz(ANote);
      v = AVelocity * S2_INV127F;
    }

    void on_noteOff(uint32 ANote, uint32 AVelocity) {
      v = 0;
    }

    void on_pitchBend(uint32 ABend) {
    }

    void on_control(uint32 AIndex, uint32 AValue) {
    }

    void on_parameter(uint32 AIndex, float AValue) {
    }

    void on_process(float* ABuffer, uint32 ALength) {
      for (uint32 i=0; i<ALength; i++) {
        t += dt;
        ABuffer[i] = ( (t * 2.0f) - 1.0f) * v;
      }
    }

    //void on_process(float* ABuffer) {
    //  for (uint32 i=0; i<S2_MODULE_TICKSIZE; i++) {
    //    t += dt;
    //    ABuffer[i] = ( (t * 2.0f) - 1.0f) * v;
    //  }
    //}

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#include "common/s2_memory_stdlib.h"

//----------------------------------------------------------------------

struct s2_voice_event2 {
  int32   ofs;            // 4 bytes
  uint8   msg1,msg2,msg3; // 3 bytes
  uint8   padding;        // 1 byte
};                        // = 8 bytes

//----------

template <class V, int N>
class S2_VoiceManager2
: public S2_Module<S2_VoiceManager2<V,N> > {

  private:

    V           MVoices[N];
    V*          MNoteToVoice[128];
    V*          MChannelToVoice[16];      // MPE
    uint32      MNoteToChannel[128];      // MPE
    float       MVoiceBuffer[S2_MODULE_BUFFERSIZE]; // todo: alignment (S2_ALIGNMENT_CACHE/_SIMD)
    float       MTickBuffer[S2_MODULE_BUFFERSIZE];  // todo: alignment --"--
    uint32      MOverSample;
    float       MPitchBendRange;

  protected: // events

    int32           MNumEvents;
    int32           MNextEvent;
    int32           MCurrEvent;
    s2_voice_event2 MEvents[S2_MODULE_MAX_EVENTS]; // todo: alignment (S2_ALIGNMENT_CACHE/_SIMD)

  //------------------------------
  //
  //------------------------------

  public:

    S2_VoiceManager2() {
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

    int32   numVoices(void)               { return N; }
    V*      voice(uint32 AIndex)          { return &MVoices[AIndex]; }
    float   pitchBendRange(void)          { return MPitchBendRange; }
    void    pitchBendRange(float ARange)  { MPitchBendRange = ARange; }

    //void    initialize(void)              { for (uint32 i=0; i<N; i++) MVoices[i].initialize(this,i); }
    void    sampleRate(float ARate)       { for (uint32 i=0; i<N; i++) MVoices[i].sampleRate(ARate); }
    void    blockSize(uint32 ASize)       { for (uint32 i=0; i<N; i++) MVoices[i].blockSize(ASize); }

  //------------------------------
  //
  //------------------------------

  private:

    //----------

    V* findOffVoice(void) {
      for (uint32 i=0; i<N; i++) {
        if (MVoices[i].state() == s2_vs_off) return &MVoices[i];
      }
      return S2_NULL;
    }

    //----------

    V* findReleasedVoice(void) {
      for (uint32 i=0; i<N; i++) {
        if (MVoices[i].state() == s2_vs_released) return &MVoices[i];
      }
      return S2_NULL;
    }

    //----------

    V* findVoice(void) {
      V* v = findOffVoice();
      if (!v) v = findReleasedVoice();
      return v;
    }

  //------------------------------
  // events
  //------------------------------

  private:

    void handleNoteOn(int32 ANote, int32 AVelocity, int32 AChannel) {
      V* v = findVoice();
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
      V* v = MNoteToVoice[ANote];
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
      //#ifdef S2_PLUGIN_MPE
      //VOICE* v = MChannelToVoice[AChannel];
      //if (v) v->control(AIndex,AValue,AChannel);
      //#else
      //#endif
    }

    //----------

    void handlePitchBend(int32 ABend, int32 AChannel) {
      //#ifdef S2_PLUGIN_MPE
      //VOICE* v = MChannelToVoice[AChannel];
      //if (v) v->pitchBend(ABend,AChannel);
      //#else
      for (uint32 i=0; i<N; i++) {
        MVoices[i].pitchBend(ABend,AChannel);
      }
      //#endif
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
      clearBuffer(ABuffer,S2_MODULE_TICKSIZE);
    }

    //----------

    void copyBuffer(float* ABuffer, float* ABuffer2) {
      copyBuffer(ABuffer,ABuffer2,S2_MODULE_TICKSIZE);
    }

    //----------

    void addBuffer(float* ABuffer, float* ABuffer2) {
      addBuffer(ABuffer,ABuffer2,S2_MODULE_TICKSIZE);
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
      return processEvents(AOffset,S2_MODULE_TICKSIZE);
    }

    //----------

    void processTick(int32 ASize) {
      clearBuffer(MTickBuffer,ASize);
      for (int32 i=0; i<N; i++) {
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
      processTick(S2_MODULE_TICKSIZE);
    }

  //------------------------------
  //
  //------------------------------

  public:

    void parameter(int32 AIndex, float AValue) {
      //S2_Trace("parameter %i = %.2f\n",AIndex,AValue);
      for (int32 i=0; i<N; i++) {
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
      if (MNumEvents<S2_MODULE_MAX_EVENTS) {
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
        if (remaining>=S2_MODULE_TICKSIZE) {
          processEvents(offset);
          processTick();
          copyBuffer(out0+offset,MTickBuffer);
          copyBuffer(out1+offset,MTickBuffer);
          remaining -= S2_MODULE_TICKSIZE;
          offset += S2_MODULE_TICKSIZE;
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


  //------------------------------
  //
  //------------------------------

  public:

    void on_noteOn(uint32 ANote, uint32 AVelocity) {
    }

    void on_noteOff(uint32 ANote, uint32 AVelocity) {
    }

    void on_pitchBend(uint32 ABend) {
    }

    void on_control(uint32 AIndex, uint32 AValue) {
    }

    void on_parameter(uint32 AIndex, float AValue) {
    }

    void on_process(float* ABuffer, uint32 ALength) {
    }

};


//----------------------------------------------------------------------








//----------------------------------------------------------------------
//#endif

#if 0

/*
*/

//----------------------------------------------------------------------

#include "audio/s2_operator.h"

#define ALIGN //__S2_ALIGNED(S2_ALIGNMENT_CACHE)

//----------------------------------------------------------------------

class S2_Module {

  private:

    S2_OpGlobal*        MGlobal;
    ALIGN S2_OpLocal    MLocal;
    ALIGN S2_Operators  MOperators;
    ALIGN S2_PtrArray   MOperatorData;
    ALIGN S2_Operators  MLayers;
    ALIGN S2_PtrArray   MLayerData;

  //------------------------------
  //
  //------------------------------

  public:

    S2_Module() {
      MGlobal = S2_NULL;
      S2_Memset(&MLocal,0,sizeof(MLocal));
    }

    //----------

    virtual ~S2_Module() {
    }

  //------------------------------
  // accessors
  //------------------------------

  public:

    void    global(S2_OpGlobal* AGlobal)  { MGlobal = AGlobal; }
    float*  tickBuffer(uint32 AIndex)     { return MGlobal->tickBuffer + AIndex * S2_OPERATOR_TICKSIZE; }
    float   param(uint32 AIndex)          { return MGlobal->params[AIndex]; }

  //------------------------------
  //
  //------------------------------

  public:

    virtual void init(float ASampleRate, uint32 ABlockSize) { S2_Trace("init\n"); }

  //------------------------------
  // config
  //------------------------------

  public:

    void appendOperator(S2_Operator AOperator, void* AData) {
      MOperators.append(AOperator);
      MOperatorData.append(AData);
    }

    //----------

    void setOperator(uint32 AIndex, S2_Operator AOperator, void* AData) {
      MOperators[AIndex] = AOperator;
      MOperatorData[AIndex] = AData;
    }

    //----------

    void appendLayer(S2_Operator AOperator, void* AData) {
      MLayers.append(AOperator);
      MLayerData.append(AData);
    }

    //----------

    void setLayer(uint32 AIndex, S2_Operator AOperator, void* AData) {
      MLayers[AIndex] = AOperator;
      MLayerData[AIndex] = AData;
    }

    //----------

    void connectOutputBuffer(float* ALeft, float* ARight) {
      MLocal.blockOutputLeft  = ALeft;
      MLocal.blockOutputRight = ARight;
    }

    //----------

    void connectTickBuffer(uint32 ALeftIndex, uint32 ARightIndex) {
      MLocal.tickOutputLeft  = tickBuffer(ALeftIndex);
      MLocal.tickOutputRight = tickBuffer(ARightIndex);
    }

    //----------

    void connectBuffer(uint32 ALeftIndex, uint32 ARightIndex, float* ALeft, float* ARight) {
      connectTickBuffer(ALeftIndex,ARightIndex);
      connectOutputBuffer(ALeft,ARight);
    }

  //------------------------------
  // events
  //------------------------------

  public:

    void midiEvent(uint32 AOffset, uint8 AMsg1, uint32 AMsg2, uint32 AMsg3) {
      //S2_Trace("midiEvent %i %i %i\n",AMsg1,AMsg2,AMsg3);
      if (MLocal.eventCount<S2_OPERATOR_MAX_EVENTS) {
        MLocal.events[MLocal.eventCount].offset = AOffset;
        MLocal.events[MLocal.eventCount].type   = s2_oe_midi;
        MLocal.events[MLocal.eventCount].i8[0]  = AMsg1;
        MLocal.events[MLocal.eventCount].i8[1]  = AMsg2;
        MLocal.events[MLocal.eventCount].i8[2]  = AMsg3;
        MLocal.eventCount++;
      }
    }

  //------------------------------
  // per tick
  //------------------------------

  protected:

    virtual void noteOn(uint32 ANote, uint32 AVelocity, uint32 AChannel)  { S2_Trace("noteOn\n"); }
    virtual void noteOff(uint32 ANote, uint32 AVelocity, uint32 AChannel) { S2_Trace("noteOff\n"); }
    virtual void control(uint32 AIndex, uint32 AValue, uint32 AChannel)   { S2_Trace("control\n"); }
    virtual void pitchBend(int32 ABend, uint32 AChannel)                  { S2_Trace("bend\n"); }

  protected:

    /*
      * no virtual methods from now on, if we can avoid it..
    */

    void tick_event(S2_OpEvent* event) {
      switch(event->type) {
        case s2_oe_midi:
          {
            uint32 evt = event->i8[0] >> 4;
            uint32 chn = event->i8[0] & 15;
            uint32 idx = event->i8[1];
            uint32 val = event->i8[2];
            switch(evt) {
              case s2_me_noteOn:
                if (val==0) noteOff(idx,val,chn);
                else noteOn(idx,val,chn);
                break;
              case s2_me_noteOff:
                noteOff(idx,val,chn);
                break;
              case s2_me_controlChange:
                control(idx,val,chn);
                break;
              case s2_me_pitchBend:
                int32 bend = idx + (val * 128) - 8192;
                pitchBend(bend,chn);
                break;
            }
          }
          break;
        //case soe_param:
        //  break;
      }
    }

  //------------------------------

    //TODO: rewrite this.. optimize this..
    //      don't count sample by sample..
    //      check if next event is in range (current offset -> offset+ticksize)..

    void tick_events(void) {
      for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
        while (MLocal.eventOffset==MLocal.eventOffsetNext) {
          S2_OpEvent event = MLocal.events[MLocal.eventIndex];
          tick_event(&event);
          MLocal.eventIndex++;
          if (MLocal.eventIndex<MLocal.eventCount) MLocal.eventOffsetNext = MLocal.events[MLocal.eventIndex].offset;
          else MLocal.eventOffsetNext = S2_UINT32_MAX;
        }
        MLocal.eventOffset++;
      }
    }

    //----------

    void tick_process(void) {
      //benchmark
      //for (uint32 mmm=0; mmm<5000; mmm++) {
      for (uint32 i=0; i<MOperators.size(); i++) {
        MOperators[i](MGlobal,&MLocal,MOperatorData[i] );
      }
      //}
    }

    //----------

    /*
      * called for every 16 (S2_OPERATOR_TICKSIZE) samples..
      * set op_signal = 1 to signal that we are finished,
        and the caller should stop
      * op_index = current layer operator

      for each operator in the MLayers array, call it, and then process
      all regular operators.. until op_signal==1..

    */

    void tick_layers(void) {
      MLocal.op_signal = 0;
      for (uint32 i=0; i<MLayers.size(); i++) {
        MLayers[i](MGlobal,&MLocal,MLayerData[i] ); // <-
        tick_process();
        if (MLocal.op_signal == s2_os_break) break;
      }
    }

    //----------

    void tick_copy(uint32 ALength=S2_OPERATOR_TICKSIZE, uint32 ATickOffset=0) {
      S2_Sample* dst;
      S2_Sample* src;
      if (MLocal.blockOutputLeft && MLocal.tickOutputLeft) {
        dst = MLocal.blockOutputLeft + MLocal.blockOffset;
        src = MLocal.tickOutputLeft + ATickOffset;
        S2_Memcpy(dst,src,ALength*sizeof(S2_Sample));
      }
      if (MLocal.blockOutputRight && MLocal.tickOutputRight) {
        dst = MLocal.blockOutputRight + MLocal.blockOffset;
        src = MLocal.tickOutputRight +  ATickOffset;
        S2_Memcpy(dst,src,ALength*sizeof(S2_Sample));
      }
    }

  //------------------------------
  // per block
  //------------------------------

  protected:

    uint32 block_remainder(uint32 numsamples) {
      if (MLocal.tickSamplesRemaining>0) {
        uint32 offset = S2_OPERATOR_TICKSIZE - MLocal.tickSamplesRemaining;
        if (numsamples <= MLocal.tickSamplesRemaining) {
          tick_copy(numsamples,offset);
          MLocal.blockOffset += numsamples;
          MLocal.tickSamplesRemaining -= numsamples;
          //return numsamples;
        }
        else {
          tick_copy(MLocal.tickSamplesRemaining,offset);
          MLocal.blockOffset += MLocal.tickSamplesRemaining;
          numsamples -= MLocal.tickSamplesRemaining;
          MLocal.tickSamplesRemaining = 0;
          //return numsamples;
        }
      }
      return numsamples;
    }

    //----------

    void block_ticks(uint32 ticks, uint32 post) {
      while (ticks>0) {
        tick_events();
        tick_process();
        tick_copy();
        MLocal.blockOffset += S2_OPERATOR_TICKSIZE;
        ticks--;
      }
      if (post>0) {
        tick_events();
        tick_process();
        tick_copy(post);
        MLocal.blockOffset += S2_OPERATOR_TICKSIZE;
        MLocal.tickSamplesRemaining = S2_OPERATOR_TICKSIZE - post;
      }
    }

    //----------

    void block_layers(uint32 ticks, uint32 post) {
      while (ticks>0) {
        tick_events();
        tick_layers(); // <- instead of tick_process
        tick_copy();
        MLocal.blockOffset += S2_OPERATOR_TICKSIZE;
        ticks--;
      }
      if (post>0) {
        tick_events();
        tick_layers(); // <- instead of tick_process
        tick_copy(post);
        MLocal.blockOffset += S2_OPERATOR_TICKSIZE;
        MLocal.tickSamplesRemaining = S2_OPERATOR_TICKSIZE - post;
      }
    }


  //------------------------------
  // process
  //------------------------------

  protected:

    void handle_preBlock(void) {
      MLocal.eventOffset = 0;
      MLocal.eventIndex = 0;
      if (MLocal.eventCount>0) MLocal.eventOffsetNext = MLocal.events[0].offset;
      else MLocal.eventOffsetNext = S2_UINT32_MAX;
    }

    //----------

    void handle_postBlock(void) {
      MLocal.eventCount = 0;
    }

    //----------

    void handle_Block(uint32 ALength) {

      //benchmark
      //for (uint32 mmm=0; mmm<5000; mmm++) {

      MLocal.blockOffset = 0;
      uint32 numsamples = block_remainder(ALength);
      if (numsamples>0) {
        uint32 post = numsamples & S2_OPERATOR_TICKMASK;
        numsamples -= post;
        uint32 ticks  = numsamples >> S2_OPERATOR_TICKSHIFT;
        if (MLayers.size()>0) block_layers(ticks,post);
        else block_ticks(ticks,post);
      }

      //}

    }

  //------------------------------
  //
  //------------------------------

  public:

    /*
      - the MOperators & MLayers arrays can not change during a block
        consider note on/off events (nandled per tick)
    */

    virtual
    void processBlock(S2_Sample** AInputs, S2_Sample** AOutputs, uint32 ALength) {
      handle_preBlock();
      handle_Block(ALength);
      handle_postBlock();
    }

};

//----------------------------------------------------------------------

#undef ALIGN

#endif // 0

//----------------------------------------------------------------------
#endif
