#ifndef kode_plugin_included
#define kode_plugin_included
//----------------------------------------------------------------------

#include "base/kode.h"

//----------------------------------------------------------------------

#define KODE_NO_MAIN(DESC,INST)

//----------------------------------------------------------------------

#ifdef KODE_PLUGIN_DSSI
  #include "plugin/dssi/kode_dssi_plugin.h"
  #define _KODE_DSSI_MAIN KODE_DSSI_MAIN
#else 
  #define _KODE_DSSI_MAIN KODE_NO_MAIN
#endif

//----------

#ifdef KODE_PLUGIN_EXE
  #include "plugin/exe/kode_exe_plugin.h"
  #define _KODE_EXE_MAIN KODE_EXE_MAIN
#else 
  #define _KODE_EXE_MAIN KODE_NO_MAIN
#endif

//----------

#ifdef KODE_PLUGIN_LADSPA
  #include "plugin/ladspa/kode_ladspa_plugin.h"
  #define _KODE_LADSPA_MAIN KODE_LADSPA_MAIN
#else 
  #define _KODE_LADSPA_MAIN KODE_NO_MAIN
#endif

//----------

#ifdef KODE_PLUGIN_LV2
  #include "plugin/lv2/kode_lv2_plugin.h"
  #define _KODE_LV2_MAIN KODE_LV2_MAIN
#else 
  #define _KODE_LV2_MAIN KODE_NO_MAIN
#endif

//----------

#ifdef KODE_PLUGIN_VST2
  #include "plugin/vst2/kode_vst2_plugin.h"
  #define _KODE_VST2_MAIN KODE_VST2_MAIN
#else 
  #define _KODE_VST2_MAIN KODE_NO_MAIN
#endif

//----------

#ifdef KODE_PLUGIN_VST3
  #include "plugin/vst3/kode_vst3_plugin.h"
  #define _KODE_VST3_MAIN KODE_VST3_MAIN
#else 
  #define _KODE_VST3_MAIN KODE_NO_MAIN
#endif

//----------------------------------------------------------------------

#define KODE_MAIN(DESC,INST)      \
    _KODE_DSSI_MAIN(DESC,INST)    \
    _KODE_EXE_MAIN(DESC,INST)     \
    _KODE_LADSPA_MAIN(DESC,INST)  \
    _KODE_LV2_MAIN(DESC,INST)     \
    _KODE_VST2_MAIN(DESC,INST)    \
    _KODE_VST3_MAIN(DESC,INST)    \
  
//----------------------------------------------------------------------
#endif
