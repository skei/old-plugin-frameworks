#ifndef template_included
#define template_included
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

//----------------------------------------------------------------------

class myPlugin : public S3_Plugin {

  private:

    float lo0,lo1,hi0,hi1,n0,n1;
    float lo_mid;
    float mid_hi;
    float loboost;
    float midboost;
    float hiboost;
    float loclip;
    float midclip;
    float hiclip;
    float lopost;
    float midpost;
    float hipost;
    float filter;
    float vol;

  public:

    //virtual
    void on_create(void) final {
      MName         = "fx_dist3";
      MAuthor       = "tor.helge.skei";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_fx_dist3_id;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      MVersion      = 0;
      MFlags       |= s3_pf_per_sample;

      appendParameter( new S3_ParamFloat( "low/mid",   0.20    ));
      appendParameter( new S3_ParamFloat( "mid/hi",    0.80    ));
      appendParameter( new S3_ParamFloat( "filter",    1       ));
      appendParameter( new S3_ParamFloat( "vol",       1       ));
      appendParameter( new S3_ParamFloat( "low boost", 1, 0, 2 ));
      appendParameter( new S3_ParamFloat( "low clip",  1       ));
      appendParameter( new S3_ParamFloat( "low post",  1, 0, 2 ));
      appendParameter( new S3_ParamFloat( "mid boost", 1, 0, 2 ));
      appendParameter( new S3_ParamFloat( "mid clip",  1       ));
      appendParameter( new S3_ParamFloat( "mid post",  1, 0, 2 ));
      appendParameter( new S3_ParamFloat( "high boost",1, 0, 2 ));
      appendParameter( new S3_ParamFloat( "high clip", 1       ));
      appendParameter( new S3_ParamFloat( "high post", 1, 0, 2 ));

      lo0 = 0;
      lo1 = 0;
      hi0 = 0;
      hi1 = 0;
      n0  = 0;
      n1  = 0;

    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      float a2 = AValue * AValue;
      float a3 = AValue * AValue * AValue;
      switch (AIndex) {
        case 0:  lo_mid   = a3; break;
        case 1:  mid_hi   = a3; break;
        case 2:  filter   = a3; break;
        case 3:  vol      = a3; break;
        case 4:  loboost  = a2; break;
        case 5:  loclip   = a3; break;
        case 6:  lopost   = a2; break;
        case 7:  midboost = a2; break;
        case 8:  midclip  = a3; break;
        case 9:  midpost  = a2; break;
        case 10: hiboost  = a2; break;
        case 11: hiclip   = a3; break;
        case 12: hipost   = a2; break;
      }
    }

    //virtual
    void on_processSample(S3_Sample** AInputs, S3_Sample** AOutputs) final {
      //updateSync;
      float spl0 = *AInputs[0];
      float spl1 = *AInputs[1];

      lo0   += ((spl0-lo0) * lo_mid);
      hi0   += ((spl0-hi0) * mid_hi);
      float low0  = loboost * lo0;
      float mid0  = midboost * (hi0-lo0);
      float high0 = hiboost * (spl0-hi0);
      low0  = lopost * S3_Max(-loclip,S3_Min(loclip,low0));
      mid0  = midpost * S3_Max(-midclip,S3_Min(midclip,mid0));
      high0 = hipost * S3_Max(-hiclip,S3_Min(hiclip,high0));
      spl0  = (low0+mid0+high0);
      n0    += ((spl0-n0)*filter);
      spl0  = (n0)*vol;

      lo1   += ((spl1-lo1) * lo_mid);
      hi1   += ((spl1-hi1) * mid_hi);
      float low1  = loboost * lo1;
      float mid1  = midboost * (hi1-lo1);
      float high1 = hiboost * (spl1-hi1);
      low1  = lopost * S3_Max(-loclip,S3_Min(loclip,low1));
      mid1  = midpost * S3_Max(-midclip,S3_Min(midclip,mid1));
      high1 = hipost * S3_Max(-hiclip,S3_Min(hiclip,high1));
      spl1  = (low1+mid1+high1);
      n1    += ((spl1-n1)*filter);
      spl1  = (n1)*vol;

      *AOutputs[0] = spl0;
      *AOutputs[1] = spl1;

    }

};

//----------------------------------------------------------------------

S3_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif

