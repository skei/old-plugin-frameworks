
// included in "test_tivks.h"

//typedef float __KODE_ALIGNED(KODE_ALIGNMENT_CACHE) KODE_NodeBuffer[KODE_AUDIO_NODE_SIZE * KODE_AUDIO_NODE_NUM_BUFFERS];
//KODE_NodeBuffer MNodeBuffer = {0};

//#include "base/kode_memory.h"

//------------------------------
private:
//------------------------------

  KODE_NoiseNode    MNoiseNode;
  KODE_MidiEvent    MEvents[KODE_MAX_NODE_EVENTS] = {0};
  uint32_t          MNumEvents                    = 0;
  uint32_t          MCurrEvent                    = 0;
  float*            MNodeBuffer                   = KODE_NULL;

//------------------------------
private:
//------------------------------

  void initialize_nodes(KODE_AudioNodeContext* AMNodeContext) {
    uint32_t size = KODE_AUDIO_NODE_SIZE * KODE_AUDIO_NODE_NUM_BUFFERS;
    MNodeBuffer = (float*)KODE_Malloc_Aligned(size,KODE_ALIGNMENT_CACHE);
    
    MNoiseNode.initialize(AMNodeContext);
    MNoiseNode.connectBuffer(0,0);
    
  }
  
  void cleanup_nodes() {
    if (MNodeBuffer) KODE_Free_Aligned(MNodeBuffer);
  }
  
    
  void node_parameter(uint32_t AIndex, float AValue) {
    MNoiseNode.parameter(AIndex,AValue);
  }
  
  void node_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {
    if (MNumEvents < KODE_MAX_NODE_EVENTS) {
      MEvents[MNumEvents].offset  = AOffset;
      MEvents[MNumEvents].msg1    = AMsg1;
      MEvents[MNumEvents].msg2    = AMsg2;
      MEvents[MNumEvents].msg3    = AMsg3;
      MNumEvents++;
    }
  }
  
  //void node_process(KODE_ProcessContext* AContext) {
  //  process_all_nodes(AContext,AContext->outputs[0]);
  //  KODE_CopyMonoBuffer(AContext->outputs[1],AContext->outputs[0],AContext->numSamples);
  //}

//------------------------------
private:
//------------------------------

  //uint32_t MPrevOffset = 0;+
  
  static
  int compare_event_offsets(const void* a, const void* b) {
    KODE_MidiEvent* ev1 = (KODE_MidiEvent*)a;
    KODE_MidiEvent* ev2 = (KODE_MidiEvent*)b;
    return (ev1->offset - ev2->offset);
  }
  

//------------------------------
private:
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
  
  /*
    0   0
    16  1..15
    32  16.31
    48
  */
  
  void process_events(uint32_t AOffset) {
    if (MCurrEvent >= MNumEvents) return;
    while (MEvents[MCurrEvent].offset <= AOffset) {
      // handle event..
      MCurrEvent++;
      if (MCurrEvent >= MNumEvents) break;
    }
  }
  
  //----------
  
  //void process_events(uint32_t AOffset, uint32_t ASize) {
  //}

  //----------
  
  void process_audio() {
    MNoiseNode.process();
  }
  
  //----------
  
  void process_audio(uint32_t ASize)  {
    MNoiseNode.process(ASize);
  }

  //----------
  
  void copy_buffer(float* ADst, float* ASrc) {
    for (uint32_t i=0; i<KODE_AUDIO_NODE_SIZE; i++) {
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
public:
//------------------------------

  void process_nodes(KODE_ProcessContext* AContext, float* AResult) {
    uint32_t remaining = AContext->numSamples;
    uint32_t offset = 0;
    preprocess_events();
    while (remaining > 0) {
      if (remaining >= KODE_AUDIO_NODE_SIZE) {
        process_events(offset);
        
        //process_audio();
//        KODE_PROCESS_NODES_FUNC
        
        copy_buffer(AResult + offset,MNodeBuffer);
        remaining -= KODE_AUDIO_NODE_SIZE;
        offset += KODE_AUDIO_NODE_SIZE;
      }
      else {
        process_events(offset/*,remaining*/);
        //process_audio(remaining);
        
//        KODE_PROCESS_NODES_VAR_FUNC
        
        copy_buffer(AResult + offset,MNodeBuffer,remaining);
        remaining = 0;
      }
    }
    postprocess_events();
  }

