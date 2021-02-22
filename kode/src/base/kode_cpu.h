#ifndef kode_cpu_included
#define kode_cpu_included
//----------------------------------------------------------------------

#include "base/kode.h"

#ifdef KODE_LINUX
  #include "base/linux/kode_linux_cpu.h"
#endif

//----------------------------------------------------------------------

bool KODE_IsLittleEndian(void) {
  uint16_t pattern = 0x0001;
  return *(char*)&pattern == 0x01;
}

//----------------------------------------------------------------------
#endif
