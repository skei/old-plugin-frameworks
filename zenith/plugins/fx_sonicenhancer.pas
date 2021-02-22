unit fx_sonicenhancer;
{$include zenith.inc}

{ we assume variables start as 0 :-/}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses

  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter;

type

  myDescriptor = class(ZDescriptor)
  public
    procedure on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    // init
    r_6, nf_k, rc0_k0, rc0_k1, fc, fs, pi2, q, omega, sn, cs, alpha,
    b0_ap, b1_ap, b2_ap,
    a0_ap, a1_ap, a2_ap,
    b0_lp, b1_lp, b2_lp,
    a1_lp, a0_lp, a2_lp,
    b0_hp, b1_hp, b2_hp,
    a1_hp, a0_hp, a2_hp : single;
    // slider
    g_in, g_lp, g_hp, cv_k, g_out, noise_floor : single;
    // sample?
    x1_ap0, x2_ap0, y1_ap0, y2_ap0,
    x1_ap1, x2_ap1, y1_ap1, y2_ap1,
    x1_lp0, x2_lp0, y1_lp0, y2_lp0,
    x1_lp1, x2_lp1, y1_lp1, y2_lp1,
    x1_hp0, x2_hp0, y1_hp0, y2_hp0,
    x1_hp1, x2_hp1, y1_hp1, y2_hp1,
    cv_rc0, cv_rc1,
    out_rc0, out_rc1 : single;
  private
    procedure init(srate:single);
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPZSample); override;
    procedure on_stateChange(AState:LongWord); override;
  end;

//----------

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  math,
  z_random,
  z_util;

//----------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

//--------------------------------------------------
// descriptor
//--------------------------------------------------

var
  txt_noise : array[0..1] of PChar = ('off','on');

procedure myDescriptor.on_create;
begin
  FName     := 'fx_sonicenhancer';
  FAuthor   := 'lubomir i ivanov';
  FProduct  := 'powered by zenith';
  FVersion  := 0;
  FUniqueId := Z_MAGIC + $0105;

  ZSetFlag(FFlags,zpf_perSample);

  appendParameter( ZParamFloat.create('Input',       0, -12, 12, 0.1 ) );
  appendParameter( ZParamFloat.create('Low Contour', 0,  0,  10, 0.1 ) );
  appendParameter( ZParamFloat.create('Process',     0,  0,  10, 0.1 ) );
  appendParameter( ZParamFloat.create('CV',          2,  0,  10, 0.1 ) );
  appendParameter( ZParamFloat.create('Output',      0, -12, 12, 0.1 ) );
  appendParameter( ZParamText.create( 'Noise Floor', 1, 2, txt_noise ) );

end;

//--------------------------------------------------
// instance
//--------------------------------------------------

procedure myInstance.init(srate:single);
begin
  r_6     := 1/6;
  nf_k    := power(2, -96.0 * r_6);
  rc0_k0  := 0.0003;
  rc0_k1  := 1 - rc0_k0;
  fc      := 700;
  fs      := srate;
  pi2     := 2*Z_PI;
  q       := 0.23;
  omega   := pi2 * fc / fs;
  sn      := sin(omega);
  cs      := cos(omega);
  alpha   := sn / (2.0 * Q);
  b0_ap   := 1.0 / (1.0 + alpha);
  b1_ap   := (-2.0 * cs) * b0_ap;
  b2_ap   := (1.0 - alpha) * b0_ap;
  a0_ap   := b2_ap;
  a1_ap   := b1_ap;
  a2_ap   := 1;
  b0_lp   := 1.0 / (1.0  + alpha);
  b1_lp   := (-2.0 * cs) * b0_lp;
  b2_lp   := (1.0 - alpha) * b0_lp;
  a1_lp   := (1.0 - cs) * b0_lp;
  a0_lp   := a1_lp * 0.5;
  a2_lp   := a0_lp;
  b0_hp   := 1.0 / (1.0 + alpha);
  b1_hp   := (-2.0 * cs) * b0_hp;
  b2_hp   := (1.0 - alpha) * b0_hp;
  a1_hp   := -(1.0 + cs) * b0_hp;
  a0_hp   := -a1_hp * 0.5;
  a2_hp   := a0_hp;
end;

//----------

procedure myInstance.on_create;
begin
end;

//----------

procedure myInstance.on_stateChange(AState:LongWord);
begin
  case AState of
    zps_resume: init(FSampleRate);
  end;
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var i : longint;
    p : ZParameter;
    v : single;
begin
  i := AIndex;
  p := FParameters[i];
  v := p.from01(AValue);
  case i of
    0 : g_in := power(2,v*r_6);   // input
    1 : g_lp := v * 0.5;          // low contour
    2 : g_hp := v * 0.5;          // process
    3 : cv_k := v;                // cv
    4 : g_out := power(2,v*r_6);  // output
    5 : noise_floor := v;         // noise floor
  end;
end;

//----------

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
var
  spl0,spl1,
  in0, in1, nf0, nf1,
  cv0, cv1,
  out_ap0, out_ap1,
  out_sv0, out_sv1,
  out_dc0, out_dc1,
  out_lp0, out_lp1,
  out_hp0, out_hp1 : single;
begin
  spl0  := AInputs[0]^;
  spl1  := AInputs[1]^;
  in0   := spl0*g_in;
  in1   := spl1*g_in;
  nf0   := 0.0;
  nf1   := 0.0;
  if noise_floor > 0.5 then
  begin
    nf0 := ZRandomSigned * nf_k;
    nf1 := ZRandomSigned * nf_k;
  end;
  out_ap0 := a0_ap*in0 + a1_ap*x1_ap0 + a2_ap*x2_ap0 - b1_ap*y1_ap0 - b2_ap*y2_ap0;
  x2_ap0  := x1_ap0;
  x1_ap0  := in0;
  y2_ap0  := y1_ap0;
  y1_ap0  := out_ap0;
  out_ap1 := a0_ap*in1 + a1_ap*x1_ap1 + a2_ap*x2_ap1 - b1_ap*y1_ap1 - b2_ap*y2_ap1;
  x2_ap1  := x1_ap1;
  x1_ap1  := in1;
  y2_ap1  := y1_ap1;
  y1_ap1  := out_ap1;
  out_lp0 := a0_lp*in0 + a1_lp*x1_lp0 + a2_lp*x2_lp0 - b1_lp*y1_lp0 - b2_lp*y2_lp0;
  x2_lp0  := x1_lp0;
  x1_lp0  := in0;
  y2_lp0  := y1_lp0;
  y1_lp0  := out_lp0;
  out_lp1 := a0_lp*in1 + a1_lp*x1_lp1 + a2_lp*x2_lp1 - b1_lp*y1_lp1 - b2_lp*y2_lp1;
  x2_lp1  := x1_lp1;
  x1_lp1  := in1;
  y2_lp1  := y1_lp1;
  y1_lp1  := out_lp1;
  out_hp0 := a0_hp*in0 + a1_hp*x1_hp0 + a2_hp*x2_hp0 - b1_hp*y1_hp0 - b2_hp*y2_hp0;
  x2_hp0  := x1_hp0;
  x1_hp0  := in0;
  y2_hp0  := y1_hp0;
  y1_hp0  := out_hp0;
  out_hp1 := a0_hp*in1 + a1_hp*x1_hp1 + a2_hp*x2_hp1 - b1_hp*y1_hp1 - b2_hp*y2_hp1;
  x2_hp1  := x1_hp1;
  x1_hp1  := in1;
  y2_hp1  := y1_hp1;
  y1_hp1  := out_hp1;
  cv0     := 1 - abs(in0)*cv_k;
  cv1     := 1 - abs(in1)*cv_k;
  cv_rc0  := rc0_k0*cv0 + rc0_k1*cv_rc0;
  cv_rc1  := rc0_k0*cv1 + rc0_k1*cv_rc1;
  out_sv0 := out_ap0 + out_hp0*cv_rc0*g_hp + out_lp0*g_lp;
  out_sv1 := out_ap1 + out_hp1*cv_rc1*g_hp + out_lp1*g_lp;
  out_rc0 := rc0_k0*out_sv0 + rc0_k1*out_rc0;
  out_rc1 := rc0_k0*out_sv1 + rc0_k1*out_rc1;
  out_dc0 := out_sv0 - out_rc0;
  out_dc1 := out_sv1 - out_rc1;
  spl0    := out_dc0*g_out + nf0;
  spl1    := out_dc1*g_out + nf1;
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//TODO: in editor

{
@gfx 0 10
gfx_r = 0.8;
gfx_g = 0.5;
gfx_b = 0;

gfx_a = min(1, 0.55 + cv_rc0*0.25 + g_hp*0.1);
gfx_x = 5;
gfx_y = 5;
gfx_rectto(gfx_w*0.5 - 3, gfx_h - 5);

gfx_a = min(1, 0.55 + cv_rc1*0.25 + g_hp*0.1);
gfx_x = gfx_w*0.5 + 3;
gfx_y = 5;
gfx_rectto(gfx_w - 5, gfx_h - 5);
}

//----------------------------------------------------------------------
end.

(*

// http://forum.cockos.com/showthread.php?t=91439

/*

  sonic enhancer thing

  Copyright (C) 2011 lubomir i. ivanov

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

desc: Sonic Enhancer

slider1:0<-12, 12, 0.1>Input
slider2:0<0, 10, 0.1>Low Contour
slider3:0<0, 10, 0.1>Process
slider4:2<0, 10, 0.1>CV
slider5:0<-12, 12, 0.1>Output
slider6:1<0, 1, {Off,On})>Noise Floor

@init
r_6 = 1/6;

nf_k = pow(2, -96 * r_6);

rc0_k0 = 0.0003;
rc0_k1 = 1 - rc0_k0;

fc = 700;
fs = srate;
pi2 = 2*$pi;
q = 0.23;

omega = pi2 * fc / fs;
sn = sin(omega);
cs = cos(omega);
alpha = sn / (2.0 * Q);

b0_ap = 1.0 / (1.0 + alpha);
b1_ap = (-2.0 * cs) * b0_ap;
b2_ap = (1.0 - alpha) * b0_ap;
a0_ap = b2_ap;
a1_ap = b1_ap;
a2_ap = 1;

b0_lp = 1.0 / (1.0  + alpha);
b1_lp = (-2.0 * cs) * b0_lp;
b2_lp = (1.0 - alpha) * b0_lp;
a1_lp = (1.0 - cs) * b0_lp;
a0_lp = a1_lp * 0.5;
a2_lp = a0_lp;

b0_hp = 1.0 / (1.0 + alpha);
b1_hp = (-2.0 * cs) * b0_hp;
b2_hp = (1.0 - alpha) * b0_hp;
a1_hp = -(1.0 + cs) * b0_hp;
a0_hp = -a1_hp * 0.5;
a2_hp = a0_hp;

@slider
g_in = pow(2, slider1 * r_6);
g_lp = slider2*0.5;
g_hp = slider3*0.5;
cv_k = slider4;
g_out = pow(2, slider5 * r_6);
noise_floor = slider6;

@sample
in0 = spl0*g_in;
in1 = spl1*g_in;

nf0 = 0.0;
nf1 = 0.0;
noise_floor ? (
  nf0 = (rand(2.0) - 1.0)*nf_k;
  nf1 = (rand(2.0) - 1.0)*nf_k;
);

out_ap0 = a0_ap*in0 + a1_ap*x1_ap0 + a2_ap*x2_ap0 - b1_ap*y1_ap0 - b2_ap*y2_ap0;
x2_ap0 = x1_ap0;
x1_ap0 = in0;
y2_ap0 = y1_ap0;
y1_ap0 = out_ap0;

out_ap1 = a0_ap*in1 + a1_ap*x1_ap1 + a2_ap*x2_ap1 - b1_ap*y1_ap1 - b2_ap*y2_ap1;
x2_ap1 = x1_ap1;
x1_ap1 = in1;
y2_ap1 = y1_ap1;
y1_ap1 = out_ap1;

out_lp0 = a0_lp*in0 + a1_lp*x1_lp0 + a2_lp*x2_lp0 - b1_lp*y1_lp0 - b2_lp*y2_lp0;
x2_lp0 = x1_lp0;
x1_lp0 = in0;
y2_lp0 = y1_lp0;
y1_lp0 = out_lp0;

out_lp1 = a0_lp*in1 + a1_lp*x1_lp1 + a2_lp*x2_lp1 - b1_lp*y1_lp1 - b2_lp*y2_lp1;
x2_lp1 = x1_lp1;
x1_lp1 = in1;
y2_lp1 = y1_lp1;
y1_lp1 = out_lp1;

out_hp0 = a0_hp*in0 + a1_hp*x1_hp0 + a2_hp*x2_hp0 - b1_hp*y1_hp0 - b2_hp*y2_hp0;
x2_hp0 = x1_hp0;
x1_hp0 = in0;
y2_hp0 = y1_hp0;
y1_hp0 = out_hp0;

out_hp1 = a0_hp*in1 + a1_hp*x1_hp1 + a2_hp*x2_hp1 - b1_hp*y1_hp1 - b2_hp*y2_hp1;
x2_hp1 = x1_hp1;
x1_hp1 = in1;
y2_hp1 = y1_hp1;
y1_hp1 = out_hp1;

cv0 = 1 - abs(in0)*cv_k;
cv1 = 1 - abs(in1)*cv_k;

cv_rc0 = rc0_k0*cv0 + rc0_k1*cv_rc0;
cv_rc1 = rc0_k0*cv1 + rc0_k1*cv_rc1;

out_sv0 = out_ap0 + out_hp0*cv_rc0*g_hp + out_lp0*g_lp;
out_sv1 = out_ap1 + out_hp1*cv_rc1*g_hp + out_lp1*g_lp;

out_rc0 = rc0_k0*out_sv0 + rc0_k1*out_rc0;
out_rc1 = rc0_k0*out_sv1 + rc0_k1*out_rc1;
out_dc0 = out_sv0 - out_rc0;
out_dc1 = out_sv1 - out_rc1;

spl0 = out_dc0*g_out + nf0;
spl1 = out_dc1*g_out + nf1;

@gfx 0 10
gfx_r = 0.8;
gfx_g = 0.5;
gfx_b = 0;

gfx_a = min(1, 0.55 + cv_rc0*0.25 + g_hp*0.1);
gfx_x = 5;
gfx_y = 5;
gfx_rectto(gfx_w*0.5 - 3, gfx_h - 5);

gfx_a = min(1, 0.55 + cv_rc1*0.25 + g_hp*0.1);
gfx_x = gfx_w*0.5 + 3;
gfx_y = 5;
gfx_rectto(gfx_w - 5, gfx_h - 5);

*)