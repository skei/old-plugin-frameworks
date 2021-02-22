#ifndef axFormatExe_included
#define axFormatExe_included
//----------------------------------------------------------------------

#include "core/axDebug.h"
#include "par/axParameter.h"
#include "par/axProgram.h"

#define AX_WIN_DEFAULT (AX_WIN_BUFFERED|AX_WIN_MSGDELETE)

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptorExe : public axDescriptor
{
  public:
    axDescriptorExe(axBase* aBase) /*: axDescriptor(aBase)*/ { /*trace("axDescriptorExe.constructor");*/ }
    virtual ~axDescriptorExe() { /*trace("axDescriptorExe.destructor");*/ }
};

typedef axDescriptorExe AX_DESCRIPTOR;

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstanceExe : public axInstance
{
  protected:
    axBase* mBase;
  private:
    //axRect mEditorRect;
    axParameters        mParameters;
    axPrograms          mPrograms;
    int                 mCurrentProgram;
    int                 mFlags;
    bool                mEditorOpen;
    axRect              mEditorRect;
    //void*               mEditorWindow;
    #ifndef AX_NOGUI
    axEditor*           mEditor;
    #endif
    int                 mPlayState;
    double              mSamplePos;
    double              mSampleRate;
    double              mBeatPos;
    double              mTempo;
    long                mBlockSize;

  public:

    axInstanceExe(axBase* aBase) //: axInstance(aBase)
      {
        //printf("axInstanceExe\n");
        //printf("aBase: %i\n",(int)aBase);
        mBase = aBase;
        /*trace("axInstanceExe.constructor");*/
        mFlags          = 0;
        mCurrentProgram = 0;
        mPlayState      = 0;
        mSamplePos      = 0;
        mSampleRate     = 0;
        mBeatPos        = 0;
        mTempo          = 0;
        mBlockSize      = 0;
        mEditorOpen     = false;
        #ifndef AX_NOGUI
        mEditor         = NULL;
        #endif
        mEditorRect     = aBase->getDescriptor()->getEditorRect();
        //printf("... axInstanceExe... ok\n");
      }
    virtual ~axInstanceExe()
      {
        /*trace("axInstanceExe.destructor");*/
        #ifndef AX_NOAUTODELETE
          deleteParameters();
          //deletePrograms();
        #endif
        }
    virtual axRect getEditorRect(void) { return mEditorRect; }
    virtual axParameter* getParameter(int aIndex) { return mParameters[aIndex]; }

    //----------------------------------------
    // parameters
    //----------------------------------------

    // should be in axInstanceBase
    virtual void appendParameter(axParameter* aParameter)
      {
        int index = mParameters.size();
        aParameter->setIndex(index);
        mParameters.append(aParameter);
      }

    //----------

    virtual void deleteParameters(void)
      {
        for (int i=0; i<mParameters.size(); i++) delete mParameters[i];
      }

    //----------

    virtual void setupParameters(void)
      {
        prepareParameters();
        transferParameters();
      }

    //----------

    // tells the vst host how many parameters we have
    // needs to be done in the constructor!
    // and initializes parameter-index
    // needed for parameter -> widget mapping if we have an editor

    virtual void prepareParameters(void)
      {
        int num = mParameters.size();
        //vst_setNumParams(num); // vst
        for (int i=0; i<num; i++)
        {
          __unused axParameter* par = mParameters[i];
          //par->setIndex(i);
          //doSetParameter(par);
        }
      }

    //----------

    // calls doSetParameter for all parameters
    // so that you can fetch them, and setup initial values
    // for your plugin

    virtual void transferParameters(void)
      {
        int num = mParameters.size();
        for (int i=0; i<num; i++)
        {
          axParameter* par = mParameters[i];
          doSetParameter(par);
        }
      }

    //----------

    //----------------------------------------
    // programs/banks
    //----------------------------------------

    virtual void appendProgram(axProgram* aProgram)
      {
        mPrograms.append(aProgram);
      }

    //----------

    virtual void deletePrograms(void)
      {
        for (int i=0; i<mPrograms.size(); i++) delete mPrograms[i];
      }

    //----------

    virtual void setupPrograms()
      {
        __unused int num = mPrograms.size();        
        //if (num>0) { vst_setNumPrograms(num); } // vst
        //else
        {
          axProgram* prog = createDefaultProgram();
          appendProgram(prog);
          //vst_setNumPrograms(1);
        }
      }

    //----------

    virtual axProgram* createDefaultProgram(void)
      {
        int num = mParameters.size();
        axProgram* prog = new axProgram("default",num);
        for (int i=0; i<num; i++)
        {
          float val = mParameters[i]->doGetValue();
          prog->setValue(i,val);
        }
        return prog;
      }

    //----------

    virtual void saveProgram(int aIndex)
      {
        //trace("saveProgram");
        if (mPrograms.size() > 0)
        {
          int num = mParameters.size();
          axProgram* prog = mPrograms[aIndex];
          for (int i=0; i<num; i++)
          {
            float val = mParameters[i]->doGetValue();
            prog->setValue(i,val);
          }
        }
      }

    //----------

    virtual void loadProgram(int aIndex)
      {
        //trace("loadProgram");
        if (mPrograms.size() > 0)
        {
          int num = mParameters.size();
          axProgram* prog = mPrograms[aIndex];
          for (int i=0; i<num; i++)
          {
            //float val = mParameters[i]->doGetValue();
            //prog->setValue(i,val);
            float val = prog->getValue(i);
            //trace("i: "<<i<<" val: "<<val);
            mParameters[i]->doSetValue(val,true);
          }
        }
      }

    //----------------------------------------
    // time/tempo/sync
    //----------------------------------------

    virtual void updateTimeInfo(void)
      {
        //trace("updateTimeInfo");
        //mTimeInfo   = vst_getTime( kVstPpqPosValid + kVstTempoValid );
        mPlayState  = 0; // mTimeInfo->flags & 0xff;
        mSamplePos  = 0; // mTimeInfo->samplePos;
        mSampleRate = 0; // mTimeInfo->sampleRate;
        mBeatPos    = 0; // mTimeInfo->ppqPos;
        mTempo      = 0; // mTimeInfo->tempo;
      }

    //----------------------------------------
    // midi
    //----------------------------------------

    void sendMidiClear(void)
      {
        //mMidiEventList.numEvents = 0;
      }

    //----------

    // send all midi event in list to host
    // and clear the list
    void sendMidiAll(void)
      {
//        //trace("sendMidiAll");
//        int num = mMidiEventList.numEvents;
//        if( num>0 )
//        {
//          //sendVstEventsToHost( (VstEvents*)&mMidiEventList );
//          axVstEvents* events = &mMidiEventList;
//          vst_processEvents( (VstEvents*)events/*(VstEvents*)&mMidiEventList*/ );
//          sendMidiClear();
//          //mMidiEventList.numEvents = 0;
//        }
      }

    //----------

    // append midi event to list
    /*virtual*/ void sendMidi(int offset, unsigned char msg1, unsigned char msg2, unsigned char msg3)
      {
//        int num = mMidiEventList.numEvents;
//        VstMidiEvent* event = (VstMidiEvent*)( mMidiEventList.events[ num ] );
//        event->type         = kVstMidiType;
//        event->deltaFrames  = offset;
//        event->midiData[0]  = msg1;
//        event->midiData[1]  = msg2;
//        event->midiData[2]  = msg3;
//        event->midiData[3]  = 0;
//        event->byteSize     = sizeof(VstMidiEvent);
//        event->flags        = 0;
//        event->noteLength   = 0;
//        event->noteOffset   = 0;
//        event->detune       = 0;
//        mMidiEventList.numEvents+=1;
      }

    //----------------------------------------
    // editor
    //----------------------------------------

//    virtual void setupEditor(int aWidth, int aHeight)
//      {
//        //trace("setupEditor...");
//        vst_hasEditor(true);
//        //trace(".");
//        mEditorRect.set(0,0,aWidth,aHeight);
//        //trace(".");
//        mFlags |= if_HasEditor;
//        //trace("...setupEditor");
//      }

    //----------------------------------------
    // axInstanceBase
    //----------------------------------------

//    // called from editor
//    // indicating, about to change the value..
//    virtual void notifyParamChanged(axParameter* aParameter)
//      {
//        //trace("notifyParamChanged");
//        int index = aParameter->getIndex();
//        float value = aParameter->doGetValue(); // 0..1
//        //vst_setParameterAutomated(index,value);
//        doSetParameter(aParameter);
//      }

    virtual void onChange(axParameter* aParameter)
      {
        //trace("onChange par");
        #ifndef AX_NOGUI
        if (mEditorOpen) mEditor->paramChanged(aParameter);
        #endif
        doSetParameter(aParameter);
      }

    //----------

    //virtual axRect getEditorRect(void)
    //  {
    //    return mEditorRect;
    //  }

    //----------

//    virtual void notifyResizeEditor(int aWidth, int aHeight)
//      {
//        mEditorRect.w = aWidth;
//        mEditorRect.h = aHeight;
//        //vst_sizeWindow(aWidth, aHeight); // vst
//      }


};

typedef axInstanceExe AX_INSTANCE;

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

class axFormatExe : public axFormat
{

  //friend int main(void);
  friend int main(int argc, char** argv);

  private:
    int result;

  //----------

  protected:
    axBase*       mBase;
    axDescriptor* mDescriptor;
    axInstance*   mInstance;
    #ifndef AX_NOGUI
    axEditor*     mEditor;
    #endif

  protected:

    virtual void* entrypoint(void* ptr)
      {
        //printf("axFormatExe.constructor\n");
        //printf("mBase: 0x%08X\n",(int)mBase);
        //printf("mDescriptor: 0x%08X\n",(int)&mDescriptor);
        //printf("mInstance: 0x%08X\n",(int)&mInstance);
        //printf("1\n");
        mDescriptor = mBase->getDescriptor();
        //printf("2\n");
        axInstance* inst; /*=*/
        //printf("3\n");
        inst = mBase->createInstance();
        //printf("4\n");
        mInstance   = inst;
        //printf("5\n");
        #ifndef AX_NOGUI
        {
          mEditor = (axEditor*)mInstance->doOpenEditor(ptr); // NULL
          if (mEditor) { mEditor->eventLoop(); }
          mInstance->doCloseEditor();
        }
        #endif
        result = 0;
        return &result;
      }

  //----------

  public:

    axFormatExe(axBase* aBase) // : axFormat(aBase)
      {
        //trace("axFormatExe.constructor");
        mBase = aBase;
        //mFormatFlags = ff_None;
      }

    virtual ~axFormatExe()
      {
        //trace("axFormatExe.destructor");
        //mInstance->doCloseEditor();
        //delete mDescriptor;
        delete mInstance;
      }

    //----------

    virtual char* getFormatName(void)
      {
        return (char*)"exe";
      }

};

//----------

typedef axFormatExe AX_FORMAT;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

// _PL = platform   win32, linux
// _IF = interface  win32, linux, nogui
// _FO = format     exe, vst, ladspa

#define AX_ENTRYPOINT(_PL,_IF,_FO,_D,_I)                                      \
                                                                              \
int main(int argc, char** argv)                                               \
{                                                                             \
  _AX_DEBUG_SETUP                                                             \
  axBaseImpl<_PL,_IF,_FO,_D,_I>* base = new axBaseImpl<_PL,_IF,_FO,_D,_I>();  \
  gGlobalScope.setBase(base);                                                 \
  _FO* format = (_FO*)base->getFormat();                                      \
  int result = *(int*)format->entrypoint(NULL);                               \
  return result;                                                              \
}

//TODO:
//  entrypoint.ptr = parsed command line arguments?

//----------------------------------------------------------------------
#endif
