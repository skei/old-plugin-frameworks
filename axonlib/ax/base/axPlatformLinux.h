#ifndef axPlatformLinux_included
#define axPlatformLinux_included
//----------------------------------------------------------------------

class axPlatformLinux : public axPlatform
{
  protected:
    axBase* mBase;
  private:    

  public:
    axPlatformLinux(axBase* aBase)// : axPlatform(aBase)
      {
        /*trace("axPlatformLinux.constructor");*/
        mBase = aBase;        
      }
    //virtual ~axPlatformLinux() { /*trace("axPlatformLinux.destructor");*/ }
    virtual char* getPlatformName(void) { return (char*)"linux"; }    
};

typedef axPlatformLinux AX_PLATFORM;

//----------------------------------------------------------------------

#include "stdio.h"

__externc void _ax_init(void) __constructor;
__externc void _ax_init(void)
{
  printf("_ax_init()\n");
}

__externc void _ax_fini(void) __destructor;
__externc void _ax_fini(void)
{
  printf("_ax_fini()\n");
}

//----------------------------------------------------------------------
#endif
