{$include sphaera.inc}

//----------------------------------------------------------------------

{$ifdef SPH_VST} library {$else} program {$endif}

build_win32;

//----------------------------------------------------------------------

uses
  s_debug,
  s_math,

  //--------------------
  // applications
  //--------------------

  //app_gui1;
  //app_modular;

  //--------------------
  // plugins
  //--------------------

  //plug_test1;
  //plug_minimal;

  //fx_applefilter12db;
  //fx_clipper;
  //fx_cutter;
  //fx_dist3;
  //fx_dynamic;
  //fx_eventhorizon;
  //fx_filter;
  //fx_filter_rbj;
  //fx_fracdelay;
  //fx_gain;
  //fx_grains;
  //fx_hallreverb;
  //fx_moog;
  //fx_pitch;
  //fx_pitchshift;
  //fx_resonantlowpass;
  //fx_sonicenhancer;
  //fx_sonicmaximizer;
  //fx_tempodelay;
  //fx_tilteq;
  //fx_transientcontroller;
  //fx_unmatched;
  //fx_vumeter;
  //midi_fibonacci;
  //midi_syncdelay;
  //midi_transpose;
  syn_poly;

//----------------------------------------------------------------------

{$ifdef SPH_VST}
  {$include sphaera_vst.inc}
{$else}
  {$include sphaera_exe.inc}
{$endif}

//----------------------------------------------------------------------
end.

