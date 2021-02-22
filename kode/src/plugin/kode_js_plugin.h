#ifndef kode_js_plugin_included
#define kode_js_plugin_included
//----------------------------------------------------------------------

/*
  simplifies porting from jesusonic plugins/scripts..
*/

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

#define min KODE_Min
#define max KODE_Max
#define pi  KODE_PI

//----------------------------------------------------------------------

class KODE_JSDescriptor : public KODE_Descriptor {

public:

  KODE_JSDescriptor()
  : KODE_Descriptor() {
    
    MName       = "KODE_JsDescriptor";
    MAuthor     = "KODE";
    MNumInputs  = 2;
    MNumOutputs = 2;
  }
  
  //----------
  
  virtual ~KODE_JSDescriptor() {
    //KODE_TRACE;
  }
  
public:

  void desc(const char* AName) {
    MName = AName;
  }
  
  void gfx(uint32_t AWidth, uint32_t AHeight) {
    // TODO
  }
  
  /*
    AIndex is ignored..
  */
  
  void slider(/*uint32_t AIndex,*/ const char* AName, float AValue, float AMin, float AMax, float AStep=0.0f) {
    appendParameter(KODE_New KODE_FloatParameter(AName,AValue,AMin,AMax,AStep));
  }
  
};

//----------------------------------------------------------------------

#define MEM_SIZE (8 * 1024 * 1024)

class KODE_JSInstance : public KODE_Instance {
  
//------------------------------
private:
//------------------------------

  bool      update_params   = false;
  bool      need_init       = true;
  uint32_t  prev_playstate  = KODE_PLUGIN_PLAYSTATE_NONE;
  float     prev_srate      = 666.0f;
  bool      is_active       = false;
  
//------------------------------
protected:
//------------------------------

  float     srate           = prev_srate;//666.0f;
  uint32_t  ext_noinit      = 0;

  uint32_t  samplesblock    = 0;
  float     spl0            = 0.0f;
  float     spl1            = 0.0f;
  float     slider[64]      = {0.0f};
  float     MEM[MEM_SIZE]   = {0.0f};
  
//------------------------------
public:
//------------------------------

  KODE_JSInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
  }
  
  virtual ~KODE_JSInstance() {
    //KODE_TRACE;
  }

//------------------------------
public:
//------------------------------

  //@init: effect load
  //@init: samplerate changes
  //@init: start of playbac
  //@init: if ext_noinit != 1
  //@slider: following an @init
  //@slider: when a parameter (slider) changes

  /*
  @init
  The code in the @init section gets executed on effect load, on samplerate
  changes, and on start of playback.
  //If you wish this code to not execute on start of playback, you can set
  //ext_noinit to 1.0, and it will only execute on load or samplerate change
  //(and not on playback start/stop). 
  */

  virtual void on_init() {}
  
  /*
  @slider
  The code in the @slider section gets executed following an @init, or when
  a parameter (slider) changes. Ideally code in here should detect when a
  slider has changed, and adapt to the new parameters (ideally avoiding clicks
  or glitches). The parameters defined with sliderX: can be read using the
  variables sliderX.
  */
  
  virtual void on_slider() {}
  
  /*
  @block
  The code in the @block section is executed before processing each sample
  block. Typically a block is whatever length as defined by the audio hardware,
  or anywhere from 128-2048 samples. In this code section the samplesblock
  variable will be valid (and set to the size of the upcoming block). 
  */
  
  virtual void on_block() {}
  
  /*
  @sample
  The code in the @sample section is executed for every PCM audio sample.
  This code can analyze, process, or synthesize, by reading, modifying, or
  writing to the variables spl0, spl1, ... spl63. 
  */
  
  virtual void on_sample() {}
  
  /*
  @gfx [width] [height]
  The @gfx section gets executed around 30 times a second when the plug-ins
  GUI is open.  
  */
  
  virtual void on_gfx() {}

//------------------------------
public:
//------------------------------

  //void on_open() final {
  //  KODE_Trace("getSampleRate() %.1f\n",getListener()->getSamplerate());
  //}
  
  //----------
    
  //void on_initialize() final {
  //  KODE_Trace("getSampleRate() %.1f\n",getListener()->getSamplerate());
  //}
  
  //----------
    
  void on_activate() final {
    //KODE_TRACE;
    is_active = true;
    KODE_Assert( getListener() );
    srate = getListener()->getSamplerate();
    //@init: effect load (@slider: following an @init)
    on_init();
    on_slider();
  }

  //----------
    
  void on_startUpdateAllParameters() final {
    //KODE_TRACE;
    update_params = true;
  }
  
  //----------
    
  void on_endUpdateAllParameters() final {
    //KODE_TRACE;
    update_params = false;
    if (is_active) {
      on_slider();
    }
  }
  
  //----------
    
  void on_parameterChange(uint32_t AIndex, float AValue) final {
    //if (is_active) {
      if (slider[AIndex+1] != AValue) {
        slider[AIndex+1] = AValue;
        if (!update_params) on_slider();
      }
    //}
  }
    
  //----------
    
  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
  //}
  
  //----------
    
  void on_process(KODE_ProcessContext* AContext) final {
    //@init: samplerate changes (@slider: following an @init)
    if (AContext->samplerate != srate) {
      on_init();
      on_slider();
    }
    srate = AContext->samplerate;
    //@init: start of playback (@slider: following an @init)
    //@init: if ext_noinit != 1
    if (ext_noinit != 1) {
      if (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING) {
        if (!prev_playstate & KODE_PLUGIN_PLAYSTATE_PLAYING) {
          on_init();
          on_slider();
        }
      }
    }
    prev_playstate = AContext->playstate;
    // In this code section the samplesblock variable will be valid
    // (and set to the size of the upcoming block)
    samplesblock = AContext->numSamples;
    on_block();
    uint32_t  len   = AContext->numSamples;
    float*    in0   = AContext->inputs[0];
    float*    in1   = AContext->inputs[1];
    float*    out0  = AContext->outputs[0];
    float*    out1  = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      spl0 = *in0++;
      spl1 = *in1++;
      on_sample();
      *out0++ = spl0;
      *out1++ = spl1;
    }
  }
  
  //----------
    
};

//----------------------------------------------------------------------
#endif
