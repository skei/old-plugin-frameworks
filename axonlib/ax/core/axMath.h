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

/*
  NOTE:
    some of the methods are not compatible with big endian systems or x64.
*/
// -----------------------------------------------------------------------------

#ifndef axMath_included
#define axMath_included

#include "axDefines.h"

#ifdef AX_HOT_INLINE_MATH
  #define __axmath_inline __hotinline
#else
  #define __axmath_inline inline
#endif

/**
 * invert of x: (1/x)
 */
#define axInv(x) (1/(x))

/**
 * square of x: (x^2)
 */
#define axSqr(x) ((x)*(x))

/**
 * cube of x: (x^3)
 */
#define axCube(x) ((x)*(x)*(x))

/**
 * convert radians to degrees <br>
 * \code
 * deg = rad*(180/pi);
 * \endcode
 */
#define axRad2Deg(x) ( (x)*57.2957795130823f )

/**
 * convert degrees to radians
 * \code
 * rad = deg*(pi/180);
 * \endcode
 */
#define axDeg2Rad(x) ( (x)*0.01745329251994f )

/**
 * returns the floor of a floating point number
 * @param[in] value float
 * @return result float
 */
__axmath_inline float axFloor(register const float v)
{
  if (v < 0.f)
    return (float)(int)(v - 1.f);
  else
    return (float)(int)(v);
}

/**
 * returns the ceil of a floating point number
 * @param[in] value float
 * @return result float
 */
__axmath_inline float axCeil(register const float v)
{
  if (v < 0.f)
    return (float)(int)(v);
  else
    return (float)(int)(v + 1.f);
}

/**
 * rounds a floating point number
 * @param[in] value float
 * @return result float
*/
__axmath_inline float axRound(register const float v)
{
   if (v < 0.f)
    return (float)(int)(v - 0.5f);
  else
    return (float)(int)(v + 0.5f); 
}

/**
 * fast modf() for floating point values. <br>
 * returns the fractional part of a floating point number
 * and stores the integer part in the second argument.
 * \code
 * // example:
 * value_pi = 3.141592;
 * float integer;
 * float fraction = axModf(value_pi, &integer);
 * // fraction = 0.141592, integer = 3
 * \endcode
 * @param[in] value float - input variable
 * @param[in] intpart float* - pointer to integer part variable
 * @return float - fractional part
 */
__axmath_inline float axModf(register const float value,
  register float* intpart)
{
  *intpart = (float)(int)value;
  return (value - *intpart);
}

/**
 * returns the remainder of the division of two arguments
 * \code
 * // example:
 * float numerator = 5.3f;
 * float denominator = 2.f;
 * float result = numerator - (floorf(numerator/denominator) * denominator);
 * // result = 1.3
 * \endcode
 * @param[in] x float - numerator (divident)
 * @param[in] y float - denominator (devisor or modulus)
 */
__axmath_inline float axFmod(register const float x, register const float y)
  {
  register float value;
  __asm__
  (
    // gets remainder; copy floating point status word into ax register;
    // copy the ah register into the condition code bits (cc);
    // jump if parity;
    "1: fprem;"    "fstsw %%ax;"   "sahf;"   "jp 1b;"
    : "=t" (value) : "0" (x), "u" (y) : "ax", "cc"
  );
  return value;
}

/**
 * returns the absolute value of a floating point number
 * @param[in] value float
 * @return value float
*/
__axmath_inline float axAbs(register const float value)
{
  // alt: fpu fabs is slower
  register union
  {
    int i;
    float j;
  } u;
  u.j = value;
  u.i &= 0x7fffffff;
  return u.j;
}

/**
 * returns the negative of a floating point number
 * @param[in] value float
 * @return value float
*/
__axmath_inline float axNeg(register float value)
{
  if (value == 0.f)
    return 0.f;
  __asm__ ( "xorl $0x80000000, %0;"    : "=r" (value)    : "0" (value) );
  return value;  
}

/**
 * returns the sign (-1 or 1) of a floating point number
 * @param[in] value float
 * @return value float
*/
__axmath_inline float axSign(register const float v)
{
  register union
  {
    signed int i;
    float f;
  } u;
  u.f = v;
  return (1 | (u.i >> 31));
}

/**
 * returns the sign bit (1 or 0) of a floating point number
 * @param[in] value float
 * @return value float
*/
__axmath_inline unsigned int axSignBit(register const float v)
{
  register union
  {
    signed int i;
    float f;
  } u;
  u.f = v;
  return (unsigned int)(-(u.i >> 31));
}

/**
 * returns the smaller of two floating point numbers
 * @param[in] a float
 * @param[in] b float
 * @return result float
 */
__axmath_inline float axMin(register const float a, register const float b)
{
  return (a < b) ? a : b;
}

/**
 * returns the larger of two floating point numbers
 * @param[in] a float
 * @param[in] b float
 * @return result float
 */
__axmath_inline float axMax(register const float a, register const float b)
{
  return (a > b) ? a : b;
}

/**
 * limits a floating point number to [-limit, limit]
 * @param[in] input float
 * @param[in] limit float
 * @return result float
 */
__axmath_inline float axLimit(register const float input,
  register const float limit)
{
  register const float _t = (input > -limit) ? input : -limit;
  return (_t > limit) ? _t : limit;
}

/**
 * returns the smaller of two integer numbers
 * @param[in] a int
 * @param[in] b int
 * @return result float
 */
__axmath_inline int axMinInt(register const int a, register const int b)
{
  return (a < b) ? a : b;
}

/**
 * returns the larger of two integer numbers
 * @param[in] a int
 * @param[in] b int
 * @return result float
 */
__axmath_inline int axMaxInt(register const int a, register const int b)
{
  return (a > b) ? a : b;
}

/**
 * limits an integer number to [-limit, limit]
 * @param[in] input int
 * @param[in] limit int
 * @return a_or_b float
 */
__axmath_inline int axLimitInt(register const int input,
  register const int limit)
{
  register const int _t = (input > -limit) ? input : -limit;
  return (_t > limit) ? _t : limit;
}

/**
 * calculates the logarithm base 2 of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axLog2f(register float value)
{
  __asm__
  (
    "fld1;"   "fxch;"  "fyl2x;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the logarithm base 2 function
 * based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
 * @param[in] val float
 * @return result float
 */
__axmath_inline float axLog2(register const float val)
{
  if (val > 0.f)
  {
    register union
    {
      int i;
      float j;
    } u;
    u.j = val;
    register const int log_2 = ((u.i >> 23) & 255) - 128;
    u.i &= ~(255 << 23);
    u.i += 127 << 23;
    return (0.05f + u.j + (float)log_2);
  }
  else
    return 0.f;
}

/**
 * calculates the natural logarithm (base e) of a floating point number
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axLogf(register float value)
{
  __asm__
  (
    "fld %0;"    "fldln2;"    "fxch;"    "fyl2x;"
    : "=t" (value)
    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the natural logarithm function
 * based on code from http://www.flipcode.com/archives/Fast_log_Function.shtml
 * @param[in] val float
 * @return result float
 */
__axmath_inline float axLog(register const float &val)
{
  return (axLog2(val)*0.69314718f);
}

/**
 * calculates the logarithm base 10 of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axLog10f(register float value)
{
  __asm__
  (
    "fldlg2;"    "fxch;"    "fyl2x;"
    : "=t" (value): "0" (value)
  );
  return value;
}

/**
 * calculates the logarithm base 10 of a floating point number
 * @param[in] x float
 * @return value float
 */
__axmath_inline float axLog10(register const float x)
{
  // log10(e) = 0.4342945239647
  // also: log10(x) = log2(x) - ln(x)
  register const float y = (x - 1)/(x + 1);
  register const float y2 = y*y;
  return (2.f*y*(1 + y2*0.3333333333f + y2*y2*0.2f))*0.4342945239647f;
}

/**
 * performs fast and accurate powf(float, float) approximation (fpu)
 * optimized for fractional exponent. <br>
 * for (long) integer exponent use axPow() <br>
 * \code
 * axPow(3.5f, 2);
 * axPowf(3.5f, 2.5f);
 * \endcode
 * @param[in] x float - base
 * @param[in] y float - exponent
 * @return float
 */
__axmath_inline float axPowf(register const float x, register const float y)
{
  register float value, exponent;
  __asm__
  (
    "fld1;"                       // |
    "fxch;"                       // |
    "fyl2x;"                      // log2(x)
    "fmul %%st(1);"               // y*log2(x)
    "fst %%st(1);"                // |
    "frndint;"                    // int(y*log2(x))
    "fxch;"                       // |
    "fsub %%st(1);"               // fract(y*log2(x))
    "f2xm1;"                      // 2^(fract(y*log2(x))) - 1
    "fld1;"                       // |
    "faddp;"                      // += 1
    "fscale;"                     // x*(2^exp)
    : "=t" (value), "=u" (exponent)   :"0" (x), "1" (y)
  );
  return value;
}

/**
 * performs fast pow(float, integer)
 * @param[in] x float
 * @param[in] n int
 * @return result float
 */
__axmath_inline float axPow(register float x, register unsigned long n)
{
  register float res = 1;
  while (n > 0)
  {
    if (n & 1)
      res *= x;
    n >>= 1;
    x *= x;
  }
  return res;
}

/**
 * approximation of [e] to the power of a number (fpu)
 * @param[in] x float input value
 * @return value float
 */
__axmath_inline float axExpf(register const float x)
{
  register float value, exponent;
  __asm__
  (
    "fldl2e;"               // e^x = 2^(x*log2(e))
    "fmul %%st(1);"         // x * log2(e)"
    "fst %%st(1);"          // |
    "frndint;"              // int(x*log2(e))
    "fxch;"                 // |
    "fsub %%st(1);"         // fract(x*log2(e))
    "f2xm1;"                // 2^(fract(x*log2(e)))
    "fld1;"                 // |
    "faddp;"                // += 1
    "fscale;"               // x*(2^exp)
    : "=t" (value), "=u" (exponent) : "0" (x)
  );
  return value;
}

/**
 * fast approximation of [e] to the power of a number <br>
 * based on http://theoval.sys.uea.ac.uk/publications/pdf/nc2000a.pdf <br>
 * note: original is for double precision (has a double to float cast)
 * @param[in] exponent float
 * @return result float
 */
__axmath_inline float axExp(register const float v)
{
  register union
  {
    double d;
    struct
    {
      #ifdef LITTLE_ENDIAN
        int i, j;
      #else
        int j, i;
      #endif
    } s;
  } u;
  u.s.i = (int)(1512775*(double)v) + 1072632447;
  u.s.j = 0;
  return (float)u.d;
}

/**
 * returns the result of x*(2^floor(y)) <br>
 * ( significand (x) multiplied by the exponent(2^y) )
 * \code
 * // example:
 * float sig = 2.f;
 * float exponent = 4.1f; // will be truncated to 4.0f
 * float result = axFscale(sig, exponent);
 * // result = 32.f
 * \endcode
 * @param[in] x float - significand
 * @param[in] y float - exponent
 * @return value float
 */
__axmath_inline float axFscale(register const float x, register const float y)
{
  register float value;
  __asm__
  (
    "fscale;"  : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

/**
 * separates the input variable x into significand*(2^exponent)<br>
 * uses IEEE logb(x)
 * \code
 * // example:
 * float value = 3.141592;
 * float exponent;
 * float significand = axFxtract(value, &exponent);
 * // significand = 1.5708, exponent = 1
 * // 3.141592 = 1.5708 * (2^1)
 * // -----------------------------------
 * // NOTE: using frexp() from libm
 * // significand = 0.785398, exponent = 2
 * // 3.141592 = 0.785398 * (2^2)
 * \endcode
 * @param[in] value float - input value
 * @param[in] _exp float* - pointer to the exponent variable
 * @return sig float - value of significand
 */
__axmath_inline float axFxtract(register float value, register float* _exp)
{
  register float sig;
  __asm__
  (
    "fxtract;"   :"=t"(sig), "=u"(*_exp)   :"0"(value)
  );
  return sig;
}

/**
 * rought approximation of a N-th root function (inaccurate) <br>
 * use axPowf(x, 1/n) instead.
 * @param[in] value float
 * @param[in] root long
 * @return value float
 */
__axmath_inline float axNrt(register float value,register  long root)
{
  // alt: newton-ramson is still slower than the fpu axPowf
  // xn = (1/n)*((n-1)*xn + x/pow(xn, n-1));
  __asm__
  (
    "subl $0x3f800000, %0;"    "subl $1, %2;"
    "shrl %b2, %0;"            "addl $0x3f800000, %0;"
    : "=r" (value)
    : "0" (value), "c" (root)
  );
  return value;
}

/**
 * returns the square root of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axSqrtf(register float value)
{
  __asm__
  (
    "fsqrt;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the squre root function <br>
 * based on: http://www.azillionmonkeys.com/qed/sqroot.html
 * @param[in] x float
 * @return value float
 */
__axmath_inline float axSqrt(register const float x)
{
  register const float halfx = x*0.5;
  register union
  {
    int i;
    float j;
  } u;
  u.j = x;
  u.i = (0xbe6ec85f - u.i) >> 1;   // good initial guess
  return x*u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration
}

/**
 * returns the invert squre root of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axInvSqrtf(register float value)
{
  __asm__
  (
    "fsqrt;"  "fld1;"   "fdivp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the invert squre root function
 * based on code found in 'quake 3 arena' by 'id software'
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axInvSqrt(register const float x)
{
  register const float halfx = 0.5f*x;
  register union
  {
    float j;
    int i;
  } u;
  u.j = x;
  u.i = 0x5f3759df - (u.i >> 1); // good initial guess
  return u.j*(1.5f - u.j*u.j*halfx) + 0.001f; // newton iteration
}

/**
 * calculates the sine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axSinf(register float value)
{
  __asm__
  (
    "fsin;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the sine function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axSin(register float x)
{
  x *= (1.2732395447f - 0.4052847345f * axAbs(x));
  return 0.225f * (x * axAbs(x) - x) + x;
}

/**
 * calculates the cosine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axCosf(register float value)
{
  __asm__
  (
    "fcos;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the cosine function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axCos(register const float x)
{
  register const float x2 = x*x;
  return (15120 + x2*(-6900 + 313*x2)) / (15120 + x2*(660 + 13*x2));
}

/**
 * calculates the tangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axTanf(register float value)
{
  __asm__
  (
    "fptan;"  "fstp %1;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the tangens function for range [-pi, pi]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axTan(register const float x)
{
  register const float x2 = x*x;
  return (x*(105 - 10*x2)) / (105 - x2*(45 - x2));
}

/**
 * calculates both the sine and cosine of a floating point number (fpu)
 * \code
 * // example:
 * float sinx;
 * float cosx;
 * axSinCosf(x, &sinx, &cosx);
 * // sinx and cosx will recieve the results
 * \endcode
 * @param[in] x float input variable
 * @param[in] sin float* pointer to sin value
 * @param[in] cos float* pointer to cos value
 * @return void
 */
__axmath_inline void axSinCosf(const float x, float* sin, float* cos)
{
  __asm__
  (
    "fsincos;" : "=t" (*cos), "=u" (*sin) : "0" (x)
  );
}

/**
 * calculates the arc-sine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAsinf(register float value)
{
  // asin(x)=atan(sqrt(x*x/(1-x*x)))
  register float tmp;
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivp;"    "fsqrt;"    "fld1;"    "fpatan;"   "fst %0;"
    : "=t" (value)  : "0" (tmp)
  );
  return value;
}

/**
 * fast approximation of the arc-sine function for range [-1, 1]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axAsin(register const float x)
{
  return PI_2 - axSqrtf(1 - x)*(1.5707288 - x*(0.2121144 + x*(0.0742610 -
  x*(0.0187293 + 0.395*x))));
}

/**
 * calculates the arc-cosine of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAcosf(register float value)
{
  // acos(x) = atan(sqrt((1-x*x)/(x*x)))
  register float tmp;
  __asm__
  (
    "fld %0;"    "fld %0;"    "fmulp;"    "fst %1;"    "fld1;"    "fsubp;"
    "fld %1;"    "fdivrp;"    "fsqrt;"    "fld1;"    "fpatan;"    "fst %0;"
    : "=t" (value)    : "0" (tmp)
  );
  return value;
}

/**
 * fast approximation of the arc-cosine function for range [-1, 1]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axAcos(register const float x)
{
  register const float x2 = x*x;
  return x*(x2*(-0.55*x2 + 0.097) - 1.008) + 1.571;
}

/**
 * calculates the arc-tangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAtanf(register float value)
{
  // from partial tangens
  __asm__
  (
    "fld1;"    "fpatan;"    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * fast approximation of the arc-tangens function for range [-2, 2]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axAtan(register const float x)
{
  const float x2 = x*x;
  return (x*(105 + 55*x2)) / (105 + x2*(90 + 9*x2));
}

/**
 * calculates the angle in radians between the positive x-axis of a plane
 * and the point given by the coordinates (x, y) on it.
 * @param[in] x float - x coordinate
 * @param[in] y float - y coordinate
 * @return value float
 */
__axmath_inline float axAtan2f(register const float y, register const float x)
{
  register float value;
  __asm__
  (
    "fpatan;"    "fld %%st(0);"    : "=t" (value) : "0" (x), "u" (y)
  );
  return value;
}

/**
 * calculates the cotangens of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axCotanf(register float value)
{
  // cotan(x) = 1/tan(x)
  __asm__
  (
    "fsincos;"    "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the cosecant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axCscf(register float value)
{
  // csc(x) = 1/sin(x)
  __asm__
  (
    "fsin;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the secant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axSecf(register float value)
{
  // sec(x) = 1/cos(x)
  __asm__
  (
    "fcos;"   "fld1;"   "fdivrp;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arccotangent of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAcotanf(register float value)
{
  // arccotan(x) = atan(1/x)
  __asm__
  (
    "fld1;"   "fxch;"   "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arcsecant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAsecf(register float value)
{
  // asec(x) = atan(sqrt(x*x-1))
  __asm__
  (
  "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fsqrt;"
  "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * calculates the arccosecant of a floating point number (fpu)
 * @param[in] value float
 * @return value float
 */
__axmath_inline float axAcscf(register float value)
{
  // acsc(x) = atan(sqrt(1/(x*x-1)))
  __asm__
  (
    "fld %0;"   "fmulp;"    "fld1;"   "fsubp;"    "fld1;"   "fdivrp;"
    "fsqrt;"    "fld1;"     "fpatan;"
    : "=t" (value)    : "0" (value)
  );
  return value;
}

/**
 * approximation of the hyperbolic-sine function for range (fpu)
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axSinhf(register const float x)
{
  if (x >= 0.f)
  {
    const float _e = axExpf(x);
    return (_e - 1.0f/_e)*0.5f;
  }
  else
  {
    const float _eneg = axExpf(-x);
    return (1.0f/_eneg - _eneg)*0.5f;
  }
}

/**
 * fast approximation of the hyperbolic-sine function for range [-3.5, 3.5]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axSinh(register const float x)
{
  register const float x2 = x*x;
  return x*(x2*(0.012*x2 + 0.156) + 1.004);
}

/**
 * approximation of the hyperbolic-cosine function (fpu)
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axCoshf(register const float x)
{
  register const float _e = axExpf(axAbs(x));
  return (_e + 1.0f/_e)*0.5f;
}

/**
 * fast approximation of the hyperbolic-cosine function for range [-3.5, 3.5]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axCosh(register const float x)
{
  register const float x2 = x*x;
  return x2*(0.065*x2 + 0.428) + 1.025;
}

/**
 * approximation of the hyperbolic-tangens function for range [-50, 50] (fpu)
 * @param[in] value const float
 * @return result float
 */
__axmath_inline float axTanhf(register const float value)
{
  if (value > 50)
    return 1;
  else if (value < -50)
    return -1;
  else
  {
    register const float _e = axExpf(value);
    register const float _er = 1.f/_e;
    return (_e - _er) / (_e + _er);
  }
}

/**
 * fast approximation of the hyperbolic-tangens function for range [-4.2, 4.2]
 * @param[in] x float
 * @return result float
 */
__axmath_inline float axTanh(register const float x)
{
  register const float x2 = x*x;
  return x*(27 + x2) / (27 + 9*x2);
}

/**
 * calculate the average value of a set of floats
 * example: <br>
 * \code
 * float ar[] = { -1.f, 2.f, 3.f, 4.f, 5.f };
 * float result = axAvrg(5, ar); // result is 2.6f
 * \endcode
 * @param n unsigned int - number of elements (n)
 * @param ar float* - array of floats
 * @return float
 */
__axmath_inline float axAvrg(register const unsigned int n,
  register const float* ar)
{
  register float total = 0;
  register unsigned int i=0;
  while (i<n)
  {
    total += ar[i];   i++;
  }
  return total/n;
}

/**
 * calculate the average value of a set of integers
 * example: <br>
 * \code
 * int ar[] = { -1, 2, 3, 4, 5 };
 * int result = axAvrgInt(5, ar); // result is 2 (truncated)
 * \endcode
 * @param n unsigned int - number of elements (n)
 * @param ar int* - array of integers
 * @return int
 */
__axmath_inline int axAvrgInt(register const unsigned int n,
  register const int* ar)
{
  register int total = 0;
  register unsigned int i=0;
  while (i<n)
  {
    total += ar[i];   i++;
  }
  return total/n;
}

/**
 * calculate the RMS of a set (array) of float numbers
 * example:
 * \code
 * float ar[] = { 1.f, 2.f, 3.f, 4.f, 5.f };
 * float result = axRMS(5, ar);   // result = 3.31662
 * \endcode
 * @param[in] n unsigned int - size of the array
 * @param[in] ar float* - array of floats
 *
 * @return float
 */
__axmath_inline float axRMS(register const unsigned int n,
  register const float* ar)
{
  register float numr = 0;
  register unsigned int i=0;
  while (i<n)
  {
    numr += ar[i]*ar[i];  i++;
  }
  return axSqrtf(numr/n);
}

/**
 * calculate the RMS of a set (array) of int numbers
 * example:
 * \code
 * int ar[] = { 1, -2, 3, 4, 5 };
 * int result = axRMS(5, ar);
 * \endcode
 * @param[in] n unsigned int - size of the array
 * @param[in] ar int* - array of integers
 *
 * @return float
 */
__axmath_inline int axRMSInt(register const unsigned int n,
  register const int* ar)
{
  register int numr = 0;
  register unsigned int i=0;
  while (i<n)
  {
    numr += ar[i]*ar[i];  i++;
  }
  return (int)(axSqrtf((float)(numr/n)));
}

#endif // axMath_included

/**
 * \file axMath.h
 * \brief math approximations and routines
 *
 * axonlib's math header with a collection of optimized functions for trigonometry,
 * algebra and analysis. <br>
 * some of the methods have a 'f' suffix version (e.g.: axTanf).
 * such versions use FPU instructions for the calculation (FPU x87 required).
 * the versions without 'f' suffix are most likely less cpu heavy approximations,
 * but with reduced accuracy and acceptable input range (e.g. axCos). <br>
 * <br>
 * <b>notes:</b> <br> <br>
 * gcc 4.x outperforms 3.x many times with -O2, -O3 <br>
 * you can get gcc 4.x for windows at http://www.tdragon.net/recentgcc/
  <br>
  <br>
  <b>short performance table for axMath.h</b>: <br>
  <br>
  cpu: amd athlon 1800+ <br>
  parameters: -O3 -msse -mfpmath=sse,387 <br>
  input values: pi, -pi, 1, -1, other <br>
  iterations: 10e+5 or more <br>
  <br>
  *some comparisons may use a weighting function to reduce the number
  of iterations <br>
  <br>
  <table>  <tr>  <td>
  axFloor: 5ms <br>  floorf: 25ms <br>
  axRound: 6ms <br>  roundf: 42ms <br>
  axAbs: 36ms <br>  fabs: 63ms <br>
  axLog2: 7ms <br>  axLog2f: 69ms <br>  log2f: 75ms <br>
  axLogf: 20ms <br>  logf: 80ms <br>
  axLog10: 30ms <br>  axLog10f: 69ms <br>  log10f: 75ms <br>
  </td>
  <td>
  axPowf: 96ms <br>  powf: 161ms <br>
  axExp: 3ms <br>  axExpf: 34ms <br>  expf: 137ms <br>
  axSqrt: <1ms <br>  axSqrtf: <1ms <br>  sqrtf: 27ms <br>
  axSin: 56ms <br>  axSinf: 98ms <br>  sinf: 114ms <br>
  axAcosf: <1ms <br>  acosf: 75ms <br>
  axTanhf: 70ms <br>  tanf: 194ms <br>
  </td>  </tr>  </table>
  <br>
  */
