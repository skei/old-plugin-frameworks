#ifndef s3_debug_trace_included
#define s3_debug_trace_included
//----------------------------------------------------------------------

#ifdef S3_DEBUG

  #include <stdarg.h>       // va_list, va_start, va_arg, va_end
  #include <stdio.h>        // printf
  #include "common/s3_convert.h" // SIntToStr
  //#include "common/s3_string_stdlib.h"  //

  //----------------------------------------------------------------------

  class S3_Debug_Trace {

    private:

      char  MPrefix[256];
      int   MPrefixLen;

    public:

      S3_Debug_Trace() {
        MPrefix[0] = '\0';
        MPrefixLen = 0;
      }

      //----------

      ~S3_Debug_Trace() {
      }

      //----------

      void prefix(const char* file, const char* func, int line) {
        char linebuf[256];
        S3_Strcpy(MPrefix,"[");
        S3_Strcat(MPrefix,file);
        S3_Strcat(MPrefix,":");
        S3_Strcat(MPrefix,func);
        S3_Strcat(MPrefix,":");
        S3_Strcat(MPrefix,S3_IntToString(linebuf,line));
        S3_Strcat(MPrefix,"] ");
        MPrefixLen = S3_Strlen(MPrefix);
      }

      //----------

      void trace(const char* format, ...) {
          char buffer[256];
          S3_Strcpy(buffer,MPrefix);
          va_list args;
          va_start(args,format);
          vsprintf((buffer+MPrefixLen),format,args);
          #ifdef S3_DEBUG_CONSOLE
            S3_Debug_Console_Print(buffer);
          #elif defined S3_DEBUG_SOCKET
            S3_Debug_Socket_Print(buffer);
          #elif defined S3_DEBUG_LOGFILE
            S3_Debug_Logfile_Print(buffer);
          #elif defined S3_DEBUG_WINDOW
            S3_Debug_Window_Print(buffer);
          #else
            printf("%s",buffer);
          #endif
          va_end(args);
      }

      void traceln(const char* format, ...) {
          char buffer[256];
          S3_Strcpy(buffer,MPrefix);
          va_list args;
          va_start(args,format);
          vsprintf((buffer+MPrefixLen),format,args);
          S3_Strcat(buffer,"\n");
          #ifdef S3_DEBUG_CONSOLE
            S3_Debug_Console_Print(buffer);
          #elif defined S3_DEBUG_SOCKET
            S3_Debug_Socket_Print(buffer);
          #elif defined S3_DEBUG_LOGFILE
            S3_Debug_Logfile_Print(buffer);
          #elif defined S3_DEBUG_WINDOW
            S3_Debug_Window_Print(buffer);
          #else
            printf("%s",buffer);
          #endif
          va_end(args);
      }

      //----------

      void dtrace(const char* format, ...) {
        char buffer[256];
        //SStrcpy(buffer,MPrefix);
        va_list args;
        va_start(args,format);
        //vsprintf((buffer+MPrefixLen),format,args);
        vsprintf(buffer,format,args);
        #ifdef S3_DEBUG_CONSOLE
          S3_Debug_Console_Print(buffer);
        #elif defined S3_DEBUG_SOCKET
          S3_Debug_Socket_Print(buffer);
        #elif defined S3_DEBUG_LOGFILE
          S3_Debug_Logfile_Print(buffer);
        #elif defined S3_DEBUG_WINDOW
          S3_Debug_Window_Print(buffer);
        #else
          printf("%s",buffer);
        #endif
        va_end(args);
      }

      void dtraceln(const char* format, ...) {
        char buffer[256];
        //SStrcpy(buffer,MPrefix);
        va_list args;
        va_start(args,format);
        //vsprintf((buffer+MPrefixLen),format,args);
        vsprintf(buffer,format,args);
        S3_Strcat(buffer,"\n");
        #ifdef S3_DEBUG_CONSOLE
          S3_Debug_Console_Print(buffer);
        #elif defined S3_DEBUG_SOCKET
          S3_Debug_Socket_Print(buffer);
        #elif defined S3_DEBUG_LOGFILE
          S3_Debug_Logfile_Print(buffer);
        #elif defined S3_DEBUG_WINDOW
          S3_Debug_Window_Print(buffer);
        #else
          printf("%s",buffer);
        #endif
        va_end(args);
      }

  };

  //----------------------------------------------------------------------

  static S3_Debug_Trace s3_debug_trace;

  //----------------------------------------------------------------------

  #define S3_Trace \
    s3_debug_trace.prefix(S3_StripPath(__FILE__),__FUNCTION__,__LINE__); \
    s3_debug_trace.trace

  #define S3_TraceLn \
    s3_debug_trace.prefix(S3_StripPath(__FILE__),__FUNCTION__,__LINE__); \
    s3_debug_trace.traceln

  #define S3_DTrace \
    s3_debug_trace.dtrace

  #define S3_DTraceLn \
    s3_debug_trace.dtraceln

#else // S3_DEBUG

  _S3_ALWAYS_INLINE void S3_Trace(const char* format, ...) {}
  _S3_ALWAYS_INLINE void S3_TraceLn(const char* format, ...) {}
  _S3_ALWAYS_INLINE void S3_DTrace(const char* format, ...) {}
  _S3_ALWAYS_INLINE void S3_DTraceLn(const char* format, ...) {}

#endif //S3_DEBUG

//

#define S3_TRACE S3_Trace("\n")

//----------------------------------------------------------------------
#endif
