#ifndef midi_fibonacci_included
#define midi_fibonacci_included
//----------------------------------------------------------------------

/*
  changes notes based on the fibonacci series

  version history:
  0: initial release (preview)
  1: changed from note-creation to note-modification
  2: adapted to skeilib
*/

//----------------------------------------------------------------------

//#define S3_DEBUG_MEM
#ifdef S3_PLUGIN_VST
  //#define S3_DEBUG_VST
  #define S3_DEBUG_SOCKET
#endif

//----------

#include "s3.h"
#include "audio/s3_audio_utils.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

//----------------------------------------------------------------------

static const char* txt_dir[] = {"positive","negative"};

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public S3_Plugin {

  private:
    int32 MNoteBuffer[128*16];
    int32 MChanBuffer[128*16];
    int32 MNum1;
    int32 MNum2;
  private:
    int32 p_num1;
    int32 p_num2;
    int32 p_mod;
    int32 p_dir;

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void _fib_init(void) {
      //S3_Trace("_fib_init\n");
      MNum1 = p_num1;
      MNum2 = p_num2;
      for (int32 chan=0; chan<16; chan++) {
        for (int32 note=0; note<128; note++) {
          int32 index = (chan*128)+note;
          MNoteBuffer[index] = -1;
          MChanBuffer[index] = 0;
        }
      }
      //S3_Trace("_fib_init ok\n");
    }

    //----------

    /*
      aha!
      midi is processed before process(), and since we check the transport in the
      start of process, there is already a note playing, and it will be shut off
    */

    void _fib_reset(void) {
      //S3_Trace("_fib_reset\n");
      MNum1 = p_num1;
      MNum2 = p_num2;
      for (int32 chan=0; chan<16; chan++) {
        for (int32 note=0; note<128; note++) {
          int32 index = (chan*128)+note;
          int32 n = MNoteBuffer[index];
          int32 c = MChanBuffer[index];
          if (n != -1) sendMidiToHost(0,0x80+c,n,0); // $80 = note off
          MNoteBuffer[index] = -1;
          MChanBuffer[index] = 0;
        }
      }
      //S3_Trace("_fib_reset ok\n");
    }

    //----------

    int32 _fib_next(void) {
      //S3_Trace("_fib_next\n");
      int32 next;
      if (p_dir < 1) next = (MNum1 + MNum2) % p_mod;
      else next = (MNum1 - MNum2) % p_mod;
      int32 result = MNum1 % p_mod; // mod to catch 1-note length series.. 0,1,<next>
      MNum1 = MNum2; // 0 -> 1
      MNum2 = next;  // 1 -> 1
      //S3_Trace("_fib_next ok\n");
      return result;
    }

    //----------

    void _noteOn(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      //S3_Trace("_noteOn\n");
      int32 chan = AMsg1 & 0x0f;
      int32 note = AMsg2 & 0x7f;
      int32 index = (chan*128)+note;
      int32 n = note + _fib_next();
      int32 c = chan;
      MNoteBuffer[index] = n;
      MChanBuffer[index] = c;
      sendMidiToHost(AOffset,0x90+c,n,AMsg3); // $90 = note on
      //S3_Trace("_noteOn ok\n");
    }

    //----------

    void _noteOff(uint32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
      //S3_Trace("_noteOff\n");
      int32 chan = AMsg1 & 0x0f;
      int32 note = AMsg2 & 0x7f;
      int32 index = (chan*128)+note;
      int32 n = MNoteBuffer[index];
      int32 c = MChanBuffer[index]; // AMsg1 and 15;
      if (n != -1) sendMidiToHost(AOffset,0x80+c,n,AMsg3); // $80 = note off
      else sendMidiToHost(AOffset,AMsg1,AMsg2,AMsg3);
      MNoteBuffer[index] = -1;
      MChanBuffer[index] = 0;
      //S3_Trace("_noteOff ok\n");
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) final {
      MName         = "midi_fibonacci";
      MAuthor       = "tor.helge.skei";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_midi_fibonacci_id;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      MFlags |= s3_pf_sendMidi;
      MFlags |= s3_pf_receiveMidi;
      //MFlags |= s3_pf_tool;
      //MEditorRect.set(220+90+40,100);
      appendParameter( new S3_ParamInt( "num1", 0,  -24, 24 ));
      appendParameter( new S3_ParamInt( "num2", 1,  -24, 24 ));
      appendParameter( new S3_ParamInt( "mod",  12,  1,  48 ));
      appendParameter( new S3_ParamText("dir",  0,   2,  txt_dir ));
      _fib_init();
    }

    //----------

    //virtual
    //void on_transportChange(uint32 ATransport) {
    //  if ((ATransport & s3_pt_play) == 0) { _fib_reset(); }
    //}

    //----------

    //virtual
    //void  on_stateChange(uint32 AState) final {
    //  switch(AState) {
    //    case s3_ps_resume:
    //      _fib_reset();
    //      break;
    //  }
    //}


    //----------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      float v = AValue;
      int32 i = S3_Trunc(v);
      switch(AIndex) {
        case 0:
          p_num1 = i;
          MNum1 = i;
          break;
        case 1:
          p_num2 = i;
          MNum2 = i;
          break;
        case 2:
          p_mod = i;
          _fib_reset();
          break;
        case 3:
          p_dir = i;
          _fib_reset();
          break;
      }
    }

    //----------

    //virtual
    void on_midiEvents(S3_MidiEvent* AEvents, uint32 ANum) final {
      for (uint32 i=0; i<ANum; i++) {
        S3_Trace("midi events: %i\n",ANum);
        uint32 ofs  = AEvents[i].offset;
        uint8  msg1 = AEvents[i].msg1;
        uint8  msg2 = AEvents[i].msg2;
        uint8  msg3 = AEvents[i].msg3;
        on_midiEvent(ofs,msg1,msg2,msg3);
      }
    }

    //virtual
    void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) final {
      switch(AMsg1 & 0xF0) {
        case 0x80:
          _noteOff(AOffset,AMsg1,AMsg2,AMsg3);
          break;
        case 0x90:
          _noteOn(AOffset,AMsg1,AMsg2,AMsg3);
          break;
        default:
          sendMidiToHost(AOffset,AMsg1,AMsg2,AMsg3);
          break;
      }
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      S3_CopyStereoBuffer(AOutputs,AInputs,ANumSamples);
      //S3_Memcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(S3_Sample));
      //S3_Memcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(S3_Sample));
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif





