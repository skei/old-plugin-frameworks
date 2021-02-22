#ifndef kode_vst3_instance_included
#define kode_vst3_instance_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/vst3/kode_vst3.h"
#include "plugin/vst3/kode_vst3_host.h"
#include "plugin/vst3/kode_vst3_utils.h"

//----------------------------------------------------------------------

const char* type_txt[]      = { "kAudio", "kEvent" };
const char* dir_txt[]       = { "kInput", "kOutput" };
const char* iomode_txt[]    = { "kSimple", "kAdvanced", "kOfflineProcessing" };
const char* procmode_txt[]  = { "kRealtime", "kPrefetch", "kOffline" };
const char* sample_txt[]    = { "kSample32", "kSample64" };

//----------------------------------------------------------------------

class KODE_Vst3Instance

: public IComponent
, public IAudioProcessor
, public IUnitInfo
, public IConnectionPoint
// midi
, public IMidiMapping
, public IKeyswitchController
  //, public INoteExpressionController
// editor
, public IEditController
, public IEditController2
, public IPlugView
// IRunLoop
, public ITimerHandler

{

//------------------------------
private:
//------------------------------

  KODE_Vst3Host*      MHost               = nullptr;
  KODE_Descriptor*    MDescriptor         = nullptr;
  KODE_Instance*      MInstance           = nullptr;
  KODE_Editor*        MEditor             = nullptr;

  uint32_t            MRefCount           = 1;
  IComponentHandler*  MComponentHandler   = nullptr;
  IComponentHandler2* MComponentHandler2  = nullptr;
  IPlugFrame*         MPlugFrame          = nullptr;
  IHostApplication*   MHostApp            = nullptr;
  ParameterInfo*      MParamInfos         = nullptr;

  IRunLoop*           MRunLoop            = nullptr;

  uint32_t            MIoMode             = 0;
  char                MHostName[129]      = {0};
  uint8_t             MStateBuffer[KODE_PLUGIN_VST3_MAX_STATE_SIZE] = {0};

  //

  bool MNeedToInitializeParameters = true;

//------------------------------
public:
//------------------------------

  KODE_Vst3Instance(KODE_Descriptor* ADescriptor) {
    VST3_TRACE;
    MRefCount   = 1;
    MHost       = new KODE_Vst3Host(ADescriptor);
    MDescriptor = ADescriptor;
    MInstance   = KODE_CreateInstance(MDescriptor,MHost);
    _setupParameterInfo();
  }

  //----------

  virtual ~KODE_Vst3Instance() {
    VST3_TRACE;
    if (MHost) delete MHost;
    if (MInstance) delete MInstance;
    if (MParamInfos) free(MParamInfos);
  }

//------------------------------
public:
//------------------------------

  KODE_Vst3Host*  getHost()     { return MHost; }
  KODE_Instance*  getInstance() { return MInstance; }

//----------------------------------------
//public:
private:
//----------------------------------------

  /*
    struct ParameterInfo {
      ParamID id;                         // unique identifier of this parameter (named tag too)
      String128 title;                    // parameter title (e.g. "Volume")
      String128 shortTitle;               // parameter shortTitle (e.g. "Vol")
      String128 units;                    // parameter unit (e.g. "dB")
      int32 stepCount;                    // number of discrete steps (0: continuous, 1: toggle, discrete value otherwise
                                          // (corresponding to max - min, for example: 127 for a min = 0 and a max = 127) - see \ref vst3parameterIntro)
      ParamValue defaultNormalizedValue;  // default normalized value [0,1] (in case of discrete value: defaultNormalizedValue = defDiscreteValue / stepCount)
      UnitID unitId;                      // id of unit this parameter belongs to (see vst3UnitsIntro)
      int32 flags;                        // ParameterFlags (see below)
      enum ParameterFlags {
        kCanAutomate      = 1 << 0,   // parameter can be automated
        kIsReadOnly       = 1 << 1,   // parameter cannot be changed from outside (implies that kCanAutomate is false)
        kIsWrapAround     = 1 << 2,   // attempts to set the parameter value out of the limits will result in a wrap around [SDK 3.0.2]
        kIsList           = 1 << 3,   // parameter should be displayed as list in generic editor or automation editing [SDK 3.1.0]
        kIsProgramChange  = 1 << 15,  // parameter is a program change (unitId gives info about associated unit
                                      // - see \ref vst3UnitPrograms)
        kIsBypass         = 1 << 16   // special bypass parameter (only one allowed): Plug-in can handle bypass
                                      // (highly recommended to export a bypass parameter for effect Plug-in)
      };
    };
  */

  //void _describeParameters() {
  void _setupParameterInfo() {
    if (!MParamInfos) {
      uint32_t num = MDescriptor->getNumParameters();
      MParamInfos = (ParameterInfo*)malloc( num * sizeof(ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        KODE_Parameter* param = MDescriptor->getParameter(i);
        MParamInfos[i].id = i;
        char_to_utf16((char*)param->getName(),MParamInfos[i].title);
        char_to_utf16((char*)param->getShortName(),MParamInfos[i].shortTitle);
        char_to_utf16((char*)param->getLabel(),MParamInfos[i].units);
        if (param->isStepped() == true) MParamInfos[i].stepCount = param->getNumSteps() - 1;
        else MParamInfos[i].stepCount = 0;
        MParamInfos[i].defaultNormalizedValue = param->getDefValue();
        MParamInfos[i].unitId = kRootUnitId; //-1;
        int32_t flags = 0;
        if (param->hasFlag(KODE_PARAMETER_AUTOMATE)) flags += ParameterInfo::kCanAutomate;
        else flags += ParameterInfo::kIsReadOnly;
        MParamInfos[i].flags = flags;
      }
    }
  }

  //----------

  /*
    class IParameterChanges: public FUnknown {
      virtual int32             PLUGIN_API getParameterCount();
      virtual IParamValueQueue* PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* PLUGIN_API addParameterData(const Vst::ParamID& id, int32& index);
    };
  */

  void _handleParametersInProcess(ProcessData& data) {
    //KODE_Trace("\n");
    IParameterChanges* paramChanges = data.inputParameterChanges;
    if (paramChanges) {
      int32_t num = paramChanges->getParameterCount();
      if (num > 0) {
        for (int32_t i=0; i<num; i++) {
          IParamValueQueue* paramQueue = paramChanges->getParameterData(i);
          if (paramQueue) {
            uint32_t id = paramQueue->getParameterId();

            // parameter
            if (id < MDescriptor->getNumParameters()) {
              //for (int32_t j=0; j<paramQueue->getPointCount(); j++) {
                int32_t offset = 0;
                double value = 0;
                int32_t pointcount = paramQueue->getPointCount();
                paramQueue->getPoint(pointcount-1,offset,value); // last point

//                MInstance->setProcessValue(id,value); // do we need this?

                KODE_Parameter* param = MDescriptor->getParameter(id);
                if (param) value = param->from01(value);
                MInstance->on_parameterChange(id,value);

              //}
            } // id = param

            // if KODE_PLUGIN_RECEIVE_MIDI
            // IMidiMapping
            else {
              int32_t offset = 0;
              double value = 0.5;
              int32_t pointcount = paramQueue->getPointCount();
              paramQueue->getPoint(pointcount-1,offset,value);      // last point
              uint32_t midi_ev = (id & 0xffff0000);
              uint32_t midi_ch = (id & 0x0000ffff);
              switch(midi_ev) {
                case KODE_VST3_PARAM_AFTERTOUCH: {
                  //KODE_TRACE("aftertouch ch %i value %.3f\n",midi_ch,value);
                  //if (offset != 0)
                  MInstance->on_midiEvent(offset,KODE_MIDI_CHANNEL_AFTERTOUCH+midi_ch,value*127.0f,0);
                  break;
                } // at
                case KODE_VST3_PARAM_PITCHBEND: {
                  //KODE_TRACE("pitchbend ch %i value %.3f\n",midi_ch,value);
                  float v2 = value * 16383.0f;
                  uint32_t i2 = (uint32_t)v2;
                  //KODE_TRACE("pitchbend ofs %i ch %i value %.3f i2 %i \n",offset,midi_ch,value,i2);
                  //if (midi_ch != 0) {
                    uint32_t m2 = i2 & 0x7f;
                    uint32_t m3 = i2 >> 7;
                    //if (offset != 0)
                    MInstance->on_midiEvent(offset,KODE_MIDI_PITCHBEND+midi_ch,m2,m3);
                  //}
                  break;
                } // pb
                case KODE_VST3_PARAM_BRIGHTNESS: {
                  //KODE_TRACE("brightness ch %i value %.3f\n",midi_ch,value);
                  //if (offset != 0)
                  MInstance->on_midiEvent(offset,KODE_MIDI_CONTROL_CHANGE+midi_ch,74,value*127.0f);
                  break;
                } // br
              } // midi_ev
            } //IMidiMapping

          } // paramqueue
        } // for all params
      } // numparams > 0
    } // paramchanges
  }

  //----------

  /*
    class IEventList: public FUnknown {
      virtual int32   PLUGIN_API getEventCount();
      virtual tresult PLUGIN_API getEvent(int32 index, Event& e);
      virtual tresult PLUGIN_API addEvent(Event& e);
    };
  */

  void _handleEventsInProcess(ProcessData& data) {
    // if KODE_PLUGIN_RECEIVE_MIDI
    IEventList* inputEvents = data.inputEvents;
    if (inputEvents) {
      int32_t num = inputEvents->getEventCount();
      for (int32_t i=0; i<num; i++) {
        Event event;
        //tresult res =
        inputEvents->getEvent(i,event);
        uint32_t offset  = 0;
        uint8_t  msg1    = 0;
        uint8_t  msg2    = 0;
        uint8_t  msg3    = 0;
        //uint32_t type    = kve_none;
        //uint32_t noteid  = 0;
        //float    value   = 0.0f;
        switch (event.type) {
          case Event::kNoteOnEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_NOTE_ON + event.noteOn.channel;
            msg2    = event.noteOn.pitch;
            msg3    = event.noteOn.velocity * 127;
            //noteid  = event.noteOn.noteId;
            MInstance->on_midiEvent(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kNoteOffEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_NOTE_OFF + event.noteOff.channel;
            msg2    = event.noteOff.pitch;
            msg3    = event.noteOff.velocity * 127;
            //noteid  = event.noteOff.noteId;
            MInstance->on_midiEvent(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kDataEvent:
            break;
          case Event::kPolyPressureEvent:
            offset  = event.sampleOffset;
            msg1    = KODE_MIDI_POLY_AFTERTOUCH + event.polyPressure.channel;
            msg2    = event.polyPressure.pitch;
            msg3    = event.polyPressure.pressure * 127;
            //noteid  = event.polyPressure.noteId;
            MInstance->on_midiEvent(offset,msg1,msg2,msg3);
            break;
          /*
          case Event::kNoteExpressionValueEvent:
            //KODE_Trace("note expr\n");
            offset = event.sampleOffset;
            noteid = event.noteExpressionValue.noteId;
            value  = event.noteExpressionValue.value;
            switch(event.noteExpressionValue.typeId) {
              case kTuningTypeID:     type = kve_bend; break;
              case kBrightnessTypeID: type = kve_slide; break;
              //case kVolumeTypeID:     type = kve_none;  break;
              //case kPanTypeID:        type = kve_none;  break;
              //case kVibratoTypeID:    type = kve_none;  break;
              //case kExpressionTypeID: type = kve_none;  break;
            }
            on_noteExpression(offset,type,noteid,value);
            break;
          */
          case Event::kNoteExpressionTextEvent:
            break;
          case Event::kChordEvent:
            break;
          case Event::kScaleEvent:
            break;
          default:
            break;
        }
      }
    }
  }

//----------------------------------------
public:
//----------------------------------------

  //--------------------
  // FUnknown
  //--------------------

  /*
    Adds a reference and return the new reference count.
    Remarks: The initial reference count after creating an object is 1.
  */

  uint32 PLUGIN_API addRef() override {
    VST3_Trace("-> %i\n",MRefCount+1);
    MRefCount++;
    return MRefCount;
  }

  //----------

  /*
    Releases a reference and return the new reference count.
    If the reference count reaches zero, the object will be destroyed in
    memory.
  */

  uint32 PLUGIN_API release() override {
    VST3_Trace("-> %i\n",MRefCount-1);
    const uint32 r = --MRefCount;
    if (r == 0) {
      //MInstance->on_terminate();
      //MInstance->on_close();
      delete this; // ???
    };
    return r;
  }

  //----------

  /*
    Query for a pointer to the specified interface.
    Returns kResultOk on success or kNoInterface if the object does not
    implement the interface.
    The object has to call addRef when returning an interface.
    _iid: (in)  16 Byte interface identifier (-> FUID)
    obj:  (out) On return, *obj points to the requested interface

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    Although it is not recommended, it is possible to implement both, the
    processing part and the controller part in one component class. The host
    tries to query the IEditController interface after creating an
    IAudioProcessor and on success uses it as controller.

    reaper asks for:
      C3B17BC0-2C174494-80293402-FBC4BBF8 (IContextInfoHandler,   plsextensions)
      31E29A7A-E55043AD-8B95B9B8-DA1FBE1E (IContextInfoHandler2,  pslextensions)

    bitwig asks for:
      6C389611-D391455D-B870B833-94A0EFDD (IUnitData)
      8683B01F-7B354F70-A2651DEC-353AF4FF (IProgramListData)
      7F4EFE59-F3204967-AC27A3AE-AFB63038 (IEditController2)
      65ED9690-8AC44525-8AADEF7A-72EA703F (IPlugViewContentScaleSupport)

    ( 65ed9690-8ac44525-8aadef7a-72ea703f (IPlugInViewScaling, pslextensions) !! )

      633b9012-d19a4953-b477b436-9daaeedb
      8e8bbb11-73384477-ac6b1eeb-383df4f0
      ccc87233-accb4371-bc74a1f2-5ebba43c

      7743fe5e-f3224d6d-a729ada9-aab2333b
      cd291308-775e4492-b5db37be-62bb1a54
      41c5087e-4bbb4e8b-8da21bd2-97dc6679
      001a4b89-86924ed8-b7aacdee-cc12d6dd
      98e97900-c220484c-82a3edcd-d06cdd45

      6bec9099-83c14927-83a8ee72-79e97733

  */

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override {
    char buffer[64];
    char* iid_txt = kode_vst3_tuid_to_ascii(_iid,buffer);
    VST3_Trace("%s : ",iid_txt);
    *obj = nullptr;
    if ( FUnknownPrivate::iidEqual(IAudioProcessor_iid,_iid) ) {
      VST3_DTrace("IAudioProcessor_iid\n");
      *obj = (IAudioProcessor*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IEditController_iid,_iid) ) {
      VST3_DTrace("IEditController_iid\n");
      *obj = (IEditController*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IMidiMapping_iid,_iid) ) {
      VST3_DTrace("IMidiMapping_iid\n");
      *obj = (IMidiMapping*)this;
      addRef();
      return kResultOk;
    }
    if ( FUnknownPrivate::iidEqual(IUnitInfo_iid,_iid) ) {
      VST3_DTrace("IUnitInfo_iid\n");
      *obj = (IUnitInfo*)this;
      addRef();
      return kResultOk;
      //return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(INoteExpressionController_iid,_iid) ) {
      VST3_DTrace("(INoteExpressionController_iid)\n");
      //*obj = (INoteExpressionController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(IKeyswitchController_iid,_iid) ) {
      VST3_DTrace("(IKeyswitchController_iid)\n");
      //*obj = (IKeyswitchController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( FUnknownPrivate::iidEqual(IConnectionPoint_iid,_iid) ) {
      VST3_DTrace("(IConnectionPoint_iid)\n");
      *obj = (IConnectionPoint*)this;
      addRef();
      return kResultOk;
      //return kNoInterface;
    }
    //if ( FUnknownPrivate::iidEqual(ITimerHandler_iid,_iid) ) {
    //  VST3_DTrace("ITimerHandler_iid\n");
    //  *obj = (ITimerHandler*)this;
    //  addRef();
    //  return kResultOk;
    //}
    VST3_DTrace("(unknown)\n");
    return kNoInterface;
  }

  //--------------------
  // IPluginBase
  //--------------------

  /*
    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    The context parameter passed to IPluginBase::initialize is
    Vst::IHostApplication. Hosts should not call others functions before
    initialize is called!, except IComponent::setIoMode which need to be
    called before or IComponent::getControllerClassId which could be called
    before.

    note: Extensive memory allocations etc. should be performed in this
    method rather than in the class' constructor!
    If the method does NOT return kResultOk, the object is released
    immediately. In this case terminate is not called!
  */

  /*
    class IHostApplication: public FUnknown {
      // Gets host application name.
      virtual tresult PLUGIN_API getName (String128 name) = 0;
      // Creates host object (e.g. Vst::IMessage).
      virtual tresult PLUGIN_API createInstance (TUID cid, TUID _iid, void** obj) = 0;
  */

  /*
    3.6.12
    Allow a Plug-in to ask the host if a given Plug-in interface is
    supported/used by the host. It is implemented by the hostContext given
    when the component is initialized.
    tresult PLUGIN_API MyPluginController::initialize (FUnknown* context) {
      // ...
      FUnknownPtr<IPlugInterfaceSupport> plugInterfaceSupport (context);
      if (plugInterfaceSupport) {
        if (plugInterfaceSupport->isPlugInterfaceSupported (IMidiMapping::iid) == kResultTrue)
          // IMidiMapping is used by the host
      }
      // ...
    }
  */

  /*
    IHostApplication: passed as 'context' in to IPluginBase::initialize()
  */

  tresult PLUGIN_API initialize(FUnknown* context) override {
    //VST3_Trace("");
    MHostApp = (IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      String128 u;
      MHostApp->getName(u);
      utf16_to_char(u,MHostName);
      VST3_Trace("host: %s\n",MHostName);
    }
    else {
      VST3_Trace("(unknown host)\n");
    }

    MInstance->on_initialize();
    return kResultOk;
  }

  //----------

  /*
    This function is called before the Plug-in is unloaded and can be used
    for cleanups. You have to release all references to any host application
    interfaces.
  */

  tresult PLUGIN_API terminate() override {
    VST3_TRACE;
    MInstance->on_terminate();
    return kResultOk;
  }

  //--------------------
  // IComponent
  //--------------------

  /*
    Called before initializing the component to get information about the
    controller class.

    file:///WORK/code/_backup/vst3/VST3SDK/index.html

    In order to enable the host to create a corresponding edit controller the
    processing component has to provide the matching class-ID. The host uses
    the module's class factory to create the controller component.
    (IComponent::getControllerClassId)
  */

  tresult PLUGIN_API getControllerClassId(TUID classId) override {
    char buffer[64];
    char* classid_txt = kode_vst3_tuid_to_ascii(classId,buffer);
    VST3_Trace("%s\n",classid_txt);
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      memcpy(classId,MDescriptor->getLongEditorId(),16);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Called before 'initialize' to set the component usage (optional).
    See IoModes
  */

  tresult PLUGIN_API setIoMode(IoMode mode) override {
    VST3_Trace("mode %s\n",iomode_txt[mode]);
    //switch (mode) {
    //  case kSimple:             VST3_Trace("(kSimple)\n"); break;
    //  case kAdvanced:           VST3_Trace("(kAdvanced)\n"); break;
    //  case kOfflineProcessing:  VST3_Trace("(kOfflineProcessing)\n"); break;
    //  default:                  VST3_Trace("(unknown)\n"); break;
    //}
    MIoMode = mode;
    return kResultOk;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    enum MediaTypes {
      kAudio = 0,   // audio
      kEvent,       // events
      kNumMediaTypes
    };
    enum BusDirections {
      kInput = 0,   //input bus
      kOutput       // output bus
    };
  */

  int32 PLUGIN_API getBusCount(MediaType type, BusDirection dir) override {
    VST3_Trace("type %s dir %s",type_txt[type],dir_txt[dir]);
    if (type == kAudio) {
      if ((dir == kOutput) && (MDescriptor->getNumOutputs() > 0)) { VST3_DTrace(" -> 1\n"); return 1; }// && numOutputs > 0
      if ((dir == kInput)  && (MDescriptor->getNumInputs()  > 0)) { VST3_DTrace(" -> 1\n"); return 1; }; // && numInputs > 0
    }
    if (type == kEvent) {
      //if (dir==kOutput) return 1;
      //else
      if (MDescriptor->hasFlag(KODE_PLUGIN_RECEIVE_MIDI)) {
        if (dir == kInput) { VST3_DTrace(" -> 1\n"); return 1; }
      }
    }
    VST3_DTrace(" -> 0\n");
    return 0;
  }

  //----------

  /*
    Called after the Plug-in is initialized.
    See MediaTypes, BusDirections

    struct BusInfo {
      MediaType mediaType;    // Media type - has to be a value of \ref MediaTypes
      BusDirection direction; // input or output \ref BusDirections
      int32 channelCount;     // number of channels (if used then need to be recheck after \ref IAudioProcessor::setBusArrangements is called)
      String128 name;         // name of the bus
      BusType busType;        // main or aux - has to be a value of \ref BusTypes
      uint32 flags;           // flags - a combination of \ref BusFlags
      enum BusFlags {
        kDefaultActive = 1 << 0	///< bus active per default
      };
    };
  */

  tresult PLUGIN_API getBusInfo(MediaType type, BusDirection dir, int32 index, BusInfo& bus) override {
    VST3_Trace("type %s dir %s index %i\n",type_txt[type],dir_txt[dir],index);
    if (type == kAudio) {
      bus.mediaType = kAudio;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = MDescriptor->getNumInputs();
        char_to_utf16("Audio In",bus.name);
        //VST3_DTrace("-> direction: kInput\n");
        //VST3_DTrace("   channels: %i\n",MDescriptor->getNumInputs());
        //VST3_DTrace("   name: Audio In\n");
      }
      else {
        bus.direction = kOutput;
        bus.channelCount = MDescriptor->getNumOutputs();
        char_to_utf16("Audio Out",bus.name);
        //VST3_DTrace("-> direction: kOutput\n");
        //VST3_DTrace("   channels: %i\n",MDescriptor->getNumOutputs());
        //VST3_DTrace("   name: Audio Out\n");
      }
      bus.flags = 0;//kDefaultActive;
      //VST3_DTrace("   flags: 0\n");
      return kResultOk;
    }
    else if (type == kEvent) {
      bus.mediaType = kEvent;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = 1;
        char_to_utf16("Midi In",bus.name);
        //VST3_DTrace("-> direction: kInput\n");
        //VST3_DTrace("   channels: 1\n");
        //VST3_DTrace("   name: Midi In\n");
      }
      bus.flags = 0;//kDefaultActive;
      //VST3_DTrace("   flags: 0\n");
      return kResultOk;
    }

    return kResultFalse;
  }

  //----------

  /*
    Retrieves routing information (to be implemented when more than one
    regular input or output bus exists).
    The inInfo always refers to an input bus while the returned outInfo must
    refer to an output bus!

    struct RoutingInfo
    {
      MediaType mediaType;  // media type see \ref MediaTypes
      int32 busIndex;       // bus index
      int32 channel;        // channel (-1 for all channels)
    };
  */

  tresult PLUGIN_API getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo) override {
    VST3_TRACE;
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kAudio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    return kResultOk;
  }

  //----------

  /*
    Called upon (de-)activating a bus in the host application. The Plug-in
    should only processed an activated bus, the host could provide less
    AudioBusBuffers in the process call (see IAudioProcessor::process) if
    last buses are not activated
  */

  tresult PLUGIN_API activateBus(MediaType type, BusDirection dir, int32 index, TBool state) override {
    VST3_Trace("type %s dir %s index %i state %i\n",type_txt[type],dir_txt[dir],index,state);
    return kResultOk;
  }

  //----------

  /*
    Activates / deactivates the component.
  */

  tresult PLUGIN_API setActive(TBool state) override {
    VST3_Trace("state %i\n",state);
    //if (state) on_stateChange(kps2_activate);
    //else on_stateChange(kps2_deactivate);
    return kResultOk;
  }

  //----------

  /*

    How does persistence work?

    An instantiated Plug-in often has state information that must be saved in
    order to properly re-instantiate that Plug-in at a later time. A VST 3
    Plug-in has two states which are saved and reloaded: its component state
    and its controller state.
    The sequence of actions for saving is:
      component->getState (compState)
      controller->getState (ctrlState)
    The sequence of actions for loading is:
      component->setState (compState)
      controller->setComponentState (compState)
      controller->setState (ctrlState)
    In the latter sequence you can see that the controller part will receive
    the component state. This allows the 2 parts to synchronize their states.

    What's the difference between IEditController::setComponentState and
    IEditController::setState?

    After a preset is loaded, the host calls IEditController::setComponentState
    and IComponent::setState. Both delivering the same information.
    IEditController::setState is called by the host so that the Plug-in is able
    to update its controller dependent parameters, eg. position of scroll bars.
    Prior to this, there should have been a call by the host to
    IEditController::getState, where the Plug-in writes these various
    parameters into the stream.

    Can IComponent::getState()/setState() could be called during processing?

    Yes, setState and getState are called normally from the UI Thread when the
    Plug-in is used in a realtime context, in an offline context set/getState
    could be called in the same thread than the process call. Check the
    workflow diagram Audio Processor Call Sequence for more info about in which
    state which interfaces are called.
  */

  //----------

  /*
    Restore: When the states are restored, the host passes the processor state
    to both the processor and the controller (IEditController::setComponentState).
    A host must always pass that state to the processor first. The controller
    then has to synchronize its parameters to this state (but must not perform
    any IComponentHandler callbacks).
    After restoring a state, the host will rescan the parameters (asking the
    controller) in order to update its intern representation.
  */

  /*
    IBStream->read(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesRead=0)     // how many bytes have been read from stream (set to 0 if this is of no interest)
    )

    IBStream->write(
      void*   buffer,             // destination buffer
      int32   numBytes,           // amount of bytes to be read
      int32*  numBytesWritten=0)  // how many bytes have been written to stream (set to 0 if this is of no interest)
    )

  */

  /*
    Sets complete state of component.
    //state->read(MParamValues,MNumParameters*sizeof(float));
  */

  tresult PLUGIN_API setState(IBStream* state) override {
    VST3_TRACE;


    #ifdef KODE_PLUGIN_SIMPLE_STATE

      uint32_t  version     = MDescriptor->getVersion();
      uint32_t  num_params  = MDescriptor->getNumParameters();
      uint32_t  mode        = 0;
      uint32_t  size        = 0;
      int       num_read    = 0;

      state->read(&mode,sizeof(uint32_t),&num_read);
      state->read(&version,sizeof(uint32_t),&num_read);
      state->read(&size,sizeof(uint32_t),&num_read);

      for (uint32_t i=0; i<num_params; i++) {
        float v = 0.f;
        state->read(&v,sizeof(float),&num_read);
        //MInstance->setParameter(i,v);
        MInstance->setProcessValue(i,v);
      }
      MInstance->initializeAllParameters();

    #else

      /*
      state->read(&mode,sizeof(uint32_t),&num_read);
      state->read(&version,sizeof(uint32_t),&num_read);
      state->read(&size,sizeof(uint32_t),&num_read);
      state->read(MStateBuffer,size,&num_read);
      */

    #endif

    return kResultOk;
  }

  //----------

  /*
    Retrieves complete state of component.
    //state->write(MParamValues,MNumParameters*sizeof(float));
  */

  tresult PLUGIN_API getState(IBStream* state) override {
    VST3_TRACE;

    #ifdef KODE_PLUGIN_SIMPLE_STATE

      uint32_t version      = MDescriptor->getVersion();
      uint32_t num_params   = MDescriptor->getNumParameters();
      uint32_t mode         = 0;
      uint32_t size         = num_params * sizeof(float);
      int      num_written  = 0;

      state->write(&mode,sizeof(uint32_t),&num_written);
      state->write(&version,sizeof(uint32_t),&num_written);
      state->write(&size,sizeof(uint32_t),&num_written);

      for (uint32_t i=0; i<num_params; i++) {
        float v = MInstance->getProcessValue(i);
        state->write(&v,sizeof(float),&num_written);
      }

    #else // KODE_PLUGIN_SIMPLE_STATE

      /*
      mode = 1;
      MInstance->on_getState(&size,&buffer);
      state->write(&mode,sizeof(uint32_t),&num_written);
      state->write(&version,sizeof(uint32_t),&num_written);
      state->write(&size,sizeof(uint32_t),&num_written);
      if (buffer) {
        state->write(buffer,size,&num_written);
      }
      */

    #endif // KODE_PLUGIN_SIMPLE_STATE

    return kResultOk;
  }

  //--------------------
  // IAudioProcessor
  //--------------------

  /*
    Try to set (from host) a predefined arrangement for inputs and outputs.
    The host should always deliver the same number of input and output buses
    than the Plug-in needs (see IComponent::getBusCount).
    The Plug-in returns kResultFalse if wanted arrangements are not supported.
    If the Plug-in accepts these arrangements, it should modify its buses to
    match the new arrangements
    (asked by the host with IComponent::getInfo() or
    IAudioProcessor::getBusArrangement ()) and then return kResultTrue.
    If the Plug-in does not accept these arrangements, but can adapt its
    current arrangements (according to the wanted ones), it should modify its
    buses arrangements and return kResultFalse.
  */

  // const SpeakerArrangement kStereo = kSpeakerL | kSpeakerR;

  tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) override {
    VST3_Trace("numins %i numouts %i\n",numIns,numOuts);
    return kResultTrue;
  }

  //----------

  /*
    Gets the bus arrangement for a given direction (input/output) and index.
    Note: IComponent::getInfo() and IAudioProcessor::getBusArrangement()
    should be always return the same information about the buses arrangements.
  */

  tresult PLUGIN_API getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr) override {
    VST3_Trace("dir %s index %i\n",dir_txt[dir],index);
    if ((dir==kOutput) && (index==0)) {
      arr = Steinberg::Vst::kSpeakerL | Steinberg::Vst::kSpeakerR;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Asks if a given sample size is supported.
    see SymbolicSampleSizes.

    enum SymbolicSampleSizes {
      kSample32,    // 32-bit precision
      kSample64     // 64-bit precision
    };
  */

  tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) override {
    VST3_Trace_RT("size %s",sample_txt[symbolicSampleSize]);
    if (symbolicSampleSize==kSample32) {
      VST3_DTrace_RT(" -> true\n");
      return kResultTrue;
    }
    VST3_DTrace_RT(" -> false\n");
    return kResultFalse;// kResultFalse;// kNotImplemented;
  }

  //----------

  /*
    Gets the current Latency in samples.
    The returned value defines the group delay or the latency of the Plug-in.
    For example, if the Plug-in internally needs to look in advance (like
    compressors) 512 samples then this Plug-in should report 512 as latency.
    If during the use of the Plug-in this latency change, the Plug-in has to
    inform the host by using IComponentHandler::restartComponent(kLatencyChanged),
    this could lead to audio playback interruption because the host has to
    recompute its internal mixer delay compensation.
    Note that for player live recording this latency should be zero or small.
  */

  uint32 PLUGIN_API getLatencySamples() override {
    VST3_Trace("-> 0\n");
    return 0;
  }

  //----------

  /*
    Called in disable state (not active) before processing will begin.
  */

  /*
    enum ProcessModes {
      kRealtime,  // realtime processing
      kPrefetch,  // prefetch processing
      kOffline    // offline processing
    };
    enum SymbolicSampleSizes {
      kSample32,  // 32-bit precision
      kSample64   // 64-bit precision
    };
    struct ProcessSetup {
      int32 processMode;        // ProcessModes
      int32 symbolicSampleSize; // \ref SymbolicSampleSizes
      int32 maxSamplesPerBlock; // maximum number of samples per audio block
      SampleRate sampleRate;    // sample rate (double)
    };
  */

  tresult PLUGIN_API setupProcessing(ProcessSetup& setup) override {
    VST3_Trace("[processmode:%s samplesize:%s samplerate:%.2f ]\n",procmode_txt[setup.processMode],sample_txt[setup.symbolicSampleSize],setup.sampleRate);
  //  MSampleRate = setup.sampleRate;
    //MBlockSize  = setup.blockSize;
    //MProcessMode = setup.processMode;
    //MSampleSize = setup.symbolicSampleSize;
    // setSampleRate(MSampleRate);
  //  MInstance->on_stateChange(kps_prepare);
  //  changeAllParameters();
    return kResultOk;
  }

  //----------

  /*
    Informs the Plug-in about the processing state. This will be called
    before any process calls start with true and after with false.
    Note that setProcessing (false) may be called after setProcessing (true)
    without any process calls.
    In this call the Plug-in should do only light operation (no memory
    allocation or big setup reconfiguration), this could be used to reset
    some buffers (like Delay line or Reverb).
  */

  tresult PLUGIN_API setProcessing(TBool state) override {
    VST3_Trace("state %i\n",state);
    if (state) MInstance->on_activate();
    else MInstance->on_deactivate();
    return kResultOk;
  }

  //----------

  /*
    The Process call, where all information (parameter changes, event,
    audio buffer) are passed.

    struct AudioBusBuffers {
      int32   numChannels;            // number of audio channels in bus
      uint64  silenceFlags;           // Bitset of silence state per channel
      union {
        Sample32**  channelBuffers32; // sample buffers to process with 32-bit precision
        Sample64**  channelBuffers64; // sample buffers to process with 64-bit precision
      };
    };

    class IParamValueQueue {
      virtual ParamID PLUGIN_API getParameterId();
      virtual int32   PLUGIN_API getPointCount();
      virtual tresult PLUGIN_API getPoint (int32 index, int32& sampleOffset, ParamValue& value);
      virtual tresult PLUGIN_API addPoint (int32 sampleOffset, ParamValue value, int32& index);
    };

    class IParameterChanges: public FUnknown {
      virtual int32             PLUGIN_API getParameterCount();
      virtual IParamValueQueue* PLUGIN_API getParameterData(int32 index);
      virtual IParamValueQueue* PLUGIN_API addParameterData(const Vst::ParamID& id, int32& index);
    };

    class IEventList: public FUnknown {
      virtual int32   PLUGIN_API getEventCount();
      virtual tresult PLUGIN_API getEvent(int32 index, Event& e);
      virtual tresult PLUGIN_API addEvent(Event& e);
    };

    struct ProcessContext {
      // Transport state & other flags
      enum StatesAndFlags {
        kPlaying                = 1 << 1,   // currently playing
        kCycleActive            = 1 << 2,   // cycle is active
        kRecording              = 1 << 3,   // currently recording
        kSystemTimeValid        = 1 << 8,   // systemTime contains valid information
        kContTimeValid          = 1 << 17,  // continousTimeSamples contains valid information
        kProjectTimeMusicValid  = 1 << 9,   // projectTimeMusic contains valid information
        kBarPositionValid       = 1 << 11,  // barPositionMusic contains valid information
        kCycleValid             = 1 << 12,  // cycleStartMusic and barPositionMusic contain valid information
        kTempoValid             = 1 << 10,  // tempo contains valid information
        kTimeSigValid           = 1 << 13,  // timeSigNumerator and timeSigDenominator contain valid information
        kChordValid             = 1 << 18,  // chord contains valid information
        kSmpteValid             = 1 << 14,  // smpteOffset and frameRate contain valid information
        kClockValid             = 1 << 15   // samplesToNextClock valid
      };
      uint32        state;                // a combination of the values from \ref StatesAndFlags
      double        sampleRate;           // current sample rate (always valid)
      TSamples      projectTimeSamples;   // project time in samples (always valid)
      int64         systemTime;           // system time in nanoseconds (optional)
      TSamples      continousTimeSamples; // project time, without loop (optional)
      TQuarterNotes projectTimeMusic;     // musical position in quarter notes (1.0 equals 1 quarter note)
      TQuarterNotes barPositionMusic;     // last bar start position, in quarter notes
      TQuarterNotes cycleStartMusic;      // cycle start in quarter notes
      TQuarterNotes cycleEndMusic;        // cycle end in quarter notes
      double        tempo;                // tempo in BPM (Beats Per Minute)
      int32         timeSigNumerator;     // time signature numerator (e.g. 3 for 3/4)
      int32         timeSigDenominator;   // time signature denominator (e.g. 4 for 3/4)
      Chord         chord;                // musical info
      int32         smpteOffsetSubframes; // SMPTE (sync) offset in subframes (1/80 of frame)
      FrameRate     frameRate;            // frame rate
      int32         samplesToNextClock;   // MIDI Clock Resolution (24 Per Quarter Note), can be negative (nearest)
    };

    struct ProcessData {
      int32               processMode;            // processing mode - value of \ref ProcessModes
      int32               symbolicSampleSize;     // sample size - value of \ref SymbolicSampleSizes
      int32               numSamples;             // number of samples to process
      int32               numInputs;              // number of audio input buses
      int32               numOutputs;             // number of audio output buses
      AudioBusBuffers*    inputs;                 // buffers of input buses
      AudioBusBuffers*    outputs;                // buffers of output buses
      IParameterChanges*  inputParameterChanges;	// incoming parameter changes for this block
      IParameterChanges*  outputParameterChanges; // outgoing parameter changes for this block (optional)
      IEventList*         inputEvents;            // incoming events for this block (optional)
      IEventList*         outputEvents;           // outgoing events for this block (optional)
      ProcessContext*     processContext;         // processing context (optional, but most welcome)
    };

  */

  /*
    VST3_SDK/doc/vstsdk/faq.html#faqCommunication6
    Q: How my parameter changes (from UI interaction) are send to the
    processor if the host does not process?
    When a parameter is changed in the Plug-in UI by user action, the plug
    sends this change to the host with performEdit (do not forget to call
    beginEdit and endEdit), then the host has the responsibility to transfer
    this parameter change to the processor part: => if the audio engine is
    running (playing) this will be done in the next available process call =>
    if the audio engine is not running, the host has to flush parameter
    changes from time to time by sending them to processor by calling process
    (with audio buffer set to null), in this case the Plug-in should only
    update the parameters changes without processing any audio. This is very
    important that the host supports this flush mechanism else by saving
    Plug-ins state (project/preset) the host will not get the correct updated
    one.

    VST3_SDK/doc/vstinterfaces/index.html#vst3Editing
    Any change to a parameter that is caused by user interaction in the
    Plug-in GUI must be properly reported to the Steinberg::Vst::
    IComponentHandler. The host is responsible for transmitting the change to
    the processor. In order to make recording of automation work accordingly
    it is necessary to call beginEdit, performEdit and endEdit in the
    expected order!
    When the controller transmits a parameter change to the host, the host
    synchronizes the processor by passing the new values as Steinberg::Vst::
    IParameterChanges to the process call.
  */

  /*
    http://www.noelborthwick.com/2014/01/22/developer-notes-sonar-x3-vst3-internals/
    The host may also send parameters to the plugin when no processing is
    active via a special “flush parameters” process call. This happens when
    the audio engine has been stopped in SONAR and the user manipulates an
    envelope or the user interface widget to change a plugin parameter.
    In this call numInputs and numOutputs of Steinberg::Vst::AudioBusBuffers
    are zeroed as well as numSamples. Plugin’s should expect and properly
    handle this call.
  */

  // assume only 1 input/output bus..

  tresult PLUGIN_API process(ProcessData& data) override {
    if (MNeedToInitializeParameters) {
      MInstance->initializeAllParameters();
      MNeedToInitializeParameters = false;
    }

    _handleEventsInProcess(data);
    _handleParametersInProcess(data);
    //MInstance->updateEventsInProcess();

    //KODE_DTrace("num %i : in %i out %i\n",data.numSamples,data.inputs[0].numChannels,data.outputs[0].numChannels);

    bool _flush = ( (data.numInputs == 0)
                 && (data.numOutputs == 0)
                 && (data.numSamples == 0) );
    if (!_flush) {
      //printf("num %i : in %i out %i\n",data.numSamples,data.inputs[0].numChannels,data.outputs[0].numChannels);
      //KODE_Trace("1\n");
      KODE_ProcessContext context;// = {0};
      uint32_t i;
      for (i=0; i<MDescriptor->getNumInputs(); i++) { context.inputs[i]  = data.inputs[0].channelBuffers32[i]; }
      for (i=0; i<MDescriptor->getNumOutputs(); i++) { context.outputs[i] = data.outputs[0].channelBuffers32[i]; }

      context.length        = data.numSamples;
      context.oversample    = 1;
      context.samplerate    = data.processContext->sampleRate;
      context.samplepos     = data.processContext->continousTimeSamples;
      context.beatpos       = data.processContext->projectTimeMusic;
      context.tempo         = data.processContext->tempo;
      context.timesig_num   = data.processContext->timeSigNumerator;
      context.timesig_denom = data.processContext->timeSigDenominator;

      context.playstate = KODE_PLUGIN_PLAYSTATE_NONE;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kPlaying)      context.playstate |= KODE_PLUGIN_PLAYSTATE_PLAYING;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kRecording)    context.playstate |= KODE_PLUGIN_PLAYSTATE_RECORDING;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kCycleActive)  context.playstate |= KODE_PLUGIN_PLAYSTATE_LOOPING;

      MInstance->on_processBlock(&context);

      //on_postProcess();
      //if hasFlag(KODE_PLUGIN_SEND_MIDI) flushMidi();
      //
    }

    /*
      https://forum.juce.com/t/vst3-plugin-wrapper/12323/5
      I recall the Steinberg Validator complaining that process() should just
      return kResultTrue...
    */

    return kResultOk; // = kResulttrue (0)



  }

  //----------

  /*
    Gets tail size in samples. For example, if the Plug-in is a Reverb
    Plug-in and it knows that the maximum length of the Reverb is 2sec, then
    it has to return in getTailSamples()
    (in VST2 it was getGetTailSize()) : 2*sampleRate.
    This information could be used by host for offline processing, process
    optimization and downmix (avoiding signal cut (clicks)).
    It should return:
    - kNoTail when no tail
    - x * sampleRate when x Sec tail.
    - kInfiniteTail when infinite tail.
  */

  uint32 PLUGIN_API getTailSamples() override {
    //VST3_Trace("-> kNoTail\n");
    return kNoTail;
  }

  //--------------------
  // IMidiMapping
  //--------------------

  /*
    busIndex              index of Input Event Bus
    channel               channel of the bus
    midiControllerNumber  see \ref ControllerNumbers for expected values (could be bigger than 127)
    id                    return the associated ParamID to the given midiControllerNumber

    see ivstmidicontrollers.h

    kCtrlModWheel
    kCtrlFilterResonance (cc74) // Filter Resonance (Brightness)
    kAfterTouch
    kPitchBend

  */

  /*
    todo:
    ParamId, bitmask..
    0x0000.0x0000 'normal' parameter
    0x0001.0x0000 midi
    etc..
  */

  tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id) override {
    VST3_Trace("index %i channel %i ctrlnum %i",busIndex,channel,midiControllerNumber);
    //if (busIndex == 0) {
      switch (midiControllerNumber) {
        case kAfterTouch: // 128
          id = KODE_VST3_PARAM_AFTERTOUCH + channel;
          VST3_DTrace(" -> %i (KODE_VST3_PARAM_AFTERTOUCH+ch)\n",id);
          return kResultOk;
        case kPitchBend: // 129
          id = KODE_VST3_PARAM_PITCHBEND + channel;
          VST3_DTrace(" -> %i (KODE_VST3_PARAM_PITCHBEND+ch)\n",id);
          return kResultOk;
        case kCtrlFilterResonance: // cc 74 (slide)
          id = KODE_VST3_PARAM_BRIGHTNESS + channel;
          VST3_DTrace(" -> %i (KODE_VST3_PARAM_BRIGHTNESS+ch)\n",id);
          return kResultOk;
      }
    //}
    VST3_DTrace(" -> kResultFalse\n");
    return kResultFalse;
  }

  //--------------------
  // INoteExpressionController
  //--------------------

      // Returns number of supported note change types for event bus index and channel.

  /*
  int32 PLUGIN_API getNoteExpressionCount(int32 busIndex, int16 channel) override {
    VST3_Trace("vst3: instance/getNoteExpressionCount busIndex:%i channel:%i\n",busIndex,channel);
    //if (busIndex==0) return 1;
    return 2;
  }

  //----------

  //flags: NoteExpressionTypeInfo::kIsBipolar, kIsOneShot, kIsAbsolute, kAssociatedParameterIDValid

  tresult PLUGIN_API getNoteExpressionInfo(int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info) override {
    VST3_Trace("vst3: instance/getNoteExpressionInfo busIndex:%i channel:%i noteExpressionIndex:%i\n",busIndex,channel,noteExpressionIndex);
    //if (busIndex==0) {
      switch(noteExpressionIndex) {
        case 0:
          info.typeId                 = 0; // ??
          char_to_utf16("Tuning",info.title);
          char_to_utf16("Tun",info.shortTitle);
          char_to_utf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = NoteExpressionTypeInfo::kIsBipolar;
          return kResultOk;
        case 1:
          info.typeId                 = 1; // ??
          char_to_utf16("Brightness",info.title);
          char_to_utf16("Bri",info.shortTitle);
          char_to_utf16("",info.units);
          info.unitId                 = 0;
          info.valueDesc.defaultValue = 0.0;
          info.valueDesc.minimum      = 0;
          info.valueDesc.maximum      = 1;
          info.associatedParameterId  = -1;
          info.flags                  = 0;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //----------

  // Gets a user readable representation of the normalized note change value.

  tresult PLUGIN_API getNoteExpressionStringByValue(int32 busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized, String128 string) override {
    VST3_Trace("vst3: instance/getNoteExpressionStringByValue busIndex:%i channel:%i id:%i valueNormalized:%.3f\n",busIndex,channel,id,valueNormalized);
    char temp[100];
    KODE_FloatToString(temp,valueNormalized);
    char_to_utf16(temp,string);
    return kResultOk;
  }

  //----------

  // Converts the user readable representation to the normalized note change value.

  tresult PLUGIN_API getNoteExpressionValueByString(int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string, NoteExpressionValue& valueNormalized) override{
    VST3_Trace("vst3: instance/getNoteExpressionValueByString busIndex:%i  channel:%i id:%i string:%s\n",busIndex,channel,id,string);
    char temp[129];
    utf16_to_char(string,temp);
    float res = const char*ToFloat(temp);
    valueNormalized = res;
    return kResultOk;
  }

  */

  //--------------------
  // IKeyswitchController
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/keyswitch.html

  /*
    When a (instrument) Plugin supports such interface, the host could get
    from the Plug-in the current set of used key switches (megatrig/
    articulation) for a given channel of a event bus and then automatically use
    them (like in Cubase 6) to create VST Expression Map (allowing to
    associated symbol to a given articulation / key switch).
  */

  //----------

  // Returns number of supported key switches for event bus index and channel.

  int32 PLUGIN_API getKeyswitchCount(int32 busIndex, int16 channel) override {
    VST3_Trace("index %i channel %i\n",busIndex,channel);
    return 0;
  }

  //----------

  /*
  struct KeyswitchInfo {
    KeyswitchTypeID typeId; // see KeyswitchTypeID
    String128 title;        // name of key switch (e.g. "Accentuation")
    String128 shortTitle;   // short title (e.g. "Acc")
    int32 keyswitchMin;     // associated main key switch min (value between [0, 127])
    int32 keyswitchMax;     // associated main key switch max (value between [0, 127])
    int32 keyRemapped;      // optional remapped key switch (default -1), the Plug-in could provide one remapped key for a key switch (allowing better location on the keyboard of the key switches)
    int32 unitId;           // id of unit this key switch belongs to (see \ref vst3UnitsIntro), -1 means no unit used.
    int32 flags;            // not yet used (set to 0)
  };
  */

  // Returns key switch info.

  tresult PLUGIN_API getKeyswitchInfo(int32 busIndex, int16 channel, int32 keySwitchIndex, KeyswitchInfo& info) override {
    VST3_Trace("index %i channel %i keyswitch %i\n",busIndex,channel,keySwitchIndex);
    return kResultFalse;
  }

  //--------------------
  // IConnectionPoint
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/index.html#vst3Communication

  /*
    This interface is used for the communication of separate components.
    Note that some hosts will place a proxy object between the components so
    that they are not directly connected.
  */

  //----------

  // Connects this instance with another connection point.

  /*
    class IConnectionPoint: public FUnknown {
    public:
      virtual tresult PLUGIN_API connect (IConnectionPoint* other) = 0;
      virtual tresult PLUGIN_API disconnect (IConnectionPoint* other) = 0;
      virtual tresult PLUGIN_API notify (IMessage* message) = 0;
      static const FUID iid;
    };
  */

  tresult PLUGIN_API connect(IConnectionPoint* other) override {
    //KODE_Trace("other %p\n",other);
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    VST3_Trace("other %p\n",other);
    return kResultFalse;
  }

  //----------

  //Disconnects a given connection point from this.

  tresult PLUGIN_API disconnect(IConnectionPoint* other)  override {
    //KODE_Trace("other %p\n",other);
    VST3_Trace("other %p\n",other);
    return kResultFalse;
  }

  //----------

  // Called when a message has been sent from the connection point to this.

  /*
  class IMessage: public FUnknown {
  public:
    virtual FIDString PLUGIN_API getMessageID () = 0;
    virtual void PLUGIN_API setMessageID (FIDString id) = 0;
    virtual IAttributeList* PLUGIN_API getAttributes () = 0;
    static const FUID iid;
  };
  */

  tresult PLUGIN_API notify(IMessage* message) override {
    //KODE_Trace("message %p\n",message);
    VST3_TRACE;
    return kResultFalse;
  }

  //--------------------
  // IUnitInfo
  //--------------------

  // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Units.html

  /*
    IUnitInfo describes the internal structure of the Plug-in.
    - The root unit is the component itself, so getUnitCount must return 1 at least.
    - The root unit id has to be 0 (kRootUnitId).
    - Each unit can reference one program list - this reference must not change.
    - Each unit using a program list, references one program of the list.
  */

  //----------

  // Returns the flat count of units.

  int32 PLUGIN_API getUnitCount() override {
    VST3_Trace("-> 1\n");
    return 1;
  }

  //----------

  // Gets UnitInfo for a given index in the flat list of unit.

  /*
  struct UnitInfo {
    UnitID id;                    // unit identifier
    UnitID parentUnitId;          // identifier of parent unit (kNoParentUnitId: does not apply, this unit is the root)
    String128 name;               // name, optional for the root component, required otherwise
    ProgramListID programListId;  // id of program list used in unit (kNoProgramListId = no programs used in this unit)
  };
  */

  tresult PLUGIN_API getUnitInfo(int32 unitIndex, UnitInfo& info) override {
    VST3_Trace("index %i\n",unitIndex);
    if (unitIndex==0) {
      info.id = kRootUnitId; //0;
      info.parentUnitId = kNoParentUnitId;
      char_to_utf16("root",info.name);
      info.programListId = kNoProgramListId; // 0
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets the count of Program List.

  int32 PLUGIN_API getProgramListCount() override {
    VST3_Trace("-> 0\n");
    return 0; // 1
  }

  //----------

  // Gets for a given index the Program List Info.

  /*
  struct ProgramListInfo {
    ProgramListID id;     // program list identifier
    String128 name;       // name of program list
    int32 programCount;   // number of programs in this list
  };
  */

  tresult PLUGIN_API getProgramListInfo(int32 listIndex, ProgramListInfo& info) override {
    VST3_Trace("index %i\n",listIndex);
    if (listIndex == 0) {
      info.id = 0;
      char_to_utf16("program",info.name);
      info.programCount = 1;
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  // Gets for a given program list ID and program index its program name.

  tresult PLUGIN_API getProgramName(ProgramListID listId, int32 programIndex, String128 name) override {
    VST3_Trace("list %i program %i\n",listId,programIndex);
    if ((listId == 0) && (programIndex == 0)) {
      char_to_utf16("program",name);
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Gets for a given program list ID, program index and attributeId the
    associated attribute value.
  */

  //CString attributeId, String128 attributeValue) {
  tresult PLUGIN_API getProgramInfo(ProgramListID listId, int32 programIndex, Steinberg::Vst::CString attributeId, String128 attributeValue) override {
    VST3_Trace("list %i program %i\n",listId,programIndex);
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  char_to_utf16("",attributeValue);
    //  return kResultOk;
    //}
    return kResultFalse;
  }

  //----------

  // Returns kResultTrue if the given program index of a given program list ID supports PitchNames.

  tresult PLUGIN_API hasProgramPitchNames(ProgramListID listId, int32 programIndex) override {
    VST3_Trace("list %i program %i\n",listId,programIndex);
    return kResultFalse;
  }

  //----------

  /*
    Gets the PitchName for a given program list ID, program index and pitch.
    If PitchNames are changed the Plug-in should inform the host with
    IUnitHandler::notifyProgramListChange.
  */

  tresult PLUGIN_API getProgramPitchName(ProgramListID listId, int32 programIndex, int16 midiPitch, String128 name) override {
    VST3_Trace("list %i program %i\n",listId,programIndex);
    //char_to_utf16("ptch",name);
    return kResultFalse;
  }

  //----------

  // Gets the current selected unit.

  UnitID PLUGIN_API getSelectedUnit() override {
    VST3_Trace("-> 0\n");
    return 0;
  }

  //----------

  // Sets a new selected unit.

  tresult PLUGIN_API selectUnit(UnitID unitId) override {
    VST3_Trace("unit %i\n",unitId);
    return kResultOk;
  }

  //----------

  /*
    Gets the according unit if there is an unambiguous relation between a
    channel or a bus and a unit.
    This method mainly is intended to find out which unit is related to a
    given MIDI input channel.
  */

  tresult PLUGIN_API getUnitByBus(MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId) override {
    VST3_Trace("type %s dir %s index %i channel %i\n",type_txt[type],dir_txt[dir],busIndex,channel);
    unitId = 0;
    return kResultOk;//False;
  }

  //----------

  /*
    Receives a preset data stream.
    - If the component supports program list data (IProgramListData), the destination of the data
      stream is the program specified by list-Id and program index (first and second parameter)
    - If the component supports unit data (IUnitData), the destination is the unit specified by the first
      parameter - in this case parameter programIndex is < 0).
  */

  tresult PLUGIN_API setUnitProgramData(int32 listOrUnitId, int32 programIndex, IBStream* data) override {
    VST3_Trace("listorunit %i program %i\n",listOrUnitId,programIndex);
    return kResultFalse;
  }

  //--------------------
  // IEditController
  //--------------------

    // file:///WORK/code/VST3_SDK/doc/vstinterfaces/vst3Automation.html

  //----------

  /*
    // https://forum.juce.com/t/automation-issue-in-cubase-nuendo/14566/5
    // Cubase and Nuendo need to inform the host of the current parameter values
    if (AudioProcessor* const pluginInstance = getPluginInstance()) {
      for (int i = 0; i < pluginInstance->getNumParameters(); ++i)
        setParamNormalized((Vst::ParamID) i, (double) pluginInstance->getParameter (i));
    }
    return kResultFalse; // false?
  */

  tresult PLUGIN_API setComponentState(IBStream* state) override {
    VST3_TRACE;
    /*
    // we receive the current state of the component (processor part)
    if (state == nullptr) return kResultFalse;
    // using helper to read the stream
    IBStreamer streamer(state, kLittleEndian);
    // ABSwitchParamID::kAudioSwitch
    float savedParam1 = 0.f;
    if (!streamer.readFloat(savedParam1)) return kResultFalse;
    setParamNormalized(ABSwitchParamID::kAudioSwitch, savedParam1);
    return kResultOk;
    */
    //return kResultOk;//False;

    /*
      https://forum.juce.com/t/possible-demo-plugin-bug/19131/9
      "..which in turn returns kNotImplemented, should you not be returning
      kResultOk here?"
      "That is intended. I can’t remember why but some DAWs needed that."

    */

    /*
    int num_read = 0;
    uint32_t test_int = 0;
    state->read(&test_int,4,&num_read);
    VST3_DTrace("  %i : num_read = %i\n",test_int,num_read);
    */

    //KODE_Trace("todo: update editor\n");
    // MEditor->updateParameterFromHost(param,index,value);
    return kResultOk;
  }

  //----------

  //tresult PLUGIN_API setState(IBStream* state) {
  tresult PLUGIN_API setEditorState(IBStream* state) override {
    VST3_TRACE;
    return kResultOk;
  }

  //----------

  //tresult PLUGIN_API getState(IBStream* state) {
  tresult PLUGIN_API getEditorState(IBStream* state) override {
    VST3_TRACE;
    return kResultOk;
  }

  //----------

  /*
    called repeatedly by bitwig, even if plugin is deactivated
  */

  int32 PLUGIN_API getParameterCount() override {
    VST3_Trace("-> %i\n",MDescriptor->getNumParameters());
    return MDescriptor->getNumParameters();
  }

  //----------

  tresult PLUGIN_API getParameterInfo(int32 paramIndex, ParameterInfo& info) override {
    VST3_Trace("index %i",paramIndex);
    if (paramIndex >= 0) {
      if (paramIndex < (int32)MDescriptor->getNumParameters()) {
        KODE_Parameter* param = MDescriptor->getParameter(paramIndex);
        if (param) {
          VST3_DTrace(" : %s\n",param->getName());
          memcpy(&info,&MParamInfos[paramIndex],sizeof(ParameterInfo));
          return kResultOk;
        }
      } // index < numparams
      else {
        //KODE_int32 p2 = paramIndex - MNumParameters;
        //KODE_Trace("%i\n",p2);
        switch (paramIndex) {
          case kAfterTouch: // 128
            VST3_DTrace(" : kAfterTouch\n");
            //KODE_Trace("kAfterTouch\n");
            break;
          case kPitchBend: // 129
            //KODE_Trace("kPitchBend\n");
            VST3_DTrace(" : kPitchBend\n");
            break;
          case kCtrlFilterResonance: // cc 74 (slide)
            //KODE_Trace("kCtrlFilterResonance\n");
            VST3_DTrace(" : kPitchBend\n");
            break;
        }
        VST3_DTrace(" : ?\n");
        return kResultOk;
      }
    }
    VST3_Trace(" : unknown\n");
    return kResultFalse;
    //return _get_parameterinfo(paramIndex,info);
  }

  //----------

  tresult PLUGIN_API getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string) override {
    VST3_Trace("id %i value %.2f",id,valueNormalized);
    //VST3_Trace("  MDescriptor->getNumParameters() = %i\n",MDescriptor->getNumParameters());
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      char buffer[32]; // ???
      param->getDisplayText(valueNormalized,buffer);
      //printf("  %s\n",buffer);
      char_to_utf16(buffer,string);
      VST3_DTrace(" -> %s\n",buffer);
      return kResultOk;
    }
    else {
      VST3_DTrace(" -> kResultFalse\n");
      return kResultFalse;
    }
  }

  //----------

  tresult PLUGIN_API getParamValueByString(ParamID id, TChar* string, ParamValue& valueNormalized) override {
    VST3_Trace("id %i",id);
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      char temp[129];
      utf16_to_char(string,temp);
      float v = atoi(temp); //const char*ToFloat(temp);
      float v2 = param->to01(v);
      valueNormalized = v2;
      VST3_DTrace(" -> %.3f\n",valueNormalized);
      return kResultOk;
    }
    else {
      VST3_DTrace(" -> kResultFalse\n");
      return kResultFalse;
    }
  }

  //----------

  ParamValue PLUGIN_API normalizedParamToPlain(ParamID id, ParamValue valueNormalized) override {
    VST3_Trace("id %i value %.2f",id,valueNormalized);
    if (id < MDescriptor->getNumParameters()) {
      //VST3_Trace("vst3: instance/normalizedParamToPlain id:%i value:%.2f\n",id,valueNormalized);
      KODE_Parameter* param = MDescriptor->getParameter(id);
      float v = param->from01(valueNormalized);
      VST3_DTrace(" -> %.3f\n",v);
      return v;
    }
    else {
      VST3_DTrace(" -> %.3f (valueNormalized\n",valueNormalized);
      return valueNormalized;
    }
  }

  //----------

  ParamValue PLUGIN_API plainParamToNormalized(ParamID id, ParamValue plainValue) override {
    VST3_Trace("id %i value %.2f",id,plainValue);
    if (id < MDescriptor->getNumParameters()) {
      KODE_Parameter* param = MDescriptor->getParameter(id);
      float v = param->to01(plainValue);
      VST3_DTrace(" -> %.3f\n",v);
      return v;
    }
    else {
      VST3_DTrace(" -> %.3f (plainValue\n",plainValue);
      return plainValue;
    }
  }

  //----------

  ParamValue PLUGIN_API getParamNormalized(ParamID id) override {
    VST3_Trace("id %i",id);
    if (id < MDescriptor->getNumParameters()) {
      float v = MInstance->getEditorValue(id);
      VST3_DTrace(" -> %.3f\n",v);
      return v;
    }
    else {
      VST3_DTrace(" -> 0\n");
      return 0;
    }
  }

  //----------

  /*
    The host is responsible for updating the Plug-in GUI when automation data
    is transmitted to the processor. This is realized by frequent calls of
    Steinberg::Vst::IEditController::setParamNormalized.
  */

  /*
    Sets the normalized value to the parameter associated to the paramID.
    The controller must never pass this value-change back to the host via the
    IComponentHandler. It should update the according GUI element(s) only!
  */

  tresult PLUGIN_API setParamNormalized(ParamID id, ParamValue value) override {
    VST3_Trace("id %i value %.2f\n",id,value);
    MInstance->setEditorValue(id,value);
    if (MEditor) {
      MEditor->setParameterValue(id,value);
      MEditor->redrawParameter(id);
    }
    return kResultOk;
  }

  //----------

  /*
    Gets from host a handler.
    aka host.. beginEdit, performEdit, endEdit, restartComponent
  */

  /*
  class IComponentHandler: public FUnknown {
  public:
    virtual tresult PLUGIN_API beginEdit (ParamID id) = 0;
    virtual tresult PLUGIN_API performEdit (ParamID id, ParamValue valueNormalized) = 0;
    virtual tresult PLUGIN_API endEdit (ParamID id) = 0;
    virtual tresult PLUGIN_API restartComponent (int32 flags) = 0;
    static const FUID iid;
  };

  notifyHostParameterChanged
    MComponentHandler->begin/perform/endEdit

  */

  tresult PLUGIN_API setComponentHandler(IComponentHandler* handler) override {
    VST3_Trace("handler %p\n",handler);
    KODE_Trace("component handler %p\n",handler);
    MComponentHandler = handler;
    //handler->queryInterface(IComponentHandler2::iid,(void**)&MComponentHandler2);
    KODE_Vst3Host* host = (KODE_Vst3Host*)MInstance->getHost();
    if (host) host->setComponentHandler(MComponentHandler,MComponentHandler2);
    return kResultOk;
  }

  //----------

  /*
    Creates the editor view of the Plug-in, currently only "editor" is
    supported, see ViewType.
    The life time of the editor view will never exceed the life time of
    this controller instance.
  */

  IPlugView* PLUGIN_API createView(FIDString name) override {
    VST3_Trace("name %s\n",name);
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      if (name && (strcmp(name,ViewType::kEditor) == 0)) {
        addRef();
        return (IPlugView*)this;
      }
    }
    return nullptr;
  }

  //--------------------
  // IEditController2
  //--------------------

  tresult PLUGIN_API setKnobMode(KnobMode mode) override {
    VST3_Trace("mode %i\n",mode);
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API openHelp(TBool onlyCheck) override {
    VST3_Trace("onlycheck %i\n",onlyCheck);
    return kResultFalse;
  }

  //----------

  tresult PLUGIN_API openAboutBox(TBool onlyCheck) override {
    VST3_Trace("onlycheck %i\n",onlyCheck);
    return kResultFalse;
  }

  //--------------------
  // IPlugView
  //--------------------

  tresult PLUGIN_API isPlatformTypeSupported(FIDString type) override {
    VST3_Trace("type %s\n",type);
    //VST3_Trace("vst3: instance/isPlatformTypeSupported\n");
    // "X11EmbedWindowID"
    if (type && strcmp(type,kPlatformTypeX11EmbedWindowID) == 0) {
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    The parent window of the view has been created, the (platform)
    representation of the view should now be created as well.

    Note that the parent is owned by the caller and you are not allowed
    to alter it in any way other than adding your own views. Note that
    in this call the Plug-in could call a IPlugFrame::resizeView ()!

    Parameters
      parent  : platform handle of the parent window or view
      type    : Platform UI Types which should be created
  */

  tresult PLUGIN_API attached(void* parent, FIDString type) override {
    VST3_Trace("type %s\n",type);
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      if (MPlugFrame) {
        uint32_t w = MDescriptor->getEditorWidth();
        uint32_t h = MDescriptor->getEditorHeight();
        if (w == 0) w = MInstance->getDefaultEditorWidth();
        if (h == 0) h = MInstance->getDefaultEditorHeight();
        ViewRect r;
        r.left    = 0;
        r.top     = 0;
        r.right   = w;
        r.bottom  = h;
        MPlugFrame->resizeView(this,&r);
      }
      MEditor = (KODE_Editor*)MInstance->on_openEditor(parent);
      MInstance->copyParameterValuesToEditor(MEditor);
      //MEditor->on_realign(true);
      MEditor->open();
      MRunLoop->registerTimer(this,KODE_PLUGIN_VST3_TIMER_MS);
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    The parent window of the view is about to be destroyed.
    You have to remove all your own views from the parent window or view.
  */

  tresult PLUGIN_API removed() override {
    VST3_TRACE;
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      MRunLoop->unregisterTimer(this);
      MEditor->close();
      MInstance->on_closeEditor(/*editor*/);
      MEditor = nullptr;
      return kResultOk;
    }
    //return kResultFalse;
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onWheel(float distance) override {
    VST3_Trace("distance %.2f\n",distance);
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onKeyDown(char16 key, int16 keyCode, int16 modifiers) override {
    VST3_TRACE;
    return kResultOk;
  }

  //----------

  tresult PLUGIN_API onKeyUp(char16 key, int16 keyCode, int16 modifiers) override {
    VST3_TRACE;
    return kResultOk;
  }

  //----------

  /*
    Returns the size of the platform representation of the view.
  */

  tresult PLUGIN_API getSize(ViewRect* size) override {
    VST3_Trace("-> width %i height %i\n",MDescriptor->getEditorWidth(),MDescriptor->getEditorHeight());
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      size->left    = 0;
      size->top     = 0;
      size->right   = MDescriptor->getEditorWidth();
      size->bottom  = MDescriptor->getEditorHeight();
      return kResultOk;
    }
    return kResultFalse;
  }

  //----------

  /*
    Resizes the platform representation of the view to the given rect.
    Note that if the Plug-in requests a resize (IPlugFrame::resizeView())
    onSize has to be called afterward.
  */

  tresult PLUGIN_API onSize(ViewRect* newSize) override {
    VST3_Trace("width %i height %i\n",newSize->getWidth(),newSize->getHeight());
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
    //  MEditorWidth = newSize->getWidth();
    //  MEditorHeight = newSize->getHeight();
    //  //TODO: resize/redraw editor
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Focus changed message.
  */

  tresult PLUGIN_API onFocus(TBool state) override {
    VST3_Trace("state %i\n",state);
    return kResultOk;
  }

  //----------

  /*
    Sets IPlugFrame object to allow the Plug-in to inform the host
    about resizing.
  */

  tresult PLUGIN_API setFrame(IPlugFrame* frame) override {
    VST3_TRACE;
    if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
      MPlugFrame = frame;
      //tresult res =
      MPlugFrame->queryInterface(IRunLoop_iid, (void**)&MRunLoop);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  //----------

  /*
    Is view sizable by user?
  */

  tresult PLUGIN_API canResize() override {
    VST3_Trace("-> false\n");
    return kResultFalse;
  }

  //----------

  /*
    On live resize this is called to check if the view can be resized to the
    given rect, if not adjust the rect to the allowed size.
  */

  tresult PLUGIN_API checkSizeConstraint(ViewRect* rect) override {
    VST3_Trace("-> ok\n");
    return kResultOk;//False;
  }

  //--------------------
  // ITimerHandler
  //--------------------

  // tid = same as setComponentHandler..

  void onTimer() override {
    //MInstance->on_timer();
    KODE_Assert(MHost);
    //MInstance->updateEditorInIdle();
    MInstance->on_idleEditor();
    MHost->notifyHostAboutUpdatedParameters();

    //if (MDescriptor->hasFlag(KODE_PLUGIN_HAS_EDITOR)) {
    //  if (MIsEditorOpen) {
    //    KODE_Assert(MEditor);
    //    MInstance->updateEditorInIdle(MEditor);
    //  }
    //}


  }

};

//----------------------------------------------------------------------
#endif
