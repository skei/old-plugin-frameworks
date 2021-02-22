#ifndef s3_interger_included
#define s3_interger_included
//----------------------------------------------------------------------

#include <math.h> // sqrtf

/*
  calculate the average value of a set of integers
  example:
    int ar[] = { -1, 2, 3, 4, 5 };
    int result = SAvrgInt(5, ar); // result is 2 (truncated)
  n - number of elements (n)
  ar - array of integers
  return int
*/

int S3_AverageI(const unsigned int n, const int* ar) {
  int total = 0;
  unsigned int i=0;
  while (i<n) {
    total += ar[i];   i++;
  }
  return total/n;
}

//----------

/*
  bit reverse algorithm
  origin: http://graphics.stanford.edu/~seander/bithacks.html
    unsigned int b = 0x0000000a;
    unsigned int result = axBitReverse(b); // result = 0xa0000000;
*/

uint32 S3_BitReverse(uint32 v) {
  uint32 r = v;
  int s = sizeof(v) * S3_CHAR_BIT - 1;
  for (v >>= 1; v; v >>= 1) {
    r <<= 1;
    r |= v & 1;
    s--;
  }
  r <<= s;
  return r;
}

//----------

// limits an integer number to [-limit, limit]

int S3_ClampI(const int input, const int limit) {
  const int _t = (input > -limit) ? input : -limit;
  //return (_t > limit) ? _t : limit;
  return (_t < limit) ? _t : limit;
}

//----------

// http://bits.stephan-brumme.com/isPowerOfTwo.html

bool S3_IsPowerOfTwo(uint32 x) {
  return ((x&(x-1))==0);
}

//----------

// returns the larger of two integer numbers

int S3_MaxI(const int a, const int b) {
  return (a > b) ? a : b;
}

//----------

// returns the smaller of two integer numbers

int S3_MinI(const int a, const int b) {
  return (a < b) ? a : b;
}

//----------

// http://bits.stephan-brumme.com/roundUpToNextPowerOfTwo.html

uint32 S3_NextPowerOfTwo(uint32 x) {
  x--;
  x |= x >> 1;  // handle  2 bit numbers
  x |= x >> 2;  // handle  4 bit numbers
  x |= x >> 4;  // handle  8 bit numbers
  x |= x >> 8;  // handle 16 bit numbers
  x |= x >> 16; // handle 32 bit numbers
  x++;
  return x;
}

//----------

// http://eli.thegreenplace.net/2004/07/30/a-cool-algorithm-for-counting-ones-in-a-bitstring/

uint32 S3_NumBits(uint32 num) {
  uint32 count = 0;
  while (num) {
    num &= (num-1);
    ++count;
  }
  return count;
}

//----------

/*
  calculate the RMS of a set (array) of int numbers
  example:
    int ar[] = { 1, -2, 3, 4, 5 };
    int result = SRMSI(5, ar);
  n - size of the array
  ar - array of integers
*/

int S3_RmsI(const unsigned int n, const int* ar) {
  int numr = 0;
  unsigned int i=0;
  while (i<n) {
    numr += ar[i]*ar[i];
    i++;
  }
  return (int)(sqrtf((float)(numr/n)));
}

//----------------------------------------------------------------------
#endif
