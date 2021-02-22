#ifndef s3_convert_included
#define s3_convert_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

char* S3_IntToString(char* _st, int n, unsigned int maxlen = 10, unsigned int base = 10, unsigned int fg = 0);
int   S3_StringToInt(const char* s);
char* S3_FloatToString(char* st, double f, int maxlen = 5, const unsigned int fg = 0);
float S3_StringToFloat(char* s);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#include "common/s3_utils.h"

//----------------------------------------
// int to string
//----------------------------------------

/*
  h_Itoa(string, intnumber, maximumchars, base, flag)

  base: 16 = hex, 10 = dec, 2 = bin, etc.

  flag (works only for base = 10):
  0 - show minus sign only
  1 - show minus or plus sign
  2 - show minus sign or indent positive values with one space

  'maximumchars' is the maximum number of characters including the
  sign. maximum is 33, default is 10
  for base 2, intnumber=2147483647 requires string length=33
  for base 10 use 'maximumchars' = N to show N chars max
  (e.g. -9999, +9999)
  allocated memory for 'string' should be >= 'maximumchars'
*/

//static __S3_UNUSED
char* S3_IntToString(char* _st, int n, unsigned int maxlen, unsigned int base, unsigned int fg)  {
  if (!_st || maxlen > 33) return (char*)"0";
  unsigned int v, p = 1;
  int i = 0;
  char* st = _st;
  char _t[33];
  char* t = _t;
  v = (unsigned int) n;
  if (base == 10) {
    if (n < 0) {
      v = -n;
      *st++ = '-';
      maxlen--;
    }
    else {
      if (fg == 1) { *st++ = '+'; maxlen--; }
      if (fg == 2) { *st++ = ' '; maxlen--; }
    }
  }
  while (i < (int)maxlen) {
    p *= 10;
    i++;
  }
  if (base == 10 && v >= (p-1)) {
    while (maxlen--)
      *st++ = '9';
  }
  else {
    while (v || t == _t) {
      i = v % base;
      v = v / base;
      if (i < 10) *t++ = '0' + i;
      else *t++ = 'A' + i - 10;
    }
    while (t > _t) *st++ = *--t;
  }
  *st = 0;
  return _st;
}

//----------------------------------------
// string to int
//----------------------------------------

//static __S3_UNUSED
int S3_StringToInt(const char* s)
{
  if (!s) return 0;
  const char digits[] = "0123456789";
  unsigned val = 0;
  int neg = 0;
  while (*s == ' ' || *s == '\t') s++;
  if (*s == '-') {
    neg = 1;
    s++;
  }
  else if (*s == '+') s++;
  while (*s) {
    const char *w;
    unsigned digit;
    w = S3_Strchr(digits, *s);
    if (!w) break;
    digit = (w - digits);
    val = val*10 + digit;
    s++;
  }
  if (neg) return -val;
  return val;
}

//----------------------------------------
// float to string
//----------------------------------------

/*
 * h_Ftoa(string, floatnumber, maximumchars, flag)
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

//static __S3_UNUSED
char* S3_FloatToString(char* st, double f, int maxlen, const unsigned int fg) {
  if (!st) return (char*)"0";
  char* ret = st;
  register int exp = 0;
  register int z; // CppCheck: scope of z can be reduced
  int j = 0;
  if (f < 0) {
   *st++ = '-';
   j++;
   f = -f;
  }
  else {
    if (fg == 1) { *st++ = '+';  j++; }
    if (fg == 2) { *st++ = ' ';  j++; }
  }
  if (f) {
    while (f < 1.f) {
      f *= 10.f;
      exp--;
    }
    while ( (f >= 10.f && exp < maxlen) || exp < 0 ) {
      f *= 0.1f;
      exp++;
    }
  }
  if (exp > maxlen - ( j + 1 )) {
    maxlen -= j;
    while (maxlen--) *st++ = '9';
  }
  else {
    while ( (exp > 0) && (exp <= maxlen) && j < maxlen-1 ) {
      *st++ = '0' + (char)f;
      z = (int)f;
      f -= z;
      f *= 10.f;
      exp--;
      j++;
    }
    *st++ = '0' + (char)f;
    z = (int)f;
    f -= z;
    f *= 10.f;
    j++;
    if (j < maxlen-1) {
      *st++ = '.';
      j++;
      register unsigned int i = 0;
      while (j < maxlen) {
        *st++ = '0' + (char)f;
        z = (int)f;
        f -= z;
        f *= 10.f;
        i++;
        j++;
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
      *st++ = '-';
      exp = -exp;
    }
    else *st++ = '+';
    register int expd10 = exp/10;
    *st++ = '0' + expd10;
    *st++ = '0' + (exp -= expd10 * 10);
  }
  */
  *st++ = 0;
  return ret;
}

//----------------------------------------
// string to float
//----------------------------------------

//static __S3_UNUSED
float S3_StringToFloat(char* s) {
  if (!s) return 0.f;
  register float a = 0.f;
  register int e = 0;
  register unsigned int c;
  float _asign = 1.f;
  if ( s[0] == '-' ) {
    _asign = -1.f;
    s++; //*s++;
  }
  while ( (c = *s++) != '\0' && S3_IsDigit(c) )
    a = a*10.f + (c - '0');
  if (c == '.')
    while ( (c = *s++) != '\0' && S3_IsDigit(c) ) {
      a = a*10.f + (c - '0');
      e = e-1;
    }
  if (c == 'e' || c == 'E') {
    int sign = 1;
    register int i = 0;
    c = *s++;
    if (c == '+') c = *s++;
    else if (c == '-') {
      c = *s++;
      sign = -1;
    }
    while ( S3_IsDigit(c) ) {
      i = i*10 + (c - '0');
      c = *s++;
    }
    e += i*sign;
  }
  while (e > 0) {
    a *= 10.f;
    e--;
  }
  while (e < 0) {
    a *= 0.1f;
    e++;
  }
  return a*_asign;
}

//----------------------------------------------------------------------
#endif
