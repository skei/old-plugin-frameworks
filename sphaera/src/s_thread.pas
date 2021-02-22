{$include sphaera.inc}
unit s_thread;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifdef SPH_WIN32}
uses
  Windows;
  //#include <unistd.h> // sleep
{$endif}

type

  SThreadBase = class
  protected
    mThreadRunning  : Boolean;
    mThreadSleep    : LongInt;
  public
    constructor create;
    destructor  destroy; override;
    procedure   startThread(ms:LongInt=-1); virtual; // -1 = no timer
    procedure   stopThread; virtual;
  public
    procedure   on_ThreadFunc; virtual; // called at thread creation, or every timer tick if ms > 0
  end;

  {$ifdef SPH_WIN32}

  SThreadWin32 = class(SThreadBase)
  private
    mThreadHandle : HANDLE;
    mThreadID : DWORD;
  public
    constructor create;
    destructor  destroy;
    procedure   startThread(ms:LongInt=-1); override;
    procedure   stopThread; override;
  end;

  SThreadImpl = SThreadWin32;

  {$endif}

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// SThreadBase
//----------------------------------------------------------------------

constructor SThreadBase.create;
begin
  mThreadRunning := False;
  mThreadSleep := -1;
end;

destructor SThreadBase.destroy;
begin
end;

procedure SThreadBase.startThread(ms:LongInt);
begin
end;

procedure SThreadBase.stopThread;
begin
end;

procedure SThreadBase.on_ThreadFunc;
begin
end;

//----------------------------------------------------------------------
// TThreadWin32
//----------------------------------------------------------------------

{$ifdef SPH_WIN32}

//static DWORD WINAPI threadProc(LPVOID data)
function win32_threadProc(data:LPVOID) : DWORD;
var
  thr : SThreadWin32;
begin
  thr := SThreadWin32(data);
  if Assigned (thr) then
  begin
    if  thr.mThreadSleep >= 0 then
    begin
      while thr.mThreadRunning = True do
      begin
        thr.on_ThreadFunc();
        Sleep(thr.mThreadSleep);
      end;
    end else thr.on_ThreadFunc();
  end;
  result := 0;
end;

//----------

constructor SThreadWin32.create;
begin
  inherited;
  mThreadHandle := 0;
  mThreadID := 0;
end;

destructor SThreadWin32.destroy;
begin
  if mThreadRunning then stopThread;
end;

procedure SThreadWin32.startThread(ms:LongInt);
begin
  mThreadSleep := ms;
  mThreadRunning := true;
  mThreadHandle := CreateThread(nil,0,@win32_threadProc,LPVOID(self),0,mThreadID);
end;

procedure SThreadWin32.stopThread;
var
  ret : Pointer;
  waiter : DWORD;
begin
  mThreadRunning := false;
  waiter := WaitForSingleObject(mThreadHandle,INFINITE);
  CloseHandle(mThreadHandle);
end;

{$endif} // SPH_WIN32

(*

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
        axThread* thr = (axThread* )data;
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

*)

//----------------------------------------------------------------------
end.

