
#ifndef fx_simpverb_included
#define fx_simpverb_included
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

//#define S3_DEBUG_MEM

#ifdef S3_PLUGIN_VST
  //#define S3_DEBUG_VST
  #define S3_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "s3.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin : public S3_Plugin
{

  private:

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

  //----------------------------------------
  //
  //----------------------------------------

  public:

    // desc: SimpVerb (Simple Reverb)
    //slider1:50<0,100,1>Room Size (%)
    //slider2:80<0,100,1>Damping (%)
    //slider3:-6<-120,0,.1>Dry (dB)
    //slider4:-6<-120,0,.1>Wet (dB)

    //virtual
    void on_create(void) {
      MName       = "fx_simpverb";
      MAuthor     = "michael gruhn";
      MDescription    = MName;
      MUniqueId   = S3_MAGIC + s3_fx_simpverb_id;
      MNumInputs  = 2;
      MNumOutputs = 2;
      appendParameter( new S3_ParamFloat( "Size",     50,  0,   100, 1 ));
      appendParameter( new S3_ParamFloat( "Damping",  80,  0,   100, 1 ));
      appendParameter( new S3_ParamFloat( "Dry",     -6,  -120, 0 ));
      appendParameter( new S3_ParamFloat( "Wet",     -6,  -120, 0 ));
      // .. init variables/members..
      slider1 = 0;
      slider2 = 0;
      slider3 = 0;
      slider4 = 0;
      allpassL1Start  = 0;
      allpassL2Pos    = 0;
      allpassR2Pos    = 0;
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    //void on_stateChange(uint32 AState) {
      /*
      switch(AState) {
        case sps_open: break;
        case sps_close: break;
        case sps_sampleRate: break;
        case sps_blockSize: break;
        case sps_suspend: break;
        case sps_resume: break;
        case sps_start: break;
        case sps_stop: break;
        case sps_bypass: break;
        case sps_bypassOff: break;
      }
      */
    //}

    //virtual
    //void on_transportChange(uint32 ATransport) {
      /*
      if (ATransport & spt_changed) {}  // play, cycle or record state has changed
      if (ATransport & spt_play) {}     // Host sequencer is currently playing
      if (ATransport & spt_cycle) {}    // Host sequencer is in cycle mode
      if (ATransport & spt_record) {}   // Host sequencer is in record mode
      if (ATransport & spt_awrite) {}   // automation write mode active (record parameter changes)
      if (ATransport & spt_aread) {}    // automation read mode active (play parameter changes)
      */
    //}

    /*
    @slider
    roomSize = 10+slider1/100*90 ;
    damp = min(1-slider2/100,0.9);
    ring = 0.5;
    dry = 2^(slider3/6);
    wet = 2^(slider4/6);

    comb1Pos = comb2Pos = comb3Pos = comb4Pos = allpassL1Pos = allpassR1Pos = 0;

    comb1 = 0;
    comb1Length = floor(1 * roomSize * FSampleRate /1000);
    comb2 = comb1Length;
    comb2Length = floor(1.1 * roomSize * FSampleRate /1000);
    comb3 = comb2Length + comb1Length;
    comb3Length = floor(1.3 * roomSize * FSampleRate /1000);
    allpassL1 = comb3Length + comb2Length + comb1Length;
    allpassL1Length = floor(2 * FSampleRate /1000);
    allpassR1 = allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    allpassR1Length = floor(2.5 * FSampleRate /1000);

    comb4 = allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    comb4Length = floor(1.5 * roomSize * FSampleRate /1000);

    allpassL2 = comb4 + allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    allpassL2Length = floor(1 * FSampleRate /1000);
    allpassR2 = allpassL2Length + comb4 + allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
    allpassR2Length = floor(1.5 * FSampleRate /1000);
    */

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      switch(AIndex) {
        case 0: slider1 = AValue; break;
        case 1: slider2 = AValue; break;
        case 2: slider3 = AValue; break;
        case 3: slider4 = AValue; break;
      }

      roomSize          = 10 + slider1 / 100 * 90;
      damp              = S3_Min(1-slider2/100,0.9);
      ring              = 0.5;
      dry               = pow(2,slider3/6);
      wet               = pow(2,slider4/6);
      comb1             = 0;
      comb1Pos          = 0;
      comb1Length       = floor(1 * roomSize * MSampleRate /1000);
      comb2             = comb1Length;
      comb2Pos          = 0;
      comb2Length       = floor(1.1 * roomSize * MSampleRate /1000);
      comb3             = comb2Length + comb1Length;
      comb3Pos          = 0;
      comb3Length       = floor(1.3 * roomSize * MSampleRate /1000);
      allpassL1         = comb3Length + comb2Length + comb1Length;
      allpassL1Pos      = 0;
      allpassL1Length   = floor(2 * MSampleRate /1000);
      allpassR1         = allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
      allpassR1Pos      = 0;
      allpassR1Length   = floor(2.5 * MSampleRate /1000);
      comb4             = allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
      comb4Pos          = 0;
      comb4Length       = floor(1.5 * roomSize * MSampleRate /1000);
      allpassL2         = comb4 + allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
      allpassL2Length   = floor(1 * MSampleRate /1000);
      allpassR2         = allpassL2Length + comb4 + allpassR1Length + allpassL1Length + allpassL1Start + comb3Length + comb2Length + comb1Length;
      allpassR2Length   = floor(1.5 * MSampleRate /1000);
    }

    //virtual
    //void on_midiEvent(int32 AOffset, uint8 AMsg1, uint8 AMsg2, uint8 AMsg3) {
    //}

    //virtual
    //void on_programChange(int32 AIndex) {
    //}

    //virtual
    //void on_preProgram(int32 AIndex) {
    //}

    //virtual
    //void on_postProgram(void) {
    //}

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    /*
    @sample
    reverb = ( spl0 + spl1 )/2;
    spl0 *= dry;
    spl1 *= dry;
    comb1[comb1Pos] = reverb + comb1[comb1Pos] * damp;
    comb2[comb2Pos] = reverb*0.7 + comb2[comb2Pos] * damp;
    comb3[comb3Pos] = reverb*0.5 + comb3[comb3Pos] * damp;
    comb4[comb4Pos] = reverb*0.2 + comb4[comb4Pos] * damp;
    (comb1Pos+=1) >= comb1Length ? comb1Pos = 0;
    (comb2Pos+=1) >= comb2Length ? comb2Pos = 0;
    (comb3Pos+=1) >= comb3Length ? comb3Pos = 0;
    (comb4Pos+=1) >= comb4Length ? comb4Pos = 0;
    reverb = (comb1[comb1Pos]+comb2[comb2Pos]+comb3[comb3Pos]+comb4[comb4Pos]);
    allpassL1[allpassL1Pos] = reverb + allpassL1[allpassL1Pos] * ring;
    left = (reverb - allpassL1[allpassL1Pos] * ring);
    (allpassL1Pos+=1) >= allpassL1Length ? allpassL1Pos = 0;
    allpassL2[allpassL2Pos] = left + allpassL2[allpassL2Pos] * ring;
    spl0 += (left - allpassL2[allpassL2Pos] * ring) * wet;
    (allpassL2Pos+=1) >= allpassL2Length ? allpassL2Pos = 0;
    allpassR1[allpassR1Pos] = reverb + allpassR1[allpassR1Pos] * ring;
    right = (reverb - allpassR1[allpassR1Pos] * ring);
    (allpassR1Pos+=1) >= allpassR1Length ? allpassR1Pos = 0;
    allpassR2[allpassR2Pos] = right + allpassR2[allpassR2Pos] * ring;
    spl1 += (right - allpassR2[allpassR2Pos] * ring) * wet;
    (allpassR2Pos+=1) >= allpassR2Length ? allpassR2Pos = 0;
    */

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (uint32 i=0; i<ANumSamples; i++) {
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

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
