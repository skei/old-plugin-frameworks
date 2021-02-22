#define AX_DEBUG_MEM
#define AX_DEBUG_NEW
//#define AX_DEBUG_CONSOLE
//#define AX_DEBUG_LOG "test_test.log"

//----------------------------------------------------------------------

#include "base/axBase.h"
AX_MAIN(AX_DESCRIPTOR,AX_INSTANCE)

//[test_test.cpp:9] malloc(new), 0xbf92aa54, 20, 20       +baseimpl
//[axBase.h:117] malloc(new), 0xbf92aa34, 520, 544        +platform
//[axBase.h:118] malloc(new), 0xbf92aa34, 16, 564         +interface
//[axBase.h:119] malloc(new), 0xbf92aa34, 4, 576          +descriptor
//[axBase.h:120] malloc(new), 0xbf92aa34, 24, 604         +format
//[axBase.h:141] malloc(new), 0xbf92a9f4, 112, 720        +instance
//[axArray.h:50] malloc, 0xbf92a9c4, 64, 788                +parameters
//[axArray.h:50] malloc, 0xbf92a9c4, 64, 856                +programs
//[axFormatExe.h:425] free(delete), 0xbf92a9dc, 116, 740
//[axFormatExe.h:425] free(delete), 0xbf92a9dc, 28, 712
//[axBase.h:128] free(delete), 0xbf92a9fc, 12, 700        -format
//[axBase.h:129] free(delete), 0xbf92a9dc, 20, 680        -descriptor
//[axBase.h:130] free(delete), 0xbf92a9dc, 524, 156       -interface
//[axBase.h:130] free(delete), 0xbf92a9fc, 20, 136        -platform

//missing:
//  -baseimpl [gGlobalScope, axBase.destructor]
//  -instance [axFormatExe.destructor


//----------------------------------------------------------------------

//#include "par/axParameter.h"
//#include "par/axProgram.h"
//
//int main(void)
//{
//  axParameters mParameters;
//  axPrograms   mPrograms;
//  return 0;
//}

//----------------------------------------------------------------------
