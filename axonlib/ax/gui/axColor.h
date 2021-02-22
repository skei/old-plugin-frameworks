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

#ifndef axColor_included
#define axColor_included
//----------------------------------------------------------------------

#include "core/axDefines.h"
#include "core/axArray.h"

//----------------------------------------------------------------------

#define AX_BLACK            0,   0,   0
#define AX_GREY_DARK       64,  64,  64
#define AX_GREY           128, 128, 128
#define AX_GREY_LIGHT     192, 192, 192
#define AX_WHITE          255, 255, 255

#define AX_RED            255,   0,   0
#define AX_ORANGE         255, 128,   0
#define AX_YELLOW         255, 255,   0
#define AX_GREEN            0, 255,   0
#define AX_CYAN             0, 255, 255
#define AX_BLUE             0,   0, 255
#define AX_MAGENTA        255,   0, 255

#define AX_RED_DARK       128,   0,   0
#define AX_ORANGE_DARK    128,  64,   0
#define AX_YELLOW_DARK    128, 128,   0
#define AX_GREEN_DARK       0, 128,   0
#define AX_CYAN_DARK        0, 128, 128
#define AX_BLUE_DARK        0,   0, 128
#define AX_MAGENTA_DARK   128,   0, 128

//----------------------------------------------------------------------

struct axColor
{
  unsigned long mColor;
  axColor()                             { mColor=0; }
  axColor(unsigned long aColor)         { mColor=aColor; }
  inline void set(unsigned long aColor) { mColor=aColor; }
  inline unsigned long get(void)        { return mColor; }
};

typedef axArray<axColor> axColors;

//----------------------------------------------------------------------
#endif
