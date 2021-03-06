
#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

/*
  based on the jesusonic plugin 'simpverb' (?)
  (C) 2007, Michael Gruhn.

  NO WARRANTY IS GRANTED.  THIS PLUG-IN  IS PROVIDED FOR FREE  ON  AN  "AS  IS"
  BASIS, WITHOUT WARRANTY OF ANY KIND.  NO LIABILITY IS GRANTED, INCLUDING, BUT
  NOT LIMITED TO, ANY DIRECT OR INDIRECT,  SPECIAL, INCIDENTAL OR CONSEQUENTIAL
  DAMAGE ARISING OUT OF THE USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER
  FAILTURE  OF  MALFUNCTION  INCLUDED.  THE  USE  OF THE  SOURCE  CODE,  EITHER
  PARTIALLY OR IN TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S
  INTENTION,  AND USED WITH ACKNOWLEDGEMENT OF THE AUTHOR.  LAST BUT NOT LEAST,
  BY USING THIS  PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR,  AS WELL
  AS YOUR CLAIM TO ENTRUST SOMEBODY ELSE WITH DOING SO.
*/

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {
    
    //KODE_TRACE;
    
    MName       = "fx_simpverb";
    MAuthor     = "skei.audio";
    MNumInputs  = 2;
    MNumOutputs = 2;
    
    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );
    
      appendParameter( KODE_New KODE_FloatParameter( "Size",     50,  0,   100, 1 ));
      appendParameter( KODE_New KODE_FloatParameter( "Damping",  80,  0,   100, 1 ));
      appendParameter( KODE_New KODE_FloatParameter( "Dry",     -6,  -120, 0 ));
      appendParameter( KODE_New KODE_FloatParameter( "Wet",     -6,  -120, 0 ));
    
    
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
  
    float BUFFER[1024*1024];
    float slider1, slider2, slider3, slider4;
    float roomSize, damp, ring, dry, wet;
    int32 comb1, comb1Pos, comb1Length;
    int32 comb2, comb2Pos, comb2Length;
    int32 comb3, comb3Pos, comb3Length;
    int32 comb4, comb4Pos, comb4Length;
    int32 allpassL1, allpassL1Pos, allpassL1Length;
    int32 allpassR1, allpassR1Pos, allpassR1Length;
    int32 allpassL2, allpassL2Length;
    int32 allpassR2, allpassR2Length;
    int32 allpassL1Start;
    int32 allpassL2Pos, allpassR2Pos;
    
    bool need_recalc = true;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
    slider1         = 0;
    slider2         = 0;
    slider3         = 0;
    slider4         = 0;
    allpassL1Start  = 0;
    allpassL2Pos    = 0;
    allpassR2Pos    = 0;
  }
  
  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

  void recalc(float srate) {
    roomSize          = 10 + slider1 / 100 * 90;
    damp              = KODE_Min(1-slider2/100,0.9);
    ring              = 0.5;
    dry               = pow(2,slider3/6);
    wet               = pow(2,slider4/6);
    comb1             = 0;
    comb1Pos          = 0;
    comb1Length       = floor(1 * roomSize * srate /1000);
    comb2             = comb1Length;
    comb2Pos          = 0;
    comb2Length       = floor(1.1 * roomSize * srate /1000);
    comb3             = comb2Length + comb1Length;
    comb3Pos          = 0;
    comb3Length       = floor(1.3 * roomSize * srate /1000);
    allpassL1         = comb3Length + comb2Length + comb1Length;
    allpassL1Pos      = 0;
    allpassL1Length   = floor(2 * srate /1000);
    allpassR1         = allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    allpassR1Pos      = 0;
    allpassR1Length   = floor(2.5 * srate /1000);
    comb4             = allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    comb4Pos          = 0;
    comb4Length       = floor(1.5 * roomSize * srate /1000);
    allpassL2         = comb4 + allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    allpassL2Length   = floor(1 * srate /1000);
    allpassR2         = allpassL2Length + comb4 + allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    allpassR2Length   = floor(1.5 * srate /1000);
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
    switch(AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
      case 3: slider4 = AValue; break;
    }
    need_recalc = true;
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
      
        float spl0 = *input0++;
        float spl1 = *input1++;

        float reverb = ( spl0 + spl1 ) / 2;
        spl0 *= dry;
        spl1 *= dry;

        BUFFER[comb1+comb1Pos] = reverb      + BUFFER[comb1+comb1Pos] * damp;
        BUFFER[comb2+comb2Pos] = reverb*0.7  + BUFFER[comb2+comb2Pos] * damp;
        BUFFER[comb3+comb3Pos] = reverb*0.5  + BUFFER[comb3+comb3Pos] * damp;
        BUFFER[comb4+comb4Pos] = reverb*0.2  + BUFFER[comb4+comb4Pos] * damp;
        comb1Pos += 1;
        comb2Pos += 1;
        comb3Pos += 1;
        comb4Pos += 1;
        if (comb1Pos >= comb1Length) comb1Pos = 0;
        if (comb2Pos >= comb2Length) comb2Pos = 0;
        if (comb3Pos >= comb3Length) comb3Pos = 0;
        if (comb4Pos >= comb4Length) comb4Pos = 0;

        reverb = (BUFFER[comb1+comb1Pos]+BUFFER[comb2+comb2Pos]+BUFFER[comb3+comb3Pos]+BUFFER[comb4+comb4Pos]);

        BUFFER[allpassL1+allpassL1Pos] = reverb + BUFFER[allpassL1+allpassL1Pos] * ring;
        float left = (reverb - BUFFER[allpassL1+allpassL1Pos] * ring);
        allpassL1Pos += 1;
        if (allpassL1Pos >= allpassL1Length) allpassL1Pos = 0;

        BUFFER[allpassL2+allpassL2Pos] = left + BUFFER[allpassL2+allpassL2Pos] * ring;
        spl0 += (left - BUFFER[allpassL2+allpassL2Pos] * ring) * wet;
        allpassL2Pos += 1;
        if (allpassL2Pos >= allpassL2Length) allpassL2Pos = 0;

        BUFFER[allpassR1+allpassR1Pos] = reverb + BUFFER[allpassR1+allpassR1Pos] * ring;
        float right = (reverb - BUFFER[allpassR1+allpassR1Pos] * ring);
        allpassR1Pos += 1;
        if (allpassR1Pos >= allpassR1Length) allpassR1Pos = 0;

        BUFFER[allpassR2+allpassR2Pos] = right + BUFFER[allpassR2+allpassR2Pos] * ring;
        spl1 += (right - BUFFER[allpassR2+allpassR2Pos] * ring) * wet;
        allpassR2Pos += 1;
        if (allpassR2Pos >= allpassR2Length) allpassR2Pos = 0;

        *output0++ = spl0;
        *output1++ = spl1;
      
      
    }
  }
  
  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}
  
};

//----------------------------------------------------------------------

KODE_MAIN(myDescriptor,myInstance)
