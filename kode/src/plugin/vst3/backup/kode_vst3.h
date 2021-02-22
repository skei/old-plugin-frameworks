#ifndef kode_vst3_included
#define kode_vst3_included
//----------------------------------------------------------------------

#include "base/kode.h"

#define KODE_VST3_PARAM_AFTERTOUCH  0x10000 // kAfterTouch (128)
#define KODE_VST3_PARAM_PITCHBEND   0x20000 // kPitchBend (129)
#define KODE_VST3_PARAM_BRIGHTNESS  0x30000 // kCtrlFilterResonance (74)

//----------------------------------------------------------------------

#ifdef KODE_DEBUG_VST3
  #define VST3_Trace    KODE_Trace
  #define VST3_DTrace   KODE_DTrace
  #define VST3_TRACE    KODE_TRACE
#else
  #define VST3_Trace    KODE_NoTrace
  #define VST3_DTrace   KODE_NoTrace
  #define VST3_TRACE    {}
#endif

#ifdef KODE_DEBUG_VST3_RT
  #define VST3_Trace_RT  KODE_Trace
  #define VST3_DTrace_RT KODE_DTrace
  #define VST3_TRACE_RT  KODE_TRACE
#else
  #define VST3_Trace_RT  KODE_NoTrace
  #define VST3_DTrace_RT KODE_NoTrace
  #define VST3_TRACE_RT  {}
#endif

//----------

//#include "pluginterfaces/base/conststringtable.h"
//#include "pluginterfaces/base/ustring.h"

//#include "pluginterfaces/base/conststringtable.cpp"
#include "pluginterfaces/base/coreiids.cpp"
//#include "pluginterfaces/base/funknown.cpp"
#include "pluginterfaces/base/ustring.cpp"

#include "pluginterfaces/base/ipluginbase.h"
#include "pluginterfaces/gui/iplugview.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstcomponent.h"

// https://github.com/phil-monroe/midiplug/blob/master/SDKs/vst3/public.sdk/source/vst/vstsinglecomponenteffect.h
// work around for the name clash of IComponent::setState and IEditController::setState

#define setState setEditorState
#define getState getEditorState
#include "pluginterfaces/vst/ivsteditcontroller.h"
#undef setState
#undef getState

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivsthostapplication.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstunits.h"

//----------

using namespace Steinberg;
using namespace Steinberg::Linux;
using namespace Vst;

//----------------------------------------------------------------------
#endif
