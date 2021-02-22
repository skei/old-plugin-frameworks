#ifndef s3_queue_included
#define s3_queue_included
//----------------------------------------------------------------------

// thread-safe, lock/wait-free queue
// single producer, single consumer, probably..

/*
  MReadPos/MWritePos volatile?
*/

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

template<class _T, int SIZE>
class S3_Queue
{
  private:
    _T*     MBuffer;
    uint32  MBufferSize;
    uint32  MBufferMask;
    uint32  MReadPos;
    uint32  MWritePos;
  public:
    S3_Queue();
    ~S3_Queue();
    void  clear(void);
    bool  read(_T* AItem);
    bool  write(_T AItem);
};

//----------

template<class _T, int SIZE>
class S3_Queue_Buffer
{
  private:
    _T            MData[SIZE];
    volatile int  MWritePos;  // volatile?
    volatile int  MReadPos;
  public:
    S3_Queue_Buffer();
    void  write(_T a_Data);
    int   read(_T* a_Buffer);
};


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

/*
  WARNING:
  - loses (new) events if buffer is full (when writing)
*/

//----------

template<class _T, int SIZE>
S3_Queue<_T,SIZE>::S3_Queue() {
  MBuffer     = (_T*)S3_Malloc(SIZE*sizeof(_T));
  MBufferSize = SIZE;
  MBufferMask = SIZE - 1;
  MReadPos    = 0;
  MWritePos   = 0;
}

template<class _T, int SIZE>
S3_Queue<_T,SIZE>::~S3_Queue() {
  S3_Free(MBuffer);
}

template<class _T, int SIZE>
void S3_Queue<_T,SIZE>::clear(void) {
  MReadPos  = 0;
  MWritePos = 0;
}

/*
  notes to self:
  TODO: remove when we are 110% sure it works

  (wp-rp) & FBufferMask = 'items in buffer'
  in write(), we avoid filling the entire buffer, by letting there always be
  at least one free item, because read() could misinterpret a full buffer
  as empty..
  if the write fail (returns false), what should we do?
  - ignore, losing events..
  - sleep a little and try again? (spin?)
  - the reading happens at the start of process, so in worst case,
    if buffer is full already, we must wait until the next processBlock,
    (reaper's pause doesn't call process, and doesn't let us know it's paused, i think..)
  - buffer/cache updates in case the original buffer is full? double-buffer..
  buffer is emptied at the start of process()
*/

template<class _T, int SIZE>
bool S3_Queue<_T,SIZE>::read(_T* AItem) {
  uint32 wp = MWritePos;
  uint32 rp = MReadPos;
  if (( (wp-rp) & MBufferMask ) > 0) {
    *AItem = MBuffer[rp];
    uint32 next = (rp+1) & MBufferMask;
    MReadPos = next;
    return true;
  }
  return false;
}

/*
  returns true if item written to queue,
  false if queue is full
*/

template<class _T, int SIZE>
bool S3_Queue<_T,SIZE>::write(_T AItem) {
  uint32 rp = MReadPos;
  uint32 wp = MWritePos;
  // -1 to avoid filling the entire buffer..
  // .. which could be mis-interpreted by read() as empty..
  if (((wp-rp) & MBufferMask) < (MBufferSize-1)) {
    //FBuffer[wp].FIndex := AItem.FIndex;
    //FBuffer[wp].FValue := AItem.FValue;
    MBuffer[wp] = AItem;
    uint32 next = (wp+1) & MBufferMask;
    MWritePos = next;
    return true;
  }
  return false;
}

/*
  check if there is an event with same index already in the queue
  todo: a) update value (replace event)
        b) remove old event, insert new at end of queue
*/

// this is not thread-safe !!

//template<class _T, int SIZE>
//bool S3_Queue::hasEventWithIndex(int32 AIndex) {
//  //result := False;
//  uint32 = MReadPos;
//  uint32 = MWritePos;
//  while (rpos != wpos) {
//    if FBuffer[rpos].FIndex =;
//  }
//}

//----------------------------------------------------------------------

/*
  safe for single-reader, single-writer ??
  (thread-safe?) circular/ring-buffer
  - write one item
  - read multiple items
*/

//----------

template<class _T, int SIZE>
S3_Queue_Buffer<_T,SIZE>::S3_Queue_Buffer() {
  SMemset(MData,0,SIZE*(sizeof(_T)));
  MWritePos = 0;
  MReadPos = 0;
}

template<class _T, int SIZE>
void S3_Queue_Buffer<_T,SIZE>::write(_T a_Data) {
  MData[MWritePos] = a_Data;
  MWritePos = (MWritePos + 1) % SIZE; // & SIZE if ^2
}

// return num items copied into buffer

template<class _T, int SIZE>
int S3_Queue_Buffer<_T,SIZE>::read(_T* a_Buffer) {
  int count = 0;
  int writepos = MWritePos;
  if (MReadPos > writepos) {
    S3_Memcpy((char*)&a_Buffer[count],
              (char*)&MData[MReadPos],
              (SIZE - MReadPos) * sizeof(_T));
    count = SIZE - MReadPos;
    MReadPos = 0;
  }
  S3_Memcpy((char*)&a_Buffer[count],
            (char*)&MData[MReadPos],
            (writepos - MReadPos) * sizeof(_T));
  count += writepos - MReadPos;
  MReadPos = writepos;
  return count;
}

//TODO: return one:
//template<class _T, int SIZE>
//_T* S3_Queue_Buffer<_T,SIZE>::read(void)
//  {
//  }

//----------------------------------------------------------------------
#endif
