#ifndef s3_main_included
#define s3_main_included
//----------------------------------------------------------------------

/*
  defines a S3:MAIN macro that sets up (and exports) the main
  function/entrypoint.

    exe: main()
    vst: VstPluginMain()

  this function initializes debug stuff, creates a plugin instance,
  and calls the newly created plugin's main() method..
*/

//----------------------------------------------------------------------
// exe
//----------------------------------------------------------------------

#ifdef S3_PLUGIN_EXE

  #define S3_MAIN(S3_PLUGIN)                \
    int main(void) {                        \
      S3_Debug_Initialize();                \
      S3_PLUGIN* plugin = new S3_PLUGIN();  \
      plugin->on_create();                  \
      plugin->main(S3_NULL);                \
      plugin->on_destroy();                 \
      delete plugin;                        \
      return 0;                             \
    }

#endif // S3_PLUGIN_EXE

//----------------------------------------------------------------------
// vst
//----------------------------------------------------------------------

/*
  TODO:
  - be sure we handle multiple instances properly..
    main() is called for each instance
    and also for the initial enumeration

  - simplify the ASM_MAIN_SYMBOL setup below..
*/

//----------

#ifdef S3_PLUGIN_VST

  // move these to s3_plugin_vst.h ??
  #define __cdecl
  #include "extern/vst/aeffect.h"
  #include "extern/vst/aeffectx.h"

  //----------

  // https://gcc.gnu.org/onlinedocs/gcc/Asm-Labels.html
  // "6.44.4 Controlling Names Used in Assembler Code"

  #ifdef S3_LINUX
    //#define S3_ASM_MAIN_SYMBOL asm ("main");
    #define S3_ASM_MAIN_SYMBOL asm ("VSTPluginMain");
  #endif // S3_LINUX

  #ifdef S3_WIN32
    //#define S3_ASM_MAIN_SYMBOL asm ("_main");
    #ifdef S3_32BIT
      #define S3_ASM_MAIN_SYMBOL asm ("_VSTPluginMain");
    #endif
    #ifdef S3_64BIT
      #define S3_ASM_MAIN_SYMBOL asm ("VSTPluginMain");
    #endif
  #endif // S3_WIN32

  AEffect* main_plugin(audioMasterCallback audioMaster) S3_ASM_MAIN_SYMBOL
  #define  main main_plugin

  //----------

  /*
    the 'plugin' object is deleted in: s3_plugin_vst.h,
    vst_dispatcher_callback(), if (opcode==effClose)
  */

  #define S3_MAIN(S3_PLUGIN)                                    \
    _S3_DLLEXPORT                                               \
    AEffect* main(audioMasterCallback audioMaster) {            \
      if (!audioMaster(0,audioMasterVersion,0,0,0,0)) return 0; \
      S3_Debug_Initialize();                                    \
      S3_PLUGIN* plugin = new S3_PLUGIN();                      \
      plugin->on_create();                                      \
      return (AEffect*)plugin->main((void*)audioMaster);        \
    }

#endif // S3_PLUGIN_VST

//----------------------------------------------------------------------
#endif
