#ifndef s2_filter_allpass_included
#define s2_filter_allpass_included
//----------------------------------------------------------------------

class S2_Filter_Allpass {

  private:

    float a;
    float x0,x1,x2;
    float y0,y1,y2;

  public:

    //SFilter_Allpass(float ACoefficient) {
    //  a   = ACoefficient;
    //  x0  = 0;
    //  x1  = 0;
    //  x2  = 0;
    //  y0  = 0;
    //  y1  = 0;
    //  y2  = 0;
    //}

    S2_Filter_Allpass() {
      setCoefficient(0);
    }

    S2_Filter_Allpass(float ACoefficient) {
      setCoefficient(ACoefficient);
    }

    void setCoefficient(float ACoefficient) {
      a   = ACoefficient;
      x0  = 0;
      x1  = 0;
      x2  = 0;
      y0  = 0;
      y1  = 0;
      y2  = 0;
    }

    float process(float AInput) {
      x2 = x1;
      x1 = x0;
      x0 = AInput;
      y2 = y1;
      y1 = y0;
      //output := x2 + ( (AInput-y2) * a );
      //output := KKillDenorm(output);
      //y0 := output;
      y0 = x2 + ( (AInput-y2) * a );

      y0 = S2_KillDenorm(y0);

      return y0;
    }

};

//----------------------------------------------------------------------
#endif













/*

https://github.com/rjeschke/cetonesynths/blob/master/cetoneorg/reverb.h

class AllPassFilter
{
public:
    AllPassFilter()
    {
        this->Reset();
    }

	~AllPassFilter()
    {
    }

	void setSize(int size)
	{
		this->_Size = size;
		this->Reset();
	}

    void Reset()
    {
		memset(this->_Buffer, 0, 3072 * sizeof(float));
    }

    __forceinline float Process(float input)
    {
        float output, buf;

        buf = this->_Buffer[this->_Pos];
        output = buf - input;

		this->_Buffer[this->_Pos] = input + (buf * 0.5f);
		UNDENORM(this->_Buffer[this->_Pos]);

		this->_Pos = (this->_Pos + 1) % this->_Size;

        return output;
    }
private:
    float _Buffer[3072];
    int _Size;
    int _Pos;
};

*/
