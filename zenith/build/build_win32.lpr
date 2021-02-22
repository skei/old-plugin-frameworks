
{$include zenith.inc}
{$ifdef Z_LIB} library {$else} program {$endif} build_win32;
uses
  heaptrc,
  zenith,

  //minimal;
  //test1;

  //fx_clipper;
  //fx_eventhorizon;
  //fx_filter_rbj;
  //fx_hallreverb;
  //fx_pitchshift;
  //fx_sonicenhancer;
  //fx_sonicmaximizer;
  //fx_tempodelay;
  //fx_tilteq;
  //midi_fibonacci;
  midi_syncdelay;
  //midi_transpose;

{$include zenith_main.inc}

