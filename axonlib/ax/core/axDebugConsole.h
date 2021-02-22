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

#ifndef axDebugConsole_included
#define axDebugConsole_included

///////////////////////////////////////////////////////////////////////////////
// axDebugConsole.h
///////////////////////////////////////////////////////////////////////////////

#include "axDefines.h"

#if defined AX_WIN32 && defined AX_DEBUG && defined AX_DEBUG_CONSOLE

#include <windows.h>
#include <io.h>
#include <stdio.h>

#include "axDetectWine.h"

class axDebugConsole
{
  private:
    FILE* axSfile;  // file stream

  public:
    bool init;

    axDebugConsole()
    {
      AllocConsole();
      SetConsoleTitle("axDebug");
      HWND hCw = GetConsoleWindow();
      if (hCw)
      {
        HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
        // ENABLE_EXTENDED_FLAGS = 0x0080, ENABLE_QUICK_EDIT_MODE = 0x0040
        SetConsoleMode(hIn,0x0080|0x0040);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SMALL_RECT cDim = {0,0,70,20};
        SetConsoleWindowInfo(hOut,true,&cDim);
        SetConsoleCtrlHandler(NULL,true);
        // ------------
        // detect wine
        if (!axDetectWine())
          SetWindowPos(hCw,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
        // ------------
	      HMENU hMenu = GetSystemMenu(hCw,0);
	      if (hMenu)
	      {
          DeleteMenu(hMenu,SC_CLOSE,MF_BYCOMMAND);
          DrawMenuBar(hCw);
	      }
        // _O_TEXT = 0x4000
        ax_intptr axHcrt = _open_osfhandle((ax_intptr)GetStdHandle(STD_OUTPUT_HANDLE),
                        0x4000);
        if (axHcrt)
        {
          axSfile = _fdopen(axHcrt,"w");
          if (axSfile)
          {
            *stdout = *axSfile;
            setvbuf(stdout,NULL,_IONBF,0);
            init = 1;
          }
        }
      }
    }

    ~axDebugConsole()
    {
      FreeConsole();
      if (axSfile)
        close((ax_intptr)axSfile);
    }

};

#endif // defined AX_WIN32 && defined AX_DEBUG && defined AX_DEBUG_AUTOSTD

#endif // axDebugConsole_included


/*
    #include <windows.h>
    #include <io.h>
    #include <stdio.h>


    // create a console debugger window (only one instance per process, fast)
    //allocates console and routes stdout as seen in example:
    //http://support.microsoft.com/kb/105305

    static __thread unsigned int axHcrt = 0;          // crt handle
    static __thread FILE *axSfile;                    // file stream

    // ----------------
    // destroy console
    #define axDebugConsoleDestroy \
      FreeConsole(); \
      close((int)axSfile); \
      axHcrt = 0 \

    // ----------------
    // create console
    #define axDebugConsoleCreate _axDebugConsoleCreate()
    void _axDebugConsoleCreate(void)
      if (!axHcrt)
      {
        // allocate console
        AllocConsole();
        // set title
        SetConsoleTitle("axDebug");
        // get handle for console
        // requires _WIN32_WINNT >= 0x0500 (set before windows.h)
        HWND hCw = GetConsoleWindow();
        if(hCw)
	      {
          // ENABLE_EXTENDED_FLAGS = 0x0080
          // ENABLE_QUICK_EDIT_MODE = 0x0040
          HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
          SetConsoleMode(hIn, 0x0080|0x0040);
          // size & color
          HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
          SMALL_RECT cDim = {0, 0, 50, 20};
          SetConsoleWindowInfo(hOut, true, &cDim);
          SetConsoleCtrlHandler(NULL, true);
          SetWindowPos(hCw, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		      HMENU hMenu = GetSystemMenu(hCw, 0);
		      if(hMenu)
		      {
            DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
            DrawMenuBar(hCw);
		      }
        }
        // get std handle for text output, _O_TEXT = 0x4000
        axHcrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 0x4000);
        axSfile = _fdopen(axHcrt, "w");
        *stdout = *axSfile;
        setvbuf(stdout, NULL, _IONBF, 0);
      }
    }
*/
