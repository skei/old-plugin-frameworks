#ifndef kode_point_included
#define kode_point_included
//----------------------------------------------------------------------

#include <math.h> // sqrt

#include "kode.h"

//----------------------------------------------------------------------

class KODE_Point {

  public:

    union {
      float x;
      float w;
    };

    union {
      float y;
      float h;
    };

  //------------------------------
  //
  //------------------------------

  public:

    KODE_Point() {
    }

    KODE_Point(float av) {
      set(av);
    }

    KODE_Point(float ax, float ay) {
      set(ax,ay);
    }

    //KODE_Point(const KODE_Point& APoint) {
    //  set(APoint);
    //}

    ~KODE_Point() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    void set(float av) {
      x = av;
      y = av;
    }

    void set(float ax, float ay) {
      x = ax;
      y = ay;
    }

    void set(KODE_Point APoint) {
      x = APoint.x;
      y = APoint.y;
    }

  //------------------------------
  //
  //------------------------------

public:

    //   =

    KODE_Point& operator = (const float AValue) {
      set(AValue);
      return *this;
    }

    KODE_Point& operator = (const KODE_Point& APoint) {
      if (this == &APoint) return *this;
      x = APoint.x;
      y = APoint.y;
      return *this;
    }

    //  +

    KODE_Point& operator + (const float f) {
      x += f;
      y += f;
      return *this;
    }

    KODE_Point& operator + (const KODE_Point& APoint) {
      x += APoint.x;
      y += APoint.y;
      return *this;
    }

    //  -

    KODE_Point& operator - (const float f) {
      x += f;
      y += f;
      return *this;
    }

    KODE_Point& operator - (const KODE_Point& APoint) {
      x -= APoint.x;
      y -= APoint.y;
      return *this;
    }

    //  *

    KODE_Point& operator * (const float f) {
      x *= f;
      y *= f;
      return *this;
    }

    KODE_Point& operator * (const KODE_Point& APoint) {
      x *= APoint.x;
      y *= APoint.y;
      return *this;
    }

    //  /

    KODE_Point& operator / (const float f) {
      x /= f;
      y /= f;
      return *this;
    }

    KODE_Point& operator / (const KODE_Point& APoint) {
      x /= APoint.x;
      y /= APoint.y;
      return *this;
    }

    //  +=

    KODE_Point& operator += (const float f) {
      x += f;
      y += f;
      return *this;
    }

    KODE_Point& operator += (const KODE_Point& APoint) {
      x += APoint.x;
      y += APoint.y;
      return *this;
    }

    //  -=

    KODE_Point& operator -= (const float f) {
      x += f;
      y += f;
      return *this;
    }

    KODE_Point& operator -= (const KODE_Point& APoint) {
      x -= APoint.x;
      y -= APoint.y;
      return *this;
    }

    //  *=

    KODE_Point& operator *= (const float f) {
      x *= f;
      y *= f;
      return *this;
    }

    KODE_Point& operator *= (const KODE_Point& APoint) {
      x *= APoint.x;
      y *= APoint.y;
      return *this;
    }

    //  /=

    KODE_Point& operator /= (const float f) {
      x /= f;
      y /= f;
      return *this;
    }

    KODE_Point& operator /= (const KODE_Point& APoint) {
      x /= APoint.x;
      y /= APoint.y;
      return *this;
    }

  //------------------------------
  //
  //------------------------------

  public:

    float distance(KODE_Point APoint) {
      float dx = (APoint.x - x);
      float dy = (APoint.y - y);
      float d = (dx*dx) + (dy*dy);
      return sqrtf(d);
    }

    KODE_Point midpoint(KODE_Point APoint) {
      KODE_Point result;
      result.x = (x + APoint.x) * 0.5f;
      result.x = (x + APoint.y) * 0.5f;
      return result;
    }

};

//----------------------------------------------------------------------
#endif