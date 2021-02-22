#ifndef s2_audio_buffer_included
#define s2_audio_buffer_included
//----------------------------------------------------------------------

// length = number of samples
// size   = njumber of bytes

//----------

class S2_AudioBuffer {

  private:
    S2_Sample*  MBuffer;
    uint32      MLength;
    bool        MAllocated;
    uint32      MAllocatedLength;
  private:
    uint32      MReadPos;
    uint32      MWritePos;

  public:

    S2_AudioBuffer() {
      MBuffer           = S2_NULL;
      MLength           = 0;
      MAllocated        = false;
      MAllocatedLength  = 0;
      MReadPos          = 0;
      MWritePos         = 0;
    }

    //----------

    S2_AudioBuffer(uint32 ASize) {
      allocate(ASize);
    }

    //----------

    S2_AudioBuffer(S2_Sample* ABuffer, uint32 ALength) {
      buffer(ABuffer,ALength);
    }

    //----------

    ~S2_AudioBuffer() {
      if (MAllocated) S2_Free(MBuffer);
    }

  //------------------------------
  //
  //------------------------------

  public:


  //------------------------------
  //
  //------------------------------

  public:

    void buffer(S2_Sample* ABuffer, uint32 ALength) {
      if (MAllocated) deallocate();
      MLength = ALength;
      MBuffer = ABuffer;
    }

    //----------

    S2_Sample* buffer(void)  {
      return MBuffer;
    }

    //----------

    void length(uint32 ALength) {
      MLength = ALength;
      MReadPos  = MReadPos % MLength;
      MWritePos = MWritePos % MLength;
    }

    //----------

    uint32 length(void) {
      return MLength;
    }

    //----------

    bool allocated(void) {
      return MAllocated;
    }

    //----------

    void allocate(uint32 ALength) {
      if (MAllocated) deallocate();
      MBuffer = (S2_Sample*)S2_Malloc(MLength*S2_SampleSize);
      MLength = ALength * S2_SampleSize;
      MAllocated = true;
      MAllocatedLength = MLength;
    }

    //----------

    void deallocate(void) {
      if (MAllocated) {
        S2_Free(MBuffer);
        MBuffer = S2_NULL;
        MLength = 0;
        MAllocated = false;
        MAllocatedLength = 0;
      }
    }

    //----------

    void clear(void) {
      S2_Memset(MBuffer,0,MLength*S2_SampleSize);
    }

    //----------

    void reset(void) {
      MReadPos = 0;
      MWritePos = 0;
    }

  //------------------------------
  //
  //------------------------------

  public:

    S2_Sample read(void) {
      float res = MBuffer[MReadPos];
      MReadPos++;
      if (MReadPos>=MLength) MReadPos -= MLength;
      return res;
    }

    //----------

    void write(S2_Sample ASample) {
      MBuffer[MWritePos] = ASample;
      MWritePos++;
      if (MWritePos>=MLength) MWritePos -= MLength;
    }

    //----------

};

//----------------------------------------------------------------------
#endif
