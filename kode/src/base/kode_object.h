#ifndef kode_object_included
#define kode_object_included
//----------------------------------------------------------------------

class KODE_Object {
  public:
    KODE_Object() {}
    virtual ~KODE_Object() {}
    virtual void initialize() {}
    virtual void terminate() {}
    virtual const char* getObjectName() { return "KODE_Object"; }
};

//----------------------------------------------------------------------
#endif