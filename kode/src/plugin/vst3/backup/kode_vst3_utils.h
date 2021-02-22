#ifndef kode_vst3_utils_included
#define kode_vst3_utils_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/vst3/kode_vst3.h"

//----------------------------------------------------------------------

#define char_to_utf16(C,U) UString128(C).copyTo(U,128)
#define utf16_to_char(U,C) UString128(U).toAscii(C,128)

//----------

char* kode_vst3_tuid_to_ascii(const TUID _iid, char* ABuffer) {
  //char buf[64];
  const char hextable[] = "0123456789abcdef";
  char* ptr = ABuffer;
  for (uint32_t i=0; i<4; i++) {
    for (uint32_t j=0; j<4; j++) {
      uint32_t k = i * 4 + j;
      uint8_t byte = _iid[k];
      *ptr++ = hextable[byte >> 4];
      *ptr++ = hextable[byte % 15];
    }
    if (i<3) *ptr++ = '-';
  }
  *ptr = 0;
  return ABuffer;
}

//----------------------------------------------------------------------
#endif
