#ifndef kode_vst3_plugin_included
#define kode_vst3_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
//#include "plugin/kode_instance.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_host.h"
#include "plugin/vst3/kode_vst3_instance.h"

//----------------------------------------------------------------------

class KODE_Vst3Plugin
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t            MRefCount     = 1;
  FUnknown*           MHostContext  = nullptr;
  KODE_Descriptor*    MDescriptor   = nullptr;

public:

  KODE_Vst3Plugin() {
    VST3_TRACE;
    MRefCount = 1;
    MDescriptor = KODE_GetDescriptor();
  }

  //----------

  virtual ~KODE_Vst3Plugin() {
    VST3_TRACE;
  }

//------------------------------
public:
//------------------------------


  //--------------------
  // FUnknown
  //--------------------

  uint32  PLUGIN_API addRef() override {
    VST3_Trace("-> %i\n",MRefCount+1);
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32  PLUGIN_API release() override {
    VST3_Trace("-> %i\n",MRefCount-1);
    const uint32_t r = --MRefCount; // was:uint32
    if (r == 0) delete this;
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    char buffer[64];
    char* iid_txt =  kode_vst3_tuid_to_ascii(_iid,buffer);
    VST3_Trace("%s : ",iid_txt);
    if (FUnknownPrivate::iidEqual(IPluginFactory2_iid,_iid)) {
      VST3_DTrace("IPluginFactory2_iid\n");
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (FUnknownPrivate::iidEqual(IPluginFactory3_iid,_iid)) {
      VST3_DTrace("IPluginFactory3_iid\n");
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    VST3_DTrace("(unknown)\n");
    *obj = nullptr;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  /*
    Class factory that any Plug-in defines for creating class instances.
    From the host's point of view a Plug-in module is a factory which can
    create a certain kind of object(s). The interface IPluginFactory provides
    methods to get information about the classes exported by the Plug-in and a
    mechanism to create instances of these classes (that usually define the
    IPluginBase interface)
  */

  //----------

  tresult PLUGIN_API getFactoryInfo(PFactoryInfo* info) override {
    VST3_TRACE;
    strcpy(info->vendor,MDescriptor->getAuthor());
    strcpy(info->url,MDescriptor->getUrl());
    strcpy(info->email,MDescriptor->getEmail());
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------

  int32   PLUGIN_API countClasses() override {
    //VST3_Trace("-> %i\n", (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) ? 2 : 1);
    VST3_Trace("-> 1\n");
    return 1;
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    VST3_Trace("%i\n",index);
    switch (index) {
      case 0:
        strncpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        info->cardinality = PClassInfo::kManyInstances;
        strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
        strncpy(info->name,MDescriptor->getName(),PClassInfo::kNameSize);
        return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    char buffer[64];
    char* cid_txt =  kode_vst3_tuid_to_ascii(cid,buffer);
    VST3_Trace("%s: ",cid_txt);
    //char buffer2[64];
    //char* _iid_txt =  kode_vst3_tuid_to_ascii(_iid,buffer2);
    //VST3_Trace("%s [_iid=%s]: ",cid_txt,_iid_txt);
    if (FUnknownPrivate::iidEqual(MDescriptor->getLongId(),cid)) {
      VST3_DTrace("myPlugin\n");
      MDescriptor = KODE_GetDescriptor();
      KODE_Vst3Instance* MVst3Instance = new KODE_Vst3Instance(MDescriptor);
      KODE_Instance* MInstance = MVst3Instance->getInstance();
      MInstance->on_initialize(); // open?
      *obj = (Vst::IComponent*)MVst3Instance;
      return kResultOk;
    }
    VST3_Trace("unknown\n");
    *obj = nullptr;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    VST3_Trace("index %i\n",index);
    switch (index) {
      case 0:
        strcpy(info->cid,(const char*)MDescriptor->getLongId());
        info->cardinality = PClassInfo::kManyInstances;
        strcpy(info->category,kVstAudioEffectClass);
        strcpy(info->name,MDescriptor->getName());
        info->classFlags = 0;
        if (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) {
          strcpy(info->subCategories,Vst::PlugType::kInstrument);
        }
        else {
          strcpy(info->subCategories,Vst::PlugType::kFx);
        }
        strcpy(info->vendor,MDescriptor->getAuthor());
        strcpy(info->version,MDescriptor->getVersionText());
        strcpy(info->sdkVersion,kVstVersionString);
        return kResultOk;
    }
    return kResultFalse;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    VST3_Trace("index %i\n",index);
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    VST3_TRACE;
    MHostContext = context;
    return kResultOk;
  }

};

//----------------------------------------------------------------------
//
// .so entrypoint
//
//----------------------------------------------------------------------

#define KODE_VST3_MAIN_SYMBOL asm ("GetPluginFactory");
IPluginFactory* kode_vst3_entryPoint() KODE_VST3_MAIN_SYMBOL

//----------

__KODE_DLLEXPORT
//IPluginFactory* kode_vst3_entryPoint() {
IPluginFactory* PLUGIN_API GetPluginFactory() {
  VST3_TRACE;
  IPluginFactory* plugin = new KODE_Vst3Plugin();
  return plugin;
}

//----------------------------------------------------------------------
#endif
