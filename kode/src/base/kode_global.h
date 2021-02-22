#ifndef kode_global_included
#define kode_global_included
//----------------------------------------------------------------------

#include "base/kode_object.h"

#define KODE_MAX_GLOBAL_OBJECTS 256

//----------------------------------------------------------------------

class KODE_Global {

  private:

    bool          MInitialized          = false;
    bool          MTerminated           = false;
    uint32_t      MNumRegisteredObjects = 0;
    KODE_Object*  MRegisteredObjects[KODE_MAX_GLOBAL_OBJECTS] = {0};

  //------------------------------
  //
  //------------------------------

  public:

    KODE_Global() {
      //initialize();
    }

    //----------

    ~KODE_Global() {
      terminate();
    }

  //------------------------------
  //
  //------------------------------

  public:

    void initialize() {
      if (!MInitialized) {
        for (uint32_t i=0; i<MNumRegisteredObjects; i++) {
          MRegisteredObjects[i]->initialize();
        }
        MInitialized = true;
      }
    }

    //----------

    void terminate() {
      if (!MTerminated) {
        for (uint32_t i=0; i<MNumRegisteredObjects; i++) {
          MRegisteredObjects[i]->terminate();
        }
        #ifndef KODE_NO_AUTODELETE
        deleteObjects();
        #endif
        MTerminated = true;
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    void registerObject(KODE_Object* AObject) {
      if (MNumRegisteredObjects < KODE_MAX_GLOBAL_OBJECTS) {
        MRegisteredObjects[MNumRegisteredObjects] = AObject;
        MNumRegisteredObjects += 1;
      }
    }

    //----------

    void deleteObjects() {
      for (uint32_t i=0; i<MNumRegisteredObjects; i++) {
        if (MRegisteredObjects[i]) {
          KODE_Delete MRegisteredObjects[i];
          MRegisteredObjects[i] = KODE_NULL;
        }
      }
    }

    //----------

};

//----------------------------------------------------------------------

//static
KODE_Global KODE_GLOBAL;

//----------------------------------------------------------------------
#endif




//----------------------------------------------------------------------

/*
  https://stackoverflow.com/questions/51146196/what-happens-to-the-thread-spawned-by-a-shared-library-upon-dlclose

  common implementations attempt to catch and "fix" leaks via atexit and such
  by running the handlers at dlclose time, but there doesn't seem to be any way
  to catch and fix a thread left running.

  A(nother) way to prevent unloading is have a constructor call dlopen on
  itself to leak a reference, so that the reference count will always be
  positive and dlclose will do nothing.

  https://stackoverflow.com/questions/41110448/dlclose-gets-implicitly-called

  ... normal process termination when calling exit() or returning from main().
  dlopen() presumably registers an atexit() handler that executes all the
  termination functions of the dynamic libraries.

  It's not feasible for libraries to execute any code when a process is
  terminated abnormally. If the process is terminated by the OS instead of by
  exiting normally, the OS just releases any file handles, but it won't execute
  code in the context of the process.

*/
