/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axThread_included
#define axThread_included
//----------------------------------------------------------------------

#include "core/axDefines.h"

//----------------------------------------------------------------------

class axThreadBase
{
  protected:
    bool  mThreadRunning;
    int   mThreadSleep;
  public:
    axThreadBase()                      { mThreadRunning=false; mThreadSleep=-1; }
    virtual ~axThreadBase()             {}
    virtual void startThread(int ms=-1) {} // -1 = no timer
    virtual void stopThread(void)       {}
    //
    // override this..
    // called at thread creation, or every timer tick if ms > 0
    virtual void doThreadFunc(void)     {}
    //
};

//----------------------------------------------------------------------
#ifdef AX_LINUX
//----------------------------------------------------------------------

#include <pthread.h>
#include <unistd.h> // sleep

class axThread : public axThreadBase
{
  private:
    pthread_t mThreadHandle;
    //bool      mThreadRunning;
    //int       mThreadSleep;

  private:

    static void* threadProc(void* data)
      {
        axThread* thr = (axThread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning)
            {
              thr->doThreadFunc();
              usleep(thr->mThreadSleep*1000); //ms*1000;
            }
          } else thr->doThreadFunc();
        }
        return NULL;
      }

  public:

    axThread()
    : axThreadBase()
      {
        mThreadHandle = 0;
        //mThreadRunning = false;
        //mThreadSleep = -1;
      }

    virtual ~axThread()
      {
        if (mThreadRunning) stopThread();
      }

    //virtual void doThreadFunc(void) {}

    virtual void startThread(int ms=-1)
      {
        mThreadSleep = ms;
        mThreadRunning = true;
        pthread_create(&mThreadHandle,NULL,&threadProc,this);
      }

    virtual void stopThread(void)
      {
        mThreadRunning = false;
        void* ret;
        pthread_join(mThreadHandle,&ret);
      }

};

#endif

//----------------------------------------------------------------------
#ifdef AX_WIN32
//----------------------------------------------------------------------

#include <windows.h>
//#include <unistd.h> // sleep

class axThread : public axThreadBase
{
  private:
    HANDLE  mThreadHandle;
    DWORD   mThreadID;
      //bool  mThreadRunning;
      //int   mThreadSleep;

  private:

    static DWORD WINAPI threadProc(LPVOID data)
      {
        axThread* thr = (axThread*)data;
        if (thr)
        {
          if (thr->mThreadSleep>=0)
          {
            while (thr->mThreadRunning)
            {
              thr->doThreadFunc();
              Sleep(thr->mThreadSleep);
            }
          } else thr->doThreadFunc();
        }
        return NULL;
      }

  public:

    axThread() //: axThreadBase()
      {
        mThreadHandle = 0;
        mThreadID = 0;
        //mThreadRunning = false;
        //mThreadSleep = -1;
      }

    virtual ~axThread()
      {
        if (mThreadRunning) stopThread();
      }

    virtual void startThread(int ms=-1)
      {
        mThreadSleep   = ms;
        mThreadRunning = true;
        mThreadHandle = CreateThread(NULL,0,&threadProc,(LPVOID)this,NULL,&mThreadID);
      }

    virtual void stopThread(void)
      {
        void* ret;
        mThreadRunning = false;
        DWORD waiter = WaitForSingleObject(mThreadHandle,INFINITE);
        CloseHandle(mThreadHandle);
      }

};

#endif
//----------------------------------------------------------------------
#endif
