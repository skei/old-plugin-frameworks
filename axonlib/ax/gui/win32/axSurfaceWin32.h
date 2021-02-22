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

#ifndef axSurfaceWin32_included
#define axSurfaceWin32_included
//----------------------------------------------------------------------

//#include "base/axContext.h"
#include "core/axDefines.h"
//#include "base/axSurfaceBase.h"
#include "gui/axSurfaceBase.h"

class axSurfaceWin32 : public axSurfaceBase
{
  private:
    axBase* mBase;
    HWND    mWindow;
    HBITMAP mBitmap, mOldHandle;
  protected:
//    int       mWidth;
//    int       mHeight;
//    int       mDepth;
//    axCanvas* mCanvas;
  public:
    axSurfaceWin32(axBase* aBase, void* aSource, int aWidth, int aHeight, int aDepth)
    : axSurfaceBase(aWidth,aHeight,aDepth)
      {
        mBase = aBase;
        mWindow   = (HWND)aSource;
        //mWidth    = aWidth;
        //mHeight   = aHeight;
        //mDepth    = 24;
        HDC tempdc = GetDC(0);                                    // ...if this value is NULL, GetDC retrieves the device context for the entire screen...
        //#ifdef AX_DIBSECTION
        #ifdef AX_ALPHA
          BITMAPINFO bmi;
          memset(&bmi,0,sizeof(BITMAPINFO));
          bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
          bmi.bmiHeader.biWidth       = mWidth;           // width of the bitmap, in pixels.
          bmi.bmiHeader.biHeight      = -mHeight;         // height (negative)
          bmi.bmiHeader.biPlanes      = 1;                // number of planes for target device. must be set to 1.
          bmi.bmiHeader.biBitCount    = mDepth;//32;               // bits per pixel
          bmi.bmiHeader.biCompression = BI_RGB;           // uncompressed
          bmi.bmiHeader.biSizeImage   = 0;//mWidth*mHeight*4; // size, in bytes, of the image. may be set to zero for BI_RGB bitmaps.
          //HDC tempdc = GetDC(0);
          //HDC tempdc = GetDC(mWin);
          void* ptr;
          mBitmap = CreateDIBSection(tempdc, &bmi, DIB_RGB_COLORS, &ptr, NULL, 0);
        #else
          mBitmap = CreateCompatibleBitmap(tempdc,mWidth,mHeight);  // creates a bitmap compatible with the device associated with the specified device context.
        #endif
        ReleaseDC(0,tempdc);                                      // releases a device context (DC), freeing it for use by other applications.
        mCanvas = createCanvas();
        mOldHandle = (HBITMAP)SelectObject((HDC)mCanvas->getHandle(),mBitmap);
      }

    virtual ~axSurfaceWin32()
      {
        SelectObject((HDC)mCanvas->getHandle(),mOldHandle);
        delete mCanvas;
        DeleteObject(mBitmap);
      }

    //----------

    // called by: axCanvasLinux.drawImage
    virtual void* getHandle(void) { return (void*)mCanvas->getHandle(); }
    virtual void* getBitmap(void) { return (void*)mBitmap; }
    virtual axCanvas* getCanvas(void) { return mCanvas; }

    virtual axCanvas* createCanvas(void)
      {
        //wtrace("axSurfaceWin32.createCanvas");
        //axContext ctx(mWindow);
        //axContext ctx(0);
        //axCanvas* can = new axCanvas(&ctx);
        axCanvas* can = new axCanvas(mBase,NULL);
        return can;
      }

};

typedef axSurfaceWin32 axSurfaceImpl;


//----------------------------------------------------------------------
#endif

