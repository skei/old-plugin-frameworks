/*
  Copyright 2006, Thomas Scott Stillwell
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

  Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  The name of Thomas Scott Stillwell may not be used to endorse or
  promote products derived from this software without specific prior written
  permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
  OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define AX_NOGUI

#include "base/axBase.h"
#include "par/parFloat.h"

char* str_params[] = { (char*)"threshold", (char*)"ceiling", (char*)"softclip"/*, (char*)"ratio"*/ };

class myDescriptor : public AX_DESCRIPTOR
{
  public:
    myDescriptor(axBase* aBase) : AX_DESCRIPTOR(aBase) { }
    virtual char*         getName(void)             { return (char*)"eventhorizon"; }
    virtual char*         getAuthor(void)           { return (char*)"sstillwell"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib port"; }
    virtual unsigned int  getUniqueId(void)         { return AX_MAGIC + 0x0000; }
    virtual int           getNumParams(void)        { return 3; } //4
    virtual char*         getParamName(int aIndex)  { return str_params[aIndex]; }
};

class myInstance : public AX_INSTANCE
{
  private:
    float _log2db, _db2log;
    float slider1,slider2,slider3,slider4;
    float thresh,threshdb;
    float ceiling,ceildb,makeup,makeupdb;
    float sc,scv,sccomp,peakdb,peaklvl;
    float scratio,scmult;

  public:

    //myPlugin(axContext* aContext, int aFlags)
    //: axFormat(aContext)
    myInstance(axBase* aBase) : AX_INSTANCE(aBase)
      {
        _log2db = 8.6858896380650365530225783783321; // 20 / ln(10)
        _db2log = 0.11512925464970228420089957273422; // ln(10) / 20
        slider1=slider2=slider3=slider4 = 0;
        appendParameter( new parFloat(this,"threshold",      "",    0.0, -30.0,  0.0, 0.1  ) );
        appendParameter( new parFloat(this,"ceiling",        "",   -0.1, -20.0,  0.0, 0.1  ) );
        appendParameter( new parFloat(this,"soft clip",      "db",  2.0,   0.0,  6.0, 0.01 ) );
      //appendParameter( new parFloat(this,"soft clip ratio","",   10.0,   3.0, 20.0, 0.1  ) );
        //setupParameters();
        prepareParameters();

      }

    //----------

    virtual void doStateChange(int aState)
      {
        switch (aState)
        {
          case is_Resume:
            //transferParameters();
            recalcAll();
            break;
        }
      }

    //----------

    //[internal]
    void recalcAll(void)
      {
        thresh    = axExp(slider1 * _db2log);
        threshdb  = slider1;
        ceiling   = axExp(slider2 * _db2log);
        ceildb    = slider2;
        makeup    = axExp((ceildb-threshdb) * _db2log);
        makeupdb  = ceildb - threshdb;
        sc        = -slider3;
        scv       = axExp( sc * _db2log);
        sccomp    = axExp(-sc * _db2log);
        peakdb    = ceildb + 25;
        peaklvl   = axExp(peakdb * _db2log);
        scratio   = slider4;
        scmult    = axAbs((ceildb - sc) / (peakdb - sc));
      }

    //----------

    virtual void  doSetParameter(axParameter* aParameter)
      {
        float val = aParameter->getValue();
        switch (aParameter->getIndex())
        {
          case 0: slider1=val; break;
          case 1: slider2=val; break;
          case 2: slider3=val; break;
          case 3: slider4=val; break;
        }
        recalcAll();
      }

    //----------

    virtual void doProcessSample(SPL** aInputs, SPL** aOutputs)
      {
        float spl0 = *aInputs[0];
        float spl1 = *aInputs[1];
        //float peak = axMax(axAbs(spl0),axAbs(spl1));
        spl0 = spl0 * makeup;
        spl1 = spl1 * makeup;
        float sign0 = axSign(spl0);
        float sign1 = axSign(spl1);
        float abs0 = axAbs(spl0);
        float abs1 = axAbs(spl1);
        float overdb0 = 2.08136898 * axLog(abs0) * _log2db - ceildb;
        float overdb1 = 2.08136898 * axLog(abs1) * _log2db - ceildb;
        if (abs0>scv) { spl0 = sign0 * (scv + axExp(overdb0*scmult)*_db2log); }
        if (abs1>scv) { spl1 = sign1 * (scv + axExp(overdb1*scmult)*_db2log); }
        spl0 = axMin(ceiling,axAbs(spl0)) * axSign(spl0);
        spl1 = axMin(ceiling,axAbs(spl1)) * axSign(spl1);
        *aOutputs[0] = spl0;
        *aOutputs[1] = spl1;
      }

};

AX_MAIN(myDescriptor,myInstance)
//AX_ENTRYPOINT(AX_PLATFORM,axInterfaceNone,AX_FORMAT,myDescriptor,myInstance)

//----------------------------------------------------------------------

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
