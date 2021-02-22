#ifndef fx_tempo_delay_included
#define fx_tempo_delay_included
//----------------------------------------------------------------------

#define S3_DEBUG_MEM

#ifdef S3_PLUGIN_VST
  #define S3_DEBUG_SOCKET
  //#define S3_DEBUG_VST
#endif

//----------

#include "s3.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

/*
  buffersize = beats * ( (60/tempo) * samplerate )
  tempo = 30, samplerate = 192k, beats = 4
    4 * ( (60/30) * 192000 )
  = 4 * (  2      * 192k )
  = 4 * 384
  = 1536k * stereo (2)
  = 3072k
  = 3mb
*/

#define MAX_DELAY_LENGTH (1024*1024*4)

//----------------------------------------------------------------------

class myPlugin : public S3_Plugin {

  private:

    float   MBeats;
    float   MFeedback;
    float   MWet;
    float   MDry;
    float   MBuffer[MAX_DELAY_LENGTH*2];
    uint32  MBufferPos;
    uint32  MBufferSize;

  public:

    //virtual
    void on_create(void) final {
      MName         = "fx_tempo_delay";
      MAuthor       = "tor.helge.skei";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_fx_tempo_delay_id;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      MVersion      = 0;
      MFlags       |= s3_pf_per_sample;
      appendParameter( new S3_ParamFloat("beats",    0.75, (1.0f/16.0f), 4, (1.0f/16.0f) ));
      appendParameter( new S3_ParamFloat("feedback", 0.75 ));
      appendParameter( new S3_ParamFloat("dry",      1 ));
      appendParameter( new S3_ParamFloat("wet",      0.25 ));
      MBeats      = 0;
      MFeedback   = 0;
      MWet        = 0;
      MDry        = 1;
      MBufferPos  = 0;
      MBufferSize = 0;
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      switch (AIndex) {
        case 0: MBeats    = AValue; break;
        case 1: MFeedback = AValue; break;
        case 2: MDry      = AValue; break;
        case 3: MWet      = AValue; break;
      }
    }

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float beatsize;
      //float samplerate;
      //float samplepos;
      //float tempo;
      //float beatpos;
      //FHost.getTime(samplerate{%H-},samplepos{%H-},tempo{%H-},beatpos{%H-});
      updateHostTime();
      beatsize = ( 60.0 / MTempo ) * MSampleRate;
      MBufferSize = S3_Trunc( MBeats * beatsize );
      if (MBufferPos > 0) {
        while (MBufferPos >= MBufferSize) MBufferPos -= MBufferSize;
      }
    }

    //virtual
    void on_processSample(S3_Sample** AInputs, S3_Sample** AOutputs) final {
      uint32 pos2 = MBufferPos * 2;
      MBufferPos += 1;
      if (MBufferPos >= MBufferSize) MBufferPos = 0;
      float in0  = *AInputs[0];
      float in1  = *AInputs[1];
      float buf0 = MBuffer[pos2];
      float buf1 = MBuffer[pos2+1];
      MBuffer[pos2] = in0 + buf0 * MFeedback;
      MBuffer[pos2+1] = in1 + buf1 * MFeedback;
      *AOutputs[0] = in0*MDry + buf0*MWet;
      *AOutputs[1] = in1*MDry + buf1*MWet;
    }

};

//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
