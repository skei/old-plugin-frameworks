#ifndef s3_string_included
#define s3_string_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_String {
  private:
    char MBuffer[S3_MAX_STRING_SIZE];
    char temp_buffer[256]; // // CppCheck: not initialized in constructor
  public:
    S3_String();
    S3_String(const char* format, ...);
    ~S3_String();
    char* ptr(void);
    void  clear(bool AErase=false);
    int32 length(void);
    void  append(const char AChar);
    void  remove(const unsigned int APos);
    S3_String& operator =  (const char* AChars);
    S3_String& operator +  (const char* AChars);
    S3_String& operator += (const char* AChars);
    S3_String& operator =  (S3_String& AString);
    S3_String& operator +  (S3_String& AString);
    S3_String& operator += (S3_String& AString);
    S3_String& operator =  (const int32 AValue);
    S3_String& operator +  (const int32 AValue);
    S3_String& operator += (const int32 AValue);
    char       operator [] (const unsigned int AIndex);
};

//----------

void S3_RemoveChars(char *str, char c);
void S3_RemoveChunks(char *str, char *cnk);
void S3_ReplaceChars(char *str, char c1, char c2);
void S3_ReplaceChunks(char *str, char *cnk1, char *cnk2);
void S3_Reverse(char *str);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#include <stdarg.h> //va_start, ...
#include "common/s3_array.h"
#include "common/s3_convert.h"

class S3_String;
typedef S3_Array<S3_String*> S3_StringArray;

//----------

S3_String::S3_String() {
  clear();
}

//S3_String(const char* AChars) {
//  clear();
//  S3_Strcpy(MBuffer,AChars);
//}

S3_String::S3_String(const char* format, ...) {
  clear();
  va_list args;
  va_start(args,format);
  vsprintf(temp_buffer,format,args);
  S3_Strcpy(MBuffer,temp_buffer);
  va_end(args);
}


S3_String::~S3_String() {
}

char* S3_String::ptr(void) {
  return MBuffer;
}

void S3_String::clear(bool AErase) {
  if (AErase) S3_Memset(MBuffer,0,S3_MAX_STRING_SIZE);
  else MBuffer[0] = 0;
}

int32 S3_String::length(void) {
  return S3_Strlen(MBuffer);
}

void S3_String::append(const char AChar) {
  MBuffer[length()] = AChar;
}

void S3_String::remove(const unsigned int APos) {
  if (length()==1) clear();
  else { for (int32 i=APos; i<length()-1; i++ ) MBuffer[i] = MBuffer[i+1]; }
}

//void S3_String::operator = (S3_String str)   { SText = str.mText; }
//void S3_String::operator = (h_String* str)   { mText = str->mText; }
//void S3_String::operator = (char* txt)       { mText = txt; }
//void S3_String::operator = (const char* txt) { mText = (char*)txt; }

S3_String& S3_String::operator = (const char* AChars) {
  S3_Strcpy(MBuffer,AChars);
  return *this;
}

S3_String& S3_String::operator + (const char* AChars) {
  S3_Strcat(MBuffer,AChars);
  return *this;
}

S3_String& S3_String::operator += (const char* AChars) {
  S3_Strcat(MBuffer,AChars);
  return *this;
}

// CppCheck: temp_buffer not assigned a value

S3_String& S3_String::operator = (S3_String& AString) {
  if (this==&AString) return *this;
  S3_Strcpy(MBuffer,AString.ptr());
  return *this;
}

S3_String& S3_String::operator + (S3_String& AString) {
  S3_Strcat(MBuffer,AString.ptr());
  return *this;
}

S3_String& S3_String::operator += (S3_String& AString) {
  S3_Strcat(MBuffer,AString.ptr());
  return *this;
}

/*
S3_String& S3_String::operator = (S3_String AString) {
  if (this==&AString) return *this;
  S3_Strcpy(MBuffer,AString.ptr());
  return *this;
}

S3_String& S3_String::operator + (S3_String& AString) {
  S3_Strcat(MBuffer,AString.ptr());
  return *this;
}

S3_String& S3_String::operator += (S3_String& AString) {
  S3_Strcat(MBuffer,AString.ptr());
  return *this;
}
*/

S3_String& S3_String::operator = (const int32 AValue) {
  S3_Strcpy( MBuffer, S3_IntToString(temp_buffer,AValue) );
  return *this;
}

S3_String& S3_String::operator + (const int32 AValue) {
  S3_Strcat( MBuffer, S3_IntToString(temp_buffer,AValue) );
  return *this;
}

S3_String& S3_String::operator += (const int32 AValue) {
  S3_Strcat( MBuffer, S3_IntToString(temp_buffer,AValue) );
  return *this;
}

char S3_String::operator [] (const unsigned int AIndex) {
  return MBuffer[AIndex];
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// http://www.cprogramming.com/snippets/source-code/useful-c-string-functions

//----------

// remove specified characters from a string

//static __S3_UNUSED
void S3_RemoveChars(char *str, char c) {
	char *pos;
	while( ( pos = S3_Strchr(str,c) ) ) {
		S3_Memmove(pos, pos + 1, S3_Strlen(pos));
  }
}

//----------

// remove specified chunks from a string

//static __S3_UNUSED
void S3_RemoveChunks(char *str, char *cnk) {
	char *pos;
	int clen = S3_Strlen(cnk);
	while( ( pos = S3_Strstr(str, cnk) ) ) {
		S3_Memmove(pos, pos + clen, S3_Strlen(pos) - clen + 1);
  }
}

//----------

// replace specified characters in a string

//static __S3_UNUSED
void S3_ReplaceChars(char *str, char c1, char c2) {
	char *pos;
	while( ( pos = S3_Strchr(str, c1) ) ) {
		*pos = c2;
  }
}

//----------

// replace specified chunks in a string
// (size-independent, just remember about memory)

//static __S3_UNUSED
void S3_ReplaceChunks(char *str, char *cnk1, char *cnk2)
{
	char *pos;
	int clen1 = S3_Strlen(cnk1), clen2 = S3_Strlen(cnk2);
	while( ( pos = S3_Strstr(str,cnk1) ) )
	{
		S3_Memmove(pos + clen2, pos + clen1, S3_Strlen(pos) - clen1 + 1);
		S3_Memcpy(pos, cnk2, clen2);
	}
}

//----------

//reverse a string

//static __S3_UNUSED
void S3_Reverse(char *str) {
	int i;
	int len = S3_Strlen(str) - 1;
	int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
	for (i=0; i<=mid; ++i) {
		char buf = str[i];
		str[i] = str[len - i];
		str[len - i] = buf;
	}
}

//----------------------------------------------------------------------
#endif
