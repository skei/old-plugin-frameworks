#ifndef s2_operator_test_included
#define s2_operator_test_included
//----------------------------------------------------------------------

#include "audio/s2_operator.h"

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

struct S2_Operator_X_Data {
};

//----------

//__S2_ALIGNED(S2_ALIGNMENT_CACHE)
void S2_Operator_X(S2_OpGlobal* AGlobal, S2_OpLocal* ALocal, void* AData) {
  //S2_Operator_X_Data* data = (S2_Operator_X_Data*)AData;
  for (uint32 i=0; i<S2_OPERATOR_TICKSIZE; i++) {
  }
}


//----------------------------------------------------------------------
#endif
