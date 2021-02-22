#ifndef s3_debug_console_included
#define s3_debug_console_included
//----------------------------------------------------------------------

#ifdef S3_DEBUG_CONSOLE

//#include "common/s3_string_stdlib.h"

//----------------------------------------------------------------------

/*
  hmmm.. S3_Trace hasn't been defined yet..
  it needs to know if it should print to a console, a socket, or stdlib
  and, in a vst host (for example bitwig), there isn't any other way
  of informing the user that a console can't be created, since
  it 'intersects' stdout.. :-/
*/

class S3_Debug_Console {

  private:

    #ifdef S3_WIN32
    HANDLE  s3_console_out;
    #endif

  public:

    S3_Debug_Console() {
      #ifdef S3_WIN32
      //BOOL res = AllocConsole();
      //if (res==0) S3_Trace("ERROR: couldn't create console\n");
      AllocConsole();
      SetConsoleTitle("s3_console");
      s3_console_out = GetStdHandle(STD_OUTPUT_HANDLE);
      #endif
    }

    //----------

    ~S3_Debug_Console() {
      #ifdef S3_WIN32
      FreeConsole();
      #endif
    }

    //----------

    void print(const char* ABuffer) {
      #ifdef S3_WIN32
      unsigned long written;
      WriteConsole(s3_console_out,ABuffer,S3_Strlen(ABuffer),&written,S3_NULL);
      #endif
    }

};

//----------------------------------------------------------------------

static
S3_Debug_Console s3_debug_console;

//----------------------------------------------------------------------

#define S3_Debug_Console_Print  s3_debug_console.print

#endif // S3_DEBUG_CONSOLE

//----------------------------------------------------------------------
#endif
