#ifndef s2_fft_included
#define s2_fft_included
//----------------------------------------------------------------------

#if 0

/*
  todo: linear -> logarithmix buffer/map
  map linear index -> logarithmic
  precalc when setting hz_min/max/scale
*/

#include "extern/pffft/pffft.h"
#include "extern/pffft/pffft.c"

#include "s2_math_fast.h"

//----------------------------------------------------------------------

class S2_FFT {

  private:
    PFFFT_Setup*  MFft;
    int32         MFftSize;
    float         MFftSizeInv;
    float*        MFftBuffer;
    float         MSampleRate;

  public:

    S2_FFT() {
      MFft        = S2_NULL;
      MFftSize    = 0;
      MFftSizeInv = 0;
      MFftBuffer  = S2_NULL;
      MSampleRate = 0;
    }

    //----------

    S2_FFT(uint32 ASize) {
      create(ASize);
    }

    //----------

    virtual ~S2_FFT() {
      destroy();
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    float* buffer(void) { return MFftBuffer; }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    /*
      allocates memory, etc..
      (locking, not thread-safe)
    */

    void create(int32 ASize) {
      S2_Assert( SIsPowerOfTwo(ASize) );
      if (ASize>0) {
        MFftSize = ASize;
        MFftSizeInv = 1.0f / (float)ASize;
        MFft = pffft_new_setup(MFftSize,PFFFT_COMPLEX);
        uint32 num_bytes  = MFftSize * 2 * sizeof(float);
        MFftBuffer  = (float*)pffft_aligned_malloc(num_bytes);
        S2_Memset(MFftBuffer,0,num_bytes);
      }
    }

    //----------

    void destroy(void) {
      MFftSize = 0;
      MFftSizeInv = 1.0f;
      if (MFftBuffer) pffft_aligned_free(MFftBuffer);
      MFftBuffer = S2_NULL;
      if (MFft) pffft_destroy_setup(MFft);
      MFft = S2_NULL;
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void setSize(int32 ASize) {
      SAssert( S2_IsPowerOfTwo(ASize) );
      if (ASize != MFftSize) {
        if (MFft) destroy();
        create(ASize);
      }
    }

    //----------

    void process(float* ABuffer) {
      S2_Assert(MFft!=S2_NULL);
      if (MFft) {
        pffft_transform_ordered(MFft,ABuffer,MFftBuffer,S2_NULL,PFFFT_FORWARD);
      }
    }

    void scale( float AScale ) {
      for (int32 i=0; i<MFftSize; i++) {
        MFftBuffer[i*2] *= AScale;
      }
    }

    //float  getBinLinear(uint32 AIndex) {
    //  return 0;
    //}

    //float  getBinLogarithmic(uint32 AIndex) {
    //  return 0;
    //}

    //----------

    /*
    void squaredMagnitude(float* dst) {
      int32 size = MFftSize; // /2?
      for (int32 i=0; i<size; i++) {
        float re = MFftBuffer[i*2];
        float im = MFftBuffer[i*2+1];
        dst[i] = re*re + im*im;
      }
    }
    */

    //----------

    /*
    void db(float* dst, float* src) {
      int32 size = MFftSize; // /2?
      for (int32 i=0; i<size; i++) {
        float n = src[i];
        //dst[i] = 10 * log10(n);
        dst[i] = 10 * SLog10F(n);
      }
    }
    */

    //void setSampleRate(float ARate) {
    //  MSampleRate = ARate;
    //}

    //----------

    /*
      in:
        AIndex: 0..MFftSize (bin, linear)
        AMinHz/AMaxHz: hz
      result:
        0..MFftSize (bin, logarithmic)


      todo: ABin -> pos 0..1 ??

    */

    // https://github.com/automatl/dsp-utility/blob/master/Scaling.h

    //int32 getBinLog(int32 ABin, float AMinHz, float AMaxHz) {
    //  float octaveCount = log2(AMaxHz) - log2(AMinHz);
    //  SAssert( octaveCount>0 );
    //  float pixelsPerOctave = (float)MFftSize / octaveCount;
    //  SAssert( pixelsPerOctave>0 );
    //  float offset = round(log2(AMinHz / pixelsPerOctave) * pixelsPerOctave);
    //  float value = pixelsPerOctave * pow(2.0f, (((float)ABin + offset) / pixelsPerOctave));
    //  int32 idx = (floor)(value);
    //  SAssert( idx>=0 );
    //  SAssert( idx < MFftSize );
    //  return idx;
    //}

    //int32 getBinLin(int32 ABin, float AMinHz, float AMaxHz) {
    //    float scale_hz = AMaxHz - AMinHz;
    //    float b = too...
    //    //AMinHz *= MSampleRate;
    //    //AMaxHz *= MSampleRate;
    //}


    //----------

    /*
      AIndex = bin (0..fftsize)
      result = db
    */

    //float getValueDb(int32 AIndex) {
    //  float re = MFftBuffer[AIndex*2];
    //  float im = MFftBuffer[AIndex*2+1];
    //  float re_im = re*re + im*im;
    //  SAssert( re_im >= 0 );
    //  //  normalized bin magnitude
    //  float magnitude = 2.0f * /*sqrt*/SSqrtF(re_im) * MFftSizeInv;
    //  SAssert( magnitude >= 0 );
    //  // convert to dB
    //  float amplitude = 20.0f * /*log10*/SLog10F( magnitude );
    //  return amplitude;
    //  //float range_db = (float)(MMaxDb - MMinDb);
    //  //if (range_db > 0) float db_scale = (float)MRect.h / range_db;
    //  //amplitude *= db_scale;
    //}

    //----------

    //float getMagnitude(int32 ABin) {
    //  float re = MFftBuffer[ABin*2];
    //  float im = MFftBuffer[ABin*2+1];
    //  float re_im = re*re + im*im;
    //  SAssert( re_im >= 0 );
    //  //  normalized bin magnitude
    //  return 2.0f * /*sqrt*/SSqrtF(re_im) * MFftSizeInv;
    //}

    //float getMagnitude(float APos) {
    //  int32 bin = STrunc((float)MFftSize * APos);
    //  return getMagnitude(bin);
    //}


    //----------

    //float getAmplitude(float AMagnitude) {
    //  // convert to dB
    //  return 20.0f * /*log10*/SLog10F( AMagnitude );
    //}

    //----------

    //float getDbScale(float AMinDb=96.0f, float AMaxDb=0.0f) {
    //  float range = AMaxDb - AMinDb;
    //  if (range > 0) return 1.0f / range;
    //  else return 1.0f;
    //}

    //----------

    /*
      AHeight = pixels available
      return = ypos
    */

    //void getYPos(int32 AIndex, float AHeight, float AMinHz=0.f, float AMaxHz=1.0f) {
    //  //for(int32 i=0;i<MFftSize; i++) {
    //    int32 b = 0;
    //    //int32 b = getLogBin(AIndex,AMinHz,AMaxHz);
    //    switch (MScaleHz) {
    //      case 0: b = getBinLin(AIndex,AMinHz,AMaxHz);
    //      case 1: b = getBinLog(AIndex,AMinHz,AMaxHz);
    //    }
    //    float m = getMagnitude(b);
    //    float a = getAmplitude(m);
    //    float y = a * AHeight * getDbScale(-96,0);
    //  //}
    //}

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//void* S2_MallocAligned(size_t nb_bytes) {
//  return pffft_aligned_malloc(nb_bytes);
//}

//----------

//void  S2_MallocFree(void* ptr) {
//  pffft_aligned_free(ptr);
//}

#endif // 0

//----------------------------------------------------------------------
#endif
