#ifndef s3_file_included
#define s3_file_included
//----------------------------------------------------------------------

#include <stdio.h>

//----------------------------------------------------------------------

//#define S3_FILE_RB    (char*)"rb"
//#define S3_FILE_R     (char*)"r"
//#define S3_FILE_WB    (char*)"wb"
//#define S3_FILE_W     (char*)"w"
//#define S3_FILE_AB    (char*)"ab"
//#define S3_FILE_A     (char*)"a"

// file modes

#define s3_fm_readBinary    (char*)"rb"
#define s3_fm_readText      (char*)"r"
#define s3_fm_writeBinary   (char*)"wb"
#define s3_fm_writeText     (char*)"w"
#define s3_fm_appendBinary  (char*)"ab"
#define s3_fm_appendText    (char*)"w"

//----------------------------------------------------------------------

class S3_File {

  protected:

    FILE*       MHandle;
    const char* MName;
    const char* MMode;

  public:

    /*
      does not open the file!
    */

    S3_File() {
      MHandle = S3_NULL;
      MName   = "";
      MMode   = "";
    }

    S3_File(const char* AFilename, const char* AMode=s3_fm_readBinary) {
      open(AFilename,AMode);
    }

    //----------

    virtual ~S3_File() {
      close();
    }

    //----------

    virtual
    bool exists(const char* AName) {
      if (MHandle) return true;
      FILE* fp = fopen(AName,s3_fm_readText);
      if (fp) {
        close();
        return true;
      }
      else {
        return false;
      }
    }

    //----------

    virtual
    uint32 length(void) {
      uint32 len = 0;
      if (MHandle) {
        int32 pos = ftell(MHandle);
        fseek(MHandle,0,SEEK_END);
        len = ftell(MHandle);
        fseek(MHandle,pos,SEEK_SET);
      }
      else {
        FILE* fp = fopen(MName,s3_fm_readText);
        if (!fp) return 0;
        fseek(fp,0,SEEK_END);
        len = ftell(fp);
        //fseek(fp,0,SEEK_SET);
        fclose(fp);
      }
      return len;
    }

    //----------

    virtual
    bool open(const char* AFilename, const char* AMode=s3_fm_readBinary) {
      MName   = AFilename;
      MMode   = AMode;
      MHandle = fopen(MName,MMode);
      if (MHandle) return true;
      return false;
    }

    //----------

    virtual
    void close(void) {
      if (MHandle) fclose(MHandle);
      MHandle = S3_NULL;
    }

    //----------

    virtual
    uint32 read(void* ABuffer, uint32 ALength, uint32 ASize=1) {
      uint32 num_read = fread(ABuffer,ASize,ALength,MHandle);
      return num_read;
    }

    //----------

    virtual
    uint32 write(void* ABuffer, uint32 ALength, uint32 ASize=1) {
      uint32 num_written = fwrite(ABuffer,ASize,ALength,MHandle);
      return num_written;
    }

};

//----------------------------------------------------------------------


/*

#include <stdio.h>
#include "s3_basepath.h"
#include "s3_const.h"
#include "s3_memory.h"
#include "s3_string.h"
#include "s3_utils.h"

//----------

#define S3_FILE_RB    (char*)"rb"
#define S3_FILE_R     (char*)"r"

#define S3_FILE_WB    (char*)"wb"
#define S3_FILE_W     (char*)"w"
#define S3_FILE_AB    (char*)"ab"
#define S3_FILE_A     (char*)"a"

#define S3_FILE_RAUTO 0

class S3_File {

  private:

    char*   MMode;
    FILE*   MFile;
    char    MFilePath[S3_MAX_PATH_SIZE];
    void*   MBuffer;
    uint32  MLength;
    uint32  MSize;

  public:

    void*   getBuffer(void) { return MBuffer; }
    uint32  getLength(void) { return MLength; }

  public:

    S3_File() {
      MFile = S3_NULL;
      MBuffer = S3_NULL;
    }

    //----------

    virtual ~S3_File() {
      close();
      freebuf();
    }

    //----------

    void close(void) {
      if (MFile) fclose(MFile);
      MFile = S3_NULL;
    }

    //----------

    int test(const char* AFilePath, char* AMode) {
      MFile = fopen(AFilePath, AMode);
      if (MFile) {
        close();
        return 1;
      }
      else {
        return 0;
      }
    }

    //----------

    void freebuf(void) {
      if (MBuffer) SFree(MBuffer);
      MBuffer = S3_NULL;
    }

    //----------

    char* path(const char* AFileName) {
      MFilePath[0] = '\0';
      SGetBasePath(MFilePath);
      SStrcat(MFilePath, (char*)AFileName);
      return MFilePath;
    }

    //----------

    uint32 length(const char* AFileName, char* AMode = S3_FILE_RB) {
      MMode = AMode;
      //path(AFileName);
      FILE* tmp_file = fopen(MFilePath, MMode);
      if (!tmp_file) return 0;
      fseek(tmp_file, 0, SEEK_END);
      MLength = ftell(tmp_file);
      fseek(tmp_file, 0, SEEK_SET);
      fclose(tmp_file);
      return MLength;
    }

    //----------

    int32 read(const char* AFileName, void* ABuffer, const uint32 ALength = S3_FILE_RAUTO,
                       const uint32 ASize = 1, char* AMode = S3_FILE_RB) {
      MFile   = S3_NULL;
      MBuffer = ABuffer;
      MLength = ALength;
      MSize   = ASize;
      MMode   = AMode;
      //path(AFileName);
      SStrcpy(MFilePath,(char*)AFileName);
      MFile = fopen(MFilePath,MMode);
      if (!MFile) return -1;
      if (ALength == S3_FILE_RAUTO) {
        MLength = length(AFileName,AMode);
        if (!MLength) return -2;
        MBuffer = SMalloc(MLength+1); // +1 for final 0
      }
      unsigned long result = fread(MBuffer,MSize,MLength,MFile);
      if (!result) return -3;
      close();
      char* ptr = (char*)MBuffer;
      ptr[MLength] = '\0';
      return result;
    }

    //----------

    int32 write(const char* AFileName, void* ABuffer, const uint32 ALength = 0,
                        const uint32 ASize = 1, char* AMode = S3_FILE_WB) {
      if (ABuffer == S3_NULL) {
        //DTrace("h_File.write, #ERR no buffer for: "<< MFilePath);
        return 0;
      }
      MFile   = S3_NULL;
      MBuffer = ABuffer;
      MLength = ALength;
      MSize   = ASize;
      MMode   = AMode;
      //path(AFileName);
      MFile = fopen(MFilePath, MMode);
      if (!MFile) return -1;
      if (!MLength) return -2;
      if (!fwrite(MBuffer, MSize, MLength, MFile)) {
        return -3;
      }
      close();
      return 1;
    }

};

*/

//----------------------------------------------------------------------
#endif
