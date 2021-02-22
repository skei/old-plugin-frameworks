#ifndef s3_plugin_included
#define s3_plugin_included
//----------------------------------------------------------------------

/*
  Plugin_Base
    Plugin_Exe/Vst
      Plugin

  * cross-platform
*/

//----------------------------------------------------------------------

#ifdef S3_PLUGIN_EXE
  #include "plugin/s3_plugin_exe.h"
  typedef S3_Plugin_Exe S3_Plugin_Implementation;
#endif

#ifdef S3_PLUGIN_VST
  #include "plugin/s3_plugin_vst.h"
  typedef S3_Plugin_Vst S3_Plugin_Implementation;
#endif

#include "common/s3_thread.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Plugin;
class S3_Plugin_Thread;

//----------

class S3_Plugin_Thread : public S3_Thread {
  void on_threadFunc(void* usr) override final;
};

//----------

class S3_Plugin
: public S3_Plugin_Implementation {
  private:
    S3_Plugin_Thread* MIdleThread;
    int32             MIdleRate; // ms
    //bool              MUpdateAllProcessValues;
    //bool              MUpdateAllEditorValues;
  public:
    S3_Plugin();
    virtual ~S3_Plugin();
  public:
    void appendParameter(S3_Parameter* AParameter);
    void deleteParameters(void);
    void startIdleThread(int32 ARate); // ms
    void stopIdleThread(void);
    void saveProgram(int32 AIndex);
    void loadProgram(int32 AIndex);

  public:
    virtual void on_idle(void) { /*S3_TRACE;*/ }
  protected:
    void updateParametersInProcess(void) final;
    void updateEditorInIdle(void) final;
    void setDefaultParamValues(void) final;
    void distributeParamValues(void) final;
};

//----------


//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Plugin::S3_Plugin()
: S3_Plugin_Implementation() {
  MIdleThread = S3_NULL;
  MIdleRate = -1;
  //MUpdateAllProcessValues = false;
  //MUpdateAllEditorValues = false;
}

//----------

S3_Plugin::~S3_Plugin() {
  #ifndef S3_NO_AUTODELETE
  deleteParameters();
  #endif
  stopIdleThread();
}

//----------

void S3_Plugin::appendParameter( S3_Parameter* AParameter) {
  uint index = MParameters.size();
  AParameter->index(index);
  MParameters.append(AParameter);
}

//----------

void S3_Plugin::deleteParameters(void) {
  for (uint32 i=0; i<MParameters.size(); i++) {
    delete MParameters[i];
    //MParameters[i] = S3_NULL;
  }
}

//----------

void S3_Plugin::setDefaultParamValues(void) {
  for (uint32 i=0; i<MParameters.size(); i++) {
    float v = MParameters[i]->defaultValue();
    MHostParamValues[i] = v;
    MProcessParamValues[i] = v + 1; // force on_ParameterChange
    //MEditorParamValues[i]  = v + 1; // force redraw
  }
  MUpdateAllProcessValues = true;
  //MUpdateAllEditorValues = true;
}

//----------

void S3_Plugin::distributeParamValues(void) {
  for (uint32 i=0; i<MParameters.size(); i++) {
    float v = MHostParamValues[i];
    MProcessParamValues[i] = v + 1;  // force on_ParameterChange
    //MEditorParamValues[i]  = v + 1;  // force redraw
  }
  MUpdateAllProcessValues = true;
  //MUpdateAllEditorValues = true;
}


//----------

void S3_Plugin::startIdleThread(int32 ARate) {
  if (!MIdleThread) {
    MIdleThread = new S3_Plugin_Thread();
    MIdleRate = ARate;
    if (MIdleThread) MIdleThread->start(this,MIdleRate);
  }
}

//----------

void S3_Plugin::stopIdleThread(void) {
  if (MIdleThread) {
    MIdleThread->stop();
    delete MIdleThread;
    MIdleThread = S3_NULL;
  }
}

//----------------------------------------------------------------------

void S3_Plugin::saveProgram(int32 AIndex) {
  MPrograms.clear();
  uint32 num = MPrograms.size();
  S3_Program* prog = MPrograms[AIndex];
  for (uint32 i=0; i<num; i++) {
    float val = MHostParamValues[i];
    prog->appendValue(val);
  }
}

//----------

void S3_Plugin::loadProgram(int32 AIndex) {
  S3_Program* prog = MPrograms[AIndex];
  uint32 num = MPrograms.size();
  for (uint32 i=0; i<num; i++) {
    float val = prog->getValue(i);
    MHostParamValues[i] = val;
    distributeParamValues();
  }
}

//----------------------------------------------------------------------

/*
  if the queue isn't checked and new values removed from the queue,
  it will fill up, and we might lose updates..
*/

void S3_Plugin::updateParametersInProcess(void) {
  if (MUpdateAllProcessValues) {
    MUpdateAllProcessValues = false;
    for (uint32 i=0; i<MParameters.size(); i++) {
      float v = MHostParamValues[i];
      MProcessParamValues[i] = v;
      on_parameterChange(i, MParameters[i]->from01(v));
    }
  }
  else {
    #ifdef S3_PLUGIN_PARAMS_PROCESS_QUEUE
      uint32 i;
      while (MProcessParamQueue.read(&i)) {
        float v = MHostParamValues[i];
        if (v != MProcessParamValues[i]) {
          MProcessParamValues[i] = v;
          on_parameterChange(i, MParameters[i]->from01(v));
        }
      }
    #else
      for (uint32 i=0; i<MParameters.size(); i++) {
        float v = MHostParamValues[i];
        if (v != MProcessParamValues[i]) {
          MProcessParamValues[i] = v;
          on_parameterChange(i, MParameters[i]->from01(v));
        }
      }
    #endif
  }
}

//----------

void S3_Plugin::updateEditorInIdle(void) {
  if (MUpdateAllEditorValues) {
    MUpdateAllEditorValues = false;
    for (uint32 i=0; i<MParameters.size(); i++) {
      float v = MHostParamValues[i];
      MEditorParamValues[i] = v;
      //on_parameterChange(i, MParameters[i]->from01(v));
      //MEditor->updateParameterInIdle(i, MParameters[i]->from01(v))
    }
  }
  else {
    #ifdef S3_PLUGIN_PARAMS_EDITOR_QUEUE
      uint32 i;
      while (MEditorParamQueue.read(&i)) {
        float v = MHostParamValues[i];
        if (v != MEditorParamValues[i]) {
          MEditorParamValues[i] = v;
          //S3_Parameter* p = MParameters[i];
          //MEditor->updateParameterFromPlugin(p,v);
          //MEditor->on_pluginUpdateParameter(p,v);
        }
      }
    #else
      for (uint32 i=0; i<MParameters.size(); i++) {
        float v = MHostParamValues[i];
        if (v != MEditorParamValues[i]) {
          MEditorParamValues[i] = v;
          //S3_Parameter* p = MParameters[i];
          //MEditor->updateParameter(p,v);
        }
      }
    #endif
  }
}

//----------------------------------------------------------------------
// plugin idle thread
//----------------------------------------------------------------------

void S3_Plugin_Thread::on_threadFunc(void* usr) {
  S3_Plugin* plugin = (S3_Plugin*)usr;
  if (plugin) plugin->on_idle();
}

//----------------------------------------------------------------------
#endif
