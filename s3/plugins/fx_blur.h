#ifndef fx_blur_included
#define fx_blur_included
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

const char* str_onoff[] = { (char*)"off", (char*)"on" };

//----------------------------------------------------------------------

class myPlugin : public S3_Plugin {

  private:
    float*  BUF;
    float*  BUF_t;
    int     pos;
    int     bufsize;
    int     bufsize_t;
  public:
    float   decay, decay_t;
    float   decay2;
    float   vol;
    float   bs,d,d2,v,fr;

  public:

    //virtual
    void on_create(void) final {
      MName         = "fx_blur";
      MAuthor       = "tor.helge.skei";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_fx_blur_id;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      MVersion      = 0;
      MFlags       |= s3_pf_per_sample;
      appendParameter( new S3_ParamFloat("size",    0.5 ) );
      appendParameter( new S3_ParamFloat("decay",   0.2 ) );
      appendParameter( new S3_ParamFloat("xfade",   0.5 ) );
      appendParameter( new S3_ParamFloat("volume",  0.7 ) );
      appendParameter( new S3_ParamText( "freeze",  0, 2, str_onoff ) );
      BUF = new float[0x40000];
      S3_Memset(BUF,0,0x30000*4);
      BUF_t = BUF + 0x10000;
      pos = 0;
      bufsize = 0;
      decay = 0;
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      switch (AIndex) {
        case 0:
          bs = AValue / 2;
          bufsize_t = MSampleRate * (bs*bs);
          break;
        case 1:
          d = AValue;
          decay_t = d*d;
          break;
        case 2:
          d2 = AValue / 10;
          decay2 = d2*d2;
          break;
        case 3:
          v = AValue * 2;
          vol = v*v;
          break;
        case 4:
          fr = AValue;
          break;
      }
    }

    //virtual
    void on_processSample(S3_Sample** AInputs, S3_Sample** AOutputs) final {
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];
      bufsize += ( bufsize_t - bufsize ) * decay2;
      decay   += ( decay_t   - decay   ) * decay2;
      // left
      if (fr<0.5) BUF_t[pos] = spl0;
      BUF[pos] += (BUF_t[pos]-BUF[pos]) * decay;
      spl0 = BUF[pos] * vol;
      // right
      int po2 = pos + 0x20000;
      if( fr<0.5 ) BUF_t[po2] = spl1;
      BUF[po2] += ( BUF_t[po2] - BUF[po2] ) * decay;
      spl1 = BUF[po2] * vol;
      pos += 1;
      if( pos >= (int)bufsize ) pos=0;
      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;
    }

};

//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif
