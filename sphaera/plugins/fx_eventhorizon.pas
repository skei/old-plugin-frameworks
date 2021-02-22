{$include sphaera.inc}
unit fx_eventhorizon;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_parameter;

type

  myPlugin = class(SPlugin)
  private
    _log2db, _db2log,
    slider1,slider2,slider3,slider4,
    thresh,threshdb,
    ceiling,ceildb,makeup,makeupdb,
    sc,scv,sccomp,peakdb,peaklvl,
    scratio,scmult : single;
  private
    procedure   recalcAll;
  public
    procedure   on_create; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPLugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //z_math,
  Math,
  s_util;

//----------------------------------------------------------------------

// [internal]
procedure myPlugin.recalcAll;
begin
  //
  thresh    := Exp(slider1 * _db2log);
  threshdb  := slider1;
  ceiling   := Exp(slider2 * _db2log);
  ceildb    := slider2;
  makeup    := Exp((ceildb-threshdb) * _db2log);
  makeupdb  := ceildb - threshdb;
  sc        := -slider3;
  scv       := Exp( sc * _db2log);
  sccomp    := Exp(-sc * _db2log);
  peakdb    := ceildb + 25;
  peaklvl   := Exp(peakdb * _db2log);
  scratio   := slider4;
  scmult    := Abs((ceildb - sc) / (peakdb - sc));
end;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_eventhorizon';
  FAuthor := 'thomas scott stillwell';
  FProduct := 'ported by skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  //FFlags += [ zpf_perSample ];
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;

  appendParameter( SParamFloat.create('threshold',      0,   -30, 0, 0.1 ));
  appendParameter( SParamFloat.create('ceiling',       -0.1, -20, 0, 0.1  ) );
  appendParameter( SParamFloat.create('soft clip (db)', 2,     0, 6, 0.01 ) );
  //appendParameter( new parFloat("soft clip ratio","",   10.0,   3.0, 20.0, 0.1  ) );
  //
  _log2db := 8.6858896380650365530225783783321; // 20 / ln(10)
  _db2log := 0.11512925464970228420089957273422; // ln(10) / 20
  slider1 := 0;
  slider2 := 0;
  slider3 := 0;
  slider4 := 0;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  //p : ZParameter;
  v : Single;
begin
  //p := FParameters[AIndex];
  v := AValue;//p.from01(AValue);
  case AIndex of
    0 : slider1 := v;
    1 : slider2 := v;
    2 : slider3 := v;
    //3 : slider4 := v;
  end;
  recalcAll;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  sign0,sign1,abs0,abs1,overdb0,overdb1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  //float peak = axMax(axAbs(spl0),axAbs(spl1));
  spl0 := spl0 * makeup;
  spl1 := spl1 * makeup;
  sign0 := Sign(spl0);
  sign1 := Sign(spl1);
  abs0 := Abs(spl0);
  abs1 := Abs(spl1);
  overdb0 := 2.08136898 * Ln(abs0) * _log2db - ceildb; // c++ Log = pascal Ln ?????
  overdb1 := 2.08136898 * Ln(abs1) * _log2db - ceildb;
  if abs0 > scv then spl0 := sign0 * (scv + Exp(overdb0*scmult)*_db2log);
  if abs1 > scv then spl1 := sign1 * (scv + Exp(overdb1*scmult)*_db2log);
  spl0 := Min(ceiling,Abs(spl0)) * Sign(spl0);
  spl1 := Min(ceiling,Abs(spl1)) * Sign(spl1);

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

(*

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

*)
