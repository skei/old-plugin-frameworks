unit z_const;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const

  // (status) buttons

  zbu_none    = 0;
  zbu_left    = 1;
  zbu_right   = 2;
  zbu_middle  = 4;
  zbu_shift   = 8;
  zbu_ctrl    = 16;
  zbu_alt     = 32;

  // (mouse) cursors

  zcu_default        = 0;
  zcu_arrow          = 1;
  zcu_arrowUp        = 2;
  zcu_arrowDown      = 3;
  zcu_arrowLeft      = 4;
  zcu_arrowRight     = 5;
  zcu_arrowUpDown    = 6;
  zcu_arrowLeftRight = 7;
  zcu_arrowDiagLeft  = 8;
  zcu_arrowDiagRight = 9;
  zcu_move           = 10;
  zcu_wait           = 11;
  zcu_arrowWait      = 12;
  zcu_hand           = 13;
  zcu_finger         = 14;
  zcu_cross          = 15;
  zcu_pencil         = 16;
  zcu_plus           = 17;
  zcu_question       = 18;
  zcu_ibeam          = 19;

  // event types

  zet_none      = 0;
  zet_user      = 1;
  zet_parameter = 2;
  zet_widget    = 3;
  zet_midi      = 4; // ??

  // parameter flags

  //zpf_none      = 0; // 'conflict' with parameter flags
  zpf_automate  = 1;    // all parameters automatable...

  // plugin flags

  zpf_none        = 0;
  zpf_hasEditor   = 1;
  zpf_isSynth     = 2;
  zpf_perSample   = 4;
  //zpf_autoSync    = 8;
  zpf_sendMidi    = 16;
  zpf_receiveMidi = 32;

  // paint modes

  zpm_full      = 0;
  zpm_update    = 1;
  zpm_hover     = 2;
  zpm_activate  = 3;

  // plugin state

  zps_open        = 1;
  zps_close       = 2;
  zps_suspend     = 3;
  zps_resume      = 4;
  zps_sampleRate  = 5;
  zps_blockSize   = 6;

  // plugin transport

  zpt_changed     = 1;
  zpt_play        = 2;

  // thread signel

  zts_timer = 1;
  zts_kill  = 2;

  // widget alignment

  zwa_none          = 0;
  zwa_client        = 1;
  zwa_left          = 2;
  zwa_right         = 3;
  zwa_top           = 4;
  zwa_bottom        = 5;
  zwa_left_top      = 6;
  zwa_left_bottom   = 7;
  zwa_right_top     = 8;
  zwa_right_bottom  = 9;
  zwa_top_left      = 0;
  zwa_top_right     = 11;
  zwa_bottom_left   = 12;
  zwa_bottom_right  = 13;
  zwa_stacked_horiz = 14;
  zwa_stacked_vert  = 15;

  // widget flags

  zwf_active    = 1;
  zwf_visible   = 2;
  zwf_align     = 4;
  zwf_capture   = 8;
  zwf_clip      = 16;
  zwf_opaque    = 32;

//----------------------------------------------------------------------

  Z_CR        = LineEnding; // #13#10;
  Z_HEX_TABLE = '0123456789abcdef';
  Z_PI        = 3.1415926535;
  Z_PI2       = (Z_PI*2);

  // http://www.cplusplus.com/forum/articles/3483/
  // c++: false is zero and true is any non-zero.
  // This includes negative numbers.

  Z_TRUE      = 1; // not Z_FALSE;
  Z_FALSE     = 0;

  //----------

  // 'Z_' (high word) + $0000..$ffff
  // usage: uniqueid := Z_MAGIC + $0000;
  //
  // 0x40    0x48    0x50    0x58
  // |       |       |       |
  // @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`
  // idea: 'Z' for lib-makers, 'z' for lib-users

  Z_MAGIC = $5A5F0000;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
end.

