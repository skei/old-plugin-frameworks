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

#ifndef axPoint_included
#define axPoint_included
//----------------------------------------------------------------------

struct axPoint
{
  int x,y;

  axPoint()
    {
    }

  axPoint(int i)
    {
      x = i;
      y = i;
    }

  axPoint(int aX, int aY)
    {
      x = aX;
      y = aY;
    }

  //----------

  void clear(void)
    {
    }

  void set(int i)
    {
      x = i;
      y = i;
    }

  void set(int aX, int aY)
    {
      x = aX;
      y = aY;
    }

  inline void add(int aX, int aY)
    {
      x += aX;
      y += aY;
    }

};

#define NULL_POINT axPoint(0,0)

//----------------------------------------------------------------------
#endif


