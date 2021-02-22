//#if 0

#define AX_WIN32
#define AX_FORMAT_VST
#define AX_NOGUI

//#include "core/axDefines.h"
//#include "core/axDebug.h"

#include <stdio.h> // printf

#include "axFormat.h"

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

// global/static names for parameters, inputs & outputs
// these could be set up programmatically, but this is easier..

static char* g_params[]  = { (char*)"gain" };

//----------

/*

  this class is created once, when the main entrypoint function is called,
  after the host loads the dll/so.
  handles things that the host might want to know about the plugin before
  any instances is created. the plugin. name, number of parameters, etc..

*/

class myDescriptor : public axDescriptor
{
  public:
    //myDescriptor(axPlatform* aPlatform) : axDescriptor(aPlatform) {}
    myDescriptor(axFormat* aFormat) : axDescriptor(aFormat) {}
    virtual char*         getName(void)             { return (char*)"gain"; }
    virtual char*         getAuthor(void)           { return (char*)"ccernn"; }
    virtual char*         getProduct(void)          { return (char*)"axonlib example plugin"; }
    virtual int           getNumParams(void)        { return 1; }
    virtual char*         getParamName(int aIndex)  { return g_params[aIndex]; }
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

/*

  the actual plugin (instance).
  quite similar to what was our main plugin class, except that the plugin
  info/description (enumeration info) has moved to axDescriptor.

*/

class myInstance : public axInstance
{
  public:

    //myInstance(axDescriptor* aDescriptor)
    myInstance(axFormat* aFormat)
    //: axInstance(aDescriptor)
    : axInstance(aFormat)
      {
      }

    virtual ~myInstance()
      {
      }

    //----------

    // only called for standalone EXEs, we can use contructor instead...
    //virtual int main(int argc, char** argv) { return 0; }

    // virtual void doProcessSample(SPL** aInputs, SPL** aOutputs) {}
    // virtual void doSetParameter(axParameter* aParameter) {}
    // ...

};

//----------------------------------------------------------------------

/*

  editor

*/

//class myEditor : public axEditor
//{
//  public:
//    myEditor(axInstance* aInstance) : axEditor(aInstance) {}
//};

//----------------------------------------------------------------------
//
// main / entrypoint
//
//----------------------------------------------------------------------

//AX_MAIN(myDescriptor,myInstance)
//AX_MAIN(myDescriptor,myInstance,myInterface)

AX_ENTRYPOINT(myDescriptor,myInstance,axInterface,axPlatform)

//#endif // 0
