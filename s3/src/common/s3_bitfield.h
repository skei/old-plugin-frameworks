#ifndef s3_bitfield_included
#define s3_bitfield_included
//----------------------------------------------------------------------

// TODO

template <typename NUMINTS>
class S3_Bitfield {
  private:
    uint32 MBuffer[NUMINTS]
  public:
  
    void clear(void) {
      S3_Memset(MBuffer,0,sizeof(MBuffer));
    }
    
    void set(uint32 ABit) {
      uint32 _bit = (ABit & 31) << ABit;
      uint32 _int = ABit >> 5;
      MBuffer[_int] |= _bit;
    }
  
    void clear(uint32 ABit) {
      uint32 _bit = (ABit & 31) << ABit;
      uint32 _int = ABit >> 5;
      MBuffer[_int] &= ~_bit;
    }
    
    bool set(uint32 ABit) {
      uint32 _bit = (ABit & 31) << ABit;
      uint32 _int = ABit >> 5;
      return (MBuffer[_int] & _bit);
    }
  
};

//----------------------------------------------------------------------
#endif
