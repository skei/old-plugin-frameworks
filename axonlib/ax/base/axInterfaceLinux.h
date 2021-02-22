#ifndef axInterfaceLinux_included
#define axInterfaceLinux_included
//----------------------------------------------------------------------

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "gui/axEditor.h"

//----------------------------------------------------------------------

class axInterfaceLinux : public axInterface
{
  protected:
    axBase*   mBase;
  private:
    Display*  mDisplay;
    Window    mRootWindow;

  public:

    axInterfaceLinux(axBase* aBase)// : axInterface(aBase)
      {
        //trace("axInterfaceLinux.constructor");
        mBase = aBase;
        XInitThreads();
        mDisplay = XOpenDisplay(NULL);              // må denne være unik for hver instance? i så fall, fløuytt til createWindow
        mRootWindow = XDefaultRootWindow(mDisplay);
      }

    virtual ~axInterfaceLinux()
      {
        //trace("axInterfaceLinux.destructor");
        XCloseDisplay(mDisplay);
      }

    virtual void* getHandle(void)
      {
        return (void*)mDisplay;
      }

    virtual void* createEditor(void* aParent, axRect aRect, int aFlags)
      {
        axEditor* editor;
        void* parent = aParent;
        if (aParent) editor = new axEditor(mBase,aParent,aRect,aFlags);
        else editor = new axEditor(mBase,&mRootWindow,aRect,aFlags);
        return (void*)editor;
      }

};

//----------

typedef axInterfaceLinux AX_INTERFACE;

//----------------------------------------------------------------------
#endif
