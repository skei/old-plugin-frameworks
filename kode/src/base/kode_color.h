#ifndef kode_color_included
#define kode_color_included
//----------------------------------------------------------------------

#include "kode.h"

struct KODE_Color {

  float r,g,b,a;

  /*
  KODE_Color();
  KODE_Color(float v);
  KODE_Color(float ar, float ag, float ab, float aa=1.0f);
  KODE_Color KODE_BlendColor(float value, float r1, float g1, float b1, float r2, float g2, float b2);
  KODE_Color KODE_BlendColor(float value, KODE_Color AColor1, KODE_Color AColor2);
  */

  KODE_Color() {
  }

  //----------

  KODE_Color(float v) {
    r = v;
    g = v;
    b = v;
    a = 1.0f;
  }

  //----------

  KODE_Color(float ar, float ag, float ab, float aa=1.0f) {
    r = ar;
    g = ag;
    b = ab;
    a = aa;
  }

  //----------

//  /*
//    used in:
//      KXlibPainter.fillRectangleGradient
//  */
//
//  // values = 0..1
//
//  KODE_Color KODE_BlendColor(float value, float r1, float g1, float b1, float r2, float g2, float b2) {
//    KODE_Color col;
//    col.r = (r1 * (1.0f-value)) + (r2 * value);
//    col.g = (g1 * (1.0f-value)) + (g2 * value);
//    col.b = (b1 * (1.0f-value)) + (b2 * value);
//    col.a = 1.0f;
//    return col;
//  }
//
//  //----------
//
//  KODE_Color KODE_BlendColor(float value, KODE_Color AColor1, KODE_Color AColor2) {
//    return KODE_BlendColor(value,AColor1.r,AColor1.g,AColor1.b,AColor2.r,AColor2.g,AColor2.b);
//  }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

  /*
    used in:
      KXlibPainter.fillRectangleGradient
  */

  // values = 0..1

  KODE_Color KODE_BlendColor(float value, float r1, float g1, float b1, float r2, float g2, float b2) {
    KODE_Color col;
    col.r = (r1 * (1.0f-value)) + (r2 * value);
    col.g = (g1 * (1.0f-value)) + (g2 * value);
    col.b = (b1 * (1.0f-value)) + (b2 * value);
    col.a = 1.0f;
    return col;
  }

  //----------

  KODE_Color KODE_BlendColor(float value, KODE_Color AColor1, KODE_Color AColor2) {
    return KODE_BlendColor(value,AColor1.r,AColor1.g,AColor1.b,AColor2.r,AColor2.g,AColor2.b);
  }

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_White          KODE_Color( 1.0, 1.0, 1.0 )

#define KODE_BrightGrey     KODE_Color( 1.0, 1.0, 1.0 )
#define KODE_BrightRed      KODE_Color( 1.0, 0.0, 0.0 )
#define KODE_BrightOrange   KODE_Color( 1.0, 0.5, 0.0 )
#define KODE_BrightYellow   KODE_Color( 1.0, 1.0, 0.0 )
#define KODE_BrightGreen    KODE_Color( 0.0, 1.0, 0.0 )
#define KODE_BrightCyan     KODE_Color( 0.0, 1.0, 1.0 )
#define KODE_BrightBlue     KODE_Color( 0.0, 0.0, 1.0 )
#define KODE_BrightMagenta  KODE_Color( 1.0, 0.0, 1.0 )

#define KODE_LightGrey      KODE_Color( 0.8, 0.8, 0.8 )
#define KODE_LightRed       KODE_Color( 0.8, 0.0, 0.0 )
#define KODE_LightOrange    KODE_Color( 0.8, 0.4, 0.0 )
#define KODE_LightYellow    KODE_Color( 0.8, 0.8, 0.0 )
#define KODE_LightGreen     KODE_Color( 0.0, 0.8, 0.0 )
#define KODE_LightCyan      KODE_Color( 0.0, 0.8, 0.8 )
#define KODE_LightBlue      KODE_Color( 0.0, 0.0, 0.8 )
#define KODE_LightMagenta   KODE_Color( 0.8, 0.0, 0.8 )

#define KODE_Grey           KODE_Color( 0.6, 0.6, 0.6 )
#define KODE_Red            KODE_Color( 0.6, 0.0, 0.0 )
#define KODE_Orange         KODE_Color( 0.6, 0.3, 0.0 )
#define KODE_Yellow         KODE_Color( 0.6, 0.6, 0.0 )
#define KODE_Green          KODE_Color( 0.0, 0.6, 0.0 )
#define KODE_Cyan           KODE_Color( 0.0, 0.6, 0.6 )
#define KODE_Blue           KODE_Color( 0.0, 0.0, 0.6 )
#define KODE_Magenta        KODE_Color( 0.6, 0.0, 0.6 )

#define KODE_DarkGrey       KODE_Color( 0.4, 0.4, 0.4 )
#define KODE_DarkRed        KODE_Color( 0.4, 0.0, 0.0 )
#define KODE_DarkOrange     KODE_Color( 0.4, 0.2, 0.0 )
#define KODE_DarkYellow     KODE_Color( 0.4, 0.4, 0.0 )
#define KODE_DarkGreen      KODE_Color( 0.0, 0.4, 0.0 )
#define KODE_DarkCyan       KODE_Color( 0.0, 0.4, 0.4 )
#define KODE_DarkBlue       KODE_Color( 0.0, 0.0, 0.4 )
#define KODE_DarkMagenta    KODE_Color( 0.4, 0.0, 0.4 )

#define KODE_Black          KODE_Color( 0.0, 0.0, 0.0 )

//----------------------------------------------------------------------
#endif