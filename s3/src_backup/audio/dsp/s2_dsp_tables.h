#ifndef s2_dsp_tables_included
#define s2_dsp_tables_included
//----------------------------------------------------------------------

//#include <stdlib.h>
//#include <math.h>
//#include "tables.h"
//#include "types.h"

//----------------------------------------------------------------------

/*
  sin table (range 0 - 1)
*/

struct sintable {
  float values[8010];
  sintable() {
    for (uint32 i = 0; i < 8010; i++) {
      values[i] = sin(S2_PI2 * float(i) / 8000.0f);
    }
  }
  float fast(float in) {
    // XXX doesn't check for boundaries
    return values[int(8000 * in) % 8000];
  }
  float linear(float in) {
    float pos = 8000.0f * in;
    float rem = pos - floor(pos);
    return rem * values[int(pos + 1)] + (1.0f - rem) * values[int(pos)];
  }
};

/*
  cos table (range 0 - 1)
*/

struct costable {
  float values[8010];
  costable() {
    for (uint32 i = 0; i < 8010; i++) {
      values[i] = cos(S2_PI2 * float(i) / 8000.0f);
    }
  }
  float fast(float in) {
    // XXX doesn't check for boundaries
    return values[int(8000 * in)];
  }
  float linear(float in) {
    float pos = 8000.0f * in;
    float rem = pos - floor(pos);
    return rem * values[int(pos + 1)] + (1.0f - rem) * values[int(pos)];
  }
};

/*
  tanh table (range -5 to 5)

*/

struct tanhtable {
  float values[16010];
  tanhtable() {
    for (uint32 i = 0; i < 16010; i++) {
      values[i] = tanh(float(int(i) - 8000) / 1600.0f);
    }
  }
  float fast(float in) {
    if (in > 5.0f) return 1.0f;
    else if (in < -5.0f) return -1.0f;
    return values[int(1600.0f * (in + 5.0f))];
  }
  float linear(float in) {
    if (in > 5.0f) return 1.0f;
    else if (in < -5.0f) return -1.0f;
    float pos = 1600.0f * (in + 5.0f);
    float rem = pos - floor(pos);
    return rem * values[int(pos + 1)] + (1.0f - rem) * values[int(pos)];
  }
};

//----------------------------------------------------------------------

//extern
sintable sin_;
//extern
costable cos_;
//extern
tanhtable tanh_;

//----------------------------------------------------------------------
#endif
