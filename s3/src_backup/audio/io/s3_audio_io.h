#ifndef s2_audio_driver_included
#define s2_audio_driver_included
//----------------------------------------------------------------------

class S2_AudioIO {

  private:

    //S2_Channel*  MChannel;

  public:

    S2_AudioIO() {
    }

    virtual ~S2_AudioIO() {
    }

  public:

    //S2_Channel* channel(void)               { return MChannel; }
    //void        channel(SChannel* AChannel) { MChannel = AChannel; }

  public:

    virtual void processBlock(S2_Sample** AInputs, S2_Sample** AOutputs, uint32 ALength) {
      //MChannel->processBlock(AInputs,AOutputs,ALength);
    }

};

//----------------------------------------------------------------------
#endif
