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
  \file dspRC.h
  \brief RC filter
*/

#ifndef dspRC_included
#define dspRC_included
//----------------------------------------------------------------------

class dspRC
{
  private:
    float mValue;
    float mTarget;
    float mWeight;
    float mSRate;

  public:

    dspRC()
      {
        mValue  = 0;
        mTarget = 0;
        mWeight = 0;
        mSRate = 0; // !!!
      }

    void setup(float aValue=0, float aTarget=0, float aWeight=0)
      {
        mValue  = aValue;
        mTarget = aTarget;
        mWeight = aWeight;
      }

    inline void setValue(float aValue)    { mValue = aValue; }
    inline void setTarget(float aTarget)  { mTarget = aTarget; }
    inline void setWeight( float aWeight) { mWeight = aWeight; }

    inline float getValue(void)   { return mValue; }
    inline float getTarget(void)  { return mTarget; }
    inline float getWeight(void)  { return mWeight; }

    inline void setSRate(float aSRate) { mSRate = aSRate; }

    //-----

    void setFreq(float aFreq)
      {
        if (mSRate>0) mWeight = 1 - axExpf(-2*PI*aFreq/mSRate);
        else mWeight = 0;
      }

    //

    void setFreq(float aFreq, float aSRate)
      {
        if (aSRate>0) mWeight = 1 - axExpf(-2*PI*aFreq/aSRate);
        else mWeight = 0;
      }

    //-----

    void setTime(float aTime)
      {
        if (aTime>0) mWeight = 1 - axExpf(-1/aTime);
        else mWeight = 0;
      }

    //-----

    virtual float process(void)
    {
      mValue += ((mTarget-mValue)*mWeight);
      return mValue;
    }

    virtual float process(float in)
    {
      mValue += ((in-mValue)*mWeight);
      return mValue;
    }

};

//----------------------------------------------------------------------
#endif

/*

http://forum.cockos.com/showpost.php?p=348189&postcount=7

    desc:lowpass / weighted average
    slider1:0.5<0,1,0.001>weight
    @init
      n0 = 0;
      n1 = 0;
    @slider
      weight = slider1*slider1;         // ???
    @sample
      spl0 =  (n0+=((spl0-n0)*weight)); // lowpass
      spl1 -= (n1+=((spl1-n1)*weight)); // highpass

The low/highpass is a perfectly correct RC filter as far as I can tell.
The formula to calculate the "weight" from frequency is:
weight = 1-exp(-2*$pi*fc/srate);
and for time (that is RC time constant) it is:
weight = 1-exp(-1/t);
where t is the time it takes the filter to decay to
36.8% of its initial input or reach 63.2% of its final output.

*/
