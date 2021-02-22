
#ifdef S3_DEBUG
  #define S3_DEBUG_MEM
  #ifdef S3_PLUGIN_VST
    #define S3_DEBUG_SOCKET
    //#define S3_DEBUG_VST
  #endif
#endif

//----------------------------------------------------------------------

#include "s3.h"
#include "common/s3_math.h"
#include "plugin/s3_plugin.h"
#include "plugin/s3_plugin_id.h"

//----------------------------------------------------------------------

class myPlugin : public S3_Plugin {

  private:
    float MCurve;

  public:

    //virtual
    void on_create(void) final {
      MName         = "fx_curve_shaper";
      MAuthor       = "tor.helge.skei";
      MDescription  = MName;
      MUniqueId     = S3_MAGIC + s3_fx_curve_shaper_id;
      MNumInputs    = 2;
      MNumOutputs   = 2;
      MVersion      = 0;
      appendParameter( new S3_ParamFloat( "curve", 0.5 ) );
      MCurve        = 0.5f;
    }

    //------------------------------

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) final {
      if (AIndex == 0) MCurve = AValue;
    }

    //------------------------------

    //virtual
    void on_processBlock(S3_Sample** AInputs, S3_Sample** AOutputs, uint32 ANumSamples) final {
      float* in0 = AInputs[0];
      float* in1 = AInputs[1];
      float* out0 = AOutputs[0];
      float* out1 = AOutputs[1];
      for (uint32 i=0; i<ANumSamples; i++) {

        float v0 = *in0++;
        *out0++ = S3_Curve( fabs(v0), MCurve ) * S3_Sign(v0);
        //*out0++ = fabs(v0) * s0;

        float v1 = *in1++;
        *out1++ = S3_Curve( fabs(v1), MCurve ) * S3_Sign(v1);
        //*out1++ = fabs(v1) * s1;

      }
    }

};

//----------------------------------------------------------------------

S3_MAIN(myPlugin);
