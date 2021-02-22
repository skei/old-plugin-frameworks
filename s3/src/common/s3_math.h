#ifndef s3_math_included
#define s3_math_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

float S3_AddDb(float val);
float S3_Average(const unsigned int n, const float* ar);
float S3_Clamp(const float input, const float limit);
float S3_Clamp(const float input, const float minval, const float maxval );
float S3_Clip(float x, float l, float h);
float S3_Curve(float x, float t);
float S3_DbToAmp(float g);
float S3_Fract(const float value);
float S3_Fract(const float value, float* intpart);
float S3_KillDenorm(float n);
float S3_Max(const float a, const float b);
float S3_Min(const float a, const float b);
float S3_Modulo(float x, float y);
float S3_NoteToHz(float ANote);
float S3_Rms(const unsigned int n, const float* ar);
float S3_Round(const float v);
float S3_Sigmoid(float x);
float S3_Sign(const float v);
float S3_Sinc(float x);
float S3_Sqr(float x);
float S3_Trunc(const float v);
float S3_Windowing_Rect(float pos);
float S3_Windowing_Hann(float pos);
float S3_Windowing_Hamming(float pos);
float S3_Windowing_Blackman(float pos);
float S3_Windowing_Gaussian(float pos);
float S3_Windowing_Welch(float pos);
float S3_Windowing_Bartlett(float pos);
float S3_Wrap(float v, float lo, float hi);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------


/*
  http://www.kvraudio.com/forum/viewtopic.php?p=2788669#p2788669
  const EXP2LUT = [1/32,1/16,1/8,1/4,1/2,1,2,4,8,16,32];
  const 1_OVER_LN2 = 1/0.693147180559;
  inline float PowerOfTwo(float exponent) {
    int whole = Floor(exponent);
    float f = exponent - whole;
    float x = 3.4437662E-01*f*f + 6.5561579E-01*f + 1.0000038E+00;
    result = x * EXP2LUT[5+whole];
  }
  inline float Exp(float exponent) {
    exponent = exponent * 1_OVER_LN2;
    int whole = Floor(exponent);
    float f = exponent - whole;
    float x = 3.4437662E-01*f*f + 6.5561579E-01*f + 1.0000038E+00;
    result = x * EXP2LUT[5+whole];
  }
*/

//----------------------------------------------------------------------

#include <math.h>
#include <stdlib.h> // abs

#include "common/s3_math_fast.h"
#include "common/s3_utils.h"

//----------------------------------------------------------------------

// denormalization killing with dc as presented by Laurent de Soras in
// http://ldesoras.free.fr/doc/articles/denormal-en.pdf

//static __S3_UNUSED
float S3_AddDb(float val) {
  // static const float antidenormal_dc = 1e-20;
  static const float antidenormal_dc = 0.000000000000000001f;
  return val + antidenormal_dc;
}

//----------
/*
  calculate the average value of a set of floats
  example:
    float ar[] = { -1.f, 2.f, 3.f, 4.f, 5.f };
    float result = SAverage(5,ar); // result is 2.6f
  n - number of elements (n)
  ar - array of floats
*/

//static __S3_UNUSED
float S3_Average(const unsigned int n, const float* ar) {
  float total = 0;
  unsigned int i=0;
  while (i<n) {
    total += ar[i];   i++;
  }
  return total/n;
}

//----------

// limits a floating point number to [-limit, limit]

//static __S3_UNUSED
float S3_Clamp(const float input, const float limit) {
  const float _t = (input > -limit) ? input : -limit;
  //return (_t > limit) ? _t : limit;
  return (_t < limit) ? _t : limit;
}

//----------

// limits a floating point number to [-limit, limit]

//static __S3_UNUSED
float S3_Clamp(const float input, const float minval, const float maxval ) {
  const float _t = (input > minval) ? input : minval;
  return (_t < maxval) ? _t : maxval;
}

//----------

//static __S3_UNUSED
float S3_Clip(float x, float l, float h) {
  return  (abs(x-l) + (l+h) - abs(x-h)) * 0.5;
}

//----------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=2049788#p2049788

  't' is a scaling variable - 0.5 gives you a straight line output (for
  straight line input) > 0.5 and the curve gets logarithmic-like, < 0.5
  and it gets exponential-like.
*/

//static __S3_UNUSED
float S3_Curve(float x, float t) {
  t = (t*0.998) + 0.001;
  float a = ( 1.0f - 1.0f/t );
  return x / (x+a * (x-1) );
}

//return x / (x+(1-1/t)*(x-1));

/*
  see also:

  http://www.kvraudio.com/forum/viewtopic.php?p=3890342#p3890342
  For arbitrary shapes of x, between 0..1, try this:
    y = x / (x + (1 - (1/shape)) * (x - 1));
  where:
    shape = 0.5 -> lin
    shape > 0.5 -> log
    shape < 0.5 -> exp
  Just plot it in Excel or so to get the hang of it.
  http://www.kvraudio.com/forum/viewtopic.php?p=3890396#p3890396
  man, that is such a cool equation. Has it got a name? Thanks.
  Just need to invert it and add a scale multiplier.
    y = 1 + (-cx / (cx + (1 - (1/shape)) * (cx - 1)));
*/


//----------

/*
  db to amplitude and vice versa
  double atodb(double a) {
	  return 20*log10(a);
  }
  double dbtoa(double db) {
  	return pow(2, db/6);
  }

  ---

  there is no 10^(db/20) == 0
  BTW, 2^(db/6) is close, but not quite accurate.

*/

// Convert a value in dB's to a linear amplitude

//static __S3_UNUSED
float S3_DbToAmp(float g) {
  if (g > -144.0) return exp(g * 0.115129254);
  else return 0;
}

//----------

/*
  returns the fractional part of a floating point number
  (and stores the integer part in the second argument)

  example:
    value_pi = 3.141592;
    float integer;
    float fraction = SFract(value_pi, &integer);
    // fraction = 0.141592, integer = 3

  value - input variable
  intpart - pointer to integer part variable
  return fractional part
*/

/*
  SFract(0.1)   =  0.1
  SFract(0.9)   =  0.9
  SFract(1.1)   =  0.1
  SFract(1.9)   =  0.9
  SFract(-0.1)  = -0.1
  SFract(-0.9)  = -0.9
  SFract(-1.1)  = -0.1
  SFract(-1.9)  = -0.9
*/

//static __S3_UNUSED
float S3_Fract(const float value) {
  float intpart = (float)(int)value;
  return (value - intpart);
}

//----------

//static __S3_UNUSED
float S3_Fract(const float value, float* intpart) {
  *intpart = (float)(int)value;
  return (value - *intpart);
}

//----------

/*
  http://www.kvraudio.com/forum/viewtopic.php?p=4817458#p4817458
*/

/*
// skei_const.pas: S3_DENORM = 1.4012984e-45;
const double _denorm = 1.0e-18;//1.0e-56;

float S3_KillDenorm(float v) {
   v += _denorm;
   v -= _denorm;
   return v;
}
*/

//----------

// DelphiASIOVST/DAVDCommon.pas

/*
function S3_KillDenorm(value : Single) : Single;
begin
 if (abs(value) < 1.0e-20)
  then Result := 0.0
  else Result := value;
end;
*/

//----------

/*
  denormalize input value
*/

//static __S3_UNUSED
float S3_KillDenorm(float n) {
  /*register*/ union
  {
    float f;
    uint32 /*unsigned int*/ i;
  } u;
  u.f = n;
  if ( !(u.i & 0x7f800000) ) n = 0.f;
  return n;
}

// TODO:

//static __S3_UNUSED
//double S3_KillDenorm(double n) {
//  return n;
//}

//----------

//static __S3_UNUSED
float S3_Max(const float a, const float b) {
  return (a > b) ? a : b;
}

//----------

// returns the smaller of two floating point numbers

//static __S3_UNUSED
float S3_Min(const float a, const float b) {
  return (a < b) ? a : b;
}

//----------

// http://music.columbia.edu/pipermail/music-dsp/2002-July/049893.html
//
// there is an ANSI function (math.h):
// double fmod(double x,double y)
// returns the floating-point remainder of x / y. Or NaN if y==0.0.

//----------

// http://music.columbia.edu/pipermail/music-dsp/2002-July/049899.html
//
// Use fmod() rather than % for floating point values.
// Or use something like this:
//    fractional = phase / strive; // shouldn't this be "stride" ?
//    fractional -= floorf( fractional );
// Which is what you ended up with, but using an explicit floating
// point floorf() (which usually gets intrinsic-ed) rather than the
// casts.

//----------

//static __S3_UNUSED
float S3_Modulo(float x, float y) {
  return x - (y * float(floor(x / y)));
}

//----------

//static __S3_UNUSED
float S3_NoteToHz(float ANote) {
  return 440 * pow(2.0,(ANote-69)*S3_INV12F);
  //return 440 * S3_PowF2(2.0,(ANote-69)*S3_INV12F);  // too much error for notetohz

}

//----------

/*
  calculate the RMS of a set (array) of float numbers

  example:
    float ar[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
    float result = SRMS(5, ar);   // result = 3.31662

  n - size of the array
  ar - array of floats
*/

//static __S3_UNUSED
float S3_Rms(const unsigned int n, const float* ar) {
  float numr = 0;
  unsigned int i=0;
  while (i<n) {
    numr += ar[i]*ar[i];
    i++;
  }
  return sqrtf(numr/n);
}

// rounds a floating point number

/*
  SRound(0.1)   =  0.0
  SRound(0.9)   =  1.0
  SRound(1.1)   =  1.0
  SRound(1.9)   =  2.0
  SRound(-0.1)  = -0.0
  SRound(-0.9)  = -1.0
  SRound(-1.1)  = -1.0
  SRound(-1.9)  = -2.0
*/

//static __S3_UNUSED
float S3_Round(const float v) {
  if (v < 0.f) return (float)(int)(v - 0.5f);
  else return (float)(int)(v + 0.5f);
}

//----------

/*
  SSigmoid(0.0)   //  0
  SSigmoid(0.1)   //  0.1495
  SSigmoid(0.3)   //  0.4365
  SSigmoid(0.5)   //  0.5875
  SSigmoid(0.7)   //  0.8785
  SSigmoid(0.9)   //  0.9855
  SSigmoid(1.0)   //  1
*/


//static __S3_UNUSED
float S3_Sigmoid(float x) {
  if ( abs(x) < 1.0f ) return x * (1.5f - 0.5f * x * x);
  else {
    if (x < 0.0f) return -1.0f;
    else return 1.0f;
  }
}


//----------

/*
  SSign(0.5)  = 1
  SSign(1.5)  = 1
  SSign(-0.5) = -1
  SSign(-1.5) = -1
*/

//static __S3_UNUSED
float S3_Sign(const float v) {
  //union {
  //  int32 i;
  //  float f;
  //} u;
  //u.f = v;
  //return (1 | (u.i >> 31));
  if (v < 0) return -1.0f;
  else if (v > 0) return 1.0f;
  else return 0.0f;
}

//----------

/*
  0.0 = 1
  0.1 = 0.9836
  0.3 = 0.8584
  0.5 = 0.6366
  0.7 = 0.3678
  0.9 = 0.1093
  1.0 = 0
*/

//double SSinc(double x) {
//  if (x==0) return 1;
//  else {
//    double pix = S3_PI * x;
//    return sin(pix) / pix;
//  }
//}

//static __S3_UNUSED
float S3_Sinc(float x) {
  if (x==0) return 1;
  else {
    float pix = S3_PI * x;
    return sin(pix) / pix;
  }
}


//----------

//static __S3_UNUSED
float S3_Sqr(float x) {
  return x*x;
}

//----------

/*
  STrunc(0.1)   =  0.0
  STrunc(0.9)   =  0.0
  STrunc(1.1)   =  1.0
  STrunc(1.9)   =  1.0
  STrunc(-0.1)  =  0.0
  STrunc(-0.9)  =  0.0
  STrunc(-1.1)  = -1.0
  STrunc(-1.9)  = -1.0
*/

//----------

//static __S3_UNUSED
float S3_Trunc(const float v) {
  return (float)(int)v;
}

//----------

//static __S3_UNUSED
float S3_Windowing_Rect(float pos) {
  return 1.0;
}

//static __S3_UNUSED
float S3_Windowing_Hann(float pos) {
  float x = cos(pos*S3_PI/2.0);
  return x*x;
}

//static __S3_UNUSED
float S3_Windowing_Hamming(float pos) {
  return 0.54 + 0.46 * cos(S3_PI*pos);
}

//static __S3_UNUSED
float S3_Windowing_Blackman(float pos) {
  return 0.42 + 0.5 * cos(S3_PI*pos) + 0.08 * cos(2.0*S3_PI*pos);
}

//static __S3_UNUSED
float S3_Windowing_Gaussian(float pos) {
  float a = 4.0;
  float ax = a * pos;
  return exp(-0.5 * ax*ax);
}

//static __S3_UNUSED
float S3_Windowing_Welch(float pos) {
  return 1.0 - pos*pos;
}

//static __S3_UNUSED
float S3_Windowing_Bartlett(float pos) {
 if (pos == 0.0) return 1.0;
 else if (pos > -1.0 && pos < 0.0) return pos + 1.0;
 else if (pos > 0.0 && pos < 1.0) return 1.0 - pos;
 else return 0.0;
}

//----------

float S3_Wrap(float v, float lo, float hi) {
  if (lo > hi) { S3_Swap(lo,hi); }
  v -= lo; // adjust to 0
  float range = hi - lo;
  if (range == 0) { return hi; } // avoid dividing by 0
  return v - (range * floor(v/range)) + lo;
}

//----------------------------------------------------------------------
// https://github.com/Ixox/preenFM2/blob/master/src/third/wirish_math.h
//----------------------------------------------------------------------

/*
  Remap a number from one range to another.
  That is, a value equal to fromStart gets mapped to toStart, a value
  of fromEnd to toEnd, and other values are mapped proportionately.
  Does not constrain value to lie within [fromStart, fromEnd].
  If a "start" value is larger than its corresponding "end", the
  ranges are reversed, so map(n, 1, 10, 10, 1) would reverse the
  range [1,10].
  Negative numbers may appear as any argument.
  value     the value to map.
  fromStart the beginning of the value's current range.
  fromEnd   the end of the value's current range.
  toStart   the beginning of the value's mapped range.
  toEnd     the end of the value's mapped range.
  return    the mapped value.
*/

//int32 map(int32 value, int32 fromStart, int32 fromEnd, int32 toStart, int32 toEnd) {
//  return (value - fromStart) * (toEnd - toStart) / (fromEnd - fromStart) + toStart;
//}

//#define PI          3.1415926535897932384626433832795
//#define HALF_PI     1.5707963267948966192313216916398
//#define TWO_PI      6.283185307179586476925286766559
//#define DEG_TO_RAD  0.017453292519943295769236907684886
//#define RAD_TO_DEG 57.295779513082320876798154814105

//#define min(a,b)                ((a)<(b)?(a):(b))
//#define max(a,b)                ((a)>(b)?(a):(b))
//#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
//#define round(x)                ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
//#define radians(deg)            ((deg)*DEG_TO_RAD)
//#define degrees(rad)            ((rad)*RAD_TO_DEG)
//#define sq(x)                   ((x)*(x))

// undefine stdlib's abs
//#ifdef abs
//#undef abs
//#endif
//#define abs(x) (((x) > 0) ? (x) : -(x))

//----------------------------------------------------------------------
#endif

