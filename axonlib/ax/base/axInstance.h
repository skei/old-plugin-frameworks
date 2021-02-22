#ifndef axInstance_included
#define axInstance_included
//----------------------------------------------------------------------

#define is_Open     1
#define is_Close    2
#define is_Suspend  3
#define is_Resume   4
#define is_Rate     5
#define is_Block    6
#define is_Bypass   7

//----------------------------------------------------------------------

class axInstance : public axParameterListener
{
  public:
    //axInstance(axBase* aBase)  { /*printf("axInstance.constructor\n");*/ }
    virtual ~axInstance()      { /*trace("axInstance.destructor");*/ }
    //
    virtual void    doStateChange(int aState) {}
    virtual void    doSetParameter(axParameter* aParameter) {}
    virtual void    doPreProgram(int aProgram) {}
    virtual void    doSetProgram(int aProgram) {}
    virtual void    doProcessMidi(int aOffset, unsigned char aMsg1, unsigned char aMsg2, unsigned char aMsg3) {}
    virtual bool    doProcessBlock(float** aInputs, float** aOutputs, int Length) { return false; }
    virtual void    doProcessSample(float** aInputs, float** aOutputs) {}
    virtual void    doPostProcess(float** aInputs, float** aOutputs, int Length) {}
    //
    virtual void*   doOpenEditor(void* ptr) { return NULL; }
    virtual void    doCloseEditor(void) {}
    virtual void    doIdleEditor(void) {}
    //
    //virtual void    notifyParamChanged(axParameter* aParameter) {}
    //virtual void    notifyResizeEditor(int aWidth, int aHeight) {}
    //
    virtual void    onChange(axParameter* aParameter) {}

//    virtual void        appendParameter(axParameter* aParameter) {}
//    virtual void        deleteParameters(void)
//    virtual void        setupParameters(void)
//    virtual void        prepareParameters(void)
//    virtual void        transferParameters(void)
//
//    virtual void        appendProgram(axProgram* aProgram)
//    virtual void        deletePrograms(void)
//    virtual void        setupPrograms()
//    virtual axProgram*  createDefaultProgram(void)
//    virtual void        saveProgram(int aIndex)
//    virtual void        loadProgram(int aIndex)

    virtual bool    isEditorOpen(void)      { return false; }
    virtual axRect  getEditorRect(void)     { return axRect(0,0,100,100); }
    virtual int     getCurrentProgram(void) { return 0; }
    virtual int     getPlayState(void)      { return 0; }
    virtual double  getSamplePos(void)      { return 0; }
    virtual double  getSampleRate(void)     { return 0; } // 44100
    virtual double  getBeatPos(void)        { return 0; }
    virtual double  getTempo(void)          { return 0; }

};

//----------------------------------------------------------------------
#endif
