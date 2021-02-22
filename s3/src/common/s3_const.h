#ifndef s3_const_included
#define s3_const_included
//----------------------------------------------------------------------


//----------
//
//----------------------------------------------------------------------

#define S3_MAGIC            0x53330000    /* 'S3' + 0x0000 */
#define S3_MAGIC_USER       0x73330000    /* 's3' + 0x0000 */

#ifdef S3_CPP11
  #define S3_NULL           nullptr
#else
  #define S3_NULL           0
#endif

#define S3_EOF             -1
#define S3_OK               0
#define S3_ERROR           -1
#define S3_ALIGNMENT_SIMD   16
#define S3_ALIGNMENT_CACHE  64

//#define S3_MAX_PATH_SIZE    512
//#define S3_MAX_STRING_SIZE  256

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define S3_PI           3.14159265358f
#define S3_PI2          (S3_PI*2.0f)
#define S3_PI05         (S3_PI*0.5f)

#define S3_INVPI        (1.0f/S3_PI)
#define S3_INVPI2       (1.0f/S3_PI2)
#define S3_INVPI05      (1.0f/S3_PI05)

#define S3_TINY         (0.001f)
#define S3_VERY_TINY    (0.00001f)

#define S3_INV360       (1/360)
#define S3_INV12        (1/12)
#define S3_INV16        (1/16)
#define S3_INV32        (1/16)
#define S3_INV127       (1/127)
#define S3_INV256       (1/256)
#define S3_INV8192      (1/8192)

#define S3_INV360F      (1.0f/360.0f)
#define S3_INV12F       (1.0f/12.0f)
#define S3_INV16F       (1.0f/16.0f)
#define S3_INV32F       (1.0f/32.0f)
#define S3_INV127F      (1.0f/127.0f)
#define S3_INV256F      (1.0f/256.0f)
#define S3_INV8192F     (1.0f/8192.0f)

//#define S3_INVRANDMAX   (1.0f / (float)RAND_MAX)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define S3_CHAR_BIT       8
#define S3_CHAR_MAX       0x7f                /* 127 */
#define S3_SHORT_MAX      0x7fff              /* 32767 */
//#define S3_WCHAR_MAX      65535U
#define S3_INT32_MAX      0x7fffffff          /* 2147483647 */
#define S3_UINT32_MAX     0xffffffff          /* 8589934591U */
#define S3_IN64_MAX       0x7fffffffffffffff  /* 9223372036854775807LL */
#define S3_UINT64_MAX     0x7fffffffffffffff  /* 18446744073709551615ULL */
#define S3_FLOAT_MAX      3.40282347e+38F
#define S3_FLOAT_MIN      1.17549435E-38F
#define S3_FLOAT_EPSILON  1.19209290E-7F
#define S3_FLOAT_DENORM   1.40129846E-45F
#define S3_DOUBLE_MAX     1.7976931348623157e+308
#define S3_DOUBLE_MIN     2.2250738585072014E-308

#define S3_DOUBLE_EPSILON 2.2204460492503131E-16
#define S3_DOUBLE_DENORM  4.9406564584124654E-324
//#define S3_LDBL_MAX       1.18973149535723176502e+4932L
//#define S3_LDBL_MIN       3.36210314311209350626E-4932L
//#define S3_LDBL_EPSILON   1.08420217248550443401E-19L
//#define S3_LDBL_DENORM    3.64519953188247460253E-4951L

//----------

#ifdef S3_32BIT
  #define S3_INT_MAX     S3_INT32_MAX   // 2147483647L
  #define S3_UINT_MAX    S3_UINT32_MAX  // 8589934591UL
#endif

#ifdef S3_64BIT
  #define S3_INT_MAX     S3_INT64_MAX   // 9223372036854775807L
  #define S3_UINT_MAX    S3_UINT64_MAX  // 18446744073709551615UL
#endif

//----------

#define S3_RANDMAX    RAND_MAX
#define S3_INVRANDMAX (1/S3_RANDMAX)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// hmmm.. should this be here?
// do we have any place for "global/static data"?

static
const char S3_HEX_TABLE[17] = "0123456789ABCDEF";    // +'\0' = 17

//----------------------------------------------------------------------
#endif
