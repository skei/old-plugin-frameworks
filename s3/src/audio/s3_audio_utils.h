#ifndef s3_audio_utils_included
#define s3_audio_utils_included
//----------------------------------------------------------------------

#include "common/s3_memory_stdlib.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void S3_CopyMonoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength);
void S3_ClearMonoBuffer(S3_Sample** ADst, uint32 ALength);
void S3_AddMonoBuffer(S3_Sample** ADst, S3_Sample** AAdd, uint32 ALength);
void S3_ScaleMonoBuffer(S3_Sample** ADst, float AScale, uint32 ALength);

void S3_CopyMonoBuffer(S3_Sample* ADst, S3_Sample* ASrc, uint32 ALength);
void S3_ClearMonoBuffer(S3_Sample* ADst, uint32 ALength);
void S3_AddMonoBuffer(S3_Sample* ADst, S3_Sample* AAdd, uint32 ALength);
void S3_ScaleMonoBuffer(S3_Sample* ADst, float AScale, uint32 ALength);

// S3_PLUGIN_PROCESS_TICKSIZE

void S3_CopyMonoBuffer(S3_Sample** ADst, S3_Sample** ASrc);
void S3_ClearMonoBuffer(S3_Sample** ADst);
void S3_AddMonoBuffer(S3_Sample** ADst, S3_Sample** AAdd);
void S3_ScaleMonoBuffer(S3_Sample** ADst, float AScale);

void S3_CopyMonoBuffer(S3_Sample* ADst, S3_Sample* ASrc);
void S3_ClearMonoBuffer(S3_Sample* ADst);
void S3_AddMonoBuffer(S3_Sample* ADst, S3_Sample* AAdd);
void S3_ScaleMonoBuffer(S3_Sample* ADst, float AScale);

//------------------------------
// stereo
//------------------------------

void S3_CopyStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength);
void S3_ClearStereoBuffer(S3_Sample** ADst, uint32 ALength);
void S3_AddStereoBuffer(S3_Sample** ADst, S3_Sample** AAdd, uint32 ALength);
void S3_ScaleStereoBuffer(S3_Sample** ADst, float AScale, uint32 ALength);
void S3_ScaleStereoBuffer(S3_Sample** ADst, float ALeft, float ARight, uint32 ALength);

// S3_PLUGIN_PROCESS_TICKSIZE

void S3_CopyStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc);
void S3_ClearStereoBuffer(S3_Sample** ADst);
void S3_AddStereoBuffer(S3_Sample** ADst, S3_Sample** AAdd);
void S3_ScaleStereoBuffer(S3_Sample** ADst, float AScale);
void S3_ScaleStereoBuffer(S3_Sample** ADst, float ALeft, float ARight);

//------------------------------
// mono -> stereo
//------------------------------

void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength);
void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength);
void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc, uint32 ALength);
void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc, uint32 ALength);

// S3_PLUGIN_PROCESS_TICKSIZE

void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc);
void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc);
void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc);
void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void S3_CopyMonoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength) {
  S3_Memcpy(ADst[0],ASrc[0],ALength*sizeof(S3_Sample));
}

//----------

void S3_ClearMonoBuffer(S3_Sample** ADst, uint32 ALength) {
  S3_Memset(ADst[0],0,ALength*sizeof(S3_Sample));
}

//----------

void S3_AddMonoBuffer(S3_Sample** ADst, S3_Sample** AAdd, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* add0 = AAdd[0];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void S3_ScaleMonoBuffer(S3_Sample** ADst, float AScale, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void S3_CopyMonoBuffer(S3_Sample* ADst, S3_Sample* ASrc, uint32 ALength) {
  S3_Memcpy(ADst,ASrc,ALength*sizeof(S3_Sample));
}

//----------

void S3_ClearMonoBuffer(S3_Sample* ADst, uint32 ALength) {
  S3_Memset(ADst,0,ALength*sizeof(S3_Sample));
}

//----------

void S3_AddMonoBuffer(S3_Sample* ADst, S3_Sample* AAdd, uint32 ALength) {
  S3_Sample* dst0 = ADst;
  S3_Sample* add0 = AAdd;
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void S3_ScaleMonoBuffer(S3_Sample* ADst, float AScale, uint32 ALength) {
  S3_Sample* dst0 = ADst;
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------
// mono, ticksize
//------------------------------

/*

void S3_CopyMonoBuffer(S3_Sample** ADst, S3_Sample** ASrc) {
  S3_Memcpy(ADst[0],ASrc[0],S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
}

//----------

void S3_ClearMonoBuffer(S3_Sample** ADst) {
  S3_Memset(ADst[0],0,S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
}

//----------

void S3_AddMonoBuffer(S3_Sample** ADst, S3_Sample** AAdd) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* add0 = AAdd[0];
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void S3_ScaleMonoBuffer(S3_Sample** ADst, float AScale) {
  S3_Sample* dst0 = ADst[0];
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void S3_CopyMonoBuffer(S3_Sample* ADst, S3_Sample* ASrc) {
  S3_Memcpy(ADst,ASrc,S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
}

//----------

void S3_ClearMonoBuffer(S3_Sample* ADst) {
  S3_Memset(ADst,0,S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
}

//----------

void S3_AddMonoBuffer(S3_Sample* ADst, S3_Sample* AAdd) {
  S3_Sample* dst0 = ADst;
  S3_Sample* add0 = AAdd;
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void S3_ScaleMonoBuffer(S3_Sample* ADst, float AScale) {
  S3_Sample* dst0 = ADst;
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

*/

//------------------------------
// stereo
//------------------------------

void S3_CopyStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength) {
  S3_Memcpy(ADst[0],ASrc[0],ALength*sizeof(S3_Sample));
  S3_Memcpy(ADst[1],ASrc[1],ALength*sizeof(S3_Sample));
}

//----------

void S3_ClearStereoBuffer(S3_Sample** ADst, uint32 ALength) {
  S3_Memset(ADst[0],0,ALength*sizeof(S3_Sample));
  S3_Memset(ADst[1],0,ALength*sizeof(S3_Sample));
}

//----------

void S3_AddStereoBuffer(S3_Sample** ADst, S3_Sample** AAdd, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* add0 = AAdd[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* add1 = AAdd[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

void S3_ScaleStereoBuffer(S3_Sample** ADst, float AScale, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void S3_ScaleStereoBuffer(S3_Sample** ADst, float ALeft, float ARight, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  for(uint32 i=0; i<ALength; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

//------------------------------
// stereo, ticksize
//------------------------------

/*

void S3_CopyStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc) {
  S3_Memcpy(ADst[0],ASrc[0],S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
  S3_Memcpy(ADst[1],ASrc[1],S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
}

//----------

void S3_ClearStereoBuffer(S3_Sample** ADst) {
  S3_Memset(ADst[0],0,S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
  S3_Memset(ADst[1],0,S3_PLUGIN_PROCESS_TICKSIZE*sizeof(S3_Sample));
}

//----------

void S3_AddStereoBuffer(S3_Sample** ADst, S3_Sample** AAdd) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* add0 = AAdd[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* add1 = AAdd[1];
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

void S3_ScaleStereoBuffer(S3_Sample** ADst, float AScale) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void S3_ScaleStereoBuffer(S3_Sample** ADst, float ALeft, float ARight) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

*/

//------------------------------
// mono -> stereo
//------------------------------

void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc[0];
  for (uint32 i=0; i<ALength; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc[0];
  for(uint32 i=0; i<ALength; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------

void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc;
  for (uint32 i=0; i<ALength; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc, uint32 ALength) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc;
  for(uint32 i=0; i<ALength; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------
// mono -> stereo, ticksize
//------------------------------

/*

void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc[0];
  for (uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample** ASrc) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc[0];
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------

void S3_CopyMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc;
  for (uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void S3_AddMonoToStereoBuffer(S3_Sample** ADst, S3_Sample* ASrc) {
  S3_Sample* dst0 = ADst[0];
  S3_Sample* dst1 = ADst[1];
  S3_Sample* src0 = ASrc;
  for(uint32 i=0; i<S3_PLUGIN_PROCESS_TICKSIZE; i++) {
    S3_Sample spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

*/

//----------------------------------------------------------------------
#endif

