#ifndef s3_library_included
#define s3_library_included
//----------------------------------------------------------------------

#include <dlfcn.h>

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Library {
  protected:
    bool  MLibLoaded;
    void* MLibHandle;
  public:
    S3_Library();
    virtual ~S3_Library();
  //public:
  //  bool  loaded(void) { return MLibLoaded; }
  public:
    virtual bool  load(const char* AFilename);
    virtual void  unload(void);
    virtual void* getSymbol(const char* ASymbol);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

S3_Library::S3_Library() {
  MLibLoaded = false;
  MLibHandle = S3_NULL;
}

//----------

S3_Library::~S3_Library() {
  MLibLoaded = false;
  MLibHandle = S3_NULL;
}

//----------------------------------------

bool S3_Library::load(const char* AFilename) {
  MLibHandle = dlopen(AFilename,RTLD_LAZY | RTLD_LOCAL ); // RTLD_NOW, RTLD_LAZY
  if (!MLibHandle) return false;
  MLibLoaded = true;
  return true;
}

//----------

void S3_Library::unload(void) {
  if (MLibLoaded && MLibHandle) {
    dlclose(MLibHandle);
    MLibLoaded = false;
    MLibHandle = S3_NULL;
  }
}

//----------

void* S3_Library::getSymbol(const char* ASymbol) {
  void* result = dlsym(MLibHandle,ASymbol);
  return result;
}

//----------------------------------------------------------------------
#endif
