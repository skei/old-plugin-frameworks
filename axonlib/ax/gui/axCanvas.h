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

#ifndef axCanvas_included
#define axCanvas_included
//----------------------------------------------------------------------

// note: it loses its pens,brushes, etc, if you resize the window?
// resources are 'appended' to the canvas, and a new surface, and thus
// also the canvas if you resize the window
// either:
// set default set (or skin) in constructor
// or find a way to copy the resources in resizeBuffer

//----------------------------------------------------------------------

#include "core/axDefines.h"
#include "core/axPoint.h"
#include "core/axRect.h"
//#include "base/axCanvasBase.h"
#include "gui/axCanvasBase.h"

//----------

#ifdef AX_LINUX
  #include "gui/linux/axCanvasLinux.h"
#endif

#ifdef AX_WIN32
  #include "gui/win32/axCanvasWin32.h"
#endif

//----------------------------------------------------------------------

class axCanvas : public axCanvasImpl
{
  friend class axSurfaceWin32;
  friend class axSurfaceLinux;
  friend class axWindowWin32;
  friend class axWindowLinux;
  //public:
  protected:
    axCanvas(axBase* aBase, void* aSource)
    : axCanvasImpl(aBase,aSource)
      {
      }
  public:
    virtual ~axCanvas()
      {
      }

};

//----------------------------------------------------------------------
#endif

/*
  some notes about bitmaps & blitting:

  win32:
  http://en.wikipedia.org/wiki/Graphics_Device_Interface
  "In Windows Vista... GDI is no longer hardware-accelerated by the video card driver."
  "Windows 7 includes GDI hardware acceleration for blitting operations"
  "GDI+ will continue to rely on software rendering in Windows 7"

  linux:
  http://en.wikipedia.org/wiki/Xrender
  "..designed to target the 3D graphics capabilities of newer video cards."
*/
