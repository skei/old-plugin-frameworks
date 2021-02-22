#ifndef axPlatform_included
#define axPlatform_included
//----------------------------------------------------------------------

#include "base/axBase.h"

//----------------------------------------------------------------------

#ifdef AX_WIN32
#include "base/axPlatformWin32.h"
#endif

#ifdef AX_LINUX
#include "base/axPlatformLinux.h"
#endif

//----------------------------------------------------------------------
#endif
