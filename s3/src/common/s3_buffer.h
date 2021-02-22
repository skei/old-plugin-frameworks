#ifndef s3_buffer_included
#define s3_buffer_included
//----------------------------------------------------------------------

/*
  allocated on the heap
*/

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

template<class _T>
class S3_Buffer {
  private:
    _T*     MData;
    uint32  MSize;
    uint32  MReadPos;
    uint32  MWritePos;
  public:
    S3_Buffer();
    S3_Buffer(uint32 ASize);
    ~S3_Buffer();
    void        setSize(uint32 ASize);
    void        resize(uint32 ASize);
    _T&         operator [] (const uint32 AIndex);
    _T&         item(const uint32 AIndex);
    uint32      size(void);
    uint32      itemSize(void);
    void*       ptr(void);
    S3_Buffer&  operator = (const S3_Buffer &ABuffer);
    void        clear(void);
    void        fill(uint8 AValue);
    void        copy(_T* APointer, uint32 ASize);
    void        reset(void);
    void        readPos(uint32 APos);
    void        writePos(uint32 APos);
    uint32      readPos(void);
    uint32      writePos(void);
    _T          read(void);
    _T          write(_T AItem);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

template<class _T>
S3_Buffer<_T>::S3_Buffer() {
  MSize = 0;
  MData = S3_NULL;
}

template<class _T>
S3_Buffer<_T>::S3_Buffer(uint32 ASize) {
  setSize(ASize);
}

template<class _T>
S3_Buffer<_T>::~S3_Buffer() {
  if (MData) S3_Free(MData);
}

template<class _T>
void S3_Buffer<_T>::setSize(uint32 ASize) {
  if (MData) MFree(MData);
  MSize = ASize;
  MData = (_T*)S3_Malloc( MSize * sizeof(_T) );
}

template<class _T>
void S3_Buffer<_T>::resize(uint32 ASize) {
  MSize = ASize;
  MData = (_T*)S3_Realloc( MData, MSize * sizeof(_T) );
}

template<class _T>
_T& S3_Buffer<_T>::operator [] (const uint32 AIndex) {
  return MData[AIndex];
}

template<class _T>
_T& S3_Buffer<_T>::item(const uint32 AIndex) {
  return MData[AIndex];
}

template<class _T>
uint32 S3_Buffer<_T>::size(void) {
  return MSize;
}

template<class _T>
uint32 S3_Buffer<_T>::itemSize(void) {
  return sizeof(_T);
}

template<class _T>
void* S3_Buffer<_T>::ptr(void) {
  return MData;
}

template<class _T>
S3_Buffer<_T>& S3_Buffer<_T>::operator = (const S3_Buffer<_T> &ABuffer) {
  if (this==&ABuffer) return *this;
  //if (ABuffer.MSize==0) clear();
  setSize(ABuffer.MSize);
  SMemcpy(MData, ABuffer.MData, sizeof(_T)*ABuffer.MSize);
  return *this;
}

template<class _T>
void S3_Buffer<_T>::clear(void) {
  SMemset(MData,0,MSize*sizeof(_T));
}

template<class _T>
void S3_Buffer<_T>::fill(uint8 AValue) {
  SMemset(MData,AValue,MSize*sizeof(_T));
}

template<class _T>
void S3_Buffer<_T>::copy(_T* APointer, uint32 ASize) {
  if (ASize != MSize) resize(ASize);
  SMemcpy(MData,MSize*sizeof(_T));
}

template<class _T>
void S3_Buffer<_T>::reset(void) {
  MReadPos=0;
  MWritePos=0;
}

template<class _T>
void S3_Buffer<_T>::readPos(uint32 APos) {
  MReadPos=APos;
}

template<class _T>
void S3_Buffer<_T>::writePos(uint32 APos) {
  MWritePos=APos;
}

template<class _T>
uint32 S3_Buffer<_T>::readPos(void) {
  return MReadPos;
}

template<class _T>
uint32 S3_Buffer<_T>::writePos(void) {
  return MWritePos;
}

template<class _T>
_T S3_Buffer<_T>::read(void) {
  _T result = MData[MReadPos];
  MReadPos++;
  if (MReadPos>=MSize) MReadPos = 0;
  return result;
}

template<class _T>
_T S3_Buffer<_T>::write(_T AItem) {
  _T result = MData[MWritePos];
  MData[MWritePos] = AItem;
  MWritePos++;
  if (MWritePos>=MSize) MWritePos = 0;
  return result;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef S3_Buffer<uint8>  S3_ByteBuffer;
typedef S3_Buffer<char>   S3_CharBuffer;
typedef S3_Buffer<char*>  S3_StringBuffer; // conflicts with SString class...
typedef S3_Buffer<int32>  S3_IntBuffer;
typedef S3_Buffer<uint32> S3_UIntBuffer;
typedef S3_Buffer<void*>  S3_PtrBuffer;
typedef S3_Buffer<float>  S3_FloatBuffer;

//----------------------------------------------------------------------
#endif

