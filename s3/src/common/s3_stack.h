#ifndef s3_stack_included
#define s3_stack_included
//----------------------------------------------------------------------

/*
  - primitive
  - no error checking (yet)
*/

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

template<class _T,int SIZE>
class S3_Stack
{
  private:
    _T  MBuffer[SIZE];
    int MIndex;
  public:
    S3_Stack();
    ~S3_Stack();
    _T*   getTop(void);
    void  reset(void);
    void  push(_T AItem);
    _T    pop(void);
    void  dup(void);
    void  dup2(void);
    void  drop(void);
};


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------




template<class _T,int SIZE>
S3_Stack<_T,SIZE>::S3_Stack() {
  reset();
}

template<class _T,int SIZE>
S3_Stack<_T,SIZE>::~S3_Stack() {
}

template<class _T,int SIZE>
_T* S3_Stack<_T,SIZE>::getTop(void) {
  return &MBuffer[MIndex-1];
}

// a b -  ->  - - -

template<class _T,int SIZE>
void S3_Stack<_T,SIZE>::reset(void) {
  MIndex = 0;
}

// c  ->  a b -  ->  c a b -

template<class _T,int SIZE>
void S3_Stack<_T,SIZE>::push(_T AItem) {
  MBuffer[MIndex] = AItem;
  MIndex++;
}

// a b -  ->  b - -  -> [a]

template<class _T,int SIZE>
_T S3_Stack<_T,SIZE>::pop(void) {
  MIndex--;
  return MBuffer[MIndex];
}

// a b -  ->  a a b -

template<class _T,int SIZE>
void S3_Stack<_T,SIZE>::dup(void) {
  MBuffer[MIndex] = MBuffer[MIndex-1];
  MIndex++;
}

// a b -  ->  b a b -

template<class _T,int SIZE>
void S3_Stack<_T,SIZE>::dup2(void) {
  MBuffer[MIndex] = MBuffer[MIndex-2];
  MIndex++;
}

// a b -  ->  b - -  ->

template<class _T,int SIZE>
void S3_Stack<_T,SIZE>::drop(void) {
  MIndex--;
}

//----------------------------------------------------------------------
#endif
