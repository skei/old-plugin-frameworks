unit z_thread;
{$include zenith.inc}

interface
implementation
end.

(*

  4.5 Thread Variables

  For a program which uses threads, the variables can be really global, i.e.
  the same for all threads, or thread-local: this means that each thread gets a
  copy of the variable. Local variables (deﬁned inside a procedure) are always
  thread-local. Global variables are normally the same for all threads. A global
  variable can be declared thread-local by replacing the var keyword at the
  start of the variable declaration block with Threadvar:

  Threadvar
    IOResult : Integer;

  If no threads are used, the variable behaves as an ordinary variable. If
  threads are used then a copy is made for each thread (including the main
  thread). Note that the copy is made with the original value of the variable,
  not with the value of the variable at the time the thread is started.
  Threadvars should be used sparingly: There is an overhead for retrieving or
  setting the variable’s value. If possible at all, consider using local
  variables; they are always faster than thread variables.
  Threads are not enabled by default. For more information about programming
  threads, see the chapter on threads in the Programmer’s Guide.

  BeginThread
  EndThread
  SuspendThread
  ResumeThread
  KillThread
  WaitForThreadTerminate
  ThreadSetPriority
  GetCurrentThreadId
  ThreadSwitch

  InitCriticalSection
  DoneCriticalSection
  EnterCriticalSection
  LeaveCriticalSection

  TThreadID
  TThreadFunc

  TThreadManager
    InitThreadVar
    RelocateThreadVar
    AllocateThreadVar
    ReleaseThreadVar

  InterlockedIncrement

*)
