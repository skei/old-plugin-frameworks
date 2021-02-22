#ifndef axInterface_included
#define axInterface_included
//----------------------------------------------------------------------

#include "base/axBase.h"

//----------------------------------------------------------------------

#ifdef AX_NOGUI
#include "base/axInterfaceNone.h"
#else

  #ifdef AX_LINUX
  #include "base/axInterfaceLinux.h"
  #endif

  #ifdef AX_WIN32
  #include "base/axInterfaceWin32.h"
  #endif

#endif // !NOGUI

//----------------------------------------------------------------------
#endif
