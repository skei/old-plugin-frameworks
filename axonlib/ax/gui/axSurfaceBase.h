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

#ifndef axSurfaceBase_included
#define axSurfaceBase_included
//----------------------------------------------------------------------

//#include "base/axContext.h"
#include "gui/axCanvas.h"
//#include "base/axImage.h"

class axCanvas;

class axSurfaceBase// : public axImage
{
  protected:
    int       mWidth;
    int       mHeight;
    int       mDepth;
    axCanvas* mCanvas;
  public:
    axSurfaceBase(int aWidth, int aHeight, int aDepth/*=24*/)
      {
        mWidth  = aWidth;
        mHeight = aHeight;
        mDepth  = aDepth;
        mCanvas = NULL;
      }
    virtual ~axSurfaceBase() {}

    inline int        getWidth(void) { return mWidth; }
    inline int        getHeight(void) { return mHeight; }
    inline int        getDepth(void) { return mDepth; }
    virtual axCanvas* getCanvas(void) { return NULL; }

    // was axImage.h
    virtual void* getHandle(void)   { return  0; }
    virtual long getPicture(void)  { return 0; }


};

//----------------------------------------------------------------------
#endif
