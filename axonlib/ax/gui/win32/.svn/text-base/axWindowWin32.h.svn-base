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

#ifndef axWindowWin32_included
#define axWindowWin32_included
//----------------------------------------------------------------------

//#include <windows.h>

//----------------------------------------------------------------------

//LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//----------------------------------------------------------------------

// mouse cursor shapes
// we need an integer with max length here - ax_intptr
#define cu_Arrow           ((ax_intptr)IDC_ARROW)
#define cu_ArrowUp         ((ax_intptr)IDC_UPARROW)
#define cu_ArrowDown       cu_Arrow
#define cu_ArrowLeft       cu_Arrow
#define cu_ArrowRight      cu_Arrow
#define cu_ArrowUpDown     ((ax_intptr)IDC_SIZENS)
#define cu_ArrowLeftRight  ((ax_intptr)IDC_SIZEWE)
#define cu_ArrowDiagLeft   ((ax_intptr)IDC_SIZENESW)
#define cu_ArrowDiagRight  ((ax_intptr)IDC_SIZENWSE)
#define cu_Move            ((ax_intptr)IDC_SIZEALL)
#define cu_Wait            ((ax_intptr)IDC_WAIT)
#define cu_ArrowWait       ((ax_intptr)IDC_APPSTARTING)
#define cu_Hand            ((ax_intptr)IDC_HAND)
//#define cu_Finger          cu_Arrow
#define cu_Finger          ((ax_intptr)IDC_HAND)
#define cu_Cross           ((ax_intptr)IDC_CROSS)
#define cu_Pencil          cu_Arrow
#define cu_Plus            cu_Arrow
#define cu_Question        cu_Arrow
#define cu_Ibeam           cu_Arrow


                      
#define DEF_CURSOR    (cu_Arrow)

// #DEFINE idc_arrow       (32512)
// #DEFINE idc_ibeam       (32513)
// #DEFINE idc_wait        (32514)
// #DEFINE idc_cross       (32515)
// #DEFINE idc_uparrow     (32516)
// #DEFINE idc_size        (32640)
// #DEFINE idc_icon        (32641)
// #DEFINE idc_sizenwse    (32642)
// #DEFINE idc_sizenesw    (32643)
// #DEFINE idc_sizewe      (32644)
// #DEFINE idc_sizens      (32645)


//----------------------------------------------------------------------

class axWindowWin32 : public axWindowBase
{
  private:
    axBase*     mBase;
    HINSTANCE   mInstance;
    HWND        mWindow;
    PAINTSTRUCT mPS;
    HCURSOR     mWinCursor;
    int         mPrevCursor;
    int         mClickedButton;
    HWND        mParent; // hwnd =pvoid
    int         mAdjustWidth, mAdjustHeight;

  public:
    axWindowWin32(axBase* aBase, void* aParent, axRect aRect, int aWinFlags)
    : axWindowBase(aBase,aParent,aRect,aWinFlags)
      {
        //trace("axWindowWin32.constructor()");
        mBase = aBase;
        mInstance = (HINSTANCE)aBase->getInterface()->getHandle();
        mWinCursor  = LoadCursor(NULL,IDC_ARROW);
        mPrevCursor = 0;
        mClickedButton = bu_None;
        RECT rc = { mRect.x, mRect.y, mRect.x2(), mRect.y2() }; // left, top, right, bottom

        // --- create window ---

        if (mWinFlags&AX_WIN_EMBEDDED) // embedded ---
        {
          AdjustWindowRectEx(&rc,/*WS_OVERLAPPEDWINDOW|*/WS_POPUP,FALSE,WS_EX_TOOLWINDOW);
          //trace("adjusted rc (embedded): " << rc.left << "," << rc.top << " : " << rc.right << "," << rc.bottom);
          mWindow = CreateWindowEx(
            WS_EX_TOOLWINDOW,
            mBase->getInterface()->getName(), //   classname,
            0,
            WS_POPUP,
            rc.left,//wPosX,          // center x
            rc.top,//wPosY,           // center y
            rc.right-rc.left+1,       //wWidth,
            rc.bottom-rc.top+1,       //wHeight,
            0,                        //(HWND)mParent,//0,
            0,
            mInstance,
            0
          );
          reparent(/*(int)*/aParent);   // !!! int is not 64-bit safe, i guess... use void*
        } //embedded

        else // windowed ---
        {
          AdjustWindowRectEx(&rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
          //trace("adjusted rc (windowed): " << rc.left << "," << rc.top << " : " << rc.right << "," << rc.bottom);
          const unsigned int wPosX =
            ((GetSystemMetrics(SM_CXSCREEN)-mRect.w)>>1) + rc.left;
          const unsigned int wPosY =
            ((GetSystemMetrics(SM_CYSCREEN)-mRect.h)>>1) + rc.top;
          mWindow = CreateWindowEx(
            WS_EX_OVERLAPPEDWINDOW,   // dwExStyle
            mBase->getInterface()->getName(), //   classname,               // lpClassName
            0,                        // lpWindowName
            WS_OVERLAPPEDWINDOW,      // dwStyle
            wPosX,                    // center x
            wPosY,                    // center y
            rc.right-rc.left+1,       // wWidth,
            rc.bottom-rc.top+1,       // wHeight,
            0,                        // hWndParent
            0,                        // hMenu
            mInstance,                // hInstance
            0                         // lpParam
          );
          SetFocus(mWindow);
        }

        mAdjustWidth = (rc.right - rc.left + 1) - mRect.w;
        mAdjustHeight = (rc.bottom - rc.top + 1) - mRect.h;

        // ---
        // vista / gcc / x64:
        // GWPL_USERDATA undefined (deprecated)
                
        #ifndef GWL_USERDATA
          #define GWL_USERDATA -21
        #endif 
        SetWindowLong(mWindow,GWL_USERDATA,(LONG_PTR)this);
        
        //DragAcceptFiles(mWindow,true);
        mCanvas = createCanvas();
        if (aWinFlags & AX_WIN_BUFFERED) mSurface = createSurface(mRect.w,mRect.h,32);

      }

    //----------------------------------------

    ~axWindowWin32()
      {
        DestroyWindow(mWindow);
        if (mCanvas) delete mCanvas;
        if (mSurface) delete mSurface;
      }

    //----------------------------------------

    //virtual int getHandle(void) { return (int)mWindow; }

    virtual axCanvas* createCanvas(void)
      {
        return new axCanvas(mBase,mWindow);
      }

    //----------

    virtual axSurface* createSurface(int aWidth, int aHeight, int aDepth)
      {
        return new axSurface(mBase,mWindow,aWidth,aHeight, aDepth);
      }

    //----------

    virtual axBitmap* createBitmap(int aWidth, int aHeight, int aDepth)
      {
        return new axBitmap(mBase,aWidth,aHeight,aDepth);
      }


    //----------------------------------------
    // low level
    //----------------------------------------

    //virtual void flush(void)
    //  {
    //    //GdiFlush();
    //  }

    //----------

    //virtual void sync(void)
    //  {
    //  }

    //----------

    //virtual void lock(void)
    //  {
    //  }

    //----------

    //virtual void unlock(void)
    //  {
    //  }

    //----------

    virtual void show(void)
      {
        //trace("show");
        //SetWindowPos(mWindow,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
        ShowWindow(mWindow,SW_SHOW);
      }

    //----------

    virtual void hide(void)
      {
        //trace("hide");
        ShowWindow(mWindow,SW_HIDE);
      }

    //----------

    virtual void setPos(int aX, int aY)
      {
        SetWindowPos(mWindow,0,aX,aY,0,0,SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
      }

    //----------

    virtual void setSize(int aWidth, int aHeight)
      {
        //trace("setSize: " << aWidth << "," << aHeight);
        int w = aWidth + mAdjustWidth + 0;
        int h = aHeight + mAdjustHeight + 0;
        SetWindowPos(mWindow,HWND_TOP,0,0,w,h, SWP_NOMOVE);
        //SetWindowPos(mWindow,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
      }

    //------------------------------

      //internal
      void getWindowSize(HWND pWnd, int* pW, int* pH)
      {
          if (pWnd)
          {
            RECT r;
            GetWindowRect(pWnd, &r);
            *pW = r.right - r.left;
            *pH = r.bottom - r.top;
          }
          else
          {
            *pW = *pH = 0;
          }
        }

      //----------

      //internal
      bool isChildWindow(HWND pWnd)
        {
          if (pWnd)
          {
            int style = GetWindowLong(pWnd, GWL_STYLE);
            int exStyle = GetWindowLong(pWnd, GWL_EXSTYLE);
            return ((style & WS_CHILD) && !(exStyle & WS_EX_MDICHILD));
          }
          return false;
        }

    //------------------------------

    #define SETPOS_FLAGS SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE

    virtual void setParentSize(int aWidth, int aHeight)
      {
        int dw = aWidth - mRect.w, dh = aHeight - mRect.h;
        HWND pParent = 0, pGrandparent = 0;
        int w=0, h=0, parentW=0, parentH=0, grandparentW=0, grandparentH=0;
        getWindowSize(mWindow,&w,&h);
        if (isChildWindow(mWindow))
        {
          pParent = GetParent(mWindow);
          getWindowSize(pParent, &parentW, &parentH);
          if (isChildWindow(pParent))
          {
            pGrandparent = GetParent(pParent);
            getWindowSize(pGrandparent, &grandparentW, &grandparentH);
          }
        }
        SetWindowPos(mWindow, 0, 0, 0, w + dw, h + dh, SETPOS_FLAGS);
        if (pParent)
        {
          SetWindowPos(pParent, 0, 0, 0, parentW + dw, parentH + dh, SETPOS_FLAGS);
        }
        if (pGrandparent)
        {
          SetWindowPos(pGrandparent, 0, 0, 0, grandparentW + dw, grandparentH + dh, SETPOS_FLAGS);
        }
      }

    #undef SETPOS_FLAGS

    //----------

    virtual void setTitle(axString aTitle)
      {
        SetWindowText(mWindow, aTitle.ptr());
      }

    //----------

    //virtual void reparent(int aParent)
    virtual void reparent(void* aParent)
      {
        //trace("reparent");
        mParent = (HWND)aParent; // !!!
        SetWindowLong(mWindow,GWL_STYLE,(GetWindowLong(mWindow,GWL_STYLE)&~WS_POPUP)|WS_CHILD);
        SetParent(mWindow,(HWND)aParent);
      }

    //virtual void resetCursor(void)
    //  {
    //  }

    //----------

    virtual void setCursor(int aCursor)
      {
        if (aCursor<0)
        {
          hideCursor();// aCursor = DEF_CURSOR;
          mPrevCursor = aCursor;
        } //-1
        else
        {
          if (mPrevCursor<0) showCursor();
          if (aCursor!=mPrevCursor)
          {
            mWinCursor = LoadCursor(NULL,(char*)aCursor);
            SetCursor( mWinCursor );
            mPrevCursor = aCursor;
          }
        } //>0
      }

    //----------

    virtual void setCursorPos(int aX, int aY)
      {
        SetCursorPos(aX,aY);
      }

    //----------

    virtual void showCursor(void)
      {
        ShowCursor(true);
      }

    //----------

    virtual void hideCursor(void)
      {
        ShowCursor(false);
      }

    //----------

    virtual void grabCursor(void)
      {
        SetCapture(mWindow);
      }

    //----------

    virtual void releaseCursor(void)
      {
        ReleaseCapture();
      }

    //----------------------------------------
    // medium level
    //----------------------------------------

    // The invalidated areas accumulate in the update region until the region is processed when the next
    // WM_PAINT message occurs or until the region is validated by using the ValidateRect or ValidateRgn function.
    // The system sends a WM_PAINT message to a window whenever its update region is not empty
    // and there are no other messages in the application queue for that window.

    virtual void invalidate(int aX, int aY, int aW, int aH)
      {
        RECT R;
        R.left   = aX;
        R.top    = aY;
        R.right  = aX + aW - 1;
        R.bottom = aY + aH - 1;
        InvalidateRect(mWindow,&R,false);
        //UpdateWindow(mWindow);
      }

    //----------

    virtual void resizeBuffer(int aWidth, int aHeight)
      {
        //trace("axWindowWin32.resizeBuffer: " << aWidth << "," << aHeight);
        //if( aWidth!=mRect.w || aHeight!=mRect.h )
        //{
          if (mWinFlags&AX_WIN_BUFFERED)
          {
            //mSurfaceMutex.lock();
            axSurface* srf;
            if (mSurface)
            {
              srf = mSurface;
              mSurface = NULL;
              delete srf;
            }
            //srf = new axSurface(aWidth,aHeight/*,mWinFlags*/);
            srf = createSurface(aWidth,aHeight,32);
            mSurface = srf;
            //mSurfaceMutex.unlock();
          }
          //mRect.w = aWidth;
          //mRect.h = aHeight;
          //doSetSize(aWidth,aHeight);
        //} //newsize
      }

    //----------

    // The BeginPaint function prepares the specified window for painting
    // and fills a PAINTSTRUCT structure with information about the painting
    //
    //
    // The BeginPaint function automatically sets the clipping region of the device context
    // to exclude any area outside the update region. The update region is set by the InvalidateRect
    // or InvalidateRgn function and by the system after sizing, moving, creating, scrolling,
    // or any other operation that affects the client area. If the update region is marked for erasing,
    // BeginPaint sends a WM_ERASEBKGND message to the window.
    //
    // An application should not call BeginPaint except in response to a WM_PAINT message

    virtual void beginPaint(void)
      {
        //wtrace("axWindowWin32.beginPaint");
        /*mPaintDC = */BeginPaint(mWindow,&mPS);
      }

    //----------

    virtual void endPaint(void)
      {
        EndPaint(mWindow,&mPS);
        //wtrace("axWindowWin32.endPaint");
      }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void startTimer(int ms)
      {
        /*mTimer = */SetTimer(mWindow,667,ms,NULL/*timerProc*/);
      }

    //----------

    virtual void stopTimer(void)
      {
        KillTimer(mWindow,667);
      }

    //----------------------------------------
    // events
    //----------------------------------------

    virtual void sendEvent(unsigned int aValue=0)
    {
    }

    //----------------------------------------
    //
    //----------------------------------------

    virtual void eventLoop(void)
      {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }

    //----------------------------------------
    //
    //----------------------------------------

//#define bu_None    0
//#define bu_Left    1
//#define bu_Right   2
//#define bu_Middle  4
//#define bu_Shift   8
//#define bu_Ctrl    16
//#define bu_Alt     32

    int remapButton(int aButton)
      {
        return aButton;
      }

    //----------

    int remapKey(int aKey)
      {
        int ret = bu_None;
        if (aKey & MK_SHIFT)        ret |= bu_Shift;
        if (aKey & MK_CONTROL)      ret |= bu_Ctrl;
        if (GetKeyState(VK_MENU)<0) ret |= bu_Alt;
        return ret;
      }

      //------------------------------

    LRESULT eventHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
      {
        //MSG msg2;
        //HDC dc;
        LRESULT result = 0;
        int btn = 0;
        axRect rc;
        int w,h;
        int left,top,right,bottom;

        //TRACE("win32 eventHandler. msg=%x\n",message);

        switch (message)
        {
          case WM_GETDLGCODE:
            result = DLGC_WANTALLKEYS;
            break;
          case WM_PAINT:
            //wtrace("WM_PAINT\n");
            beginPaint();
            left   = mPS.rcPaint.left;
            top    = mPS.rcPaint.top;
            right  = mPS.rcPaint.right;
            bottom = mPS.rcPaint.bottom;
            //trace("WM_PAINT. RECT: " << left << "," << top << " : " << right << "," << bottom);
            rc = axRect(  mPS.rcPaint.left,
                          mPS.rcPaint.top,
                          mPS.rcPaint.right -  mPS.rcPaint.left + 2,
                          mPS.rcPaint.bottom - mPS.rcPaint.top  + 2);
            //mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
            if ((mWinFlags&AX_WIN_BUFFERED) && mSurface )
            {
              //mSurfaceMutex.lock();
              axCanvas* can = mSurface->getCanvas();
              can->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(can,rc);
              //mCanvas->drawImage(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              mCanvas->drawSurface(mSurface,rc.x,rc.y,rc.x,rc.y,rc.w,rc.h);
              can->clearClipRect();
              //mSurfaceMutex.unlock();
            }
            else
            {
              // http://msdn.microsoft.com/en-us/library/dd183439%28VS.85%29.aspx
              // if you obtain a device context handle from the  BeginPaint function,
              // the DC contains a predefined rectangular clipping region that corresponds
              // to the invalid rectangle that requires repainting
              mCanvas->setClipRect(rc.x,rc.y,rc.x2(),rc.y2());
              doPaint(mCanvas,rc);
              mCanvas->clearClipRect();
            }
            //mCanvas->clearClipRect();
            endPaint();
            break;
          case WM_LBUTTONDOWN: case WM_RBUTTONDOWN: case WM_MBUTTONDOWN:
            switch (message)
            {
              case WM_LBUTTONDOWN: btn = bu_Left;   break;
              case WM_MBUTTONDOWN: btn = bu_Middle; break;
              case WM_RBUTTONDOWN: btn = bu_Right;  break;
            }
            mClickedButton = btn;
            doMouseDown(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
            if (mCapturedWidget) grabCursor();
            break;
          case WM_LBUTTONUP: case WM_RBUTTONUP: case WM_MBUTTONUP:
            switch (message)
            {
              case WM_LBUTTONUP: btn = bu_Left;   break;
              case WM_MBUTTONUP: btn = bu_Middle; break;
              case WM_RBUTTONUP: btn = bu_Right;  break;
            }
            mClickedButton = bu_None;
            doMouseUp(short(LOWORD(lParam)), short(HIWORD(lParam)), btn | remapKey(wParam));
            if (!mCapturedWidget) releaseCursor();
            break;
          case WM_MOUSEMOVE:
            doMouseMove(short(LOWORD(lParam)), short(HIWORD(lParam)), mClickedButton | remapKey(wParam));
            //return 1;
            break;
          //case WM_CHAR:
          //  doKeyChar(wParam,lParam);
          //  break;
          case WM_KEYDOWN:
            doKeyDown(wParam,lParam);
            break;
          case WM_KEYUP:
            doKeyDown(wParam,lParam);
            break;
          case WM_SIZE:
            //trace("WM_SIZE\n");
            //lParam:
            //  The low-order word of lParam specifies the new width of the client area.
            //  The high-order word of lParam specifies the new height of the client area.
            //TODO: resize surface, if any
            //int x = ev->xconfigure.x;
            //int y = ev->xconfigure.y;
            w = short(LOWORD(lParam));
            h = short(HIWORD(lParam));
            //trace("WM_SIZE: " << w << "," << h);

            //if (!(mWinFlags&AX_WIN_EMBEDDED))
            //{
            //  w += mAdjustWidth;
            //  h += mAdjustHeight;
            //}

            //if (w!=mRect.w || h!=mRect.h)
            //{
            //// hack: ignore this if there is other WM_SIZE messages in the queue
            //if ( PeekMessage(&msg2,mWindow,WM_SIZE,WM_SIZE,PM_NOREMOVE) )
            //{
            //  TRACE("there are oher WN_SIZE messages in the queue, so we're ignoring this one\n");
            //}
            //else
            //{
              //flush();
              resizeBuffer(w,h);
              doSetSize(w,h);
            //}
            //}
            result = 0;
            break;
          case WM_DESTROY:
            if (!(mWinFlags&AX_WIN_EMBEDDED))
            {
              //trace("Quit");
              /*
              note:
                one issue here is that, there is no know way to pause the
                console for a while before closing. this way the windows user
                could see the last bit of messages such as free() and other
                'exit' related ones.
                a log file debug method might be needed on windows after all..
              */
              PostQuitMessage(0);
            }
            break;
          case WM_TIMER:
            //wtrace("timer " << wParam);
            if (wParam==667)
            {
              //wtrace("axWIndowWin32.eventHandler :: timer");
              doTimer();
            }
            result = 0;
            break;
          case WM_SETCURSOR:
            //wtrace("WM_SETCURSOR");
            if (LOWORD(lParam)==HTCLIENT)
            {
              SetCursor(mWinCursor);
              result = 1;
            }
            else result = DefWindowProc(hWnd,message,wParam,lParam);
            break;
          default:
            result = DefWindowProc(hWnd,message,wParam,lParam);
        }
        return result;
      }

};

//----------------------------------------------------------------------
//
// eventProc
//
//----------------------------------------------------------------------

//LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//  axWindowWin32* wnd = (axWindowWin32*)GetWindowLong(hWnd,GWL_USERDATA);
//	if (wnd==0) return DefWindowProc(hWnd,message,wParam,lParam);
//  return wnd->eventHandler(hWnd, message, wParam, lParam);
//}

//----------------------------------------------------------------------

typedef axWindowWin32 axWindowImpl;

//----------------------------------------------------------------------
#endif
