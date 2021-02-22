
#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

/*
  based on:
    BBE Sonic Maximizer modelisation, (C) 20011, Dominique Guichaoua.
    http://forum.cockos.com/showthread.php?t=91439
    license: ?
    from the forum:
    skei (me) "i converted both the sonic maximizer (just the first version,
               yet..) and the sonic enhancer to vst, available here [...] i
               haven't studied the licensing agreements, so if you don't want
               me to port your plugins, let me know, and i'll remove it, no
               problem.."
    domino49: "great!!"
    so i think it should be ok?
*/

//#define cDenorm   10e-30
#define c_ampdB   8.65617025
#define freqHP    2243.2
#define freqLP    224.32
#define freqHP_p2 -KODE_PI2 * freqHP
#define freqLP_p2 -KODE_PI2 * freqLP


//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    
    //KODE_TRACE;
    
    MName       = "fx_sonic_maximizer";
    MAuthor     = "skei.audio";
    MNumInputs  = 2;
    MNumOutputs = 2;
    
    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );
    
      appendParameter( KODE_New KODE_FloatParameter("Low Cont",  1,  0,  10, 0.1 ) );
      appendParameter( KODE_New KODE_FloatParameter("Process",   1,  0,  10, 0.1 ) );
      appendParameter( KODE_New KODE_FloatParameter("Outout",    1, -30, 0,  0.1 ) );

  }
  
  //----------
  
  virtual ~myDescriptor() {
    //KODE_TRACE;
  }
  
};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {
  
private:

  //float MLeft   = 0.0f;
  //float MRight  = 0.0f;
  
    float slider1,slider2,slider3;
    float band1,band2,band3,amp;
    float tmplLP,tmprLP;
    float tmplHP,tmprHP;
    float xLP,xHP;
    float a0LP,a0HP;
    float b1LP,b1HP;
    
    bool need_recalc = true;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
      slider1 = 0;
      slider2 = 0;
      slider3 = 0;
      band1   = 0;
      band2   = 1;
      band3   = 0;
      tmplLP  = 0;
      tmprLP  = 0;
      tmplHP  = 0;
      tmprHP  = 0;
  }
  
  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

  void recalc(float srate) {
    xHP  = expf(freqHP_p2/srate);
    a0HP = 1.0 - xHP;
    b1HP = -xHP;
    xLP  = expf(freqLP_p2/srate);
    a0LP = 1.0 - xLP;
    b1LP = -xLP;
  }

public:

  //void on_open()        final { KODE_TRACE; }
  //void on_close()       final { KODE_TRACE; }
  //void on_initialize()  final { KODE_TRACE; }
  //void on_terminate()   final { KODE_TRACE; }
  //void on_activate()    final { KODE_TRACE; }
  //void on_deactivate()  final { KODE_TRACE; }
  //void on_startPreset() final { KODE_TRACE; }
  //void on_endPreset()   final { KODE_TRACE; }
  
  void on_parameterChange(uint32_t AIndex, float AValue) final {
      switch (AIndex) {
        case 0:
          slider1 = AValue;
          band1 = expf( (slider1+3) / c_ampdB );
          break;
        case 1:
          slider2 = AValue;
          band3 = expf( (slider2+3) / c_ampdB );
          break;
        case 2:
          slider3 = AValue;
          amp = expf( slider3 / c_ampdB );
          break;
      }
      //band2 := 1; // exp(0/c_ampdB);
  }
    
  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
  
  void on_process(KODE_ProcessContext* AContext) final {
    
    if (need_recalc) {
      need_recalc = false;
      recalc(AContext->samplerate);
    }
    
    uint32_t len = AContext->numSamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;
      
        float s0 = *input0++;
        float s1 = *input1++;
        tmplLP = a0LP*s0 - b1LP*tmplLP;// + cDenorm;
        tmprLP = a0LP*s1 - b1LP*tmprLP;// + cDenorm;
        float sp0 = tmplLP;
        float sp1 = tmprLP;
        tmplHP = a0HP*s0 - b1HP*tmplHP;// + cDenorm;
        tmprHP = a0HP*s1 - b1HP*tmprHP;// + cDenorm;
        float sp4 = s0 - tmplHP;
        float sp5 = s1 - tmprHP;
        float sp2 = s0 - sp0 - sp4;
        float sp3 = s1 - sp1 - sp5;
        *output0++ = (sp0 * band1 + sp2 /* * band2 */ + sp4 * band3) * amp;
        *output1++ = (sp1 * band1 + sp3 /* * band2 */ + sp5 * band3) * amp;
      
      
    }
  }
  
  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}
  
};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)

