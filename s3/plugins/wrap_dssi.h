
/*
  some debugging settings
  (setup these before including "s3.h", etc..
*/

#ifdef S3_DEBUG
  #define S3_DEBUG_MEM
  //#define S3_DEBUG_LADSPA
  //#define S3_DEBUG_DSSI
  #ifdef S3_PLUGIN_VST
    #define S3_DEBUG_SOCKET
    //#define S3_DEBUG_VST
  #endif
#endif

//----------------------------------------------------------------------

#include "s3.h"
#include "audio/s3_audio_utils.h"
#include "audio/s3_dssi.h"
#include "common/s3_file.h"
#include "common/s3_integer.h"
#include "common/s3_parser.h"
#include "common/s3_paths.h"
#include "plugin/s3_plugin.h"

//----------------------------------------------------------------------

static const char* dssi_text_onoff[] = { "Off","On" };

//----------

class myPlugin : public S3_Plugin {

  private:
    S3_File   MFile;
    S3_Parser MParser;
    S3_Dssi   MDssi;
    float     MControlInputValues[256];
    float     MControlOutputValues[256];
    float     MDummyInput[65536];
    float     MDummyOutput[65536];

    uint32    MNumPrograms;

  //----------------------------------------
  // dssi
  //----------------------------------------

  public:

    /*
      load config file with the same name as the plugin itself, from the
      same directory, but with the .so extension replaced with .cfg
    */

    char* dssi_load_config_file(char* buffer) {
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

    //----------

    /*
      setup (create and append) vst parameters
    */

    void dssi_setup_parameters(void) {
      for (uint32 i=0; i<MDssi.numControlInputs(); i++) {
        S3_Ladspa_Port* port = MDssi.controlInputPort(i);
        if (port->flags & s3_lp_toggled) {
          const char* name  = port->name;
          float val         = port->val;
          MParameters.append( new S3_ParamText(name,val,2,dssi_text_onoff) );
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

    //----------

    /*
      must be called after dssi plugin is instantiated..
    */

    void dssi_setup_programs(void) {
      MDssi.initPrograms();
      for (uint32 i=0; i<MDssi.numPrograms(); i++) {
        const char* name = MDssi.getProgramName(i);;
        MPrograms.append( new S3_Program(name) );

      }
    }

    //----------

    /*
      connect control inputs (parameters) to parameter values,
    */

    void dssi_connect_control_inputs(void) {
      for (uint32 i=0; i<MDssi.numControlInputs(); i++) {
        S3_Ladspa_Port* port = MDssi.controlInputPort(i);
        MControlInputValues[i] = port->val;
        MDssi.connect_port( MDssi.controlInputIndex(i), &MControlInputValues[i] );
      }
    }

    //----------

    /*
      connect control outputs (not used) to dummy array
    */

    void dssi_connect_control_outputs(void) {
      for (uint32 i=0; i<MDssi.numControlOutputs(); i++) {
        MDssi.connect_port( MDssi.controlOutputIndex(i), &MControlOutputValues[i] );
      }
    }

  //----------------------------------------
  // plugin
  //----------------------------------------

  public:

    /*
      called just after the plugin has been created..
      we need to tell the vst host about parameters, number of inputs
      and outputs etc, so we examine the dssi_descriptor and
      prepare things..
    */

    //virtual
    void on_create(void) final {
      S3_Memset(MDummyInput,0,sizeof(MDummyInput));
      S3_Memset(MDummyOutput,0,sizeof(MDummyOutput));

      char        config_buffer[256];
      char*       config        = dssi_load_config_file(config_buffer);
      const char* filename      = "";
      uint32      index         = 0;
      uint32      i             = 0;

      MParser.parse(config,S3_Strlen(config));
      while (i < MParser.numTokens()) {
        const char* t = MParser.token(i++);
        if (!S3_Strcmp(t,"filename"))     filename      = MParser.token(i++);
        if (!S3_Strcmp(t,"index"))        index         = S3_StringToInt(MParser.token(i++));
      }

      MDssi.load(filename);
      MDssi.initDescriptor(index);
      MDssi.initPorts();
      dssi_setup_parameters();

      MDssi.instantiate(44100);
      dssi_setup_programs();
      MDssi.cleanup();

      MFlags       |= (s3_pf_receiveMidi | s3_pf_synth);
      MName         = MDssi.name();
      MAuthor       = MDssi.maker();
      MDescription  = MName;
      MNumInputs    = 0;//MDssi.numAudioInputs();
      MNumOutputs   = 2;//MDssi.numAudioOutputs();
    }

    //------------------------------

    /*
      called just before the plugin will be deleted..
      we need to cleanup and delete the dssi instance..
    */

    //virtual
    void on_destroy(void) final {
      if (MInitialized) MDssi.cleanup();
      MDssi.unload();
    }

    //------------------------------

    //virtual
    //void on_idle(void) final {
    //  S3_TRACE;
    //}

    //------------------------------

    /*
      s3_ps_initialize
        called the first time resume is executed.. create the dssi
        instance, and connect control ports (audio ports connected
        in on_processBlock()
      s3_ps_resume
        called when power is turned on for the plugin..
        activate the dssi plugin
      s3_ps_suspend
        called when power is turned off for the plugin..
        deactivate the dssi plugin
    */

    //virtual
    void on_stateChange(uint32 AState) final {
      //S3_Trace("state: %i\n",AState);
      switch(AState) {
        case s3_ps_open:
          S3_Trace("open\n");
          break;
        case s3_ps_close:
          S3_Trace("close\n");
          break;
        case s3_ps_initialize:
          S3_Trace("initialize\n");
          MDssi.instantiate(MSampleRate);
          //MDssi.dumpDssiDescriptor();
          //MDssi.dumpLadspaDescriptor();
          dssi_connect_control_inputs();
          dssi_connect_control_outputs();
          break;
        case s3_ps_resume:
          S3_Trace("resume\n");
          MDssi.resetMidi();
          MDssi.activate();
          MDssi.setProgram( MCurrentProgram );
          break;
        case s3_ps_suspend:
          S3_Trace("suspend\n");
          MDssi.deactivate();
          break;
        case s3_ps_start:
          S3_Trace("start\n");
          break;
        case s3_ps_stop:
          S3_Trace("stop\n");
          break;
      }
    }

    //------------------------------

    /*
      dssi control input ports are 'connected' to an internal
      array of values, so we just copy the new parameter value
      to the correct entry in this array (and the dssi plugin
      will pick it up from there)..
    */

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      //S3_TRACE;
      MControlInputValues[AIndex] = AValue;
    }

    //------------------------------

    //virtual
    void on_programChange(int32 AIndex) final {
      //S3_TRACE;
      MDssi.setProgram(AIndex);
      for (uint32 i=0; i<MParameters.size(); i++) {
        S3_Parameter* param = MParameters[i];
        float v = param->to01( MControlInputValues[i] );
        notifyHostParameterChanged(i,v);
        MHostParamValues[i] = v;
      }
      // send on_parameterChange() for modified parameters (start of process())
      //distributeParamValues();
    }

    //------------------------------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) final {
      MDssi.midiEvent(AOffset,AMsg1,AMsg2,AMsg3);
    }


    //------------------------------

    /*
      process one block of audio.. pointers (the inputs/outputs arguments)
      might be different for each call, so we (re-)connect the audio
      in/out ports before we call 'run'
    */

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {

      S3_Assert( MNumOutputs==1 || MNumOutputs==2 );

      //float* in0 = AInputs[0];
      //float* in1 = AInputs[1];
      float* out0 = AOutputs[0];
      float* out1 = AOutputs[1];
      switch(MNumOutputs) {
        case 1:
          MDssi.connect_port( MDssi.audioOutputIndex(0), out0 );
          MDssi.run_synth(ANumSamples);
          S3_CopyMonoBuffer( out1, out0, ANumSamples );
          break;
        case 2:
          MDssi.connect_port( MDssi.audioOutputIndex(0), out0 );
          MDssi.connect_port( MDssi.audioOutputIndex(1), out1 );
          MDssi.run_synth(ANumSamples);
          break;
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

