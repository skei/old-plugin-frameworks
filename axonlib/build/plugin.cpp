//#define AX_NO_MALLOC
//#define AX_DEBUG_AUTO_STD

//#define AX_DEBUG_MEM
//#define AX_DEBUG_NEW
//#define AX_DEBUG_PNG
//#define AX_DEBUG_CONSOLE
//#define AX_DEBUG_LOG "plugin.log"

//----------------------------------------------------------------------

/*

TODO:
- fill out the callback functions in ladspa
  (copy/paste from vst + tweak)
- vst editor resizing

*/
//----------------------------------------------------------------------
// test / demo
//----------------------------------------------------------------------

//#include "../source/demo/axDemo.cpp"

//#include "../source/test/gain.cpp"
//#include "../source/test/test_gain_nogui.cpp"
//#include "../source/test/test_gain_gui.cpp"
//#include "../source/test/test_gain_gui_skin.cpp"

//#include "../ax/base/test.cpp"

//#include "../test/test_dmc.cpp"
//#include "../test/test_math.cpp"
//#include "../test/test_memory.cpp"
//#include "../test/test_render.cpp"
//#include "../test/test_script.cpp"
//#include "../test/test_skin.cpp"
//#include "../test/test_test.cpp"

//----------------------------------------------------------------------
// ccernn
//----------------------------------------------------------------------

// no gui

//#include "../source/ccernn/fx_blur.cpp"
//#include "../source/ccernn/fx_distortion.cpp"
//#include "../source/ccernn/fx_freeze.cpp"
#include "../source/ccernn/fx_svf.cpp"
//#include "../source/ccernn/fx_wgtavg.cpp"
//#include "../source/ccernn/midi_transpose.cpp"
//#include "../source/ccernn/syn_poly.cpp"

// gui

//#include "../source/ccernn/fx_grains.cpp"
//#include "../source/ccernn/fx_tempodelay.cpp"

//----------------------------------------------------------------------
// liteon / neolit123
//----------------------------------------------------------------------

//#include "../source/liteon/liteon_biquad.cpp"
//#include "../source/liteon/liteon_tilteq.cpp"

//----------------------------------------------------------------------
// ports
//----------------------------------------------------------------------

//#include "../source/ports/sstillwell_eventhorizon.cpp"
//#include "../source/ports/mverb.cpp"

//----------------------------------------------------------------------
