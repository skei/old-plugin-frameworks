#ifndef kode_string_included
#define kode_string_included
//----------------------------------------------------------------------

/*
  TODO:
  - move all functionality to separate functions/classes,
    and use them from here..
  - dynamic string = subclass of array?
*/

#include "kode.h"
#include "base/kode_convert.h"
#include "base/kode_random.h"

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// todo
// inherit from KODE_DynamicBuffer

template <int SIZE>
class KODE_StaticString {

private:

  //char MBuffer[KODE_MAX_STRING_LENGTH];
  char MBuffer[SIZE];

//------------------------------
//
//------------------------------

public:

  KODE_StaticString() {
    MBuffer[0] = 0;
  }

  KODE_StaticString(const KODE_StaticString& str) {
    KODE_StaticString* sstr = (KODE_StaticString*)&str;
    KODE_Strcpy(MBuffer,sstr->ptr());
  }

  KODE_StaticString(const char* str) {
    KODE_Strcpy(MBuffer,str);
  }

//------------------------------
public:
//------------------------------

  const char* ptr() {
    return MBuffer;
  }

  //----------

  uint32_t length() {
    return KODE_Strlen(MBuffer);
  }

  //----------

  bool empty(void) {
    if (MBuffer[0] == 0) return true;
    return false;
  }

  //----------

  bool notEmpty(void) {
    if (MBuffer[0] == 0) return false;
    return true;
  }

  //----------

  void clear(bool AErase=false) {
    if (AErase) KODE_Memset(MBuffer,0,sizeof(MBuffer));
    else MBuffer[0] = 0;
  }

//------------------------------
public:
//------------------------------

  const char& operator [] (uint32_t AIndex) const {
    return MBuffer[AIndex];
  }

  //----------

  operator char* () {
    return MBuffer;
  }

  //----------

  operator const char* ()	const {
    return MBuffer;
  }

  //----------

  KODE_StaticString& operator = (char* str) {
    KODE_Strcpy(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator = (const char* str) {
    KODE_Strcpy(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator = (/*const*/ KODE_StaticString& str) {
    KODE_Strcpy(MBuffer,str.ptr());
    return *this;
  }

  //----------

  KODE_StaticString& operator = (int AValue) {
    char temp[16];
    char* str = KODE_IntToString(temp,AValue);
    KODE_Strcpy(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator = (float AValue) {
    char temp[16];
    char* str = KODE_FloatToString(temp,AValue);
    KODE_Strcpy(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator += (char* str) {
    KODE_Strcat(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator += (const char* str) {
    KODE_Strcat(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator += (/*const*/ KODE_StaticString& str) {
    KODE_Strcat(MBuffer,str.ptr());
    return *this;
  }

  //----------

  KODE_StaticString& operator += (const int i) {
    char temp[16];
    char* str = KODE_IntToString(temp,i);
    //KODE_Trace("%s\n",str);
    KODE_Strcat(MBuffer,str);
    return *this;
  }

  //----------

  KODE_StaticString& operator += (const float f) {
    char temp[16];
    char* str = KODE_FloatToString(temp,f);
    KODE_Strcat(MBuffer,str);
    return *this;
  }

  //----------

  bool operator == (const char* str) {
    return (KODE_Strcmp(MBuffer,str) == 0) ? true : false;
  }

  //----------

  bool operator == (KODE_StaticString& str) {
    return (KODE_Strcmp(MBuffer,str.ptr()) == 0) ? true : false;
  }

  //----------

  bool operator != (const char* str) {
    return (KODE_Strcmp(MBuffer,str) != 0) ? true : false;
  }

  //----------

  bool operator != (KODE_StaticString& str) {
    return (KODE_Strcmp(MBuffer,str.ptr()) != 0) ? true : false;
  }

//------------------------------
private:
//------------------------------

  bool _match(char* wildcards, char* str) {
    bool Yes = 1;
    //iterate and delete '?' and '*' one by one
    while(*wildcards != '\0' && Yes && *str != '\0') {
      if (*wildcards == '?') str ++;
      else if (*wildcards == '*') {
        Yes = _scan(wildcards, str);
        wildcards --;
      }
      else {
        Yes = (*wildcards == *str);
        str ++;
      }
      wildcards ++;
    }
    while (*wildcards == '*' && Yes)  wildcards ++;
    return Yes && *str == '\0' && *wildcards == '\0';
  }

  //----------

  bool _scan(char*& wildcards, char*& str) {
    // remove the '?' and '*'
    for (wildcards ++; *str != '\0' && (*wildcards == '?' || *wildcards == '*'); wildcards ++)
      if (*wildcards == '?') str ++;
    while ( *wildcards == '*') wildcards ++;
    // if str is empty and wildcards has more characters or,
    // wildcards is empty, return
    if (*str == '\0' && *wildcards != '\0') return false;
    if (*str == '\0' && *wildcards == '\0')	return true;
    // else search substring
    else {
      char* wds_copy = wildcards;
      char* str_copy = str;
      bool  Yes     = 1;
      do {
        if (!_match(wildcards,str))	str_copy++;
        wildcards = wds_copy;
        str = str_copy;
        while ((*wildcards != *str) && (*str != '\0')) str ++;
        wds_copy = wildcards;
        str_copy = str;
      } while ((*str != '\0') ? !_match(wildcards, str) : (Yes = false) != false);
      if (*str == '\0' && *wildcards == '\0')	return true;
      return Yes;
    }
  }

//------------------------------
public:
//------------------------------

  // search the match string
  // wildcards can be '?' and '*' combination
  // return value : true (pattern matchs string), false (no match)

  bool search(const char* wildcards) {
    return _match((char*)wildcards, MBuffer);
  }

  //----------

  // returns first occurence of 'str'
  // or NULL if not found

  char* find(char* str) {
    return KODE_Strstr(MBuffer,str);
  }

  //----------

  void append(const char* str) {
    KODE_Strcat(MBuffer,str);
  }

  //----------

  void append(/*const*/KODE_StaticString& str) {
    KODE_Strcat(MBuffer,str.ptr());
  }

  //----------

  // replace all instances of c1 with c2

  void replaceChars(const char c1, const char c2) {
    char *pos;
    while ((pos = KODE_Strchr(MBuffer,c1))) {
      *pos = c2;
    }
  }

  //----------

  // replace all occurences of str1 with str2
  // assumes final size will be < 1024
  // be careful about recursion! replace r with r2 for example.. :-/
  // todo: wipe out src after replacing, so we don't re-find it?

  void replace(const char* str1, const char* str2) {
    //_grow_buffer(1024);
    char temp[1024];
    uint32_t length = KODE_Strlen(MBuffer);
    KODE_Memcpy(temp,MBuffer,length+1);
    char *pos;
    int clen1 = KODE_Strlen(str1);
    int clen2 = KODE_Strlen(str2);
    while ((pos = KODE_Strstr(temp,str1))) {
      //char* bufpos = pos - temp + MBuffer;
      KODE_Memmove(pos + clen2, pos + clen1, KODE_Strlen(pos) - clen1 + 1);
      KODE_Memcpy(pos, str2, clen2);
      //KODE_Memset(pos , 1, clen1);
    }
    //_resize_buffer( KODE_Strlen(MBuffer) );
    KODE_Strcpy(MBuffer,temp);
  }

  //----------

  void lowerCase(void) {
    char* ptr = MBuffer;
    int c;
    while ((c = *ptr)) {
      if ((c>='A') && (c<='Z')) *ptr = c + 0x20;
      ptr++;
    }
  }

  //----------

  void upperCase(void) {
    char* ptr = MBuffer;
    int c;
    while ((c = *ptr)) {
      if ((c>='a') && (c<='z')) *ptr = c - 0x20;
      ptr++;
    }
  }

  //----------

  void reverse(void) {
    int i;
    int len = KODE_Strlen(MBuffer) - 1;
    int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
    for (i=0; i<=mid; ++i) {
      char buf = MBuffer[i];
      MBuffer[i] = MBuffer[len - i];
      MBuffer[len - i] = buf;
    }
  }

  //----------

  void makeValidSymbol(void) {
    for (uint32_t i=0; i<KODE_Strlen(MBuffer); i++) {
      unsigned char c = MBuffer[i];
      if ((c<32) || (c>127)) MBuffer[i] = '_';
      else MBuffer[i] = KODE_VALID_SYMBOL_CHARS[c];
    }
  }

  //----------

  // ptr to (reversed) hex
  // prefix + ptr2hex (inverted) + random number
  // (for creating unique strings (based on pointers)
  // TODO: hash?

  void createUnique(const char* APrefix, void* APtr) {
    const char hex_table[17] = "0123456789ABCDEF";  // +'\0' = 17
    const uint32_t hex_size  = sizeof(void*) * 2;
    const uint32_t rand_size = sizeof(int32_t) * 2;
    //_resize_buffer( KODE_Strlen(APrefix) + hex_size + 1 + rand_size );
    char* buf = MBuffer;//temp;
    while (*APrefix != '\0') *buf++ = *APrefix++;   // memset + update ptrs
    intptr_t iptr = (intptr_t)APtr;
    for (uint32_t i=0; i<hex_size; i++) {
      int j = iptr & 0x0f;
      iptr >>= 4;
      *buf++ = hex_table[j];
    }
    *buf++ = ':';
    int32_t rnd = KODE_RandomInt();
    for (uint32_t i=0; i<rand_size; i++) {
      int j = rnd & 0x0f;
      rnd >>= 4;
      *buf++ = hex_table[j];
    }
    *buf++ = '\0';
  }

  //----------

  // https://stackoverflow.com/questions/7666509/hash-function-for-string

  uint32_t hash(void) {
    char* ptr = MBuffer;
    unsigned long h = 5381;
    int c;
    while ((c = *ptr++)) {
      h = ((h << 5) + h) + c; // h * 33 + c
    }
    return h;
  }

  //----------

  // problem if there is a dot in the path
  // todo: find last dot..

  /*
  void stripFileExt(void) {
    char *dot;
    dot = KODE_Strrchr(MBuffer,'.');
    if (dot) {
      int32_t len = dot - MBuffer;
      _resize_buffer(len);
    }
  }
  */

  //----------

  /*
  void replaceFileExt(const char* ext) {
    stripFileExt();
    append(".");
    append(ext);
  }
  */

  //----------

  // /home/skei/test -> test

  /*
  void stripPath(void) {
    const char *slash, *backslash;
    slash     = KODE_Strrchr(MBuffer,'/');
    backslash = KODE_Strrchr(MBuffer,'\\');//+1;
    if (slash) {
      _copy_buffer(slash + 1);
    }
    else if (backslash) {
      _copy_buffer(backslash + 1); // see above
    }
  }
  */


};

//----------------------------------------------------------------------

//template <class T>
//KODE_StaticString operator + (T var, const KODE_StaticString& str)  {
//  KODE_StaticString svar = var;
//  return svar += str;
//}

template <int SIZE, class T>
KODE_StaticString<SIZE> operator + (T var, const KODE_StaticString<SIZE>& str)  {
  KODE_StaticString<SIZE> svar = var;
  return svar += str;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// todo
// inherit from KODE_DynamicBuffer

class KODE_DynamicString {
  
private:

  uint32_t  MLength;
  uint32_t  MBufferSize;
  char*     MBuffer;
  
private:

  void resize(uint32_t ASize) {
  }
  
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

typedef KODE_StaticString<KODE_MAX_STRING_LENGTH> KODE_String;

//----------------------------------------------------------------------
#endif