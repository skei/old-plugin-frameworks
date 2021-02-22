#ifndef s2_dsp_delay_included
#define s2_dsp_delay_included
//----------------------------------------------------------------------

/*
  Delay code based on The Synthesis ToolKit in C++ (STK)
  by Perry R. Cook and Gary P. Scavone, 1995-2012.
*/

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  non-interpolating delay line class.
  This class implements a non-interpolating digital delay-line. If
  the delay and maximum length are not specified during
  instantiation, a fixed maximum length of 4095 and a delay of zero
  is set.
*/

class S2_Delay {
  private:
    float*  buffer;
    uint32  length;// = 4096;
    uint32  delay;// = 0;
    uint32  inPoint;// = 0;
    uint32  outPoint;// = 0;
    float   last;// = 0.0;
  public:
    S2_Delay(uint32 l=4096) {
      buffer = new float[l];
      length = l;
      delay = 0;
      inPoint = 0;
      outPoint = 0;
      clear();
    }
    ~S2_Delay() {
      delete buffer;
    }
    void  setDelay(uint32 d) {
      if (inPoint >= d) outPoint = inPoint - d;
      else outPoint = length + inPoint - d;
      delay = d;
    }
    void  setMax(uint32 d) {
      if (d > length) {
        delete buffer;
        buffer = new float[d];
        for (uint32 i = 0; i < d; i++) {
          buffer[i] = 0;
        }
      }
      length = d;
    }
    void  clear(void) {
      for (uint32 i = 0; i < length; i++) buffer[i] = 0;
      last = 0;
    }
    float nextOut(void) {
      return buffer[outPoint];
    }
    float process(float in) {
      buffer[inPoint++] = in;
      if (inPoint == length) inPoint = 0;
      last = buffer[outPoint++];
      if (outPoint == length) outPoint = 0;
      return last;
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  Multi tap delay
*/

class S2_MDelay {
  private:
    float*  buffer;
    uint32  length;// = 4096;
    uint32  inPoint;// = 0;
  public:
    S2_MDelay(uint32 l = 4096) {
      buffer = new float[l];
      length = l;
      inPoint = 0;
      clear();
    }
    ~S2_MDelay() {
      delete buffer;
    }
    void  setMax(uint32 d) {
      if (d > length) {
          delete buffer;
          buffer = new float[d];
          for (uint32 i = 0; i < d; i++) {
              buffer[i] = 0;
          }
      }
      length = d;
    }
    void  clear(void) {
      for (uint32 i = 0; i < length; i++) {
          buffer[i] = 0;
      }
    }
    float at(int32 samples) {
      int32 outPoint = int32(inPoint) - samples;
      if (outPoint < 0) outPoint += length;
      return buffer[outPoint];
    }
    void  tick(float in) {
      buffer[inPoint++] = in;
      if (inPoint == length) inPoint = 0;
    }

};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  allpass interpolating delay line class.
  This class implements a fractional-length digital delay-line using
  a first-order allpass filter.
*/

class S2_DelayA {
  private:
    float*  buffer;// = 0;
    uint32  length;// = 4096;
    uint32  inPoint_;
    uint32  outPoint_;
    float   delay_;
    float   alpha_;
    float   coeff_;
    float   last_;// = 0.0;
    float   apInput_;// = 0.0;
    float   nextOutput_;// = 0.0;
    bool    doNextOut_;// = true;
  public:
    S2_DelayA(uint32 l = 4096) {
      buffer = new float[l];
      length = l;
      nextOutput_ = 0.0;
      doNextOut_ = true;
      clear();
    }
    ~S2_DelayA() {
      //delete buffer; FIXME
    }
    void  setDelay(float delay) {
      float outPointer = inPoint_ - delay + 1.0; // outPoint chases inpoint
      delay_ = delay;
      while (outPointer < 0) {
          outPointer += length; // modulo maximum length
      }
      outPoint_ = (int) outPointer; // integer part
      if (outPoint_ == length) {
          outPoint_ = 0;
      }
      alpha_ = 1.0 + outPoint_ - outPointer; // fractional part
      if (alpha_ < 0.5) {
          // The optimal range for alpha is about 0.5 - 1.5 in order to
          // achieve the flattest phase delay response.
          outPoint_ += 1;
          if (outPoint_ >= length) {
              outPoint_ -= length;
          }
          alpha_ += 1.0f;
      }
      coeff_ = (1.0 - alpha_) / (1.0 + alpha_); // coefficient for allpass
    }
    void  setMax(uint32 d) {
      if (d > length) {
          delete buffer;
          buffer = new float[d];
          for (uint32 i = 0; i < d; i++) {
              buffer[i] = 0;
          }
      }
      length = d;
    }
    void  clear(void) {
      for (uint32 i = 0; i < length; i++) {
          buffer[i] = 0.0f;
      }
      last_ = 0.0;
      inPoint_ = 0;
      apInput_ = 0.0;
    }
    float nextOut(void) {
      if (doNextOut_) {
          // Do allpass interpolation delay.
          nextOutput_ = -coeff_ * last_;
          nextOutput_ += apInput_ + (coeff_ * buffer[outPoint_]);
          doNextOut_ = false;
      }
      return nextOutput_;
    }
    float process(float in) {
      buffer[inPoint_++] = in;
      if (inPoint_ == length) {
          inPoint_ = 0;
      }
      last_ = nextOut();
      doNextOut_ = true;
      // Save the allpass input and increment modulo length.
      apInput_ = buffer[outPoint_++];
      if (outPoint_ == length) {
          outPoint_ = 0;
      }
      return last_;
    }
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
  linear interpolating delay line class
  This class implements a fractional-length digital delay-line using
  first-order linear interpolation.  If the delay and maximum length
  are not specified during instantiation, a fixed maximum length of
  4095 and a delay of zero is set.
*/

class S2_DelayL {
  private:
    float*  buffer;
    uint32  length;// = 4096
    uint32  inPoint_;
    uint32  outPoint_;
    float   delay_;
    float   alpha_;
    float   omAlpha_;
    float   last_;// = 0.0;
    float   nextOutput_;// = 0.0;
    bool    doNextOut_;// = true;

  public:
    S2_DelayL(uint32 l = 4096) {
      buffer = new float[l];
      length = l;
      inPoint_ = 0;
      nextOutput_ = 0.0;
      doNextOut_ = true;
      clear();
    }
    ~S2_DelayL() {
      delete buffer;
    }
    void setDelay(float delay) {
      float outPointer = inPoint_ - delay;  // read chases write
      delay_ = delay;

      while (outPointer < 0) {
          outPointer += length; // modulo maximum length
      }

      outPoint_ = (uint32) outPointer;   // integer part
      if (outPoint_ == length) outPoint_ = 0;
      alpha_ = outPointer - outPoint_; // fractional part
      omAlpha_ = (float) 1.0 - alpha_;
    }
    void setMax(uint32 d) {
      if (d > length) {
          delete buffer;
          buffer = new float[d];
          for (uint32 i = 0; i < d; i++) {
              buffer[i] = 0;
          }
      }
      length = d;
    }
    void clear(void) {
      for (uint32 i = 0; i < length; i++) {
          buffer[i] = 0.0f;
      }
      last_ = 0.0;
    }
    float nextOut(void) {
      if (doNextOut_) {
          // First 1/2 of interpolation
          nextOutput_ = buffer[outPoint_] * omAlpha_;
          // Second 1/2 of interpolation
          if (outPoint_+ 1 < length)
            nextOutput_ += buffer[outPoint_+1] * alpha_;
          else
            nextOutput_ += buffer[0] * alpha_;
          doNextOut_ = false;
      }
      return nextOutput_;
    }
    float process(float in) {
      buffer[inPoint_++] = in;
      if (inPoint_ == length) {
          inPoint_ = 0;
      }
      last_ = nextOut();
      doNextOut_ = true;
      if (++outPoint_ == length) {
          outPoint_ = 0;
      }
      return last_;
    }
};

//----------------------------------------------------------------------
#endif
