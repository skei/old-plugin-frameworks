#ifndef s3_debug_included
#define s3_debug_included
//----------------------------------------------------------------------

/*
  win32:

  https://support.microsoft.com/en-us/kb/317799
  In debug builds, MFC defines a preprocessor macro that expands the new
  operator to an overloaded new operator that takes two extra parameters. The
  extra parameters are the source file name and code line number. MFC can use
  this information to report memory leaks to the programmer when in debug mode.
  This works for MFC classes because MFC provides overloads for new that accept
  the extra parameters.
  However, because this expansion is done by the preprocessor, it affects all
  usage of the new operator. If any non-MFC classes are used in the project,
  their new operator is also expanded, even if no suitable overload of new is
  available in that class. This is what happens in GDI+, and as a result, you
  receive a compile-time error message.

  ----------

  * S3_DEBUG_WINDOW
    open a window with a S3_Widget_TextArea, S3_Trace appends text to this.
    problem: HINSTANCE (needed to register a window class), isn't available
    until DllMain has executed, and this is not called until all static
    objects/variables have been initialized..
    which means, we can't have a global/static debug-window, like the
    console or sockets..
    :-(
    maybe we should initialize at the start of main() ?
    as a first thing, call a function, for example S3_InitializeDebug()?
    define it in s3_debug_window.h, include in s2.h/s3_debug.h, and
    call in main()/s3_main
*/

//----------------------------------------------------------------------

#ifdef S3_DEBUG
  #define S3_DEBUG_ASSERT
#else
  #undef S3_DEBUG_ASSERT
  #undef S3_DEBUG_CALLSTACK
  #undef S3_DEBUG_CONSOLE
  #undef S3_DEBUG_CRASHHANDLER
  #undef S3_DEBUG_GDI
  #undef S3_DEBUG_LOGFILE
  #undef S3_DEBUG_MEM
  #undef S3_DEBUG_MEM_PRINT
  #undef S3_DEBUG_SOCKET
  #undef S3_DEBUG_TEST
  #undef S3_DEBUG_WINDOW
  #undef S3_DEBUG_XLIB
#endif

//----------

#ifndef S3_WIN32
  //#undef S3_DEBUG_CONSOLE
  #undef S3_DEBUG_GDI
#endif

#ifndef S3_LINUX
  //#undef S3_DEBUG_SOCKET
  #undef S3_DEBUG_XLIB
#endif

#ifdef S3_DEBUG_MEM_PRINT
  #define S3_DEBUG_MEM
#endif

#ifdef S3_DEBUG_CRASHHANDLER
  #define S3_DEBUG_CALLSTACK
#endif

//----------------------------------------------------------------------

// trace output 'destinations'

#include "common/debug/s3_debug_console.h"
#include "common/debug/s3_debug_logfile.h"
#include "common/debug/s3_debug_socket.h"
#include "common/debug/s3_debug_window.h"

// trace outputs to one of the above

#include "common/debug/s3_debug_trace.h"

// the rest uses S3_DTrace

#include "common/debug/s3_debug_memtrace.h"
#include "common/debug/s3_debug_assert.h"
#include "common/debug/s3_debug_callstack.h"
#include "common/debug/s3_debug_crashhandler.h"

#ifdef S3_LINUX
#include "common/debug/s3_debug_xlib.h"
#endif

#ifdef S3_WIN32
#include "common/debug/s3_debug_gdi.h"
#endif

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef S3_DEBUG
  #define S3_Breakpoint asm("int $0x3\n")
  //#define S3_Error(name, fmt, ...) fprintf( stderr, "S3_ERROR " #name ": " fmt "\n", __VA_ARGS__ )
#else // S3_DEBUG
  #define S3_Breakpoint {}
  //#define S3_Error(name, fmt, ...) {}
#endif // S3_DEBUG

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  called from main()
  should we initialize everything here, instead of the static object
  contructors?
*/

void S3_Debug_Initialize(void) {
  #ifdef S3_DEBUG_WINDOW
  s3_debug_window.initialize();
  #endif
}

//----------------------------------------------------------------------
#endif
