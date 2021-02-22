#ifndef axInterfaceWin32_included
#define axInterfaceWin32_included
//----------------------------------------------------------------------

#include "core/axStdlib.h"
//#include "gui/axWindow.h"
#include "gui/axEditor.h"

//----------------------------------------------------------------------

// axonlib_0.1.2_window

LRESULT CALLBACK eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  axWindowWin32* wnd = (axWindowWin32*)GetWindowLong(hWnd,GWL_USERDATA);
	if (wnd==0) return DefWindowProc(hWnd,message,wParam,lParam);
  return wnd->eventHandler(hWnd, message, wParam, lParam);
}

//----------------------------------------------------------------------

class axInterfaceWin32 : public axInterface
{
  protected:
    axBase*   mBase;
  private:
    HINSTANCE mWinInstance;
    char      mWinClassName[256];
    HCURSOR   mWinCursor;
    HICON     mWinIcon;
    WNDCLASS  mWinClass;

  public:

    axInterfaceWin32(axBase* aBase)// : axInterface(aBase)
      {
        //trace("axInterfaceWin32.constructor");
        mBase = aBase;
        mWinInstance = (HINSTANCE)aBase->getPlatform()->getHandle();
        axStrcpy( mWinClassName, "axonlib_" );
        axStrcat( mWinClassName, (char*)AX_VERSION_STRING );
        axStrcat( mWinClassName, "_window" );
        mWinIcon = LoadIcon(mWinInstance, "axicon");
        mWinCursor = NULL; // LoadCursor(NULL, IDC_ARROW);
        axMemset(&mWinClass,0,sizeof(mWinClass));
        mWinClass.style         = CS_HREDRAW | CS_VREDRAW;
        mWinClass.lpfnWndProc   = &eventProc;
        mWinClass.hInstance     = mWinInstance;
        mWinClass.lpszClassName = mWinClassName;
        mWinClass.hCursor       = mWinCursor;
        mWinClass.hIcon         = mWinIcon;
        RegisterClass(&mWinClass);
      }

    //----------

    virtual ~axInterfaceWin32()
      {
        //trace("axInterfaceWin32.destructor");
        UnregisterClass(mWinClassName,mWinInstance);
      }

    //--------------------------------------------------

    //virtual void* getHandle(void)
    //  {
    //    return NULL
    //  }

    //----------

    virtual char* getName(void)
      {
        return mWinClassName;
      }

    //----------

    virtual void* createEditor(void* aParent, axRect aRect, int aFlags)
      {
        //trace("axInterfaceWin32.createWindow");
        axEditor* editor;
        if (aParent) editor = new axEditor(mBase,*(void**)aParent,aRect,aFlags);  // !!!!!
        else editor = new axEditor(mBase,NULL,aRect,aFlags);
        return (void*)editor;
      }

};

//----------

typedef axInterfaceWin32 AX_INTERFACE;

//----------------------------------------------------------------------
#endif
