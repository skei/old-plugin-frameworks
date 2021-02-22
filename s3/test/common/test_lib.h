
#include "s3.h"
#include "common/s3_library.h"
#include "audio/s3_ladspa2.h"
#include "audio/s3_dssi2.h"

int main(void) {
  //S3_Ladspa lib;
  //lib.loadLibrary("/usr/lib/ladspa/guitarix.so");
  S3_Dssi lib;
  lib.loadLibrary("/usr/lib/dssi/whysynth.so");
  lib.init();
  lib.initDescriptor(0);
  lib.initPorts();
  lib.dumpInfo();
  //lib.instantiate(44100);
  //S3_Trace("num audio inputs    : %i\n",lib.numAudioInputs());
  //S3_Trace("num audio outputs   : %i\n",lib.numAudioOutputs());
  //S3_Trace("num control inputs  : %i\n",lib.numControlInputs());
  //S3_Trace("num control outputs : %i\n",lib.numControlOutputs());
  //lib.cleanup();
  lib.unloadLibrary();
  return 0;
}


