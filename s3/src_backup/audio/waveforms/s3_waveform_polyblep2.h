#ifndef s2_waveform_polyblep2_incuded
#define s2_waveform_polyblep2_included
//----------------------------------------------------------------------

// https://github.com/timowest/rogue/blob/master/dsp/polyblep.h

float S2_PolyBlep(float t) {
  if (t > 0.0f) {
    return t - (t*t)/2.0f - 0.5f;
  } else {
    return (t*t)/2.0f + t + 0.5f;
  }
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

float S2_Waveform_Saw_PolyBlep(float t, float dt) {
  float mod = 0.0f;
  if (t < dt) { // start
    mod = S2_PolyBlep(t/dt);
  } else if (t > (1.0f - dt)) { // end
    mod = S2_PolyBlep((t - 1.0f) / dt);
  }
  return mod;
}

//----------

// w = width

float S2_Waveform_Pulse_PolyBlep(float t, float dt, float w) {
  float mod = 0.0f;
  if (t < w) {
    if (t < dt) { // start
      mod = S2_PolyBlep(t / dt);
    } else if (t > (w - dt)) {
      mod = -S2_PolyBlep( (t - w) / dt);
    }
  } else {
    if (t > (1.0f - dt)) { // end
      mod = S2_PolyBlep( (t - 1.0f) / dt);
    } else if (t < (w + dt)) {
      mod = -S2_PolyBlep( (t - w) / dt);
    }
  }
  return mod;
}

//----------------------------------------------------------------------
// sync variants
//----------------------------------------------------------------------

// s = sync

float S2_Waveform_SawSync_PolyBlep(float t, float t_, float dt, float s) {
  float mod = 0.0f;
  if (s >= 0.0f) { // sync start
    mod = (t_ + dt - t) * S2_PolyBlep(s);
  } else if (s > -1.0f) { // sync end
    mod = (t + (1.0 + s) * dt) * S2_PolyBlep(s);
  } else if (t < dt) { // start
    mod = S2_PolyBlep(t / dt);
  } else if (t > (1.0f - dt)) { // end
    mod = S2_PolyBlep((t - 1.0f) / dt);
  }
  return mod;
}

//----------

float S2_Waveform_PulseSync_PolyBlep(float t, float t_, float dt, float w, float s) {
  float mod = 0.0f;
  if (t < w) {
    if (t < dt) { // start
      if (s < 0.0f || t_ >= w) {
        mod = S2_PolyBlep(t / dt);
      }
    } else if (t > (w - dt)) {
      if (s < -1.0f) {
        mod = -S2_PolyBlep( (t - w) / dt);
      }
    }
  } else {
    if (t > (1.0f - dt)) { // end
      mod = S2_PolyBlep( (t - 1.0f) / dt);
    } else if (t < (w + dt)) {
      mod = -S2_PolyBlep( (t - w) / dt);
    } else if (s > -1.0f) {
      mod = S2_PolyBlep(s);
    }
  }
  return mod;
}

//----------------------------------------------------------------------



//----------------------------------------------------------------------
#endif

