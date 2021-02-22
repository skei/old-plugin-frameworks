#ifndef s3_vector_included
#define s3_vector_included
//----------------------------------------------------------------------

#include <math.h>
#include "common/s3_math.h"

template <typename T>
struct S3_Vec2 {

  T x,y;

  S3_Vec2() {
  }

  S3_Vec2(T av) {
    set(av);
  }

  S3_Vec2(T ax, T ay) {
    set(ax,ay);
  }

  void set(T av) {
    x = av;
    y = av;
  }

  void set(T ax, T ay) {
    x = ax;
    y = ay;
  }

  // length
  // normalize
  // dot
  // cross

  // operators

};

//----------------------------------------------------------------------
#endif
