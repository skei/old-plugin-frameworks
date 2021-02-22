#ifndef axFormatLadspa_included
#define axFormatLadspa_included
//----------------------------------------------------------------------

#include "../extern/ladspa.h" //TODO: proper ladspa sdk
#include "core/axDefines.h"
#include "par/parFloat.h"
#include "par/parInteger.h"

// inputs + outputs + parameters
#define MAX_LADSPA_PORTS    256

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class axDescriptorLadspa : public axDescriptor
{
  public:
    axDescriptorLadspa(axBase* aBase) {}
    virtual ~axDescriptorLadspa() {}
};

//----------

typedef axDescriptorLadspa AX_DESCRIPTOR;

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class axInstanceLadspa : public axInstance
{
  protected:
    axBase*       mBase;
    axDescriptor* mDescriptor;
    int           mNumInputs;
    int           mNumOutputs;
    int           mNumPorts;
    int           mNumParams;
    float*        mInputs[MAX_LADSPA_PORTS];
    float*        mOutputs[MAX_LADSPA_PORTS];
    LADSPA_Data*  mParamPtr[MAX_LADSPA_PORTS];
    LADSPA_Data   mParamPrev[MAX_LADSPA_PORTS];
    int           mBlockSize;
    axParameters  mParameters;

  public:

    axInstanceLadspa(axBase* aBase) /*: axInstance(aBase)*/
      {
        //trace("axInstanceLadspa.constructor");
        mBase       = aBase;
        mDescriptor = mBase->getDescriptor();
        mNumInputs  = mDescriptor->getNumInputs();
        mNumOutputs = mDescriptor->getNumOutputs();
        mNumParams  = mDescriptor->getNumParams();

        // setup parameters

        for (int i=0; i<mDescriptor->getNumParams(); i++)
        {
          axParamInfo p = mDescriptor->getParamInfo(i);

          float val = p.mDef;
          if (val<0.33) val=0.25;
          else if (val>0.66) val=0.75;
          else val=0.5;

          switch(p.mType)
          {
            case pa_Par:
              appendParameter( new axParameter( this,p.mName,"", val ) );
              break;
            case pa_Float:
              appendParameter( new parFloat(    this,p.mName,"",val, p.mMin, p.mMax, p.mStep ) );
              break;
            case pa_Pow:
              appendParameter( new parFloatPow( this,p.mName,"",val, p.mMin, p.mMax, p.mStep, p.mAux ) );
              break;
            case pa_Int:
              appendParameter( new parInteger(  this,p.mName,"",val, p.mMin, p.mMax, p.mStr ) );
              break;
            case pa_Ctm:
              appendParameter( new parFloatCustom( this,p.mName,"",p.mDef, p.mMin, p.mMax, p.mStep, p.mPtr ) );
              break;
          }
        }
        setupParameters();
      }

    //----------

    virtual ~axInstanceLadspa()
      {
        //trace("axInstanceLadspa.destructor");
      }

    //--------------------------------------------------
    // callbacks
    //--------------------------------------------------

    virtual void lad_connect_port(unsigned long Port, LADSPA_Data* DataLocation)
      {
        //trace("axFormatLadspa.lad_connect_port");
        unsigned int io = mNumInputs + mNumOutputs;
        if (Port<io) // audio in/out
        {
          //TODO: don't hardcode ports!!! (multichannel...)
          switch (Port)
          {
            case 0: mInputs[0]  = DataLocation; break;
            case 1: mInputs[1]  = DataLocation; break;
            case 2: mOutputs[0] = DataLocation; break;
            case 3: mOutputs[1] = DataLocation; break;
          }
        }
        else // parameter
        {
          int po = Port - io;
          mParamPtr[po] = DataLocation;
          // TODO: doSetParameter ?
        }
      }

    //----------

    virtual void lad_activate(void)
      {
        //trace("axFormatLadspa.lad_activate");
        doStateChange(is_Resume);
      }

    //----------

    virtual void lad_run(unsigned long SampleCount)
      {
        //trace("axFormatLadspa.lad_run");
        int io  = mNumInputs + mNumOutputs;
        int par = mNumParams; // mParameters.size();

        // check if any parameter have changed tjhe values since last time,
        // and if so, call doSetValue, so the new value is propagated
        // throughout the library (mainly our doSetParameter)

        for (int i=0; i<par; i++)
        {
          float val = *mParamPtr[i];
          if (val!=mParamPrev[i])
          {
            mParameters[i]->doSetValue(val,true);
            mParamPrev[i] = val;
          }
        }

        // process audio.. if doProcessBlock returns false, we call
        // doProcessSample for each sample in the block..
        // (and keep track of the pointers to the input/output buffers)

        mBlockSize = SampleCount;
        bool swallowed = doProcessBlock(mInputs,mOutputs,mBlockSize);
        if ( !swallowed )
        {
          float* ins[2];
          float* outs[2];
          ins[0]  = mInputs[0];
          ins[1]  = mInputs[1];
          outs[0] = mOutputs[0];
          outs[1] = mOutputs[1];
          int num = SampleCount;
          while (--num >= 0)
          {
            doProcessSample(ins,outs);
            ins[0]++;   ins[1]++;
            outs[0]++;  outs[1]++;
          } //SampleCount
        } //process_block

        // and eventual posrprocessing...

        doPostProcess(mInputs,mOutputs,mBlockSize);
      }

    //----------

    //virtual void lad_run_adding(unsigned long SampleCount) {}
    //virtual void lad_set_run_adding_gain(LADSPA_Data Gain) {}

    //----------

    virtual void lad_deactivate(void)
      {
        //trace("axFormatLadspa.lad_deactivate");
        doStateChange(is_Suspend);
      }

    //----------

    virtual void lad_cleanup(void)
      {
        //trace("axFormatLadspa.lad_cleanup");
        doStateChange(is_Close);
      }

    //--------------------------------------------------

    //virtual void appendParameter(axParameter* aParameter) {}
    //virtual void setupParameters(void) {}

    //----------

    // # methods have to be overloaded properly i.e. defined even if never
    // used otherwise the linker will apparently report the vtable errors

    //----------

    virtual void updateTimeInfo(void)
      {
      }

    //----------

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
        //trace("setupParameters");
        prepareParameters();
        //transferParameters();
      }

    //----------

    virtual void prepareParameters(void)
      {
        trace("prepareParameters");
        // no point in setting ladspa's port hints etc here...
        // the descriptor has already been delivered to the host
      }

    //----------

    // this can not be run in the constructor!
    // connect_port must have been called, so we
    // have the pointers...

    virtual void transferParameters(void)
      {
        //trace("transferParameters");
        int par = mParameters.size();
        for (int i=0; i<par; i++)
        {
          float val = mParameters[i]->getValue(); //doGetValue();
          *mParamPtr[i] = val;
        }
      }

};

//----------

typedef axInstanceLadspa AX_INSTANCE;

//----------------------------------------------------------------------
//
// format
//
//----------------------------------------------------------------------

class axFormatLadspa : public axFormat
{

  friend const LADSPA_Descriptor* ladspa_descriptor(unsigned long index);

  private:
    LADSPA_Descriptor ladspadescr;

  private:

    //--------------------------------------------------
    // static callback functions, host -> plugin
    //--------------------------------------------------

    // called by host to instantiate a plugin..
    // return the ptr to the instance (LADSPA_Handle)

    static LADSPA_Handle lad_instantiate_callback(const LADSPA_Descriptor* Descriptor, unsigned long SampleRate)
      {
        //trace("lad_instantiate_callback");
        axFormatLadspa* desc = (axFormatLadspa*)Descriptor->ImplementationData;
        return desc->lad_instantiate(SampleRate);
      }

    //----------

    // the rest are redirected to the instance(s)

    static void lad_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data * DataLocation)
      {
        //trace("lad_connect_port_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_connect_port(Port,DataLocation);
      }

    //----------

    static void lad_activate_callback(LADSPA_Handle Instance)
      {
        //trace("lad_activate_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_activate();
        //
        //
      }

    //----------

    static void lad_run_callback(LADSPA_Handle Instance, unsigned long SampleCount)
      {
        //trace("lad_run_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_run(SampleCount);
        //
        //
      }

    //----------

    //static void lad_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount)
    //  {
    //    axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //    inst->lad_run_adding(SampleCount);
    //  }

    //----------

    //static void lad_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain)
    //  {
    //    axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
    //    inst->lad_set_run_adding_gain(Gain);
    //  }

    //----------

    static void lad_deactivate_callback(LADSPA_Handle Instance)
      {
        //trace("lad_deactivate_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_deactivate();
        //
        //
      }

    //----------

    static void lad_cleanup_callback(LADSPA_Handle Instance)
      {
        //trace("lad_cleanup_callback");
        axInstanceLadspa* inst = (axInstanceLadspa*)Instance;
        inst->lad_cleanup();
        delete inst; // !!!
        //
        //
      }

    //--------------------------------------------------
    // redirected callback functions
    // (these could be in an instance class)
    //--------------------------------------------------

    // called from static callback function (above)

    // return ptr to instance
    virtual LADSPA_Handle lad_instantiate(unsigned long SampleRate)
      {
        trace("axFormatLadspa.lad_instantiate");
        //axInstanceLadspa* inst = (axInstanceLadspa*)mBase->createInstance();
        axInstance* inst = mBase->createInstance();
        //inst->mSampleRate = SampleRate;

        inst->doStateChange(is_Open);
        return inst;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

  protected:
    axBase*               mBase;
    axDescriptor*         mDescriptor;
    char*                 mPortNames[MAX_LADSPA_PORTS];
    LADSPA_PortDescriptor mPortDesc[MAX_LADSPA_PORTS];
    LADSPA_PortRangeHint  mPortHint[MAX_LADSPA_PORTS];

  protected:

    // temporary
    // while setting up the port hints...

/* Hint LADSPA_HINT_BOUNDED_BELOW indicates that the LowerBound field
   of the LADSPA_PortRangeHint should be considered meaningful. The
   value in this field should be considered the (inclusive) lower
   bound of the valid range. If LADSPA_HINT_SAMPLE_RATE is also
   specified then the value of LowerBound should be multiplied by the
   sample rate. */

/* Hint LADSPA_HINT_BOUNDED_ABOVE indicates that the UpperBound field
   of the LADSPA_PortRangeHint should be considered meaningful. The
   value in this field should be considered the (inclusive) upper
   bound of the valid range. If LADSPA_HINT_SAMPLE_RATE is also
   specified then the value of UpperBound should be multiplied by the
   sample rate. */

/* Hint LADSPA_HINT_TOGGLED indicates that the data item should be
   considered a Boolean toggle. Data less than or equal to zero should
   be considered `off' or `false,' and data above zero should be
   considered `on' or `true.' LADSPA_HINT_TOGGLED may not be used in
   conjunction with any other hint except LADSPA_HINT_DEFAULT_0 or
   LADSPA_HINT_DEFAULT_1. */

/* Hint LADSPA_HINT_SAMPLE_RATE indicates that any bounds specified
   should be interpreted as multiples of the sample rate. For
   instance, a frequency range from 0Hz to the Nyquist frequency (half
   the sample rate) could be requested by this hint in conjunction
   with LowerBound = 0 and UpperBound = 0.5. Hosts that support bounds
   at all must support this hint to retain meaning. */

/* Hint LADSPA_HINT_LOGARITHMIC indicates that it is likely that the
   user will find it more intuitive to view values using a logarithmic
   scale. This is particularly useful for frequencies and gains. */

/* Hint LADSPA_HINT_INTEGER indicates that a user interface would
   probably wish to provide a stepped control taking only integer
   values. Any bounds set should be slightly wider than the actual
   integer range required to avoid floating point rounding errors. For
   instance, the integer set {0,1,2,3} might be described as [-0.1,
   3.1]. */

   //

/* The various LADSPA_HINT_HAS_DEFAULT_* hints indicate a `normal'
   value for the port that is sensible as a default. For instance,
   this value is suitable for use as an initial value in a user
   interface or as a value the host might assign to a control port
   when the user has not provided one. Defaults are encoded using a
   mask so only one default may be specified for a port. Some of the
   hints make use of lower and upper bounds, in which case the
   relevant bound or bounds must be available and
   LADSPA_HINT_SAMPLE_RATE must be applied as usual. The resulting
   default must be rounded if LADSPA_HINT_INTEGER is present. Default
   values were introduced in LADSPA v1.1. */

/* LADSPA_HINT_DEFAULT_NONE indicates that no default is provided. */

/* LADSPA_HINT_DEFAULT_MINIMUM indicates that the suggested lower bound for the
   port should be used. */

/* LADSPA_HINT_DEFAULT_LOW indicates that a low value between the suggested
   lower and upper bounds should be chosen. For ports with
   LADSPA_HINT_LOGARITHMIC, this should be exp(log(lower) * 0.75 +
   log(upper) * 0.25). Otherwise, this should be (lower * 0.75 + upper
   * 0.25). */

/* LADSPA_HINT_DEFAULT_MIDDLE indicates that a middle value between the
   suggested lower and upper bounds should be chosen. For ports with
   LADSPA_HINT_LOGARITHMIC, this should be exp(log(lower) * 0.5 +
   log(upper) * 0.5). Otherwise, this should be (lower * 0.5 + upper *
   0.5). */

/* LADSPA_HINT_DEFAULT_HIGH indicates that a high value between the suggested
   lower and upper bounds should be chosen. For ports with
   LADSPA_HINT_LOGARITHMIC, this should be exp(log(lower) * 0.25 +
   log(upper) * 0.75). Otherwise, this should be (lower * 0.25 + upper
   * 0.75). */

/* LADSPA_HINT_DEFAULT_MAXIMUM indicates that the suggested upper bound for the
   port should be used. */

/* LADSPA_HINT_DEFAULT_0 indicates that the number 0 should be used. Note
   that this default may be used in conjunction with
   LADSPA_HINT_TOGGLED. */

/* LADSPA_HINT_DEFAULT_1 indicates that the number 1 should be used. Note
   that this default may be used in conjunction with
   LADSPA_HINT_TOGGLED. */

/* LADSPA_HINT_DEFAULT_100 indicates that the number 100 should be used. */

/* LADSPA_HINT_DEFAULT_440 indicates that the Hz frequency of `concert A'
   should be used. This will be 440 unless the host uses an unusual
   tuning convention, in which case it may be within a few Hz. */

    virtual void* entrypoint(void* ptr)
      {
        trace("axFormatLadspa.entrypoint");
        mDescriptor = mBase->getDescriptor();
        int i;
        int index = 0;

        // audio inputs

        for (i=0; i<mDescriptor->getNumInputs(); i++)
        {
          mPortNames[index]               = mDescriptor->getInputName(i);
          mPortDesc[index]                = LADSPA_PORT_AUDIO | LADSPA_PORT_INPUT;
          mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
          mPortHint[index].LowerBound     = -1;
          mPortHint[index].UpperBound     =  1;
          index++;
        }

        // audio outputs

        for (i=0; i<mDescriptor->getNumOutputs(); i++)
        {
          mPortNames[index]               = mDescriptor->getOutputName(i);
          mPortDesc[index]                = LADSPA_PORT_AUDIO | LADSPA_PORT_OUTPUT;
          mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_NONE;
          mPortHint[index].LowerBound     = -1;
          mPortHint[index].UpperBound     =  1;
          index++;
        }

        // parameters

        for (i=0; i<mDescriptor->getNumParams();  i++)
        {
          axParamInfo paraminfo = mDescriptor->getParamInfo(i);
          mPortNames[index] = paraminfo.mName;
          mPortDesc[index]  = LADSPA_PORT_CONTROL | LADSPA_PORT_INPUT;

          LADSPA_PortRangeHintDescriptor hint = LADSPA_HINT_DEFAULT_NONE;
          // default value
          float val   = paraminfo.mDef - paraminfo.mMin;
               if (val == paraminfo.mMin ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_MINIMUM;
          else if (val == paraminfo.mMax ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_MAXIMUM;
          else if (val == 0 )   mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_0;
          else if (val == 1 )   mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_1;
          else if (val == 100 ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_100;
          else if (val == 440 ) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_440;
          else
          {
            float sval  = (paraminfo.mDef-paraminfo.mMin) / (paraminfo.mMax-paraminfo.mMin);
            if (sval < 0.25) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_LOW;
            else if (sval > 0.75) mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_HIGH;
            else mPortHint[index].HintDescriptor = LADSPA_HINT_DEFAULT_MIDDLE;
          }

          if (paraminfo.mType==pa_Pow) hint |= LADSPA_HINT_LOGARITHMIC;

          if (paraminfo.mType==pa_Int)
          {
            if (paraminfo.mMin==0 && paraminfo.mMax==1) hint |= LADSPA_HINT_TOGGLED;
            else hint |= LADSPA_HINT_INTEGER;
          }

          mPortHint[index].HintDescriptor = hint | LADSPA_HINT_BOUNDED_BELOW |  LADSPA_HINT_BOUNDED_ABOVE;
          mPortHint[index].LowerBound = paraminfo.mMin;
          mPortHint[index].UpperBound = paraminfo.mMax;

          index++;
        }

        // descriptor

        axMemset(&ladspadescr,0,sizeof(ladspadescr));
        ladspadescr.UniqueID            = mDescriptor->getUniqueId();
        ladspadescr.Label               = mDescriptor->getName();
        ladspadescr.Properties          = LADSPA_PROPERTY_REALTIME | LADSPA_PROPERTY_HARD_RT_CAPABLE;
        ladspadescr.Name                = mDescriptor->getName();
        ladspadescr.Maker               = mDescriptor->getAuthor();
        ladspadescr.Copyright           = mDescriptor->getProduct();
        ladspadescr.PortCount           = mDescriptor->getNumInputs() + mDescriptor->getNumOutputs() + mDescriptor->getNumParams();
        ladspadescr.PortDescriptors     = mPortDesc;
        ladspadescr.PortNames           = mPortNames;
        ladspadescr.PortRangeHints      = mPortHint;
        ladspadescr.ImplementationData  = this;
        ladspadescr.instantiate         = lad_instantiate_callback;
        ladspadescr.connect_port        = lad_connect_port_callback;
        ladspadescr.activate            = lad_activate_callback;
        ladspadescr.run                 = lad_run_callback;
        ladspadescr.run_adding          = NULL; //lad_run_adding_callback;
        ladspadescr.set_run_adding_gain = NULL; //lad_set_run_adding_gain_callback;
        ladspadescr.deactivate          = lad_deactivate_callback;
        ladspadescr.cleanup             = lad_cleanup_callback;
        return (void*)&ladspadescr;
      }

    //--------------------------------------------------
    //
    //--------------------------------------------------

  public:

    axFormatLadspa(axBase* aBase)
      {
        trace("axFormatLadspa.constructor");
        mBase = aBase;
      }

    //----------

    virtual ~axFormatLadspa()
      {
        trace("axFormatLadspa.destructor");
      }

    //----------

    virtual char* getFormatName(void)
      {
        return (char*)"ladspa";
      }

};

//----------

typedef axFormatLadspa AX_FORMAT;

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

#include <stdio.h> // printf

#define AX_ENTRYPOINT(_PL,_IF,_FO,_D,_I)                                      \
                                                                              \
__externc __dllexport                                                         \
const LADSPA_Descriptor* ladspa_descriptor(unsigned long Index)               \
{                                                                             \
  printf("ladspa_descriptor, Index: %lu\n", Index);                           \
  if (Index>0) return NULL;                                                   \
  _AX_DEBUG_SETUP                                                             \
  axBaseImpl<_PL,_IF,_FO,_D,_I>* base = new axBaseImpl<_PL,_IF,_FO,_D,_I>();  \
  gGlobalScope.setBase(base);                                                 \
  _FO* format = (_FO*)base->getFormat();                                      \
  LADSPA_Descriptor* descr = (LADSPA_Descriptor*)format->entrypoint(NULL);    \
  return descr;                                                               \
}

//----------------------------------------------------------------------
#endif
