#ifndef kode_exe_plugin_included
#define kode_exe_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_timer.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance_listener.h"
#include "plugin/exe/kode_exe_instance.h"

//----------------------------------------------------------------------

template<class DESC, class INST>
class KODE_ExePlugin
: public KODE_InstanceListener
, public KODE_TimerListener {
  
//------------------------------
private:
//------------------------------

  KODE_Descriptor*    MDescriptor   = KODE_NULL;
  KODE_Instance*      MInstance     = KODE_NULL;
  KODE_ExeInstance*   MExeInstance  = KODE_NULL;
  #ifndef KODE_NO_GUI
    KODE_Editor*        MEditor       = KODE_NULL;
    KODE_Timer*         MTimer        = KODE_NULL;
  #endif // KODE_NO_GUI
  
  //KODE_ProcessContext MContext;
  

//------------------------------
public:
//------------------------------

  KODE_ExePlugin() {
    //KODE_TRACE;
  }
  
  ~KODE_ExePlugin() {
    //KODE_TRACE;
  }
  
//------------------------------
public:
//------------------------------

  int entrypoint(int argc, char** argv) {
    //KODE_TRACE;
    MDescriptor   = KODE_New DESC();
    MInstance     = KODE_New INST(MDescriptor);
    MExeInstance  = KODE_New KODE_ExeInstance(MInstance);
    MInstance->setListener(MExeInstance);
    MInstance->on_open();
    MInstance->setDefaultParameterValues();
    MInstance->updateAllParameters();
    MInstance->on_initialize();
    MInstance->on_activate();
    #ifndef KODE_NO_GUI
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      //KODE_Editor* MEditor = MInstance->on_openEditor(KODE_NULL);
      MEditor = MInstance->openEditor(KODE_NULL);
      MEditor->setTitle(MDescriptor->getName());
//      MEditor->realignChildren();
      MEditor->open();
      KODE_Timer* timer = KODE_New KODE_Timer(this);
      timer->start(20);
      //MInstance->on_updateEditor(MEditor);
      MEditor->eventLoop();
      timer->stop();
      KODE_Delete timer;
      MEditor->close();
      //MInstance->on_closeEditor(MEditor);
      MInstance->closeEditor(MEditor);
    }
    #endif // KODE_NO_GUI
    MInstance->on_deactivate();
    MInstance->on_terminate();
    MInstance->on_close();
    KODE_Delete MExeInstance;
    KODE_Delete MInstance;
    KODE_Delete MDescriptor;
    return 0;
  }
  
//------------------------------
public: // timer
//------------------------------

  #ifndef KODE_NO_GUI

    void on_timerCallback(void) override {
      //KODE_Trace("timer!\n");
      MInstance->flushParamsToProcess();    
      //MInstance->on_process(&MContext);
      MInstance->on_updateEditor(MEditor);
      MInstance->flushParamsToEditor();    
    }
    
  #endif // KODE_NO_GUI

};

//----------------------------------------------------------------------
// exec .so
//----------------------------------------------------------------------

  /*
    https://stackoverflow.com/questions/31484785/how-to-make-a-linux-shared-object-library-runnable-on-its-own

    gcc
      main.c
      -Wall
      -Wextra
      -fPIC
      -shared
      -o main.so
      -DINTERPRETER=\"/lib64/ld-linux-x86-64.so.2\"
      -Wl,-e,entry__point
  */

//  #include <unistd.h>
//  #include <stdlib.h>
//  #include <stdio.h>
//
//  //#ifdef INTERPRETER
//  //  const char interp[] __attribute__ ((section(".interp"))) = INTERPRETER;
//  //#endif
//  
//  const char interp[] __attribute__ ((section(".interp"))) = "/lib64/ld-linux-x86-64.so.2";
//  
//  __KODE_DLLEXPORT
//  void entry__point(void) {
//    KODE_TRACE;
//    fprintf(stderr, "hello executable shared object world!\n");
//    _exit(0);
//  }

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_EXE_MAIN(DESC,INST)              \
                                              \
  KODE_ExePlugin<DESC,INST> _EXE_PLUGIN;      \
                                              \
  int main(int argc, char** argv) {           \
    /*KODE_TRACE;*/                           \
    return _EXE_PLUGIN.entrypoint(argc,argv); \
  }

//----------------------------------------------------------------------
#endif