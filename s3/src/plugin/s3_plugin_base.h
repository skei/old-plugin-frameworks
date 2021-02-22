#ifndef s3_plugin_base_included
#define s3_plugin_base_included
//----------------------------------------------------------------------

/*
  this class defines member variables and methods and (empty) virtual
  functions that sub-classes will call..
  a lot of variables are exposed (protected), since implementations
  read or write to them directly, and we don't want gazillions of
  (virtual) accessors..
*/

//----------------------------------------------------------------------

#include "common/s3_array.h"
#include "common/s3_queue.h"
#include "common/s3_rect.h"
#include "plugin/s3_parameter.h"
#include "plugin/s3_program.h"

//----------------------------------------------------------------------

typedef S3_Queue<uint32,S3_PLUGIN_MAX_QUEUE_SIZE> S3_PluginParamQueue;

//----------

struct S3_MidiEvent {
  uint32 offset;
  uint8  msg1;
  uint8  msg2;
  uint8  msg3;
  uint8  dummy;
};

//----------

class S3_Plugin_Base {
  protected:
    // info/descriptor
    const char*         MName;
    const char*         MAuthor;
    const char*         MDescription;
    uint32              MVersion;
    uint32              MUniqueId;
    uint32              MLatency;
    int32               MTail;
    uint32              MNumInputs;
    uint32              MNumOutputs;
    // state
    bool                MInitialized;
    bool                MIsOpen;
    bool                MIsProcessing;
    bool                MIsSuspended;
    // parameters
    S3_ParameterArray   MParameters;
    float               MHostParamValues[S3_PLUGIN_MAX_PARAMETERS];
    float               MProcessParamValues[S3_PLUGIN_MAX_PARAMETERS];
    float               MEditorParamValues[S3_PLUGIN_MAX_PARAMETERS];
    bool                MUpdateAllProcessValues;
    bool                MUpdateAllEditorValues;
    #ifdef S3_PLUGIN_PARAMS_PROCESS_QUEUE
    S3_PluginParamQueue MProcessParamQueue;
    #endif
    #ifdef S3_PLUGIN_PARAMS_EDITOR_QUEUE
    S3_PluginParamQueue MEditorParamQueue;
    #endif
    // midi
    S3_MidiEvent        MMidiEvents[S3_PLUGIN_MAX_MIDI_RECEIVE];
    // program
    S3_ProgramArray     MPrograms;
    uint32              MCurrentProgram;
    char                MProgramName[32];
    // editor
    bool                MEditorIsOpen;
    S3_Rect             MEditorRect;
    uint32              MEditorKnobMode;
    // time info
    float               MSampleRate;
    int32               MBlockSize;
    int32               MPlayState;
    float               MSamplePos;
    float               MTempo;
    float               MBeatPos;
    float               MLoopStart;
    float               MLoopEnd;
    // host info
    uint32              MHostCanDo;
    uint32              MHostId;
    uint32              MHostVersion;
    uint32              MHostVstVer;
  public:
    S3_Plugin_Base();
    virtual ~S3_Plugin_Base();
  public:
    // format-specific implementations..
    // implemented by formats: exe/vst
    virtual void* main(void* AMaster) { return S3_NULL; }
    virtual void  updateHostInfo(void) {}
    virtual void  updateHostTime(void) {}
    virtual void  sendMidiToHost(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {}
    virtual void  notifyHostParameterChanged(int32 AIndex, float AValue) {}
    virtual void  resizeHostWindow(int32 AWidth, int32 AHeight) {}
  protected:
    // cross-format
    // implemented in s3_plugin.h
    virtual void  updateParametersInProcess(void) {}
    virtual void  updateEditorInIdle(void) {}
    virtual void  setDefaultParamValues(void) {}
    virtual void  distributeParamValues(void) {}
  public:
    // override these in your plugin
    virtual void  on_create(void) {}
    virtual void  on_destroy(void) {}
    virtual void  on_stateChange(uint32 AState) {}
    virtual void  on_parameterChange(int32 AIndex, float AValue) {} // TODO: uint32 AMode ??
    virtual void  on_programChange(int32 AIndex) {}
    virtual void  on_midiEvents(S3_MidiEvent* AMidiEvents, uint32 ANumEvents) {}
    virtual void  on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {}
    virtual void  on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) {}
    virtual void  on_processSample(S3_Sample** AInputs, S3_Sample** AOutputs) {}
    virtual void  on_postProcess(void) {}
    virtual void* on_openEditor(void* AParent) { return S3_NULL; }
    virtual bool  on_closeEditor(void* AEditor) { return false; }
    virtual void  on_idleEditor(void* AEditor) {}
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Plugin_Base::S3_Plugin_Base() {
  // info/descriptor
  MName         = "test plugin";
  MAuthor       = "tor.helge.skei";
  MDescription  = "s3 example plugin";
  MVersion      = 0;
  MUniqueId     = S3_MAGIC + 0x0000;
  MLatency      = 0;
  MTail         = 0; // -1 = no tail
  MNumInputs    = 0; // 2;
  MNumOutputs   = 0; // 2;
  // state
  MInitialized  = false;
  MIsOpen       = false;
  MIsProcessing = false;
  MIsSuspended  = true;
  // parameters
  MParameters.clear();
  S3_Memset(MHostParamValues,0,sizeof(MHostParamValues));
  S3_Memset(MProcessParamValues,0,sizeof(MHostParamValues));
  S3_Memset(MEditorParamValues,0,sizeof(MHostParamValues));
  MUpdateAllProcessValues = false;
  MUpdateAllEditorValues = false;
  // midi
  S3_Memset(MMidiEvents,0,sizeof(MMidiEvents));
  // program
  MPrograms.clear();
  MCurrentProgram = 0;
  S3_Strcpy(MProgramName,"default");
  // editor
  MEditorIsOpen = false;
  MEditorRect   = S3_Rect(0,0,256,256);
  MEditorKnobMode = s3_km_linear;
  // time info
  MSampleRate     = 0;
  MBlockSize      = 0;
  MPlayState      = 0;
  MSamplePos      = 0;
  MTempo          = 0;
  MBeatPos        = 0;
  MLoopStart      = 0;
  MLoopEnd        = 0;
  // host info
  MHostCanDo      = s3_hc_none;
  MHostId         = s3_hi_unknown;
  MHostVersion    = 0;
  MHostVstVer     = 0;
}

//----------

S3_Plugin_Base::~S3_Plugin_Base() {
}

//----------------------------------------------------------------------
#endif
