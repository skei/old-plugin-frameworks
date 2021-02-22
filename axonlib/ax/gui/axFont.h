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

#ifndef axFont_included
#define axFont_included
//----------------------------------------------------------------------

#include "core/axDefines.h"
#include "core/axString.h"

//----------------------------------------------------------------------

#ifdef AX_LINUX

struct axFont
{
  axString  mName;
  int       mSize;
  int       mStyle;
};

#endif

#ifdef AX_WIN32

struct axFont
{
  axString  mName;
  int       mSize;
  int       mStyle;
};

#endif

typedef axArray<axFont> axFonts;

//----------------------------------------------------------------------
#endif
