#ifndef s3_ladspa_included
#define s3_ladspa_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include <dlfcn.h>
#include "common/s3_library.h"
#include "common/s3_memory_stdlib.h"
#include "extern/ladspa/ladspa.h"

#define S3_MAX_LADSPA_AUDIO_INPUTS     32
#define S3_MAX_LADSPA_AUDIO_OUTPUTS    32
#define S3_MAX_LADSPA_CONTROL_INPUTS   512
#define S3_MAX_LADSPA_CONTROL_OUTPUTS  512
#define S3_MAX_LADSPA_PORTS            512

//----------

struct S3_Ladspa_Port {
  const char* name;
  uint32      flags;
  float       val;
  float       minval;
  float       maxval;
};

//----------

class S3_Ladspa : public S3_Library {
  protected:
    bool MInstantiated;
    bool MActivated;
  protected:
    uint32                      MLadspaFlags;
    LADSPA_Handle               MLadspaHandle;
    const LADSPA_Descriptor*    MLadspaDescriptor;
    LADSPA_Descriptor_Function  MGetLadspaDescriptor;
  protected:
    uint32                      MNumAudioInputs;
    uint32                      MNumAudioOutputs;
    uint32                      MNumControlInputs;
    uint32                      MNumControlOutputs;
    S3_Ladspa_Port              MLadspaPorts[S3_MAX_LADSPA_PORTS];
    int32                       MAudioInputIndex[S3_MAX_LADSPA_AUDIO_INPUTS];
    int32                       MAudioOutputIndex[S3_MAX_LADSPA_AUDIO_OUTPUTS];
    int32                       MControlInputIndex[S3_MAX_LADSPA_CONTROL_INPUTS];
    int32                       MControlOutputIndex[S3_MAX_LADSPA_CONTROL_OUTPUTS];
  public:
    //bool              instantiated(void)            { return MInstantiated; }
    //bool              activated(void)               { return MActivated; }
    const char*       name(void)                    { return MLadspaDescriptor->Name; }
    const char*       label(void)                   { return MLadspaDescriptor->Label; }
    const char*       maker(void)                   { return MLadspaDescriptor->Maker; }
    const char*       copyright(void)               { return MLadspaDescriptor->Copyright; }
    uint32            uniqueId(void)                { return MLadspaDescriptor->UniqueID; }
    S3_Ladspa_Port*   port(int i)                   { return &MLadspaPorts[i]; }
    S3_Ladspa_Port*   audioInputPort(int i)         { return &MLadspaPorts[MAudioInputIndex[i]]; }
    S3_Ladspa_Port*   audioOutputPort(int i)        { return &MLadspaPorts[MAudioOutputIndex[i]]; }
    S3_Ladspa_Port*   controlInputPort(int i)       { return &MLadspaPorts[MControlInputIndex[i]]; }
    S3_Ladspa_Port*   controlOutputPort(int i)      { return &MLadspaPorts[MControlOutputIndex[i]]; }
    uint32            numAudioInputs(void)          { return MNumAudioInputs; }
    uint32            numAudioOutputs(void)         { return MNumAudioOutputs; }
    uint32            numControlInputs(void)        { return MNumControlInputs; }
    uint32            numControlOutputs(void)       { return MNumControlOutputs; }
    uint32            audioInputIndex(uint32 i)     { return MAudioInputIndex[i]; }
    uint32            audioOutputIndex(uint32 i)    { return MAudioOutputIndex[i]; }
    uint32            controlInputIndex(uint32 i)   { return MControlInputIndex[i]; }
    uint32            controlOutputIndex(uint32 i)  { return MControlOutputIndex[i]; }
  public:
    S3_Ladspa();
    virtual ~S3_Ladspa();
  public:
    virtual bool    load(const char* AFilename) override;
  public:
    virtual uint32  countDescriptors(void);
    virtual bool    initDescriptor(uint32 AIndex);
    virtual void    initPorts(void);
    virtual void    dumpLadspaDescriptor(void);
  public:
    virtual void    instantiate(uint32 ASampleRate);
    virtual void    connect_port(uint32 APort, float* APtr);
    virtual void    activate(void);
    virtual void    run(uint32 ANum);
    virtual void    run_adding(uint32 ANum);
    virtual void    set_run_adding_gain(float AGain);
    virtual void    deactivate(void);
    virtual void    cleanup(void);
  };

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Ladspa::S3_Ladspa()
: S3_Library() {
  MInstantiated       = false;;
  MActivated          = false;;
  MLadspaFlags        = s3_lf_none;
  MLadspaHandle       = S3_NULL;
  MLadspaDescriptor   = S3_NULL;
  MNumAudioInputs     = 0;
  MNumAudioOutputs    = 0;
  MNumControlInputs   = 0;
  MNumControlOutputs  = 0;
  S3_Memset(MLadspaPorts,       0,sizeof(MLadspaPorts));
  S3_Memset(MAudioInputIndex,   0,sizeof(MAudioInputIndex));
  S3_Memset(MAudioOutputIndex,  0,sizeof(MAudioOutputIndex));
  S3_Memset(MControlInputIndex, 0,sizeof(MControlInputIndex));
  S3_Memset(MControlOutputIndex,0,sizeof(MControlOutputIndex));
}

//----------

S3_Ladspa::~S3_Ladspa() {
}

//----------------------------------------
//
//----------------------------------------

bool S3_Ladspa::load(const char* AFilename) {
  //S3_DTrace("S3_Ladspa::load %s\n",AFilename);
  if (S3_Library::load(AFilename)) {
    MGetLadspaDescriptor = (LADSPA_Descriptor_Function)getSymbol("ladspa_descriptor");
    return true;
  }
  return false;
}

//----------------------------------------
//
//----------------------------------------

uint32 S3_Ladspa::countDescriptors(void) {
  //S3_DTrace("S3_Ladspa::countDescriptors\n");
  uint32 num = 0;
  if (MGetLadspaDescriptor) {
    const LADSPA_Descriptor* desc = MGetLadspaDescriptor(num);
    while (desc) {
      num++;
      desc = MGetLadspaDescriptor(num);
    }
  }
  return num;
}

//----------

bool S3_Ladspa::initDescriptor(uint32 AIndex) {
  //S3_DTrace("S3_Ladspa::InitDescriptor %i\n",AIndex);
  if (MGetLadspaDescriptor) {
    MLadspaDescriptor = MGetLadspaDescriptor(AIndex);
    return (MLadspaDescriptor);
  }
  return false;
}

//----------

void S3_Ladspa::initPorts(void) {
  //S3_DTrace("S3_Ladspa::initPorts\n");
  MNumAudioInputs     = 0;
  MNumAudioOutputs    = 0;
  MNumControlInputs   = 0;
  MNumControlOutputs  = 0;
  MLadspaFlags        = s3_lf_none;
  if (MLadspaDescriptor) {
    if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_REALTIME)         { MLadspaFlags |= s3_lf_realtime; }
    if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_INPLACE_BROKEN)   { MLadspaFlags |= s3_lf_inplaceBroken; }
    if (MLadspaDescriptor->Properties & LADSPA_PROPERTY_HARD_RT_CAPABLE)  { MLadspaFlags |= s3_lf_hardRTCapable; }
    for (uint32 i=0; i<MLadspaDescriptor->PortCount; i++) {
      MLadspaPorts[i].name    = MLadspaDescriptor->PortNames[i];
      MLadspaPorts[i].flags   = s3_lp_none;
      MLadspaPorts[i].val     = 0;
      MLadspaPorts[i].minval  = 0;
      MLadspaPorts[i].maxval  = 1;
      LADSPA_PortDescriptor pd = MLadspaDescriptor->PortDescriptors[i];
      if (pd & LADSPA_PORT_AUDIO)   { MLadspaPorts[i].flags |= s3_lp_audio; }
      if (pd & LADSPA_PORT_CONTROL) { MLadspaPorts[i].flags |= s3_lp_control; }
      if (pd & LADSPA_PORT_INPUT)   { MLadspaPorts[i].flags |= s3_lp_input; }
      if (pd & LADSPA_PORT_OUTPUT)  { MLadspaPorts[i].flags |= s3_lp_output; }
      uint32 hints = MLadspaDescriptor->PortRangeHints[i].HintDescriptor;
      if (hints&LADSPA_HINT_BOUNDED_BELOW)  { MLadspaPorts[i].minval = MLadspaDescriptor->PortRangeHints[i].LowerBound; }
      if (hints&LADSPA_HINT_BOUNDED_ABOVE)  { MLadspaPorts[i].maxval = MLadspaDescriptor->PortRangeHints[i].UpperBound; }
      if (hints&LADSPA_HINT_TOGGLED)        { MLadspaPorts[i].flags |= s3_lp_toggled; }
      if (hints&LADSPA_HINT_SAMPLE_RATE)    { MLadspaPorts[i].flags |= s3_lp_samplerate; }
      if (hints&LADSPA_HINT_LOGARITHMIC)    { MLadspaPorts[i].flags |= s3_lp_logarithmic; }
      if (hints&LADSPA_HINT_INTEGER)        { MLadspaPorts[i].flags |= s3_lp_integer; }
      uint32 defmask = hints & LADSPA_HINT_DEFAULT_MASK;
      if (defmask == LADSPA_HINT_DEFAULT_MINIMUM) MLadspaPorts[i].val =  MLadspaPorts[i].minval;
      if (defmask == LADSPA_HINT_DEFAULT_LOW)     MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.75 + MLadspaPorts[i].maxval * 0.25);
      if (defmask == LADSPA_HINT_DEFAULT_MIDDLE)  MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.50 + MLadspaPorts[i].maxval * 0.50);
      if (defmask == LADSPA_HINT_DEFAULT_HIGH)    MLadspaPorts[i].val = (MLadspaPorts[i].minval * 0.25 + MLadspaPorts[i].maxval * 0.75);
      if (defmask == LADSPA_HINT_DEFAULT_MAXIMUM) MLadspaPorts[i].val =  MLadspaPorts[i].maxval;
      if (defmask == LADSPA_HINT_DEFAULT_0)       MLadspaPorts[i].val = 0;
      if (defmask == LADSPA_HINT_DEFAULT_1)       MLadspaPorts[i].val = 1;
      if (defmask == LADSPA_HINT_DEFAULT_100)     MLadspaPorts[i].val = 100;
      if (defmask == LADSPA_HINT_DEFAULT_440)     MLadspaPorts[i].val = 440;
      if ( (MLadspaPorts[i].flags&s3_lp_audio)   && (MLadspaPorts[i].flags&s3_lp_input)  ) MAudioInputIndex[   MNumAudioInputs++]    = i;
      if ( (MLadspaPorts[i].flags&s3_lp_audio)   && (MLadspaPorts[i].flags&s3_lp_output) ) MAudioOutputIndex[  MNumAudioOutputs++]   = i;
      if ( (MLadspaPorts[i].flags&s3_lp_control) && (MLadspaPorts[i].flags&s3_lp_input)  ) MControlInputIndex[ MNumControlInputs++]  = i;
      if ( (MLadspaPorts[i].flags&s3_lp_control) && (MLadspaPorts[i].flags&s3_lp_output) ) MControlOutputIndex[MNumControlOutputs++] = i;
    }
  }
}

//----------

void S3_Ladspa::dumpLadspaDescriptor(void) {
  S3_DTrace("LADSPA_Descriptor:\n");
  S3_DTrace("  name:               '%s'\n",     MLadspaDescriptor->Name);
  S3_DTrace("  label:              '%s'\n",     MLadspaDescriptor->Label);
  S3_DTrace("  maker:              '%s'\n",     MLadspaDescriptor->Maker);
  S3_DTrace("  copyright:          '%s'\n",     MLadspaDescriptor->Copyright);
  S3_DTrace("  unique id:           %i\n",      MLadspaDescriptor->UniqueID);
  S3_DTrace("  num ports:           %i\n",      MLadspaDescriptor->PortCount);
  S3_DTrace("  flags:               0x%08x\n",  MLadspaFlags);
  S3_DTrace("  instantiate:         0x%08x\n",  MLadspaDescriptor->instantiate);
  S3_DTrace("  connect_port:        0x%08x\n",  MLadspaDescriptor->connect_port);
  S3_DTrace("  activate:            0x%08x\n",  MLadspaDescriptor->activate);
  S3_DTrace("  run:                 0x%08x\n",  MLadspaDescriptor->run);
  S3_DTrace("  run_adding:          0x%08x\n",  MLadspaDescriptor->run_adding);
  S3_DTrace("  set_run_adding_gain: 0x%08x\n",  MLadspaDescriptor->set_run_adding_gain);
  S3_DTrace("  deactivate:          0x%08x\n",  MLadspaDescriptor->deactivate);
  S3_DTrace("  cleanup:             0x%08x\n",  MLadspaDescriptor->cleanup);
  S3_DTrace("  num audio input ports:     %i\n", MNumAudioInputs );
  S3_DTrace("  num audio output ports:    %i\n", MNumAudioOutputs );
  S3_DTrace("  num control input ports:   %i\n", MNumControlInputs );
  S3_DTrace("  num control output ports:  %i\n", MNumControlOutputs );
}

//----------------------------------------
//
//----------------------------------------

void S3_Ladspa::instantiate(uint32 ASampleRate) {
  if (MLibLoaded) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->instantiate) {
        MLadspaHandle = MLadspaDescriptor->instantiate(MLadspaDescriptor,ASampleRate);
        if (MLadspaHandle) MInstantiated = true;
      }
    }
  }
}

//----------

void S3_Ladspa::connect_port(uint32 APort, float* APtr) {
  if (MInstantiated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->connect_port) {
        MLadspaDescriptor->connect_port(MLadspaHandle,APort,APtr);
      }
    }
  }
}

//----------

void S3_Ladspa::activate(void) {
  if (MInstantiated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->activate) {
        MLadspaDescriptor->activate(MLadspaHandle);
        MActivated = true;
      }
    }
  }
}

//----------

void S3_Ladspa::run(uint32 ANum) {
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->run) {
        MLadspaDescriptor->run(MLadspaHandle,ANum);
      }
    }
  }
}

//----------

void S3_Ladspa::run_adding(uint32 ANum) {
  //S3_DTrace("S3_Ladspa::run_adding %i\n",ANum);
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->run_adding) {
        MLadspaDescriptor->run_adding(MLadspaHandle,ANum);
      }
    }
  }
}

//----------

void S3_Ladspa::set_run_adding_gain(float AGain) {
  //S3_DTrace("S3_Ladspa::set_run_adding_gain %f\n",AGain);
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->set_run_adding_gain) {
        MLadspaDescriptor->set_run_adding_gain(MLadspaHandle,AGain);
      }
    }
  }
}

//----------

void S3_Ladspa::deactivate(void) {
  //S3_DTrace("S3_Ladspa::deactivate\n");
  if (MActivated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->deactivate) {
        MLadspaDescriptor->deactivate(MLadspaHandle);
      }
    }
  }
}

//----------

void S3_Ladspa::cleanup(void) {
  //S3_DTrace("S3_Ladspa::cleanup\n");
  if (MInstantiated) {
    if (MLadspaDescriptor) {
      if (MLadspaDescriptor->cleanup) {
        MLadspaDescriptor->cleanup(MLadspaHandle);
        MInstantiated = false;
      }
    }
  }
}

//----------------------------------------------------------------------
#endif












/*
  https://ccrma.stanford.edu/mirrors/lalists/lad/2001/Sep/0014.html

  Hi,
  I would like to know if there is any agreed upon logarithmic function to
  use for LADSPA control ports when the LADSPA_HINT_LOGARITHMIC is set. In
  practice, the function should map display coordinates (the slider) to
  control values.
  The best I have come up with myself is this:
    Control port value, v
    Control port low limit, v_low
    Control port high limit, v_high
    Display coordinates, d
    Display coordinates low limit, d_low
    Display coordinates high limit, d_high
    Transposition to positive range, log_offset
    Setup:
      log_offset = -v_low+1;
      d_low = log(v_low + log_offset);
      d_high = log(v_high + log_offset);
      d = log(v + log_offset);
    Slider moved:
      v = exp(d) - log_offset;
  Marcus
*/

//----------

/*
  http://nwoeanhinnogaehr.github.io/ladspa.rs/ladspa/enum.DefaultValue.html
  Enum ladspa::DefaultValue
  Minimum
    Equal to the lower_bound of the Port.
  Low
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.75 + log(upper_bound) * 0.25)
    Otherwise, this should be
      (lower_bound * 0.75 + upper_bound * 0.25)
  Middle
    For ports with CONTROL_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.5 + log(upper_bound) * 0.5)
    Otherwise, this should be
      (lower_bound * 0.5 + upper_bound * 0.5)
  High
    For ports with LADSPA_HINT_LOGARITHMIC, this should be
      exp(log(lower_bound) * 0.25 + log(upper_bound) * 0.75)
      Otherwise, this should be
      (lower_bound * 0.25 + upper_bound * 0.75)
*/

//----------

/*
  http://lists.linuxaudio.org/pipermail/linux-audio-dev/2006-April/015714.html
  if a is the users parameter UI control value in [0,1], u is the lower
  bound, v is the upper bound, and b is the log base, then:
  port val  = ( (1-a) log(u) + a log(v) )^b
            = ( -a log(u) + log(u) + a log(v) )^b
            = log( ( u+v^a ) / u^a )^b
            = (u + v^a) / u^a
*/
