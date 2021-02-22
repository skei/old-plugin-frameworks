#ifndef fx_grains_included
#define fx_grains_included
//----------------------------------------------------------------------

/*

*/

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//----- plugins -----

//#define S3_PLUGIN_PER_SAMPLE
//#define S3_PLUGIN_RECEIVE_MIDI


//----- format -----

//#define S3_DEBUG_MEM

#ifdef S3_PLUGIN_VST
  //#define S3_DEBUG_VST
  #define S3_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "s3.h"
#include "common/s3_random.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

#define S3_MAX_GRAINS 1024
#define S3_BUFFERSIZE (1024*1024)

struct S3_Grain {
  bool  on;
  float pos;
  float sta;
  float end;
  float dur;
  float spd;
  float ph;
  float pa;
  float dh;
  float da;
};

const char* freeze_txt[2] = { "off", "on" };

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

class myPlugin
: public S3_Plugin
{

  private:

    S3_Grain  MGrains[S3_MAX_GRAINS];
    float     MBuffer[S3_BUFFERSIZE];

    float     master;
    int32     numgrains;
    int32     buffersize;
    bool      freeze;
    float     graindist;
    float     grainsize;
    float     graindur;
    float     grainpitch;
    float     grainenv;
    float     startjit;
    float     pitchjit;
    float     sizejit;
    float     durjit;
    int32     index;
    float     countdown;

  public:

    //------------------------------------------------------------
    //
    //------------------------------------------------------------

    //virtual
    void on_create(void) {
      MName       = "fx_grains";
      MAuthor     = "tor.helge.skei";
      MDescription    = MName;
      MUniqueId   = S3_MAGIC + s3_fx_grains_id;
      MNumInputs  = 2;
      MNumOutputs = 2;
      MFlags |= s3_pf_per_sample;
      //
      master      = 0.0f;
      numgrains   = 0;
      buffersize  = 0;
      freeze      = false;
      graindist   = 0.0f;
      grainsize   = 0.0f;
      graindur    = 0.0f;
      grainpitch  = 0.0f;
      grainenv    = 0.0f;
      startjit    = 0.0f;
      pitchjit    = 0.0f;
      sizejit     = 0.0f;
      durjit      = 0.0f;
      //
      appendParameter( new S3_ParamFloat( "master (db)",    -6,   -60, 6     ));
      appendParameter( new S3_ParamInt(   "num grains",      10,   1,  100   ));
      appendParameter( new S3_ParamFloat( "buf size (ms)",   1000, 1,  1000  ));
      appendParameter( new S3_ParamText(  "freeze",          0,    2,  freeze_txt )); // text
      appendParameter( new S3_ParamFloat( "grain dist (ms)", 20,   0,  100   ));
      appendParameter( new S3_ParamFloat( "grain size (ms)", 30,   1,  100   ));
      appendParameter( new S3_ParamFloat( "grain dur (ms)",  300,  1,  1000  ));
      appendParameter( new S3_ParamFloat( "grain pitch",     1,    0,  10    ));
      appendParameter( new S3_ParamFloat( "grain env",       0               ));
      appendParameter( new S3_ParamFloat( "dist jitter",     0.2             ));
      appendParameter( new S3_ParamFloat( "pitch jitter",    0.2             ));
      appendParameter( new S3_ParamFloat( "size jitter",     0.2             ));
      appendParameter( new S3_ParamFloat( "dur jitter",      0.2             ));
      //
      index = 0;
      countdown = 0;

    }

    //----------

    //virtual
    void on_destroy(void) {
    }

    //------------------------------------------------------------
    // events
    //------------------------------------------------------------

    void on_parameterChange(int32 AIndex, float AValue) final {
      switch(AIndex) {
        case 0:  master      = pow(2,(AValue/6)); break;
        case 1:  numgrains   = trunc( AValue ); break;
        case 2:  buffersize  = trunc( (AValue/1000) * MSampleRate ); break;
        case 3:  AValue > 0.5 ? freeze=true : freeze=false; break;
        case 4:  graindist   = (AValue/1000) * MSampleRate; break;
        case 5:  grainsize   = (AValue/1000) * MSampleRate; break;
        case 6:  graindur    = (AValue/1000) * MSampleRate; break;
        case 7:  grainpitch  = AValue; break;
        case 8:  grainenv    = AValue; break;
        case 9:  startjit    = AValue; break;
        case 10: pitchjit    = AValue; break;
        case 11: sizejit     = AValue; break;
        case 12: durjit      = AValue; break;
      }
    }

    //------------------------------------------------------------
    // audio
    //------------------------------------------------------------

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      S3_Memcpy(AOutputs[0],AInputs[0],ANumSamples*sizeof(S3_Sample));
      S3_Memcpy(AOutputs[1],AInputs[1],ANumSamples*sizeof(S3_Sample));
    }

    //virtual
    void on_processSample(S3_Sample** AInputs, S3_Sample** AOutputs) {

      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];

      float in0 = spl0;
      float in1 = spl1;
      float out0 = 0;
      float out1 = 0;
      int32 newgrain = -1;

      //if (numgrains > 0) {
        for (int32 i=0; i<numgrains; i++) {
          if (MGrains[i].on) {
            MGrains[i].pos += MGrains[i].spd;
            if (MGrains[i].pos >= MGrains[i].end) MGrains[i].pos = MGrains[i].sta;
            if (MGrains[i].pos >= buffersize) MGrains[i].pos -= buffersize;
            MGrains[i].ph += (MGrains[i].pa*2);
            if (MGrains[i].ph >= 2) MGrains[i].ph -= 2;
            MGrains[i].dh += (MGrains[i].da*2);
            if (MGrains[i].dh >=2) MGrains[i].dh -= 2;
            MGrains[i].dur -= 1;
            if (MGrains[i].dur <= 0) MGrains[i].on = false;
            float gvol = MGrains[i].ph * (2-abs(MGrains[i].ph)); // abs-neg ?
            float dvol = MGrains[i].dh * (2-abs(MGrains[i].dh));
            int32 temp = trunc( MGrains[i].pos );
            temp *= 2;
            out0 += MBuffer[ temp ] * dvol * gvol;
            out1 += MBuffer[ temp+1 ] * dvol * gvol;
          }
          else newgrain = i;
        }
      //}

      if (countdown <= 0) {
        countdown = graindist;
        if (newgrain > 0) {
          float startrnd = 1 + (startjit * S3_RandomSigned());
          float pitchrnd = 1 + (pitchjit * S3_RandomSigned());
          float sizernd  = 1 + (sizejit  * S3_RandomSigned());
          float durrnd   = 1 + (durjit   * S3_RandomSigned());
          float siz = (grainsize*sizernd);
          float st = index * startrnd;
          if (st >= buffersize) st -= buffersize;
          if (st <  0) st += buffersize;
          float en = st + siz;
          if (en >= buffersize) en = buffersize; // clamp
          if (en <  0) en = 0;
          float du = graindur*durrnd;
          MGrains[newgrain].on   = true;
          MGrains[newgrain].pos  = st;
          MGrains[newgrain].sta  = st;
          MGrains[newgrain].end  = en;
          MGrains[newgrain].dur  = du;
          MGrains[newgrain].spd  = grainpitch * pitchrnd;
          MGrains[newgrain].ph   = 0;
          MGrains[newgrain].pa   = 1 / siz;
          MGrains[newgrain].dh   = 0;
          MGrains[newgrain].da   = 1 / du;
        }
      }
      countdown -= 1;

      if (!freeze) {
        MBuffer[index*2] = in0;
        MBuffer[index*2+1] = in1;
      }

      index += 1;
      if (index >= buffersize) index -= buffersize;

      spl0 = out0 * master;
      spl1 = out1 * master;

      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

    //virtual
    void on_postProcess(void) {
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

