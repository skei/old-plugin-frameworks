#ifndef s3_thread_included
#define s3_thread_included
//----------------------------------------------------------------------

#ifdef S3_LINUX
  #include <pthread.h>
  //#include <unistd.h> // sleep
#endif

#ifdef S3_WIN32
  #include <windows.h>
#endif

#include "common/s3_time.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Thread {
  private:
    bool        MThreadRunning;
    int32       MThreadSleep;
    void*       MUsr;
    #ifdef S3_LINUX
    pthread_t   MThreadHandle;
    #endif
    #ifdef S3_WIN32
    HANDLE      MThreadHandle;
    DWORD       MThreadID;
    #endif
  private:
    #ifdef S3_LINUX
    static void* threadProc(void* data);
    #endif
    #ifdef S3_WIN32
    static DWORD WINAPI threadProc(LPVOID data);
    #endif
  public:
    S3_Thread();
    virtual ~S3_Thread();
    void start(void* usr, int32 ms=-1); // -1 = no timer
    void stop(void);
  public:
    virtual void on_threadFunc(void* usr) { S3_Trace("on_threadFunc\n"); };
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Thread::S3_Thread() {
  MUsr = S3_NULL;
  MThreadRunning = false;
  MThreadSleep = -1;
  #ifdef S3_LINUX
  MThreadHandle = 0;
  #endif
  #ifdef S3_WIN32
  MThreadHandle = 0;
  MThreadID = 0;
  #endif
}

//----------

S3_Thread::~S3_Thread() {
  if (MThreadRunning) stop();
}

//----------

// -1 = no timer
void S3_Thread::start(void* usr, int32 ms) {
  S3_Trace("start\n");
  MUsr = usr;
  MThreadSleep = ms;
  MThreadRunning = true;
  #ifdef S3_LINUX
  pthread_create(&MThreadHandle,S3_NULL,&threadProc,this);
  #endif
  #ifdef S3_WIN32
  MThreadHandle = CreateThread(S3_NULL,0,&threadProc,(LPVOID)this,S3_NULL,&MThreadID);
  #endif
}

//----------

void S3_Thread::stop(void) {
  S3_Trace("stop\n");
  MThreadRunning = false;
  #ifdef S3_LINUX
  void* ret;
  pthread_join(MThreadHandle,&ret);
  #endif
  #ifdef S3_WIN32
  //void* ret;
  //DWORD waiter = WaitForSingleObject(MThreadHandle,INFINITE);
  CloseHandle(MThreadHandle);
  #endif
}

//----------------------------------------------------------------------

#ifdef S3_LINUX
void* S3_Thread::threadProc(void* data) {
  S3_Trace("threadProc\n");
  S3_Thread* thr = (S3_Thread*)data;
  if (thr) {
    if (thr->MThreadSleep>=0) {
      while (thr->MThreadRunning) {
        thr->on_threadFunc(thr->MUsr);
        //usleep(thr->MThreadSleep*1000); //ms*1000;
        S3_Sleep(thr->MThreadSleep);
      }
    } else thr->on_threadFunc(thr->MUsr);
  }
  return S3_NULL;
}
#endif

//----------

#ifdef S3_WIN32
DWORD WINAPI S3_Thread::threadProc(LPVOID data) {
  S3_Thread* thr = (S3_Thread*)data;
  if (thr) {
    if (thr->MThreadSleep>=0) {
      while (thr->MThreadRunning) {
        thr->on_threadFunc(thr->MUsr);
        Sleep(thr->MThreadSleep);
      }
    } else thr->on_threadFunc(thr->MUsr);
  }
  return S3_NULL;
}
#endif

//----------------------------------------------------------------------
#endif












#if 0






class S3_Thread {

  private:

    bool        MThreadRunning;
    int         MThreadSleep;
    #ifdef S3_LINUX
    pthread_t   MThreadHandle;
    #endif
    #ifdef S3_WIN32
    HANDLE      MThreadHandle;
    DWORD       MThreadID;
    #endif

  //----------------------------------------
  //
  //----------------------------------------

  private:

    #ifdef S3_LINUX
    static void* threadProc(void* data) {
      S3_Thread* thr = (S3_Thread*)data;
      if (thr) {
        if (thr->MThreadSleep>=0) {
          while (thr->MThreadRunning) {
            thr->on_threadFunc();
            usleep(thr->MThreadSleep*1000); //ms*1000;
          }
        } else thr->on_threadFunc();
      }
      return S3_NULL;
    }
    #endif

    //----------

    #ifdef S3_WIN32
    static DWORD WINAPI threadProc(LPVOID data) {
      S3_Thread* thr = (S3_Thread*)data;
      if (thr) {
        if (thr->MThreadSleep>=0) {
          while (thr->MThreadRunning) {
            thr->on_threadFunc();
            Sleep(thr->MThreadSleep);
          }
        } else thr->on_threadFunc();
      }
      return S3_NULL;
    }
    #endif

  public:

    S3_Thread() {
      MThreadRunning = false;
      MThreadSleep = -1;
      #ifdef S3_LINUX
      MThreadHandle = 0;
      #endif
      #ifdef S3_WIN32
      MThreadHandle = 0;
      MThreadID = 0;
      #endif
    }

    //----------

    virtual ~S3_Thread() {
      if (MThreadRunning) stop();
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // -1 = no timer



    void start(int ms=-1) {
      MThreadSleep = ms;
      MThreadRunning = true;
      #ifdef S3_LINUX
      pthread_create(&MThreadHandle,S3_NULL,&threadProc,this);
      #endif
      #ifdef S3_WIN32
      MThreadHandle = CreateThread(S3_NULL,0,&threadProc,(LPVOID)this,S3_NULL,&MThreadID);
      #endif
    }

    virtual void stop(void) {
      MThreadRunning = false;
      #ifdef S3_LINUX
      void* ret;
      pthread_join(MThreadHandle,&ret);
      #endif
      #ifdef S3_WIN32
      //void* ret;
      //DWORD waiter = WaitForSingleObject(MThreadHandle,INFINITE);
      CloseHandle(MThreadHandle);
      #endif
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // override this..
    // called at thread creation, or every timer tick if ms > 0

    virtual
    void on_threadFunc(void) {
    }

};


#endif // 0
