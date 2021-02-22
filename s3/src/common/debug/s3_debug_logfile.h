#ifndef s3_debug_logfile_included
#define s3_debug_logfile_included
//----------------------------------------------------------------------

/*
  TODO:
  can't get this to work..
*/

#ifdef S3_DEBUG_LOGFILE

#include "common/s3_string_stdlib.h"
#include "common/s3_basepath.h"
#include <stdio.h>

//----------------------------------------------------------------------

/*
  hmmm.. S3_Trace hasn't been defined yet..
  it needs to know if it should print to a console, a socket, or stdlib
  and, in a vst host (for example bitwig), there isn't any other way
  of informing the user that a console can't be created, since
  it 'intersects' stdout.. :-/
*/

class S3_Debug_Logfile {

  private:
    FILE* MHandle;
    char  MPath[256];

  public:

    S3_Debug_Logfile() {
      S3_GetBasePath(MPath);
      S3_Strcat(MPath,"s3_logfile.txt");
      MHandle = fopen(MPath,"w"); // w = create
      //MHandle = fopen("c:\\s2.txt","w"); // w = create
      //if (MHandle == NULL) S3_Trace("ERROR: couldn't create log file\n")
    }

    //----------

    ~S3_Debug_Logfile() {
      fclose(MHandle);
    }

    //----------

    /*
    MHandle = fopen(MPath,"a"); // a = append
    fputs(ABuffer,MHandle);
    //fflush(MHandle);
    fclose(MHandle);
    */

    void print(const char* ABuffer) {
      fputs(ABuffer,MHandle);
      //fwrite(ABuffer,1,S3_Strlen(ABuffer),MHandle);
      fflush(MHandle);
    }

};

//----------------------------------------------------------------------

static
S3_Debug_Logfile s3_debug_logfile;

//----------------------------------------------------------------------

#define S3_Debug_Logfile_Print  s3_debug_logfile.print

#endif // S3_DEBUG_LOGFILE

//----------------------------------------------------------------------
#endif

