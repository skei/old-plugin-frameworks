#ifndef kode_instance_included
#define kode_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
//#include "base/kode_event.h"
#include "base/kode_queue.h"
#include "audio/kode_process_context.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_editor_listener.h"
#include "plugin/kode_instance_listener.h"

#ifndef KODE_NO_GUI
  //#include "plugin/kode_default_editor.h"
  #include "plugin/kode_editor.h"
  #include "gui/kode_widgets.h"
#endif // KODE_NO_GUI

//----------------------------------------------------------------------

#define KODE_INSTANCE_MAX_PARAMETER_CHANGES 1024

//----------------------------------------------------------------------

class KODE_Instance
#ifdef KODE_NO_GUI
{
#else // KODE_NO_GUI
: public KODE_EditorListener {
#endif // KODE_NO_GUI
  
  typedef KODE_Queue<uint32_t,KODE_INSTANCE_MAX_PARAMETER_CHANGES> KODE_ParameterQueue;

//------------------------------
private:
//------------------------------

  KODE_InstanceListener*  MListener     = KODE_NULL;
  KODE_Descriptor*        MDescriptor   = KODE_NULL;
  float*                  MParamValues  = KODE_NULL;
  KODE_ParameterQueue     MParamToProcessQueue;
  
  #ifndef KODE_NO_GUI
    KODE_Editor*            MEditor     = KODE_NULL;
    KODE_ParameterQueue     MParamToEditorQueue;
    KODE_ParameterQueue     MParamToHostQueue;
  #endif // KODE_NO_GUI
  
  //uint32_t                MStateSize    = 0;
  //void*                   MStatePtr     = KODE_NULL;
  
//------------------------------
public:
//------------------------------
  
  KODE_Instance(KODE_Descriptor* ADescriptor) {
    MDescriptor = ADescriptor;
    uint32_t num = MDescriptor->getNumParameters();
    MParamValues = (float*)KODE_Malloc(num * sizeof(uint32_t));
    //don't call virtual functions in constructor??
    //instance->on_open();
    //instance->setDefaultParameterValues();
    //instance->updateAllParameters();
  }
  
  //----------
  
  virtual ~KODE_Instance() {
    if (MParamValues) KODE_Free(MParamValues);
  }
  
//------------------------------
public:
//------------------------------

  virtual void on_open() {}
  virtual void on_close() {}
  virtual void on_initialize() {}
  virtual void on_terminate() {}
  virtual void on_activate() {}
  virtual void on_deactivate() {}
  virtual void on_startUpdateAllParameters() {}
  virtual void on_endUpdateAllParameters() {}
  virtual void on_sampleRate(float ASampleRate) {}
  virtual void on_parameterChange(uint32_t AIndex, float AValue) {}
  virtual void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
  virtual void on_process(KODE_ProcessContext* AContext) {}
  
  #ifndef KODE_NO_GUI
    virtual KODE_Editor* on_openEditor(void* AParent) { return KODE_NULL; }
    virtual void on_closeEditor(KODE_Editor* AEditor) {}
    virtual void on_updateEditor(KODE_Editor* AEditor) {}
  #endif // KODE_NO_GUI
  
  virtual uint32_t on_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = KODE_NULL; return 0; }
  virtual void on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}
  
//------------------------------
public:
//------------------------------

  KODE_Descriptor*        getDescriptor()                               { return MDescriptor; }
  void                    setListener(KODE_InstanceListener* AListener) { MListener = AListener; }
  KODE_InstanceListener*  getListener()                                 { return MListener; }
  float                   getParamValue(uint32_t AIndex)                { return MParamValues[AIndex]; }
  void                    setParamValue(uint32_t AIndex, float AValue)  { MParamValues[AIndex] = AValue; }
  float*                  getParamValues()                              { return MParamValues; }
  
  //void                    setStateSize(uint32_t ASize)                  { MStateSize = ASize; }
  //void                    setStatePtr(void* APointer)                   { MStatePtr = APointer; }
  //uint32_t                getStateSize()                                { return MStateSize; }
  //void*                   getStatePtr()                                 { return MStatePtr; }
  
//------------------------------
public:
//------------------------------

  virtual uint32_t getPluginFormat() { return MListener->getPluginFormat(); }

//------------------------------
public: // parameter queues
//------------------------------

  /*
    vst2 write: setParameter, read: processBlock
  */
  
  void queueParamToProcess(uint32_t AIndex) {
    MParamToProcessQueue.write(AIndex);
  }
  
  /*
    vst2 write: setParameter, read: idle/updateEditor
  */
  
  #ifndef KODE_NO_GUI
  void queueParamToEditor(uint32_t AIndex) {
      MParamToEditorQueue.write(AIndex);
  }
  #endif // KODE_NO_GUI
  
  /*
    vst3: write: gui, read: gui timer (specific thread)
  */
  
  #ifndef KODE_NO_GUI
  void queueParamToHost(uint32_t AIndex) {
    MParamToHostQueue.write(AIndex);
  }
  #endif // KODE_NO_GUI
  
  //----------
  
  void flushParamsToProcess() {
    uint32_t index = 0;
    while (MParamToProcessQueue.read(&index) == true) {
      KODE_Parameter* param = MDescriptor->getParameter(index);
      float value = MParamValues[index];
      value = param->from01(value);
      on_parameterChange(index,value);
    }
  }
  
  //----------
  
  #ifndef KODE_NO_GUI
  void flushParamsToEditor() {
    uint32_t index = 0;
    while (MParamToEditorQueue.read(&index) == true) {
      float value = MParamValues[index];
      //KODE_Trace("%i %.3f\n",index,value);
      if (MEditor) MEditor->setParameterValueAndRedraw(index,value);
    }
  }
  #endif // KODE_NO_GUI
  
  //----------
  
  #ifndef KODE_NO_GUI
  void flushParamsToHost() {
    uint32_t index = 0;
    while (MParamToHostQueue.read(&index) == true) {
      float value = MParamValues[index];
      //notifyHostParameterChanged(index,value);
      //KODE_TRACE;
      if (MListener) MListener->notifyHostUpdateParameter(index,value);
    }
  }
  #endif // KODE_NO_GUI
  
  //----------
  
  //void notifyHostParameterChanged(uint32_t AIndex, float AValue) {
  //  if (MListener) MListener->updateParameterFromInstance(AIndex,AValue);
  //}

//------------------------------
public: // parameters
//------------------------------

  void setDefaultParameterValues() {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* param = MDescriptor->getParameter(i);
      float def = param->getDefValue();
      //def = param->to01(def);
      MParamValues[i] = def;
      //KODE_Trace("%i = %.3f\n",i,def);
    }
  }
  
  //----------
  
  void setAllParameterValues(float* AValues) {
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      MParamValues[i] = AValues[i];
    }
  }
  
  //----------
  
  void updateAllParameters() {
    on_startUpdateAllParameters();
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      KODE_Parameter* param = MDescriptor->getParameter(i);
      float v = MParamValues[i];
      v = param->from01(v);
      on_parameterChange(i,v);
    }
    on_endUpdateAllParameters();
  }
  
  //----------
  
  #ifndef KODE_NO_GUI
  void copyParameterValuesToEditor() {
    //KODE_TRACE;
    uint32_t num = MDescriptor->getNumParameters();
    for (uint32_t i=0; i<num; i++) {
      //KODE_Parameter* param = MDescriptor->getParameter(i);
      float v = MParamValues[i];
      //v = param->from01(v);
      //on_parameterChange(i,v);
      MEditor->setParameterValue(i,v);
    }
  }
  #endif // KODE_NO_GUI
  
//------------------------------
public:
//------------------------------
  
  #ifndef KODE_NO_GUI

  KODE_Editor* openEditor(void* ptr) {
    MEditor = (KODE_Editor*)on_openEditor(ptr);
    //if (!MEditor) createDefaultEditor(ptr);
    copyParameterValuesToEditor();
    return MEditor;
  }
  
  //----------

  void closeEditor(KODE_Editor* AEditor) {
    on_closeEditor(AEditor);
    MEditor = KODE_NULL;
  }
  
  #endif // KODE_NO_GUI

//------------------------------
public: // editor listener
//------------------------------

  #ifndef KODE_NO_GUI

  /*
    TODO:
    - do we need both param to process, and to host for 
      all formats?
    - will the host 'call back' to us with updated parameters (for process),
      after we notify the host?
  */
  
  /*
    vst2: notify editor
    vst3: notify host, host notifies editor
  */

  void updateParameterFromEditor(uint32_t AIndex, float AValue) override {
    //KODE_Trace("AIndex %i AValue %.3f\n",AIndex,AValue);
    //if (MIsEditorOpen) MInstance->queueParamToEditor(index);
    MParamValues[AIndex] = AValue;
    uint32_t format = MListener->getPluginFormat();
    switch(format) {
      case KODE_PLUGIN_FORMAT_EXE:
        on_parameterChange(AIndex,AValue);
        break;
      case KODE_PLUGIN_FORMAT_VST2:
        // flushed in: KODE_Vst2instance.vst2_process
        queueParamToProcess(AIndex);
        MListener->notifyHostUpdateParameter(AIndex,AValue);
        break;
      case KODE_PLUGIN_FORMAT_VST3:
        //queueParamToProcess(AIndex);
        //flushed in: KODE_Vst3Instance.onTimer
        queueParamToHost(AIndex);
        break;
    }
  }
  
  //----------

  //void sendMidiFromEditor(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
  //  KODE_TRACE;
  //  MListener->notifyHostMidiOutput(AOffset,AMsg1,AMsg2,AMsg3);
  //}
  
  //----------

  void resizeWindowFromEditor(uint32_t AWidth, uint32_t AHeight) override {
    KODE_TRACE;
    MListener->notifyHostResizeWindow(AWidth,AHeight);
  }
  
  #endif // KODE_NO_GUI

};

//----------------------------------------------------------------------
#endif
