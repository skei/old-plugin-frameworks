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
 * @file axUtils.h
 * \brief contains various utility methods.
 */

 ///////////////
 // TODO: CLEAN

#ifndef axUtils_included
#define axUtils_included

#include "axDefines.h"
#include "axMath.h"
#include "axStdlib.h"

#ifdef AX_HOT_INLINE_UTILS
  #define __axutils_inline __hotinline
#else
  #define __axutils_inline inline
#endif

////////////////////////////////////////////////////////////////////////////////

#define axStrExpand(x) #x
/// expand a macro to string
#define axStr(x) axStrExpand(x)

/**
 * swap the values of two variables <br>
 */
#define axSwap(x,y) { typeof(x) tmp = (x);(x) = (y);(y) = (tmp); }

/**
 * axGetArrSize() helper function
 */
template<class T, size_t N> T decay_array_to_subtype(T (&a)[N]);

/**
 * returns the size of an array <br> 
 */
#define axGetArrSize(x) (sizeof(x)/sizeof(decay_array_to_subtype(x)))

/**
 * bit reverse algorithm <br>
 * origin: http://graphics.stanford.edu/~seander/bithacks.html 
 */
__axutils_inline unsigned long axBitReverse (unsigned long v)
{
  unsigned long r = v;
  int s = sizeof(v) * CHAR_BIT - 1;
  for (v >>= 1; v; v >>= 1)
  {
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s;
  return r;
}

/// convert to boolean
#define axBool(x)         (!(!(x)))

/// get bit
#define axBitGet(x, bit)  ( 1  &  ((x)  >> (bit)))

/// set bit
#define axBitSet(x, bit)  ((x) |  (1    << (bit)))

/// clear bit
#define axBitClr(x, bit)  ((x) & ~(1    << (bit)))

/// flip bit
#define axBitFlp(x, bit)  ((x) ^  (1    << (bit)))

/// converts linear value to decibel
#define axLOG2DB(x) ( LOG2DB*axLogf( (x) ) )

/// converts decibel value to linear 
#define axDB2LOG(x) ( axExpf( DB2LOG*(x) ) )

/**
 * sums a set (array) of dBFS values
 * @param[in] n unsigned integer - array length
 * @param[in] ar float* - array of floats
 * @return result float
 */
__axutils_inline float axSumDB(unsigned int n, const float* ar)
{
  float sum = 0.f;
  for (unsigned int i=0; i<n; i++) sum += axDB2LOG(ar[i]);
  return axLogf(sum)*LOG2DB;
}

/// conversation from bandwidth (octaves) to q factor
__axutils_inline float axOctaves2Q(const float n)
{
  const float _pow2n = axPowf(2.f, n);
  return -axSqrtf(_pow2n) / (1.f - _pow2n);
}

/// conversation from q factor to bandwidth (octaves)
__axutils_inline float axQ2Octaves(const float q)
{
  return 1.4426950408889634f * axSinhf(0.5f * (q));
}

/// denormalize input value
__axutils_inline float axDenorm(register float n)
{
  register union
  {
    float f;
    unsigned int i;
  } u;
  u.f = n;
  if ( !(u.i & 0x7f800000) ) n = 0.f;
  return n;
}

/// check for little endian
__axutils_inline unsigned int axLittleEndian (void)
  { const int i = 1; return  (*(char*)&i); }

/// check for big endian
__axutils_inline unsigned int axBigEndian (void)
  { const int i = 1; return !(*(char*)&i); }

/// axRadix
__axutils_inline void axRadix(long* source, long* dest,
  unsigned long N, int byte)
{
  unsigned int i;
  long count[256];
  long index[256];
  
  axMemset(count, 0, sizeof(count));
      
  for (i=0; i<N; i++)
    count[ ((source[i]) >> (byte*8)) & 0xff ]++;
  index[0] = 0;
  for (i=1; i<256; i++)
    index[i] = index[i-1]+count[i-1];
  for (i=0; i<N; i++)
    dest[ index[ ((source[i])>>(byte*8))&0xff ]++ ] = source[i];
}

#endif // axUtils_included
