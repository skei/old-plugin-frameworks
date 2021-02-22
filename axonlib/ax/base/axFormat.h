#ifndef axFormat_included
#define axFormat_included
//----------------------------------------------------------------------

#include "base/axBase.h"

//----------------------------------------------------------------------

#ifdef AX_FORMAT_EXE
#include "base/axFormatExe.h"
#endif

#ifdef AX_FORMAT_VST
#include "base/axFormatVst.h"
#endif

#ifdef AX_FORMAT_LADSPA
#include "base/axFormatLadspa.h"
#endif

//----------------------------------------------------------------------

#ifdef AX_NOGUI
  #define AX_MAIN(d,i) AX_ENTRYPOINT(AX_PLATFORM,axInterfaceNone,AX_FORMAT,d,i)
#else
  #define AX_MAIN(d,i) AX_ENTRYPOINT(AX_PLATFORM,AX_INTERFACE,AX_FORMAT,d,i)
#endif

//----------------------------------------------------------------------

#ifndef AX_WIN_DEFAULT
  #define AX_WIN_DEFAULT 0
#endif

//----------------------------------------------------------------------
#endif
