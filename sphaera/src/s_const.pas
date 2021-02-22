unit s_const;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const

  //------------------------------
  // s_canvas
  //------------------------------

  // text alignment
  sta_none    = 0;
  sta_left    = 1;
  sta_right   = 2;
  sta_top     = 3;
  sta_bottom  = 4;
  sta_center  = 5;

  //------------------------------
  // s_filter_rbj
  //------------------------------

  sft_lowpass       = 0;
  sft_hipass        = 1;
  sft_bandpass_csg  = 2;
  sft_bandpass_czpg = 3;
  sft_notch         = 4;
  sft_allpass       = 5;
  sft_peaking       = 6;
  sft_lowshelf      = 7;
  sft_hishelf       = 8;

  //------------------------------
  // s_parameter
  //------------------------------

  // parameter flags
  //spf_none      = 0;  // name 'conflict' with plugin flags
  spf_automate  = 1;    // parameter automatable...

  // parameter types
  spt_parameter   = 0;
  spt_paramFloat  = 1;
  spt_paramInt    = 2;
  spt_paramText   = 3;

  //------------------------------
  // s_plugin
  //------------------------------

  // host cando
  shc_none                            = 0;
  shc_sendVstEvents                   = 1;
  shc_sendVstMidiEvent                = 2;
  shc_sendVstTimeInfo                 = 4;
  shc_receiveVstEvents                = 8;
  shc_receiveVstMidiEvent             = 16;
  shc_reportConnectionChanges         = 32;
  shc_acceptIOChanges                 = 64;
  shc_sizeWindow                      = 128;
  shc_offline                         = 256;
  shc_openFileSelector                = 512;
  shc_closeFileSelector               = 1024;
  shc_startStopProcess                = 2048;
  shc_shellCategory                   = 4096;
  shc_sendVstMidiEventFlagIsRealtime  = 8192;

  // host id

  shi_unknown   = 0;
  shi_reaper    = 1;
  shi_energyxt  = 2;
  shi_live      = 3;
  shi_fruity    = 4;

  // plugin flags
  spf_none        = $00;
  spf_perSample   = $01;  // call on_processSample per sample
  spf_sendMidi    = $02;
  spf_receiveMidi = $04;
  spf_hasEditor   = $08;
  spf_isSynth     = $10;
  spf_autoSync    = $20;  // automatically handles getTime and on_transportChange
  spf_reaper      = $40;  // has/uses reaper support

  // plugin state
  sps_open        = 1;
  sps_close       = 2;
  sps_sampleRate  = 3;
  sps_blockSize   = 4;
  sps_suspend     = 5;
  sps_resume      = 6;
  sps_start       = 7;
  sps_stop        = 8;
  sps_bypass      = 9;
  sps_bypassOff   = 10;

  // plugin transport
  spt_changed     = 1;  // play, cycle or record state has changed
  spt_play        = 2;  // Host sequencer is currently playing
  spt_cycle       = 4;  // Host sequencer is in cycle mode
  spt_record      = 8;  // Host sequencer is in record mode
  spt_awrite      = 64; // automation write mode active (record parameter changes)
  spt_aread       = 128; // automation read mode active (play parameter changes)
  //todo:
  spt_pause       = 256;

  //------------------------------
  // s_widget
  //------------------------------

  // widget alignment
  swa_none          = 0;
  swa_fillClient    = 1;
  swa_fillLeft      = 2;
  swa_fillRight     = 3;
  swa_fillTop       = 4;
  swa_fillBottom    = 5;
  swa_left_top      = 6;
  swa_left_bottom   = 7;
  swa_right_top     = 8;
  swa_right_bottom  = 9;
  swa_top_left      = 0;
  swa_top_right     = 11;
  swa_bottom_left   = 12;
  swa_bottom_right  = 13;
  swa_stacked_horiz = 14;
  swa_stacked_vert  = 15;

  // widget flags
  swf_none      = 0;
  swf_active    = 1;
  swf_visible   = 2;
  swf_align     = 4;
  swf_capture   = 8;
  swf_clip      = 16;
  swf_opaque    = 32;

  //------------------------------
  // s_widget_click
  //------------------------------

  // click mode
  scm_spring = 0;
  scm_toggle = 1;

  //------------------------------
  // s_widget_value
  //------------------------------

  // drag direction
  sdd_up    = 0;
  sdd_down  = 1;
  sdd_left  = 2;
  sdd_right = 3;

  //------------------------------
  // s_window
  //------------------------------

  // thread signal
  sts_timer = 666;

  // buttons
  smb_none    = 0;
  smb_left    = 1;
  smb_right   = 2;
  smb_middle  = 4;
  smb_shift   = 8;
  smb_ctrl    = 16;
  smb_alt     = 32;

  // mouse cursors
  smc_default         = 0;
  smc_Arrow           = 1;
  smc_ArrowUp         = 2;
  smc_ArrowDown       = 3;
  smc_ArrowLeft       = 4;
  smc_ArrowRight      = 5;
  smc_ArrowUpDown     = 6;
  smc_ArrowLeftRight  = 7;
  smc_ArrowDiagLeft   = 8;
  smc_ArrowDiagRight  = 9;
  smc_Move            = 10;
  smc_Wait            = 11;
  smc_ArrowWait       = 12;
  smc_Hand            = 13;
  smc_Finger          = 14;
  smc_Cross           = 15;
  smc_Pencil          = 16;
  smc_Plus            = 17;
  smc_Question        = 18;
  smc_Ibeam           = 19;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const
  S_CR        = LineEnding; // #13#10;
  S_HEX_TABLE = '0123456789abcdef';
  S_PI        = 3.1415926535;
  S_PI2       = (S_PI*2);
  S_DENORM    = 1.4012984e-45;
  S_INV127    = (1/127);
  S_EPSILON   = 1.19209290e-7;
  S_E         = 2.71828182845;



  // http://www.cplusplus.com/forum/articles/3483/
  // c++: false is zero and true is any non-zero.
  // This includes negative numbers.

  S_TRUE  = 1; // not S_FALSE;
  S_FALSE = 0;

  //----------

  // 'S_' (high word) + $0000..$ffff
  // usage: uniqueid := S_MAGIC + $0000;
  //
  // 0x40    0x48    0x50    0x58
  // |       |       |       |
  // @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`

  S_MAGIC     = $535F0000; // S_ (lib-dev)
  S_MAGIC_USR = $735F0000; // s_ (lib-usr)

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
end.

