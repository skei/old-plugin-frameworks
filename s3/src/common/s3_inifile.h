#ifndef s3_inifile_included
#define s3_inifile_included
//----------------------------------------------------------------------

#include "common/s3_file.h"
#include "common/s3_parser.h"
#include "common/s3_paths.h"

class S3_Inifile {
  private:
    uint32      MBufferSize;
    void*       MBuffer;
    S3_File     MFile;
    S3_Parser   MParser;
  public:
    S3_Inifile();
    ~S3_Inifile();
    bool load(void);
    bool load(const char* filename);
    void unload(void);
};

//----------------------------------------------------------------------

S3_Inifile::S3_Inifile() {
  MBufferSize = 0;
  MBuffer = S3_NULL;
}

//----------

S3_Inifile::~S3_Inifile() {
  if (MBuffer) S3_Free(MBuffer);
}

//----------

bool S3_Inifile::load(void) {
  char filename[256];
  S3_GetBaseFile(filename);
  S3_StripFileExt(filename);
  S3_Strcat(filename,".ini");
  return load(filename);
}

//----------

bool S3_Inifile::load(const char* filename) {
  S3_Trace("Loading: '%s'\n",filename);
  if (MFile.exists(filename)) {
    MFile.open(filename,s3_fm_readBinary);
    MBufferSize = MFile.length();
    if (MBufferSize>0) {
      MBuffer = S3_Malloc(MBufferSize);
      MFile.read(MBuffer,MBufferSize);
      MParser.parse((const char*)MBuffer,MBufferSize);
      MParser.dumpTokens();
    }
    MFile.close();
    //char* filename = MParser.token(0);
    //if (MParser.numTokens() == 2) index = S3_StringToInt(MParser.token(1));
  }
  return false;
}

//----------

void S3_Inifile::unload(void) {
  if (MBuffer) S3_Free(MBuffer);
  MBuffer = S3_NULL;
}

//----------------------------------------------------------------------
#endif
