#ifndef s3_defines_included
#define s3_defines_included
//----------------------------------------------------------------------

//----------------------------------------
// os
// - S3_LINUX
// - S3_WIN32
//----------------------------------------

//#ifdef linux
//#ifdef __linux__ // linux kernel
#ifdef __gnu_linux__
  #define S3_LINUX
#endif

//#ifdef WIN32
#ifdef _WIN32
  #define  S3_WIN32
  //#define  WINVER 0x0501
  //#undef  _WIN32_WINNT
  //#define _WIN32_WINNT 0x0501
  // exclude APIs such as Cryptography, DDE, RPC, Shell, and Windows Sockets..
  //#define  WIN32_LEAN_AND_MEAN
#endif

//----------------------------------------
// architecture (32/64 bits)
// - S3_32BIT
// - S3_64BIT
//----------------------------------------

#if defined  (__i386__) || defined (_X86_) || defined (__X86__)
  #define S3_32BIT
#endif

#if defined (__x86_64) || (__LP64__)
  #define S3_64BIT
#endif

//----------------------------------------
// endian-ness
// - S3_BIG_ENDIAN
// - S3_LITTLE_ENDIAN
//----------------------------------------

//#if defined (__ORDER_BIG_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  #define S3_BIG_ENDIAN
#endif

//#if defined (__ORDER_LITTLE_ENDIAN__)
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  #define S3_LITTLE_ENDIAN
#endif

//----------------------------------------
// c++11
//----------------------------------------

//__cplusplus = 201103L

#if (__cplusplus >= 201103L)
  #define S3_CPP11
#endif

//----------------------------------------
// gui
// defines S3_GUI
//----------------------------------------

#undef S3_GUI

//----------

#ifdef S3_GUI_CAIRO
  #define S3_GUI
  #define S3_CAIRO
  #ifdef S3_LINUX
    #define S3_XLIB
  #endif
  #ifdef S3_WIN32
    #define S3_GDI
  #endif
#endif

#ifdef S3_GUI_GDI
  #define S3_GUI
  #define S3_GDI
#endif

#ifdef S3_GUI_GDI
  #define S3_GUI
  #define S3_GDI
  #define S3_GDIPLUS
#endif

#ifdef S3_GUI_OPENGL
  #define S3_GUI
  #define S3_OPENGL
#endif

#ifdef S3_GUI_XLIB
  #define S3_GUI
  #define S3_XLIB
#endif

#ifdef S3_GUI_XRENDER
  #define S3_GUI
  #define S3_XLIB
  #define S3_XRENDER
  #define S3_XFT
#endif

//----------

#ifdef S3_XRENDER
  #define S3_XLIB
#endif

#ifdef S3_XFT
  #define S3_XLIB
  #define S3_XRENDER
#endif

//----------

//#ifndef S3_GUI
//  #ifndef S3_NO_STDGUI
//  #endif
//#endif

//----------------------------------------
// plugin format
//----------------------------------------

//#undef S3_PLUGIN

#ifdef S3_PLUGIN_DSSI
  #define S3_LIB
#endif

#ifdef S3_PLUGIN_EXE
  #undef S3_LIB
#endif

#ifdef S3_PLUGIN_LADSPA
  #define S3_LIB
#endif

#ifdef S3_PLUGIN_LV2
  #define S3_LIB
#endif

#ifdef S3_PLUGIN_VST
  #define S3_LIB
#endif

//----------

#ifdef S3_PLUGIN_SYNTH
  #define S3_PLUGIN_RECEIVE_MIDI
#endif

//----------------------------------------------------------------------
#endif
