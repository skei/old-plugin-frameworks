#ifndef s3_rect_included
#define s3_rect_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include "common/s3_point.h"

struct S3_Rect {
  int32 x,y,w,h;
  S3_Rect();
  S3_Rect(int32 i);
  S3_Rect(int32 AW, int32 AH);
  S3_Rect(int32 AX, int32 AY, int32 AW, int32 AH);
  void      set(int32 i);
  void      set(int32 AW, int32 AH);
  void      set(int32 AX, int32 AY, int32 AW, int32 AH);
  void      set(S3_Rect ARect);
  void      pos(int32 AX, int32 AY);
  void      size(int32 AW, int32 AH);
  int32     x2(void);
  int32     y2(void);
  void      x2(int32 i);
  void      y2(int32 i);
  void      add(int32 AX, int32 AY);
  void      add(int32 AX, int32 AY, int32 AW, int32 AH);
  S3_Point  pos(void);
  bool      isEmpty(void);
  bool      isNotEmpty(void);
  bool      contains(int32 AX, int32 AY);
  void      combine(int32 AX, int32 AY, int32 AW, int32 AH);
  void      combine(S3_Rect ARect);
  bool      intersects(int32 AX, int32 AY, int32 AW, int32 AH);
  bool      intersects(S3_Rect ARect);

  void      grow(int32 AValue);
  void      shrink(int32 AValue);
  void      scale(float AFactor);

  void      grow(int32 AXValue, int32 AYValue);
  void      shrink(int32 AXValue, int32 AYValue);
  void      scale(float AXFactor, float AYFactor);

  //void      crop(int32 AX, int32 AY, int32 AW, int32 AH);
  //void      intersection(int32 AX, int32 AY, int32 AW, int32 AH);
  //void      intersection(S3_Rect ARect);
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//#include "common/s3_array.h"
#include "common/s3_point.h"

/*
  union {
    int32 x,y,w,h;
    int32 data[4];
  };
*/

//----------------------------------------------------------------------

S3_Rect::S3_Rect() {
  x = 0;
  y = 0;
  w = 0;
  h = 0;
}

S3_Rect::S3_Rect(int32 i) {
  x = i;
  y = i;
  w = i;
  h = i;
}

S3_Rect::S3_Rect(int32 AW, int32 AH) {
  x = 0;
  y = 0;
  w = AW;
  h = AH;
}

S3_Rect::S3_Rect(int32 AX, int32 AY, int32 AW, int32 AH) {
  x = AX;
  y = AY;
  w = AW;
  h = AH;
}

void S3_Rect::set(int32 i) {
  x = i;
  y = i;
  w = i;
  h = i;
}

void S3_Rect::set(int32 AW, int32 AH) {
  x = 0;
  y = 0;
  w = AW;
  h = AH;
}

void S3_Rect::set(int32 AX, int32 AY, int32 AW, int32 AH) {
  x = AX;
  y = AY;
  w = AW;
  h = AH;
}

void S3_Rect::set(S3_Rect ARect) {
  x = ARect.x;
  y = ARect.y;
  w = ARect.w;
  h = ARect.h;
}

void S3_Rect::pos(int32 AX, int32 AY) {
  x = AX;
  y = AY;
}

void S3_Rect::size(int32 AW, int32 AH) {
  w = AW;
  h = AH;
}

int32 S3_Rect::x2(void)  {
  return x + w - 1;
}

int32 S3_Rect::y2(void)  {
  return y + h - 1;
}

void S3_Rect::x2(int32 i) {
  w = i - x + 1;
}

void S3_Rect::y2(int32 i) {
  h = i - y + 1;
}

void S3_Rect::add(int32 AX, int32 AY) {
  x += AX;
  y += AY;
}

void S3_Rect::add(int32 AX, int32 AY, int32 AW, int32 AH) {
  x += AX;
  y += AY;
  w += AW;
  h += AH;
}

S3_Point S3_Rect::pos(void) {
  return S3_Point(x,y);
}

bool S3_Rect::isEmpty(void) {
  if ((w>0) && (h>0)) return false;
  else return true;
}

bool S3_Rect::isNotEmpty(void) {
  if ((w>0) && (h>0)) return true;
  else return false;
}

// return true if rect contains the specified point32

bool S3_Rect::contains(int32 AX, int32 AY) {
  if( AX < x ) return false;
  if( AY < y ) return false;
  if( AX >= (x+w) ) return false;
  if( AY >= (y+h) ) return false;
  return true;
}

//  __            _____
// |aa|          :  :  |
// |aa|          :..:  |
//     __    ->  |   ..|
//    |  |       |  :  :
//    |__|       |__:__:
//

// create a rect big enough to hold itself and specified rect

void S3_Rect::combine(int32 AX, int32 AY, int32 AW, int32 AH) {
  if( AX < x ) { w += (x-AX); x=AX; }
  if( AY < y ) { h += (y-AY); y=AY; }
  if( (AX+AW) >= (x+w) ) w = (AX+AW) - x;
  if( (AY+AH) >= (y+h) ) h = (AY+AH) - y;
}

void S3_Rect::combine(S3_Rect ARect) {
  return combine( ARect.x, ARect.y, ARect.w, ARect.h );
}

// return true if rect overlap/touches the specified rect

bool S3_Rect::intersects(int32 AX, int32 AY, int32 AW, int32 AH) {
  if( (AX+AW) < x ) return false;
  if( (AY+AH) < y ) return false;
  if( AX >= (x+w) ) return false;
  if( AY >= (y+h) ) return false;
  return true;
}

bool S3_Rect::intersects(S3_Rect ARect) {
  return intersects( ARect.x, ARect.y, ARect.w, ARect.h );
}

//----------------------------------------------------------------------


void S3_Rect::grow(int32 AValue) {
  x -= AValue;
  y -= AValue;
  w += AValue*2;
  h += AValue*2;
}

void S3_Rect::shrink(int32 AValue) {
  x += AValue;
  y += AValue;
  w -= AValue*2;
  h -= AValue*2;
}

void S3_Rect::scale(float AFactor) {
  float w05 = (float)w * 0.5f;    // half width
  float h05 = (float)h * 0.5f;    // jalf height
  float ws = (float)w * AFactor;  // width scaled
  float hs = (float)h * AFactor;  // height scaled
  float xc = (float)x + w05;      // center x
  float yc = (float)y + h05;      // center y
  x = xc - (w05 * AFactor);
  y = yc - (h05 * AFactor);
  w = ws;
  h = hs;
}

void S3_Rect::grow(int32 AXValue, int32 AYValue) {
  x -= AXValue;
  y -= AYValue;
  w += AXValue*2;
  h += AYValue*2;
}

void S3_Rect::shrink(int32 AXValue, int32 AYValue) {
  x += AXValue;
  y += AYValue;
  w -= AXValue*2;
  h -= AYValue*2;
}

void S3_Rect::scale(float AXFactor, float AYFactor) {
  float w05 = (float)w * 0.5f;    // half width
  float h05 = (float)h * 0.5f;    // jalf height
  float ws = (float)w * AXFactor; // width scaled
  float hs = (float)h * AYFactor; // height scaled
  float xc = (float)x + w05;      // center x
  float yc = (float)y + h05;      // center y
  x = xc - (w05 * AXFactor);
  y = yc - (h05 * AYFactor);
  w = ws;
  h = hs;
}

//----------------------------------------------------------------------

/*

//    ___
//   |  _|_          _:.
//   | | | |   ->   | | :
//   | |_|_|        |_|.:
//   |___|            :
//

// keep only intersecting area

void S3_Rect::crop(int32 AX, int32 AY, int32 AW, int32 AH) {
  if( AX > x ) x = AX;
  if( AY > y ) y = AY;
  if( (AX+AW) <= (x+w) ) w = (AX+AW) - x;
  if( (AY+AH) <= (y+h) ) h = (AY+AH) - y;
}

// largest rect contained in both the specified rect and in this rect.
// or if the rects do not intersect, an empty rect.

void S3_Rect::intersection(int32 AX, int32 AY, int32 AW, int32 AH) {
  int32 tx1 = x;
  int32 ty1 = y;
  int32 rx1 = AX;
  int32 ry1 = AY;
  int32 tx2 = tx1 + w;
  int32 ty2 = ty1 + h;
  int32 rx2 = rx1 + AW;
  int32 ry2 = ry1 + AH;
  if (tx1 < rx1) tx1 = rx1;
  if (ty1 < ry1) ty1 = ry1;
  if (tx2 > rx2) tx2 = rx2;
  if (ty2 > ry2) ty2 = ry2;
  tx2 -= tx1;
  ty2 -= ty1;
  x = tx1;
  y = ty1;
  w = tx2-tx1+1;
  h = ty2-ty1+1;
}

void S3_Rect::intersection(S3_Rect ARect) {
  intersection( ARect.x, ARect.y, ARect.w, ARect.h );
}

*/

//----------------------------------------------------------------------
#endif
