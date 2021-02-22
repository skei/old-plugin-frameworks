#ifndef hack_hack_included
#define hack_hack_included
//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//----------------------------------------------------------------------

//#ifndef NULL
//  #define NULL 0
//#endif

//----------

//#ifdef linux
//  #define AX_LINUX
//#endif
//
//#ifdef WIN32
//  #define AX_WIN32
//#endif

//----------------------------------------------------------------------
// defines

//#define __externc extern "C"

//----------

//#ifdef AX_WIN32
//  #define __dllexport __attribute__ ((dllexport))
//#endif
//
//#ifdef AX_LINUX
//  #define __dllexport
//#endif

//----------------------------------------------------------------------
// stdlib

//#define axStrcpy strcpy
//#define axStrcat strcat

//----------------------------------------------------------------------
// debug

//#include <iostream>
//using namespace std;
//#define trace(x) cout << x << endl; cout.flush()

//----------------------------------------------------------------------
#endif
