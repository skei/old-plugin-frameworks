#ifndef kode_debug_trace_included
#define kode_debug_trace_included
//----------------------------------------------------------------------

//#define KODE_DEBUG_TRACE
//#define KODE_DEBUG_TRACE_SOCKET
//#define KODE_DEBUG_TRACE_THREAD


/*
  see socket output by entering this in a console:
  nc -U -l -k /tmp/kode.socket
*/

//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_stdlib.h"

#ifdef KODE_DEBUG_TRACE

  #include <stdarg.h>
  #include <stdio.h> 

  #ifdef KODE_DEBUG_TRACE_SOCKET
    #include <unistd.h>
    #include <sys/socket.h>
    #include <sys/un.h>
  #endif

  #ifdef KODE_DEBUG_TRACE_THREAD
    #include <sys/syscall.h>
    #include <sys/unistd.h>
    #define gettid() syscall(SYS_gettid)
    pid_t getProcessID() { return getpid(); }
    pid_t getThreadID() { return gettid(); }
  #endif // KODE_DEBUG_TRACE_THREAD

  #ifdef KODE_DEBUG_TRACE_TIME
    #include "base/kode_time.h"
  #endif

  //----------------------------------------------------------------------


  class KODE_DebugTrace {

  //------------------------------
  private:
  //------------------------------

    char        buffer[256]         = {0};
    char        prefix_buffer[256]  = {0};
    #ifdef KODE_DEBUG_TRACE_SOCKET
    int         socket_handle       = 0;
    #endif

    #ifdef KODE_DEBUG_TRACE_TIME
  //double      start_time          = 0;
    KODE_Time   time;
    #endif

  //------------------------------
  public:
  //------------------------------

    KODE_DebugTrace() {
      //print("KODE_DebugTrace()\n");
      //setlocale(LC_NUMERIC, "C");
      #ifdef KODE_DEBUG_TRACE_SOCKET
        init_socket();
      #endif
      #ifdef KODE_DEBUG_TRACE_TIME
        //dprint("CLOCKS_PER_SEC %i\n",CLOCKS_PER_SEC);
        //start_time = clock(); //KODE_GetTimeMS();
        time.start();
      #endif
    }

    //----------

    ~KODE_DebugTrace() {
      //print("~KODE_DebugTrace()\n");
      #ifdef KODE_DEBUG_TRACE_SOCKET
        close_socket();
      #endif
    }

  //------------------------------
  private:
  //------------------------------

    // /home/skei/test -> test

    const char* strip_path(const char* buffer) {
      const char *slash;
      //const char *backslash;
      slash = KODE_Strrchr(buffer,'/');
      //backslash = KODE_Strrchr(buffer,'\\');//+1;
      if (slash) {
        return slash + 1;
      }
      //else if (backslash) {
      //  _copy_buffer(backslash + 1); // see above
      //}
      return buffer;
    }

    //----------

    /*
    int32_t get_time(void) {
      time_t    sec;
      timespec  spec;
      clock_gettime(CLOCK_REALTIME, &spec);
      sec = spec.tv_sec;
      uint32_t millisec = round(spec.tv_nsec / 1.0e6); // nanoseconds -> milliseconds
      if (millisec > 999) { sec++; millisec = 0; }
      return ((sec*1000) + millisec) - start_time;
      //uint32_t microsec = round(spec.tv_nsec / 1.0e3); // nanoseconds -> milliseconds
      //if (microsec > 999999) { sec++; microsec = 0; }
      //return ((sec*1000000) + microsec) - start_time;
    }
    */

  //------------------------------
  public:
  //------------------------------

    #ifdef KODE_DEBUG_TRACE_SOCKET

      void init_socket() {
        //open_socket();
        socket_handle = socket(PF_UNIX,SOCK_STREAM,0);
        sockaddr_un address;
        KODE_Memset(&address,0,sizeof(sockaddr_un));
        address.sun_family = AF_UNIX;
        snprintf(address.sun_path,108,"/tmp/kode.socket"); // max 108?
        connect(socket_handle,reinterpret_cast<sockaddr*>(&address),sizeof(sockaddr_un));
      }

      void close_socket() {
        close(socket_handle);
      }

      void print_socket(char* buffer) {
        dprintf(socket_handle,"%s",buffer);
      }

    #endif

  //------------------------------
  public:
  //------------------------------

    void print_stdout(char* buffer) {
      printf("%s",buffer);
    }

    //----------

    void prefix(/*double time,*/const char* file, const char* func, int line) {

      //double t = time - start_time;
      //double t = (clock() - start_time) / 10000.0;// / CLOCKS_PER_SEC;
      //double t = time.ms() / 10.0;
      
      #ifdef KODE_DEBUG_TRACE_THREAD
        #ifdef KODE_DEBUG_TRACE_TIME
          double t = time.ms() / 10.0;
          sprintf(prefix_buffer,"[%.4f:%i:%s:%s:%i] ",t,getThreadID(),func,strip_path(file),line);
        #else
          sprintf(prefix_buffer,"[%i:%s:%s:%i] ",getThreadID(),func,strip_path(file),line);
        #endif
      #else
        #ifdef KODE_DEBUG_TRACE_TIME
          double t = time.ms() / 10.0;
          sprintf(prefix_buffer,"[%.4f:%s:%s:%i] ",t,func,strip_path(file),line);
        #else
          sprintf(prefix_buffer,"[%s:%s:%i] ",func,strip_path(file),line);
        #endif
      #endif
    }

    //----------

    void print(const char* format, ...) {
      va_list args;
      va_start(args,format);
      vsprintf(buffer,format,args);
      KODE_Strcat(prefix_buffer,buffer);
      #ifdef KODE_DEBUG_TRACE_SOCKET
        print_socket(prefix_buffer);
      #else
        print_stdout(prefix_buffer);
      #endif
      prefix_buffer[0] = 0;
      va_end(args);
    }

    //----------

    void dprint(const char* format, ...) {
      va_list args;
      va_start(args,format);
      vsprintf(buffer,format,args);
      #ifdef KODE_DEBUG_TRACE_SOCKET
        print_socket(buffer);
      #else
        print_stdout(buffer);
      #endif
      va_end(args);
    }

  };

  //----------

  KODE_DebugTrace kode_global_trace;

  #define KODE_Trace \
    kode_global_trace.prefix( /*KODE_GetTimeMS(),*/ __FILE__, __FUNCTION__, __LINE__ ); \
    kode_global_trace.print

  #define KODE_DTrace \
    kode_global_trace.dprint

#else // KODE_DEBUG_TRACE

  #define KODE_Trace    KODE_NoTrace
  #define KODE_DTrace   KODE_NoTrace

#endif // KODE_DEBUG_TRACE

//----------

#define KODE_TRACE KODE_Trace("\n");
void KODE_NoTrace(const char*,...) {}

//----------------------------------------------------------------------
#endif


























#if 0

#include <iostream>
#include <fstream>
using namespace std;

#include "src/h_BasePath.h"

#ifdef H_DEBUG_LOG_UNIQUE
  #include "src/h_Timestamp.h"
#endif

//----------------------------------------------------------------------

#ifndef H_DEBUG_LOG_APPEND
  #define H_DEBUG_LOG_APPEND
#else
  #undef H_DEBUG_LOG_APPEND
  #define H_DEBUG_LOG_APPEND | std::fstream::app
#endif

//----------------------------------------------------------------------

class KODE_LogFile {
  
private:

  char          m_LogFileName[H_MAX_PATHSIZE];
  fstream       m_LogFile; // static?
  #ifdef H_DEBUG_LOG_UNIQUE
  h_Timestamp m_TS;
  #endif
  
public:

  inline fstream& _trace(void) { return m_LogFile; }
  
public:

  KODE_LogFile(const char* a_FileName)
    {
      #ifndef H_DEBUG_LOG_HOME
        // try writing to base path

        m_LogFileName[0] = '\0';
        h_GetBasePath(m_LogFileName);
        #ifdef H_DEBUG_LOG_UNIQUE
          h_Strcat(m_LogFileName, m_TS.str());
          h_Strcat(m_LogFileName, (char*)"_");
        #endif

        h_Strcat(m_LogFileName, a_FileName);
        m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);

        if (m_LogFile.fail())
        {
          printf( "ERROR: cannot write log to base path: %s\n",
                m_LogFileName);
      #endif
          // write to home path instead

          m_LogFileName[0] = '\0';
          h_GetHomePath(m_LogFileName);
          #ifdef H_DEBUG_LOG_UNIQUE
            h_Strcat(m_LogFileName, m_TS.str());
            h_Strcat(m_LogFileName, (char*)"_");
          #endif

          h_Strcat(m_LogFileName, a_FileName);
          m_LogFile.open(m_LogFileName, std::fstream::out H_DEBUG_LOG_APPEND);

          if (m_LogFile.fail())
          {
            printf( "ERROR: cannot write log to home path: %s\n",
                  m_LogFileName);
            m_LogFile.clear();
          }
      #ifndef H_DEBUG_LOG_HOME
        }
      #endif
    }

  ~KODE_LogFile()
    {
      m_LogFile.close();
    }

  //void print(const char* x)
  //  {
  //    m_LogFile << x << endl;
  //  }

};

#endif // 0
