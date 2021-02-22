#ifndef s3_plugin_vst_included
#define s3_plugin_vst_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

//#ifdef S3_DEBUG_PROCESS
//#include "common/s3_lock.h"
//#endif

#include "plugin/s3_plugin_base.h"
#include "extern/vst/aeffectx.h"

#ifdef S3_DEBUG_VST
  #include "plugin/s3_plugin_vst_dispatcher_opcodes.h"
#endif

//----------

#ifdef S3_DEBUG_VST
  #define VST_TRACE S3_DTrace
#else
  #define VST_TRACE(x) {}
#endif

//----------

//class S3_Plugin_Vst;
//typedef S3_Plugin_Vst S3_Plugin_Implementation;

struct S3_VstEvents {
  VstInt32  numEvents;
  VstIntPtr reserved;
  VstEvent* events[S3_PLUGIN_MAX_MIDI_SEND];
};

//----------

class S3_Plugin_Vst
: public S3_Plugin_Base {
  private: // vst
    audioMasterCallback MMaster;
    AEffect             MAEffect;
    S3_VstEvents        MVstEvents;
    VstMidiEvent        MMidiSendEvents[S3_PLUGIN_MAX_MIDI_SEND]; //VstEvent;
    ERect               MVstRect;
    //#ifdef S3_DEBUG_PROCESS
    //volatile bool       MInProcess;
    //#endif
  public:
    S3_Plugin_Vst();
    virtual ~S3_Plugin_Vst();
  public:
    // vst implementations
    /*virtual*/ void* main(void* AMaster) final;
    /*virtual*/ void  updateHostInfo(void) final;
    /*virtual*/ void  updateHostTime(void) final;
    /*virtual*/ void  sendMidiToHost(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) final;
    /*virtual*/ void  notifyHostParameterChanged(int32 AIndex, float AValue) final;
    /*virtual*/ void  resizeHostWindow(int32 AWidth, int32 AHeight) final;
  private:
    void          flushMidi(void);
  public:
    // called by callbacks
    void          vst_setParameter(int32 index, float value);
    float         vst_getParameter(int32 index);
    void          vst_process(float** inputs, float** outputs, int32 sampleFrames);
    void          vst_processDouble(double** inputs, double** outputs, int32 sampleFrames);
    VstIntPtr     vst_dispatcher(int32 opcode, int32 index, intptr value, void* ptr, float opt);
  private:
    void          host_automate(VstInt32 AIndex, float AValue);
    bool          host_beginEdit(VstInt32 AIndex);
    bool          host_canDo(const char* AText);
    VstInt32      host_currentId(void);
    bool          host_endEdit(VstInt32 AIndex);
    bool          host_getProductString(const char* ABuffer);
    float         host_getSampleRate(void);
    VstTimeInfo*  host_getTime(VstInt32 AFilter);
    bool          host_getVendorString(const char* ABuffer);
    VstInt32      host_getVendorVersion(void);
    void          host_idle(void);
    bool          host_ioChanged(void);
    bool          host_processEvents(VstEvents* events);
    bool          host_sizeWindow(int32 AWidth, int32 AHeight);
    VstInt32      host_version(void);
};

//----------

VstIntPtr vst_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);
void      vst_setParameter_callback(AEffect* effect, VstInt32 index, float parameter);
float     vst_getParameter_callback(AEffect* effect, VstInt32 index);
void      vst_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames);
void      vst_processDouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Plugin_Vst::S3_Plugin_Vst()
: S3_Plugin_Base() {
  MMaster = S3_NULL;
  S3_Memset(&MAEffect,0,sizeof(AEffect));
  S3_Memset(&MVstEvents,0,sizeof(S3_VstEvents));
  S3_Memset(&MMidiSendEvents,0,sizeof(MMidiSendEvents));
  S3_Memset(&MVstRect,0,sizeof(ERect));
  for (uint32 i=0; i<S3_PLUGIN_MAX_MIDI_SEND; i++) MVstEvents.events[i] = (VstEvent*)&MMidiSendEvents[i];
  //#ifdef S3_DEBUG_PROCESS
  //MInProcess = false;
  //#endif
}

//----------

S3_Plugin_Vst::~S3_Plugin_Vst() {
}

//----------------------------------------------------------------------
// private
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

void* S3_Plugin_Vst::main(void* AMaster) {
  setDefaultParamValues();
  MMaster = (audioMasterCallback)AMaster;
  int32 flags = effFlagsCanReplacing;
  #ifdef S3_PLUGIN_SYNTH
  flags |= effFlagsIsSynth;
  #endif
  #ifdef S3_PLUGIN_EDITOR
  flags |= effFlagsHasEditor;
  #endif
  #ifdef S3_PLUGIN_CHUNKS
  flags |= effFlagsProgramChunks;
  #endif
  #ifdef S3_PLUGIN_SILENT_STOP
  flags |= effFlagsNoSoundInStop;
  #endif
  #ifdef S3_PLUGIN_DOUBLE
  flags |= effFlagsCanDoubleReplacing;
  #endif
  //S3_Memset(&MAEffect,0,sizeof(AEffect));
  MAEffect.magic                    = kEffectMagic; //'VstP';
	MAEffect.uniqueID                 = MUniqueId;
	MAEffect.flags                    = flags;
	MAEffect.version                  = MVersion;
	MAEffect.numInputs                = MNumInputs;//2; // MAudioInputs.size();
	MAEffect.numOutputs               = MNumOutputs;//2; // MAudioOutputs.size();
	MAEffect.numParams                = MParameters.size();
	MAEffect.numPrograms              = MPrograms.size();
	MAEffect.initialDelay             = MLatency;
	MAEffect.object                   = this;
	MAEffect.user                     = S3_NULL;
  MAEffect.dispatcher               = vst_dispatcher_callback;
	MAEffect.setParameter             = vst_setParameter_callback;
	MAEffect.getParameter             = vst_getParameter_callback;
	MAEffect.processReplacing         = vst_process_callback;
	MAEffect.processDoubleReplacing   = vst_processDouble_callback;
  return &MAEffect;
}

//----------------------------------------------------------------------

/*
  TODO:
  - iterate through table, with cando-string and flag
*/

void S3_Plugin_Vst::updateHostInfo(void) {
  MHostCanDo = s3_hc_none;
  if (host_canDo("sendVstEvents"))                  MHostCanDo += s3_hc_sendVstEvents;
  if (host_canDo("sendVstMidiEvent"))               MHostCanDo += s3_hc_sendVstMidiEvent;
  if (host_canDo("sendVstTimeInfo"))                MHostCanDo += s3_hc_sendVstTimeInfo;
  if (host_canDo("receiveVstEvents"))               MHostCanDo += s3_hc_receiveVstEvents;
  if (host_canDo("receiveVstMidiEvent"))            MHostCanDo += s3_hc_receiveVstMidiEvent;
  if (host_canDo("reportConnectionChanges"))        MHostCanDo += s3_hc_reportConnectionChanges;
  if (host_canDo("acceptIOChanges"))                MHostCanDo += s3_hc_acceptIOChanges;
  if (host_canDo("sizeWindow"))                     MHostCanDo += s3_hc_sizeWindow;
  if (host_canDo("offline"))                        MHostCanDo += s3_hc_offline;
  if (host_canDo("openFileSelector"))               MHostCanDo += s3_hc_openFileSelector;
  if (host_canDo("closeFileSelector"))              MHostCanDo += s3_hc_closeFileSelector;
  if (host_canDo("startStopProcess"))               MHostCanDo += s3_hc_startStopProcess;
  if (host_canDo("shellCategory"))                  MHostCanDo += s3_hc_shellCategory;
  if (host_canDo("sendVstMidiEventFlagIsRealtime")) MHostCanDo += s3_hc_sendVstMidiEventFlagIsRealtime;
  MHostId = s3_hi_unknown;
  char buffer[256];
  host_getProductString(buffer);
  //TODO: iterate through table, with hostname-string and id
  if (S3_Strcmp("REAPER",buffer) == 0)              MHostId = s3_hi_reaper;
  if (S3_Strcmp("energyXT",buffer) == 0)            MHostId = s3_hi_energyxt;
  if (S3_Strcmp("Live",buffer) == 0)                MHostId = s3_hi_live;
  if (S3_Strcmp("Fruity Wrapper",buffer) == 0)      MHostId = s3_hi_fruity;
  if (S3_Strcmp("Qtractor",buffer) == 0)            MHostId = s3_hi_qtractor;
  if (S3_Strcmp("Carla",buffer) == 0)               MHostId = s3_hi_carla;
  //if (S3_Strcmp("Carla_Discovery",buffer) == 0)     MHostId = s3_hi_carla_discovery; // 'Carla_Discovery' during scanning..
  if (S3_Strcmp("Bitwig Studio",buffer) == 0)       MHostId = s3_hi_bitwig;
  if (S3_Strcmp("Multi Host 4 Linux",buffer) == 0)  MHostId = s3_hi_jost;
  MHostVersion = host_getVendorVersion();
  MHostVstVer = host_version();

  #ifdef S3_DEBUG_VST
    VST_TRACE("  updateHostInfo()\n");
    VST_TRACE("    MHostId:      %i : ",MHostId);
    switch(MHostId) {
      case s3_hi_reaper:    VST_TRACE(" s3_hi_reaper\n");   break;
      case s3_hi_energyxt:  VST_TRACE(" s3_hi_energyxt\n"); break;
      case s3_hi_live:      VST_TRACE(" s3_hi_live\n");     break;
      case s3_hi_fruity:    VST_TRACE(" s3_hi_fruity\n");   break;
      case s3_hi_qtractor:  VST_TRACE(" s3_hi_qtractor\n"); break;
      case s3_hi_carla:     VST_TRACE(" s3_hi_carla\n");    break;
      case s3_hi_bitwig:    VST_TRACE(" s3_hi_bitwig\n");   break;
      case s3_hi_jost:      VST_TRACE(" s3_hi_jost\n");     break;
      default:              VST_TRACE(" s3_hi_unknown\n");  break;
    }
    VST_TRACE("    MHostVersion: %08x (%i)\n",MHostVersion,MHostVersion);
    VST_TRACE("    MHostVstVer:  %08x (%i)\n",MHostVstVer,MHostVstVer);
    VST_TRACE("    MHostCanDo:\n");
    if (MHostCanDo & s3_hc_sendVstEvents)                   VST_TRACE("      s3_hc_sendVstEvents\n");
    if (MHostCanDo & s3_hc_sendVstMidiEvent)                VST_TRACE("      s3_hc_sendVstMidiEvent\n");
    if (MHostCanDo & s3_hc_sendVstTimeInfo)                 VST_TRACE("      s3_hc_sendVstTimeInfo\n");
    if (MHostCanDo & s3_hc_receiveVstEvents)                VST_TRACE("      s3_hc_receiveVstEvents\n");
    if (MHostCanDo & s3_hc_receiveVstMidiEvent)             VST_TRACE("      s3_hc_receiveVstMidiEvent\n");
    if (MHostCanDo & s3_hc_reportConnectionChanges)         VST_TRACE("      s3_hc_reportConnectionChanges\n");
    if (MHostCanDo & s3_hc_acceptIOChanges)                 VST_TRACE("      s3_hc_acceptIOChanges\n");
    if (MHostCanDo & s3_hc_sizeWindow)                      VST_TRACE("      s3_hc_sizeWindow\n");
    if (MHostCanDo & s3_hc_offline)                         VST_TRACE("      s3_hc_offline\n");
    if (MHostCanDo & s3_hc_openFileSelector)                VST_TRACE("      s3_hc_openFileSelector\n");
    if (MHostCanDo & s3_hc_closeFileSelector)               VST_TRACE("      s3_hc_closeFileSelector\n");
    if (MHostCanDo & s3_hc_startStopProcess)                VST_TRACE("      s3_hc_startStopProcess\n");
    if (MHostCanDo & s3_hc_shellCategory)                   VST_TRACE("      s3_hc_shellCategory\n");
    if (MHostCanDo & s3_hc_sendVstMidiEventFlagIsRealtime)  VST_TRACE("      s3_hc_sendVstMidiEventFlagIsRealtime\n");
  #endif

}

//----------

/*
  TODO:
  - check hostCanDo (but some hosts return 0)
*/

void S3_Plugin_Vst::updateHostTime(void) {
  if (MMaster) {
    uint32 flags = kVstPpqPosValid + kVstTempoValid + kVstCyclePosValid;
    VstTimeInfo* ti = host_getTime(flags);
    if (ti) {
      //todo: check flags?
      MPlayState    = ti->flags & 0xff;
      MSamplePos    = ti->samplePos;
      MSampleRate   = ti->sampleRate;
      MBeatPos      = ti->ppqPos;
      MTempo        = ti->tempo;
      MLoopStart    = ti->cycleStartPos;
      MLoopEnd      = ti->cycleEndPos;
    }
  }
}

//----------

/*
  add midi events into array..
  will be sent to the host at the end of process
*/

void S3_Plugin_Vst::sendMidiToHost(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
  //S3_Trace("%i : %i %i %i\n",AOffset,AMsg1,AMsg2,AMsg3);
  int32 num           = MVstEvents.numEvents;
  VstMidiEvent* event = &MMidiSendEvents[num];
  event->type         = kVstMidiType;
  event->deltaFrames  = AOffset;
  event->midiData[0]  = AMsg1;
  event->midiData[1]  = AMsg2;
  event->midiData[2]  = AMsg3;
  event->midiData[3]  = 0;
  event->byteSize     = sizeof(VstMidiEvent) - 2 * sizeof(int32);
  event->flags        = 0; // kVstMidiEventIsRealtime;
  event->noteLength   = 0;
  event->noteOffset   = 0;
  event->detune       = 0;
  num += 1;
  MVstEvents.numEvents = num;
}

//----------

void S3_Plugin_Vst::notifyHostParameterChanged(int32 AIndex, float AValue) {
  host_beginEdit(AIndex);
  host_automate(AIndex,AValue);
  host_endEdit(AIndex);
}

//----------

void S3_Plugin_Vst::resizeHostWindow(int32 AWidth, int32 AHeight) {
  host_sizeWindow(AWidth,AHeight);
}

//----------------------------------------------------------------------
// private
//----------------------------------------------------------------------

void S3_Plugin_Vst::flushMidi(void) {
  if (MVstEvents.numEvents > 0) {
    host_processEvents((VstEvents*)&MVstEvents);
    MVstEvents.numEvents = 0;
  }
}

//----------------------------------------------------------------------
// vst
//----------------------------------------------------------------------

/*
  vst sdk 2.4:
  (http://www.dith.it/listing/vst_stuff/vstsdk2.4/doc/html/sequences.html)

  "In general, processEvents(), startProcess(), stopProcess(), process(),
  processReplacing() and processDoubleReplacing() are called from a
  time-critical high priority thread (except for offline processing).
  The same applies to setParameter() and setProgram() when playing automation
  from host. Any others functions are called from UI thread in general."
*/

void S3_Plugin_Vst::vst_setParameter(int32 index, float value) {

  //#ifdef S3_DEBUG_PROCESS
  //  if (MInProcess==true) S3_Trace("!!!!! setParameter() called in process()\n");
  //#endif

  #ifdef S3_DEBUG_VST
    VST_TRACE("vst_setParameter / index %i value %.3f\n",index,value);
  #endif

  MHostParamValues[index] = value;
  // find better / more descriptive name..
  #ifdef S3_PLUGIN_PARAMS_DIRECT
    S3_Parameter* param = MParameters[index];
    on_parameterChange(index,param->from01(value);
  #else
    #ifdef S3_PLUGIN_PARAMS_PROCESS_QUEUE
      MProcessParamQueue.write(index);
    #endif
    #ifdef S3_PLUGIN_EDITOR
      #ifdef S3_PLUGIN_PARAMS_EDITOR_QUEUE
        if (MEditorIsOpen) MEditorParamQueue.write(index);
      #endif
    #endif
  #endif // S3_PLUGIN_PARAMS_DIRECT
}

//----------

float S3_Plugin_Vst::vst_getParameter(int32 index) {
  float value = MHostParamValues[index];
  #ifdef S3_DEBUG_VST
    VST_TRACE("vst_getParameter / index %i -> %.3f\n",index,value);
  #endif
  return value;
}

//----------------------------------------
// process
//----------------------------------------

/*
  assumption:
  * since midi events are timestamped (in samples) relative to the (upcoming)
    audio block, it doesn't make sense to call this other places than
    inbetween blocks..
    or..
  * could a host try to be smart, and send the midi evenst for the NEXT block
    while we're still processing the previous block?
  * midi events from a separate thread? midi learn?
    kind of "react to this midi as fast as possible"

    on_MidiEvent(s) has already been called from effProcessEvents
  should we handle midi events similar to parameters?
*/

void S3_Plugin_Vst::vst_process(float** inputs, float** outputs, int32 sampleFrames) {

  //#ifdef S3_DEBUG_PROCESS
  //MInProcess = true;
  //#endif

  // parameters
  #ifndef S3_PLUGIN_PARAMS_DIRECT
  updateParametersInProcess();
  #endif

  // sync/time
  #ifdef S3_PLUGIN_AUTOSYNC
    updateHostTime();
  #endif

  // process block
  if (sampleFrames > 0) {
    on_processBlock(inputs,outputs,sampleFrames);
  }

  // process ticks
  #ifdef S3_PLUGIN_TICKS
    //float* out0 = outputs[0];
    //float* out1 = outputs[1];
    int32 remaining = sampleFrames;
    int32 offset = 0;
    //MEvents.preProcess();
    while (remaining>0) {
      if (remaining>=S3_PLUGIN_PROCESS_TICKSIZE) {
        //MEvents.process(offset);
        //processTick();
        //copyBuffer(out0+offset,MTickBuffer);
        //copyBuffer(out1+offset,MTickBuffer);
        remaining -= S3_PLUGIN_PROCESS_TICKSIZE;
        offset += S3_PLUGIN_PROCESS_TICKSIZE;
      }
      else {
        //MEvents.process(offset,remaining);
        //processTick(remaining);
        //copyBuffer(out0+offset,MTickBuffer,remaining);
        //copyBuffer(out1+offset,MTickBuffer,remaining);
        remaining = 0;
      }
    }
    //MEvents.postProcess();
  #endif

  // process samples
  #ifdef S3_PLUGIN_PER_SAMPLE
    float* ins[2];
    float* outs[2];
    ins[0] = inputs[0];
    ins[1] = inputs[1];
    outs[0] = outputs[0];
    outs[1] = outputs[1];
    uint32 len = sampleFrames;
    while (len > 0) {
      on_processSample(ins,outs);
      ins[0]++;
      ins[1]++;
      outs[0]++;
      outs[1]++;
      len--;
    }
  #endif

  // post process
  on_postProcess();

  #ifdef S3_PLUGIN_SEND_MIDI
  flushMidi();
  #endif

  //#ifdef S3_DEBUG_PROCESS
  //MInProcess = false;
  //#endif

}

//----------------------------------------
//
//----------------------------------------

void S3_Plugin_Vst::vst_processDouble(double** inputs, double** outputs, int32 sampleFrames) {
  #ifdef S3_DEBUG_VST
    //VST_TRACE("vst_processDouble / frames %i\n",sampleFrames);
  #endif
}

//----------------------------------------------------------------------

VstIntPtr S3_Plugin_Vst::vst_dispatcher(int32 opcode, int32 index, intptr value, void* ptr, float opt) {

  //#ifdef S3_DEBUG_VST
  //const char* opcode_name = s3_vst_dispatcher_opcode_names[opcode];
  //S3_DTrace("vst_dispatcher / opcode %i (%s) index %i value %i ptr %08x opt %.3f\n",opcode,opcode_name,index,value,ptr,opt);
  //#endif

  //char*   p;
  //uint32  i;

  switch(opcode) {

    /*
      called when plug-in is initialized
      initialise this plugin instance
    */

    case effOpen:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effOpen\n");
      #endif
      MIsOpen = true;
      updateHostInfo();
      on_stateChange(s3_ps_open);
      break;

    /*
      called when plug-in will be released
      exit this plugin instance, release all memory and other resources
    */

    case effClose:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effClose\n");
      #endif
      MIsOpen = false;
      on_stateChange(s3_ps_close);
      //return 1;
      break;

    /*
      host has changed the current program number
      host must call this inside a plugBeginSetProgram/plugEndSetProgram sequence..
      value: program number
    */

    case effSetProgram: {
      uint32 program = (uint32)value;
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetProgram %i\n",program);
      #endif
      if (program != MCurrentProgram) {
        //on_preProgram(MCurrentProgram);
        //MPrograms.saveProgram(MCurrentProgram,&MParameters);
        MCurrentProgram = program;
        //MPrograms.loadProgram(MCurrentProgram,&MParameters);
        on_programChange(MCurrentProgram);
        //MParameters.distributeValues();
        /*
        on_programChange(program);
        MCurrentProgram = program;
        */
      }
      break;
    }

    /*
      host requests the current program number
      return: current program number
      * reaper calls this repeatedly, 4-5 times per second (if editor open?)
      * EnergyXt calls this before/after each effEditIdle (even if we don't have an editor)
    */

    case effGetProgram:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetProgram -> %i\n",MCurrentProgram);
      #endif
      return MCurrentProgram;
      //break;

    /*
      host has changed the current program name
      ptr: pointer to new program name (max 24 char + 0)
      stuff the name field of the current program with name.
      Limited to kVstMaxProgNameLen.
    */

    case effSetProgramName: {
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetProgramName '%s'\n",(char*)ptr);
      #endif
      const char* txt = (const char*)ptr;
      S3_Strcpy(MProgramName,txt);
      break;
    }

    /*
      host requests the current program name
      ptr: returns string-pointer to program name (max 24 char + 0)
      stuff name with the name of the current program.
      Limited to kVstMaxProgNameLen.
    */

    case effGetProgramName:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetProgramName -> '%s'\n",MProgramName);
      #endif
      S3_Strcpy((char*)ptr,MProgramName);
      return 1;
      //break;

    /*
      host requests parameter label
      index: parameter index
      ptr: returns string-pointer to parameter label (max 8 char + 0),
           examples: sec, dB, type
      Stuff label with the units in which parameter index is displayed
      (i.e. "sec", "dB", "type", etc...).
      Limited to kVstMaxParamStrLen.
    */

    case effGetParamLabel: {
      S3_Parameter* pa = MParameters[index];
      S3_Strcpy((char*)ptr,pa->label());
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetParamLabel %i -> '%s'\n",index,pa->label());
      #endif
      return 1;
      //break
    }

    /*
      stuff text with a string representation of the value of parameter index.
      ("0.5", "-3", "PLATE", etc...)
      Limited to kVstMaxParamStrLen.
    */

    case effGetParamDisplay: {
      S3_Parameter* pa = MParameters[index];
      //float v = getParameterValue(index);
      float v = MHostParamValues[index];
      const char* txt = pa->displayText(v);
      S3_Strcpy((char*)ptr,txt);
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetParamDisplay %i -> '%s'\n",index,txt);
      #endif
      return 1;
      //break;
    }

    /*
      stuff text with the name of parameter index.
      ("Time", "Gain", "RoomType", etc...)
      Limited to kVstMaxParamStrLen.
    */

    case effGetParamName: {
      S3_Parameter* pa = MParameters[index];
      S3_Strcpy((char*)ptr,pa->name());
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetParamName %i -> '%s'\n",index,pa->name());
      #endif
      return 1;
      //break
    }

    /*
      called if (flags and (effFlagsHasClip or effFlagsHasVu))
      return: 'VU' value; > 1.0 means clipped
    */

    case DECLARE_VST_DEPRECATED(effGetVu):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetVu (deprecated)\n");
      #endif
      break;

    /*
      called when the sample rate changes
      host sets SampleRate
      opt: float value in Hz, for example 44100.0Hz
      (always in a suspend state).
    */

    case effSetSampleRate:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetSampleRate %.3f\n",opt);
      #endif
      MSampleRate = opt;
      on_stateChange(s3_ps_sampleRate);
      break;

    /*
      called when the maximun block size changes (always in a suspend state).
      note that the sampleFrames in process calls could be smaller than
      this block size, but NOT bigger.
    */

    case effSetBlockSize:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetBlockSize %i\n",value);
      #endif
      MBlockSize = (VstInt32)value;
      on_stateChange(s3_ps_blockSize);
      break;

    /*
      the host has switched the 'power on' button. This only switches audio
      processing on/off.
      value: 0 = OFF, the plugin must call Resume() which in turn must flush
                 any delay buffers etc. here because otherwise pending data
                 would sound again when the effect is switched on next time.
             1 = ON, the plug must call Suspend()

      This method is called when the effect is turned off by the user.
      ex: The buffer gets flushed here because otherwise pending data would
          sound again when the effect is switched on next time.

      called from non-rt thread..

      I believe resume() is called from a non-real-time thread, so you can
      allocate memory and stuff. startProcessing() is called from a real-time
      thread, so it's supposed to be "fast" in each plug-in. Thus, you can do
      things like taking initial timer measurements, spin up hardware buffers
      if needed, etc. startProcessing() gives you a chance to do the "last half"
      of your process() functionality, if you're interfacing with something that
      needs double-buffering, for example.
      -
      Ah, right, now that I checked it, the documentation says startProcess()/
      stopProcess() are supposed to be called in RT thread, and resume()/
      suspend() are not in the list. Ok fine, that makes sense.
      -
      flush event list(s) on resume?

      suspend: called when plug-in is switched to off
      resume:  called when plug-in is switched to on
    */

    case effMainsChanged:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effMainsChanged %i",value);
      #endif
      if (value == 0) { // suspend
        #ifdef S3_DEBUG_VST
          VST_TRACE(" (suspend)\n");
        #endif
        //MIsProcessing = false;
        MIsSuspended = true;
        on_stateChange(s3_ps_suspend);
      }
      else { // resume
        if (!MInitialized) {
          #ifdef S3_DEBUG_VST
            VST_TRACE(" (initialize)\n");
          #endif
          //on_initialize();
          on_stateChange(s3_ps_initialize);
          MInitialized = true;
        }
        #ifdef S3_DEBUG_VST
          VST_TRACE(" (resume)\n");
        #endif
        //MIsProcessing = true;
        MIsSuspended = false;
        on_stateChange(s3_ps_resume);
      }
      break;

    /*
      ptr: pointer to rect*(top, left, bottom, right)
      return: 1 = successful
      called repeatedly by reaper (4-5 times per second)
      NOTE:
        ERect has 16 bit integers.. does it matter?
    */

    case effEditGetRect:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditGetRect\n");
      #endif
      #ifdef S3_PLUGIN_EDITOR
        MVstRect.left     = 0;
        MVstRect.top      = 0;
        MVstRect.right    = MEditorRect.w; // +1
        MVstRect.bottom   = MEditorRect.h; // +1
        *(ERect**)ptr = &MVstRect;
        return 1;
      #endif
      //break;

    /*
      ptr: contains system dependant Window handle/pointer
      return: 1 = successful
      ext2/linux:  display ptr (32-bit?)
    */

    case effEditOpen:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditOpen %08x\n",ptr);
      #endif
      //if (!MEditorIsOpen) {
      //  MEditor = (S3_Editor*)on_openEditor(ptr);
      //  if (MEditor) {
      //    if ((MEditorStateBuffer) && (MEditorStateSize > 0)) {
      //      MEditor->on_editorRestoreState(MEditor,MEditorStateBuffer,MEditorStateSize);
      //    }
      //    MParameters.distributeValues(); // this is called when setting program too...
      //    MEditor->on_widgetAlign(MEditor.editor);
      //    MEditor->on_bufferPaint(MEditor.editor,S3_NULL,s3_pm_normal); // !!!!!!!!!
      //    MEditor->open();
      //    MEditorIsOpen = true;
      //    return 1;
      //  }
      //} // !editor is open
      break;

    /*
    */

    case effEditClose:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditClose\n");
      #endif
      //if (MEditor)  {
      //  if (MEditorIsOpen) {
      //    //
      //    if ((MEditorStateBuffer) && (MEditorStateSize > 0)) {
      //      S3_Free(MEditorStateBuffer);
      //      MEditorStateBuffer = S3_NULL;
      //      MEditorStateSize = 0;
      //    }
      //    MEditorStateSize = MEditor->on_editorGetStateSize(MEditor);
      //    if (MEditorStateSize > 0) {
      //      MEditorStateBuffer = S3_Malloc(MEditorStateSize);
      //      MEditor->on_editorSaveState(MEditor,MEditorStateBuffer,MEditorStateSize);
      //    }
      //    //MEditor.rect = MEditor.editor->rect();  // save editor size
      //    MEditor->close();               // stops event thread
      //    if (!on_closeEditor(MEditor)) {        // deletes window
      //      delete MEditor;
      //    }
      //    MEditorIsOpen = false;
      //    MEditor = S3_NULL;
      //    return 1;
      //  } // editor is open
      //} // editor
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effEditDraw):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditDraw (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effEditMouse):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditMouse (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effEditKey):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditKey (deprecated)\n");
      #endif
      break;

    /*
      no arguments. Be gentle!
      (it would be better if this call could be avoided)
      don't push stuff or changes to editor,
      notify editor that we want to change, and do it in the editor
      NOTE:
      EnergyXt calls this even if we don't have our own editor!
      FL studio --"--
      reaper, also in 'pause' state (when processing is not being called)
    */

    case effEditIdle:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditIdle\n");
      #endif
      //#ifndef S3_LINUX_IDLE_THREAD
      //if (MEditorIsOpen) {
      //  if (MEditor) {
      //    on_idleEditor(MEditor);
      //    updateEditorInIdle();
      //    return 1;
      //  }
      //}
      //#endif // S3_LINUX_IDLE_THREAD
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effEditTop):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditTop (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effEditSleep):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditSleep (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effIdentify):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effIdentify (deprecated) >> 'NvEf'\n");
      #endif
      return CCONST('N','v','E','f');
      break;

    /*
      host requests pointer to a Bank/Program chunk,
      plugin is responsible for memory allocation/de-allocation of chunk
      index: 0 = isBank, 1 = isProgram
      ptr: pointer to chunk memory-block
      return: byteSize

      VstInt32 AudioEffect::getChunk (void** data, bool isPreset)
      data - should point to the newly allocated memory block containg state
      data. You can savely release it in next suspend/resume call.
      isPreset - true when saving a single program, false for all programs
      note: If your plug-in is configured to use chunks (see
      AudioEffect::programsAreChunks)
      the Host will ask for a block of memory describing the current plug-in
      state for saving.
      To restore the state at a later stage, the same data is passed back to
      AudioEffect::setChunk. Alternatively, when not using chunk, the Host
      will simply save all parameter values.

      SONAR is sending the setChunks on separated cores. (dual-core machine here)
      Or on separated threads.
      i think:
      reaper asks for chunk repeatedly (1.5 sec intervals?)
      bitwig per 5 sec ??
      see:
        "proper way to load and save presets with getChunk setChunk"
        https://www.kvraudio.com/forum/viewtopic.php?p=6477202
        "Correctly restoring a VST plugin current program on host saved project"
        https://www.kvraudio.com/forum/viewtopic.php?p=6330607
        "getChunk question"
        http://www.kvraudio.com/forum/viewtopic.php?p=5530705
        sonigen:
        can you just assign the new chunk and delete the previous one in the
        getChunk call? The host is just going to keep one "chunk" pointer
        isnt it? So as long as that points to the last chunk created
        everything is ok?
        very angry mobster:
        I only use the one chunk and have not noticed any problems yet.
        mystran:
        Yeah, same here.. my chunk is simply an std::string and I just
        replace the contents on every new call to getChunk(), then pass the
        data() and length() back to the host. Never heard of any issues
        anywhere.
    */

    case effGetChunk:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetChunk ptr %08x index ",ptr);
        if (index==0) VST_TRACE(" 0 (isBank)",index);
        if (index==1) VST_TRACE(" 1 (isProgram)",index);
        VST_TRACE(" >> 0\n");
      #endif
      //#ifdef S3_PLUGIN_PRESET_BUFFER
      //if (index==0) return on_saveBank((void**)ptr);
      //else /*if (index==1)*/ return on_saveProgram((void**)ptr);
      //#endif
      break;

    /*
      plug-in receives a Bank/Program chunk,
      (host restores plug-in state)
      host is responsible for memory allocation/de-allocation of chunk
      index: 0 = isBank, 1 = isProgram
      value: byteSize
      ptr: pointer to chunk memory-block

      VstInt32 AudioEffect::setChunk (void* data, VstInt32 byteSize, bool isPreset)
      data - pointer to state data (owned by Host)
      byteSize - size of state data
      isPreset - true when restoring a single program, false for all programs
    */

    case effSetChunk:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetChunk ptr %08x index ",ptr);
        if (index==0) VST_TRACE(" 0 (isBank)",index);
        if (index==1) VST_TRACE(" 1 (isProgram)",index);
        VST_TRACE(" byteSize %i\n",value);
      #endif
      //#ifdef S3_PLUGIN_PRESET_BUFFER
      //if (index==0) return on_loadBank(ptr,value); // was not retrurn
      //else {
      //  /*if (index==1)*/ return on_loadProgram(ptr,value);
      //}
      //#endif
      break;

    //case effGetChunk: v = getChunk ((void**)ptr, index ? true : false); break;
    //case effSetChunk: v = setChunk (ptr, (VstInt32)value, index ? true : false); break;

// vst 2

    /*
      ptr: pointer to VstEvents*, host usually call ProcessEvents just before
           calling ProcessReplacing
      return: VST2.4: host should ignore return value, <=VST2.3: 0 = 'wants no
              more', else return=1

      assumptions:
        * called just before process(), from the same thread,
          non-overlapping (non-concurrent?)
        * sorted by offset
      todo:
        * check: negative offsets, offsets past buffer
                 concurrency (what happens IF it is called during process()

      Are you taking deltaFrames offsets into account? (Including the fact that
      they can be negative, indicating the event should have happened in the
      last sample buffer. As will necessarily be the case with any real-time
      MIDI input.)

      called repeatedly! by fl studio, even if nothing happens???
      energyxt calls this repeatedly (with num = 0), before each block?
    */

    case effProcessEvents: {

      //#ifdef S3_DEBUG_PROCESS
      //  if (MInProcess==true) S3_Trace("!!!!! effProcessEvents() called in process()\n");
      //#endif

      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effProcessEvents\n");
      #endif

      #ifdef S3_PLUGIN_RECEIVE_MIDI

        VstEvents* ev = (VstEvents*)ptr;
        int num_events = ev->numEvents;

        #ifdef S3_PLUGIN_PER_MIDI_EVENT
          for (int32 i=0; i<num_events; i++) {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            if (event->type == kVstMidiType) {
              on_midiEvent(event->deltaFrames,event->midiData[0],event->midiData[1],event->midiData[2]);
            }
          }
        #else
          for (int32 i=0; i<num_events; i++) {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            if (event->type == kVstMidiType) {
              MMidiEvents[i].offset  = event->deltaFrames;
              MMidiEvents[i].msg1    = event->midiData[0];
              MMidiEvents[i].msg2    = event->midiData[1];
              MMidiEvents[i].msg3    = event->midiData[2];
            }
          }
          on_midiEvents(MMidiEvents,num_events);
        #endif

      #endif // receive_midi

      // sort?
      return 1;
      //break;
    }

    /*
      hosts asks if parameter can be automated, this should be called each time
      before attempting to call SetParameter()
      index: parameter index
      return: 1 = parameter can be automated
    */

    case effCanBeAutomated: {
      uint32 res = 0;
      if (MParameters[index]->flags() & s3_pf_automate) res = 1;
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effCanBeAutomated %i -> %i\n",index,res);
      #endif
      return res;
      //break;
    }

    /*
      host must call this two-fold:
      1) host checks the capability of plugin if it can convert a string into a parameter float value
          index: parameter index
          ptr: NULL
          return: 1 = plug CAN convert string2Parameter,
                  0 = plug CANNOT convert string2Parameter
      2) host requests plug to set a parameter from the parameter textual representation into a parameter
         float value (range 0.0 to 1.0), if this conversion is successful then internally the plugin must
         call setParameter as part of this call
          index: parameter index
          ptr: string-pointer to textual representation of parameter
          return: 1 = conversion successful,
                  0 = fail
    */

    case effString2Parameter:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effString2Parameter\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effGetNumProgramCategories):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetNumProgramCategories (deprecated)\n");
      #endif
      break;

    /*
      host requests program name
      index: program index
      value: deprecated in VST2.4
      ptr: returns string-pointer to program name (max 24 char + 0)
      return: 1 = OK, 0 = fail
      energy xt 2 calls this before effClose ??? (investigate)
    */

    case effGetProgramNameIndexed:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetProgramNameIndexed\n");
      #endif
      //S3_Strcpy((char*)ptr,MProgramName);
      //return (VstIntPtr)MPrograms[index]->getName();
      S3_Strncpy((char*)ptr, MPrograms[index]->getName(), 24 );
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effCopyProgram):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effCopyProgram (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effConnectInput):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effConnectInput (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effConnectOutput):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effConnectOutput (deprecated)\n");
      #endif
      break;

    /*
      struct VstPinProperties {
        char label[kVstMaxLabelLen];	          // pin name
        VstInt32 flags;					                // @see VstPinPropertiesFlags
        VstInt32 arrangementType;		            // @see VstSpeakerArrangementType
        char shortLabel[kVstMaxShortLabelLen];  // short name (recommended: 6 + delimiter)
        char future[48];				                // reserved for future use
      };
      enum VstPinPropertiesFlags {
        kVstPinIsActive   = 1 << 0,   // pin is active, ignored by Host
        kVstPinIsStereo   = 1 << 1,   // pin is first of a stereo pair
        kVstPinUseSpeaker = 1 << 2    // #VstPinProperties::arrangementType is valid and can be used to get the wanted arrangement
      };
      enum VstSpeakerArrangementType {
        kSpeakerArrUserDefined = -2,  // user defined
        kSpeakerArrEmpty = -1,        // empty arrangement
        kSpeakerArrMono  =  0,        // M
        kSpeakerArrStereo,            // L R
        kSpeakerArrStereoSurround,    // Ls Rs
        ...
      };
    */

    case effGetInputProperties: {
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetInputProperties %i\n",index);
      #endif
      VstPinProperties* pin = (VstPinProperties*)ptr;
      char* pc = S3_Strcpy(pin->label,"input "); // returns ptr to end of string? ptr to the '\0', or after the 0?
      *pc++ = S3_HEX_TABLE[index&0x0f];
      *pc = 0;
      pin->flags = 1; // active
      if ( (index&1) == 0) pin->flags = pin->flags | 2; // first of stereo pair
      return 1;
    }

    /*
    */

    case effGetOutputProperties: {
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetOutputProperties %i\n",index);
      #endif
      VstPinProperties* pin = (VstPinProperties*)ptr;
      char* pc = S3_Strcpy(pin->label,"output ");
      *pc++ = S3_HEX_TABLE[index&0x0f];
      *pc = 0;
      pin->flags = 1; // active
      if ( (index&1) == 0) pin->flags = pin->flags | 2; // first of stereo pair
      return 1;
    }

    /*
      host requests plugin category
      return: category number:
         0 = kPlugCategUnknown        Unknown, category not implemented
         1 = kPlugCategEffect         Simple Effect,
         2 = kPlugCategSynth          VST Instrument (Synths, samplers,...)
         3 = kPlugCategAnalysis       Analysis. Scope, Tuner, ...
         4 = kPlugCategMastering      Mastering, Dynamics, ...
         5 = kPlugCategSpacializer    Spacializer (panners),
         6 = kPlugCategRoomFx         RoomFx (delays and reverbs),
         7 = kPlugSurroundFx          SurroundFx (dedicated surround processors),
         8 = kPlugCategRestoration    Restoration, denoiser
         9 = kPlugCategOfflineProcess OfflineProcess,
        10 = kPlugCategShell          Shell (is only a container of plugins), @see effShellGetNextPlugin
        11 = kPlugCategGenerator      Generator, TonGenerator, ..
    */

    case effGetPlugCategory: {
      uint32 res = kPlugCategEffect;

      #ifdef S3_PLUGIN_SYNTH
      res = kPlugCategSynth;
      #endif

      #ifdef S3_PLUGIN_TOOL
      res = kPlugCategGenerator;
      #endif

      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetPlugCategory");
        #ifdef S3_PLUGIN_SYNTH
        VST_TRACE(" -> kPlugCategSynth (%i)\n",kPlugCategSynth);
        #else
        VST_TRACE(" -> kPlugCategEffect (%i)\n",kPlugCategEffect);
        #endif
      #endif
      return res;
    }

    /*
    */

    case DECLARE_VST_DEPRECATED(effGetCurrentPosition):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetCurrentPosition (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effGetDestinationBuffer):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetDestinationBuffer (deprecated)\n");
      #endif
      break;

    /*
      index: start flag
      value: count
      ptr: pointer to VstAudioFile array
      return: 1 = successful
    */

    case effOfflineNotify:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effOfflineNotify\n");
      #endif
      break;

    /*
      value: count
      ptr: pointer to VstOfflineTask array
      return: 1 = successful
    */

    case effOfflinePrepare:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effOfflinePrepare\n");
      #endif
      break;

    /*
      value: count
      ptr: pointer to VstOfflineTask array
      return: 1 = successful
    */

    case effOfflineRun:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effOfflineRun\n");
      #endif
      break;

    /*
      ptr: pointer to VariableIO*
      return: 1 = successful
    */

    case effProcessVarIo:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effProcessVarIo\n");
      #endif
      break;

    /*
      value: pointer to VstSpeakerArrangement* pluginInput
      ptr: pointer to VstSpeakerArrangement* pluginOutput
      return: 1 = successful
    */

    case effSetSpeakerArrangement:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetSpeakerArrangement\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effSetBlockSizeAndSampleRate):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetBlockSizeAndSampleRate (deprecated)\n");
      #endif
      break;

    /*
      the host has switched the 'softBypass' state
      value: onOff (0 = off, 1 = On)
      return: 1 = successful
      ---
      returns true if the plug-in supports SoftBypass, which allows the process
      to be called, even if the plug-in was bypassed. This is extremely useful
      if the plug-in should be able to maintain a processing state even when
      turned off, e.g. surround decoders/encoders. Moreover, this feature may be
      automated by the host.
    */

    case effSetBypass:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetBypass %i\n",value);
      #endif
      if (value == 0) on_stateChange(s3_ps_bypassOff);
      else on_stateChange(s3_ps_bypass);
      return 1;
      break;

    /*
      ptr: returns string-pointer to a string with effect name (max 32 bytes)
      return: 1 = successful
    */

    case effGetEffectName:
      S3_Strcpy((char*)ptr,MName);
      #ifdef S3_32BIT
        S3_Strcat((char*)ptr,"_32");
      #endif
      #ifdef S3_64BIT
        S3_Strcat((char*)ptr,"_64");
      #endif
      #ifdef S3_DEBUG
        S3_Strcat((char*)ptr,"_debug");
      #endif
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetEffectName -> '%s'\n",ptr);
      #endif
      return 1;

    /*
    */

    case DECLARE_VST_DEPRECATED(effGetErrorText):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetErrorText (deprecated)\n");
      #endif
      break;

    /*
      ptr: returns string-pointer to a string identifying the vendor (max 64 ch)
      return: 1 = successful
    */

    case effGetVendorString:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetVendorString -> '%s'\n",MAuthor);
      #endif
      S3_Strcpy((char*)ptr,MAuthor);
      break;

    /*
      ptr: returns string-pointer to a string with product name (max 64 char)
      return: 1 = successful
    */

    case effGetProductString:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetProductString -> '%s'\n",MDescription);
      #endif
      S3_Strcpy((char*)ptr,MDescription);
      break;

    /*
      return: vendor-specific version
    */

    case effGetVendorVersion:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetVendorVersion -> %08x (%i)\n",MVersion,MVersion);
      #endif
      return MVersion;

    /*
      no definition, vendor specific handling
      The MouseWheel handling is a bit of an after thought, and has been
      implemented by Stenberg via the effVendorSpecific call as follows:
      index: 'stCA' / $73744341
      value: 'Whee' / $57686565
      ptr: 1.0 = wheel-up, -1.0 = wheel-down
      return: 1 = MouseWheel used

      case effVendorSpecific:
        if (index == effGetParamDisplay && ptr) {
          if (value>=0 && value<NUM_PARAMS) {
            sprintf(ptr,"%f",opt);
            return 0xbeef;
          }
        }
    */

    case effVendorSpecific:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effVendorSpecific\n");
      #endif
      break;

   /*
      host queries a plugin 'cando' capability
      ptr: string-pointer to cando string
      return: 1='cando', 0='don't know' (default), -1='No'

      Bitwig asks about:
        midiSingleNoteTuningChange
        midiKeyBasedInstrumentControl
        MPE

      Jost asks about:
        plugAsChannelInsert
        plugAsSend
        mixDryWet

      see: MIDI specifications for Multidimensional Polyphonic Expression (MPE)
      https://docs.google.com/document/d/1-26r0pVtVBrZHM6VGA05hpF-ij5xT6aaXY9BfDzyTx8/edit?pli=1#

      reaper calls this after resume & start plugin..
    */

    case effCanDo: {
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effCanDo '%s'",(char*)ptr);
      #endif
      char* p = (char*)ptr;
      // plug-in will send Vst/MIDI events to Host
      #ifdef S3_PLUGIN_SEND_MIDI
        if (!S3_Strcmp(p,"sendVstEvents"))    { VST_TRACE(" -> 1\n"); return 1; }
        if (!S3_Strcmp(p,"sendVstMidiEvent")) { VST_TRACE(" -> 1\n"); return 1; }
      #endif
      // plug-in can receive Vst/MIDI events to Host
      #ifdef S3_PLUGIN_RECEIVE_MIDI
        if (!S3_Strcmp(p,"receiveVstEvents"))     { VST_TRACE(" -> 1\n"); return 1; }
        if (!S3_Strcmp(p,"receiveVstMidiEvent"))  { VST_TRACE(" -> 1\n"); return 1; }
      #endif
      // plug-in can receive Time info from Host
      if (!S3_Strcmp(p,"receiveVstTimeInfo")) { VST_TRACE(" -> 1\n"); return 1; }
      // plug-in supports offline functions (#offlineNotify, #offlinePrepare, #offlineRun)
      if (!S3_Strcmp(p,"offline")) { VST_TRACE("-> 0\n"); return 0; }
      // plug-in supports function #getMidiProgramName ()
      if (!S3_Strcmp(p,"midiProgramNames")) { VST_TRACE("-> 0\n"); return 0; }
      // plug-in supports function setBypass()
      if (!S3_Strcmp(p,"bypass")) { VST_TRACE(" -> 1\n"); return 1; }

      if (!S3_Strcmp(p,"MPE")) {
        //#ifdef S3_DEBUG_VST
        //#ifdef S3_PLUGIN_MPE
        //VST_TRACE("vst dispatcher: effCanDo '%s' >> 1",(char*)ptr);
        //#else
        //VST_TRACE("vst dispatcher: effCanDo '%s' >> 0",(char*)ptr);
        //#endif
        //#endif // S3_PLUGIN_MPE
        //#ifdef S3_PLUGIN_MPE
        //MHostMPE = true;
        //// MVoices.setMPE(true);
        //on_setMode(s3_pm_mpe,MHostMPE);
        //return 1; // 0
        //#else
        //return 0;
        //#endif
      }

      /*
        http://www.reaper.fm/sdk/vst/vst_ext.php
        A REAPER aware VST plug-in can respond to effCanDo "hasCockosExtensions",
        with 0xbeef0000 (returning this value), which will inform the host that
        it can call certain extended calls. A plug-in that responds to
        "hasCockosExtensions" may choose to implement any subset of the extended
        calls.
      */

      //#ifdef S3_PLUGIN_REAPER_EXT
      //if (MFlags&kpf_reaper) {
      //  // Warning: range check error while evaluating constants
      //  //and (Pos(ptr,'hasCockosExtensions') <> 0) then result := {%H-}$beef0000;
      //  if (!SStrcmp(p,"hasCockosExtensions")) return 0xbeef0000;
      //}
      //#endif // S3_PLUGIN_REAPER_EXT

      if (!S3_Strcmp(p,"hasCockosExtensions")) { VST_TRACE("-> 0\n"); return 0; }


      VST_TRACE("-> 0\n");
      return 0;
    }

    /*
      returns tail size; 0 is default (return 1 for 'no tail')
      called a lot by Bitwig
    */

    case effGetTailSize:
      #ifdef S3_DEBUG_VST
        if (MHostId != s3_hi_bitwig)
          VST_TRACE("vst_dispatcher / effGetTailSize\n");
      #endif
      if (MTail == -1) return 1;
      return MTail;
      //break;

    /*
      host sends this idle call in response to plugin's 'audioMasterneedIdle'
      request
      return: 1 must be returned to keep idle calls being issued by host

      energyXt2 calls this
      reaper too, if editor is open?
      reaper calls this even if we don't have an editor..
    */

    case DECLARE_VST_DEPRECATED(effIdle):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effIdle (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effGetIcon):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetIcon (deprecated)\n");
      #endif
      break;

    /*
    */

    case DECLARE_VST_DEPRECATED(effSetViewPosition):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetViewPosition (deprecated)\n");
      #endif
      break;

    /*
      of parameter <index>, VstParameterProperties* in <ptr>
      return=1 when successful
    */

    case effGetParameterProperties: {
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetParameterProperties\n");
      #endif
      //VstParameterProperties* param_prop = (VstParameterProperties*)ptr;
      break;
    }

    /*
    */

    case DECLARE_VST_DEPRECATED(effKeysRequired):
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effKeysRequired (deprecated)\n");
      #endif
      break;

    /*
      returns 2; older versions return 0
    */

    case effGetVstVersion:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetVstVersion -> 2\n");
      #endif
      return 2;
      //break;

// vst 2.1

    /*
      index: character
      value: virtual key-code
      opt: key-modifiers
      return: 0 = KeyDown not used, 1 = KeyDown used
    */

    case effEditKeyDown:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditKeyDown\n");
      #endif
      break;

    /*
      index: character
      value: virtual key-code
      opt: key-modifiers
      return: 0 = not KeyUp used, 1 = KeyUp used
    */

    case effEditKeyUp:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEditKeyUp\n");
      #endif
      break;

    /*
      value: knobmode 0=circular, 1=circular relativ, 2=linear
      return: 1 = successful
    */

    case effSetEditKnobMode:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetEditKnobMode\n");
      #endif
      MEditorKnobMode = (uint32)value;
      return 1;
      //break;

    /*
      ptr: passed points to MidiProgramName struct.
      ptr: struct will be filled with information for 'thisProgramIndex'.
      return: number of used programIndexes.
              if 0 is returned, no MidiProgramNames supported.
    */

    case effGetMidiProgramName:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetMidiProgramName\n");
      #endif
      break;

    /*
      ptr: passed points to MidiProgramName struct.
      ptr: struct will be filled with information for the current program.
      return: the programIndex of the current program.
    */

    case effGetCurrentMidiProgram:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetCurrentMidiProgram\n");
      #endif
      break;

    /*
      ptr: passed points to MidiProgramCategory struct.
      ptr: struct will be filled with information for 'thisCategoryIndex'.
      return: number of used categoryIndexes.
              if 0 is returned, no MidiProgramCategories supported.
    */

    case effGetMidiProgramCategory:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetMidiProgramCategory\n");
      #endif
      break;

    /*
      return 1 if the MidiProgramNames or MidiKeyNames had changed on this
      channel, 0 otherwise. <ptr> ignored.
    */

    case effHasMidiProgramsChanged:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effHasMidiProgramsChanged\n");
      #endif
      break;

    /*
      ptr: passed points to MidiKeyName struct.
      ptr: struct will be filled with information for 'thisProgramIndex' and 'thisKeyNumber'.
    */

    case effGetMidiKeyName:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetMidiKeyName\n");
      #endif
      break;

    /*
      called before all setParameter calls when changing program..
      host calls this before a new program (effSetProgram) is loaded
    */

    case effBeginSetProgram:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effBeginSetProgram\n");
      #endif
      //on_programChange(-1); // S3_BEGIN_PROGRAM
      break;

    /*
      host calls this after the new program (effSetProgram) has been loaded
    */

    case effEndSetProgram:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effEndSetProgram\n");
      #endif
      //on_programChange(-2); // S3_END_PROGRAM
      break;

// vst 2.3

    /*
      value: pointer to VstSpeakerArrangement** pluginInput
      ptr: pointer to VstSpeakerArrangement** pluginOutput
    */

    case effGetSpeakerArrangement:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetSpeakerArrangement\n");
      #endif
      break;

    /*
      This opcode is only called, if plugin is of type kPlugCategShell.
      <ptr> points to a char buffer of size 64, which is to be filled with the
      name of the plugin including the terminating zero.
      return: the next plugin's uniqueID.
    */

    case effShellGetNextPlugin:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effShellGetNextPlugin\n");
      #endif
      break;

    /*
      host calls this before the start of process call and after e.g. resume()

      http://ygrabit.steinberg.de/~ygrabit/public_html/vstsdk/OnlineDoc/vstsdk2.3/html/plug/2.0/AudioEffectX.html#startProcess
      Called one time before the start of process call.
      example:
        resume () // the user activates the plugin (User Interface Thread)
        startProcess () // Audio Thread
        process () // or ProcessReplacing ()
        process ()
        process ()
        stopProcess () // something has stopped the engine...or delayed the call
        startProcess ()
        process ()
        process ()
        process ()
        stopProcess () // yet the user has really stopped the plugin (Off)
        suspend ()

      pre vst 2.3 hosts don't call this, do they?
      should we handle it in suspend/resume?

      ext2 2.5 doesn't call this
      http://www.kvraudio.com/forum/viewtopic.php?t=277899
    */

    case effStartProcess:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effStartProcess\n");
      #endif
      MIsProcessing = true;
      on_stateChange(s3_ps_start);
      break;

    /*
      host calls this after the stop of process call and before e.g. suspend()
      - not called in energy xt2 linux
    */

    case effStopProcess:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effStopProcess\n");
      #endif
      MIsProcessing = false;
      on_stateChange(s3_ps_stop);
      break;

    /*
      Called in offline (non RealTime) Process before process is called,
      indicates how many sample will be processed
    */

    case effSetTotalSampleToProcess:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetTotalSampleToProcess\n");
      #endif
      break;

    /*
      PanLaw:
        Type (Linear, Equal Power,.. see enum PanLaw Type) in <value>,
        Gain in <opt>: for Linear :
        [1.0 => 0dB PanLaw], [~0.58 => -4.5dB], [0.5 => -6.02dB]
    */

    case effSetPanLaw:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetPanLaw\n");
      #endif
      break;

    /*
      Called before a Bank is loaded, <ptr> points to VstPatchChunkInfo structure
      return: -1 if the Bank cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
      ala beginSetProgram
      called by: ableton live
    */

    case effBeginLoadBank:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effBeginLoadBank\n");
      #endif
      break;

    /*
      Called before a Program is loaded, <ptr> points to VstPatchChunkInfo structure
      return: -1 if the Program cannot be loaded, 1 if it can be loaded, 0 (for compatibility)
    */

    case effBeginLoadProgram:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effBeginLoadProgram\n");
      #endif
      break;

  // vst 2.4

    /*
      value
      Symbolic precision constants used for effSetProcessPrecision.
    */

    case effSetProcessPrecision:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effSetProcessPrecision\n");
      #endif
      break;

    /*
      return: number of used MIDI input channels (1-15)
    */

    case effGetNumMidiInputChannels:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetNumMidiInputChannels\n");
      #endif
      break;

    /*
      return: number of used MIDI output channels (1-15)
    */

    case effGetNumMidiOutputChannels:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / effGetNumMidiOutputChannels\n");
      #endif
      break;

    default:
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher / unknown dispatcher opcode %i\n",opcode);
      #endif
      break;

  };
  return 0;
}

//----------------------------------------------------------------------
// hpst
//----------------------------------------------------------------------

// typedef VstIntPtr(VSTCALLBACK *audioMasterCallback) (AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt);

//----------

void S3_Plugin_Vst::host_automate(VstInt32 AIndex, float AValue) {
  if (MMaster) {
    MMaster(&MAEffect,audioMasterAutomate,AIndex,0,S3_NULL,AValue);
  }
}

//----------

bool S3_Plugin_Vst::host_beginEdit(VstInt32 AIndex) {
  if (MMaster) {
    if ( (VstInt32)MMaster(&MAEffect,audioMasterBeginEdit,AIndex,0,S3_NULL,0) == 1) return true;
  }
  return false;
}

//----------

bool S3_Plugin_Vst::host_canDo(const char* AText) {
  if (MMaster) {
    if ( (VstInt32)MMaster(&MAEffect,audioMasterCanDo,0,0,(void*)AText,0) == 1) return true;
  }
  return false;
}

//----------

VstInt32 S3_Plugin_Vst::host_currentId(void) {
  if (MMaster) {
    return (VstInt32)MMaster(&MAEffect,audioMasterCurrentId,0,0,S3_NULL,0);
  }
  return 0;
}

//----------

bool S3_Plugin_Vst::host_endEdit(VstInt32 AIndex) {
  if (MMaster) {
    if ((VstInt32)MMaster(&MAEffect,audioMasterEndEdit,AIndex,0,S3_NULL,0) == 1) return true;
  }
  return false;
}

//----------

bool S3_Plugin_Vst::host_getProductString(const char* ABuffer) {
  if (MMaster) {
    if ((VstInt32)MMaster(&MAEffect,audioMasterGetProductString,0,0,(void*)ABuffer,0) == 1) return true;
  }
  return false;
}

//----------

float S3_Plugin_Vst::host_getSampleRate(void) {
  if (MMaster) {
    return MMaster(&MAEffect,audioMasterGetSampleRate,0,0,S3_NULL,0);
  }
  return 0;
}

//----------

VstTimeInfo* S3_Plugin_Vst::host_getTime(VstInt32 AFilter) {
  if (MMaster) {
    // Hint: Conversion between ordinals and pointers is not portable
    return (VstTimeInfo*)MMaster(&MAEffect,audioMasterGetTime,0,AFilter,S3_NULL,0);
  }
  return S3_NULL;
}

//----------

bool S3_Plugin_Vst::host_getVendorString(const char* ABuffer) {
  if (MMaster) {
    if ((VstInt32)MMaster(&MAEffect,audioMasterGetVendorString,0,0,(void*)ABuffer,0) == 1) return true;
  }
  return  false;
}

//----------

VstInt32 S3_Plugin_Vst::host_getVendorVersion(void) {
  if (MMaster) return (VstInt32)MMaster(&MAEffect,audioMasterGetVendorVersion,0,0,S3_NULL,0);
  return 0;
}

//----------

void S3_Plugin_Vst::host_idle(void) {
  if (MMaster) MMaster(&MAEffect,audioMasterIdle,0,0,S3_NULL,0);
}

//----------

bool S3_Plugin_Vst::host_ioChanged(void) {
  if (MMaster) {
    if ((VstInt32)MMaster(&MAEffect,audioMasterIOChanged,0,0,S3_NULL,0) != 0) return true;
  }
  return false;
}

//----------

bool S3_Plugin_Vst::host_processEvents(VstEvents* events) {
  if (MMaster) {
    if ((VstInt32)MMaster(&MAEffect,audioMasterProcessEvents,0,0,events,0) == 1) return true;
  }
  return false;
}

//----------

bool S3_Plugin_Vst::host_sizeWindow(int32 AWidth, int32 AHeight) {
  if (MMaster) {
    if ((VstInt32)MMaster(&MAEffect,audioMasterSizeWindow,AWidth,AHeight,S3_NULL,0) == 1) return true;
  }
  return false;
}

//----------

VstInt32 S3_Plugin_Vst::host_version(void) {
  VstInt32 result = 1;
  if (MMaster) result = (VstInt32)MMaster(&MAEffect,audioMasterVersion,0,0,S3_NULL,0);
  if (result==0) result = 1; // old (!version)
  return result;
}

//----------------------------------------------------------------------
// callbacks
//----------------------------------------------------------------------

VstIntPtr vst_dispatcher_callback(AEffect* effect, VstInt32 opcode, VstInt32 index, VstIntPtr value, void* ptr, float opt) {
  VstIntPtr result = 0;
  S3_Plugin_Vst* plugin = (S3_Plugin_Vst*)effect->object;
  if (plugin) {
    result = plugin->vst_dispatcher(opcode,index,value,ptr,opt);
    if (opcode==effClose) {
      #ifdef S3_DEBUG_VST
        VST_TRACE("vst_dispatcher_callback / effClose -> delete plugin\n");
      #endif
      plugin->on_destroy();
      delete plugin;
    }
  }
  return result;
}

//----------

void vst_setParameter_callback(AEffect* effect, VstInt32 index, float parameter) {
  S3_Plugin_Vst* plugin = (S3_Plugin_Vst*)effect->object;
  if (plugin) plugin->vst_setParameter(index,parameter);
}

//----------

float vst_getParameter_callback(AEffect* effect, VstInt32 index) {
  float result = 0;
  S3_Plugin_Vst* plugin = (S3_Plugin_Vst*)effect->object;
  if (plugin) result = plugin->vst_getParameter(index);
  return result;
}

//----------

void vst_process_callback(AEffect* effect, float** inputs, float** outputs, VstInt32 sampleFrames) {
  S3_Plugin_Vst* plugin = (S3_Plugin_Vst*)effect->object;
  if (plugin) plugin->vst_process(inputs,outputs,sampleFrames);
}

//----------

void vst_processDouble_callback(AEffect* effect, double** inputs, double** outputs, VstInt32 sampleFrames) {
  S3_Plugin_Vst* plugin = (S3_Plugin_Vst*)effect->object;
  if (plugin) plugin->vst_processDouble(inputs,outputs,sampleFrames);
}

#undef VST_TRACE

//----------------------------------------------------------------------
#endif
