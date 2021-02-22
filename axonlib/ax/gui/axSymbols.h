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

#ifndef axSymbols_included
#define axSymbols_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
#include "core/axRect.h"
#include "gui/axColor.h"
#include "gui/axCanvas.h"

//----------------------------------------------------------------------

struct sym_item
{
  axRect mRect;
  sym_item(axRect aRect) { mRect=aRect; }
  sym_item(int x, int y, int w, int h) { mRect.x=x; mRect.y=y; mRect.w=w; mRect.h=h; }
};

typedef axArray<sym_item> sym_items;

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class axSymbols
{
  protected:
    axSurface*  mSurface;
    sym_items   mItems;

  public:

    axSymbols(axSurface* aSurface)
      {
        mSurface = aSurface;
        //axCanvas* can = mSurface->getCanvas();
      }

    //----------

    virtual int create(char* aGrid, axRect aRect, axColor aOnColor, axColor aHalfColor) // returns index
      {
        char* ptr = aGrid;
        int rx = aRect.x;
        int ry = aRect.y;
        int rw = aRect.w;
        int rh = aRect.h;
        int rx2 = rx+rw;
        int ry2 = ry+rh;
        axCanvas* can = mSurface->getCanvas();
        for (int y=ry; y<ry2; y++ )
        {
          for (int x=rx; x<rx2; x++ )
          {
            char c = *ptr++;
            if (c=='O')
            {
              can->setPenColor(aOnColor);
              can->drawPoint(x,y);
            }
            else if (c=='o')
            {
              can->setPenColor(aHalfColor);
              can->drawPoint(x,y);
            }
          }
        }
        int num = mItems.size();
        mItems.append( sym_item(rx,ry,rw,rh) );
        return num;
      }

    //----------

    virtual void draw(axCanvas* aCanvas, int x, int y, int aIndex)
      {
        int sx = mItems[aIndex].mRect.x;
        int sy = mItems[aIndex].mRect.y;
        int sw = mItems[aIndex].mRect.w;
        int sh = mItems[aIndex].mRect.h;
        //aCanvas->drawImage(mSurface, x, y, sx,sy,sw,sh );
        aCanvas->drawSurface(mSurface, x, y, sx,sy,sw,sh );
      }
};

//----------------------------------------------------------------------
#endif
