#ifndef kode_vst3_plugin_included
#define kode_vst3_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance_listener.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_instance.h"
#include "plugin/vst3/kode_vst3_utils.h"

//----------------------------------------------------------------------

template<class DESC, class INST>
class KODE_Vst3Plugin
: public IPluginFactory3 {
  
//------------------------------
private:
//------------------------------

  uint32_t          MRefCount     = 1;
  FUnknown*         MHostContext  = KODE_NULL;
  KODE_Descriptor*  MDescriptor   = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_Vst3Plugin() {
    //KODE_TRACE;
    MRefCount = 1;
    MDescriptor = KODE_New DESC();
  }

  //----------

  virtual ~KODE_Vst3Plugin() {
    //KODE_TRACE;
    if (MDescriptor) KODE_Delete MDescriptor;
  }

//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32  PLUGIN_API addRef() override {
    MRefCount++;
    return MRefCount;
  }

  //----------

  uint32  PLUGIN_API release() override {
    const uint32_t r = --MRefCount;
    if (r == 0) KODE_Delete this;
    return r;
  }

  //----------

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    //char buffer[64];
    //char* iid_txt =  kode_vst3_tuid_to_ascii(_iid,buffer);
    if (FUnknownPrivate::iidEqual(IPluginFactory2_iid,_iid)) {
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (FUnknownPrivate::iidEqual(IPluginFactory3_iid,_iid)) {
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    *obj = KODE_NULL;
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
    KODE_Strcpy(info->vendor,MDescriptor->getAuthor());
    KODE_Strcpy(info->url,MDescriptor->getUrl());
    KODE_Strcpy(info->email,MDescriptor->getEmail());
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  //----------
  
  int32   PLUGIN_API countClasses() override {
    return 1;
  }

  //----------

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) override {
    switch (index) {
      case 0:
        //KODE_Strncpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        KODE_Memcpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        info->cardinality = PClassInfo::kManyInstances;
        KODE_Strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
        KODE_Strncpy(info->name,MDescriptor->getName(),PClassInfo::kNameSize);
        return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) override {
    //char buffer[64];
    //char* cid_txt =  kode_vst3_tuid_to_ascii(cid,buffer);
    //char buffer2[64];
    //char* _iid_txt =  kode_vst3_tuid_to_ascii(_iid,buffer2);
    
    if (FUnknownPrivate::iidEqual(MDescriptor->getLongId(),cid)) {

      INST* instance = KODE_New INST(MDescriptor);
      KODE_Vst3Instance* vst3_instance = KODE_New KODE_Vst3Instance(instance);
      instance->setListener(vst3_instance);
      
      instance->on_open();
      instance->setDefaultParameterValues();
      instance->updateAllParameters();
      
      *obj = (Vst::IComponent*)vst3_instance;
      
      return kResultOk;
    }
    *obj = KODE_NULL;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) override {
    switch (index) {
      case 0:
      
        //KODE_Strcpy(info->cid,(const char*)MDescriptor->getLongId());
        //KODE_Strncpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        KODE_Memcpy(info->cid,(const char*)MDescriptor->getLongId(),16);
        
        info->cardinality = PClassInfo::kManyInstances;
        KODE_Strcpy(info->category,kVstAudioEffectClass);
        KODE_Strcpy(info->name,MDescriptor->getName());
        info->classFlags = 0;
        if (MDescriptor->hasFlag(KODE_PLUGIN_IS_SYNTH)) {
          KODE_Strcpy(info->subCategories,Vst::PlugType::kInstrument);
        }
        else {
          KODE_Strcpy(info->subCategories,Vst::PlugType::kFx);
        }
        KODE_Strcpy(info->vendor,MDescriptor->getAuthor());
        KODE_Strcpy(info->version,MDescriptor->getVersionText());
        KODE_Strcpy(info->sdkVersion,kVstVersionString);
        return kResultOk;
    }
    return kResultFalse;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) override {
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API setHostContext(FUnknown* context) override {
    MHostContext = context;
    return kResultOk;
  }

//------------------------------

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_VST3_MAIN_SYMBOL asm ("GetPluginFactory");
IPluginFactory* PLUGIN_API kode_vst3_entryPoint() KODE_VST3_MAIN_SYMBOL

//----------

#define KODE_VST3_MAIN(DESC,INST)                     \
                                                      \
  __KODE_DLLEXPORT                                    \
  /*IPluginFactory* PLUGIN_API GetPluginFactory() {*/ \
  IPluginFactory* PLUGIN_API kode_vst3_entryPoint() { \
    /*KODE_TRACE;*/                                   \
    return KODE_New KODE_Vst3Plugin<DESC,INST>();     \
  }

//----------------------------------------------------------------------
#endif