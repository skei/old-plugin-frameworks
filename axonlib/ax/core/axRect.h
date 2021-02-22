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

#ifndef axRect_included
#define axRect_included
//----------------------------------------------------------------------

#include "core/axArray.h"

struct axRect
{

  int x;
  int y;
  int w;
  int h;

  //union
  //{
  //  //struct { int x,y,w,h; };
  //  //struct { int data[4]; };
  //  int x,y,w,h;
  //  int data[4];
  //};

  axRect() { }
  axRect(int i) { x=i; y=i; w=i; h=i; }
  axRect(int aW, int aH) { x=0; y=0; w=aW; h=aH; }
  axRect(int aX, int aY, int aW, int aH) { x=aX; y=aY; w=aW; h=aH; }

  //----------------------------------------

  void set(int i) { x=i; y=i; w=i; h=i; }
  void set(int aX, int aY, int aW, int aH) { x=aX; y=aY; w=aW; h=aH; }

  void setPos(int aX, int aY) { x=aX; y=aY; }
  void setSize(int aW, int aH) { w=aW; h=aH; }

  //----------------------------------------

  inline int x2(void) { return x+w-1; }
  inline int y2(void) { return y+h-1; }
  inline void x2(int i) { w=i-x+1; }
  inline void y2(int i) { h=i-y+1; }

  //----------

  inline void add(int aX, int aY) { x+=aX; y+=aY; }
  inline void add(int aX, int aY, int aW, int aH) { x+=aX; y+=aY; w+=aW; h+=aH; }

  //----------------------------------------

  inline bool contains(int aX, int aY)
    {
      if( aX < x ) return false;
      if( aY < y ) return false;
      if( aX >= (x+w) ) return false;
      if( aY >= (y+h) ) return false;
      return true;
    }

  bool intersects(int aX, int aY, int aW, int aH)
    {
      if( (aX+aW) < x ) return false;
      if( (aY+aH) < y ) return false;
      if( aX >= (x+w) ) return false;
      if( aY >= (y+h) ) return false;
      return true;
    }

  //  __            _____
  // |  |          :  :  |
  // |__|          :..:  |
  //     __    ->  |   ..|
  //    |  |       |  :  :
  //    |__|       |__:__:
  //

  inline void combine(int aX, int aY, int aW, int aH)
    {
      if( aX < x ) { w += (x-aX); x=aX; }
      if( aY < y ) { h += (y-aY); y=aY; }
      if( (aX+aW) >= (x+w) ) w = (aX+aW) - x;
      if( (aY+aH) >= (y+h) ) h = (aY+aH) - y;
    }

  //    ___
  //   |  _|_          _:.
  //   | | | |   ->   | | :
  //   | |_|_|        |_|.:
  //   |___|            :
  //

  inline void crop(int aX, int aY, int aW, int aH)
    {
      if( aX > x ) x = aX;
      if( aY > y ) y = aY;
      if( (aX+aW) <= (x+w) ) w = (aX+aW) - x;
      if( (aY+aH) <= (y+h) ) h = (aY+aH) - y;
    }

  //----------------------------------------

  inline bool intersects(axRect aRect) { return intersects( aRect.x, aRect.y, aRect.w, aRect.h ); }
  inline void combine(axRect aRect)    { return combine( aRect.x, aRect.y, aRect.w, aRect.h ); }

};

#define NULL_RECT axRect(0,0,0,0)

typedef axArray<axRect*> axRects;

//----------------------------------------------------------------------
#endif

