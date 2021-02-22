#ifndef template_included
#define template_included
//----------------------------------------------------------------------

/*
  performance tests:

  debug build:

  * 10.000 parameters
    samplerate: 44100
    buffersize: 128
      deactivated = 5.5%
      activated   = 7.5%


*/

//----------------------------------------------------------------------

//#define TEST_NUM_PARAMS 10000

#define S3_PLUGIN_SEND_MIDI
#define S3_PLUGIN_RECEIVE_MIDI
#define S3_PLUGIN_PER_MIDI_EVENT

//----------

#define S3_DEBUG_MEM
#define S3_DEBUG_PROCESS

#ifdef S3_PLUGIN_VST
  #define S3_DEBUG_SOCKET
  #define S3_DEBUG_VST
#endif


//----------

#include "s3.h"
#include "plugin/s3_plugin.h"

//----------------------------------------------------------------------

class myPlugin : public S3_Plugin {

  public:

    void on_create(void) final {
      //S3_TRACE;
      MName         = "name";
      MAuthor       = "author";
      MDescription  = "description"; // MName;
      MVersion      = 0;
      MUniqueId     = S3_MAGIC_USER + 0x0000;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      //MLatency      = 0;
      //MTail         = 0;

      appendParameter( new S3_Parameter("param1",0.1) );

      // test performance
      //for (uint32 i=0; i<TEST_NUM_PARAMS; i++) {
      //  appendParameter( new S3_Parameter("param",0.1) );
      //}

    }

    void on_destroy(void) final {
      //S3_TRACE;
    }

    void  on_stateChange(uint32 AState) final {
      /*
      S3_Trace("on_stateChange: ");
      switch(AState) {
        case s3_ps_open:        S3_DTrace("open\n");       break;
        case s3_ps_close:       S3_DTrace("close\n");      break;
        case s3_ps_sampleRate:  S3_DTrace("sampleRate (%.2f)\n",MSampleRate); break;
        case s3_ps_blockSize:   S3_DTrace("blockSize (%i)\n",MBlockSize);     break;
        case s3_ps_suspend:     S3_DTrace("suspend\n");    break;
        case s3_ps_resume:      S3_DTrace("resume\n");     break;
        case s3_ps_initialize:  S3_DTrace("initialize\n"); break;
        case s3_ps_start:       S3_DTrace("start\n");      break;
        case s3_ps_stop:        S3_DTrace("stop\n");       break;
        case s3_ps_bypass:      S3_DTrace("bypass\n");     break;
        case s3_ps_bypassOff:   S3_DTrace("bypassOff\n");  break;
        default:                S3_DTrace("[unknown]\n");  break;
      }
      */
    }

    void on_parameterChange(int32 AIndex, float AValue) final {
      //S3_Trace("%i : %.3f\n",AIndex,AValue);
    }

    void on_programChange(int32 AIndex) final {
      //S3_Trace("%i\n",AIndex);
    }

    void on_midiEvents(S3_MidiEvent* AMidiEvents, uint32 ANumEvents) final {
      //S3_TRACE;
    }

    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) final {
      //S3_Trace("%i : %03i %03i %03i\n",AOffset,AMsg1,AMsg2,AMsg3);
    }

    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
    }

    void on_processSample(S3_Sample** AInputs, S3_Sample** AOutputs) final {
    }

    void on_postProcess(void) final {
    }

    void* on_openEditor(void* AParent) final {
      //S3_TRACE;
      return S3_NULL;
    }

    bool on_closeEditor(void* AEditor) final {
      //S3_TRACE;
      return false;
    }

    void on_idleEditor(void* AEditor) final {
      //S3_TRACE;
    }

};

//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

/*
  prints: (bitwig, linux)

  create

  [template.h:on_create:24]
  [template.h:on_stateChange:54] on_stateChange: open
  [template.h:on_stateChange:54] on_stateChange: sampleRate (44100.00)
  [template.h:on_stateChange:54] on_stateChange: blockSize (256)
  [template.h:on_stateChange:54] on_stateChange: initialize
  [template.h:on_stateChange:54] on_stateChange: resume
  [template.h:on_stateChange:54] on_stateChange: start

  deactivate

  [template.h:on_stateChange:54] on_stateChange: stop
  [template.h:on_stateChange:54] on_stateChange: suspend

  activate

  [template.h:on_stateChange:54] on_stateChange: resume
  [template.h:on_stateChange:54] on_stateChange: start

  delete

  [template.h:on_stateChange:54] on_stateChange: stop
  [template.h:on_stateChange:54] on_stateChange: suspend
  [template.h:on_stateChange:54] on_stateChange: close
  [template.h:on_destroy:50]

*/
