#ifndef s2_dsp_polyblep_included
#define s2_dsp_polyblep_included
//----------------------------------------------------------------------

//#include <math.h>
//#include "types.h"

//static
float polyblep(float t) {
  if (t > 0.0f) return t - (t*t)/2.0f - 0.5f;
  else return (t*t)/2.0f + t + 0.5f;
}

//static
float saw_polyblep(float p, float inc) {
  float mod = 0.0f;
  if (p < inc) mod = polyblep(p / inc); // start
  else if (p > (1.0f - inc)) mod = polyblep((p - 1.0f) / inc); // end
  return mod;
}

//static
float pulse_polyblep(float p, float width, float inc) {
  float mod = 0.0f;
  if (p < width) {
    if (p < inc) mod = polyblep(p / inc); // start
    else if (p > (width - inc)) mod = -polyblep( (p - width) / inc);
  } else {
    if (p > (1.0f - inc)) mod = polyblep( (p - 1.0f) / inc); // end
    else if (p < (width + inc)) mod = -polyblep( (p - width) / inc);
  }
  return mod;
}

// sync variants

//static
float saw_sync(float p, float p_, float inc, float sync) {
  float mod = 0.0f;
  if (sync >= 0.0f) mod = (p_ + inc - p) * polyblep(sync); // sync start
  else if (sync > -1.0f) mod = (p + (1.0 + sync) * inc) * polyblep(sync); // sync end
  else if (p < inc) mod = polyblep(p / inc); // start
  else if (p > (1.0f - inc)) mod = polyblep((p - 1.0f) / inc); // end
  return mod;
}

//static
float pulse_sync(float p, float p_, float width, float inc, float sync) {
  float mod = 0.0f;
  if (p < width) {
    if (p < inc) { // start
      if (sync < 0.0f || p_ >= width) mod = polyblep(p / inc);
    } else if (p > (width - inc)) {
      if (sync < -1.0f) mod = -polyblep( (p - width) / inc);
    }
  } else {
    if (p > (1.0f - inc)) mod = polyblep( (p - 1.0f) / inc); // end
    else if (p < (width + inc)) mod = -polyblep( (p - width) / inc);
    else if (sync > -1.0f) mod = polyblep(sync);
  }
  return mod;
}

//----------------------------------------------------------------------
#endif
