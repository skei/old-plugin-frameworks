#ifndef s3_color_included
#define s3_color_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

struct S3_Color {
  float r,g,b,a;
  S3_Color();
  S3_Color(float ac);
  S3_Color(float ar, float ag, float ab);
  S3_Color(float ar, float ag, float ab, float aa);
  void  set(float ac);
  void  set(float ar, float ag, float ab);
  void  set(float ar, float ag, float ab, float aa);
  void  blend(float value, float ar, float ag, float ab);
  void  blend(float value, S3_Color AColor);
};

//----------

uint32    S3_BGR(uint8 r, uint8 g, uint8 b, uint8 a=255);
uint32    S3_BGR(S3_Color AColor);
uint32    S3_RGB(uint8 r, uint8 g, uint8 b, uint8 a=255);
uint32    S3_RGB(S3_Color AColor);
uint8     S3_AlphaColor(uint32 c, uint32 a);
uint8     S3_ScaleColor(uint8 c, float n );
uint32    S3_AlphaRgb(uint32 c, uint8 a);
uint32    S3_AlphaMix(uint32 c1, uint32 c2, uint8 a);
S3_Color  S3_BlendColor(float value, float r1, float g1, float b1, float r2, float g2, float b2);
S3_Color  S3_BlendColor(float value, S3_Color AColor1, S3_Color AColor2);

//----------

#define S3_White          S3_Color( 1.0, 1.0, 1.0 )

#define S3_BrightGrey     S3_Color( 1.0, 1.0, 1.0 )
#define S3_BrightRed      S3_Color( 1.0, 0.0, 0.0 )
#define S3_BrightOrange   S3_Color( 1.0, 0.5, 0.0 )
#define S3_BrightYellow   S3_Color( 1.0, 1.0, 0.0 )
#define S3_BrightGreen    S3_Color( 0.0, 1.0, 0.0 )
#define S3_BrightCyan     S3_Color( 0.0, 1.0, 1.0 )
#define S3_BrightBlue     S3_Color( 0.0, 0.0, 1.0 )
#define S3_BrightMagenta  S3_Color( 1.0, 0.0, 1.0 )

#define S3_LightGrey      S3_Color( 0.8, 0.8, 0.8 )
#define S3_LightRed       S3_Color( 0.8, 0.0, 0.0 )
#define S3_LightOrange    S3_Color( 0.8, 0.4, 0.0 )
#define S3_LightYellow    S3_Color( 0.8, 0.8, 0.0 )
#define S3_LightGreen     S3_Color( 0.0, 0.8, 0.0 )
#define S3_LightCyan      S3_Color( 0.0, 0.8, 0.8 )
#define S3_LightBlue      S3_Color( 0.0, 0.0, 0.8 )
#define S3_LightMagenta   S3_Color( 0.8, 0.0, 0.8 )

#define S3_Grey           S3_Color( 0.6, 0.6, 0.6 )
#define S3_Red            S3_Color( 0.6, 0.0, 0.0 )
#define S3_Orange         S3_Color( 0.6, 0.3, 0.0 )
#define S3_Yellow         S3_Color( 0.6, 0.6, 0.0 )
#define S3_Green          S3_Color( 0.0, 0.6, 0.0 )
#define S3_Cyan           S3_Color( 0.0, 0.6, 0.6 )
#define S3_Blue           S3_Color( 0.0, 0.0, 0.6 )
#define S3_Magenta        S3_Color( 0.6, 0.0, 0.6 )

#define S3_DarkGrey       S3_Color( 0.4, 0.4, 0.4 )
#define S3_DarkRed        S3_Color( 0.4, 0.0, 0.0 )
#define S3_DarkOrange     S3_Color( 0.4, 0.2, 0.0 )
#define S3_DarkYellow     S3_Color( 0.4, 0.4, 0.0 )
#define S3_DarkGreen      S3_Color( 0.0, 0.4, 0.0 )
#define S3_DarkCyan       S3_Color( 0.0, 0.4, 0.4 )
#define S3_DarkBlue       S3_Color( 0.0, 0.0, 0.4 )
#define S3_DarkMagenta    S3_Color( 0.4, 0.0, 0.4 )

#define S3_Black          S3_Color( 0.0, 0.0, 0.0 )

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#include <math.h>
#include "common/s3_math.h"

//----------

S3_Color::S3_Color() {
}

S3_Color::S3_Color(float ac) {
  set(ac,ac,ac);
}

S3_Color::S3_Color(float ar, float ag, float ab) {
  set(ar,ag,ab);
}

S3_Color::S3_Color(float ar, float ag, float ab, float aa) {
  set(ar,ag,ab,aa);
}

void S3_Color::set(float ac) {
  r = ac;
  g = ac;
  b = ac;
  a = 1; // 0
}

void S3_Color::set(float ar, float ag, float ab) {
  r = ar;
  g = ag;
  b = ab;
  a = 1; // 0
}

void S3_Color::set(float ar, float ag, float ab, float aa) {
  r = ar;
  g = ag;
  b = ab;
  a = aa;
}

// value = 0 = src
// value = 1 = arg

void S3_Color::blend(float value, float ar, float ag, float ab/*, float aa*/) {
  r = (r * (1-value)) + (ar * value);
  g = (g * (1-value)) + (ag * value);
  b = (b * (1-value)) + (ab * value);
  //a = (a * (1-value)) + (aa * value);
  //a = 1;
}

void S3_Color::blend(float value, S3_Color AColor) {
  blend(value,AColor.r,AColor.g,AColor.b/*,AColor.a*/);
}

//----------------------------------------------------------------------
// rgba
//----------------------------------------------------------------------

//uint32 rgba(KColor AColor) {
//  float bb = AColor.b * 255.0;
//  float gg = AColor.g * 255.0;
//  float rr = AColor.r * 255.0;
//  float aa = AColor.a * 255.0;
//  uint32 b = (uint32)trunc(bb);
//  uint32 g = (uint32)trunc(gg);
//  uint32 r = (uint32)trunc(rr);
//  uint32 a = (uint32)trunc(aa);
//  uint32  res = (b) + (g<<8) + (r<<16) + (a<<24);
//  return res;
//}

//uint32 SRgb(S3_Color AColor) {
//  return ((int)(AColor.r*255) << 16)
//       + ((int)(AColor.g*255) << 8)
//       +  (int)(AColor.b*255);
//}

//----------------------------------------------------------------------
// rgb
//----------------------------------------------------------------------

//TODO: maybe rgb/bgr should be swapped? (little-endian..)

//uint32 S3_BGR(uint8 r, uint8 g, uint8 b, uint8 a=255) {
//  return   0//((uint32)a<<24)
//         + ((uint32)r<<16)
//         + ((uint32)g<<8)
//         + b;
//}

//uint32 S3_BGR(S3_Color AColor) {
//  return S3_BGR(AColor.r*255,AColor.g*255,AColor.b*255,AColor.a*255);
//}

uint32 S3_BGR(uint8 r, uint8 g, uint8 b, uint8 a) {
  return   ((uint32)a<<24)
         + ((uint32)r<<16)
         + ((uint32)g<<8)
         + b;
}

uint32 S3_BGR(S3_Color AColor) {
  return S3_BGR(AColor.r*255,AColor.g*255,AColor.b*255,AColor.a*255);
}


//----------------------------------------------------------------------
// bgr
//----------------------------------------------------------------------

//uint32 S3_RGB(uint8 r, uint8 g, uint8 b/*, uint8 a*/) {
//  return  ((uint32)b<<16)
//        + ((uint32)g<<8)
//        + r;
//}

uint32 S3_RGB(uint8 r, uint8 g, uint8 b, uint8 a) {
  return  ((uint32)a<<24)
        + ((uint32)b<<16)
        + ((uint32)g<<8)
        + r;
}

uint32 S3_RGB(S3_Color AColor) {
  uint32 col = S3_RGB(AColor.r*255,AColor.g*255,AColor.b*255,AColor.a*255);
  //S3_Trace("%8x\n",col);
  return col;
}

//----------------------------------------------------------------------
// utils
//----------------------------------------------------------------------

uint8 S3_AlphaColor(uint32 c, uint32 a) {
  uint32 ret = (c*a) >> 8;
  return ret & 0xff;
}

//----------

uint8 S3_ScaleColor(uint8 c, float n ) {
  float nc = n * (float)c;
  return (uint8)S3_Min(255,nc);
}

//----------

uint32 S3_AlphaRgb(uint32 c, uint8 a) {
  uint32 r = (c & 0xff0000) >> 16;
  uint32 g = (c & 0x00ff00) >> 8;
  uint32 b = (c & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  return S3_RGB(r,g,b);
}

//----------

uint32 S3_AlphaMix(uint32 c1, uint32 c2, uint8 a) {
  uint32 r = (c1 & 0xff0000) >> 16;
  uint32 g = (c1 & 0x00ff00) >> 8;
  uint32 b = (c1 & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c1 = S3_RGB(r,g,b);
  a = 255-a;
  r = (c2 & 0xff0000) >> 16;
  g = (c2 & 0x00ff00) >> 8;
  b = (c2 & 0x0000ff);
  r = (r*a) >> 8;
  g = (g*a) >> 8;
  b = (b*a) >> 8;
  c2 = S3_BGR(r,g,b);
  return c1+c2;
}

// values = 0..1

S3_Color S3_BlendColor(float value, float r1, float g1, float b1, float r2, float g2, float b2) {
  S3_Color col;
  col.r = (r1 * (1.0f-value)) + (r2 * value);
  col.g = (g1 * (1.0f-value)) + (g2 * value);
  col.b = (b1 * (1.0f-value)) + (b2 * value);
  col.a = 1.0f;
  return col;
}

S3_Color S3_BlendColor(float value, S3_Color AColor1, S3_Color AColor2) {
  return S3_BlendColor(value,AColor1.r,AColor1.g,AColor1.b,AColor2.r,AColor2.g,AColor2.b);
}

//----------------------------------------------------------------------

/*

int S3_Blend(unsigned char result[4], unsigned char fg[4], unsigned char bg[4]) {
  unsigned int alpha = fg[3] + 1;
  unsigned int inv_alpha = 256 - fg[3];
  result[0] = (unsigned char)((alpha * fg[0] + inv_alpha * bg[0]) >> 8);
  result[1] = (unsigned char)((alpha * fg[1] + inv_alpha * bg[1]) >> 8);
  result[2] = (unsigned char)((alpha * fg[2] + inv_alpha * bg[2]) >> 8);
  result[3] = 0xff;
}

unsigned int S3_BlendPreMulAlpha(unsigned int colora, unsigned int colorb, unsigned int alpha) {
  unsigned int rb = (colora & 0xFF00FF) + (alpha * (colorb & 0xFF00FF)) >> 8;
  unsigned int g = (colora & 0x00FF00) + (alpha * (colorb & 0x00FF00)) >> 8;
  return (rb & 0xFF00FF) + (g & 0x00FF00);
}

// 0 <= alpha <= 0x100
unsigned int S3_BlendAlpha(unsigned int colora, unsigned int colorb, unsigned int alpha) {
  unsigned int rb1 = ((0x100 - alpha) * (colora & 0xFF00FF)) >> 8;
  unsigned int rb2 = (alpha * (colorb & 0xFF00FF)) >> 8;
  unsigned int g1  = ((0x100 - alpha) * (colora & 0x00FF00)) >> 8;
  unsigned int g2  = (alpha * (colorb & 0x00FF00)) >> 8;
  return ((rb1 | rb2) & 0xFF00FF) + ((g1 | g2) & 0x00FF00);
}

Pixel S3_AlphaBlendPixels(Pixel p1, Pixel p2) {
  static const int AMASK = 0xFF000000;
  static const int RBMASK = 0x00FF00FF;
  static const int GMASK = 0x0000FF00;
  static const int AGMASK = AMASK | GMASK;
  static const int ONEALPHA = 0x01000000;
  unsigned int a = (p2 & AMASK) >> 24;
  unsigned int na = 255 - a;
  unsigned int rb = ((na * (p1 & RBMASK)) + (a * (p2 & RBMASK))) >> 8;
  unsigned int ag = (na * ((p1 & AGMASK) >> 8)) + (a * (ONEALPHA | ((p2 & GMASK) >> 8)));
  return ((rb & RBMASK) | (ag & AGMASK));
}

unsigned S3_Blend2(unsigned src, unsigned dst) {
  unsigned alpha = src >> 24;
  alpha += (alpha > 0);
  unsigned srb = src & 0xff00ff;
  unsigned sg = src & 0x00ff00;
  unsigned drb = dst & 0xff00ff;
  unsigned dg = dst & 0x00ff00;
  unsigned orb = (drb + (((srb - drb) * alpha + 0x800080) >> 8)) & 0xff00ff;
  unsigned og = (dg + (((sg - dg ) * alpha + 0x008000) >> 8)) & 0x00ff00;
  return orb+og;
}

inline void rgb_to_ycbcr(const unsigned int& length, double* red, double* green, double* blue,
                                                     double* y,   double* cb,    double* cr)
{
  unsigned int i = 0;
  while (i < length)
  {
    ( *y) =   16.0 + (  65.481 * (*red) +  128.553 * (*green) +  24.966 * (*blue));
    (*cb) =  128.0 + ( -37.797 * (*red) +  -74.203 * (*green) + 112.000 * (*blue));
    (*cr) =  128.0 + ( 112.000 * (*red) +  -93.786 * (*green) -  18.214 * (*blue));
    ++i;
    ++red; ++green; ++blue;
    ++y;   ++cb;    ++cr;
  }
}

inline void ycbcr_to_rgb(const unsigned int& length, double* y,   double* cb,    double* cr,
                                                     double* red, double* green, double* blue)
{
  unsigned int i = 0;
  while (i < length)
  {
    double y_  =  (*y) -  16.0;
    double cb_ = (*cb) - 128.0;
    double cr_ = (*cr) - 128.0;
    (*red) = 0.000456621 * y_                    + 0.00625893 * cr_;
    (*green) = 0.000456621 * y_ - 0.00153632 * cb_ - 0.00318811 * cr_;
    (*blue) = 0.000456621 * y_                    + 0.00791071 * cb_;
    ++i;
    ++red; ++green; ++blue;
    ++y;   ++cb;    ++cr;
  }
}

*/

//----------------------------------------------------------------------
#endif


