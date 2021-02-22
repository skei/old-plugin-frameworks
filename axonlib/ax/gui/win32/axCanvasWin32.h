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

#ifndef axCanvasWin32_included
#define axCanvasWin32_included
//----------------------------------------------------------------------

//#include <math.h>
#include "core/axMath.h" // axMax

//#include "base/axContext.h"
#include "core/axDefines.h"
//#include "base/axCanvasBase.h"
#include "gui/axCanvasBase.h"
//#include "base/axSurfaceBase.h"
#include "gui/axBitmap.h"

//#ifndef AlphaBlend
//WINGDIAPI BOOL WINAPI AlphaBlend(HDC,int,int,int,int,HDC,int,int,int,int,BLENDFUNCTION);
//#endif

// #if (WINVER >= 0x0410)
//WINGDIAPI BOOL WINAPI GradientFill(HDC,PTRIVERTEX,ULONG,PVOID,ULONG,ULONG);
//WINGDIAPI BOOL WINAPI TransparentBlt(HDC,int,int,int,int,HDC,int,int,int,int,UINT);
//#endif


//----------------------------------------------------------------------

#define DEF_PENWIDTH 0

//----------------------------------------------------------------------

class axCanvasWin32 : public axCanvasBase
{
  private:
		HDC     mDC;
    HFONT   mFont;
		HPEN    mPen, mOldPen;      // set when clearPen, use with resetPen()
    HPEN    mNullPen;     // empty pen (shapes without borders)
		HBRUSH  mBrush, mOldBrush;
    HBRUSH  mNullBrush;
    RECT    R;            // temp
    HRGN    mClipRegion;
    BLENDFUNCTION mBlendFunc;
  protected:
		//int     mWinHandle;   // from context
		HWND    mWinHandle;   // from context
    axColor mPenColor;
    axColor mBrushColor;
    axColor mTextColor;
    int     mCurrentPen;
    int     mCurrentBrush;
    int     mXpos,mYpos;
    bool    mHasClipRect;
    int     mClipX1,mClipY1,mClipX2,mClipY2;

  public:

    axCanvasWin32(axBase* aBase, void* aSource)
    : axCanvasBase(/*aContext*/)
      {
        mPenColor   = getColor(AX_GREY_LIGHT);
        mBrushColor = getColor(AX_GREY);
        mTextColor  = getColor(AX_WHITE);
        mCurrentPen = 0;
        mCurrentBrush = 0;
        mHasClipRect = false;
        HDC tempdc = GetDC(0);
        mWinHandle = (HWND)aSource;
        //if (aContext->mWindow)
        if (mWinHandle)
        {
          // window
          //mWinHandle = (int)aContext->mWindow;
          mDC = GetDC(mWinHandle);
        }
        else
        {
          // surface
          //mWinHandle = 0;
          mDC = CreateCompatibleDC(tempdc);
        }
// font
        LOGFONT LFont;
        memset(&LFont,0,sizeof(LFont));
        strcpy(LFont.lfFaceName,"Arial");
        LFont.lfHeight = -MulDiv(8,GetDeviceCaps(tempdc,LOGPIXELSY),72);
        mFont = CreateFontIndirect(&LFont);
        SelectObject(mDC,mFont);
//
        ReleaseDC(0,tempdc);
        mNullPen = CreatePen(PS_NULL,0,0);
        //http://msdn.microsoft.com/en-us/library/dd145035%28v=VS.85%29.aspx
        LOGBRUSH logbrush;
        logbrush.lbStyle = BS_NULL; // BS_HATCHED, BS_HOLLOW, BS_NULL, BS_SOLID, ++
        logbrush.lbColor = 0;       // ignored if null
        logbrush.lbHatch = 0;       // if BS_HATCHED: HS_BDIAGONAL, HS_CROSS, HS_DIAGCROSS, HS_FDIAGONAL, HS_HORIZONTAL, HS_VERTICAL
        mNullBrush = CreateBrushIndirect(&logbrush);
        mPen   = (HPEN)GetStockObject(DC_PEN);
        mBrush = (HBRUSH)GetStockObject(DC_BRUSH);
        SelectObject(mDC,mPen);
        SelectObject(mDC,mBrush);
        // from wingdi.h:
        //#define AC_SRC_OVER		          0x00
        //#define AC_SRC_ALPHA		        0x01
        //#define AC_SRC_NO_PREMULT_ALPHA	0x01
        //#define AC_SRC_NO_ALPHA		      0x02
        //#define AC_DST_NO_PREMULT_ALPHA	0x10
        //#define AC_DST_NO_ALPHA		      0x20
        mBlendFunc.BlendOp              = AC_SRC_OVER;
        mBlendFunc.BlendFlags           = 0;
        mBlendFunc.SourceConstantAlpha  = 255;//128;//0x7f;
        mBlendFunc.AlphaFormat          = AC_SRC_ALPHA; // 0 =  ignore source alpha channel
      }

    //----------

    virtual ~axCanvasWin32()
      {
        DeleteObject(mNullPen);
        DeleteObject(mNullBrush);
        if (mFont) DeleteObject(mFont);
        if (mWinHandle) ReleaseDC((HWND)mWinHandle,mDC);
        else DeleteDC(mDC);
      }

    //----------

    virtual void* getHandle(void) { return (void*)mDC; }

    //--------------------------------------------------
    //
    //--------------------------------------------------

    virtual void setPos(int aX, int aY)
      {
        MoveToEx(mDC,aX,aY,0);
        mXpos = aX;
        mYpos = aY;
      }

    //--------------------

    virtual axColor getColor(int aRed, int aGreen, int aBlue)
      {
        axColor color = RGB(aRed,aGreen,aBlue);
        return color;
      }

    virtual void setPenColor(axColor aColor)
      {
        mPenColor = aColor;
        SetDCPenColor(mDC, aColor.get());
      }

    virtual void setBrushColor(axColor aColor)
      {
        mBrushColor = aColor;
        SetDCBrushColor(mDC, aColor.get());
      }

    virtual void setTextColor(axColor aColor)
      {
        mTextColor = aColor;
        SetTextColor(mDC,aColor.get());
      }

    virtual void clearPen(void)
      {
        mOldPen = (HPEN)SelectObject( (HDC)mDC, mNullPen );
      }

    virtual void resetPen(void)
      {
        HPEN prev = (HPEN)SelectObject(mDC,mOldPen);
        DeleteObject(prev);
      }

    virtual void clearBrush(void)
      {
        mOldBrush = (HBRUSH)SelectObject( (HDC)mDC, mNullBrush );
      }

    virtual void resetBrush(void)
      {
        HBRUSH prev = (HBRUSH)SelectObject(mDC,mOldBrush);
        DeleteObject(prev);
      }

    // internal

    void setPen(int aColor, int aWidth=DEF_PENWIDTH)
      {
        HPEN pen = CreatePen(PS_SOLID,aWidth,aColor);
        mOldPen = (HPEN)SelectObject((HDC)mDC, pen);
      };

    void setBrush(int aColor)
      {
        if (mBrush)
        {
          SelectObject(mDC,mOldBrush);
          DeleteObject(mBrush);
        }
        mBrush = CreateSolidBrush(aColor);
        mOldBrush = (HBRUSH)SelectObject((HDC)mDC, mBrush);
      };

    // internal

    virtual void setPenWidth(int aWidth)
      {
        //mPenWidth = aWidth;
        setPen(mPenColor.mColor,aWidth);
      };

    virtual void setPenStyle(int aStyle)
      {
      };

    virtual void setBrushStyle(int aStyle)
      {
      };


    //--------------------------------------------------
    // clip rect
    //--------------------------------------------------

    // The CreateRectRgn function creates a rectangular region.

    // Regions created by the Create<shape>Rgn methods
    // (such as CreateRectRgn and CreatePolygonRgn) only include
    // the interior of the shape; the shape's outline is excluded
    // from the region. This means that any point on a line between
    // two sequential vertices is not included in the region.
    // If you were to call PtInRegion for such a point, it would return zero as the result.

    // The SelectClipRgn function selects a region as the current clipping region for the specified device context.

    virtual void setClipRect(int aX1, int aY1, int aX2, int aY2)
      {
        //HRGN hrgn = /*hrgn = */CreateRectRgn( aX1-1, aY1-1, aX2+1, aY2+1 );
        if (mHasClipRect) DeleteObject(mClipRegion);
        mClipRegion = /*hrgn = */CreateRectRgn( aX1-1, aY1-1, aX2+1, aY2+1 );
        SelectClipRgn(mDC,mClipRegion);
        mHasClipRect = true;
        mClipX1 = aX1;
        mClipY1 = aY1;
        mClipX2 = aX2;
        mClipY2 = aY2;
      }

    //----------

    virtual void resetClipRect(void)
      {
        //clearClipRect();
        setClipRect(mClipX1,mClipY1,mClipX2,mClipY2);
      }

    //----------

    // To remove a device-context's clipping region, specify a NULL region handle.

    virtual void clearClipRect(void)
      {
        SelectClipRgn(mDC,NULL);
        if (mHasClipRect) DeleteObject(mClipRegion);
      }

    //--------------------------------------------------
    // shapes
    //--------------------------------------------------

    virtual void drawPoint(int aX, int aY)
      {
        //SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
        SetPixel(mDC,aX,aY,mPenColor.get());
      }

    //----------

    virtual void drawPoint(int aX, int aY, axColor aColor)
      {
        //SetPixel(mDC,aX,aY,mPens[mCurrentPen].mColor.get());
        SetPixel(mDC,aX,aY,aColor.get());
      }

    //----------

    // The LineTo function draws a line from the current position up to, but not including, the specified point.

    virtual void drawLine(int aX1, int aY1, int aX2, int aY2)
      {
        setPos(aX1,aY1);
        LineTo(mDC,aX2,aY2);
        drawPoint(aX2,aY2);         // !!!
      }

    //----------

    virtual void drawRect(int aX1, int aY1, int aX2, int aY2)
      {
        setPos(aX1,aY1);
        LineTo(mDC,aX2,aY1);
        LineTo(mDC,aX2,aY2);
        LineTo(mDC,aX1,aY2);
        LineTo(mDC,aX1,aY1);
        //drawPoint(aX2,aY2);         // !!!
      }

    //----------

    // The FillRect function fills a rectangle by using the specified brush.
    // This function includes the left and top borders,
    // but excludes the right and bottom borders of the rectangle.

    virtual void fillRect(int aX1, int aY1, int aX2, int aY2)
      {
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2+1;           // !!!
        R.bottom  = aY2+1;           // !!!
        FillRect(mDC,&R,mBrush);
        //Rectangle(mDC,aX1,aY1,aX2,aY2);
      }

    //----------

    virtual void drawCircle(int aX1, int aY1, int aX2, int aY2)
      {
        //Ellipse(mDC, aX1,aY1,aX2,aY2 );
        Arc( mDC,aX1,aY1,aX2+1,aY2+1,0,0,0,0);
      }

    //----------

    // The Ellipse function draws an ellipse. The center of the ellipse is the
    // center of the specified bounding rectangle. The ellipse is outlined by
    // using the current pen and is filled by using the current brush.

    virtual void fillCircle(int aX1, int aY1, int aX2, int aY2)
      {
        clearPen();
        Ellipse( mDC,aX1,aY1,aX2,aY2);
        resetPen();
      }

    //----------


    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void drawArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( axAbs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          float a1 = aAngle1 -= 0.25;
          float a2 = a1 + aAngle2;
          if( aAngle2>0 )
          {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }
          float w = aX2-aX1;
          float h = aY2-aY1;
          float x = aX1 + w*0.5;
          float y = aY1 + h*0.5;
          float size = axMax(w,h);
          float x1 = x + axCosf(a1*PI2) * size;
          float y1 = y + axSinf(a1*PI2) * size;
          float x2 = x + axCosf(a2*PI2) * size;
          float y2 = y + axSinf(a2*PI2) * size;
          Arc(mDC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
        }
      }

    //----------

    // angle 1 = start angle, relative to 3 o'clock
    // angle 2 = 'distance' 0..1, counter-clockwise
    virtual void fillArc(int aX1, int aY1, int aX2, int aY2, float aAngle1, float aAngle2)
      {
        if( axAbs(aAngle2) >= 0.01/*EPSILON*/ )
        {
          clearPen();
          float a1 = aAngle1 -= 0.25;
          float a2 = a1 + aAngle2;
          if( aAngle2>0 )
          {
            float temp = a1;
            a1 = a2;
            a2 = temp;
          }
          float w = aX2-aX1;
          float h = aY2-aY1;
          float x = aX1 + w*0.5;
          float y = aY1 + h*0.5;
          float size = axMax(w,h);
          float x1 = x + axCosf(a1*PI2) * size;
          float y1 = y + axSinf(a1*PI2) * size;
          float x2 = x + axCosf(a2*PI2) * size;
          float y2 = y + axSinf(a2*PI2) * size;
          Pie(mDC,aX1,aY1,aX2,aY2,(int)x1,(int)y1,(int)x2,(int)y2);
          resetPen();
        }
      }

    //--------------------------------------------------
    // text
    //--------------------------------------------------

    virtual void setTextSize(int aSize)
      {

        //mFont = CreateFontIndirect(&LFont);
        //SelectObject(mDC,mFont);


        LOGFONT lfont;
        HFONT font = (HFONT)GetCurrentObject(mDC,OBJ_FONT);
        GetObject(font,sizeof(LOGFONT),&lfont);
        lfont.lfHeight = -MulDiv(aSize,GetDeviceCaps(mDC,LOGPIXELSY),72);

        if (mFont) DeleteObject(mFont);
        mFont = CreateFontIndirect(&lfont);
        SelectObject(mDC,mFont);
      }

    virtual int textWidth(axString aText)
      {
        SIZE S;
        GetTextExtentPoint32(mDC, aText.ptr(), aText.length(), &S);
        return S.cx;
      }

    //----------

    virtual int textHeight(axString aText)
      {
        SIZE S;
        GetTextExtentPoint32(mDC, aText.ptr(), aText.length(), &S);
        return S.cy;
      }

    //----------

    // hack alert!
    // draw to a rect size 1000x1000, and align to upper left

    virtual void drawText(int aX, int aY, axString aText)
      {
        //SetBkMode(mDC,TRANSPARENT);
        //TextOut(mDC,aX,aY,aText.ptr(), aText.length() );
        drawText( aX,aY, aX+1000,aY+1000, aText, ta_Left|ta_Top);
      }

    //--------

    virtual void drawText(int aX1, int aY1, int aX2, int aY2, axString aText, int aAlign)
      {
        SetBkMode(mDC,TRANSPARENT);
        R.left    = aX1;
        R.top     = aY1;
        R.right   = aX2;
        R.bottom  = aY2;
        int flags = 0;
        if (aAlign & ta_Left) flags = flags | DT_LEFT;
        else if (aAlign & ta_Right) flags = flags | DT_RIGHT;
        else flags = flags | DT_CENTER;
        if (aAlign & ta_Top) flags = flags | DT_TOP;
        else if (aAlign & ta_Bottom) flags = flags | DT_BOTTOM;
        else flags = flags | DT_VCENTER;
        HFONT oldfont = (HFONT)SelectObject(mDC, mFont);
        DrawText(mDC, aText.ptr(), -1, &R, flags | DT_NOCLIP | DT_SINGLELINE | DT_NOPREFIX);
        SelectObject(mDC, oldfont);
      }

    //--------------------------------------------------
    // bitmap
    //--------------------------------------------------

    //BOOL BitBlt(
    //  HDC hdcDest,  // Handle to the destination device context.
    //  int nXDest,   // logical x-coordinate of the upper-left corner of the destination rectangle.
    //  int nYDest,   // logical y-coordinate of the upper-left corner of the destination rectangle.
    //  int nWidth,   // logical width of the source and destination rectangles.
    //  int nHeight,  // logical height of the source and the destination rectangles.
    //  HDC hdcSrc,   // Handle to the source device context.
    //  int nXSrc,    // logical x-coordinate of the upper-left corner of the source rectangle.
    //  int nYSrc,    // logical y-coordinate of the upper-left corner of the source rectangle.
    //  DWORD dwRop   // raster-operation code.
    //);

    virtual void drawBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        HDC tempdc = CreateCompatibleDC(mDC);
        SelectObject(tempdc,(HBITMAP)aBitmap->getHandle());
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
        DeleteDC(tempdc);
      }

    //--------------------------------------------------
    // image
    //--------------------------------------------------

    //virtual void drawImage(axImage* aImage, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void drawSurface(axSurfaceBase* aSurface, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HDC)aImage->getHandle();
        HDC tempdc = (HDC)aSurface->getHandle();
        BitBlt(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,SRCCOPY);
      }

    //----------

    //virtual void renderBitmap(axBitmap* aBitmap, int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    //  {
    //    drawBitmap(aBitmap,aX,aY,aSrcX,aSrcY,aSrcW,aSrcH);
    //  }

    //typedef struct _BLENDFUNCTION {
    //  BYTE BlendOp;
    //  BYTE BlendFlags;
    //  BYTE SourceConstantAlpha;
    //  BYTE AlphaFormat;
    //} BLENDFUNCTION, *PBLENDFUNCTION, *LPBLENDFUNCTION;

    //virtual void renderImage( axImage*  aImage,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void renderSurface( axSurfaceBase* aSurface,  int aX, int aY, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HDC)aImage->getHandle();
        HDC tempdc = (HDC)aSurface->getHandle();
        AlphaBlend(mDC,aX,aY,aSrcW,aSrcH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,mBlendFunc);
        // link with: libmsimg32
      }

    //BOOL StretchBlt(
    //  __in  HDC hdcDest,
    //  __in  int nXOriginDest,
    //  __in  int nYOriginDest,
    //  __in  int nWidthDest,
    //  __in  int nHeightDest,
    //  __in  HDC hdcSrc,
    //  __in  int nXOriginSrc,
    //  __in  int nYOriginSrc,
    //  __in  int nWidthSrc,
    //  __in  int nHeightSrc,
    //  __in  DWORD dwRop
    //);

    //virtual void stretchImage( axImage*  aImage,  int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
    virtual void stretchSurface( axSurfaceBase* aSurface, int aX, int aY, int aW, int aH, int aSrcX, int aSrcY, int aSrcW, int aSrcH)
      {
        //HDC tempdc = (HDC)aImage->getHandle();
        HDC tempdc = (HDC)aSurface->getHandle();
        AlphaBlend(mDC,aX,aY,aW,aH,tempdc,aSrcX,aSrcY,aSrcW,aSrcH,mBlendFunc);
        // link with: libmsimg32
      }

};

typedef axCanvasWin32 axCanvasImpl;

//----------------------------------------------------------------------
#endif
