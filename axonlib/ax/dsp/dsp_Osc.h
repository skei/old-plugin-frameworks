/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

/**
  \file dsp_Osc.h
  \brief primitive multi oscillator
*/

#ifndef dspOsc0_included
#define dspOsc0_included
//----------------------------------------------------------------------

#include "core/axRand.h"

// osc types
#define osc_None    0
#define osc_Const   1
#define osc_Input   2
#define osc_Noise   3
#define osc_Ramp    4
#define osc_Saw     5
#define osc_Squ     6
#define osc_Sin     7

//TODO: oty_Pulse, oty_Tri

//----------------------------------------------------------------------

class dspOsc0
{
  private:
    int     mType;
    float   mFreq;
    float   mPhase;
    float   iRate;
    axRand  rand;

  public:
    dspOsc0()
    {
    }
    void setup(int aType=0, float aFreq=0, float aPhase=0)
    {
      mType = aType;
      mFreq = aFreq;
      mPhase = aPhase;
    }

    inline void setType(int aType) { mType = aType; }
    inline void setFreq(float aFreq) { mFreq = aFreq; } // 440 * pow(2.0,(_note-69.0) / 12);
    inline void setPhase( float aPhase) { mPhase = aPhase; }
    inline void setSampleRate(float aRate) { iRate = 1/aRate; }

    virtual float process(float in=0)
    {
      float out = 0; // osc_None
      switch(mType)
      {
        case osc_Const: out = 1; break;
        case osc_Input: out = in; break;
        case osc_Noise: out = rand.randSigned(); break;
        case osc_Ramp:  out = (mPhase*2)-1; break;
        case osc_Saw:   out = 1-(mPhase*2); break;
        case osc_Squ:   if (mPhase<0.5) out=1; else out=-1; break;
        case osc_Sin:   out = axSin(mPhase*PI2); break;
      }
      mPhase += mFreq * iRate;
      if (mPhase>=1) mPhase-=1;
      if (mPhase<0) mPhase+=1;
      //while (mPhase>=1) mPhase-=1;
      //while (mPhase<0) mPhase+=1;
      return out;
    }

    virtual float process_pd(float in=0, float pd=0)
    {
      float p2 = mPhase + pd;
      if (p2>=1) p2-=1;
      if (p2<0) p2+=1;
      float out = 0; // osc_None
      switch(mType)
      {
        case osc_Const: out = 1; break;
        case osc_Input: out = in; break;
        case osc_Noise: out = rand.randSigned(); break;
        case osc_Ramp:  out = (p2*2)-1; break;
        case osc_Saw:   out = 1-(p2*2); break;
        case osc_Squ:   if (p2<0.5) out=1; else out=-1; break;
        case osc_Sin:   out = axSin(p2*PI2); break;
      }
      mPhase += mFreq * iRate;
      if (mPhase>=1) mPhase-=1;
      if (mPhase<0) mPhase+=1;
      //while (mPhase>=1) mPhase-=1;
      //while (mPhase<0) mPhase+=1;
      return out;
    }

};

//----------------------------------------------------------------------
#endif
