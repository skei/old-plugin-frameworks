//----------------------------------------------------------------------

// based on the 'transientkiller' jesusonic effect by 'loser'

/*

(C) 2006, Michael Gruhn.

NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
ENTRUST SOMEBODY ELSE WITH DOING SO.

*/
//----------------------------------------------------------------------


//#define KODE_DEBUG
//#define KODE_DEBUG_VST2

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

#define KODE_NO_GUI

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    
    KODE_TRACE;
    MName = "fx_transientkiller";
    MAuthor = "skei.audio";
    MNumInputs = 2;
    MNumOutputs = 2;
    
    /*
    slider1:0<-12,1,.1>Threshold (dB)
    slider2:1<1,50,.1>Ratio
    */
    
    appendParameter( KODE_New KODE_FloatParameter("threshold",  0.0f, -12.0f,  1.0f) );
    appendParameter( KODE_New KODE_FloatParameter("ratio",      1.0f,   1.0f, 50.0f) );
  }
  
  //----------
  
  virtual ~myDescriptor() {
    KODE_TRACE;
  }
  
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
  
private:

  /*
  @init
  cAmpDB_ = 8.65617025;
  */
  
  float cAmpDB_ = 8.65617025;

  float treshDB = 0.0f;
  float ratio   = 0.0f;
  float vol     = 0.0f;
  
  float db0 = 0.0f;
  float db1 = 0.0f;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    KODE_TRACE;
  }
  
  virtual ~myInstance() {
    KODE_TRACE;
  }
  
public:

  //void on_open()        { KODE_TRACE; }
  //void on_close()       { KODE_TRACE; }
  //void on_initialize()  { KODE_TRACE; }
  //void on_terminate()   { KODE_TRACE; }
  //void on_activate()    { KODE_TRACE; }
  //void on_deactivate()  { KODE_TRACE; }
  //void on_startPreset() { KODE_TRACE; }
  //void on_endPreset()   { KODE_TRACE; }
  
  /*
  @slider
  treshDB = slider1;
  ratio = slider2;
  vol = 1/exp( (treshDB -treshDB/ratio) / cAmpDB_);
  */
  
  void on_parameterChange(uint32_t AIndex, float AValue) {
    //KODE_Trace("%i %.3f\n",AIndex,AValue);
    switch(AIndex) {
      case 0: treshDB = AValue; break;
      case 1: ratio   = AValue; break;
    }
    vol = 1/exp( (treshDB -treshDB/ratio) / cAmpDB_);
  }
    
  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
  
  /*
  @sample
  (db0 = log(abs(spl0))*cAmpDB_) > treshDB ? spl0 = exp( (treshDB + (db0 - treshDB)/ratio) / cAmpDB_)* sign(spl0);
  (db1 = log(abs(spl1))*cAmpDB_) > treshDB ? spl1 = exp( (treshDB + (db1 - treshDB)/ratio) / cAmpDB_)* sign(spl1);
  spl0 *= vol;
  spl1 *= vol;
  */
  
  void on_process(KODE_ProcessContext* AContext) {
    uint32_t len = AContext->numSamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      float spl0 = *in0++;
      float spl1 = *in1++;
      db0 = log(abs(spl0)) * cAmpDB_;
      if (db0 > treshDB) spl0 = exp( (treshDB + (db0 - treshDB)/ratio) / cAmpDB_) * KODE_Sign(spl0);
      db1 = log(abs(spl1)) * cAmpDB_;
      if (db1 > treshDB) spl1 = exp( (treshDB + (db1 - treshDB)/ratio) / cAmpDB_) * KODE_Sign(spl1);
      *out0++ = spl0 * vol;
      *out1++ = spl1 * vol;
    }
  }
  
  //KODE_Editor* on_openEditor(void* AParent) { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) {}
  //void on_updateEditor(KODE_Editor* AEditor) {}
  
};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_MAIN(myDescriptor,myInstance)
