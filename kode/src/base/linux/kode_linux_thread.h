#ifndef kode_linux_thread_included
#define kode_linux_thread_included
//----------------------------------------------------------------------

// TODO: strip win32 (mote to separate file)

#ifdef KODE_LINUX
  #include <pthread.h>
  //#include <unistd.h> // sleep
#endif

#ifdef KODE_WIN32
  #include <windows.h>
#endif

#include "base/kode_time.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class KODE_Thread {
  private:
    bool        MThreadRunning;
    int32_t       MThreadSleep;
    void*       MUsr;
    #ifdef KODE_LINUX
    pthread_t   MThreadHandle;
    #endif
    #ifdef KODE_WIN32
    HANDLE      MThreadHandle;
    DWORD       MThreadID;
    #endif
  private:
    #ifdef KODE_LINUX
    static void* threadProc(void* data);
    #endif
    #ifdef KODE_WIN32
    static DWORD WINAPI threadProc(LPVOID data);
    #endif
  public:
    KODE_Thread();
    virtual ~KODE_Thread();
    void start(void* usr, int32_t ms=-1); // -1 = no timer
    void stop(void);
  public:
    virtual void on_threadFunc(void* usr) { KODE_Trace("on_threadFunc\n"); };
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

KODE_Thread::KODE_Thread() {
  MUsr = KODE_NULL;
  MThreadRunning = false;
  MThreadSleep = -1;
  #ifdef KODE_LINUX
  MThreadHandle = 0;
  #endif
  #ifdef KODE_WIN32
  MThreadHandle = 0;
  MThreadID = 0;
  #endif
}

//----------

KODE_Thread::~KODE_Thread() {
  if (MThreadRunning) stop();
}

//----------

// -1 = no timer
void KODE_Thread::start(void* usr, int32_t ms) {
  KODE_Trace("start\n");
  MUsr = usr;
  MThreadSleep = ms;
  MThreadRunning = true;
  #ifdef KODE_LINUX
  pthread_create(&MThreadHandle,KODE_NULL,&threadProc,this);
  #endif
  #ifdef KODE_WIN32
  MThreadHandle = CreateThread(KODE_NULL,0,&threadProc,(LPVOID)this,KODE_NULL,&MThreadID);
  #endif
}

//----------

void KODE_Thread::stop(void) {
  KODE_Trace("stop\n");
  MThreadRunning = false;
  #ifdef KODE_LINUX
  void* ret;
  pthread_join(MThreadHandle,&ret);
  #endif
  #ifdef KODE_WIN32
  //void* ret;
  //DWORD waiter = WaitForSingleObject(MThreadHandle,INFINITE);
  CloseHandle(MThreadHandle);
  #endif
}

//----------------------------------------------------------------------

#ifdef KODE_LINUX
void* KODE_Thread::threadProc(void* data) {
  KODE_Trace("threadProc\n");
  KODE_Thread* thr = (KODE_Thread*)data;
  if (thr) {
    if (thr->MThreadSleep>=0) {
      while (thr->MThreadRunning) {
        thr->on_threadFunc(thr->MUsr);
        //usleep(thr->MThreadSleep*1000); //ms*1000;
        KODE_Sleep(thr->MThreadSleep);
      }
    } else thr->on_threadFunc(thr->MUsr);
  }
  return KODE_NULL;
}
#endif

//----------

#ifdef KODE_WIN32
DWORD WINAPI KODE_Thread::threadProc(LPVOID data) {
  KODE_Thread* thr = (KODE_Thread*)data;
  if (thr) {
    if (thr->MThreadSleep>=0) {
      while (thr->MThreadRunning) {
        thr->on_threadFunc(thr->MUsr);
        Sleep(thr->MThreadSleep);
      }
    } else thr->on_threadFunc(thr->MUsr);
  }
  return KODE_NULL;
}
#endif

//----------------------------------------------------------------------
#endif