#ifndef s3_utils_included
#define s3_utils_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------

/*
  // macros
  S3_ArraySize(x)
  S3_BitClear(x,bit)
  S3_BitFlip(x,bit)
  S3_BitGet(x,bit)
  S3_BitSet(x,bit)
  S3_Bool(x)
  S3_Cube(x)
  S3_Deg2Rad(x)
  S3_Inverse(x)
  S3_Rad2Deg(x)
  S3_Square(x)
  S3_Swap(x,y)
  //S3_Db2Lin(dB)
  //S3_Lin2DB(lin)
*/

void        S3_CreateUniqueName(const char* ABuffer, const char* APrefix, void* APtr);
const char* S3_GetFileExt( char* APath);
const char* S3_GetPath( char* APath);
int         S3_IsAlpha(int c);
int         S3_IsDigit(int c);
int         S3_IsPrint(int c);
int         S3_IsSpace(int c);
int         S3_IsUpper(int c);
int         S3_IsXdigit(int c);
int         S3_IsAlnum(int c);
char*       S3_StripFileExt(char* APath);
const char* S3_StripPath(const char* APath);
int         S3_ToLower(int c);
int         S3_ToUpper(int c);
char*       S3_InsertChar(char* ABuffer, int32 APos, char AChar);
char*       S3_DeleteChar(char* ABuffer, int32 APos);

//----------------------------------------------------------------------

#include <math.h> // sqrtf,

//#include "common/s3_stdlib.h"
//#include "common/s3_memory.h"

//----------------------------------------------------------------------
// macros
//----------------------------------------------------------------------

/*
  returns the size of an array
    char a[15];
    int b[SArraySize(a) + 1];
    unsigned int j = SArraySize(b);    // j = 16
    // ----------------
    // NOTE: passing a pointer will not work
    int ar[21];
    int* ptr = ar;
    unsigned int j = SArraySize(ptr); // <- will not work
  param[in] x array type-unsafe
  return unsigned int
*/

template<class T, size_t N> T S3_DecayArrayToSubType(T (&a)[N]);
#define S3_ArraySize(x) (sizeof(x)/sizeof(S3_DecayArrayToSubType(x)))

//

//#define lengthof(x) (sizeof(x)/sizeof(x[0]))

//----------------------------------------------------------------------

// clear bit
#define S3_BitClear(x, bit)  ((x) & ~(1 << (bit)))

// flip bit
#define S3_BitFlip(x, bit)  ((x) ^  (1 << (bit)))

/*
  returns a specific bit of an integer
    int x = 0x0f;
    for(unsigned int i=0; i<32; i++)
    { cout << axGetBit(x, i); }      // 11110000000000000000000000000000
    cout << axGetBit(x, 0);          // 1
    cout << axGetBit(x, 4);          // 0
  x long int - input value
  bit unsigned int - which bit
  return unsigned int - 0x0 / 0x1
*/

// get bit
#define S3_BitGet(x, bit)  ( 1  &  ((x) >> (bit)))

// set bit
#define S3_BitSet(x, bit)  ((x) |  (1 << (bit)))

// convert to boolean
#define S3_Bool(x) (!(!(x)))

// cube of x: (x^3)
#define S3_Cube(x) ((x)*(x)*(x))

/*
  converts decibel value to linear
    float amp_db = 20.f;         // 20 decibels
    signal *= axDB2Lin(amp_db);  // *= ~10.079
*/

//#define S3_Db2Lin(dB) ( SExpf( DB2LOG*(dB) ) )

// convert degrees to radians- rad = deg*(pi/180);
#define S3_Deg2Rad(x) ( (x)*0.01745329251994f )

// invert of x: (1/x)
#define S3_Inverse(x) (1/(x))

/*
  converts linear value to decibel
  \code
  float dc_signal = 1.f;
  float amp_half = 0.5f;
  dc_signal *= amp_half;                  // half the amplitude
  float amp_db = axLin2DB(amp_half);      // amp_db = -6dbFS
  \endcode
  @param[in] lin float
  @return result float
*/

//#define S3_Lin2DB(lin) ( LOG2DB*SLogf( (lin) ) )

//----------

// convert radians to degrees. deg = rad*(180/pi);
#define S3_Rad2Deg(x) ( (x)*57.2957795130823f )

// square of x: (x^2)
#define S3_Square(x) ((x)*(x))

/*
  origin: http://graphics.stanford.edu/~seander/bithacks.html
  note: type of first variable is used
    float x = 5.f;
    float y = 3.f;
    SSwap(x,y);
*/

/*
  c++11 gave an error (typeof not defined in this scope),
  but found a solution here:
  https://gcc.gnu.org/onlinedocs/gcc/Typeof.html
  "If you are writing a header file that must work when included in
  ISO C programs, write __typeof__ instead of typeof."
*/

// swap two values
#define S3_Swap(x,y) { __typeof__(x) tmp = (x); (x) = (y); (y) = (tmp); }


//----------------------------------------------------------------------
// functions
//----------------------------------------------------------------------

/*
  http://rosettacode.org/wiki/Binary_search#Pascal
  Iterative


  Usage:
  var
    list: array[0 .. 9] of real;
    // ...
    indexof := binary_search(123, list);
*/

/*
function binary_search(element: real; list: array of real): integer;
var
    l, m, h: integer;
begin
    l := 0;
    h := High(list) - 1;
    binary_search := -1;
    while l <= h do
    begin
        m := (l + h) div 2;
        if list[m] > element then
        begin
            h := m - 1;
        end
        else if list[m] < element then
        begin
            l := m + 1;
        end
        else
        begin
            binary_search := m;
            break;
        end;
    end;
end;
*/

//----------

// ptr to (reversed) hex

//static __S3_UNUSED
void S3_CreateUniqueName(const char* ABuffer, const char* APrefix, void* APtr) {
  static char _int2hex[17] = "0123456789abcdef";    // +'\0' = 17
  char* buf = (char*)ABuffer;
  while (*APrefix != '\0') /**ABuffer++*/*buf++ = *APrefix++; // memset + update pointers
  //uint32 iptr = (uint32)APtr; // todo: 64-bit (long long?)
  uint64 iptr = (uint64)APtr;
  while (iptr>0) {
    int j = iptr & 0x0f;      // 0..f
    iptr >>= 4;               // /16 (0..f)
    /**ABuffer++*/*buf++ = _int2hex[j]; // to char
  }
  /**ABuffer++*/*buf++ = '\0';
}


//----------

// returns 'exe', not '.exe'

//static __S3_UNUSED
const char* S3_GetFileExt(/*const*/ char* APath) {
  if (APath) {
    char *dot;
    dot = S3_Strrchr(APath,'.');
    if (dot) return dot+1;
  }
  return (char*)"NULL";
}

//----------

// /home/skei/test -> /home/skei/

// modifies content of APath

//static __S3_UNUSED
const char* S3_GetPath(/*const*/ char* APath) {
  if (APath) {
    char *slash, *backslash;
    slash     = S3_Strrchr(APath,'/');
    backslash = S3_Strrchr(APath,'\\');//+1;
    if (slash) *(slash+1) = '\0';
    else if (backslash) *(backslash+1) = '\0'; // see above
    return APath;
  }
  return S3_NULL;
}

//----------

/*static __S3_UNUSED*/  int S3_IsAlpha(int c)   {return (c>='A' && c<='Z') || (c>='a' && c<='z');}
/*static __S3_UNUSED*/  int S3_IsDigit(int c)   {return (c>='0' && c<='9');}
/*static __S3_UNUSED*/  int S3_IsPrint(int c)   {return c>=' ';}
/*static __S3_UNUSED*/  int S3_IsSpace(int c)   {return ((c>=0x09 && c<=0x0D) || (c==0x20));}
/*static __S3_UNUSED*/  int S3_IsUpper(int c)   {return (c>='A' && c<='Z');}
/*static __S3_UNUSED*/  int S3_IsXdigit(int c)  {return (c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f');}
/*static __S3_UNUSED*/  int S3_IsAlnum(int c)   {return S3_IsAlpha(c) || S3_IsDigit(c);}

//----------


//----------

// -masm=intel

/*
uint32 S3_Rdtsc(void) {
  uint32 cputime = 0;
  asm(".intel_syntax noprefix");
  asm ("fwait; rdtsc; mov [cputime], eax;");
  asm(".att_syntax prefix");
  return cputime;
}
*/

//----------

// problem if there is a dot in the path
// todo: find last dot..

// modifies content of APath

//static __S3_UNUSED
char* S3_StripFileExt(char* APath) {
  //DTrace("...stripping...");
  if (APath) {
    char *dot;
    dot = S3_Strrchr(APath,'.');
    //if (dot) return dot + 1;
    if (dot) {
      //DTrace("dot");
      *dot = '\0';//return dot + 1;
      return APath;
    }
  }
  return (char*)"NULL";
}

//----------

// /home/skei/test -> test

//static __S3_UNUSED
const char* S3_StripPath(const char* APath) {
  if (APath) {
    const char *slash, *backslash;
    slash     = S3_Strrchr(APath,'/');
    backslash = S3_Strrchr(APath,'\\');//+1;
    if (slash) return slash + 1;
    else if (backslash) return backslash + 1; // see above
  }
  return (char*)"NULL";
}

//----------

//static __S3_UNUSED
int S3_ToLower(int c) {
  if (c < 'A' || c > 'Z') return c;
  return c+0x20;
}

//----------

//static __S3_UNUSED
int S3_ToUpper(int c) {
  if (c < 'a' || c > 'z') return c;
  return c-0x20;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  assume APos is valid
  // not tested.. (esp for-loop ranges..)
*/

//----------

//static __S3_UNUSED
//void S3_InsertChar(char* ABuffer, int32 APos, char AChar) {
char* S3_InsertChar(char* ABuffer, int32 APos, char AChar) {
//var
//  i,len : longint;
  if (!ABuffer) return ABuffer;
  int32 len = S3_Strlen(ABuffer);
  for (int32 i=(len+1); i>=(APos+1);i--) ABuffer[i] = ABuffer[i-1];
  ABuffer[APos] = AChar;
  return ABuffer;
}

//----------

//static __S3_UNUSED
//void S3_DeleteChar(char* ABuffer, int32 APos) {
char* S3_DeleteChar(char* ABuffer, int32 APos) {
  if (!ABuffer) return ABuffer;
  int32 len = S3_Strlen(ABuffer);
  for (int32 i=APos; i<len; i++) ABuffer[i] = ABuffer[i+1];
  return ABuffer;
}

//----------------------------------------------------------------------
#endif
