#ifndef s3_paths_included
#define s3_paths_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

char*       S3_GetHomePath(char* ABuffer);
const char* S3_GetBaseFile(char* ABuffer);
const char* S3_GetBasePath(char* ABuffer);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

/*
  linux exe
  S3_DTrace("SGetHomePath %s\n",SGetHomePath(buffer)); // '/home/skei/'
  S3_DTrace("SGetBaseFile %s\n",SGetBaseFile(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug'
  S3_DTrace("SGetBasePath %s\n",SGetBasePath(buffer)); // '/home/skei/Desktop/skeilib/bin/skeilib_debug./' !!
*/

//----------

#include <stdio.h>
#include <stdlib.h>

#ifdef S3_LINUX
  #include <unistd.h> // readlink
  #include <dlfcn.h>
#endif

#ifdef S3_WIN32
  #include <Shlobj.h>
#endif

//#include "common/s3_stdlib.h"

//----------------------------------------------------------------------

//static _S3_UNUSED
char* S3_GetHomePath(char* ABuffer) {
  //SStrcat(ABuffer, getenv("HOME"));
  ABuffer[0] = '\0';

  #ifdef S3_LINUX
  char* env = getenv("HOME");
  if (env) S3_Strcat(ABuffer,env);
  //S3_Strcat(ABuffer, (char*)"\\");
  S3_Strcat(ABuffer, (char*)"/");
  #endif // S3_LINUX

  #ifdef S3_WIN32
  // https://msdn.microsoft.com/en-us/library/windows/desktop/bb762181(v=vs.85).aspx
  WCHAR wch[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_PROFILE, NULL, 0, wch))) {
    /*int l =*/ wcstombs(ABuffer,wch,S3_MAX_STRING_SIZE-1);
    S3_Strcat(ABuffer, (char*)"\\");
  }
  #endif // S3_WIN32

  return ABuffer;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//static _S3_UNUSED
const char* S3_GetBaseFile(char* ABuffer) {
  ABuffer[0] = '\0';
  #ifdef S3_LINUX
    //#include <dlfcn.h>
    #if defined S3_LIB
      Dl_info dli;
      dladdr(__func__, &dli);
      //DTrace("SGetBaseFile: '" << dli.dli_fname << "'");
      //int32 len = SStrlen(dli.dli_fname);
      S3_Strcpy(ABuffer,dli.dli_fname);
//      return ABuffer;//dli.dli_fname;
    #else //S3_LIB
      int32 len = readlink("/proc/self/exe",ABuffer,S3_MAX_PATH_SIZE-1);
      if (len != -1) {
        ABuffer[len] = '\0';
//        return ABuffer;
      }
    #endif //S3_LIB
  #endif // S3_LINUX

  #ifdef S3_WIN32
    #if defined S3_LIB || defined S3_PLUGIN_EXE
      //char filepath[S3_MAX_PATH_SIZE] = "";
      //GetModuleFileName(s3_global_WinInstance, filepath, S3_MAX_PATH_SIZE-1);
      //filepath[S3_MAX_PATH_SIZE-1] = '\0';
      GetModuleFileName(s3_global_WinInstance, ABuffer, S3_MAX_PATH_SIZE-1);
      //ABuffer[S3_MAX_PATH_SIZE-1] = '\0';
      //const char* slash = S3_Strrchr(filepath,'\\');
      //if (slash) {
      //  int len = (slash+1) - (char*)filepath;
      //  S3_Strncpy(ABuffer, filepath, len);
      //  ABuffer[len] = '\0';
      //}
      //else {
      //  S3_Strcat(ABuffer,(char*)".\\");
      //}
    #else
      S3_Strcat(ABuffer,(char*)".\\");
    #endif
  #endif // S3_WIN32


  return ABuffer;//S3_NULL;
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#ifdef S3_LINUX

  #include <dlfcn.h>
  #include <unistd.h>

  //static __S3_UNUSED
  const char* S3_GetBasePath(char* ABuffer) {
    #if defined S3_LIB
      Dl_info dli;
      dladdr(__func__, &dli);
      const char* slash = S3_Strrchr(dli.dli_fname, '/');
      if (slash) {
        int len = (slash + 1) - (char*)dli.dli_fname;
        S3_Strncpy(ABuffer, dli.dli_fname, len);
        ABuffer[len] = '\0';
      }
      else {
        S3_Strcat(ABuffer, (char*)"./");
      }
    #elif defined S3_PLUGIN_EXE
      char filepath[S3_MAX_PATH_SIZE] = "";
      long rd_res = readlink("/proc/self/exe", filepath, S3_MAX_PATH_SIZE);
      filepath[S3_MAX_PATH_SIZE-1] = '\0';
      if (rd_res) {
        const char* slash = S3_Strrchr(filepath, '/');
        if (slash) {
          int len = (slash + 1) - (char*)filepath;
          S3_Strncpy(ABuffer, filepath, len);
          ABuffer[len] = '\0';
        }
        else {
          //SStrcat(ABuffer, (char*)"./");
          //SStrcat(ABuffer, (char*)"/"); // -> "/" -> root :-/
        }
      }
    #else
      S3_Strcat(ABuffer, (char*)"./");
    #endif
    return ABuffer;
  }

#endif // S3_LINUX

//------------------------------

#ifdef S3_WIN32

  //#include <windows.h>

  //static _S3_UNUSED
  char* S3_GetBasePath(char* ABuffer) {
    #if defined S3_LIB || defined S3_PLUGIN_EXE
      char filepath[S3_MAX_PATH_SIZE] = "";
      GetModuleFileName(s3_global_WinInstance, filepath, S3_MAX_PATH_SIZE-1);
      filepath[S3_MAX_PATH_SIZE-1] = '\0';
      const char* slash = S3_Strrchr(filepath,'\\');
      if (slash) {
        int len = (slash+1) - (char*)filepath;
        S3_Strncpy(ABuffer, filepath, len);
        ABuffer[len] = '\0';
      }
      else {
        S3_Strcat(ABuffer,(char*)".\\");
      }
    #else
      S3_Strcat(ABuffer,(char*)".\\");
    #endif
    return ABuffer;
  }

#endif // S3_WIN32

//----------------------------------------------------------------------
#endif
