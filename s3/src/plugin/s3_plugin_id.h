#ifndef s3_plugin_id_included
#define s3_plugin_id_included
//----------------------------------------------------------------------

/*

// flags

0x 80.**  gpl
0x 40.**  port
0x 20.**  wrap
0x 10.**
0x 08.**  gui

// type

*0.** - *7.**
  0 = fx
  1 = midi
  2 = syn
  3 = wrap
  4 = other

// number

+ **.00 - **.ff = num/id

*/

//----------------------------------------------------------------------
// fx
//----------------------------------------------------------------------

#define s3_fx_curve_shaper_id     0x0001
#define s3_fx_frac_delay_id       0x0002
#define s3_fx_freeze_id           0x0003
#define s3_fx_grains_id           0x0004
#define s3_fx_stutter_id          0x0005
#define s3_fx_blur_id             0x0006
#define s3_fx_dist3_id            0x0007
#define s3_fx_tempo_delay_id      0x0008

// ports

#define s3_fx_event_horizon_id    0x4001
#define s3_fx_hall_reverb_id      0x4002
#define s3_fx_pitch_shifter_id    0x4003
#define s3_fx_saturation_id       0x4004
#define s3_fx_simpverb_id         0x4005
#define s3_fx_sonic_maximizer_id  0x4006

//----------------------------------------------------------------------
// midi
//----------------------------------------------------------------------

#define s3_midi_fibonacci_id      0x0101

//----------------------------------------------------------------------
// wrap
//----------------------------------------------------------------------

#define s3_wrap_dssi_id           0x2201
#define s3_wrap_ladspa_id         0x2002

//----------------------------------------------------------------------
// gpl
//----------------------------------------------------------------------

#define s3_fx_cabinet_id          0xc001

//----------------------------------------------------------------------
// bugs / unfinished
//----------------------------------------------------------------------

#define s3_jack_id                0x0401

//----------------------------------------------------------------------
#endif
