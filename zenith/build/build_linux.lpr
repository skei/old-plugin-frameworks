
{$include zenith.inc}
{$ifdef Z_LIB} library {$else} program {$endif} build_linux;
uses
  //{$ifdef Z_DEBUG}
  heaptrc,
  //{$endif}
  zenith,

  //minimal;
  //template_basic;
  //test1;
  gui1;

  //fx_clipper;
  //fx_eventhorizon;
  //fx_filter_rbj;
  //fx_hallreverb;
  //fx_pitchshift;
  //fx_sonicenhancer;
  //fx_sonicmaximizer;
  //fx_tempodelay;
  //fx_tilteq;
  //fx_transientcontroller;

  //midi_fibonacci;
  //midi_transpose;

{$include zenith_main.inc}

