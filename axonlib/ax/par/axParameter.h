/*
 * This file is part of Axonlib.
 *
 * Axonlib is free software: you can redistribute it and/or modify
 * it under the terms of the Axonlib License, either version 1.0
 * of the License, or (at your option) any later version.
 *
 * Axonlib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE_AX for more details.
 *
 * You should have received a copy of the Axonlib License
 * If not, see <http://axonlib.googlecode.com/>.
 */

#ifndef axParameter_included
#define axParameter_included
//----------------------------------------------------------------------

//#include <stdio.h> // sprintf (doGetDisplay)
#include "core/axString.h"

// kVstMaxParamStrLen     8
#define MAX_PARAM_TXTLEN  16

//----------------------------------------------------------------------

//class axParameterInfo
//{
//  public:
//    virtual char* getName(int aIndex)  { return (char*)"param"; }
//    virtual float* getDefaultValue(void)    { return 0; }
//};
//
//typedef axArray<axParameterInfo*> axParameterInfos;

//----------------------------------------------------------------------

class axParameter;

class axParameterListener
{
  public:
    virtual void onChange(axParameter* aParameter) {}
};

//----------------------------------------------------------------------

// parameter flags
#define pf_Automate 1 // used in axFormatVst, effCanBeAutomated
#define AX_PAR_DEFAULT (pf_Automate)

//----------------------------------------------------------------------
//
// parameter
//
//----------------------------------------------------------------------

class axParameter// : public axParameterBase
{
  friend class axWindow;
  friend class axFormat;

  private:
    int mConnection;      // which widget (if any) this is connected to
    int mIndex;           // index into the parameter list (axFormatBase)

  protected:
    axParameterListener* mListener;
    axString  mName;              // move to axParameterInfo
    axString  mLabel;             // move to axParameterInfo
    float     mValue;   // 0..1
    float     mDefault;           // move to axParameterInfo
    int       mFlags;
  public:
    int       mId;
    void*     mPtr;

  public:

    // aListener: parent/owner, usually your plugin, can be editor, etc..
    // aName:     name, displayed in vst-host parameter lists, automation, etc
    //            and on widgets
    // aLabel:    extra string, like "db", "%", etc, to append after the
    //            normal display string (for vst hosts)
    // aValue:    internal value (0..1)

    // some of these must be moved to the descriptor, and the parameterinfo, because we need it
    // before the plugin is instanciated, and so it can't be a part of the plugin itself.
    // we set up some axParameterInfo's in the descriptor, and the appendParameter() can read
    // from it if it needs some info...

    axParameter(axParameterListener* aListener, axString aName, axString aLabel="", float aValue=0)
      {
        mListener   = aListener;
        mName       = aName;
        mLabel      = aLabel;
        mValue      = aValue;//0;
        mDefault    = mValue;
        mConnection = -1;
        mFlags      = AX_PAR_DEFAULT;
        mIndex      = -1;
        mId         = 0;      //aId;
        mPtr        = NULL;   //aPtr;
      }

    virtual ~axParameter() {}

    //--------------------------------------------------
    // inline
    //--------------------------------------------------

    inline int getFlags(void)                       { return mFlags; }
    inline void setListener(axParameterListener* l) { mListener = l; }
    inline axParameterListener* getListener(void)   { return mListener; }
    inline void setValueDirect(float v)             { mValue = v; }
    inline float getValueDirect(void)               { return mValue; }
    inline int  getConnection(void)                 { return mConnection; }
    inline void setConnection(int aNum)             { mConnection = aNum; }
    inline int  getIndex(void)                      { return mIndex; }
    inline void setIndex(int aNum)                  { mIndex = aNum; }

    inline axString getName(void) { return mName; }

    //----------

    virtual void  setValue(float aValue) { mValue=aValue; }
    virtual float getValue(void)         { return mValue; }

//    /*virtual*/inline float getValue2(void)  { float n=getValue(); return n*n; }
//    /*virtual*/inline float getValue3(void)  { float n=getValue(); return n*n*n; }
//    /*virtual*/inline float getValue3i(void) { float n=getValue(); return (n>0) ? 1.0f/(n*n*n) : 0; }

    //----------

    virtual void doReset(void)
      {
        mValue = mDefault;
        if (mListener) mListener->onChange(this);
      }

    //----------

    virtual void doSetValue(float aValue, bool aNotify/*=true*/)
      {
        //trace("doSetValue (aNotify=" << (aNotify?"true":"false") << ")" );
        mValue = aValue;
        //trace("mListener = " << mListener);
        if (aNotify) if (mListener) mListener->onChange(this);
      }

    //----------

    virtual float doGetValue(void)
      {
        return mValue;
      }

    //----------

    virtual void doGetName(char* buf)
      {
        axStrcpy(buf,mName.ptr());
      }

    //----------

    virtual void doGetLabel(char* buf)
      {
        axStrcpy(buf,mLabel.ptr());
      }

    //----------

    virtual void doGetDisplay(char* buf)
      {
        float val = getValue();
        //__builtin_sprintf(buf,"%.3f",val);
        axFtoa(buf,val);
      }

};

typedef axArray<axParameter*> axParameters;

//----------------------------------------------------------------------
#endif

