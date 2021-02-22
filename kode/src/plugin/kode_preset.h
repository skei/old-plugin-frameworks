#ifndef kode_preset_included
#define kode_preset_included
//----------------------------------------------------------------------
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_array.h"
#include "base/kode_convert.h"

class KODE_Preset;
typedef KODE_Array<KODE_Preset*> KODE_Presets;

//----------------------------------------------------------------------


class KODE_Preset {

//------------------------------
protected:
//------------------------------

  const char*   MName       = "";
  const char*   MShortName  = "";
  int32_t       MIndex      = -1;
  uint32_t      MFlags      = KODE_PARAMETER_AUTOMATE;
  
//------------------------------
public:
//------------------------------

  KODE_Preset(const char* AName="") {
    MName = AName;
  }

  //----------

  virtual ~KODE_Preset() {
  }

//------------------------------
public:
//------------------------------

  virtual const char*   getName()                 { return MName; }
  virtual const char*   getShortName()            { return MShortName; }
  virtual int32_t       getIndex()                { return MIndex; }
  virtual void          setIndex(int32_t AIndex)  { MIndex = AIndex; }
  virtual bool          hasFlag(uint32_t AFlag)   { return (MFlags & AFlag); }
  virtual void          setFlag(uint32_t AFlag)   { MFlags |= AFlag; }
  virtual void          clearFlag(uint32_t AFlag) { MFlags &= ~AFlag; }

public:

  virtual float from01(float AValue) {
    return AValue;
  }

  virtual float to01(float AValue) {
    return AValue;
  }

  // AValue = 0..1

  virtual char* getDisplayText(float AValue, char* ABuffer) {
    //sprintf(ABuffer,"%.2f",from01(AValue));
    KODE_FloatToString(ABuffer,AValue);
    return ABuffer;
  }

};

//----------------------------------------------------------------------
#endif
