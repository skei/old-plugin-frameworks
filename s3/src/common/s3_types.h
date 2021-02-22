#ifndef s3_types_included
#define s3_types_included
//----------------------------------------------------------------------

#include <stdint.h> // intptr_t
#include "common/s3_attributes.h"

#define S3_Sample     float
#define S3_SampleSize sizeof(S3_Sample)

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned long       uint32;
typedef unsigned long long  uint64;

typedef signed char         int8;
typedef signed short        int16;
typedef signed long         int32;
typedef signed long long    int64;

typedef intptr_t            intptr;

//----------------------------------------
// aligned
//----------------------------------------

typedef double              _S3_ALIGNED(8) double_8;
typedef long long           _S3_ALIGNED(8) longlong_8;

//----------------------------------------
// aliasing
//----------------------------------------

typedef void*               _S3_MAY_ALIAS void_ptr_a;
typedef long double         _S3_MAY_ALIAS ldouble_a;
typedef double              _S3_MAY_ALIAS double_a;
typedef float               _S3_MAY_ALIAS float_a;
typedef long long           _S3_MAY_ALIAS longlong_a;
typedef unsigned long long  _S3_MAY_ALIAS ulonglong_a;
typedef long                _S3_MAY_ALIAS long_a;
typedef unsigned long       _S3_MAY_ALIAS ulong_a;
typedef int                 _S3_MAY_ALIAS int_a;
typedef unsigned int        _S3_MAY_ALIAS uint_a;
typedef short               _S3_MAY_ALIAS short_a;
typedef unsigned short      _S3_MAY_ALIAS ushort_a;
typedef char                _S3_MAY_ALIAS char_a;
typedef unsigned char       _S3_MAY_ALIAS uchar_a;

//----------------------------------------

#ifdef S3_32BIT
  //#define S3_LONG_MAX   2147483647L
  //#define S3_ULONG_MAX  8589934591UL
#endif

#ifdef S3_64BIT
  //#define S3_LONG_MAX   9223372036854775807L
  //#define S3_ULONG_MAX  18446744073709551615UL
#endif

//----------------------------------------------------------------------
#endif
