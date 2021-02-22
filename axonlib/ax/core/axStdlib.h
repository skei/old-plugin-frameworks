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

//------------------------------------------------------------------------------

/*
  ==============================================================================
  axStdlib.h
    a collection of optimized methods for memory and string manipulation
    to replace the default ones in C.

    code in open source projects like MINGW, MINIX, DJGPP and GCC used for
    reference.

    for more information remove the "ax" prefix from a method's name
    and use google to search.

  ==============================================================================
  NOTE:
    all methods are tested (mingw32-gcc-tdm-4.4.1) with the examples
    from the website "cplusplus.com" and should work as expected.
    not much error checking going on, so the methods should be used properly
    (by the book), with "low level" programming precautions in mind.

  ==============================================================================
  TODO:
    - slighly better error handling for some of the methods
  ==============================================================================
*/

#ifndef axStdlib_included
#define axStdlib_included

#include "axDefines.h"
// #include "axMalloc.h" // axStrdup!

#ifdef AX_HOT_INLINE_STDLIB
  #define __axstdlib_inline __hotinline
#else
  #define __axstdlib_inline inline
#endif

/*
  ------------------------------------------------------------------------------
  general
  ------------------------------------------------------------------------------
*/

/**
  * axIsDigit
  */
#define axIsDigit(c)  ( ( (c) > 47 && (c) < 58 ) ? 1 : 0 )

/**
  * axIsLetter
  */
#define axIsLetter(c) \
( ( ((c) > 64 && (c) < 91) || ((c) > 96 && (c) < 123) ) ? 1 : 0 )

/*
  ------------------------------------------------------------------------------
  memory
  ------------------------------------------------------------------------------
*/

/**
 * axMemchr
 */
__axstdlib_inline void* axMemchr (register const void* src, int c,
  unsigned int len)
{
  register const unsigned char* _src = (const unsigned char*) src;
  c &= 0xff;
  while (len--)
  {
    if (*_src == c)
      return (char*) _src;
    _src++;
  }
  return NULL;
}

/**
 * axMemcmp
 */
__axstdlib_inline unsigned int axMemcmp (register const void* m1,
  register const void* m2, register unsigned int n)
{
  register unsigned char* s1 = (unsigned char*) m1;
  register unsigned char* s2 = (unsigned char*) m2;
  while (n--)
  {
    if (*s1 != *s2)
      return *s1 - *s2;
    s1++;
    s2++;
  }
  return 0;
}

/**
 * axMemcpy
 */
__axstdlib_inline void* axMemcpy (register void* dest, register const void* src,
  register unsigned int len)
{
  register char* _d = (char*) dest;
  register char* _s = (char*) src;
  while (len--)
    *_d++ = *_s++;
  return dest;
}

/**
 * axMemmove
 */
__axstdlib_inline void* axMemmove (register void* dst, register const void* src,
  unsigned int len)
{
  register char* _dst = (char*) dst;
  register const char* _src = (char*) src;
  if (_src < _dst && _dst < _src + len)
  {
    _src += len;
    _dst += len;
    while (len--)
      *--_dst = *--_src;
  }
  else
    while (len--)
      *_dst++ = *_src++;
  return dst;
}

/**
 * axMemset
 */
__axstdlib_inline void* axMemset (register void* dest, register int val,
  register unsigned int len)
{
  register unsigned char _v = (unsigned char) val;
  register char* _d = (char*) dest;
  while (len--)
    *_d++ = _v;
  return dest;
}

/*
  ------------------------------------------------------------------------------
  string
  ------------------------------------------------------------------------------
*/

/**
 * axStrlen
 */
__axstdlib_inline unsigned int axStrlen (register char* str)
{
  register unsigned int num = 0;
  while (*str++)
    num++;
  return num;
}

/**
 * axStrcpy
 */
__axstdlib_inline char* axStrcpy (register char* dest, register const char* src)
{
  while ( (*dest++ = *src++) );
  return dest;
}

/**
 * axStrncpy
 */
__axstdlib_inline char* axStrncpy (register char* dest,
  register const char* src, unsigned int n)
{
  while ( n-- && (*dest++ = *src++) != '\0' );
  return dest;
}

/**
 * axStrdup 
 *  
 */
 
/*
__axstdlib_inline char* axStrdup (register char* src)
{
  register char* dst = (char*) axMalloc(axStrlen(src) + 1);
  if (!dst) return NULL;
  axStrcpy (dst, src);
  return dst;
}
*/

/**
 * axStrchr
 */
__axstdlib_inline char* axStrchr (register const char* str,
  register const int c)
{
  while (*str && *str != c)
    str++;
  if (*str != c)
    str = NULL;
  return (char*) str;
}

/**
 * axStrrchr
 */
__axstdlib_inline char* axStrrchr (register const char* s, const int c)
{
  char* p = NULL;
  while (*s++)
    if (*s == c)
      p = (char*) s;
  return p;
}

/**
 * axStrcmp
 */
__axstdlib_inline int axStrcmp (register const char* s1,
  register const char* s2)
{
  while (*s1 != '\0' && *s1 == *s2)
  { s1++; s2++; }
  return (*(unsigned char *) s1) - (*(unsigned char*) s2);
}

/**
 * axStrncmp
 */
__axstdlib_inline int axStrncmp (register const char* s1,
  register const char* s2, register unsigned int n)
{
  register unsigned char u1, u2;
  while (n--)
  {
    u1 = (unsigned char) *s1++;
    u2 = (unsigned char) *s2++;
    if (u1 != u2)
      return u1 - u2;
    if (u1 == '\0')
      return 0;
  }
  return 0;
}

/**
 * axStrcat
 */
__axstdlib_inline const char* axStrcat (register char* s1,
  register const char* s2)
{
  while (*s1)
    s1++;
  while ((*s1++ = *s2++));
  return s1;
}

/**
 * axStrncat
 */
__axstdlib_inline char* axStrncat (register char* s1, register char* s2,
  register unsigned int n)
{
  register char* s = s1;
  while (*s1)
    s1++;
  while ( n-- && (*s1++ = *s2++) )
    if (n == 0)
      *s1 = '\0';
  return s;
}

/**
 * axStrstr
 */
__axstdlib_inline char* axStrstr (register const char* s1,
  register const char* s2)
{
  register unsigned int i;
  while (*s1++)
  {
    i = 0;
    while (1)
    {
      if (!s2[i])
        return (char*) s1;
      if (s2[i] != s1[i])
        break;
      i++;
    }
  }
  return (char*) NULL;
}

/**
 * axStrspn
 */
__axstdlib_inline unsigned int axStrspn (register const char* s1,
  register const char* s2)
{
  register const char *_s1, *_s2;
  _s1 = s1;
  while (*_s1)
  {
    _s2 = s2;
    while (*_s2 && *_s2 != *_s1)
      _s2++;
    if (*_s2 == '\0')
      break;
    _s1++;
  }
  return _s1 - s1;
}

/**
 * axStrcspn
 */
__axstdlib_inline unsigned int axStrcspn (register const char* s1,
  register const char* s2)
{
  register const char *_s1, *_s2;
  _s1 = s1;
  while (*_s1)
  {
    _s2 = s2;
    while (*_s2 != *_s1 && *_s2)
      _s2++;
    if (*_s2)
      break;
    _s1++;
  }
  return _s1 - s1;
}

/**
 * axStrpbrk
 */
__axstdlib_inline char* axStrpbrk (register const char* s1,
  register const char* s2)
{
  register const char* _s1;
  while (*s1++)
  {
    _s1 = s2;
    while (*_s1 && *_s1 != *s1)
      _s1++;
    if (*_s1)
      return (char*) s1;
  }
  return (char*) NULL;
}

/**
 * axStrtok
 */
__axstdlib_inline char* axStrtok (register char* str, const char* spr)
{
  register char *s1, *s2;
  // static buffer = kaboom ?
  static char* _save;
  // --
  if (str == NULL)
  {
    str = _save;
    if (str == NULL)
      return (char*) NULL;
  }
  s1 = str + axStrspn(str, spr);
  if (*s1 == '\0')
    return (char*) (_save = NULL);
  s2 = axStrpbrk(s1, spr);
  if (s2 != NULL)
    *s2++ = '\0';
  _save = s2;
  return s1;
}

/**
 * axStrrev
 */
__axstdlib_inline char* axStrrev (register char* str)
{
  register unsigned int i = 0;
  register unsigned int j = axStrlen(str)-1;
  while (i < j)
  {
    str[i] ^= str[j];
    str[j] ^= str[i];
    str[i] ^= str[j];
    ++i;
    --j;
  }
  return str;
}

/*
  ------------------------------------------------------------------------------
  conversations
  ------------------------------------------------------------------------------
*/

/*
 * axItoa(string, intnumber, maximumchars, base, flag)
 *
 * base: 16 = hex, 10 = dec, 2 = bin, etc.
 *
 * flag (works only for base = 10):
 * 0 - show minus sign only
 * 1 - show minus or plus sign
 * 2 - show minus sign or indent positive values with one space
 *
 * 'maximumchars' is the maximum number of characters including the
 * sign. maximum is 33, default is 10
 * for base 2, intnumber=2147483647 requires string length=33
 * for base 10 use 'maximumchars' = N to show N chars max
 * (e.g. -9999, +9999)
 * allocated memory for 'string' should be >= 'maximumchars'
 *
 */
__axstdlib_inline char* axItoa (register char* _st, int n,
  unsigned int maxlen = 10, unsigned int base = 10, unsigned int fg = 0)
{
  if (!_st || maxlen > 33)
    return (char*)"0";
  register unsigned int v, p = 1;
  register int i = 0;
  char* st = _st;
  char _t[33];
  register char* t = _t;
  v = (unsigned int) n;
  if (base == 10)
  {
    if (n < 0)
    {
      v = -n;
      *st++ = '-';
      maxlen--;
    }
    else
    {
      if (fg == 1) { *st++ = '+'; maxlen--; }
      if (fg == 2) { *st++ = ' '; maxlen--; }
    }
  }
  while (i < (int)maxlen)
  {
    p *= 10;
    i++;
  }
  if (base == 10 && v >= (p-1))
  {
    while (maxlen--)
      *st++ = '9';
  }
  else
  {
    while (v || t == _t)
    {
      i = v % base;
      v = v / base;
      if (i < 10)
        *t++ = '0' + i;
      else
        *t++ = 'a' + i - 10;
    }
    while (t > _t)
      *st++ = *--t;
  }
  *st = 0;
  return _st;
}

/**
 * axAtoi
 */
__axstdlib_inline int axAtoi (register const char* s)
{
  if (!s)
    return 0;
  const char digits[] = "0123456789";
  register unsigned val = 0;
  register int neg = 0;
  while (*s == ' ' || *s == '\t')
    s++;
  if (*s == '-')
  {
    neg = 1; s++;
  }
  else if (*s == '+')
    s++;
  while (*s)
  {
    const char *w;
    unsigned digit;
    w = axStrchr(digits, *s);
    if (!w)
      break;
    digit = (w - digits);
    val = val*10 + digit;
    s++;
  }
  if (neg)
    return -val;
  return val;
}

/*
 * axFtoa(string, floatnumber, maximumchars, flag)
 *
 * flag:
 * 0 - show minus sign only
 * 1 - show minus or plus sign
 * 2 - show minus sign or indent positive values with one space
 *
 * 'maximumchars' is the maximum number of characters including the
 * sign and decimal point.
 * allocated memory for 'string' should be >= 'maximumchars'
 *
 */
__axstdlib_inline char* axFtoa (register char* st, register double f,
  register int maxlen = 5, const unsigned int fg = 0) //, const bool e = false)
{
  if (!st)
    return (char*)"0";
  char* ret = st;
  register int exp = 0;
  register int z;
  int j = 0;
  if (f < 0)
  {
   *st++ = '-';   j++;    f = -f;
  }
  else
  {
    if (fg == 1) { *st++ = '+';  j++; }
    if (fg == 2) { *st++ = ' ';  j++; }
  }
  if (f)
  {
    while (f < 1.f)
    {
      f *= 10.f;   exp--;
    }
    while ( (f >= 10.f && exp < maxlen) || exp < 0 )
    {
      f *= 0.1f;  exp++;
    }
  }
  if (exp > maxlen - ( j + 1 ))
  {
    maxlen -= j;
    while (maxlen--)
      *st++ = '9';
  }
  else
  {
    while ( (exp > 0) && (exp <= maxlen) && j < maxlen-1 )
    {
      *st++ = '0' + (char)f;
      z = (int)f;  f -= z;  f *= 10.f;  exp--;  j++;
    }
    *st++ = '0' + (char)f;
    z = (int)f;   f -= z;   f *= 10.f;   j++;
    if (j < maxlen-1)
    {
      *st++ = '.';  j++;
      register unsigned int i = 0;
      while (j < maxlen)
      {
        *st++ = '0' + (char)f;
        z = (int)f;   f -= z;    f *= 10.f;    i++;    j++;
      }
    }
  }
  /*
  // note: exponent output is disabled. instead it writes the maximum integer.
  if (exp != 0 && e)
  {
    *st++ = 'e';
    if (exp < 0)
    {
      *st++ = '-'; exp = -exp;
    }
    else
      *st++ = '+';
    register int expd10 = exp/10;
    *st++ = '0' + expd10;
    *st++ = '0' + (exp -= expd10 * 10);
  }
  */
  *st++ = 0;
  return ret;
}

/**
  * axAtof
  */
__axstdlib_inline float axAtof (register char* s)
{
  if (!s)
    return 0.f;
  register float a = 0.f;
  register int e = 0;
  register unsigned int c;
  float _asign = 1.f;
  if ( s[0] == '-' )
  {
    _asign = -1.f; *s++;
  }
  while ( (c = *s++) != '\0' && axIsDigit(c) )
    a = a*10.f + (c - '0');
  if (c == '.')
    while ( (c = *s++) != '\0' && axIsDigit(c) )
    {
      a = a*10.f + (c - '0');
      e = e-1;
    }
  if (c == 'e' || c == 'E')
  {
    int sign = 1;
    register int i = 0;
    c = *s++;
    if (c == '+')
      c = *s++;
    else if (c == '-')
    {
      c = *s++;
      sign = -1;
    }
    while ( axIsDigit(c) )
    {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }
  while (e > 0)
  {
    a *= 10.f;
    e--;
  }
  while (e < 0)
  {
    a *= 0.1f;
    e++;
  }
  return a*_asign;
}

//------------------------------------------------------------------------------
#endif
