#ifndef kode_vst3_host_included
#define kode_vst3_host_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_host.h"
#include "plugin/vst3/kode_vst3.h"

//----------------------------------------------------------------------

class KODE_Vst3Host
: public KODE_Host {

  typedef KODE_Queue<uint32_t,KODE_PLUGIN_MAX_PROCESS_EVENTS> KODE_ParamQueue;

//------------------------------
private:
//------------------------------

  IComponentHandler*  MComponentHandler       = nullptr;
  IComponentHandler2* MComponentHandler2      = nullptr;
  float*              MParameterHostValues    = nullptr;
  KODE_ParamQueue     MUpdatedHostParameters;

//------------------------------
public:
//------------------------------

  KODE_Vst3Host(KODE_Descriptor* ADescriptor)
  : KODE_Host(ADescriptor) {
    MFormat = KODE_PLUGIN_FORMAT_VST3;
    uint32_t num = ADescriptor->getNumParameters();
    uint32_t size = num * sizeof (float*);
    MParameterHostValues = (float*)malloc(size);
  }

  //----------

  virtual ~KODE_Vst3Host() {
    if (MParameterHostValues) free(MParameterHostValues);
  }

//------------------------------
public:
//------------------------------

  void setComponentHandler(IComponentHandler* ch, IComponentHandler2* ch2=nullptr) {
    //KODE_Trace("pid %i tid %i\n",getProcessID(),getThreadID());
    MComponentHandler = ch;
    MComponentHandler2 = ch2;
  }

//------------------------------
public:
//------------------------------

  //void updateSync() override {
  //}

  //----------

  void sendMidi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) override {
  }

  //----------

  void flushMidi() override {
  }

  //----------

  void resizeEditor(uint32_t AWidth, uint32_t AHeight) override {
  }

  //----------

  /*
    we must call the MComponentHandler from the same thread as we received it,
    so we write to a queue, and read them all in updateParametersInTimer(),
    which is called via IRunLoop (same thread as setComponentHandler)
    see updateParametersInTimer below
  */

  void updateParameter(uint32_t AIndex, float AValue) override {
    //KODE_Trace("%i %.2f\n",AIndex,AValue);
    MParameterHostValues[AIndex] = AValue;
    MUpdatedHostParameters.write(AIndex);
  }

  //----------

  /*
    enum RestartFlags {
      kReloadComponent            // The Component should be reloaded
      kIoChanged                  // Input and/or Output Bus configuration has changed
      kParamValuesChanged         // Multiple parameter values have changed (as result of a program change for example)
      kLatencyChanged             // Latency has changed (IAudioProcessor.getLatencySamples)
      kParamTitlesChanged         // Parameter titles or default values or flags have changed
      kMidiCCAssignmentChanged    // MIDI Controller Assignments have changed
      kNoteExpressionChanged      // Note Expression has changed (info, count...)
      kIoTitlesChanged            // Input and/or Output bus titles have changed
      kPrefetchableSupportChanged // Prefetch support has changed (\see IPrefetchableSupport)
      kRoutingInfoChanged         // RoutingInfo has changed (\see IComponent)
    };
    MComponentHandler->restartComponent(int32 flags)
  */

  /*
    https://github.com/soundradix/JUCE/commit/2e9e66cbc8c65e889be5232ffae83c0ca78f9c7e
    performEdit ((Vst::ParamID) index, (double) newValue);
    // call setParamNormalized too, as without it value will reset at endEdit in Cubase.
    // setParamNormalized does not replace performEdit as it does not record automation.
    setParamNormalized ((Vst::ParamID) index, (double) newValue);

    https://sdk.steinberg.net/viewtopic.php?t=693
    "Remember that everything in the edit controller domain must happen on the
    main thread also calls to the IComponentHandler instance of the host. So
    don't call beginEdit, endEdit or performEdit on a secondary thread."

    NB: Cubase has problems if performEdit is called without setParamNormalized
    EditController::setParamNormalized(AIndex,AValue);

    MComponentHandler->performEdit(AIndex,AValue); // drag

  */

  void notifyHostAboutUpdatedParameters() {
    uint32_t index;
    while (MUpdatedHostParameters.read(&index)) {
      float value = MParameterHostValues[index];
      //KODE_Trace("%i %.2f\n",index,value);
      if (MComponentHandler) {
        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
        MComponentHandler->beginEdit(index);          // click
        MComponentHandler->performEdit(index,value);  // drag
        MComponentHandler->endEdit(index);            // release
        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
      }
    }
  }

};

//----------------------------------------------------------------------
#endif
