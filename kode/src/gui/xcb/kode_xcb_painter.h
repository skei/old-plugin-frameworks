#ifndef kode_xcb_painter_included
#define kode_xcb_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/kode_rgba.h"
#include "base/kode_stack.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_painter.h"
#include "gui/xcb/kode_xcb.h"
#include "gui/xcb/kode_xcb_utils.h"

typedef KODE_Stack<KODE_Rect,16> KODE_RectStack;

//----------------------------------------------------------------------

class KODE_XcbPainter
: public KODE_BasePainter {

//------------------------------
private:
//------------------------------

  xcb_gcontext_t    MGC           = XCB_NONE;
  xcb_connection_t* MConnection   = nullptr;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  KODE_Drawable*    MTarget       = nullptr;
  int32_t           MFontAscent   = 0;
  int32_t           MFontDescent  = 0;
  int32_t           MFontWidth    = 0;
  int32_t           MFontHeight   = 0;
  int32_t           MFontOrigin   = 0;
  int32_t           MFontLeft     = 0;
  int32_t           MFontRight    = 0;
  KODE_Color        MDrawColor;//= KODE_LightGrey;
  KODE_Color        MFillColor;//= KODE_DarkGrey;
  KODE_Color        MTextColor;//= KODE_Black;
  
  //bool              MHasClipRect  = false;
  KODE_Rect         MClipRect     = KODE_Rect(0);
  KODE_RectStack    MClipStack;

//------------------------------
public:
//------------------------------

  KODE_XcbPainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {
    //XCB_Trace("KODE_XcbPainter\n");
    MTarget     = ATarget;
    MConnection = ATarget->getConnection();
    MDrawable   = ATarget->getDrawable();
    MGC         = xcb_generate_id(MConnection);
    uint32_t mask =
      //XCB_GC_FUNCTION
      //XCB_GC_PLANE_MASK
      //XCB_GC_FOREGROUND
      //XCB_GC_BACKGROUND
      //XCB_GC_LINE_WIDTH
      //XCB_GC_LINE_STYLE
      //XCB_GC_CAP_STYLE
      //XCB_GC_JOIN_STYLE
      //XCB_GC_FILL_STYLE
      //XCB_GC_FILL_RULE
      //XCB_GC_TILE
      //XCB_GC_STIPPLE
      //XCB_GC_TILE_STIPPLE_ORIGIN_X
      //XCB_GC_TILE_STIPPLE_ORIGIN_Y
      //XCB_GC_FONT |
      //XCB_GC_SUBWINDOW_MODE
      XCB_GC_GRAPHICS_EXPOSURES;
      //XCB_GC_CLIP_ORIGIN_X
      //XCB_GC_CLIP_ORIGIN_Y
      //XCB_GC_CLIP_MASK
      //XCB_GC_DASH_OFFSET
      //XCB_GC_DASH_LIST
      //XCB_GC_ARC_MODE
    uint32_t values[] = {
      0
    };
    xcb_create_gc(MConnection,MGC,MDrawable,mask,values);
    xcb_flush(MConnection);
    
    //MHasCLipRect = false;
    MClipRect = KODE_Rect( 0,0,ATarget->getWidth(),ATarget->getHeight() );
    
  }

  //----------

  virtual ~KODE_XcbPainter() {
    xcb_free_gc(MConnection,MGC);
  }

public:

  KODE_Drawable* getTarget() override {
    return MTarget;
  }

//------------------------------
private:
//------------------------------

  /*
  none of these flush..
  caller do the flushing when/if necessary
  */

  // color

  void _setForegroundColor(KODE_Color AColor) {
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = KODE_BGRA(AColor);
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void _setBackgroundColor(KODE_Color AColor) {
    uint32_t mask = XCB_GC_BACKGROUND;
    uint32_t values[1];
    values[0] = KODE_BGRA(AColor);
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  // font/text
  // list available fonts: xlsfonts

  /*
  void _openFont(const char* AName) {
    closeFont();
    MFont = xcb_generate_id(MConnection);
    xcb_open_font(
      MConnection,
      MFont, //font,
      KStrlen(AName),
      AName
    );
  }
  */

  //----------

  /*
  void _closeFont(void) {
    if (MFont) {
      xcb_close_font(MConnection,MFont);
    }
  }
  */

  //----------

  /*
  void _selectFont(const char* AName) {
    openFont(AName);
    uint32_t mask = XCB_GC_FONT;
    uint32_t values[1];
    values[0] = MFont;
    xcb_change_gc(MConnection, MGC, mask, values);
  }
  */

  //----------

  // taken from http://lists.freedesktop.org/archives/xcb/2009-April/004611.html
  // but https://bitbucket.org/ryanflannery/xtabs/src/4a36b745532d3a75f98ea115ceaabfac839fb8bc/xutil.c

  /*
  typedef struct xcb_char2b_t {
    uint8_t byte1;
    uint8_t byte2;
  } xcb_char2b_t;
  */

  xcb_char2b_t* _buildChars(const char *str, size_t length) {
    unsigned int i;
    xcb_char2b_t* ret = (xcb_char2b_t *)KODE_Malloc(length * sizeof(xcb_char2b_t));
    if (!ret) return nullptr;
    for (i = 0; i < length; i++) {
      ret[i].byte1 = 0;
      ret[i].byte2 = str[i];
    }
    return ret;
  }

  //----------

  // for text output..
  // man xcb_query_text_extents
  // http://www.x.org/releases/current/doc/xproto/x11protocol.html#requests:QueryFont
  // http://www.x.org/releases/current/doc/xproto/x11protocol.html#requests:ImageText8

  void _measureString(const char *string) {
    //xcb_char2b_t* xcb_str = buildChars(string, KODE_Strlen(string));
    xcb_char2b_t xcb_str[256];
    for (uint32_t i = 0; i < KODE_Strlen(string); i++) {
      xcb_str[i].byte1 = 0;
      xcb_str[i].byte2 = string[i];
    }
    xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(MConnection, MGC/*MFont*/, KODE_Strlen(string), xcb_str);
    xcb_query_text_extents_reply_t* reply = xcb_query_text_extents_reply(MConnection, cookie, NULL);
    MFontAscent   = reply->font_ascent;
    MFontDescent  = reply->font_descent;
    MFontWidth    = reply->overall_width;
    MFontHeight   = reply->font_ascent + reply->font_descent;
    MFontOrigin   = reply->font_ascent;
    MFontLeft     = reply->overall_left;
    MFontRight    = reply->overall_right;
    //MFontOverallAscent = reply->overall_ascent;
    //MFontOverallDescent = reply->overall_descent;
    //KODE_Free(xcb_str);
    KODE_Free(reply);
  }

//------------------------------
public:
//------------------------------

  /*
  */

  void flush() override {
    xcb_flush(MConnection);
    //xcb_aux_sync(MConnection);
  }

  //----------

  /*
  */

  void dirty() override {
  }

  void dirty(float AX1, float AY1, float AX2, float AY2) override {
  }

  void dirty(KODE_Rect ARect) override {
  }

  //----------

  /*
  */

  void finish() override {
  }

//------------------------------
public:
//------------------------------

  void setDrawColor(KODE_Color AColor) override {
    MDrawColor = AColor;
    _setForegroundColor(MDrawColor);
    //setBackgroundColor(MDrawColor);
  }

  //----------

  void setFillColor(KODE_Color AColor) override {
    MFillColor = AColor;
    _setForegroundColor(MFillColor);
    //setBackgroundColor(MFillColor);
  }

  //----------

  void setTextColor(KODE_Color AColor) override {
    MTextColor = AColor;
    _setForegroundColor(MTextColor);
    //setBackgroundColor(MFillColor);
  }

  //----------

  void setPenSize(float ASize) override {
    uint32_t mask = XCB_GC_LINE_WIDTH;
    uint32_t values[1];
    values[0] = ASize;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void setPenDash(double* ADashes, uint32_t ANumDashes, float AOffset) override {
    //uint32_t mask = XCB_GC_DASH_LIST | XCB_GC_DASH_OFFSET;
    //uint32_t values[2];
    //values[0] =
    //values[0] =
    //xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void setLineCap(uint32_t ALineCap) override {
    uint32_t mask = XCB_GC_CAP_STYLE;
    uint32_t values[1];
    values[0] = ALineCap;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void setLineJoin(uint32_t ALineJoin) override {
    uint32_t mask = XCB_GC_JOIN_STYLE;
    uint32_t values[1];
    values[0] = ALineJoin;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void setTextSize(float ASize) override {
  }

  //----------

  /*
    why isn't font closed ???
    look into this!
  */


  void setTextFont(const char* AName, uint32_t ASlant, uint32_t AWeight) override {
    xcb_font_t font = xcb_generate_id(MConnection);
    xcb_open_font(
      MConnection,
      font,
      KODE_Strlen(AName),
      AName
    );
    uint32_t mask = XCB_GC_FONT;
    uint32_t values[1];
    values[0] = font;
    xcb_change_gc(MConnection, MGC, mask, values);
    
    xcb_close_font(MConnection,font);
    
  }

  //----------

  //last_measured
  //last_measured_string_hash

  float getTextWidth(const char* AText) override {
    _measureString(AText);
    return MFontWidth;
  }

  //----------

  float getTextHeight(const char* AText) override {
    _measureString(AText);
    return MFontHeight;
  }

  void drawPoint(float AX, float AY) override {
    xcb_point_t points[] =  {
      (int16_t)AX, (int16_t)AY
    };
    xcb_poly_point(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,1,points);
  }

  //----------

  void drawHLine(float AX1, float AY1, float AX2) override {
    drawLine(AX1,AY1,AX2,AY1);
  }

  //----------

  void drawVLine(float AX1, float AY1, float AY2) override {
    drawLine(AX1,AY1,AX1,AY2);
  }

  //----------

  void drawLine(float AX1, float AY1, float AX2, float AY2) override {
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1,
      (int16_t)AX2, (int16_t)AY2,
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
  }

  //----------

  //void drawLines(int32_t ANum, int32_t* ACoords) override {
  //}

  //----------

  void drawRectangle(float AX1, float AY1, float AX2, float AY2) override {
    xcb_rectangle_t rectangles[] = {{
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2-AX1+0), // +1
      (uint16_t)(AY2-AY1+0)  // +1
    }};
    xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  void drawRectangle(KODE_Rect ARect) override {
    drawRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------

  void drawRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t AC) override {
    float r  = AR;// - 1;
    float r2 = r*2;
    drawArc(  AX1,      AY1,      AX1+r2-2, AY1+r2-3, 0.75, 0.25 ); // upper left
    drawArc(  AX2-r2+1, AY1,      AX2-1,    AY1+r2-2, 0.00, 0.25 ); // upper right
    drawArc(  AX1,      AY2-r2+1, AX1+r2-2, AY2-1,    0.50, 0.25 ); // lower left
    drawArc(  AX2-r2+1, AY2-r2+2, AX2-1,    AY2-1,    0.25, 0.25 ); // lower right
    drawLine( AX1+r,    AY1,      AX2-r,    AY1       );  // top
    drawLine( AX1+r,    AY2,      AX2-r,    AY2       );  // bottom
    drawLine( AX1,      AY1+r,    AX1,      AY2-r     );  // left
    drawLine( AX2,      AY1+r,    AX2,      AY2-r     );  // right
  }

  void drawRoundedRectangle(KODE_Rect ARect, float AR, uint32_t AC) override {
    drawRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR,AC);
  }

  //----------

  void drawEllipse(float AX1, float AY1, float AX2, float AY2) override {
    xcb_arc_t arcs[] = {
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2 - AX1 + 0), // +1
      (uint16_t)(AY2 - AY1 + 0), // +1
      0,
      360 * 64
    };
    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  void drawEllipse(KODE_Rect ARect) override {
    drawEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------

  /*
    angle 1 = start angle, relative to 3 o'clock
    angle 2 = 'distance' 0..1, counter-clockwise
  */

  void drawArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25f;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    //XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
    xcb_arc_t arcs[] = {
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2 - AX1 + 0),  // +1
      (uint16_t)(AY2 - AY1 + 0),  // +1
      (int16_t)(a1 * 360.0f * 64.0f),
      (int16_t)(a2 * 360.0f * 64.0f)
    };
    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  void drawArc(KODE_Rect ARect, float AAngle1, float AAngle2) override {
    drawArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
  }

  //----------

  void drawTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,6,polyline);
  }

  //----------

  void drawCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
  }

  void fillRectangle(float AX1, float AY1, float AX2, float AY2) override {
    xcb_rectangle_t rectangles[] = {{
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2-AX1+1),
      (uint16_t)(AY2-AY1+1)
    }};
    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  void fillRectangle(KODE_Rect ARect) override {
    fillRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------

  void fillRoundedRectangle(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners) override {
    float r  = AR;// - 1;
    float r2 = r * 2;
    fillArc(       AX1-1,  AY1-1,   AX1+r2,   AY1+r2,   0.75, 0.25 ); // upper left
    fillArc(       AX2-r2, AY1-1,   AX2,      AY1+r2-1, 0.00, 0.25 ); // upper right
    fillArc(       AX1-1,  AY2-r2,  AX1+r2-1, AY2,      0.50, 0.25 ); // lower left
    fillArc(       AX2-r2, AY2-r2,  AX2,      AY2,      0.25, 0.25 ); // lower right
    fillRectangle( AX1+r,  AY1,     AX2-r,    AY1+r-1   );  // top
    fillRectangle( AX1,    AY1+r,   AX2,      AY2-r     );  // mid
    fillRectangle( AX1+r,  AY2-r+1, AX2-r,    AY2       );  // bot
  }

  void fillRoundedRectangle(KODE_Rect ARect, float AR, uint32_t ACorners) override {
    fillRoundedRectangle(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR,ACorners);
  }

  //----------

  void fillEllipse(float AX1, float AY1, float AX2, float AY2) override {
    xcb_arc_t arcs[] = {
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2 - AX1 + 1),
      (uint16_t)(AY2 - AY1 + 1),
      (int16_t)(0),
      (int16_t)(360 * 64)
    };
    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  void fillEllipse(KODE_Rect ARect) override {
    fillEllipse(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }
  //----------

  // angle 1 = start angle, relative to 3 o'clock
  // angle 2 = 'distance' 0..1, counter-clockwise

  void fillArc(float AX1, float AY1, float AX2, float AY2, float AAngle1, float AAngle2) override {
    //if (abs(AAngle2) >= 0.01) EPSILON
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    xcb_arc_t arcs[] = {
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2 - AX1 + 1),
      (uint16_t)(AY2 - AY1 + 1),
      (int16_t)(a1 * 360 * 64),
      (int16_t)(a2 * 360 * 64)
    };
    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
    //}
  }

  void fillArc(KODE_Rect ARect, float AAngle1, float AAngle2) override {
    fillArc(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AAngle1,AAngle2);
  }

  //----------

  void fillTriangle(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3) override {
    xcb_point_t polyline[] =  {
      (int16_t)AX1, (int16_t)AY1, (int16_t)AX2, (int16_t)AY2,
      (int16_t)AX2, (int16_t)AY2, (int16_t)AX3, (int16_t)AY3,
      (int16_t)AX3, (int16_t)AY3, (int16_t)AX1, (int16_t)AY1,
    };
    xcb_fill_poly(MConnection,MDrawable,MGC,XCB_POLY_SHAPE_CONVEX,XCB_COORD_MODE_ORIGIN,6,polyline);
  }

  //----------

  //void fillPolygon(int32_t ANum, int32_t* ACoords) override {
  //}

  //----------

  //void fillCurve(float AX1, float AY1, float AX2, float AY2, float AX3, float AY3, float AX4, float AY4) override {
  //}
  //
  //void fillCurve(KPoint AP1, KPoint AP2, KPoint AP3, KPoint AP4) override {
  //  fillCurve(AP1.x,AP1.y,AP2.x,AP2.y,AP3.x,AP3.y,AP4.x,AP4.y);
  //}

  //----------

  /*
  void fillRectangleGradient(float AX1, float AY1, float AX2, float AY2, KODE_Color AColor1, KODE_Color AColor2, bool AVertical) override {
    if (AVertical) {
      float h = AY2-AY1+1;
      if (h>0) {
        float v = 0;
        float vadd = 1.0f / (float)(h-1);
        for (uint32_t i=0; i<h; i++) {
          KODE_Color c = KODE_BlendColor(v,AColor1,AColor2);
          v += vadd;
          c.a = 1;
          setDrawColor(c);
          drawLine(AX1,AY1+i,AX2,AY1+i);

        } // for i
      } // h>0
    } // vertical
    else {
      float w = AX2-AX1+1;
      if (w>0) {
        float v = 0;
        float vadd = 1.0f / (float)(w-1);
        for (uint32_t i=0; i<w; i++) {
          KODE_Color c = KODE_BlendColor(v,AColor1,AColor2);
          v += vadd;
          c.a = 1;
          setDrawColor(c);
          drawLine(AX1+i,AY1,AX1+i,AY2);
        } // for i
      } // h>0
    }
  }
  */

  /*void fillRectangleGradient(KODE_Rect ARect, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) override {
    fillRectangleGradient(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AColor1,AColor2,AVertical);
  }
  */

  //----------

  /*
  void fillRoundedRectangleGradient(float AX1, float AY1, float AX2, float AY2, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) override {
  }
  */

  //----------

  /*void fillRoundedRectangleGradient(KODE_Rect ARect, float AR, uint32_t ACorners, KODE_Color AColor1, KODE_Color AColor2, bool AVertical=true) override {
    fillRoundedRectangleGradient(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AR, ACorners,AColor1,AColor2,AVertical);
  }
  */

  //----------

  void drawText(float AX, float AY, const char* AText) override {
    //uint8_t* buffer = (uint8_t*)realloc(pt->data, pt->used + required);
    //if(!buffer) return;
    //pt->data = buffer;
    uint8_t buffer[512];
    KODE_XcbPolyText8 pt;
    pt.data = buffer;//nullptr;//(uint8_t*)KMalloc(1024);
    pt.used = 0;
    kode_xcb_add_string_text8(&pt,AText);
    //uint8_t* buffer = (uint8_t*)realloc(pt->data, pt->used + required);
    //if(!buffer) return;
    //pt->data = buffer;
    xcb_poly_text_8(MConnection,MDrawable,MGC,AX,AY,pt.used,pt.data);
    //if (pt.data) KODE_Free(pt.data);
  }

  //void drawText(KPoint APos, const char* AText) override {
  //  drawText(APos.x,APos.y,AText);
  //}

  //----------

  void drawText(float AX1, float AY1, float AX2, float AY2, const char* AText, uint32_t AAlign) override {
    _measureString(AText);
    //int32_t x,y,w;
    float x,y,w;
    if (AAlign & KODE_TEXT_ALIGN_TOP) y = AY1 + MFontAscent;//MFontStruct->ascent;
    else if (AAlign & KODE_TEXT_ALIGN_BOTTOM) y = AY2 - MFontDescent;//MFontStruct->descent;
    else y = AY1 + (/*MFontStruct->ascent*/MFontAscent * 0.5f) + ((AY2-AY1) * 0.5f );

    w = MFontWidth;

    if (AAlign & KODE_TEXT_ALIGN_LEFT) x = AX1;
    else if (AAlign & KODE_TEXT_ALIGN_RIGHT) x = AX2 - w;
    else x = AX1 + ((AX2 - AX1) * 0.5f) - ( w * 0.5f );
    drawText(x,y,AText);
  }

  void drawText(KODE_Rect ARect, const char* AText, uint32_t AAlign) override {
    drawText(ARect.x,ARect.y,ARect.x2(),ARect.y2(),AText,AAlign);
  }
  
  //----------

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource) override {
    drawBitmap(ADstX,ADstY,ASource,0,0,ASource->getWidth(),ASource->getHeight());
  }

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) override {
    if (ASource->isImage()) {
      /*
      xcb_put_image(
        MConnection,
        XCB_IMAGE_FORMAT_Z_PIXMAP,
        MDrawable,//ASource->getDrawable(),
        MGC,
        ASrcW,
        ASrcH,
        ADstX,
        ADstY,
        0,                              // left_pat
        32,                             // depth
        ASource->getStride() *ASrcH,    // data_len
        (uint8_t *)ASource->getBuffer() // data
      );
      */
      xcb_image_put (
        MConnection,            // xcb_connection_t *  conn,
        MDrawable,              // xcb_drawable_t      draw,
        MGC,                    // xcb_gcontext_t      gc,
        ASource->getImage(),    // xcb_image_t *       image,
        ADstX,                  // int16_t             x,
        ADstY,                  // int16_t             y,
        0                       // uint8_t             left_pad
      );
    }
    else if (ASource->isSurface()) {
      xcb_copy_area(
        MConnection,            // Pointer to the xcb_connection_t structure
        ASource->getDrawable(), // The Drawable we want to paste
        MDrawable,              // The Drawable on which we copy the previous Drawable
        MGC,                    // A Graphic Context
        ASrcX,                  // Top left x coordinate of the region we want to copy
        ASrcY,                  // Top left y coordinate of the region we want to copy
        ADstX,                  // Top left x coordinate of the region where we want to copy
        ADstY,                  // Top left y coordinate of the region where we want to copy
        ASrcW,                  // Width                 of the region we want to copy
        ASrcH                   // Height of the region we want to copy
      );
    }
  }

  void drawBitmap(float ADstX, float ADstY, KODE_Drawable* ASource, KODE_Rect ASrc) override {
    drawBitmap(ADstX,ADstY,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
  }

  //void drawBitmap(KPoint ADst, KDrawable* ASource, KRect ASrc) override {
  //  drawBitmap(ADst.x,ADst.y,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
  //}

  //----------

  // scaled

  void drawBitmap(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) override {
  }

  void drawBitmap(KODE_Rect ADst, KODE_Drawable* ASource, KODE_Rect ASrc) override {
    drawBitmap(ADst.x,ADst.y,ADst.w,ADst.h,ASource,ASrc.x,ASrc.y,ASrc.w,ASrc.h);
  }

  /*
    If clip-mask is None, then pixels are always drawn, regardless of the
    clip origin.
    The clip-mask can also be set with the SetClipRectangles request.
  */

  //------------------------------

  void clip(float AX1, float AY1, float AX2, float AY2) override {
    //resetClip();
    xcb_rectangle_t rectangles[] = {{
      (int16_t)AX1,
      (int16_t)AY1,
      (uint16_t)(AX2-AX1+1),
      (uint16_t)(AY2-AY1+1)
    }};
    xcb_set_clip_rectangles(
      MConnection,
      XCB_CLIP_ORDERING_UNSORTED, // ordering,
      MGC,
      0, // clip_x_origin
      0, // clip_y_origin
      1, //rectangles_len
      rectangles
    );
    xcb_flush(MConnection);
  }
  
  //
  
  void clip(KODE_Rect ARect) override {
    clip(ARect.x,ARect.y,ARect.x2(),ARect.y2());
  }

  //----------

  void resetClip(void) override {
    uint32_t mask = XCB_GC_CLIP_MASK;
    uint32_t values[1];
    values[0] = XCB_NONE;
    xcb_change_gc(MConnection, MGC, mask, values);
    xcb_flush(MConnection);
  }
  
  //------------------------------

  void pushClip(KODE_Rect ARect) override {
    //KODE_Trace("%.0f, %.0f %.0f %.0f -> %.0f, %.0f %.0f %.0f\n",MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h,ARect.x,ARect.y,ARect.w,ARect.h);
    if (ARect.isEmpty()) {
      MClipRect = ARect;
      resetClip();
    }
    else {
      MClipStack.push(MClipRect);
      MClipRect = ARect;
      resetClip();
      clip(MClipRect);
    }
  }
  
  //

  void pushClip(float AX1, float AY1, float AX2, float AY2) override {
    pushClip( KODE_Rect(AX1,AY1,AX2-AX1+1,AY2-AY1+1) );
  }
  //----------

  void popClip() override {
    //KODE_Rect r = MClipRect;
    MClipRect = MClipStack.pop();
    if (MClipRect.isNotEmpty()) {
      //KODE_Trace("%.0f, %.0f %.0f %.0f -> %.0f, %.0f %.0f %.0f\n",r.x,r.y,r.w,r.h,MClipRect.x,MClipRect.y,MClipRect.w,MClipRect.h);
      resetClip();
      clip(MClipRect);
    }
    else {
      //KODE_Trace("empty\n");
      resetClip();
    }
  }

  //----------
  
  KODE_Rect getClipRect() override {
    return MClipRect;
  }
  
};

//----------------------------------------------------------------------
#endif
