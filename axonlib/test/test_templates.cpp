//#define AX_NOGUI

//#include <stdio.h> // printf
#include "core/axDefines.h"
#include "core/axDebug.h"
#include "base/axFormat.h"
#include "wdg/wdgPanel.h"

//----------

class myDescriptor : public axDescriptor
{
  public:
    myDescriptor(axFormat* aFormat)
    : axDescriptor(aFormat)
      {
        trace("myDescriptor");
      }
    virtual char* getName(void)   { return (char*)"test_template"; }
    virtual char* getAuthor(void) { return (char*)"ccernn"; }
};

//----------

class myInstance : public axInstance
{
  private:
    axFormat* mFormat;
  public:
    myInstance(axFormat* aFormat)
    : axInstance(aFormat)
      {
        trace("myInstance.constructor");
        mFormat = aFormat;
      }
    int main(int argc,char** argv)
      {
        trace("myInstance.main");
        #ifndef AX_NOGUI
        axInterface* iface;
        iface = mFormat->getInterface();
        axWindow* win = iface->createWindow(NULL,320,240);

          win->appendWidget( new wdgPanel(win,axRect(0,0,100,100),wa_None) );
          win->doRealign();

        win->show();
        win->eventLoop();
        win->hide();
        delete win;
        #endif
        return 0;
      }
};

//----------

//class myInterface : public axInterface
//{
//  public:
//    myInterface(axFormat* aFormat)
//    : axInterface(aFormat)
//      {
//        trace("myInterface");
//        axWindow* win = createWindow(NULL,320,240);
//        win->show();
//        win->setTitle("qweqwe");
//        win->eventLoop();
//        win->hide();
//        delete win;
//      }
//};

//----------

//AX_MAIN(DEFAULT_DESCRIPTOR,myInstance)
//AX_ENTRYPOINT(myDescriptor,myInstance,axInterface,axPlatform)

AX_ENTRYPOINT( myDescriptor,
               myInstance,
               DEFAULT_INTERFACE,
               DEFAULT_PLATFORM )
