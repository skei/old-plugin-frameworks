#ifndef s3_interpolate_included
#define s3_interpolate_included
//----------------------------------------------------------------------

// http://paulbourke.net/miscellaneous/interpolation/

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

float S3_Interpolate_Linear(float t, float y1,float y2);
float S3_Interpolate_Cosine(float t, float y1,float y2);
float S3_Interpolate_Cubic(float t, float y0,float y1, float y2,float y3);
float S3_Interpolate_Hermite(float t, float y0, float y1, float y2, float y3, float tension=0, float bias=0);
float S3_Interpolate_Hermite1(float t, float y0, float y1, float y2, float y3);
float S3_Interpolate_Hermite2(float t, float y0, float y1, float y2, float y3);
float S3_Interpolate_Hermite3(float t, float y0, float y1, float y2, float y3);
float S3_Interpolate_Hermite4(float t, float xm1, float x0, float x1, float x2);
float S3_Interpolate_CatmullRom(float t, float p0, float p1, float p2, float p3);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//static __S3_UNUSED
float S3_Interpolate_Linear(float t, float y1,float y2) {
  return ( y1*(1-t) + y2*t );
}

//----------------------------------------------------------------------

/*
  A suitable orientated piece of a cosine function serves to provide a
  smooth transition between adjacent segments.
*/

//static __S3_UNUSED
float S3_Interpolate_Cosine(float t, float y1,float y2) {
  float t2;
  t2 = (1-cosf(t*S3_PI))/2;
  return(y1*(1-t2)+y2*t2);
}

//----------------------------------------------------------------------

/*
  Cubic interpolation is the simplest method that offers true continuity
  between the segments. As such it requires more than just the two
  endpoints of the segment but also the two points on either side of them.
  So the function requires 4 points in all.
  This does raise issues for how to interpolate between the first and last
  segments. A common solution is the dream up two extra points at the
  start and end of the sequence, the new points are created so that they
  have a slope equal to the slope of the start or end segment.



  Paul Breeuwsma proposes the following coefficients for a smoother
  interpolated curve, which uses the slope between the previous point
  and the next as the derivative at the current point. This results
  in what are generally referred to as Catmull-Rom splines.

    a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
    a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
    a2 = -0.5*y0 + 0.5*y2;
    a3 = y1;

*/

//static __S3_UNUSED
float S3_Interpolate_Cubic(float t, float y0,float y1, float y2,float y3) {
  float a0,a1,a2,a3,t2;
  t2 = t*t;
  a0 = y3 - y2 - y0 + y1;
  a1 = y0 - y1 - a0;
  a2 = y2 - y0;
  a3 = y1;
  return(a0*t*t2+a1*t2+a2*t+a3);
}

//----------------------------------------------------------------------

/*
  Hermite interpolation like cubic requires 4 points so that it can
  achieve a higher degree of continuity. In addition it has nice tension
  and biasing controls. Tension can be used to tighten up the curvature
  at the known points. The bias is used to twist the curve about the
  known points.

  Tension: 1 is high, 0 normal, -1 is low
  Bias: 0 is even, positive is towards first segment, negative towards the other
*/

//static __S3_UNUSED
float S3_Interpolate_Hermite(float t, float y0, float y1, float y2, float y3, float tension=0, float bias=0) {
  float m0,m1,t2,t3;
  float a0,a1,a2,a3;

	t2 = t * t;
	t3 = t2 * t;
  m0  = (y1-y0)*(1+bias)*(1-tension)/2;
  m0 += (y2-y1)*(1-bias)*(1-tension)/2;
  m1  = (y2-y1)*(1+bias)*(1-tension)/2;
  m1 += (y3-y2)*(1-bias)*(1-tension)/2;
  a0 =  2*t3 - 3*t2 + 1;
  a1 =    t3 - 2*t2 + t;
  a2 =    t3 -   t2;
  a3 = -2*t3 + 3*t2;
  return(a0*y1+a1*m0+a2*m1+a3*y2);
}

//----------------------------------------------------------------------

// http://www.musicdsp.org/showone.php?id=93

// original

//static __S3_UNUSED
float S3_Interpolate_Hermite1(float t, float y0, float y1, float y2, float y3) {
  // 4-point, 3rd-order Hermite (x-form)
  float c0 = y1;
  float c1 = 0.5f * (y2 - y0);
  float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
  float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
  return ((c3 * t + c2) * t + c1) * t + c0;
}

//----------

// james mccartney

//static __S3_UNUSED
float S3_Interpolate_Hermite2(float t, float y0, float y1, float y2, float y3)
{
  // 4-point, 3rd-order Hermite (x-form)
  float c0 = y1;
  float c1 = 0.5f * (y2 - y0);
  float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
  float c2 = y0 - y1 + c1 - c3;
  return ((c3 * t + c2) * t + c1) * t + c0;
}

//----------

// james mccartney

//static __S3_UNUSED
float S3_Interpolate_Hermite3(float t, float y0, float y1, float y2, float y3)
{
  // 4-point, 3rd-order Hermite (x-form)
  float c0 = y1;
  float c1 = 0.5f * (y2 - y0);
  float y0my1 = y0 - y1;
  float c3 = (y1 - y2) + 0.5f * (y3 - y0my1 - y2);
  float c2 = y0my1 + c1 - c3;
  return ((c3 * t + c2) * t + c1) * t + c0;
}

//----------

/*
  frac_pos: fractional value [0.0f - 1.0f] to interpolator
  pntr: pointer to float array where:
  pntr[0] = previous sample (idx = -1)
  pntr[1] = current sample (idx = 0)
  pntr[2] = next sample (idx = +1)
  pntr[3] = after next sample (idx = +2)
  The interpolation takes place between pntr[1] and pntr[2].
*/

// laurent de soras

//static __S3_UNUSED
float S3_Interpolate_Hermite4(float t, float xm1, float x0, float x1, float x2)
{
  float c = (x1 - xm1) * 0.5f;
  float v = x0 - x1;
  float w = c + v;
  float a = w + v + (x2 - x0) * 0.5f;
  float b_neg = w + a;
  return ((((a * t) - b_neg) * t + c) * t + x0);
}

//----------------------------------------------------------------------


// http://www.pascalgamedevelopment.com/showthread.php?4621-Bezier-curves/page2&highlight=catmull

/*
  catmull-rom
  a0 = -0.5*y0 + 1.5*y1 - 1.5*y2 + 0.5*y3;
  a1 = y0 - 2.5*y1 + 2*y2 - 0.5*y3;
  a2 = -0.5*y0 + 0.5*y2;
  a3 = y1;
*/

//----------

// https://code.google.com/p/nxpascal/source/browse/trunk/src/nxMath.pas#87

//static __S3_UNUSED
float S3_Interpolate_CatmullRom(float t, float p0, float p1, float p2, float p3) {
  return 0.5 * ( 2*p1 + (p2-p0)*t +
               ( 2*p0 - 5*p1 + 4*p2 - p3) * t*t +
               ( 3*p1 -   p0 - 3*p2 + p3) * t*t*t );
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// http://music.columbia.edu/pipermail/music-dsp/2000-August/038440.html

/*
  (Dave/Muon Software Ltd has some 4-point spline thing?)
  Thanks Bram (and Dave), got that one too now, works ok!
  --------------------
  const double L1 = floor[-1];
  const double L0 = floor[ 0];
  const double H0 = floor[+1];
  const double H1 = floor[+2];
  const double x = fractpos;
  return (float)(L0 + .5f* x*(H0-L1 + x*(H0 + L0*(-2) + L1 + x*( (H0
  - L0)*9 + (L1 - H1)*3 + x*((L0 - H0)*15 + (H1 -  L1)*5 + x*((H0 -
  L0)*6 + (L1 - H1)*2))))));
  --------------------------
  -olli
*/

//----------

// http://music.columbia.edu/pipermail/music-dsp/2000-August/038442.html

/*
  Here's the three point version of the 5-point interpolation I posted the
  other week. Credit for these goes to Josuha Scholar of comp.dsp
  (neonfish99 at aol.com).
  I haven't had any time to turn this into a niftier function yet so just the
  raw code straight from Josh:

  // interpolates between L0 and H0 taking the previous (L1) and next (H1) points into account

  inline float ThirdInterp(const float x,const float L1,const float L0,const float H0,const float H1) {
    return
    L0 +
    .5f*
    x*(H0-L1 +
      x*(H0 + L0*(-2) + L1 +
          x*( (H0 - L0)*9 + (L1 - H1)*3 +
            x*((L0 - H0)*15 + (H1 -  L1)*5 +
                x*((H0 - L0)*6 + (L1 - H1)*2 )))));
  }

*/

//----------

/*
  http://music.columbia.edu/pipermail/music-dsp/2000-August/038455.html

  Now my interpolator collection has grown, and i need to ask if you know
  some of the missing/uncertain names, if there are agreed ones. Is any of
  these possibly "b-spline", which i've heard mentioned occasionally?

  // 1-point flat (didn't bother to copy all routines)
  // 2-point linear
  // 2-point raised cosine

  // 4-point spline (Joshua Scholar) (name???)
  const double L1 = floor[-1];
  const double L0 = floor[ 0];
  const double H0 = floor[+1];
  const double H1 = floor[+2];
  const double x = fractpos;
  return (float)(L0 + .5f* x*(H0-L1 + x*(H0 + L0*(-2) + L1 + x*( (H0
  - L0)*9 + (L1 - H1)*3 +
  x*((L0 - H0)*15 + (H1 -  L1)*5 + x*((H0 - L0)*6 + (L1 - H1)*2
  ))))));

  // 4-point spline (name???)
  const double ym1 = floor[-1];
  const double  y0 = floor[ 0];
  const double  y1 = floor[+1];
  const double  y2 = floor[+2];
  const double   x = fractpos;
  return(float)(x*x*x*(y0-y1+y2-ym1)+x*x*(-2.0*y0+y1-y2+2.0*ym1)+x*(y1-ym1)+y0);

  // 4-point geometric average spline (correct???) (name???)
  const double y0 = floor[-1];
  const double y1 = floor[ 0];
  const double y2 = floor[+1];
  const double y3 = floor[+2];
  const double yd = fractpos;
  return (float)((y1 + (y1-y0)*yd)*(1-yd)/2 + (y1 + (y2-y1)*yd)/2 +
  (y2 + (y3-y2)*(yd-1))*yd/2);

  // 6-point expanded Hermite (linear ramp between two 4th degree
  // polynomials both going through 5 points) (Is there a name for this
  // already???) UNOPTIMIZED
  const double ym2 = floor[-2];
  const double ym1 = floor[-1];
  const double  y0 = floor[ 0];
  const double  y1 = floor[+1];
  const double  y2 = floor[+2];
  const double  y3 = floor[+3];
  const double   x = fractpos;
  return (float) (
  x*x*x*x*x*(-5.0/12.0*y0+5.0/12.0*y1-5.0/24.0*y2+1.0/24.0*y3+5.0/24.0*ym1-
  1.0/24.0*ym2)+
  x*x*x*x*(13.0/12.0*y0-y1+11.0/24.0*y2-1.0/12.0*y3-7.0/12.0*ym1+0.125*ym2)+
  x*x*x*(5.0/12.0*y0-7.0/12.0*y1+7.0/24.0*y2-1.0/24.0*y3-1.0/24.0*ym1-
  1.0/24.0*ym2)+
  x*x*(-25.0/12.0*y0+1.5*y1-11.0/24.0*y2+1.0/12.0*y3+13.0/12.0*ym1-0.125*ym2)+
  x*(2.0/3.0*y1-1.0/12.0*y2-2.0/3.0*ym1+1.0/12.0*ym2)+
  y0);
*/

//----------------------------------------------------------------------
#endif
