#ifndef s3_config_included
#define s3_config_included
//----------------------------------------------------------------------



//----------------------------------------------------------------------
// values
//----------------------------------------------------------------------

#define S3_ARRAY_SIZE_INIT              16
#define S3_ARRAY_SIZE_MULT              2
#define S3_MAX_STRING_SIZE              256
#define S3_MAX_PATH_SIZE                512
#define S3_PAINTER_CLIPSTACK_SIZE       256
#define S3_PAINTER_DEFAULT_PEN_WIDTH    0
#define S3_PLUGIN_MAX_MIDI_RECEIVE      256
#define S3_PLUGIN_MAX_MIDI_SEND         256
#define S3_PLUGIN_MAX_PARAMETERS        256
#define S3_PLUGIN_MAX_QUEUE_SIZE        (S3_PLUGIN_MAX_PARAMETERS*2) //512
#define S3_PLUGIN_PROCESS_TICKSIZE      16

//----------

//#define S3_DEBUG_SLEEP_AFTER_MEMDUMP_MS 5000  // ms
//#define S3_DMC_THRESHOLD                8     // 2
//#define S3_DMC_BIGTHRESH                8     // 2
//#define S3_GLOBAL_MAX_OBJECTS           256
//#define S3_GLOBAL_PITCHBEND_RANGE       48
//#define S3_LADSPA_MAX_PORTS             256
//#define S3_LADSPA_MAX_INPUTS            16
//#define S3_LADSPA_MAX_OUTPUTS           16
//#define S3_LADSPA_MAX_PARAMS            256
//#define S3_LINUX_IDLE_MS                40  // 25 hz
//#define S3_LINUX_SLEEP_MS               30  // 33 hz
//#define S3_LINUX_THREADPROC_TICKLENGTH  1
//#define S3_LINUX_THREADPROC_TICKIDLE    30
//#define S3_MODULE_MAX_INPUTS            16
//#define S3_MODULE_MAX_OUTPUTS           16
//#define S3_MODULE_MAX_PARAMS            16
//#define S3_MODULE_MAX_OVERSAMPLE        32
//#define S3_MODULE_TICKSIZE              16  // 32
//#define S3_PLUGIN_MAX_VALUES            256
//#define S3_PROCESS_MAX_EVENTS           1024
//#define S3_PROCESS_MAX_OVERSAMPLE       8
//#define S3_PROCESS_BUFFERSIZE           (S3_PROCESS_TICKSIZE * S3_PROCESS_MAX_OVERSAMPLE)
//#define S3_VOICE_MANAGER_MAX_EVENTS     1024
//#define S3_VOICE_MANAGER_MAX_VOICES     256
//#define S3_VOICE_PITCHBEND_RANGE        2
//#define S3_WIN32_IDLE_MS                40    // 25 hz


//----------------------------------------------------------------------
// switches
//----------------------------------------------------------------------

//#define S3_CAIRO
//#define S3_DEBUG
//#define S3_DEBUG_ASSERT
//#define S3_DEBUG_CALLSTACK
//#define S3_DEBUG_CONSOLE
//#define S3_DEBUG_CRASHHANDLER
//#define S3_DEBUG_GDI
//#define S3_DEBUG_LOGFILE
//#define S3_DEBUG_MEM
//#define S3_DEBUG_MEM_PRINT
//#define S3_DEBUG_PROCESS
//#define S3_DEBUG_SLEEP_AFTER_MEMDUMP
//#define S3_DEBUG_SOCKET
//#define S3_DEBUG_VST
//#define S3_DEBUG_WINDOW
//#define S3_DEBUG_XLIB
//#define S3_GDI
//#define S3_GDIPLUS
//#define S3_GUI
//#define S3_GUI_CAIRO
//#define S3_GUI_GDI
//#define S3_GUI_GDIPLUS
//#define S3_GUI_OPENGL
//#define S3_GUI_XLIB
//#define S3_GUI_XRENDER
//#define S3_LINUX_IDLE_THREAD
//#define S3_LINUX_SLEEP_THREAD
//#define S3_LINUX_WAIT_FOR_MAPNOTIFY
//#define S3_LINUX_XINITTHREADS
//#define S3_MAIN
//#define S3_MODULES_IN_VOICE_MANAGER
//#define S3_MOUSE_DOUBLECLICK
//#define S3_NO_AUTODELETE
//#define S3_NO_BITMAP_BRESENHAM
//#define S3_NO_BITMAP_PNG
//#define S3_NO_DMC_COMPRESS
//#define S3_NO_MODULE_TICKS
//#define S3_NO_SHRINK_BACKBUFFER
//#define S3_NO_STDGUI
//#define S3_NO_WINDOW_BACKBUFFER
//#define S3_OPENGL
//#define S3_PLUGIN_AUTOSYNC
//#define S3_PLUGIN_CHUNKS
//#define S3_PLUGIN_DOUBLE
//#define S3_PLUGIN_DSSI
//#define S3_PLUGIN_EDITOR
//#define S3_PLUGIN_EXE
//#define S3_PLUGIN_LADSPA
//#define S3_PLUGIN_LV2
//#define S3_PLUGIN_MPE
//#define S3_PLUGIN_PARAMS_DIRECT
//#define S3_PLUGIN_PARAMS_EDITOR_QUEUE
#define S3_PLUGIN_PARAMS_PROCESS_QUEUE
//#define S3_PLUGIN_PER_MIDI_EVENT
//#define S3_PLUGIN_PER_SAMPLE
//#define S3_PLUGIN_PRESET_BUFFER
//#define S3_PLUGIN_REAPER_EXT
//#define S3_PLUGIN_RECEIVE_MIDI
//#define S3_PLUGIN_SEND_MIDI
//#define S3_PLUGIN_SILENT_STOP
//#define S3_PLUGIN_SYNTH
//#define S3_PLUGIN_TICKS
//#define S3_PLUGIN_TOOL
//#define S3_PLUGIN_VST
//#define S3_REPLACE_STDLIB
//#define S3_TABLET
//#define S3_WIN32_IDLE_TIMER
//#define S3_WINDOW_BACKBUFFER_IMAGE
//#define S3_XLIB
//#define S3_XRENDER
//#define S3_XFT

//----------------------------------------------------------------------
// automatic
//----------------------------------------------------------------------

/*
#define S3_CPP11
#define S3_LINUX
#define S3_WIN32
#define S3_32BIT
#define S3_64BIT
#define S3_BIG_ENDIAN
#define S3_LITTLE_ENDIAN
*/

//----------------------------------------------------------------------
#endif



