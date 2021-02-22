#ifndef kode_descriptor_included
#define kode_descriptor_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_parameter.h"
#include "plugin/kode_preset.h"

class KODE_Descriptor {
  
//------------------------------
public:
//------------------------------

  uint32_t        MIndex        = 0; 
  const char*     MName         = "";
  const char*     MAuthor       = "";
  const char*     MEmail        = "";
  const char*     MUrl          = "";
  uint32_t        MFlags        = 0;
  uint32_t        MVersion      = 0;
  const char*     MVersionText  = "";
  const char*     MLicenseText  = "";
  uint32_t        MShortId      = 0;
  const uint8_t*  MLongId       = KODE_NULL_ID;
  const uint8_t*  MLongEditorId = KODE_NULL_ID;
  uint32_t        MNumInputs    = 0;
  uint32_t        MNumOutputs   = 0;
  KODE_Parameters MParameters;
  KODE_Presets    MPresets;
  
  uint32_t        MEditorDefaultWidth  = 0;
  uint32_t        MEditorDefaultHeight = 0;

//------------------------------
public:
//------------------------------

  KODE_Descriptor() {
  }
  
  virtual ~KODE_Descriptor() {
    #ifndef KODE_NO_AUTODELETE
      deleteParameters();
      deletePresets();
    #endif
  }
  
//------------------------------
public:
//------------------------------

  uint32_t        getIndex()                    { return MIndex; }
  const char*     getName()                     { return MName; }
  const char*     getAuthor()                   { return MAuthor; }
  const char*     getEmail()                    { return MEmail; }
  const char*     getUrl()                      { return MUrl; }
  uint32_t        getFlags()                    { return MFlags; }
  uint32_t        getVersion()                  { return MVersion; }
  const char*     getVersionText()              { return MVersionText; }
  const char*     getLicenseText()              { return MLicenseText; }
  uint32_t        getShortId()                  { return MShortId; }
  const uint8_t*  getLongId()                   { return MLongId; }
  const uint8_t*  getLongEditorId()             { return MLongEditorId; }
  uint32_t        getNumInputs()                { return MNumInputs; }
  uint32_t        getNumOutputs()               { return MNumOutputs; }
  
  uint32_t        getNumParameters()            { return MParameters.size(); }
  KODE_Parameter* getParameter(uint32_t AIndex) { return MParameters[AIndex]; }

  uint32_t        getNumPresets()               { return MPresets.size(); }
  KODE_Preset*    getPreset(uint32_t AIndex)    { return MPresets[AIndex]; }

  uint32_t        getEditorDefaultWidth()       { return MEditorDefaultWidth; }
  uint32_t        getEditorDefaultHeight()      { return MEditorDefaultHeight; }
  
  void            setFlags(uint32_t AFlag)      { MFlags = AFlag; }
  void            clearFlag(uint32_t AFlag)     { MFlags &= ~AFlag; }
  bool            hasFlag(uint32_t AFlag)       { return ((MFlags & AFlag) == AFlag); }
  
  void setFlag(uint32_t AFlag) {
    MFlags |= AFlag;
    if (AFlag & KODE_PLUGIN_IS_SYNTH) MFlags |= KODE_PLUGIN_RECEIVE_MIDI;
  }
  
  void setEditorSize(uint32_t AWidth, uint32_t AHeight) {
    MEditorDefaultWidth = AWidth;
    MEditorDefaultHeight = AHeight;
  }

//------------------------------
public:
//------------------------------

  virtual void appendParameter(KODE_Parameter* AParameter) {
    //AParameter->normalizeDefValue();
    uint32_t index = MParameters.size();
    AParameter->setIndex(index);
    MParameters.append(AParameter);
  }
  
  //----------
  
  virtual void deleteParameters() {
    uint32_t num = MParameters.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Delete MParameters[i];
      MParameters[i] = KODE_NULL;
    }
  }

  //----------
  
  virtual void appendPreset(KODE_Preset* APreset) {
    MPresets.append(APreset);
  }
  
  //----------
  
  virtual void deletePresets() {
    uint32_t num = MPresets.size();
    for (uint32_t i=0; i<num; i++) {
      KODE_Delete MPresets[i];
      MPresets[i] = KODE_NULL;
    }
  }

};

//----------------------------------------------------------------------
#endif
