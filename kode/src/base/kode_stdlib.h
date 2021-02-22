#ifndef kode_stdlib_included
#define kode_stdlib_included
//----------------------------------------------------------------------

//#include <math.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

//#include <list>
//#include <vector>

using namespace std;

/*
  get rid of 'specified bound 16 equals destination size [-Wstringop-truncation]
  in string_fortified.h (gcc 9)
*/

//#pragma GCC diagnostic push
//#pragma GCC diagnostic ignored "-Wstringop-truncation"
//#pragma GCC diagnostic pop

//----------------------------------------------------------------------

#ifdef KODE_NO_STDLIB
  #include "base/kode_stdlib_math.h"
#else
  #include <math.h>
#endif

//----------------------------------------------------------------------

#ifdef KODE_NO_STDLIB
  #include "base/kode_stdlib_mem.h"
#else
  #ifdef KODE_DEBUG_MEMORY
    //#define KODE_Malloc   KODE_DebugMalloc
    //#define KODE_Realloc  KODE_DebugRealloc
    //#define KODE_Free     KODE_DebugFree
    //#define KODE_New      KODE_DebugNew
    //#define KODE_Delete   KODE_DebugDelete
  #else
    //#define KODE_Malloc   malloc
    //#define KODE_Realloc  realloc
    //#define KODE_Free     free
    //#define KODE_New      new
    //#define KODE_Delete   delete
  #endif
  #define KODE_Memcpy   memcpy
  #define KODE_Memmove  memmove
  #define KODE_Memset   memset
#endif

//----------------------------------------------------------------------

#ifdef KODE_NO_STDLIB
  #include "base/kode_stdlib_str.h"
#else
  #include <string.h>
  #define KODE_Strcat   strcat
  #define KODE_Strchr   strchr
  #define KODE_Strcmp   strcmp
  #define KODE_Strcpy   strcpy
  #define KODE_Strlen   strlen
  #define KODE_Strncpy  strncpy
  #define KODE_Strrchr  strrchr
  #define KODE_Strstr   strstr
#endif

//----------------------------------------------------------------------
#endif