#ifndef kode_buffer_included
#define kode_buffer_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------
// static 
//----------------------------------------------------------------------

template <class TYPE, int SIZE>
class KODE_StaticBuffer {
  
//------------------------------
private:
//------------------------------

  TYPE MBuffer[SIZE];
  
//------------------------------
public:
//------------------------------

  TYPE      getBuffer()     { return MBuffer; }
  uint32_t  getBufferSize() { return SIZE; }
  
//------------------------------
public:
//------------------------------

  void  clearBuffer() {
    KODE_Memset(MBuffer,0, SIZE * sizeof(TYPE));
  }
  
  void  fillBuffer(TYPE AValue) {}
};

//----------------------------------------------------------------------
// dynamic
//----------------------------------------------------------------------

template <class TYPE>
class KODE_DynamicBuffer {
  
//------------------------------
private:
//------------------------------

  TYPE* MBuffer         = KODE_NULL;
  uint32_t  MBufferSize = 0;
  
//------------------------------
public:
//------------------------------

  TYPE      getBuffer()     { return MBuffer; }
  uint32_t  getBufferSize() { return MBufferSize; }
  
//------------------------------
public:
//------------------------------

  void  clearBuffer()
  {
    KODE_Memset(MBuffer,0, MBufferSize * sizeof(TYPE));
  }
  
  void  fillBuffer(TYPE AValue) {
  }
  
  void  resizeBuffer(uint32_t ASize) {
  }
  
};

//----------------------------------------------------------------------
#endif
