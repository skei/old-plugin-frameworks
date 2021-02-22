/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axDebug_included
#define axDebug_included

// pragma message
#define _pmsg(x)  _Pragma (#x)
#define pmsg(x)   _pmsg(message (#x))

// get file name from a path string
#include "axBasePath.h"

// case: debug enabled
#ifdef AX_DEBUG

  #define _WIN32_WINNT 0x0501

  #include <iostream>
  #include <fstream>
  using namespace std;

  // --------------------------------------------------------------------------

  // cout macro
  #define axCout(x) \
    cout  << "[" << axGetFileName(__FILE__) <<  ":" \
          << __LINE__ << "] " << x << endl;
  #define _axCout(x) \
    cout << x << endl;

  // --------------------------------------------------------------------------
  // check for debug log append
  #ifndef AX_DEBUG_LOG_APPEND
    #define AX_DEBUG_LOG_APPEND
  #else
    #undef AX_DEBUG_LOG_APPEND
    #define AX_DEBUG_LOG_APPEND | std::fstream::app
  #endif

  // enable log file
  #ifdef AX_DEBUG_LOG

    fstream axDfs;
    #define axDLog(x) \
      axDfs  << "[" << axGetFileName(__FILE__) << ":" \
              << __LINE__ << "] " << x << endl;
    #define _axDLog(x) \
      axDfs << x  << endl;

    #define _AX_DLOG_CHECK    if (axDfs.is_open())
    #define _AX_DEBUG_SETUP \
      char filepath[AX_MAX_PATH] = ""; \
      axFromBasePath(filepath, AX_DEBUG_LOG); \
      axDfs.open(filepath, std::fstream::out AX_DEBUG_LOG_APPEND);

  #else // AX_DEBUG_LOG

    #define  axDLog(x) (void(0))
    #define _axDLog(x) (void(0))
    #define _AX_DLOG_CHECK    if (0)
    #define _AX_DEBUG_SETUP

  #endif  // AX_DEBUG_LOG

  // --------------------------------------------------------------------------
  #ifdef AX_WIN32

    // create console
    #ifdef AX_DEBUG_CONSOLE
      #include "axDebugConsole.h"
      #define _AX_CONCHECK if (axCon.init)
      axDebugConsole axCon;
    #else
      #define _AX_CONCHECK if (0)
    #endif

    // trace macros win32
    #define trace(x)  \
      _AX_CONCHECK  axCout(x); _AX_DLOG_CHECK  axDLog(x);
    #define _trace(x) \
      _AX_CONCHECK _axCout(x); _AX_DLOG_CHECK _axDLog(x);

  #endif // AX_WIN32

  // --------------------------------------------------------------------------
  #ifdef AX_LINUX

    // trace macros linux
    #define  trace(x)  axCout(x); _AX_DLOG_CHECK  axDLog(x);
    #define _trace(x) _axCout(x); _AX_DLOG_CHECK _axDLog(x);

  #endif // AX_LINUX

  // --------------------------------------------------------------------------
#else // AX_DEBUG

  #define NDEBUG
  #define _trace(x) ((void)0)
  #define trace(x) ((void)0)
  #define _AX_DEBUG_SETUP


#endif // AX_DEBUG

#include "axAssert.h"

#endif // axDebug_included

/**
 * @file axDebug.h
 * \brief debugger methods
 *
 * if AX_DEBUG is defined, library debug mode will be enabled. usually using the
 * compile flag -DAX_DEBUG <br><br>
 *
 * while you can observe program output in linux by running a program directly
 * from a terminal, on win32 you need to define AX_DEBUG_CONSOLE.
 *
 * methods:
 * \code
 *  trace("somevar:" << somevar);
 *  pmsg("somemsg");           // compile time msg
 *  axAssert(somevar == 0);
 *  axStaticAssert(somevar == 0);
 * \endcode
 *
 * other:
 * \code
 * #define AX_DEBUG_LOG "somefile.log" //will log to "somefile.log"
 * #define AX_DEBUG_PNG // will debug picopng and PNG loading
 * #define AX_DEBUG_MEM // will debug standard axMalloc(), axFree() calls etc.
 * #define AX_DEBUG_NEW // will debug operators "new" and "delete" (needs AX_DEBUG_MEM)
 * \endcode
 *
*/
