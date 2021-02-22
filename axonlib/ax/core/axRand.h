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
  axonlib's builtin random number generators
  64 bit integers are used so you might need to disable the 'long long'
  warning in g++: -Wno-long-long
  
  // more algorithms ?
  
*/

#ifndef axRand_included
#define axRand_included
//------------------------------------------------------------------------------

#include "axDefines.h"
#include "axCpu.h"

#ifdef AX_HOT_INLINE_RAND
  #define __axrand_inline __hotinline
#else
  #define __axrand_inline inline
#endif

#ifndef AX_NO_RAND      // if not defined: use axonlib's axRand() methods

/*
  axRand
  default fast prng
*/

class axRand
{
  private:
    unsigned int _axrnd;

  public:
    axRand(const unsigned int x)
    { _axrnd = x; }
    
    axRand()
    {
      axCpu c;
      _axrnd = (unsigned int)c.rdtsc();
    }
    
    ~axRand() {}
  
    /// set seed
    __axrand_inline void seed (const unsigned int seed)
    { _axrnd = seed; }
    
    /// get random float between 0, 1
    __axrand_inline float rand(void)
    { return (float)((_axrnd *= 16807) & 0x7FFFFFFF) * 4.6566129e-010f; }
    
    /// get random float between 0, x
    __axrand_inline float rand(const float range)
    { return rand() * range; }
    
    /// get random float between -1, 1
    __axrand_inline float randSigned(void)
    { return (float)(_axrnd *= 16807) * 4.6566129e-010f; }
    
    /// get random float between -x, x
    __axrand_inline float randSigned(const float range)
    { return randSigned() * range; }
    
    /// get random integer number between 0, x
    __axrand_inline unsigned int randInt(const unsigned int range=32767)
    { return (unsigned int)(rand() * range); }
    
    /// get random integer from bit range (0, 31)
    __axrand_inline unsigned int randBit(const unsigned int range=16)
    { return ((_axrnd *= 16807) & 0x7FFFFFFF) >> (31 - range); }

};


/*
  axRandAlvo
  slightly better prng based on:
  http://www.number.com.pt/Alvo.html  
*/

#define AXRANDSF_MAX  2147483647L
#define AXALVORND_T   double

class axRandAlvo
{
  private:
    AXALVORND_T x,a;
  public:
    axRandAlvo()
    {
      axCpu c;
      x = ((AXALVORND_T)((unsigned int)c.rdtsc()))*0.000000001f;
      a = x*0.99f;
    }
    
    ~axRandAlvo() {}
    
    __axrand_inline AXALVORND_T rand(const AXALVORND_T range = 1.f)
    {
      a += x;
      x = a*x + 0.5f;
      x -= (unsigned int)x;
      a *= 0.5f;
      return x*range;
    }
    
    __axrand_inline void seed(unsigned int s)
    {
      if (s == 0)
      {
        x = 100.f;
        a = x*0.99f;
        return;
      }
      x = ((AXALVORND_T)(s *= 16807))*0.000000001f;
      a = x*0.99f;
    }
    
    __axrand_inline AXALVORND_T randSigned(const AXALVORND_T range = 1.f)
    {  return (2.f*rand() - 1.f)*range; }
    
    __axrand_inline unsigned int randInt(const unsigned int top = AXRANDSF_MAX)
    { return (unsigned long) (rand()*top); }
    
    __axrand_inline unsigned int randBit(const unsigned int bits = 16)
    { return (unsigned int) (rand()*AXRANDSF_MAX) >> (31 - bits); }
    
};

/*
  ------------------------------------------------------------------------------
  axRandSinf
  - non-uniform, trigonometric, "negative" distribution
    (more values towards min, max)
  - uses class constructor
  - recursive
  - seeded with "rdtsc" on initialization
  - thread unsafe
  - returns integer or floats in specified ranges
  author: lubomir i. ivanov (lii) for axonlib
  ------------------------------------------------------------------------------
*/

#define AXRANDSINF_MAX    3.40282347e+38F

#define _AXRANDSINF \
  if (x >= AXRANDSINF_MAX ) x = 0.f; \
  __asm__ \
  ( \
    "fld1;" \
    "faddp;" \
    "fxch;" \
    "fmul %%st(1);" \
    "fsin;" \
    : "=t" (y), "=u" (x) : "0" (x), "1" (y) \
  )

#define _AXRANDSINF_SHIFT \
  (y + 1.f) * 0.5f

class axRandSinf
{
  private:
    float x, y;
  
  public:
    axRandSinf(void)
    {
      axCpu c;
      unsigned int _x = (unsigned int)c.rdtsc();
      x = (float)(_x >> 16);
      y = 1.f;
    }
      
    axRandSinf(const unsigned int _x)  { x = (float)_x; }
    axRandSinf(const float _x)          { x = _x;        }

    ~axRandSinf() {}

    __axrand_inline float rand(void)
    { _AXRANDSINF; return _AXRANDSINF_SHIFT; }

    __axrand_inline float rand(const float _top)
    { _AXRANDSINF; return _top * _AXRANDSINF_SHIFT; }

    __axrand_inline float randSigned(void)
    { _AXRANDSINF; return y; }

    __axrand_inline unsigned int randInt(void)
    { _AXRANDSINF;  return (unsigned int) (AXRANDSF_MAX * _AXRANDSINF_SHIFT); }

    __axrand_inline unsigned int randInt(const unsigned int _top)
    { _AXRANDSINF; return (unsigned int) ( _top * _AXRANDSINF_SHIFT ); }
};

#else // AX_NO_RAND

/*
  ------------------------------------------------------------------------------
  stdlib based methods rand()
  ------------------------------------------------------------------------------
*/

#include "math.h"
#include "time.h"
#include "stdarg.h"
#include "stdlib.h"

__deprecated ___axrand_inline void axRandomize
  (const int aSeed = (unsigned int)time(0))
{ srand(aSeed); }

__deprecated __axrand_inline float axRand(const float f = 1)
{ return (f * (float)rand() / (float)RAND_MAX); }

__deprecated __axrand_inline float axRand(const float aLow, const float aHigh)
{ return aLow + ( (float)rand() / (float)RAND_MAX ) *(aHigh - aLow); }

__deprecated __axmath_inline int axRandInt(const int i=RAND_MAX)
{ return axMinInt
    (i, (int)axFloor( (i + 1) * (float)rand() / (float)RAND_MAX ) ); }

__deprecated __axrand_inline float axRandSigned(void)
{ return (2 * (float)rand() / (float)RAND_MAX) - 1; }

#endif // AX_NO_RAND

#endif // axRand_included
