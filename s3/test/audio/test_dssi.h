
#define S3_DEBUG_LADSPA
#define S3_DEBUG_DSSI

#include "s3.h"
#include "audio/s3_dssi.h"

int main(void) {
  S3_Dssi dssi;
  dssi.load("/usr/lib/dssi/whysynth.so");
  dssi.init(0);
  dssi.instantiate(44100);

  const DSSI_Program_Descriptor* prog_descr;
  uint32 prog_count = 0;
  prog_descr = dssi.get_program(prog_count);
  while (prog_descr) {
    S3_Trace("program %i : bank %i program %i name '%s'\n",prog_count,prog_descr->Bank,prog_descr->Program,prog_descr->Name);
    prog_count++;
    prog_descr = dssi.get_program(prog_count);
  }
  //dssi.activate();
  //dssi.deactivate();
  dssi.cleanup();
  dssi.unload();
  return 0;
}

