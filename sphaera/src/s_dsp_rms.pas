unit s_dsp_rms;
interface
implementation
end.

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
 * @file dspRMS.h
 * \brief one pole filter for RMS approximation
 */

#ifndef dspRMS_included
#define dspRMS_included

/**
 * \brief one pole filter for RMS approximation
 *
 * one pole filter for RMS approximation <br>
 * usage example:
 * \code
 * // include "dspRMS.h"
 * // create instance
 * dspRMS rmsfilter;
 * // setup
 * rmsfilter.setup(300, 44100); // 300ms, samplerate (can be updateSampleRate())
 * float rmsout = rmsfilter.process(input); // process input sample
 * // to reset the rms value
 * rmsfilter.reset();
 * \endcode
 */
class dspRMS
{
  private:
  /** variables */
    float win,fout,a0,b1;

  public:
    /** constructor */
    dspRMS()
    {
      reset();
      setup(300.f, 44100.f);
    }
    /**
     * set filter coeff
     * @param[in] winlen float - window length (ms)
     * @param[in] srate unsigned int - sample rate
     * @return void
     */
    void setup(const float winlen, const unsigned int srate)
    {
      b1 = axExpf(-1/(winlen*(float)srate*0.001));
      a0 = 1 - b1;
    }

    /**
     * process input sample with filter
     * @param[in] in float
     * @return float rms value
     */
    virtual float process(const float in)
    {
      fout = a0*in*in + b1*fout + DENORM;
      return axSqrtf(fout);
    }

    /**
     * reset parameters
     * @return void
     */
    virtual void reset(void) { win = fout = a0 = b1 = 0.f; }
};

//----------------------------------------------------------------------
#endif

