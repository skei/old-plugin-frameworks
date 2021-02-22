#ifndef axBase_included
#define axBase_included
//----------------------------------------------------------------------

#include "axConfig.h"
#include "core/axDefines.h"
#include "core/axDebug.h"
#include "core/axRect.h"
#include "par/axParameter.h"

//#include "core/axMalloc.h"
//#include "core/axStdlib.h"
//#include "core/axRand.h"
//#include "core/axUtils.h"

//----------------------------------------------------------------------

class axPlatform;
class axInterface;
class axFormat;
class axDescriptor;
class axInstance;

//----------------------------------------------------------------------

class axBase
{
  public:
    //axBase() {}
    virtual ~axBase() {}
    virtual axPlatform*   getPlatform(void)     { return NULL; }
    virtual axInterface*  getInterface(void)    { return NULL; }
    virtual axFormat*     getFormat(void)       { return NULL; }
    virtual axDescriptor* getDescriptor(void)   { return NULL; }
    virtual axInstance*   createInstance(void)  { return NULL; }
};

//----------------------------------------------------------------------
// descriptor & instance
//----------------------------------------------------------------------

#include "base/axDescriptor.h"
#include "base/axInstance.h"

//----------------------------------------------------------------------
// cross-*
//----------------------------------------------------------------------

// Win32: DllMain, HINSTANCE
// Linux:

class axPlatform
{
  public:
    //axPlatform(axBase* aBase) { /*trace("axPlatform.constructor");*/ }
    virtual ~axPlatform()     { /*trace("axPlatform.destructor");*/ }
    virtual void* getHandle(void) { return NULL; }  // win32: hinstance
    virtual char* getPlatformName(void) { return (char*)""; }
    virtual char* getPath(void) { return (char*)""; }
};

//----------------------------------------------------------------------

// Linux: Display*
// Win32: register class

class axInterface
{
  public:
    //axInterface(axBase* aBase)  { /*trace("axInterface.constructor");*/ }
    virtual ~axInterface()      { /*trace("axInterface.destructor");*/ }
    virtual void* getHandle(void) { return NULL; }    // linux: display*
    virtual char* getName(void) { return (char*)""; } // win32: window class name
    virtual void* createEditor(void* aParent, axRect aRect, int aFlags) { return NULL; }
};

//----------------------------------------------------------------------

// entrypoint:
// Vst:    ptr = audioMasterCallback, return AEffect
// Ladspa: ptr = ?, return = LADSPA_Descriptor
// Exe:    ptr = portaudio/rtaudio/libaam/.., return ?

class axFormat
{
  public:
    //axFormat(axBase* aBase) { /*trace("axFormat.constructor");*/ }
    virtual ~axFormat()     { /*trace("axFormat.destructor");*/ }
    virtual void* entrypoint(void* ptr) { return NULL; }
    virtual char* getFormatName(void) { return (char*)""; }
    virtual void* getHostPtr(void) { return NULL; } // vst: audioMasterCallback
    virtual void* getUserPtr(void) { return NULL; } // vst: aeffect
};

//----------------------------------------------------------------------
// base implementation
//----------------------------------------------------------------------

//  PL - Platform
//  IF - Interface
//  FO - Format
//
//  D  - Descriptor
//  I  - Instance

template<class _PL, class _IF, class _FO, class _D, class _I>
class axBaseImpl : public axBase
{
  private:
    axPlatform*     mPlatform;
    axInterface*    mInterface;
    axFormat*       mFormat;
    axDescriptor*   mDescriptor;
  public:
    axBaseImpl()
      {
        //trace("axBaseImpl.constructor");
        mPlatform     = new _PL(this);
        mInterface    = new _IF(this);
        mDescriptor   = new _D(this);   // descriptor must be created before format, so format can get info
        mFormat       = new _FO(this);
        //printf("%i\n", sizeof(_PL) + sizeof(_IF) + sizeof(_D) + sizeof(_FO) );
        //fflush(stdout);
      }
    virtual ~axBaseImpl()
      {
        //trace("axBaseImpl.destructor");
        delete mFormat;
        delete mDescriptor;
        delete mInterface;
        delete mPlatform;
      }
  //protected:
  public:
    virtual axPlatform*   getPlatform(void)     { return mPlatform; }
    virtual axInterface*  getInterface(void)    { return mInterface; }
    virtual axFormat*     getFormat(void)       { return mFormat; }
    virtual axDescriptor* getDescriptor(void)   { return mDescriptor; }
    virtual axInstance*   createInstance(void)  { return new _I(this); }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// singleton???
class axGlobalScope
{
  private:
    axBase*     mBase;
    void*       mPtr;
  public:
    axGlobalScope()   { /*printf("create base\n");*/ mBase=NULL; mPtr=NULL; }
    ~axGlobalScope()  { /*printf("delete base\n");*/ if (mBase) delete mBase; }
    inline void     setBase(axBase* aBase)  { mBase=aBase; }
    inline void     setPtr(void* aPtr)      { mPtr=aPtr; }
    inline axBase*  getBase(void)           { return mBase; }
    inline void*    getPtr(void)            { return mPtr; }
};

//----------

static axGlobalScope gGlobalScope;

//----------------------------------------------------------------------

#include "base/axPlatform.h"
#include "base/axInterface.h"
#include "base/axFormat.h"

//----------------------------------------------------------------------
#endif


// win32: HWND   = pvoid
// linux: Window = unsigned long

//TODO: reparent (axWindowWin32/Linux)
//      use void* as parent, not int (32/64 bit safety)

/*

TODO: go through it again, after latest changes, note down the full
exec order of things....

- axGlobalScope
  - base = NULL
  - ptr = NULL
- DllMain [win32]
  - gWinInstance
- AX_ENTRYPOINT
  - create base <template classes>
    - create platform
      - GetModuleFileName/dladdr (for axGetBasePath)
      - create debug
        - global.setPtr(debug)
    - create interface
    - create descriptor
    - create format
  - global.setBase(base)
  - format.entrypoiunt
    - create instance [vst]

*/
