#ifndef s3_plugin_exe_included
#define s3_plugin_exe_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

//class S3_Plugin_Exe;
//typedef S3_Plugin_Exe S3_Plugin_Implementation;

//----------

#include "plugin/s3_plugin_base.h"

class S3_Plugin_Exe
: public S3_Plugin_Base {
  protected:
    float MSampleRate;

  public:
    S3_Plugin_Exe();
    virtual ~S3_Plugin_Exe();
    virtual void* main(void* AMaster) final;

};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Plugin_Exe::S3_Plugin_Exe()
: S3_Plugin_Base() {
  MSampleRate = 44100;
  MBlockSize  = 256;
}

S3_Plugin_Exe::~S3_Plugin_Exe() {
}

void* S3_Plugin_Exe::main(void* AMaster) {
  // init & run 'host emulation'..
  return S3_NULL;
}

//----------------------------------------------------------------------
#endif
