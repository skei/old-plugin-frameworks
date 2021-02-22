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

#ifndef axGraph_included
#define axGraph_included
//----------------------------------------------------------------------
/*
  graph = collection of modules
  called once per sample
  your plugin (or a sample accurate event scheduler) takes care of
  sample/event timing
*/
//----------------------------------------------------------------------

#include "core/axDefines.h"
#include "audio/axModule.h"

class axGraph : public axModule,
                public axModuleListener
{
  protected:
    axModules mModules;
    axModules mExecList;
    bool      mCompiled;
  public:

    axGraph(axString aName)
    : axModule(NULL,aName)
      {
        mCompiled = false;
      }

    virtual ~axGraph()
      {
        #ifndef AX_NOAUTODELETE
        deleteModules();
        #endif
      }

    //----------

    virtual void appendModule(axModule* aModule) { mModules.append(aModule); }
    virtual void deleteModules(void) { for (int i=0; i<mModules.size(); i++) delete mModules[i]; }

    //----------------------------------------

//    //TODO: juggle input/output pointers
//    virtual int connectPin(axPin* mDstPin, axPin* aSrcPin, axModule* aSrcMod)
//      {
//        return 0;
//      }

//    //TODO: here too
//    virtual int connectDirect(axPin* mDstPin, SPL* aSource)
//      {
//        return 0;
//      }

//    virtual void doSignal(int aType=0, PTR aValue=NULL)
//      {
//        // parameters?
//        // midi?
//        // system messages? (start/stop/activate/..)
//      }

    //----------------------------------------

    // axModule[Base]

    virtual void doProcess(SPL** aInputs, SPL** aOutputs)
      {
        for (int i=0; i<mModules.size(); i++)
        {
          //mModules[i]->doProcess(aInputs,aOutputs);
          axModule* moo = mModules[i];
          if (moo->isActive()) moo->doProcess(aInputs,aOutputs);
        }
      }

    //----------

    // TODO:
    //    make execlist based on which modules' output are needed first,
    //    and the pin type/rate
    // idea:
    //    a primitive version of this is quite simple:
    //    start with a mdule, append it onto the 'todo' queue/list,
    //    then: repeatedly, until todo list is empty:
    //    - get (and remove) module from todo queue
    //    - append the modules that are connected to its input pins to the todo queue,
    //    - append module to execlist (and remove from todo)
    //      except if it's already in the list, if so, delete old one, append new (to the end)
    //    then, postprocess the execlist by reversing it,
    //    or by executing it 'in reverse'.

    // naive compile: if module is active, put it into execlist
    virtual int doCompile(void)
      {
        return 0;
        mExecList.clear();
        for (int i=0; i<mModules.size(); i++)
        {
          if (mModules[i]->isActive()) mExecList.append(mModules[i]);
        }
        mCompiled = true;
      }

    //----------


    //virtual int doExecute(SPL** aInputs, SPL** aOutputs)
    virtual /*int*/void doExecute(/*SPL** aInputs, SPL** aOutputs*/)
      {
        if (mCompiled)
        {
          for (int i=0; i<mExecList.size(); i++)
          {
            mExecList[i]->doExecute();//aInputs,aOutputs);
          }
          //return 0;
        }
        //else return -1;
      }

};

//----------------------------------------------------------------------
#endif
