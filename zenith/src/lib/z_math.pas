unit z_math;
{$include zenith.inc}

// TODO
// move stuff to their right places!

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

function ZAbs(f:single) : single;
function ZAbs2(f: Single) : Single;
function ZCeil(f:double):integer;
function ZFloor(f:double):integer;
function ZNeg(f:single) : single;
function ZRound(f:double):integer;
function ZSign(f:single) : longint;
function ZTrunc(f:double):integer;

//----------

function ZExp2(val:single) : single;
function ZLog(X,Base:single) : single;
function ZLog2(val:single) : single;
function ZPower(i:single;n:integer) : single;
function ZRoot(i:single;n:integer) : single;

//----------

function ZByte2Float(input:byte):single;
function ZSByteToFloat(input:shortint):single;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math;

//----------------------------------------------------------------------

{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=132
}

function ZAbs(f:single) : single;
var i:longint;
begin
  i := longint((@f)^) and $7FFFFFFF;
  result := single((@i)^);
end;

//----------

{
}

function ZAbs2(f: Single) : Single;
var i: Integer absolute f;
begin
  i := i and $7fffffff;
  Result := f;
end;

//----------

{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=252
}

function ZCeil(f:double):integer;
var
  i:integer;
  t : double;
begin
  t := -0.5 ;
  asm
    fld f
    fadd st,st(0)
    fsubr t
    fistp i
    sar i, 1
  end;
  result:= -i
end;

//----------

{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=252
}

function ZFloor(f:double):integer;
var
  i:integer;
  t : double;
begin
  t := -0.5 ;
  asm
    fld f
    fadd st,st(0)
    fadd t
    fistp i
    sar i, 1
  end;
  result:= i
end;

//----------


{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=132
  gives you the negative number (faster than multiplying with -1)
}

function ZNeg(f:single) : single;
var i:longint;
begin
  // Mixing signed expressions and longwords gives a 64bit result
  i := longint((@f)^) {%H-}xor $80000000;
  result := single((@i)^);
end;

//----------

{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=252
}

function ZRound(f:double):integer;
var
  i:integer;
  t : double;
begin
  t := 0.5 ;
  asm
    fld f
    fadd st,st(0)
    fadd t
    fistp i
    sar i, 1
  end;
  result:= i
end;

//----------

{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=132
  gives back +1 for 0 or positive numbers, -1 for negative numbers
}

function ZSign(f:single) : longint;
begin
  result := 1 + ((longint((@f)^) shr 31) shl 1);
end;

//----------

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=252

function ZTrunc(f:double):integer;
var
  i:integer;
  t : double;
begin
  t := -0.5 ;
  asm
    fld f
    fadd st,st(0)
    fabs
    fadd t
    fistp i
    sar i, 1
  end;
  if f<0 then i := -i;
  result:= i
end;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

{
  http://www.kvraudio.com/forum/viewtopic.php?t=192434
}

function ZExp2(val: single): single;
var
  int: integer;
  frac: single;
begin
  int := Floor(val);
  frac := val-int;
  int := (int+127) shl 23;
  result := psingle(@int)^ * (1.0000038E+00 + 6.5561579E-01*frac + 3.4437662E-01*frac*frac);
end;

//----------

{
  http://www.midnightbeach.com/jon/pubs/clp-faq.htm
}

function ZLog(X,Base:single) : Single;
begin
  result := Ln(X) / Ln(Base);
end;

//----------

{
  http://www.musicdsp.org/showArchiveComment.php?ArchiveID=63
}

function ZLog2(val:single):single;
var log2,x:longint;
begin
  x:=longint((@val)^);
  log2:=((x shr 23) and 255)-128;
  x:=x and (not(255 shl 23));
  x:=x+127 shl 23;
  result:=single((@x)^)+log2;
end;

//----------

// http://www.musicdsp.org/archive.php?classid=5#133

function ZPower(i:single;n:integer):single;
var l:longint;
begin
  l:=longint((@i)^);
  l:=l-$3F800000;l:=l shl (n-1);l:=l+$3F800000;
  result:=single((@l)^);
end;

//----------

// http://www.musicdsp.org/archive.php?classid=5#133

function ZRoot(i:single;n:integer):single;
var l:longint;
begin
  l:=longint((@i)^);
  l:=l-$3F800000;l:=l shr (n-1);l:=l+$3F800000;
  result:=single((@l)^);
end;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

// http://www.kvraudio.com/forum/viewtopic.php?t=191753

// unsigned

function ZByte2Float(input:byte):single;
begin
  integer((@result)^):=$3F800000 or (input shl 15); //merge in the mantissa to exponent for +1
  result:=result-1;
end;

//----------

// signed

function ZSByteToFloat(input:shortint):single;
var s:single;
begin
  integer((@result)^):=((input and $80) shl 24) or $3F800000; //sign bit and exponent for 1
  s:=result; //remember this for later
  if input<0 then
    begin
      input:=(not(input)+1); //do correction for -ve numbers
    end;
  integer((@result)^):=integer((@result)^) or (input shl 16); //merge in the mantissa
  //result now holds 1+/-(1/input), so add/subtract 1 using FPU
  result:=result-s;
end;


//----------------------------------------------------------------------
// complex
//----------------------------------------------------------------------

// http://rosettacode.org/wiki/Arithmetic/Complex#Pascal

(*

program showcomplex(output);

type
 complex = record
            re,im: real
           end;

var
 z1, z2, zr: complex;

procedure set(var result: complex; re, im: real);
 begin
  result.re := re;
  result.im := im
 end;

procedure print(a: complex);
 begin
  write('(', a.re , ',', a.im, ')')
 end;

procedure add(var result: complex; a, b: complex);
 begin
  result.re := a.re + b.re;
  result.im := a.im + b.im;
 end;

procedure neg(var result: complex; a: complex);
 begin
  result.re := -a.re;
  result.im := -a.im
 end;

procedure mult(var result: complex; a, b: complex);
 begin
  result.re := a.re*b.re - a.im*b.im;
  result.im := a.re*b.im + a.im*b.re
 end;

procedure inv(var result: complex; a: complex);
 var
  anorm: real;
 begin
  anorm := a.re*a.re + a.im*a.im;
  result.re := a.re/anorm;
  result.im := -a.im/anorm
 end;

begin
 set(z1, 3, 4);
 set(z2, 5, 6);

 neg(zr, z1);
 print(zr); { prints (-3,-4) }
 writeln;

 add(zr, z1, z2);
 print(zr); { prints (8,10) }
 writeln;

 inv(zr, z1);
 print(zr); { prints (0.12,-0.16) }
 writeln;

 mul(zr, z1, z2);
 print(zr); { prints (-9,38) }
 writeln
end.

*)

//----------------------------------------------------------------------
// interpolation
//----------------------------------------------------------------------

// http://paulbourke.net/miscellaneous/interpolation/

//double LinearInterpolate(double y1,double y2,double mu)
{
  return(y1*(1-mu)+y2*mu);
}

//double CosineInterpolate(double y1,double y2,double mu)
{
  double mu2;
  mu2 = (1-cos(mu*PI))/2;
  return(y1*(1-mu2)+y2*mu2);
}

  {
    calmull-rom
    a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
   a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
   a2 = -0.5*y0 + 0.5*y2;
   a3 = y1;
  }

//double CubicInterpolate(double y0,double y1, double y2,double y3, double mu)
{
  double a0,a1,a2,a3,mu2;
  mu2 = mu*mu;
  a0 = y3 - y2 - y0 + y1;
  a1 = y0 - y1 - a0;
  a2 = y2 - y0;
  a3 = y1;
  return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

  {
    Tension: 1 is high, 0 normal, -1 is low
     Bias: 0 is even, positive is towards first segment, negative towards the other
  }

//double HermiteInterpolate(double y0,double y1,double y2,double y3,double mu,double tension,double bias)
{
  double m0,m1,mu2,mu3;
  double a0,a1,a2,a3;

	mu2 = mu * mu;
	mu3 = mu2 * mu;
  m0  = (y1-y0)*(1+bias)*(1-tension)/2;
  m0 += (y2-y1)*(1-bias)*(1-tension)/2;
  m1  = (y2-y1)*(1+bias)*(1-tension)/2;
  m1 += (y3-y2)*(1-bias)*(1-tension)/2;
  a0 =  2*mu3 - 3*mu2 + 1;
  a1 =    mu3 - 2*mu2 + mu;
  a2 =    mu3 -   mu2;
  a3 = -2*mu3 + 3*mu2;
  return(a0*y1+a1*m0+a2*m1+a3*y2);
}

//----------------------------------------------------------------------
end.
