#include <stdio.h> // printf
#include "axApplication.h"

#include "core/axThread.h"
#include "core/axMutex.h"

//----------------------------------------------------------------------

//#define USE_MUTEX

axMutex myMutex;
int myVar = 0;

//----------------------------------------------------------------------

class myThread1 : public axThread
{
  public:
    virtual void doThreadFunc(void)
      {
        for (int i=0;i<1000000;i++)
        {
          #ifdef USE_MUTEX
          myMutex.lock();
          #endif
          myVar += 1;
          #ifdef USE_MUTEX
          myMutex.unlock();
          #endif
        }
      }
};

//----------

class myThread2 : public axThread
{
  public:
    virtual void doThreadFunc(void)
      {
        for (int i=0;i<1000000;i++)
        {
          #ifdef USE_MUTEX
          myMutex.lock();
          #endif
          myVar += 1;
          #ifdef USE_MUTEX
          myMutex.unlock();
          #endif
        }
      }
};

//----------

class myApp : public axApplication
{
  private:
    myThread1 mThread1;
    myThread2 mThread2;
  public:
    myApp(axContext* aContext)
    : axApplication(aContext)
      {
      }
    ~myApp() {}
    virtual int main(void)
      {
        printf("hello world!\n");
        mThread1.startThread(-1);
        mThread2.startThread(-1);

        for (int i=0;i<1000000;i++)
        {
          #ifdef USE_MUTEX
          myMutex.lock();
          #endif
          myVar += 1;
          #ifdef USE_MUTEX
          myMutex.unlock();
          #endif
        }


//        printf("sleeping for 10 seconds...\n");
//        int sec = 10;
//        usleep((sec*1000)*1000); //ms*1000;
        mThread1.stopThread();
        mThread2.stopThread();
        printf("myVar = %i\n",myVar);
        return 0;
      }

};

//----------------------------------------------------------------------
AX_APP_ENTRY(myApp);
