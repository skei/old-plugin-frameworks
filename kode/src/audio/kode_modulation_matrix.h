#ifndef kode_modulation_matrix_included
#define kode_modulation_matrix_included
//----------------------------------------------------------------------

#define KODE_MODMATRIX_MAX_SRC_COUNT        64
#define KODE_MODMATRIX_MAX_DST_COUNT        64
#define KODE_MODMATRIX_MAX_MOD_COUNT        64

#define KODE_MODMATRIX_MODE_OFF             0
#define KODE_MODMATRIX_MODE_GLOBAL          1
#define KODE_MODMATRIX_MODE_VOICE           2

#define KODE_MODMATRIX_MODE_SAMPLE          3
#define KODE_MODMATRIX_MODE_EVENT           4
#define KODE_MODMATRIX_MODE_BLOCK           5
#define KODE_MODMATRIX_MODE_SMOOTH          6


//#define KODE_MODMATRIX_UPDATE_OFF           0
//#define KODE_MODMATRIX_UPDATE_SAMPLE        1
//#define KODE_MODMATRIX_UPDATE_EVENT         2
//#define KODE_MODMATRIX_UPDATE_BLOCK         3

//----------------------------------------------------------------------

class KODE_ModulationMatrix {
  
//------------------------------
private:
//------------------------------

  uint32_t    MSrcCount = 0;
  uint32_t    MDstCount = 0;
  uint32_t    MModCount = 0;

  const char* MSrcNames[KODE_MODMATRIX_MAX_SRC_COUNT]     = {0};
  float       MSrcValues[KODE_MODMATRIX_MAX_SRC_COUNT]    = {0};
  uint32_t    MSrcUpdate[KODE_MODMATRIX_MAX_SRC_COUNT]    = {0};
  
  const char* MDstNames[KODE_MODMATRIX_MAX_DST_COUNT]     = {0};
  float       MDstValues[KODE_MODMATRIX_MAX_DST_COUNT]    = {0};
  uint32_t    MDstUpdate[KODE_MODMATRIX_MAX_SRC_COUNT]    = {0};
  
  uint32_t    MModSrcIndex[KODE_MODMATRIX_MAX_MOD_COUNT]  = {0};
  uint32_t    MModDstIndex[KODE_MODMATRIX_MAX_MOD_COUNT]  = {0};
  float       MModAmount[KODE_MODMATRIX_MAX_MOD_COUNT]    = {0};
  float       MModScale[KODE_MODMATRIX_MAX_MOD_COUNT]     = {0};
  float       MModOffset[KODE_MODMATRIX_MAX_MOD_COUNT]    = {0};
  uint32_t    MModMode[KODE_MODMATRIX_MAX_MOD_COUNT]      = {0};

//------------------------------
public:
//------------------------------
  
  KODE_ModulationMatrix() {
  }
  
  //----------
  
  ~KODE_ModulationMatrix() {
  }
  
//------------------------------
public:
//------------------------------
  
  uint32_t addSrc(const char* AName, uint32_t AUpdate) {
    //KODE_Trace("src %i %s\n",MSrcCount,AName);
    MSrcNames[MSrcCount] = AName;
    MSrcUpdate[MSrcCount] = AUpdate;
    MSrcCount++;
    return MSrcCount;
  }

  //----------
  
  uint32_t addDst(const char* AName, uint32_t AUpdate) {
    //KODE_Trace("dst %i %s\n",MDstCount,AName);
    MDstNames[MDstCount] = AName;
    MDstUpdate[MDstCount] = AUpdate;
    MDstCount++;
    return MDstCount;
  }

  //----------
  
  uint32_t addMod(uint32_t AModSrcIndex, uint32_t AModDstIndex, float AModAmount, float AModScale, float AModOffset, uint32_t AModMode) {
    //KODE_Trace("mod %i src %i dst %i\n",MModCount,AModSrcIndex,AModDstIndex);
    MModSrcIndex[MModCount] = AModSrcIndex;
    MModDstIndex[MModCount] = AModDstIndex;
    MModAmount[MModCount]   = AModAmount;
    MModScale[MModCount]    = AModScale;
    MModOffset[MModCount]   = AModOffset;
    MModMode[MModCount]     = AModMode;
    MModCount++;
    return MModCount;
  }

  //----------
  
  void setSrcValue(uint32_t AIndex, float AValue)       { MSrcValues[AIndex] = AValue; }
  void setDstValue(uint32_t AIndex, float AValue)       { MDstValues[AIndex] = AValue; }
  void setModSrcIndex(uint32_t AIndex, uint32_t AValue) { MModSrcIndex[AIndex] = AValue;  }
  void setModDstIndex(uint32_t AIndex, uint32_t AValue) { MModDstIndex[AIndex] = AValue;  }
  void setModAmount(uint32_t AIndex, float AValue)      { MModAmount[AIndex] = AValue;  }
  void setModScale(uint32_t AIndex, float AValue)       { MModScale[AIndex] = AValue;  }
  void setModOffset(uint32_t AIndex, float AValue)      { MModOffset[AIndex] = AValue;  }
  void setModMode(uint32_t AIndex, uint32_t AValue)     { MModMode[AIndex] = AValue;  }

  uint32_t  getSrcCount()                   { return MSrcCount; }
  uint32_t  getDstCount()                   { return MDstCount; }
  uint32_t  getModCount()                   { return MModCount; }

  float     getSrcValue(uint32_t AIndex)    { return MSrcValues[AIndex]; }
  float     getDstValue(uint32_t AIndex)    { return MDstValues[AIndex]; }
  uint32_t  getModSrcIndex(uint32_t AIndex) { return MModSrcIndex[AIndex];  }
  uint32_t  getModDstIndex(uint32_t AIndex) { return MModDstIndex[AIndex];  }
  float     getModAmount(uint32_t AIndex)   { return MModAmount[AIndex];  }
  float     getModScale(uint32_t AIndex)    { return MModScale[AIndex];  }
  float     getModOffset(uint32_t AIndex)   { return MModOffset[AIndex];  }
  uint32_t  getModMode(uint32_t AIndex)     { return MModMode[AIndex];  }

  //----------
  
  void process(uint32_t AMode, uint32_t AUpdate) {
    for (uint32_t i=0; i<MDstCount; i++) {
      //if (MModMode[i] == AMode) MDstValues[i] = 0;
      MDstValues[i] = 0;
    }
    for (uint32_t i=0; i<MModCount; i++) {
      uint32_t src_index  = MModSrcIndex[i];
      uint32_t dst_index  = MModDstIndex[i];
      uint32_t mode       = MModMode[i];
      if (mode == AMode) {
        if (dst_index > 0) {
          if (src_index > 0) {
            float result = MSrcValues[src_index];
            result *= ( MModAmount[i] * MModScale[i] );
            result += MModOffset[i];
            MDstValues[dst_index] += result;
          }
        }
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
