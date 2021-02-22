#ifndef s3_parameter_included
#define s3_parameter_included
//----------------------------------------------------------------------

#include "common/s3_array.h"
#include "common/s3_convert.h"
#include "common/s3_math.h"

class S3_Parameter;
typedef S3_Array<S3_Parameter*> S3_ParameterArray;

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Parameter {
  protected:
    int32             MType;
    const char*       MName;
    const char*       MLabel;
    float             MDefault;
    uint32            MFlags;
    int32             MIndex;
    void*             MWidget;
  protected:
    char              MDisplay[256];
  public:
    S3_Parameter();
    S3_Parameter(const char* AName, float AValue=0);
    virtual ~S3_Parameter();
    bool hasFlag(uint32 AFlag)            { return (MFlags & AFlag); }
    bool hasFlags(uint32 AFlags)          { return ((MFlags & AFlags)==AFlags); }
    void setFlag(uint32 AFlag)            { MFlags |= AFlag; }
    void clearFlag(uint32 AFlag)          { MFlags &= ~AFlag; }
  public:
    virtual const char* name(void)                { return MName; }
    virtual const char* label(void)               { return MLabel; }
    virtual void        label(const char* ALabel) { MLabel = ALabel; }
    virtual const char* displayText(float AValue) { S3_FloatToString(MDisplay,from01(AValue)); return MDisplay; }
    virtual float       defaultValue(void)        { return MDefault; }
    virtual uint32      flags(void)               { return MFlags; }
    virtual int32       index(void)               { return MIndex; }
    virtual void        index(int32 AIndex)       { MIndex = AIndex; }
    virtual float       from01(float AValue)      { return AValue; }
    virtual float       to01(float AValue)        { return AValue; }
    virtual void*       widget(void)              { return MWidget; }
    virtual void        widget(void* AWidget)     { MWidget = AWidget; }
};

//----------

class S3_ParamFloat : public S3_Parameter {
  protected:
    float MMin;
    float MMax;
    float MStep;
    float MRange;
    float MInvRange;
    bool  MStepped;
    int32 MNumSteps;
    float MStepSize;
    float MHalfStep;
  private:
    void setup(const char* AName, const char* ALabel, float AVal, float AMin, float AMax, float AStep);
  public:
    S3_ParamFloat(const char* AName, float AVal, float AMin=0, float AMax=1, float AStep=0);
    virtual float to01(float AValue) override;
    virtual float from01(float AValue) override;
};

//----------

class S3_ParamInt : public S3_ParamFloat {
  public:
    S3_ParamInt(const char* AName, float AVal, float AMin=0, float AMax=1);
    virtual const char* displayText(float AValue) override;
};

//----------

class S3_ParamText : public S3_ParamInt {
  private:
    const char** MStrings;
  public:
    S3_ParamText(const char* AName, uint32 AIndex, uint32 ANum, const char** AText);
    virtual const char* displayText(float AValue) override;
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// parameter
//----------------------------------------------------------------------

S3_Parameter::S3_Parameter() {
  MType       = s3_pt_parameter;
  MName       = "param";
  MLabel      = "";
  MDisplay[0] = 0;
  MDefault    = 0;
  MFlags      = s3_pf_automate;
  MIndex      = -1;
  MWidget     = S3_NULL;
}

S3_Parameter::S3_Parameter(const char* AName, float AValue) {
  MType       = s3_pt_parameter;
  MName       = AName;
  MLabel      = "";
  MDisplay[0] = 0;
  MDefault    = AValue;
  MFlags      = s3_pf_automate;
  MIndex      = -1;
  MWidget     = S3_NULL;
}

S3_Parameter::~S3_Parameter() {
}

//----------------------------------------------------------------------
// float
//----------------------------------------------------------------------

S3_ParamFloat::S3_ParamFloat(const char* AName, float AVal, float AMin, float AMax, float AStep)
//: S3_Parameter(AName,AValue) {
: S3_Parameter() {
  MType = s3_pt_paramFloat;
  setup(AName,"",AVal,AMin,AMax,AStep);
}

//----------

void S3_ParamFloat::setup(const char* AName, const char* ALabel, float AVal, float AMin, float AMax, float AStep) {
  MIndex    = -1;
  MName     = AName;
  MLabel    = ALabel;
  MMin      = AMin;
  MMax      = AMax;
  MStep     = AStep;
  MRange    = MMax - MMin;
  MInvRange = 1 / MRange;
  if (MStep > 0) {
    MStepped  = true;
    MNumSteps = 1 + S3_Round( MRange / MStep );
    MStepSize = 1 / (MNumSteps - 1);
    MHalfStep = 0.5 * MStepSize;
  } else {
    MStepped  = false;
    MNumSteps = 1;
    MStepSize = 1;
    MHalfStep = 0;
  }
  MDefault = ((AVal-MMin)*MInvRange);
}

//----------

float S3_ParamFloat::to01(float AValue) {
  return ((AValue-MMin)*MInvRange);
}

//----------

float S3_ParamFloat::from01(float AValue) {
  if (MStepped) {
    float n = AValue * MNumSteps;
    int32 st = floorf(n);
    if (st > (MNumSteps-1)) st = (MNumSteps-1);
    return  MMin + (st * MStep);
  } else {
    return MMin + (AValue*MRange);
  }
}

//----------------------------------------------------------------------
// int
//----------------------------------------------------------------------

S3_ParamInt::S3_ParamInt(const char* AName, float AVal, float AMin, float AMax)
: S3_ParamFloat(AName,AVal,AMin,AMax,1) {
  MType = s3_pt_paramInt;
}

//----------

const char* S3_ParamInt::displayText(float AValue) {
  S3_IntToString(MDisplay,floorf(from01(AValue)));
  return MDisplay;
}

//----------------------------------------------------------------------
// text
//----------------------------------------------------------------------



S3_ParamText::S3_ParamText(const char* AName, uint32 AIndex, uint32 ANum, const char** AText)
: S3_ParamInt(AName,AIndex,0,ANum-1) {
  MType = s3_pt_paramText;
  MStrings = AText;
}

//----------

const char* S3_ParamText::displayText(float AValue) {
  int32 i = floorf(from01(AValue));
  S3_Strcpy(MDisplay, MStrings[i] );
  return MDisplay;
}

//----------------------------------------------------------------------
#endif
