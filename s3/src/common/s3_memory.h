#ifndef s3_memory_included
#define s3_memory_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

void* S3_Malloc_Aligned(uint32 ASize, uint32 AAlignment);
void  S3_Free_Aligned(void *p);

// S3_ClearMem(x)


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#include "common/s3_stdlib.h"

// we can't assert here.. debug is ont oncluded yet (it needs to do some
// #define tricks with memory stuff..
//SAssert( SIsPowerOfTwo(AAlignment) );

//----------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>

void* S3_Malloc_Aligned(uint32 ASize, uint32 AAlignment) {
  void* p1; // original block
  void** p2; // aligned block
  int offset = AAlignment - 1 + sizeof(void*);
  if ((p1 = (void*)S3_Malloc(ASize + offset)) == S3_NULL) return S3_NULL;
  p2 = (void**)(((size_t)(p1) + offset) & ~(AAlignment - 1));
  p2[-1] = p1;
  return p2;
}

//----------

void S3_Free_Aligned(void *p)
{
  S3_Free(((void**)p)[-1]);
}

//----------

#define S3_ClearMem(x) S3_Memset(x,0,sizeof(x))

//----------------------------------------------------------------------
#endif
