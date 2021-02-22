#ifndef s3_included
#define s3_included
//----------------------------------------------------------------------

#include "s3_config.h"

#include "common/s3_defines.h"
#include "common/s3_attributes.h"
#include "common/s3_types.h"
#include "common/s3_const.h"
#include "common/s3_enums.h"
#include "common/s3_stdlib.h"
#include "common/s3_debug.h"

#include "s3_main.h"

#ifdef S3_LINUX
  #include "common/s3_linux.h"
#endif

#ifdef S3_WIN32
  #include "common/s3_win32.h"
#endif

//----------------------------------------------------------------------
#endif


/*

  compile

  g++
    -Wall
    -std=c++11
    -fvisibility=hidden
    -m64
    -g
    -fPIC
    -rdynamic
    -Wl,-Bsymbolic
    -DS3_PLUGIN_VST
    -DS3_DEBUG
    -I../src
    -c /DATA/code/s3/build/main.cpp
    -o obj/debug/build/main.o

  link

  g++
    -shared
    obj/debug/build/main.o
    -o ../bin/s3_64_debug.so
    -m64
    -lpthread
    -ldl

  ----------

  -ffast_math
  -funsafe_math_optimizations
*/
