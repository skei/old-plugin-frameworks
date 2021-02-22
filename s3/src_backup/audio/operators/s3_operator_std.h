#ifndef s2_operator_std_included
#define s2_operator_std_included
//----------------------------------------------------------------------

#include "audio/s2_operator.h"

/*
#define S2_OP_STD_ADD       0
#define S2_OP_STD_CLEAR     0
#define S2_OP_STD_CONTAINER 0
#define S2_OP_STD_COPY      0
#define S2_OP_STD_LERP      0
#define S2_OP_STD_MUL       0
#define S2_OP_STD_NOOP      0
#define S2_OP_STD_PHASE     0
#define S2_OP_STD_RANDOM    0
#define S2_OP_STD_RESULT    0
#define S2_OP_STD_VALUE     0
*/

//----------------------------------------------------------------------
//
// add
//
//----------------------------------------------------------------------

struct S2_Operator_Add_Data {
  float*  dst;
  float*  add;
  //SVoice* first;
  //SVoice* cur;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Add(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Add_Data* data = (S2_Operator_Add_Data*)AData;
  float* dst = data->dst;
  float* add = data->add;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
    *dst++ += *add++;
  }
}

//----------------------------------------------------------------------
//
// clear
//
//----------------------------------------------------------------------

struct S2_Operator_Clear_Data {
  float*  out;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Clear(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Clear_Data* data = (S2_Operator_Clear_Data*)AData;
  float* out = data->out;
  S2_Memset(out,0,S2_OPERATOR_TICKSIZE*S2_SampleSize);
}

//----------------------------------------------------------------------
//
// container
//
//----------------------------------------------------------------------

struct S2_Operator_Container_Data {
  S2_Operators  operators;
  S2_PtrArray   operatorData;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Container(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Container_Data* data = (S2_Operator_Container_Data*)AData;
  for (uint32 i=0; i<data->operators.size(); i++) {
    data->operators[i](AGlobal,ALocal,data->operatorData[i] );
  }
}

//----------

//void S2_OpContainer_Init(S2_Operator_Container_Data* op_container) {
//}

void S2_OpContainer_Append(S2_Operator_Container_Data* op_container,S2_Operator op,void* val) {
  op_container->operators.append(op);
  op_container->operatorData.append(val);
}


//----------------------------------------------------------------------
//
// copy
//
//----------------------------------------------------------------------

struct S2_Operator_Copy_Data {
  float* dst;
  float* src;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Copy(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Copy_Data* data = (S2_Operator_Copy_Data*)AData;
  float* dst = data->dst;
  float* src = data->src;
  S2_Memcpy(dst,src,S2_OPERATOR_TICKSIZE*S2_SampleSize);
}

//----------------------------------------------------------------------
//
// lerp val
//
//----------------------------------------------------------------------

#include "common/s2_interpolation.h"

//----------

struct S2_Operator_Lerp_Data {
  float   start;
  float   end;
  float*  out;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Lerp(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Lerp_Data* data = (S2_Operator_Lerp_Data*)AData;
  float start = data->start;
  float end = data->end;
  float* out = data->out;
  float t = 0;
  float dt = S2_INV16F;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
    *out++ = S2_Interpolate_Linear(t,start,end);
    t += dt;
  }
}

//----------------------------------------------------------------------
//
// mul
//
//----------------------------------------------------------------------

struct S2_Operator_Mul_Data {
  float*  dst;
  float*  mul;
  //SVoice* first;
  //SVoice* cur;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Mul(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Mul_Data* data = (S2_Operator_Mul_Data*)AData;
  float* dst = data->dst;
  float* mul = data->mul;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
    *dst++ *= *mul++;
  }
}

//----------------------------------------------------------------------
//
// noop
//
//----------------------------------------------------------------------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Noop(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  //DTrace("."); // yes, this is being called..
}

//----------------------------------------------------------------------
//
// phase
//
//----------------------------------------------------------------------

struct S2_Operator_Phase_Data {
  float   t;
  float   dt;
  float*  out_t;
  float*  out_dt;
  //SVoice* first;
  //SVoice* cur;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Phase(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Phase_Data* data = (S2_Operator_Phase_Data*)AData;
  float t       = data->t;
  float dt      = data->dt;
  float* out_t  = data->out_t;
  float* out_dt = data->out_dt;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
    *out_t++ = t;
    *out_dt++ = dt;
    t += dt;
    if (t>=1.0f) t -= 1.0f;
  }
}

//----------------------------------------------------------------------
//
// random (slow)
//
//----------------------------------------------------------------------

#include "common/s2_random.h"

//----------

struct S2_Operator_Random_Data {
  float*  out;
};

//----------

__S2_UNUSED
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
static void S2_Operator_Random(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Random_Data* data = (S2_Operator_Random_Data*)AData;
  float* out = data->out;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
    *out++ = S2_RandomSigned();
  }
}

//----------------------------------------------------------------------
//
// result
//
//----------------------------------------------------------------------

struct S2_Operator_Signal_Data {
  int32 signal;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Signal(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  S2_Operator_Signal_Data* data = (S2_Operator_Signal_Data*)AData;
  int32 signal = data->signal;
  ALocal->op_signal = signal;
}

//----------------------------------------------------------------------
//
// value
//
//----------------------------------------------------------------------

struct S2_Operator_Value_Data {
  float*  out;
  float   value;
};

//----------

//__S2_UNUSED
//static
//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_Value(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  //DTrace(":");
  S2_Operator_Value_Data* data = (S2_Operator_Value_Data*)AData;
  float* out = data->out;
  float value = data->value;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
    *out++ = value;
  }
}

//----------------------------------------------------------------------
#endif

