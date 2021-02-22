#ifndef fx_freeze_included
#define fx_freeze_included
//----------------------------------------------------------------------

/*
  todo:
  . multiple midi events per block..
    (array & count in SMidiTiming class))
*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- format -----

//#define SKEI_EXE
//#define SKEI_XLIB
//#define SKEI_XRENDER

//----- debug -----

//#define SKEI_DEBUG
//#define SKEI_DEBUG_MEM
//#define SKEI_DEBUG_MEM_PRINT
//#define SKEI_DEBUG_VST

#ifdef S3_PLUGIN_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----- plugins -----

#define SKEI_PLUGIN_PER_SAMPLE
//#define SKEI_PLUGIN_HAS_EDITOR
//#define SKEI_PLUGIN_IS_SYNTH
//#define SKEI_PLUGIN_SEND_MIDI
#define SKEI_PLUGIN_RECEIVE_MIDI
//#define SKEI_PLUGIN_AUTOSYNC

//----------------------------------------------------------------------

//#define BUFFER_SIZE (1024*1024)
#define BUFFER_SIZE (1<<20)
#define BUFFER_MASK (BUFFER_SIZE-1)

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "s3.h"
#include "plugin/s3_plugin.h"

//#include "skei_midi_timing.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public SPlugin
, public SMidiTiming_Listener
{

  private:
    SMidiTiming*  midi_timing;
  private:
    float         buffer[BUFFER_SIZE*2];
    bool          loop_active;
    uint32        loop_start;
    float         loop_size;
    float         loop_pos;
    float         loop_speed;
    //bool          write_active;
    uint32        write_pos;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {

      MInfo.name       = "fx_freeze";
      MInfo.author     = "skei";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      midi_timing = new SMidiTiming(this);
      //
      SMemset(buffer,0,sizeof(buffer));
      loop_active   = false;
      loop_start    = 0;
      loop_size     = 0.0f;
      loop_pos      = 0.0f;
      loop_speed    = 1.0f;
      //write_active  = false;
      write_pos     = 0;
    }

    //----------

    //virtual
    void on_destroy(void) {
      delete midi_timing;
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    //virtual
    void on_stateChange(uint32 AState) {
    }

    //----------

    //virtual
    void on_transportChange(uint32 ATransport) {
    }

    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
    }

    //----------

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      midi_timing->event(AOffset,AMsg1,AMsg2,AMsg3);
    }

    //----------

    //virtual
    void on_programChange(int32 AIndex) {
    }

    //----------

    //virtual
    void on_preProgram(int32 AIndex) {
    }

    //----------

    //virtual
    void on_postProgram(void) {
    }

    //----------

    //virtual
    void on_miditiming_event(uint32 msg, uint32 chn, uint32 idx, uint32 val) {
      //DTrace("midi.. msg:%i chn:%i idx:%i val:%i\n",msg,chn,idx,val);
      //if ((msg==0x90) && (val==0)) msg = 0x80;
      if (msg==0x90) { // note on
        float freq  = 440.0f * powf(2.0f,(idx-69.0f)/12.0f);
        loop_active = true;
        loop_size   = MState.sampleRate / freq;
        loop_start  = write_pos - loop_size;
        loop_pos    = 0.0f;
        loop_speed  = 1.0f;
      }
      else if (msg==0x80) { // note off
        loop_active = false;
      }
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ANumSamples) {
      midi_timing->preProcess();
    }

    //----------

    //virtual
    void on_processSample(SSample** AInputs, SSample** AOutputs) {
      float in0 = *AInputs[0];
      float in1 = *AInputs[1];
      float out0 = 0;
      float out1 = 0;

      midi_timing->process();

      /*
        set by midi note:
          loop_active, loop_start, loop_size
          read_pos, read_speed
      */

      if (loop_active) {
        uint32 r2 = loop_start + loop_pos;
        r2 &= BUFFER_MASK;
        out0 = buffer[r2];
        out1 = buffer[r2+1];
        loop_pos += loop_speed;
        loop_pos = SModulo(loop_pos,loop_size);
      }
      else {
        out0 = in0;
        out1 = in1;
      }

      /*
        write to buffer
      */

      uint32 w2 = write_pos * 2;
      buffer[w2] = in0;
      buffer[w2+1] = in1;
      write_pos = (write_pos+1) & BUFFER_MASK;

      *AOutputs[0] = out0;
      *AOutputs[1] = out1;
    }

    //----------

    //virtual
    void on_postProcess(void) {
      midi_timing->postProcess();
    }

    //------------------------------------------------------------
    // editor
    //------------------------------------------------------------

    //virtual
    void* on_openEditor(void* AParent) {
      return SKEI_NULL;
    }

    //----------

    //virtual
    void on_closeEditor(void* AEditor) {
    }

    //----------

    //virtual
    void on_idleEditor(void* AEditor) {
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif





