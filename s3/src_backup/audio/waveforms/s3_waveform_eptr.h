#ifndef s2_waveform_eptr_included
#define s2_waveform_eptr_included
//----------------------------------------------------------------------

/*
  "Improved polynomial transition regions algorithm for alias-suppressed signal
  synthesis"
  (Dániel Ambrits an Balázs Bank)
  http://home.mit.bme.hu/~bank/publist/smc13.pdf

  based on the fact that the offset of DPW and PTR waveforms compared to the
  trivial (modulo-counter generated) waveform is due to a phase shift of the
  DPW and PTR signals. When this phase shift is removed, the linear regions of
  the waveform can be taken simply as the  trivial  waveform  values

*/

// paper also describes a dpw asymmetric triangle

//----------------------------------------------------------------------

//----------------------------------------------------------------------







//----------------------------------------------------------------------

//float SOsc_Saw_EPTR(float t, float dt) {
//  float result = t;
//  if (t > (1 - dt)) result = t - (t/dt) + (1.0f/dt) - 1.0f;
//  result -= 0.5;
//  result *= 2;
//  return result;
//}

class S2_Waveform_Saw_EPTR {
  public:
    float process(float t, float dt) {
      float result = t;
      // t = sync ? -1 : t + dt + FMBuffer[i]*FMLevel.Value;
      if (t > (1 - dt)) result = t - (t/dt) + (1.0f/dt) - 1.0f;
      result -= 0.5;
      result *= 2;
      return result;
    }
};


//----------------------------------------------------------------------
#endif

/*
  https://cycling74.com/forums/topic/please-help-extend-my-polynomial-transition-region-sawtooth-to-an-eptr-sawtooth/
  https://github.com/tebjan/VVVV.Audio/blob/master/Source/VVVV.Audio.Signals/Sources/OscSignal.cs

  http://www.kvraudio.com/forum/viewtopic.php?p=5971739#p5971739 // tonfilm

  http://home.mit.bme.hu/~bank/publist/smc13.pdf

*/

//----------

/*
  pre-computing correction polynomials for the samples in the
  transition, while the linear regions of the signal are offset
  by a constant value
*/

//----------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=5971739#p5971739
  FPhase = sync ? -1 : FPhase + t2 + FMBuffer[i]*FMLevel.Value;
  if (FPhase > 1.0f - T) //transition
  {
    sample = FPhase - (FPhase / T) + (1.0f / T) - 1.0f;
    FPhase -= 2.0f;
  }
  else
  {
    sample = FPhase;
  }
*/

//----------

/*
  EPTR Saw (efficient polynomial transition region)
  http://www.kvraudio.com/forum/viewtopic.php?p=5619182#p5619182
  float SawOut;
  phaseAccumulator = phaseAccumulator + 2.f * phaseIncrement;
  if (phaseAccumulator > 1.f - phaseIncrement)
  {
    SawOut = phaseAccumulator - (phaseAccumulator/phaseIncrement) + (1.f / phaseIncrement) - 1.f;
    phaseAccumulator = phaseAccumulator - 2.f;
  }
  else
  {
    SawOut = phaseAccumulator;
  }
  Haven't checked it against DPW but the CPU is in the same ballpark.
  ..
  I have just checked, and the frequency response is also virtually the same.
  ..
  Yeah, I though as much, thanks for testing! On my machine it has
  *slightly* better CPU, this is definetly a winner in the
  'cheap & chearful class', since it behaves better when modulating.
*/

//----------

/*
  http://home.mit.bme.hu/~bank/publist/smc13.pdf
  improved polynomial transition regions algorithm for alias-suppressed
  signal synthesis
*/
