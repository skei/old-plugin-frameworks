#ifndef s2_operator_included
#define s2_operator_included
//----------------------------------------------------------------------

/*
  hmmm...

  1. per-veoice, prepare operators for voice
  2. 'normal' voices..

  layer operator
  the whole 'normal' operator list is processed after each of them...
  they could prepare the usual set for each voice..
  copy buffer..
  write t/dt..
  etc..

  now find smart ways to utilize/exploit this to have polyphonic operators,
  aka synth voices, or miltiple layers of effetcs or whatever..
*/

#define ALIGN //__S2_ALIGNED(S2_ALIGNMENT_CACHE)

//----------------------------------------------------------------------

#define S2_OPERATOR_TICKSHIFT       4
#define S2_OPERATOR_TICKSIZE        (1<<S2_OPERATOR_TICKSHIFT)
#define S2_OPERATOR_TICKMASK        (S2_OPERATOR_TICKSIZE-1)
#define S2_OPERATOR_NUM_TICKBUFFERS 16
#define S2_OPERATOR_NUM_PARAMS      256
#define S2_OPERATOR_MAX_EVENTS      256
#define S2_OPERATOR_NUM_REGS        16
//#define S2_OPERATOR_MAX_LAYERS      16

//----------

// operator events

#define s2_oe_none      0
#define s2_oe_midi      1
#define s2_oe_param     2

// operator signals

#define s2_os_ok        0
#define s2_os_break    -1


//----------------------------------------------------------------------
//
// operator
//
//----------------------------------------------------------------------

struct S2_OpGlobal;
struct S2_OpLocal;

typedef void (*S2_Operator)(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* S2_OpData);
typedef S2_Array<S2_Operator> S2_Operators;

//----------

struct S2_OpEvent {
  uint32 offset;
  uint32 type;
  //uint32 index;
  union {
    uint8  i8[4];
    uint32 i32;
    float  f32;
  };
};

//----------


/*
  per plugin
  or, per 'context'.. oversampling, etc..
*/

struct S2_OpGlobal {
  float         sampleRate;
  uint32        blockSize;
  float         pitchBendRange;
  //float         samplePos;
  //ĸfloat        beatPos;
  ALIGN float   tickBuffer[S2_OPERATOR_TICKSIZE * S2_OPERATOR_NUM_TICKBUFFERS];
  ALIGN float   params[S2_OPERATOR_NUM_PARAMS];
  //ALIGN uint32  regs[S2_OPERATOR_NUM_REGS];
};

//----------

// per module

struct S2_OpLocal {
  //void*       owner;
  //uint32      op_index;
  int32       op_signal;
  uint32      tickSamplesRemaining;
  float*      tickOutputLeft;
  float*      tickOutputRight;
  uint32      blockOffset;
  float*      blockOutputLeft;
  float*      blockOutputRight;
  // should events be per module?
  uint32      eventOffset;
  uint32      eventOffsetNext;
  uint32      eventIndex;
  uint32      eventCount;
  S2_OpEvent  events[S2_OPERATOR_MAX_EVENTS];
};

//----------------------------------------------------------------------

#undef ALIGN

//----------------------------------------------------------------------
#endif
