#ifndef fx_moog24db_included
#define fx_moog24db_included
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

//#define SKEI_DEBUG_MEM

#ifdef SKEI_VST
  //#define SKEI_DEBUG_VST
  #define SKEI_DEBUG_SOCKET
#endif

//----------------------------------------------------------------------
// include
//----------------------------------------------------------------------

#include "skei.h"
#include "skei_plugin.h"

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

const char* txt_proc[2]   = { "stereo", "mono" };
const char* txt_flt[2]    = { "HP (6dB/oct)", "BP (6dB/oct)" };
const char* txt_onoff[2]  = { "off", "on" };

//----------

class myPlugin
: public SPlugin
{

  private:

    double spl0, spl1;
    double slider1, slider2, slider3, slider4, slider5, slider6, slider7, slider8;

    // init
    double cDenorm;
    double mv, fs, fgain;
    double c1, c2, c3;
    double bl_c1, bl_c2, bl_c3;

    // recalc
    double mono;
    double ftype;
    double sx;
    double res;
    double drive;
    double outgain;
    double limiter;
    double os;
    double drvc;
    double cutoff;
    double f;
    double scale;

    // block
    double tp, d_p, src_p, tgt_p;
    double tk, d_k, src_k, tgt_k;
    double tr, d_r, src_r, tgt_r;

    // sample
    double inl, inr;
    double outl, outr;
    double ps_out1l, ps_out1r, ps_out2l, ps_out2r;
    double o_in1l, o_in1r, o_in2l, o_in2r;
    double bl3_l1, bl3_r1, bl3_l2, bl3_r2;
    double bl2_l1, bl2_r1, bl2_l2, bl2_r2;
    double bl1_l1, bl1_r1, bl1_l2, bl1_r2;
    double bl_out1l, bl_out1r, bl_out2l, bl_out2r;
    double o_out1l, o_out2l, o_out1r, o_out2r;
    double s3l, s3r, s2l, s2r, s1l, s1r;
    double inputl, inputr;
    double xl, xr;
    double y1l, y1r, y2l, y2r, y3l, y3r, y4l, y4r;
    double oldxl, oldxr;
    double oldy1l, oldy1r, oldy2l, oldy2r, oldy3l, oldy3r;
    double in, out, input;
    double bl3_1, bl3_2, bl2_1, bl2_2, bl1_1, bl1_2;
    double bl_out1, bl_out2;
    double x,oldx;
    double y1,y2,y3,y4;
    double oldy1,oldy2,oldy3;

  //----------------------------------------
  //
  //----------------------------------------

  public:

    //virtual
    void on_create(void) {
      MInfo.name       = "fx_moog24db";
      MInfo.author     = "skeilib";
      MInfo.product    = MInfo.name;
      MInfo.uniqueId   = SKEI_MAGIC + 0x0000;
      MInfo.numInputs  = 2;
      MInfo.numOutputs = 2;
      //
      appendParameter( new SParamText(  "mode",       0,    2,  txt_proc ));
      appendParameter( new SParamText(  "filter",     0,    2,  txt_flt ));
      appendParameter( new SParamFloat( "cutoff",     100,  0,  100 ));
      appendParameter( new SParamFloat( "res",        0,    0,  0.85 ));
      appendParameter( new SParamFloat( "drive",      0,    0,  100 ));
      appendParameter( new SParamFloat( "output",     0,   -25, 25 ));
      appendParameter( new SParamText(  "limiter",    0,    2,  txt_onoff ));
      appendParameter( new SParamText(  "oversample", 0,    2,  txt_onoff ));
    }

    //----------

    //virtual
    void on_destroy(void) {
    }

  //----------------------------------------
  //
  //----------------------------------------

  private:

    void init(void) {
      cDenorm = pow(10,-30);
      mv      = pow(2,(-0.2/6));
      fs      = MState.sampleRate;
      c1      = 1;
      c2      = -0.75;
      c3      = 0.17;
      fgain   = 5;
      bl_c1   = 0.52;
      bl_c2   = 0.54;
      bl_c3   = -0.02;
    }

    void recalc(void) {
      drvc    = 1.2;
      mono    = slider1;
      ftype   = slider2;
      sx      = 16+slider3*1.20103;
      cutoff  = floor(exp(sx*log(1.059))*8.17742);
      f       = 2 * cutoff / fs;
      tgt_k   = 3.6 * f - 1.6 * f * f - 1;
      tgt_p   = (tgt_k+1) * 0.5;
      scale   = pow(2.718281828459045,((1-tgt_p)*1.386249));
      res     = slider4;
      tgt_r   = res*scale;
      drive   = 1+slider5/100;
      outgain = pow(10,(slider6/20));
      limiter = slider7;
      os      = slider8;
    }

    void sample(double* _spl0, double* _spl1) {

      spl0 = *_spl0;
      spl1 = *_spl1;

      tk += d_k;
      tp += d_p;
      tr += d_r;
      //stereo
      if (mono==0) {
        //distortion
        if (drive > 1) {
          inl = spl0;
          inr = spl1;
          if (os == 1) {
            //power series in
            ps_out1l = 0.5*(inl+ps_out2l);
            ps_out2l = 0.5*ps_out1l;
            ps_out1r = 0.5*(inr+ps_out2r);
            ps_out2r = 0.5*ps_out1r;
            //drive
            o_in1l  = ps_out1l * (abs(ps_out1l) + drive) / (pow(2,ps_out1l) + (drive-1) * abs(ps_out1l) + 1) * (drive/drvc);
            o_in2l  = ps_out2l * (abs(ps_out2l) + drive) / (pow(2,ps_out2l) + (drive-1) * abs(ps_out2l) + 1) * (drive/drvc);
            o_in1r  = ps_out1r * (abs(ps_out1r) + drive) / (pow(2,ps_out1r) + (drive-1) * abs(ps_out1r) + 1) * (drive/drvc);
            o_in2r  = ps_out2r * (abs(ps_out2r) + drive) / (pow(2,ps_out2r) + (drive-1) * abs(ps_out2r) + 1) * (drive/drvc);
            //---------------------------
            //bandlimit
            bl3_l1 = bl2_l1;
            bl3_r1 = bl2_r1;
            bl3_l2 = bl2_l2;
            bl3_r2 = bl2_r2;
            bl2_l1 = bl1_l1;
            bl2_r1 = bl1_r1;
            bl2_l2 = bl1_l2;
            bl2_r2 = bl1_r2;
            bl1_l1 = o_in1l;
            bl1_r1 = o_in1r;
            bl1_l2 = o_in2l;
            bl1_r2 = o_in2r;
            bl_out1l = (bl1_l1*bl_c1 + bl2_l1*bl_c2 + bl3_l1*bl_c3);
            bl_out1r = (bl1_r1*bl_c1 + bl2_r1*bl_c2 + bl3_r1*bl_c3);
            bl_out2l = (bl1_l2*bl_c1 + bl2_l2*bl_c2 + bl3_l2*bl_c3);
            bl_out2r = (bl1_r2*bl_c1 + bl2_r2*bl_c2 + bl3_r2*bl_c3);
            //power series out
            o_out1l = 0.5*(bl_out1l+o_out2l);
            o_out2l = 0.5*(bl_out2l+o_out1l);
            o_out1r = 0.5*(bl_out1r+o_out2r);
            o_out2r = 0.5*(bl_out2r+o_out1r);
            //fir restoration
            s3l = s2l;
            s3r = s2r;
            s2l = s1l;
            s2r = s1r;
            s1l = o_out1l;
            s1r = o_out1r;
            inputl = (s1l*c1+s2l*c2+s3l*c3)*fgain;
            inputr = (s1r*c1+s2r*c2+s3r*c3)*fgain;
          }
          else {
            inputl = inl*(abs(inl) + drive)/(pow(2,inl) + (drive-1)*abs(inl) + 1)*(drive/drvc);
            inputr = inr*(abs(inr) + drive)/(pow(2,inr) + (drive-1)*abs(inr) + 1)*(drive/drvc);
          }
        }
        else {
          inputl = spl0;
          inputr = spl1;
        }
        //filter
        xl = inputl-tr*y4l;
        xr = inputr-tr*y4r;
        y1l=xl*tp+oldxl*tp-tk*y1l;
        y1r=xr*tp+oldxr*tp-tk*y1r;
        y2l=y1l*tp+oldy1l*tp-tk*y2l;
        y2r=y1r*tp+oldy1r*tp-tk*y2r;
        y3l=y2l*tp+oldy2l*tp-tk*y3l;
        y3r=y2r*tp+oldy2r*tp-tk*y3r;
        y4l=y3l*tp+oldy3l*tp-tk*y4l;
        y4r=y3r*tp+oldy3r*tp-tk*y4r;
        oldxl = xl;
        oldxr = xr;
        oldy1l = y1l;
        oldy1r = y1r;
        oldy2l = y2l;
        oldy2r = y2r;
        oldy3l = y3l;
        oldy3r = y3r;
        if (ftype == 0) {
          outl = y4l;
          outr = y4r;
        }
        if (ftype == 1) {
          outl = inputl-y4l;
          outr = inputr-y4r;
        }
        if (ftype == 2) {
          outl = 6*(y3l-y4l);
          outr = 6*(y3r-y4r);
        }
        outl = outl*outgain;
        outr = outr*outgain;
        //limiter
        if (limiter == 0) {
          spl0 = SMin(SMax((outl+cDenorm),-mv),mv);
          spl1 = SMin(SMax((outr+cDenorm),-mv),mv);
        }
        else {
          spl0 = outl;
          spl1 = outr;
        }
      }
      //mono
      else {
        in = (spl0+spl1)/2;
        //distortion
        if (drive > 1) {
          if (os == 1) {
            //power series in
            ps_out1l = 0.5*(in+ps_out2l);
            ps_out2l = 0.5*ps_out1l;
            //drive
            o_in1l  = ps_out1l*(abs(ps_out1l) + drive)/(pow(2,ps_out1l) + (drive-1)*abs(ps_out1l) + 1)*(drive/drvc);
            o_in2l  = ps_out2l*(abs(ps_out2l) + drive)/(pow(2,ps_out2l) + (drive-1)*abs(ps_out2l) + 1)*(drive/drvc);
            //---------------------------
            //bandlimit
            bl3_1 = bl2_1;
            bl3_2 = bl2_2;
            bl2_1 = bl1_1;
            bl2_2 = bl1_2;
            bl1_1 = o_in1l;
            bl1_2 = o_in2l; // skeio_in12;
            //bl1_2 = o_in12;
            bl_out1 = (bl1_1*bl_c1 + bl2_1*bl_c2 + bl3_1*bl_c3);
            bl_out2 = (bl1_2*bl_c1 + bl2_2*bl_c2 + bl3_2*bl_c3);
            //power series out
            o_out1l = 0.5*(bl_out1+o_out2l);
            o_out2l = 0.5*(bl_out2+o_out1l);
            //fir restoration
            s3l = s2l;
            s2l = s1l;
            s1l = o_out1l;
            input = (s1l*c1+s2l*c2+s3l*c3)*fgain;
          }
          else {
            input = in*(abs(in) + drive)/(pow(2,in) + (drive-1)*abs(in) + 1)*(drive/drvc);
          }
        }
        else {
          input = in;
        }
        //filter


        x = input-tr*y4;
        y1=x*tp+oldx*tp-tk*y1;
        y2=y1*tp+oldy1*tp-tk*y2;
        y3=y2*tp+oldy2*tp-tk*y3;
        y4=y3*tp+oldy3*tp-tk*y4;
        oldx = x;
        oldy1 = y1;
        oldy2 = y2;
        oldy3 = y3;
        if (ftype == 0) {
          out = y4;
        }
        if (ftype == 1) {
          out = input-y4;
        }
        if (ftype == 2) {
          out = 6*(y3-y4);
        }
        out = out*outgain;
        //limiter
        if (limiter == 0) {
          spl0 = SMin(SMax((out+cDenorm),-mv),mv);
          spl1 = SMin(SMax((out+cDenorm),-mv),mv);
        }
        else {
          spl0 = out;
          spl1 = out;
        }
      }

      *_spl0 = spl0;
      *_spl1 = spl1;

    }

  //----------------------------------------
  // events
  //----------------------------------------

  public:

    //virtual
    void on_stateChange(uint32 AState) {
      switch(AState) {
        case sps_sampleRate:
          break;
        case sps_resume:
          break;
      }
    }

    //virtual
    void on_parameterChange(int32 AIndex, float AValue) {
      switch(AIndex) {
        case 0: slider1 = AValue; break;
        case 1: slider2 = AValue; break;
        case 2: slider3 = AValue; break;
        case 3: slider4 = AValue; break;
        case 4: slider5 = AValue; break;
        case 5: slider6 = AValue; break;
        case 6: slider7 = AValue; break;
        case 7: slider8 = AValue; break;
      }
      recalc();
    }

  //----------------------------------------
  // audio
  //----------------------------------------

  public:

    //virtual
    void on_processBlock(SSample** AInputs, SSample** AOutputs, uint32 ANumSamples) {

      d_p = (tgt_p-src_p) / ANumSamples;
      tp = src_p;
      src_p = tgt_p;
      d_k = (tgt_k-src_k) / ANumSamples;
      tk = src_k;
      src_k = tgt_k;
      d_r = (tgt_r-src_r) / ANumSamples;
      tr = src_r;
      src_r = tgt_r;

      float* input0 = AInputs[0];
      float* input1 = AInputs[1];
      float* output0 = AOutputs[0];
      float* output1 = AOutputs[1];
      for (int i=0; i<ANumSamples; i++) {
        double spl0 = *input0++;
        double spl1 = *input1++;

        sample(&spl0,&spl1);

        *output0++ = spl0;
        *output1++ = spl1;
      }
    }

};

//----------------------------------------------------------------------
// main
//----------------------------------------------------------------------

SKEI_MAIN(myPlugin);

//----------------------------------------------------------------------
#endif










#if 0








// (C) 2008-2009, Lubomir I. Ivanov

// NO WARRANTY IS GRANTED. THIS PLUG-IN IS PROVIDED ON AN "AS IS" BASIS, WITHOUT
// WARRANTY OF ANY KIND. NO LIABILITY IS GRANTED, INCLUDING, BUT NOT LIMITED TO,
// ANY DIRECT OR INDIRECT,  SPECIAL,  INCIDENTAL OR CONSEQUENTIAL DAMAGE ARISING
// OUT OF  THE  USE  OR INABILITY  TO  USE  THIS PLUG-IN,  COMPUTER FAILTURE  OF
// MALFUNCTION INCLUDED.  THE USE OF THE SOURCE CODE,  EITHER  PARTIALLY  OR  IN
// TOTAL, IS ONLY GRANTED,  IF USED IN THE SENSE OF THE AUTHOR'S INTENTION,  AND
// USED WITH ACKNOWLEDGEMENT OF THE AUTHOR. FURTHERMORE IS THIS PLUG-IN A  THIRD
// PARTY CONTRIBUTION,  EVEN IF INCLUDED IN REAPER(TM),  COCKOS INCORPORATED  OR
// ITS AFFILIATES HAVE NOTHING TO DO WITH IT.  LAST BUT NOT LEAST, BY USING THIS
// PLUG-IN YOU RELINQUISH YOUR CLAIM TO SUE IT'S AUTHOR, AS WELL AS THE CLAIM TO
// ENTRUST SOMEBODY ELSE WITH DOING SO.
//
// Released under GPL:
// <http://www.gnu.org/licenses/>.

//******************************************************************************
// References:
// Moog filter appromixation
// by CSound source code, Stilson/Smith CCRMA paper
// Distortion by Bram de Jong
//******************************************************************************

desc:Moog LP/HP/BP

slider1:0<0,1,1{Stereo,Mono}>Processing
slider2:0<0,2,1{LP (24dB/oct),HP (6dB/oct),BP (6dB/oct)}>Filter Type
slider3:100<0,100,0.05>Cutoff (Scale)
slider4:0<0,0.85,0.001>Res (Min/Max)
slider5:0<0,100,0.05>Drive (%)
slider6:0<-25,25,0.05>Output (dB)
slider7:0<0,1,1{On,Off}>Limiter
slider8:0<0,1,1{Off,On}>Oversample (x2)

@init
cDenorm = 10^-30;
mv=2^(-0.2/6);
fs = srate;

//fir restoration
c1 = 1;
c2 = -0.75;
c3 = 0.17;
fgain = 5;

//fir bandlimit
bl_c1 = 0.52;
bl_c2 = 0.54;
bl_c3 = -0.02;

@slider
os = slider8;
mono = slider1;
ftype = slider2;
res = slider4;
drive = 1+slider5/100;
drvc = 1.2;
outgain = 10^(slider6/20);
limiter = slider7;

sx = 16+slider3*1.20103;
cutoff = floor(exp(sx*log(1.059))*8.17742);

f = 2 * cutoff / fs;
tgt_k = 3.6*f-1.6*f*f-1;
tgt_p = (tgt_k+1)*0.5;
scale = 2.718281828459045^((1-tgt_p)*1.386249);
tgt_r = res*scale;

@block
d_p = (tgt_p-src_p)/samplesblock;
tp = src_p;
src_p = tgt_p;
d_k = (tgt_k-src_k)/samplesblock;
tk = src_k;
src_k = tgt_k;
d_r = (tgt_r-src_r)/samplesblock;
tr = src_r;
src_r = tgt_r;

@sample
tk += d_k;
tp += d_p;
tr += d_r;

//stereo
mono == 0 ? (

//distortion
drive > 1 ? (

inl = spl0;
inr = spl1;

os == 1 ? (
//power series in
ps_out1l = 0.5*(inl+ps_out2l);
ps_out2l = 0.5*ps_out1l;
ps_out1r = 0.5*(inr+ps_out2r);
ps_out2r = 0.5*ps_out1r;

//drive
o_in1l  = ps_out1l*(abs(ps_out1l) + drive)/(ps_out1l^2 + (drive-1)*abs(ps_out1l) + 1)*(drive/drvc);
o_in2l  = ps_out2l*(abs(ps_out2l) + drive)/(ps_out2l^2 + (drive-1)*abs(ps_out2l) + 1)*(drive/drvc);
o_in1r  = ps_out1r*(abs(ps_out1r) + drive)/(ps_out1r^2 + (drive-1)*abs(ps_out1r) + 1)*(drive/drvc);
o_in2r  = ps_out2r*(abs(ps_out2r) + drive)/(ps_out2r^2 + (drive-1)*abs(ps_out2r) + 1)*(drive/drvc);

//---------------------------
//bandlimit
bl3_l1 = bl2_l1;
bl3_r1 = bl2_r1;
bl3_l2 = bl2_l2;
bl3_r2 = bl2_r2;

bl2_l1 = bl1_l1;
bl2_r1 = bl1_r1;
bl2_l2 = bl1_l2;
bl2_r2 = bl1_r2;

bl1_l1 = o_in1l;
bl1_r1 = o_in1r;
bl1_l2 = o_in2l;
bl1_r2 = o_in2r;

bl_out1l = (bl1_l1*bl_c1 + bl2_l1*bl_c2 + bl3_l1*bl_c3);
bl_out1r = (bl1_r1*bl_c1 + bl2_r1*bl_c2 + bl3_r1*bl_c3);
bl_out2l = (bl1_l2*bl_c1 + bl2_l2*bl_c2 + bl3_l2*bl_c3);
bl_out2r = (bl1_r2*bl_c1 + bl2_r2*bl_c2 + bl3_r2*bl_c3);

//power series out
o_out1l = 0.5*(bl_out1l+o_out2l);
o_out2l = 0.5*(bl_out2l+o_out1l);
o_out1r = 0.5*(bl_out1r+o_out2r);
o_out2r = 0.5*(bl_out2r+o_out1r);

//fir restoration
s3l = s2l;
s3r = s2r;
s2l = s1l;
s2r = s1r;
s1l = o_out1l;
s1r = o_out1r;

inputl = (s1l*c1+s2l*c2+s3l*c3)*fgain;
inputr = (s1r*c1+s2r*c2+s3r*c3)*fgain;

) : (
inputl = inl*(abs(inl) + drive)/(inl^2 + (drive-1)*abs(inl) + 1)*(drive/drvc);
inputr = inr*(abs(inr) + drive)/(inr^2 + (drive-1)*abs(inr) + 1)*(drive/drvc);
);

) : (
inputl = spl0;
inputr = spl1;
);

//filter
xl = inputl-tr*y4l;
xr = inputr-tr*y4r;

y1l=xl*tp+oldxl*tp-tk*y1l;
y1r=xr*tp+oldxr*tp-tk*y1r;
y2l=y1l*tp+oldy1l*tp-tk*y2l;
y2r=y1r*tp+oldy1r*tp-tk*y2r;
y3l=y2l*tp+oldy2l*tp-tk*y3l;
y3r=y2r*tp+oldy2r*tp-tk*y3r;
y4l=y3l*tp+oldy3l*tp-tk*y4l;
y4r=y3r*tp+oldy3r*tp-tk*y4r;

oldxl = xl;
oldxr = xr;
oldy1l = y1l;
oldy1r = y1r;
oldy2l = y2l;
oldy2r = y2r;
oldy3l = y3l;
oldy3r = y3r;

ftype == 0 ? (
  outl = y4l;
  outr = y4r;
);
ftype == 1 ? (
  outl = inputl-y4l;
  outr = inputr-y4r;
);
ftype == 2 ? (
  outl = 6*(y3l-y4l);
  outr = 6*(y3r-y4r);
);

outl = outl*outgain;
outr = outr*outgain;

//limiter
limiter == 0 ? (
  spl0 = min(max((outl+cDenorm),-mv),mv);
  spl1 = min(max((outr+cDenorm),-mv),mv);
) : (
  spl0 = outl;
  spl1 = outr;
);

//mono
) : (

in = (spl0+spl1)/2;

//distortion
drive > 1 ? (

os == 1 ? (

//power series in
ps_out1l = 0.5*(in+ps_out2l);
ps_out2l = 0.5*ps_out1l;

//drive
o_in1l  = ps_out1l*(abs(ps_out1l) + drive)/(ps_out1l^2 + (drive-1)*abs(ps_out1l) + 1)*(drive/drvc);
o_in2l  = ps_out2l*(abs(ps_out2l) + drive)/(ps_out2l^2 + (drive-1)*abs(ps_out2l) + 1)*(drive/drvc);

//---------------------------
//bandlimit
bl3_1 = bl2_1;
bl3_2 = bl2_2;

bl2_1 = bl1_1;
bl2_2 = bl1_2;

bl1_1 = o_in1l;
bl1_2 = o_in12;

bl_out1 = (bl1_1*bl_c1 + bl2_1*bl_c2 + bl3_1*bl_c3);
bl_out2 = (bl1_2*bl_c1 + bl2_2*bl_c2 + bl3_2*bl_c3);

//power series out
o_out1l = 0.5*(bl_out1+o_out2l);
o_out2l = 0.5*(bl_out2+o_out1l);

//fir restoration
s3l = s2l;
s2l = s1l;
s1l = o_out1l;

input = (s1l*c1+s2l*c2+s3l*c3)*fgain;

) : (

input = in*(abs(in) + drive)/(in^2 + (drive-1)*abs(in) + 1)*(drive/drvc);

);

) : (
input = in;
);

//filter
x = input-tr*y4;

y1=x*tp+oldx*tp-tk*y1;
y2=y1*tp+oldy1*tp-tk*y2;
y3=y2*tp+oldy2*tp-tk*y3;
y4=y3*tp+oldy3*tp-tk*y4;

oldx = x;
oldy1 = y1;
oldy2 = y2;
oldy3 = y3;

ftype == 0 ? (
  out = y4;
);
ftype == 1 ? (
  out = input-y4;
);
ftype == 2 ? (
  out = 6*(y3-y4);
);

out = out*outgain;

//limiter
limiter == 0 ? (
  spl0 = min(max((out+cDenorm),-mv),mv);
  spl1 = min(max((out+cDenorm),-mv),mv);
) : (
  spl0 = out;
  spl1 = out;
);

);

@gfx 100 10
gfx_x=gfx_y=5;
gfx_lineto(gfx_x, gfx_y,0);
gfx_r=gfx_b=0;
gfx_g=gfx_a=1;
gfx_drawchar($'F');
gfx_drawchar($' ');
gfx_drawchar($'=');
gfx_drawchar($' ');
gfx_drawnumber(cutoff,0);
gfx_drawchar($' ');
gfx_drawchar($'H');
gfx_drawchar($'z');






#endif

