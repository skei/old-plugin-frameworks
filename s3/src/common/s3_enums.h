#ifndef s3_enums_included
#define s3_enums_included
//----------------------------------------------------------------------

//------------------------------
// audio
//------------------------------

enum S3_EEnvelopeStage {
  s3_es_off       = 0,
  s3_es_attack    = 1,
  s3_es_decay     = 2,
  s3_es_sustain   = 3,
  s3_es_release   = 4,
  s3_es_finished  = 5
};

//enum S3_ELadspaError {
//  s3_le_none          = 0,
//  s3_le_libNotFound   = 1,
//  s3_le_mainNotFound  = 2,
//  s3_le_descrNotFound = 3
//};

enum S3_ELadspaFlag {
  s3_lf_none          = 0,
  s3_lf_realtime      = 1,
  s3_lf_inplaceBroken = 2,
  s3_lf_hardRTCapable = 4
};

enum S3_ELadspaPortFlags {
  s3_lp_none          = 0,
  s3_lp_audio         = 1,
  s3_lp_control       = 2,
  s3_lp_input         = 4,
  s3_lp_output        = 8,
  s3_lp_toggled       = 16,
  s3_lp_samplerate    = 32,
  s3_lp_logarithmic   = 64,
  s3_lp_integer       = 128
};

enum S3_EVoiceState {
  s3_vs_off       = 0,
  s3_vs_playing   = 1,
  s3_vs_released  = 2
};


//------------------------------
// common
//------------------------------

//------------------------------
// gfx
//------------------------------

//------------------------------
// gui
//------------------------------

enum S3_EBitmapMode {
  s3_bm_draw    = 0,
  s3_bm_blend   = 1,
  s3_bm_stretch = 2
};

enum S3_EFlexMode {
  s3_fm_draw    = 0,
  s3_fm_stretch = 1,
  s3_fm_tile    = 2
};

// https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx

enum S3_EKeyCode {
  s3_kc_backspace = 8,    // VK_BACK
  s3_kc_tab       = 9,    // VK_TAB
  s3_kc_enter     = 13,   // VK_RETURN
  s3_kc_shift     = 16,   // VK_SHIFT
  s3_kc_ctrl      = 17,   // VK_CONTROL
  s3_kc_alt       = 18,   // VK_MENU
  s3_kc_pause     = 19,   // VK_PAUSE
  s3_kc_capslock  = 20,   // VK_CAPITAL
  s3_kc_esc       = 27,   // VK_ESCAPE
  s3_kc_space     = 32,   // VK_SPACE
  s3_kc_pgup      = 33,   // VK_PRIOR
  s3_kc_pgdn      = 34,   // VK_NEXT
  s3_kc_end       = 35,   // VK_END
  s3_kc_home      = 36,   // VK_HOME
  s3_kc_left      = 37,   // VK_LEFT
  s3_kc_up        = 38,   // VK_UP
  s3_kc_right     = 39,   // VK_RIGHT
  s3_kc_down      = 40,   // VK_DOWN
  s3_kc_print     = 42,   // VK_PRINT
  s3_kc_snapshot  = 44,   // VK_SNAPSHOT
  s3_kc_insert    = 45,   // VK_INSERT
  s3_kc_delete    = 46    // VK_DELETE
  // 0x30..0x39 = 0..9
  // 0x41..0x5a = A..Z
};

enum S3_EModifierKey {
  s3_mk_none  = 0,
  s3_mk_shift = 1,
  s3_mk_ctrl  = 2,
  s3_mk_alt   = 4
};

enum S3_EMouseButton {
  s3_mb_none      = 0,
  s3_mb_left      = 1,
  s3_mb_middle    = 2,
  s3_mb_right     = 3,
  s3_mb_backward  = 4,
  s3_mb_forward   = 5,
  s3_mb_side      = 8,
  s3_mb_wheelUp   = 9,
  s3_mb_wheelDown = 10
};

enum S3_EMouseCursor {
  s3_mc_show            = -2,
  s3_mc_hide            = -1,
  s3_mc_default         = 0,
  s3_mc_arrow           = 1,
  s3_mc_arrowUp         = 2,
  s3_mc_arrowDown       = 3,
  s3_mc_arrowLeft       = 4,
  s3_mc_arrowRight      = 5,
  s3_mc_arrowUpDown     = 6,
  s3_mc_arrowLeftRight  = 7,
  s3_mc_arrowDiagLeft   = 8,
  s3_mc_arrowDiagRight  = 9,
  s3_mc_move            = 10,
  s3_mc_wait            = 11,
  s3_mc_arrowWait       = 12,
  s3_mc_hand            = 13,
  s3_mc_finger          = 14,
  s3_mc_cross           = 15,
  s3_mc_pencil          = 16,
  s3_mc_plus            = 17,
  s3_mc_question        = 18,
  s3_mc_ibeam           = 19
};

enum S3_ERasterOp {
  s3_ro_black                     = 0,
  s3_ro_brush                     = 1,
  s3_ro_brush_or_invsrcor_or_dst  = 2,
  s3_ro_brush_xor_dst             = 3,
  s3_ro_dst                       = 4,
  s3_ro_dst_and_invsrc            = 5,
  s3_ro_invdst                    = 6,
  s3_ro_invdst_and_src            = 7,
  s3_ro_invsrc                    = 8,
  s3_ro_invsrc_or_dst             = 9,
  s3_ro_src                       = 10,
  s3_ro_src_and_dst               = 11,
  s3_ro_src_and_brush             = 12,
  s3_ro_src_or_dst                = 13,
  s3_ro_src_or_dst_inv            = 14,
  s3_ro_src_xor_dst               = 15,
  s3_ro_white                     = 16
};

enum S3_EThreadSignal {
  s3_ts_timer = 666,
  s3_ts_idle  = 667,
  s3_ts_kill  = 668
};

enum S3_EFontSlant {
  s3_fs_normal    = 0,
  s3_fs_italic    = 1,
  s3_fs_oblique   = 2
};

enum S3_EFontWeight {
  s3_fw_normal    = 0,
  s3_fw_bold      = 1
};

enum S3_ELineCap {
  s3_lc_butt    = 0,
  s3_lc_round   = 1,
  s3_lc_square  = 2
};

enum S3_ELineJoin {
  s3_lj_miter   = 0,
  s3_lj_round   = 1,
  s3_lj_bevele  = 2
};

//------------------------------
// parameters
//------------------------------

enum S3_EParameterFlag {
  s3_pf_none      = 0,
  s3_pf_automate  = 1,
  s3_pf_smoothing = 2
};

enum S3_EParameterType {
  s3_pt_none        = 0,
  s3_pt_parameter   = 1,
  s3_pt_paramFloat  = 2,
  s3_pt_paramInt    = 3,
  s3_pt_paramText   = 4
};

enum s3_EKnobMode {
  s3_km_circular          = 0,
  s3_km_circularRelative  = 1,
  s3_km_linear            = 2
};

//------------------------------
// plugin
//------------------------------

enum S3_EHostCando {
  s3_hc_none                            = 0,
  s3_hc_sendVstEvents                   = 1,
  s3_hc_sendVstMidiEvent                = 2,
  s3_hc_sendVstTimeInfo                 = 4,
  s3_hc_receiveVstEvents                = 8,
  s3_hc_receiveVstMidiEvent             = 16,
  s3_hc_reportConnectionChanges         = 32,
  s3_hc_acceptIOChanges                 = 64,
  s3_hc_sizeWindow                      = 128,
  s3_hc_offline                         = 256,
  s3_hc_openFileSelector                = 512,
  s3_hc_closeFileSelector               = 1024,
  s3_hc_startStopProcess                = 2048,
  s3_hc_shellCategory                   = 4096,
  s3_hc_sendVstMidiEventFlagIsRealtime  = 8192
};

enum S3_EHostId {
  s3_hi_unknown   = 0,
  s3_hi_exe       = 1,
  s3_hi_reaper    = 2,
  s3_hi_energyxt  = 3,
  s3_hi_live      = 4,
  s3_hi_fruity    = 5,
  s3_hi_qtractor  = 6,
  s3_hi_carla     = 7,
  s3_hi_bitwig    = 8,
  s3_hi_jost      = 9
};

enum S3_EMidiEvent {
  s3_me_none          = 0,
  s3_me_noteOn        = 1,
  s3_me_noteOff       = 2,
  s3_me_controlChange = 3,
  s3_me_pitchBend     = 4
};

/*
enum S3_EPluginFlags {
  //s3_pf_none        = 0, // conflict: S3_EParameterFlags
  s3_pf_synth           = 1,
  s3_pf_editor          = 2,
  s3_pf_mpe             = 4,
  s3_pf_chunks          = 8,
  s3_pf_silentStop      = 16,
  s3_pf_double          = 32,
  s3_pf_sendMidi        = 64,
  s3_pf_receiveMidi     = 128,
  s3_pf_tool            = 256,  // not used..
  s3_pf_ticks           = 512,  // not used..
  s3_pf_per_sample      = 1024,
  s3_pf_autosync        = 2048,
  s3_pf_per_midi_event  = 4096
};
*/

//enum S3_EPluginMode {
//  s3_pm_none  = 0,
//  s3_pm_mpe   = 1
//};

enum S3_EPluginState {
  s3_ps_open        = 1,
  s3_ps_close       = 2,
  s3_ps_sampleRate  = 3,
  s3_ps_blockSize   = 4,
  s3_ps_suspend     = 5,  // standby
  s3_ps_resume      = 6,  // standby_off
  s3_ps_initialize  = 7,  // (first resume)
  s3_ps_start       = 8,
  s3_ps_stop        = 9,
  s3_ps_bypass      = 10,
  s3_ps_bypassOff   = 11
};

enum S3_EPluginTransport {
  s3_pt_changed     = 1,     // play, cycle or record state has changed
  s3_pt_play        = 2,     // Host sequencer is currently playing
  s3_pt_cycle       = 4,     // Host sequencer is in cycle mode
  s3_pt_record      = 8,     // Host sequencer is in record mode
  s3_pt_awrite      = 64,    // automation write mode active (record parameter changes)
  s3_pt_aread       = 128,   // automation read mode active (play parameter changes)
  s3_pt_pause       = 256
};

//------------------------------
// widgets
//------------------------------

enum S3_EPaintMode {
  s3_pm_normal    = 0,
  s3_pm_hover     = 1,
  s3_pm_interact  = 2,
  s3_pm_enter     = 3,
  s3_pm_leave     = 4
};

enum S3_ESizerMode {
  s3_sm_none    = 0,
  s3_sm_window  = 1,
  s3_sm_left    = 2,
  s3_sm_right   = 3,
  s3_sm_top     = 4,
  s3_sm_bottom  = 5
};

enum S3_ETextAlign {
  s3_ta_center  = 0,
  s3_ta_left    = 1,
  s3_ta_right   = 2,
  s3_ta_top     = 4,
  s3_ta_bottom  = 8
};

enum S3_EWidgetAlign {
  s3_wa_none          = 0,
  s3_wa_client        = 1,
  s3_wa_overlay       = s3_wa_client,
  s3_wa_fillClient    = 2,  /// fill the entire available client area
  s3_wa_fillLeft      = 3,  /// fille left side (vertical)
  s3_wa_fillRight     = 4,  /// fill right side (vertiucal)
  s3_wa_fillTop       = 5,  /// fill top (horizontal)
  s3_wa_fillBottom    = 6,  /// fill bottom (horizontal)
  s3_wa_left          = 7,
  s3_wa_leftTop       = 8,
  s3_wa_leftBottom    = 9,
  s3_wa_right         = 10,
  s3_wa_rightTop      = 11,
  s3_wa_rightBottom   = 12,
  s3_wa_top           = 13,
  s3_wa_topLeft       = 14,
  s3_wa_topRight      = 15,
  s3_wa_bottom        = 16,
  s3_wa_bottomLeft    = 17,
  s3_wa_bottomRight   = 18,
  s3_wa_stackedHoriz  = 19,
  s3_wa_stackedVert   = 20
};

enum S3_EWidgetFlag {
  s3_wf_none          = 0,
  s3_wf_active        = 1,
  s3_wf_visible       = 2,
  s3_wf_autoalign     = 4,
  s3_wf_capture       = 8,
  s3_wf_opaque        = 16,
  s3_wf_clip          = 32,
  s3_wf_vertical      = 64
  //s3_wf_moveable      =
  //s3_wf_sizeable      =
  //s3_wf_redrawHover   =
};

//----------------------------------------------------------------------
#endif
