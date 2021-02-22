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

#ifndef axCanvasBase_included
#define axCanvasBase_included
//----------------------------------------------------------------------

#include "gui/axColor.h"
#include "gui/axFont.h"
#include "gui/axBitmap.h"

//#include "base/axImage.h"
//#include "base/axSurfaceBase.h"
#include "gui/axSurfaceBase.h"

//----------------------------------------------------------------------

// text align
#define ta_Center        0
#define ta_Left          1
#define ta_Top           2
#define ta_Right         4
#define ta_Bottom        8
#define ta_TopLeft       16
#define ta_TopRight      32
#define ta_BottomLeft    64
#define ta_BottomRight   128
#define ta_RightStretch  256
#define ta_BottomStretch 512
#define ta_Horz          1024
#define ta_Vert          2048

//----------------------------------------------------------------------

class axCanvasBase
{
  public:

    virtual ~axCanvasBase()
      {
      }

    virtual void* getHandle(void) { return 0; }

    // get / set

    virtual void    setPos(int aX, int aY) {}

    // pen/brush/font

    virtual axColor getColor(int aRed, int aGreen, int aBlue) { return axColor(0); }
    virtual void    setPenColor(axColor aColor) {}
    virtual void    setBrushColor(axColor aColor) {}
    virtual void    setTextColor(axColor aColor) {}

    virtual void    clearPen(void) {}
    virtual void    resetPen(void) {}
    virtual void    clearBrush(void) {}
    virtual void    resetBrush(void) {}

    virtual void setPenWidth(int aWidth) {}
    virtual void setPenStyle(int aStyle) {}
    virtual void setBrushStyle(int aStyle) {}

    // clip rect

    virtual void    setClipRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    resetClipRect(void) {}
    virtual void    clearClipRect(void) {}

    // shapes

    virtual void    drawPoint(int aX, int aY) {}
    virtual void    drawPoint(int aX, int aY, axColor aColor) {}
    virtual void    drawLine(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    drawRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    fillRect(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    drawCircle(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    fillCircle(int aX1, int aY1, int aX2, int aY2) {}
    virtual void    drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2) {}
    virtual void    fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2) {}

    // text

    virtual void    setTextSize(int aSize) {}
    virtual int     textWidth(axString aText) {return 0;}
    virtual int     textHeight(axString aText) {return 0;}
    virtual void    drawText(int aX, int aY, axString aText) {}
    virtual void    drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign) {}

    // bitmap

    virtual void drawBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}

//    virtual void drawImage( axImage*  aImage,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {} // aka surface
//    //virtual void renderBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}
//    virtual void renderImage( axImage*  aImage,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {} // aka surface
//    virtual void stretchImage( axImage*  aImage,  int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}

    virtual void drawSurface(   axSurfaceBase* aSurface, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {} // aka surface
    virtual void renderSurface( axSurfaceBase* aSurface, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {} // aka surface
    virtual void stretchSurface(axSurfaceBase* aSurface, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH) {}

    //----------

};

//----------------------------------------------------------------------
#endif
