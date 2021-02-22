#ifndef s3_array_included
#define s3_array_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

template<class _T>
class S3_Array {
	private:
    _T*    MBuffer;
    uint32 MTsize;
    uint32 MSize;
    uint32 MRealSize;
  public:
    S3_Array();
    S3_Array(const S3_Array& AArray);
    ~S3_Array();
    void      deleteItems(void);
    _T&       operator [] (const uint32 AIndex);
    _T&       item(const uint32 AIndex);
    uint32    size();
    void*     ptr();
    S3_Array& operator = (const S3_Array &AArray);
    void      clear(bool AErase=true);
    void      append(const _T& AItem);
    void      setSize(const uint32 ASize);
    void      remove(const uint32 APos);
    void      removeFirst(void);
    void      removeLast(void);
    int32     find(const _T& AItem);
    void      push(_T AItem);
    _T        pop(void);
};

//----------------------------------------------------------------------
// standard/predefined array types
//----------------------------------------------------------------------

typedef S3_Array<char>        S3_CharArray;
typedef S3_Array<int32>       S3_Int32Array;
typedef S3_Array<uint32>      S3_UInt32Array;
typedef S3_Array<float>       S3_FloatArray;
typedef S3_Array<bool>        S3_BoolArray;
typedef S3_Array<void*>       S3_PtrArray;
typedef S3_Array<void**>      S3_PtrPtrArray;
typedef S3_Array<char*>       S3_CharPtrArray;
typedef S3_Array<float*>      S3_FloatPtrArray;
typedef S3_Array<S3_Sample>   S3_SampleArray;
typedef S3_Array<S3_Sample*>  S3_SamplePtrArray;

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

template<class _T>
S3_Array<_T>::S3_Array()  {
  MTsize = sizeof(_T);
  MRealSize = S3_ARRAY_SIZE_INIT;
  MSize = 0;
  MBuffer = (_T*)S3_Malloc(MRealSize*MTsize);
}

template<class _T>
S3_Array<_T>::S3_Array(const S3_Array& AArray) {
  MTsize = sizeof(_T);
  MBuffer = (_T*)SMalloc(MTsize*AArray.MRealSize);
  SMemcpy(MBuffer, AArray.MBuffer, MTsize*AArray.MRealSize);
  MRealSize = AArray.MRealSize;
  MSize = AArray.MSize;
}

template<class _T>
S3_Array<_T>::~S3_Array() {
  if (MBuffer) {
    S3_Free(MBuffer);
    MBuffer = S3_NULL;
  }
}

template<class _T>
void S3_Array<_T>::deleteItems(void) {
  for (uint32 i=0; i<MSize; i++) delete MBuffer[i];
}

template<class _T>
_T& S3_Array<_T>::operator [] (const uint32 AIndex) {
  return MBuffer[AIndex];
}

template<class _T>
_T& S3_Array<_T>::item(const uint32 AIndex) {
  return MBuffer[AIndex];
}

template<class _T>
uint32 S3_Array<_T>::size() {
  return MSize;
}

template<class _T>
void* S3_Array<_T>::ptr() {
  return MBuffer;
}

template<class _T>
S3_Array<_T>& S3_Array<_T>::operator = (const S3_Array<_T> &AArray) {
  if (this==&AArray) return *this;
  if (AArray.MSize==0) clear();
  setSize(AArray.MSize);
  // CppCheck: using 'memcpy' on class that contains a virtual method
  S3_Memcpy(MBuffer, AArray.MBuffer, MTsize*AArray.MSize);
  return *this;
}

template<class _T>
void S3_Array<_T>::clear(bool AErase) {
  MSize = 0;
  if (AErase) {
    // CppCheck: MBuffer nulled but not freed upon failure
    MBuffer = (_T*)S3_Realloc((char*)MBuffer, MTsize*S3_ARRAY_SIZE_INIT);
    MRealSize = S3_ARRAY_SIZE_INIT;
  }
}

template<class _T>
void S3_Array<_T>::append(const _T& AItem) {
  MSize++;
  if (MSize > MRealSize) {
    MRealSize *= S3_ARRAY_SIZE_MULT;
    // CppCheck: MBuffer nulled but not freed upon failure
    MBuffer = (_T*)S3_Realloc(MBuffer, MTsize*MRealSize);
  }
  MBuffer[MSize-1] = AItem;
}

template<class _T>
void S3_Array<_T>::setSize(const uint32 ASize) {
  if (ASize != 0) {
    if ( (ASize>MRealSize) || (ASize<MRealSize/S3_ARRAY_SIZE_MULT) ) {
      MRealSize = ASize;
      // CppCheck: MBuffer nulled but not freed upon failure
      MBuffer = (_T*)S3_Realloc(MBuffer, MTsize*MSize);
      MSize = ASize;
    }
  }
  else clear();
}

template<class _T>
void S3_Array<_T>::remove(const uint32 APos) {
  if (MSize==1) clear();
  else {
    for(uint32 i=APos; i<MSize-1; i++ ) MBuffer[i] = MBuffer[i+1];
    MSize--;
  }
}

template<class _T>
void S3_Array<_T>::removeFirst(void) {
  if (MSize==1) clear();
  // CppCheck: using 'memmove' on class that contains a virtual method
  S3_Memmove( MBuffer, MBuffer+1, ((MSize - 1) * MTsize));
  MSize--;
}

template<class _T>
void S3_Array<_T>::removeLast(void) {
  MSize--;
}

//TODO: insert

template<class _T>
int32 S3_Array<_T>::find(const _T& AItem) {
  for (uint32 i=0; i<MSize; i++) {
    if (MBuffer[i] == AItem) return i;
  }
  return -1;
}

template<class _T>
void S3_Array<_T>::push(_T AItem) {
  append(AItem);
}

template<class _T>
_T S3_Array<_T>::pop(void) {
  if (MSize>0) {
    MSize--;
    return MBuffer[MSize];
  }
  else return S3_NULL;
}

//----------------------------------------------------------------------
#endif
