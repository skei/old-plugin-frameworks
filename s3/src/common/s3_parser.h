#ifndef s3_parser_included
#define s3_parser_included
//----------------------------------------------------------------------

//#if 0

#include "common/s3_array.h"
#include "common/s3_memory_stdlib.h"
//#include "common/s3_string.h"

//typedef SArray<char*> s_parser_str_array;

//----------

class S3_Parser {

  private:
    uint32          MBufLen;
    char*           MBuffer;
    //s_parser_str_array  MTokens;
    //S3_StringArray  MTokens;
    S3_CharPtrArray MTokens;

  public:

    S3_Parser() {
      MBufLen = 0;
      MBuffer = S3_NULL;
    }

    //----------

    ~S3_Parser() {
      if (MBuffer) S3_Free(MBuffer);
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    uint32  numTokens(void) { return MTokens.size(); }
    char*   token(uint32 i) { return MTokens[i]; }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    /*
      space is a separator! cr,lf,tab is too ( <= 32 )
      change <=32 to <32 to turn space into a character, not a separator

      copies buffer..
    */

    void parse(const char* ASource, uint32 ALength) {
      bool quotes = false;
      if (MBuffer) S3_Free(MBuffer);
      //MBufLen = 0;
      MTokens.clear(/*true*/);

      MBufLen = ALength;
      MBuffer = (char*)S3_Malloc(MBufLen+1);
      S3_Memcpy(MBuffer,(char*)ASource,MBufLen);
      MBuffer[MBufLen] = 0;

      // replace any non-visual chars with '\0'
      // keep spaces inside "" -> one token

      for (uint32 i=0; i<MBufLen; i++) {
        if (MBuffer[i]<32) {
          MBuffer[i] = 0;
        }
        if (MBuffer[i]==32) { // space
          if (!quotes) MBuffer[i] = 0;
        }
        if (MBuffer[i] == 34 ) { // "
          if (quotes) quotes = false;
          else quotes = true;
          MBuffer[i] = 0;
        }
      }

      // new tokens after each (last) 0

      char prev_c = 0;
      for (uint32 i=0; i<MBufLen; i++) {
        char c = MBuffer[i];
        if ((prev_c == 0) && (c != 0)) {
          MTokens.append(&MBuffer[i]);
        }
        prev_c = c;
      }
    }

  //----------------------------------------
  //
  //----------------------------------------

  public:

    void dumpTokens(void) {
      int32 num = MTokens.size();
      for (int32 i=0; i<num; i++) {
        S3_DTrace("%i '%s'\n",i,MTokens[i]);
      }
    }

};

//#endif // 0

//----------------------------------------------------------------------
#endif

