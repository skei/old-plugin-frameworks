#ifndef kode_config_included
#define kode_config_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------

//#define KODE_CONVERT_USE_STDLIB
//#define KODE_DMC_NO_COMPRESS
//#define KODE_NO_AUTODELETE
//#define KODE_NO_GPL
//#define KODE_NO_STDLIB

  
//----------------------------------------------------------------------
// debug
//----------------------------------------------------------------------

//#define KODE_DEBUG
#define KODE_DEBUG_ASSERT
//#define KODE_DEBUG_CALLSTACK
//#define KODE_DEBUG_CRASH_HANDLER
//#define KODE_DEBUG_MEMORY
//#define KODE_DEBUG_MEMORY_PRINT
#define KODE_DEBUG_TRACE
//#define KODE_DEBUG_TRACE_SOCKET
//#define KODE_DEBUG_TRACE_THREAD

//----------

#ifdef KODE_DEBUG_CRASH_HANDLER
  #define KODE_DEBUG_CALLSTACK
#endif

#ifndef KODE_DEBUG
  #undef KODE_DEBUG_ASSERT
  #undef KODE_DEBUG_CALLSTACK
  #undef KODE_DEBUG_CRASH_HANDLER
  #undef KODE_DEBUG_MEMORY
  #undef KODE_DEBUG_MEMORY_PRINT
  #undef KODE_DEBUG_TRACE
  #undef KODE_DEBUG_TRACE_SOCKET
  #undef KODE_DEBUG_TRACE_THREAD
  #undef KODE_DEBUG_TRACE_TIME
#endif

#ifndef KODE_DEBUG_MEMORY
  #undef KODE_DEBUG_MEMORY_PRINT
#endif

#ifndef KODE_DEBUG_TRACE
  #undef KODE_DEBUG_TRACE_SOCKET
  #undef KODE_DEBUG_TRACE_THREAD
  #undef KODE_DEBUG_TRACE_TIME
#endif

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

//#define KODE_PLUGIN_SIMPLE_STATE
//#define KODE_PLUGIN_LV2_DUMP_TTL
//#define KODE_PLUGIN_VST2_FST
#define KODE_PLUGIN_VST2_VESTIGE

// todo, not used?
//#define KODE_PLUGIN_MPE
//#define KODE_PLUGIN_PER_SAMPLE
//#define KODE_PLUGIN_PRESET_BUFFER
//#define KODE_PLUGIN_REAPER_EXT
//#define KODE_PLUGIN_TICKS

//----------

#undef KODE_PLUGIN_FORMAT_DEFINED

  #ifdef KODE_PLUGIN_DSSI
    #define KODE_PLUGIN
    #define KODE_PLUGIN_FORMAT_DEFINED
  #endif

  #ifdef KODE_PLUGIN_EXE
    #define KODE_PLUGIN
    #define KODE_PLUGIN_FORMAT_DEFINED
  #endif

  #ifdef KODE_PLUGIN_LADSPA
    #define KODE_PLUGIN
    #define KODE_PLUGIN_FORMAT_DEFINED
  #endif

  #ifdef KODE_PLUGIN_LV2
    #define KODE_PLUGIN
    #define KODE_PLUGIN_FORMAT_DEFINED
  #endif

  #ifdef KODE_PLUGIN_VST2
    #define KODE_PLUGIN
    #define KODE_PLUGIN_FORMAT_DEFINED
  #endif

  #ifdef KODE_PLUGIN_VST3
    #define KODE_PLUGIN
    #define KODE_PLUGIN_FORMAT_DEFINED
  #endif

#ifndef KODE_PLUGIN_FORMAT_DEFINED
  #define KODE_PLUGIN
  //#define KODE_PLUGIN_DSSI
  #define KODE_PLUGIN_EXE
  //#define KODE_PLUGIN_LADSPA
  //#define KODE_PLUGIN_LV2
  #define KODE_PLUGIN_VST2
  #define KODE_PLUGIN_VST3
#endif

//#ifdef KODE_PLUGIN_IS_SYNTH
//  #define KODE_PLUGIN_RECEIVE_MIDI
//#endif

//----------------------------------------------------------------------
// gui
//----------------------------------------------------------------------

//#define KODE_CAIRO
//#define KODE_CTX
//#define KODE_OPENGL
//#define KODE_SOFTWARE
//#define KODE_XCB

//#define KODE_GUI_CAIRO
//#define KODE_GUI_CTX
//#define KODE_GUI_OPENGL
//#define KODE_GUI_SOFTWARE
//#define KODE_GUI_XCB

//#define KODE_BITMAP_NO_DECODE
//#define KODE_BITMAP_NO_FILE
//#define KODE_IMAGE_NO_DECODE
//#define KODE_IMAGE_NO_FILE
//#define KODE_WINDOW_NOT_BUFFERED
//#define KODE_XCB_WAIT_FOR_MAPNOTIFY

//----------

#undef KODE_GUI_TYPE_DEFINED

  #ifdef KODE_GUI_XCB
    #define KODE_GUI
    #define KODE_XCB
    #define KODE_GUI_XCB
    #define KODE_GUI_TYPE_DEFINED
  #endif

  #ifdef KODE_GUI_CAIRO
    #define KODE_GUI
    #define KODE_XCB
    #define KODE_CAIRO
    #define KODE_GUI_CAIRO
    #define KODE_GUI_TYPE_DEFINED
  #endif

#ifdef KODE_CAIRO
  #define KODE_XCB
#endif

#ifndef KODE_GUI_TYPE_DEFINED
//  #warning no gui type defined! (KODE_GUI_XCB/CAIRO)
//  #ifndef KODE_NO_GUI
//    #define KODE_GUI
//    #define KODE_XCB
//    #define KODE_GUI_XCB
//    //#define KODE_CAIRO
//    //#define KODE_GUI_CAIRO
//  #else
    #define KODE_NO_GUI
    #undef KODE_GUI
//  #endif
#endif

//----------------------------------------------------------------------
#endif
