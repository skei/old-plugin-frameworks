#ifndef s2_modulation_matrix_included
#define s2_modulation_matrix_included
//----------------------------------------------------------------------

// matrix source mode
// how often the source/destination is/should be updated

// dst
#define s2_md_off     0
#define s2_md_voice   1
#define s2_md_global  2

#define s2_ms_off     0
#define s2_ms_const   1
#define s2_ms_block   2
#define s2_ms_tick    3
#define s2_ms_lerp    4
#define s2_ms_sample  5

//----------

#define S2_MAX_MOD_SRC   64
#define S2_MAX_MOD_DST   64
#define S2_MAX_MOD_COUNT 64

#define S2_MAX_MOD_SLOTS 64

//----------------------------------------------------------------------

class S2_Modulation_Matrix {

  private:

    uint32      MSrcCount;
    const char* MSrcName[S2_MAX_MOD_SRC];
    float*      MSrcPtr[S2_MAX_MOD_SRC];
    uint32      MSrcMode[S2_MAX_MOD_SRC];

    uint32      MDstCount;
    const char* MDstName[S2_MAX_MOD_DST];
    float*      MDstPtr[S2_MAX_MOD_DST];
    uint32      MDstMode[S2_MAX_MOD_DST];

    uint32      MModCount;
    int32       MModSrc[S2_MAX_MOD_COUNT];
    int32       MModDst[S2_MAX_MOD_COUNT];
    float       MModAmount[S2_MAX_MOD_COUNT];
    float       MModScale[S2_MAX_MOD_COUNT];
    float       MModOffset[S2_MAX_MOD_COUNT];

    float       MModValue[S2_MAX_MOD_COUNT];


  //------------------------------
  //
  //------------------------------

  public:

    S2_Modulation_Matrix() {
      MSrcCount = 0;
      S2_ClearMem(MSrcName);
      S2_ClearMem(MSrcPtr);
      S2_ClearMem(MSrcMode);
      MDstCount = 0;
      S2_ClearMem(MDstName);
      S2_ClearMem(MDstPtr);
      S2_ClearMem(MDstMode);
      MModCount = 0;
      S2_ClearMem(MModSrc);
      S2_ClearMem(MModDst);
      S2_ClearMem(MModAmount);
      for (uint32 i=0; i<S2_MAX_MOD_COUNT; i++) MModScale[i] = 1.0f;
      S2_ClearMem(MModOffset);
    }

    //----------

    ~S2_Modulation_Matrix() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    //----------

    int32         srcCount(void)                        { return MSrcCount; }
    const char**  srcNames(void)                        { return MSrcName; }
    const char*   srcName(int32 AIndex)                 { return MSrcName[AIndex]; }
    float*        srcPtr(int32 AIndex)                  { return MSrcPtr[AIndex]; }
    void          srcValue(int32 AIndex, float AValue)  { *MSrcPtr[AIndex] = AValue; }
    float         srcValue(int32 AIndex)                { return *MSrcPtr[AIndex]; }

    int32         dstCount(void)                        { return MDstCount; }
    const char**  dstNames(void)                        { return MDstName; }
    const char*   dstName(int32 AIndex)                 { return MDstName[AIndex]; }
    float*        dstPtr(int32 AIndex)                  { return MDstPtr[AIndex]; }
    void          dstValue(int32 AIndex, float AValue)  { *MDstPtr[AIndex] = AValue; }
    float         dstValue(int32 AIndex)                { return *MDstPtr[AIndex]; }

    int32         modCount(void)                        { return MModCount; }
    int32         modSrc(int32 AIndex)                  { return MModSrc[AIndex]; }
    void          modSrc(int32 AIndex, int32 AValue)    { MModSrc[AIndex] = AValue; }
    const char*   modSrcName(int32 AIndex)              { return MSrcName[MModSrc[AIndex]]; }
    float*        modSrcPtr(int32 AIndex)               { return MSrcPtr[MModSrc[AIndex]]; }
    int32         modDst(int32 AIndex)                  { return MModDst[AIndex]; }
    void          modDst(int32 AIndex, int32 AValue)    { MModDst[AIndex] = AValue; }
    const char*   modDstName(int32 AIndex)              { return MDstName[MModDst[AIndex]]; }
    float*        modDstPtr(int32 AIndex)               { return MDstPtr[MModDst[AIndex]]; }
    float         modAmount(int32 AIndex)               { return MModAmount[AIndex]; }
    void          modAmount(int32 AIndex, float AValue) { MModAmount[AIndex] = AValue; }
    float         modScale(int32 AIndex)                { return MModScale[AIndex]; }
    void          modScale(int32 AIndex, float AScale)  { MModScale[AIndex] = AScale; }
    float         modOffset(int32 AIndex)               { return MModOffset[AIndex]; }
    void          modOffset(int32 AIndex, float AOfs)   { MModOffset[AIndex] = AOfs; }

    uint32        srcMode(int32 AIndex)                 { return MSrcMode[AIndex]; }
    void          srcMode(int32 AIndex, uint32 AMode)   { MSrcMode[AIndex] = AMode; }
    uint32        dstMode(int32 AIndex)                 { return MDstMode[AIndex]; }
    void          dstMode(int32 AIndex, uint32 AMode)   { MDstMode[AIndex] = AMode; }

  //------------------------------
  //
  //------------------------------

  public:

    int32 addSource(const char* AName, float* AValuePtr, uint32 AMode) {
      if (MSrcCount<S2_MAX_MOD_SRC) {
        MSrcName[MSrcCount] = AName;
        MSrcPtr[MSrcCount] = AValuePtr;
        MSrcMode[MSrcCount] = AMode;
        MSrcCount++;
        return (MSrcCount-1);
      }
      return -1;
    }

    //----------

    int32 addDestination(const char* AName, float* AValuePtr, uint32 AMode) {
      if (MDstCount<S2_MAX_MOD_DST) {
        MDstName[MDstCount] = AName;
        MDstPtr[MDstCount] = AValuePtr;
        MDstMode[MDstCount] = AMode;
        MDstCount++;
        return (MDstCount-1);
      }
      return -1;
    }

    //----------

    int32 addModulation(uint32 ASrc=0, uint32 ADst=0, float AAmount=0, float AScale=1, float AOffset=0) {
      if (MModCount<S2_MAX_MOD_SRC) {
        MModSrc[MModCount] = ASrc;
        MModDst[MModCount] = ADst;
        MModAmount[MModCount] = AAmount;
        MModScale[MModCount] = AScale;
        MModOffset[MModCount] = AOffset;
        MModCount++;
        return (MModCount-1);
      }
      return -1;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void process(uint32 AMode) {
      for (uint32 i=0; i<MDstCount; i++) if (MDstMode[i]==AMode) *MDstPtr[i] = 0;
      for (uint32 i=0; i<MModCount; i++) {
        uint32 src = MModSrc[i];
        uint32 dst = MModDst[i];
        if (MDstMode[dst]==AMode) {
          float result = *MSrcPtr[src];
          //float amt = MModAmount[i];
          //amt *= MModScale[i];
          //result *= amt;
          //float ofs = MModOffset[i];
          //result += ofs;
          result *= ( MModAmount[i] * MModScale[i] );
          result += MModOffset[i];
          *MDstPtr[dst] += result;
        }
      }
    }

    //----------

};

//----------------------------------------------------------------------

/*

struct S2_Mod2_Src {
  const char* name;
  float       value;
  uint32      mode;
};

struct S2_Mod2_Dst {
  const char* name;
  float       value;
  uint32      mode;
};

struct S2_Mod2_Slot {
  uint32  src;
  uint32  dst;
  float   amount;
  float   scale;
  float   offset;
};

//----------

class S2_Mod2_Matrix {

  private:

    uint32        src_count;
    uint32        dst_count;
    uint32        slot_count;
    S2_Mod2_Src   src[S2_MAX_MOD_SRC];
    S2_Mod2_Dst   dst[S2_MAX_MOD_DST];
    S2_Mod2_Slot  slot[S2_MAX_MOD_SLOTS];

  //------------------------------
  //
  //------------------------------

  public:

    S2_Mod2_Matrix() {
      src_count = 0;
      dst_count = 0;
      slot_count = 0;
      S2_Memset(src,0,sizeof(src));
      S2_Memset(dst,0,sizeof(dst));
      S2_Memset(slot,0,sizeof(slot));
    }

    ~S2_Mod2_Matrix() {
    }

  //------------------------------
  // prepare
  //------------------------------

  public:

    uint32 appendSrc(const char* AName, float AValue, uint32 AMode) {
      uint32 result = src_count;
      src[src_count].name = AName;
      src[src_count].value = AValue;
      src[src_count].mode = AMode;
      src_count++;
      return result;
    }

    uint32 appendSrc(S2_Mod2_Src* ASrc) {
      return appendSrc(ASrc->name,ASrc->value,ASrc->mode);
    }

    uint32 appendDst(const char* AName, float AValue, uint32 AMode) {
      uint32 result = dst_count;
      dst[dst_count].name = AName;
      dst[dst_count].value = AValue;
      dst[dst_count].mode = AMode;
      dst_count++;
      return result;
    }

    uint32 appendDst(S2_Mod2_Dst* ADst) {
      return appendDst(ADst->name,ADst->value,ADst->mode);
    }

    uint32 appendSlot(uint32 ASrc, uint32 ADst, float AAmount, float AScale, float AOffset) {
      uint32 result = slot_count;
      slot[slot_count].src = ASrc;
      slot[slot_count].dst = ADst;
      slot[slot_count].amount = AAmount;
      slot[slot_count].scale = AScale;
      slot[slot_count].offset = AOffset;
      slot_count++;
      return result;
    }

    uint32 appendSlot(S2_Mod2_Slot* ASlot) {
      return appendSlot(ASlot->src, ASlot->dst, ASlot->amount, ASlot->scale, ASlot->offset);
    }

  //------------------------------
  // runtime
  //------------------------------

  public:

    void setSrcValue(uint32 AIndex, float AValue)  {
      src[AIndex].value = AValue;
    }

    void process(uint32 AMode) {
      for (uint32 i=0; i<dst_count; i++) if (dst[i].mode==AMode) dst[i].value = 0;
      for (uint32 i=0; i<slot_count; i++) {
        uint32 s = slot[i].src;
        uint32 d = slot[i].dst;
        if ( dst[d].mode==AMode) {
          float result = src[s].value;
          float amt = slot[i].amount;
          amt *= slot[i].scale;
          float ofs = slot[i].offset;
          result *= amt;
          result += ofs;
          dst[d].value += result;
        }
      }
    }

    float getDstValue(uint32 AIndex) {
      return dst[AIndex].value;
    }

};

*/

//----------------------------------------------------------------------
#endif
