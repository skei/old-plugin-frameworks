#ifndef s2_graph_included
#define s2_graph_included
//----------------------------------------------------------------------

///*
//  a graph is a module that can contain sub-modules
//  todo: connect.. pass on pointers
//
//  argh.. confusing with the connections and outputs, etc..
//
//*/
//
////----------------------------------------------------------------------
//
//#include "audio/s2_module.h"
//
////----------------------------------------------------------------------
//
//class SGraph : public SModule {
//
//  private:
//
//    SModuleArray  MModules;
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//    SGraph() : SModule() {
//    }
//
//    //----------
//
//    virtual ~SGraph() {
//      #ifndef S2_NO_AUTODELETE
//      deleteModules();
//      #endif
//    }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//    void appendModule(SModule* AModule) {
//      MModules.append(AModule);
//    }
//
//    //----------
//
//    void deleteModules(void) {
//      for (uint32 i=0; i<MModules.size(); i++) {
//        delete MModules[i];
//      }
//    }
//
//  //----------------------------------------
//  //
//  //----------------------------------------
//
//  public:
//
//    //virtual /*SModule*/
//    //void processNoteOn(float ANote, float AVelocity, uint32 AChannel=0) {
//    //}
//
//    //virtual /*SModule*/
//    //void processNoteOff(float ANote, float AVelocity, uint32 AChannel=0) {
//    //}
//
//    //virtual /*SModule*/
//    //void processControl(uint32 AIndex, float AValue, uint32 AChannel=0) {
//    //}
//
//    //virtual /*SModule*/
//    //void processPitchbend(float ABend, uint32 AChannel=0) {
//    //}
//
//    //virtual /*SModule*/
//    //void processParameter(uint32 AIndex, float AValue, uint32 AChannel=0) {
//    //}
//
//    //----------
//
//    //virtual /*SModule*/
//    void processBlock(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
//      for (uint32 i=0; i<MModules.size(); i++) {
//        SAssert ( MModules[i] != S2_NULL );
//        MModules[i]->processBlock(AInputs,AOutputs,ALength);
//      }
//      // self (operators/ticks)
//      //SModule::processBlock(AInputs,AOutputs,ALength);
//    }
//
//    //----------
//
//    //virtual /*SModule*/
//    void processTicks(SSample** AInputs, SSample** AOutputs, uint32 ALength) {
//      for (uint32 i=0; i<MModules.size(); i++) {
//        SAssert ( MModules[i] != S2_NULL );
//        MModules[i]->processTicks(AInputs,AOutputs,ALength);
//      }
//      // self (operators/ticks)
//      //SModule::processTicks(AInputs,AOutputs,ALength);
//    }
//
//};

//----------------------------------------------------------------------
#endif
