#ifndef s3_debug_crashhandler
#define s3_debug_crashhandler
//----------------------------------------------------------------------

/*
  based on:
  http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
*/

// compile with: -g -rdynamic

//----------------------------------------------------------------------

#ifdef S3_DEBUG_CRASHHANDLER

/*
  signal...
  this is linux only.. ?
*/

#include <signal.h>

//----------------------------------------------------------------------

/*
  hmm..
  two first entries (0,1) = crash handler itself (and glib?)
  #2 = positiof of crash..
*/

void __s3_crash_handler(int sig) {
  S3_DTrace("\ns3_crash_handler(%i)\n",sig);
  S3_DumpCallStackSkip(2);
  // You can't call exit() safely from a signal handler.
  // Use _exit() or _Exit()
  //exit(1);
  _exit(1);
}

//----------------------------------------------------------------------

// http://www.yolinux.com/TUTORIALS/C++Signals.html

//----------------------------------------------------------------------

class S3_Debug_CrashHandler {
  public:
    S3_Debug_CrashHandler() {
      signal(SIGSEGV,__s3_crash_handler);
    }
    //~S3_Debug_CrashHandler() {
    //  // uninstall signal handler?
    //  // done automatically when program exits?
    //}
};

//----------------------------------------------------------------------

static S3_Debug_CrashHandler s3_crash_handler;

#endif // S3_DEBUG_CRASHHANDLER

//----------------------------------------------------------------------
#endif
