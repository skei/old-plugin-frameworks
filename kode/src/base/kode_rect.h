#ifndef kode_rect_included
#define kode_rect_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/kode_point.h"

struct KODE_Rect {

  union {
    float x;
    float left;
  };
  
  union {
    float y;
    float top;
  };
  
  union {
    float w;
    float right;
  };
  
  union {
    float h;
    float bottom;
  };
  
  //float x,y,w,h;

  KODE_Rect() {
  }

  KODE_Rect(float f) {
    x = f;
    y = f;
    w = f;
    h = f;
  }

  KODE_Rect(float fw, float fh) {
    x = 0;
    y = 0;
    w = fw;
    h = fh;
    //KODE_Trace("%f %f\n",w,h);
  }

  KODE_Rect(float fx, float fy, float fw, float fh) {
    x = fx;
    y = fy;
    w = fw;
    h = fh;
  }

  float x2() {
    return x + w;
  }

  float y2() {
    return y + h;
  }

  void x2(float x2) {
    w = x2 - x;
  }

  void y2(float y2) {
    h = y2 - y;
  }

  KODE_Point pos() {
    KODE_Point result;
    result.x = x;
    result.y = y;
    return result;
  }

  KODE_Point size() {
    KODE_Point result;
    result.w = w;
    result.h = h;
    return result;
  }

  void set(float av) {
    x = av;
    y = av;
    w = av;
    h = av;
  }

  void set(float aw, float ah) {
    x = 0;
    y = 0;
    w = aw;
    h = ah;
  }

  void set(float ax, float ay, float aw, float ah) {
    x = ax;
    y = ay;
    w = aw;
    h = ah;
  }

  void set(KODE_Rect ARect) {
    x = ARect.x;
    y = ARect.y;
    w = ARect.w;
    h = ARect.h;
  }

  KODE_Rect operator = (const float AValue) {
    set(AValue);
    return *this;
  }

  KODE_Rect operator = (const KODE_Rect ARect) {
    if (this == &ARect) return *this;
    x = ARect.x;
    y = ARect.y;
    w = ARect.w;
    h = ARect.h;
    return *this;
  }
  
  //----------

  bool contains(float ax, float ay) {
    if (ax    <   x)    return false;
    if (ay    <   y)    return false;
    if (ax    >=  x2()) return false;
    if (ay    >=  y2()) return false;
    return true;
  }

  bool surrounds(KODE_Rect ARect) {
    if (ARect.x     <   x)    return false;
    if (ARect.y     <   y)    return false;
    if (ARect.x2()  >=  x2()) return false;
    if (ARect.y2()  >=  y2()) return false;
    return true;
  }

  bool intersects(KODE_Rect ARect) {
    if (ARect.x     >=  x2()) return false;
    if (ARect.y     >=  y2()) return false;
    if (ARect.x2()  <   x)    return false;
    if (ARect.y2()  <   y)    return false;
    return true;
  }

  bool isSurroundedBy(KODE_Rect ARect) {
    if (x     <   ARect.x)    return false;
    if (y     <   ARect.y)    return false;
    if (x2()  >=  ARect.x2()) return false;
    if (y2()  >=  ARect.y2()) return false;
    return true;
  }

  bool isVisibleInside(KODE_Rect ARect) {
    if (x     >=  ARect.x2()) return false;
    if (y     >=  ARect.y2()) return false;
    if (x2()  <   ARect.x)    return false;
    if (y2()  <   ARect.y)    return false;
    return true;
  }

  bool isEmpty() {
    if (w <= 0) return true;
    if (h <= 0) return true;
    return false;
  }

  bool isNotEmpty() {
    return !isEmpty();
  }

  //----------

  void add(KODE_Rect ARect) {
    x += ARect.x;
    y += ARect.y;
    w += ARect.w;
    h += ARect.h;
  }

  void add(float ax, float ay) {
    x += ax;
    y += ay;
  }

  void add(KODE_Point APoint) {
    x += APoint.x;
    y += APoint.y;
  }

  void add(float ax, float ay, float aw, float ah) {
    x += ax;
    y += ay;
    w += aw;
    h += ah;
  }
  
  void grow(float AAmount) {
    x -= AAmount;
    y -= AAmount;
    w += (AAmount * 2.0f);
    h += (AAmount * 2.0f);
  }

  void grow(float AXamount, float AYamount) {
    x -= AXamount;
    y -= AYamount;
    w += (AXamount * 2.0f);
    h += (AYamount * 2.0f);
  }

  void shrink(float AAmount) {
    grow(-AAmount);
  }

  void shrink(float AXamount, float AYamount) {
    grow(-AXamount,-AYamount);
  }

  void pad(KODE_Rect ARect) {
    x += ARect.left;
    y += ARect.top;
    w -= (ARect.left + ARect.right);
    h -= (ARect.top + ARect.bottom);
  }

  //----------
  
  // encompass?

  void combine(KODE_Rect ARect) {
    if (ARect.x     < x)    x = ARect.x;
    if (ARect.y     < y)    y = ARect.y;
    if (ARect.x2()  > x2()) x2(ARect.x2());
    if (ARect.y2()  > y2()) y2(ARect.y2());
  }
  
  /*
    cut away anything outside ARect
  */
  
  void intersect(KODE_Rect ARect) {
    float v;
    // left
    v = ARect.x - x;
    if (v > 0) {
      x  = ARect.x;
      w -= v;
    }
    // right
    v = (x+w) - ARect.x2();
    if (v > 0) {
      w -= v;
    }
    // top
    v = ARect.y - y;
    if (v > 0) {
      y  = ARect.x;
      h -= v;
    }
    // bottom
    v = (y+h) - ARect.y2();
    if (v > 0) {
      h -= v;
    }
  }
  
  //----------
  
  /*
    largest x/y, smallest x2/y2
    
    ex: overlap clipping rect with child clipping rect
      new_clip_rect = current.clip_rect.overlap(child.clip_rect)
      if new_clip_rect.notEmpty() {
        painter.clip(new_clip_rect);
        paintChildren();
        ..
      }
  */
  
  void overlap(KODE_Rect ARect) {
    if (ARect.x > x) x = ARect.x;
    if (ARect.x2() > x2()) x2(ARect.x2());
    if (ARect.y > y) y = ARect.y;
    if (ARect.y2() > y2()) y2(ARect.y2());
  }

};

//----------------------------------------------------------------------
#endif