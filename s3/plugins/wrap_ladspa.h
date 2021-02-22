
/*
  some debugging settings
  (setup these before including "s3.h", etc..
*/

#ifdef S3_DEBUG
  #define S3_DEBUG_MEM
  #define S3_DEBUG_LADSPA
  #ifdef S3_PLUGIN_VST
    #define S3_DEBUG_SOCKET
    //#define S3_DEBUG_VST
  #endif
#endif

//----------------------------------------------------------------------

#include "s3.h"
#include "audio/s3_audio_utils.h"
//#include "audio/s3_ladspa.h"
#include "audio/s3_ladspa.h"
#include "common/s3_file.h"
#include "common/s3_integer.h"
#include "common/s3_parser.h"
#include "common/s3_paths.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

//----------------------------------------------------------------------

static const char* ladspa_text_onoff[] = { "Off","On" };

//----------

class myPlugin : public S3_Plugin {

  private:
    S3_File   MFile;
    S3_Parser MParser;
    S3_Ladspa MLadspa;
    S3_Ladspa MLadspa2;
    bool      MMono;
    float     MControlInputValues[256];
    float     MControlOutputValues[256];
    float     MDummyInput[65536];
    float     MDummyOutput[65536];

  //----------------------------------------
  // ladspa
  //----------------------------------------

  public:

    /*
      load config file from the same directory as the plugin itself,
      but with the .so extension replaced with .cfg
    */

    char* ladspa_config_file(char* buffer) {
      *buffer = '\0';
      char filename[256];
      S3_GetBaseFile(filename);
      S3_StripFileExt(filename);
      S3_Strcat(filename,".cfg");
      if (MFile.open(filename)) {
        uint32 length = MFile.length();
        if (length > 0) {
          MFile.read(buffer,length);
        }
        MFile.close();
        return buffer;
      }
      return S3_NULL;
    }

    //------------------------------

    /*
      setup (create and append) vst parameters
    */

    void ladspa_setup_parameters(void) {
      for (uint32 i=0; i<MLadspa.numControlInputs(); i++) {
        S3_Ladspa_Port* port = MLadspa.controlInputPort(i);
        if (port->flags & s3_lp_toggled) {
          const char* name  = port->name;
          float val         = port->val;
          MParameters.append( new S3_ParamText(name,val,2,ladspa_text_onoff) );
        }
        else if (port->flags & s3_lp_integer) {
          const char* name  = port->name;
          float val         = port->val;
          float minval      = port->minval;
          float maxval      = port->maxval;
          MParameters.append( new S3_ParamInt(name,val,minval,maxval) );
        }
        else {
          const char* name  = port->name;
          float val         = port->val;
          float minval      = port->minval;
          float maxval      = port->maxval;
          MParameters.append( new S3_ParamFloat(name,val,minval,maxval) );
        }
      }
    }

    //------------------------------

    /*
      connect control inputs (parameters) to parameter values,
    */

    void ladspa_connect_control_inputs(void) {
      if (MMono) {
        for (uint32 i=0; i<MLadspa.numControlInputs(); i++) {
          S3_Ladspa_Port* port = MLadspa.controlInputPort(i);
          MControlInputValues[i] = port->val;
          MLadspa.connect_port( MLadspa.controlInputIndex(i), &MControlInputValues[i] );
          port = MLadspa2.controlInputPort(i);
          MLadspa2.connect_port( MLadspa2.controlInputIndex(i), &MControlInputValues[i] );
        }
      }
      else {
        for (uint32 i=0; i<MLadspa.numControlInputs(); i++) {
          S3_Ladspa_Port* port = MLadspa.controlInputPort(i);
          MControlInputValues[i] = port->val;
          MLadspa.connect_port( MLadspa.controlInputIndex(i), &MControlInputValues[i] );
        }
      }
    }

    //------------------------------

    /*
      connect control outputs (not used) to dummy array
    */

    void ladspa_connect_control_outputs(void) {
      if (MMono) {
        for (uint32 i=0; i<MLadspa.numControlOutputs(); i++) {
          MLadspa.connect_port( MLadspa.controlOutputIndex(i), &MControlOutputValues[i] );
          MLadspa2.connect_port( MLadspa2.controlOutputIndex(i), &MControlOutputValues[i] );
        }
      }
      else {
        for (uint32 i=0; i<MLadspa.numControlOutputs(); i++) {
          MLadspa.connect_port( MLadspa.controlOutputIndex(i), &MControlOutputValues[i] );
        }
      }
    }

  //----------------------------------------
  // plugin
  //----------------------------------------
  public:

    /*
      called just after the plugin has been created..
      we need to tell the vst host about parameters, number of inputs
      and outputs etc, so we examine the ladspa_descriptor and
      prepare things..
    */

    //virtual
    void on_create(void) final {
      S3_Memset(MDummyInput,0,sizeof(MDummyInput));
      S3_Memset(MDummyOutput,0,sizeof(MDummyOutput));
      const char* filename = "";// = "/usr/lib/ladspa/guitarix_IR.so";
      uint32 index = 0;
      char buffer[256];
      char* config = ladspa_config_file(buffer);
      MParser.parse(config,S3_Strlen(config));
      //filename = MParser.token(0);
      //if (MParser.numTokens() == 2) index = S3_StringToInt(MParser.token(1));
      uint32 i = 0;
      while (i < MParser.numTokens()) {
        const char* t = MParser.token(i++);
        if (!S3_Strcmp(t,"filename")) filename = MParser.token(i++);
        if (!S3_Strcmp(t,"index")) index = S3_StringToInt(MParser.token(i++));
      }
      MLadspa.load(filename);
      //MLadspa.init();
      MLadspa.initDescriptor(index);
      MLadspa.initPorts();
      MMono = false;
      if ((MLadspa.numAudioInputs() == 1) && (MLadspa.numAudioOutputs() == 1)) {
        MMono = true;
        MLadspa2.load(filename);
        //MLadspa2.init();
        MLadspa2.initDescriptor(index);
        MLadspa2.initPorts();
      }
      MName         = MLadspa.name();
      MAuthor       = MLadspa.maker();
      MDescription  = MName;
      if (MMono) {
        MNumInputs  = 2;
        MNumOutputs = 2;
      }
      else {
        MNumInputs  = MLadspa.numAudioInputs();
        MNumOutputs = MLadspa.numAudioOutputs();
      }
      ladspa_setup_parameters();
    }

    //------------------------------

    /*
      called just before the plugin will be deleted..
      we need to cleanup and delete the ladspa instance..
    */

    //virtual
    void on_destroy(void) final {
      if (MInitialized) {
        MLadspa.cleanup();
        if (MMono) MLadspa2.cleanup();
      }
      MLadspa.unload();
      if (MMono) MLadspa2.unload();
    }

    //------------------------------

    //virtual
    //void on_idle(void) final {
    //  S3_TRACE;
    //}

    //------------------------------

    /*
      s3_ps_initialize
        called the first time resume is executed.. create the ladspa
        instance, and connect control ports (audio ports connected
        in on_processBlock()
      s3_ps_resume
        called when power is turned on for the plugin..
        activate the ladspa plugin
      s3_ps_suspend
        called when power is turned off for the plugin..
        deactivate the ladspa plugin
    */

    //virtual
    void on_stateChange(uint32 AState) final {
      switch(AState) {
        case s3_ps_initialize:
          MLadspa.instantiate(MSampleRate);
          if (MMono) MLadspa2.instantiate(MSampleRate);
          ladspa_connect_control_inputs();
          ladspa_connect_control_outputs();
          break;
        case s3_ps_resume:
          MLadspa.activate();
          if (MMono) MLadspa2.activate();
          break;
        case s3_ps_suspend:
          MLadspa.deactivate();
          if (MMono) MLadspa2.deactivate();
          break;
      }
    }

    //------------------------------

    /*
      ladspa control input ports are 'connected' to an internal
      array of values, so we just copy the new parameter value
      to the correct entry in this array (and the ladspa plugin
      will pick it up from there)..
    */

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      MControlInputValues[AIndex] = AValue;
    }

    //------------------------------

    /*
      process one block of audio.. pointers (the inputs/outputs arguments)
      might be different for each call, so we (re-)connect the audio
      in/out ports before we call 'run'
    */

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float* in0 = AInputs[0];
      float* in1 = AInputs[1];
      float* out0 = AOutputs[0];
      float* out1 = AOutputs[1];
      if (MMono) {
        MLadspa.connect_port(  MLadspa.audioInputIndex(0),   in0 );
        MLadspa.connect_port(  MLadspa.audioOutputIndex(0),  out0 );
        MLadspa2.connect_port( MLadspa2.audioInputIndex(0),  in1 );
        MLadspa2.connect_port( MLadspa2.audioOutputIndex(0), out1 );
        MLadspa.run(ANumSamples);
        MLadspa2.run(ANumSamples);
      }
      else {
        uint32 num_in = MLadspa.numAudioInputs();
        uint32 num_out = MLadspa.numAudioOutputs();
        if (num_in  > 0) MLadspa.connect_port( MLadspa.audioInputIndex(0),  in0 );
        if (num_in  > 1) MLadspa.connect_port( MLadspa.audioInputIndex(1),  in1 );
        if (num_in  > 2) for (uint32 i=2; i<num_in; i++) MLadspa.connect_port( MLadspa.audioInputIndex(i), MDummyInput );
        if (num_out > 0) MLadspa.connect_port( MLadspa.audioOutputIndex(0), out0 );
        if (num_out > 1) MLadspa.connect_port( MLadspa.audioOutputIndex(1), out1 );
        if (num_out > 2) for (uint32 i=2; i<num_out; i++) MLadspa.connect_port( MLadspa.audioOutputIndex(i), MDummyOutput );
        MLadspa.run(ANumSamples);
      }
    }

};

//----------------------------------------------------------------------

/*
  this sets everything 'in motion'..
  defines the main entry point of the executable or library (.so),
  and handles the difference between the various formats
  (currently only exe and vst)
*/

S3_MAIN(myPlugin);
