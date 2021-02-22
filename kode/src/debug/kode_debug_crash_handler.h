#ifndef kode_debug_crash_handler_included
#define kode_debug_crash_handler_included
//----------------------------------------------------------------------

/*
  compile with: -g -rdynamic
*/

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_CRASH_HANDLER

  #include <signal.h>
  //#include <stdlib.h>
  // struct sigaction act = {0};

  /*
    https://lasr.cs.ucla.edu/vahab/resources/signals.html
    The only two signals for which a handler cannot be defined are SIGKILL and
    SIGSTOP.
  */

  const char* KODE_SignalNames[] = {
    "(0)",        // 0    kill(pid, 0)
    "SIGHUP",     // 1    Hangup.
    "SIGINT",     // 2    Interactive attention signal.
    "SIGQUIT",    // 3    Quit.
    "SIGILL",     // 4    Illegal instruction.
    "SIGTRAP",    // 5    Trace/breakpoint trap.
    "SIGABRT",    // 6    Abnormal termination.
    "(7)",
    "SIGFPE",     // 8    Erroneous arithmetic operation.
    "SIGKILL",    // 9    Killed.
    "SIGBUS",     // 10   Bus error.
    "SIGSEGV",    // 11   Invalid access to storage.
    "SIGSYS",     // 12   Bad system call.
    "SIGPIPE",    // 13   Broken pipe.
    "SIGALRM",    // 14   Alarm clock.
    "SIGTERM",    // 15   Termination request.
    "SIGURG",     // 16   Urgent data is available at a socket.
    "SIGSTOP",    // 17   Stop, unblockable.
    "SIGTSTP",    // 18   Keyboard stop.
    "SIGCONT",    // 19   Continue.
    "SIGCHLD",    // 20   Child terminated or stopped.
    "SIGTTIN",    // 21   Background read from control terminal.
    "SIGTTOU",    // 22   Background write to control terminal.
    "SIGPOLL",    // 23   Pollable event occurred (System V).
    "SIGXCPU",    // 24   CPU time limit exceeded.
    "SIGXFSZ",    // 25   File size limit exceeded.
    "SIGVTALRM",  // 26   Virtual timer expired.
    "SIGPROF",    // 27   Profiling timer expired.
    "SIGWINCH",   // 28   Window size change (4.3 BSD, Sun).
    "(29)",
    "SIGUSR1",    // 30   User-defined signal 1.
    "SIGUSR2"     // 31   User-defined signal 2.
  };

  //----------

  void kode_crash_handler_callback(int sig) {
    KODE_DTrace("\nkode_crash_handler_callback(%i)\n",sig);
    KODE_DumpCallStackSkip(0); // 2
    exit(1);
    //_exit(1);
  }

  //----------

  void KODE_InitCrashHandler(int sig) {
    signal(SIGSEGV,kode_crash_handler_callback);
  }

  bool KODE_initCrashHandlers() {
    //for (int i=0; i<32; i++) KODE_InitSignalHandler(i);
    KODE_InitCrashHandler(SIGILL);
    KODE_InitCrashHandler(SIGABRT);
    KODE_InitCrashHandler(SIGFPE);
    KODE_InitCrashHandler(SIGSEGV);
    return true;
  }

  //----------

  // uninstall signal handler?
  // done automatically when program exits?

  //------------------------------

  //static
  bool kode_is_crash_handler_initialized = KODE_initCrashHandlers();

#endif // KODE_DEBUG_CRASH_HANDLER

//----------------------------------------------------------------------
#endif