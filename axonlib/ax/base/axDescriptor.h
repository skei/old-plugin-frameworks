#ifndef axDescriptor_included
#define axDescriptor_included
//----------------------------------------------------------------------

// parameter types
#define pa_Par    0   // axParameter
#define pa_Float  1   // parFloat
#define pa_Pow    2   // parFloatPow
#define pa_Int    3   // parInteger
#define pa_Ctm    4   // parFloatCustom

//----------

struct axParamInfo
{
  int     mType;  // float,int,pow
  char*   mName;  // parameter name/label
  float   mDef;   // default value
  float   mMin;   // minimum value
  float   mMax;   // maximum value
  float   mStep;  // stepsize (0 for no stepping)
  char**  mStr;   // default value
  float   mAux;   // auxilliary value.. parFloatPow, ...
  float  (*mPtr)(float);  // function pointer
};

//----------------------------------------------------------------------

// default names for (default) stereo in/out
static char* g_default_stereo_inputs[]  = { (char*)"in1", (char*)"in2" };
static char* g_default_stereo_outputs[] = { (char*)"out1",(char*)"out2" };

//----------------------------------------------------------------------

class axDescriptor
{
  public:
    //axDescriptor(axBase* aBase)  {}
    virtual ~axDescriptor() {}
    //
    virtual char*         getName(void)             { return (char*)"plugin"; }
    virtual char*         getAuthor(void)           { return (char*)"anonymous"; }
    virtual char*         getProduct(void)          { return (char*)"unknown plugin"; }
    virtual int           getVersion(void)          { return 0; }
    virtual unsigned int  getUniqueId(void)         { return 0x00000000; }
    virtual bool          isSynth(void)             { return false; }
    virtual bool          hasEditor(void)           { return false; }
    virtual axRect        getEditorRect(void)       { return axRect(0,0,100,100); }
    virtual int           getNumInputs(void)        { return 2; }
    virtual char*         getInputName(int aIndex)  { return g_default_stereo_inputs[aIndex]; }
    virtual int           getNumOutputs(void)       { return 2; }
    virtual char*         getOutputName(int aIndex) { return g_default_stereo_outputs[aIndex]; }
    virtual int           getNumParams(void)        { return 0; }
    virtual axParamInfo   getParamInfo(int aIndex)  { return axParamInfo(); }    // !!!
    virtual int           getNumProgs(void)         { return 0; }
};

#define DESCRIPTOR_CONSTRUCT(v) v(axBase* aBase) : AX_DESCRIPTOR(aBase) {}

#define DESCRIPTOR_NAME(v)        virtual char*         getName(void)             { return (char*) #v ; }
#define DESCRIPTOR_AUTHOR(v)      virtual char*         getAuthor(void)           { return (char*) #v ; }
#define DESCRIPTOR_PRODUCT(v)     virtual char*         getProduct(void)          { return (char*) #v ; }
#define DESCRIPTOR_VERSION(v)     virtual int           getVersion(void)          { return v; }
#define DESCRIPTOR_UNIQUEID(v)    virtual unsigned int  getUniqueId(void)         { return v; }
#define DESCRIPTOR_ISSYNTH(v)     virtual bool          isSynth(void)             { return v; }
#define DESCRIPTOR_HASEDITOR(v)   virtual bool          hasEditor(void)           { return v; }
#define DESCRIPTOR_EDITORRECT(v)  virtual axRect        getEditorRect(void)       { return v; }
#define DESCRIPTOR_NUMINPUTS(v)   virtual int           getNumInputs(void)        { return v; }
#define DESCRIPTOR_INPUTNAME(v)   virtual char*         getInputName(void)        { return (char*) #v ; }
#define DESCRIPTOR_NUMOUTPUTS(v)  virtual int           getNumOutputs(void)       { return v; }
#define DESCRIPTOR_OUTPUTNAME(v)  virtual char*         getOutputName(void)       { return (char*) #v ; }
#define DESCRIPTOR_NUMPARAMS(v)   virtual int           getNumParams(void)        { return v; }
#define DESCRIPTOR_PARAMINFOS(v)  virtual axParamInfo   getParamInfo(int aIndex)  { return v[aIndex]; }
#define DESCRIPTOR_NUMPROGS(v)    virtual int           getNumProgs(void)         { return v; }

//----------------------------------------------------------------------
#endif
