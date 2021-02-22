#ifndef s3_complex_included
#define s3_complex_included
//----------------------------------------------------------------------

// http://rosettacode.org/wiki/Arithmetic/Complex#Pascal

struct S3_Complex {

  float re;
  float im;

  //------------------------------

  S3_Complex() {
    re = 0.0f;
    im = 0.0f;
  }

  //----------

  S3_Complex(float r, float i) {
    re = r;
    im = i;
  }

  //------------------------------

  void set(float r, float i) {
    re = r;
    im = i;
  }

  //----------

  void neg(void) {
    re = -re;
    im = -im;
  }

  //----------

  void inv(void) {
    float a = (re*re) + (im*im);
    re /= a;
    im /= a;
  }

  //----------

  void add(float r, float i) {
    re += r;
    im += i;
  }

  //----------

  void mul(float r, float i) {
    float rr = re;
    float ii = im;
    re = (rr*r) - (ii*i);
    im = (rr*i) + (ii*r);
  }

  //------------------------------

  void operator = (const S3_Complex A1) {
    re = A1.re;
    im = A1.im;
  }

  //----------

  void operator + (const S3_Complex A1) {
    re += A1.re;
    im += A1.im;
  }

  //----------

  void operator - (void) {
    re = -re;
    im = -im;
  }

  //----------

  void operator - (const S3_Complex A1) {
    re -= A1.re;
    im -= A1.im;
  }

  //----------

  void operator * (const S3_Complex A1) {
    float r = (re*A1.re) - (im*A1.im);
    float i = (re*A1.im) + (im*A1.re);
    re = r;
    im = i;
  }

  //----------

};

//----------------------------------------------------------------------
#endif


