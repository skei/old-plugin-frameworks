#ifndef event_horizon_included
#define event_horizon_included
//----------------------------------------------------------------------

/*
  based on:
    event horizon (peak-eating limiter) jesusonic plugin
    Copyright 2006, Thomas Scott Stillwell
    see bottom of this file for original and license info
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define S3_DEBUG_MEM

#ifdef S3_PLUGIN_VST
  //#define S3_DEBUG_VST
  #define S3_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include <math.h>
#include "s3.h"
//#include "s3_math.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

//#define _log2db 8.6858896380650365530225783783321   /* 20 / ln(10) */
//#define _db2log 0.11512925464970228420089957273422  /* ln(10) / 20 */

#define _log2db (20.0f / logf(10.0f))
#define _db2log (logf(10.0f) / 20.0f)

//----------

class myPlugin
: public S3_Plugin
{

  private:

    //float _log2db, _db2log;
    float slider1, slider2, slider3, slider4;
    float thresh, threshdb;
    float ceiling, ceildb, makeupdb;
    float makeup;
    float sc, scv, sccomp, peakdb, peaklvl;
    float scratio, scmult;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) final {
      MName         = "fx_event_horizon";
      MAuthor       = "thomas scott stillwell";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_fx_event_horizon_id;
      MNumInputs  = 2;
      MNumOutputs = 2;

      appendParameter( new S3_ParamFloat("threshold",   0,   -30, 0 ));
      appendParameter( new S3_ParamFloat("ceiling",    -0.1, -20, 0 ) );
      appendParameter( new S3_ParamFloat("soft clip",   2,    0,  6 ) );
      //appendParameter( new parFloat("soft clip ratio","",   10.0,   3.0, 20.0, 0.1  ) );

      slider1   = 0.0f;
      slider2   = 0.0f;
      slider3   = 0.0f;
      slider4   = 0.0f; //10.0;

      thresh    = 0.0f;
      threshdb  = 0.0f;
      ceiling   = 0.0f;
      ceildb    = 0.0f;
      makeupdb  = 0.0f;
      makeup    = 0.0f;
      sc        = 0.0f;
      scv       = 0.0f;
      sccomp    = 0.0f;
      peakdb    = 0.0f;
      peaklvl   = 0.0f;
      scratio   = 0.0f;
      scmult    = 0.0f;
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

  //----------------------------------------
  // internal
  //----------------------------------------

  private:

    void recalcAll(void) {
      thresh    = expf(slider1 * _db2log);
      threshdb  = slider1;
      ceiling   = expf(slider2 * _db2log);
      ceildb    = slider2;
      makeup    = expf((ceildb-threshdb) * _db2log);
      makeupdb  = ceildb - threshdb;
      sc        = -slider3;
      scv       = expf( sc * _db2log);
      sccomp    = expf(-sc * _db2log);
      peakdb    = ceildb + 25.0f;
      peaklvl   = expf(peakdb * _db2log);
      scratio   = slider4;
      scmult    = fabs((ceildb - sc) / (peakdb - sc));
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      switch (AIndex) {
        case 0: slider1 = AValue; break;
        case 1: slider2 = AValue; break;
        case 2: slider3 = AValue; break;
        //case 3: slider4 = AValue; break;
      }
      recalcAll();
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (uint32 i=0; i<ANumSamples; i++) {
        float spl0 = *input0++;
        float spl1 = *input1++;

        //float peak = SMax(abs(spl0),abs(spl1));
        spl0 *= makeup;
        spl1 *= makeup;

        float sign0 = S3_Sign(spl0);
        float sign1 = S3_Sign(spl1);
        float abs0 = fabs(spl0);
        float abs1 = fabs(spl1);

        float overdb0 = 2.08136898 * logf(abs0) * _log2db - ceildb; // c++ Log = pascal Ln ?????
        float overdb1 = 2.08136898 * logf(abs1) * _log2db - ceildb;

        if (abs0 > scv) spl0 = sign0 * (scv + expf(overdb0*scmult)*_db2log);
        if (abs1 > scv) spl1 = sign1 * (scv + expf(overdb1*scmult)*_db2log);

        spl0 = S3_Min(ceiling,fabs(spl0)) * S3_Sign(spl0);
        spl1 = S3_Min(ceiling,fabs(spl1)) * S3_Sign(spl1);

        *output0++ = spl0;
        *output1++ = spl1;
      }
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif





/*

// Copyright 2006, Thomas Scott Stillwell
// All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are permitted
//provided that the following conditions are met:
//
//Redistributions of source code must retain the above copyright notice, this list of conditions
//and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice, this list of conditions
//and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//The name of Thomas Scott Stillwell may not be used to endorse or
//promote products derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
//IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
//BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
desc:peak-eating limiter

slider1:0.0<-30.0,0.0,0.1>Threshold
slider2:-0.1<-20.0,0.0,0.1>Ceiling
slider3:2.0<0,6.0,0.01>Soft clip (dB)
//slider4:10<3,20,0.1>Soft clip ratio

@init
  pi = 3.1415926535;
  log2db = 8.6858896380650365530225783783321; // 20 / ln(10)
  db2log = 0.11512925464970228420089957273422; // ln(10) / 20

@slider
  thresh=exp(slider1 * db2log);
  threshdb=slider1;
  ceiling=exp(slider2 * db2log);
  ceildb=slider2;
  makeup=exp((ceildb-threshdb) * db2log);
  makeupdb=ceildb-threshdb;
  sc = -slider3;
  scv = exp(sc * db2log);
  sccomp = exp(-sc * db2log);
  peakdb = ceildb+25;
  peaklvl = exp(peakdb * db2log);
  scratio = slider4;
  scmult = abs((ceildb - sc) / (peakdb - sc));

@sample
  peak=max(abs(spl0),abs(spl1));
  spl0=spl0*makeup;
  spl1=spl1*makeup;
  sign0 = sign(spl0);
  sign1 = sign(spl1);
  abs0=abs(spl0);
  abs1=abs(spl1);
  overdb0 = 2.08136898 * log(abs0) * log2db - ceildb;
  overdb1 = 2.08136898 * log(abs1) * log2db - ceildb;
  abs0 > scv ? (
    spl0=sign0*(scv + exp(overdb0*scmult)*db2log);
  );
  abs1 > scv ? (
    spl1=sign1*(scv + exp(overdb1*scmult)*db2log);
  );
  spl0=min(ceiling,abs(spl0))*sign(spl0);
  spl1=min(ceiling,abs(spl1))*sign(spl1);

*/

