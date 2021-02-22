#ifndef kode_string_utils_included
#define kode_string_utils_included
//----------------------------------------------------------------------


// ptr to (reversed) hex

void KODE_CreateUniqueName(const char* ABuffer, const char* APrefix, void* APtr) {
  static char _int2hex[17] = "0123456789abcdef";
  char* buf = (char*)ABuffer;
  while (*APrefix != '\0') *buf++ = *APrefix++;
  uint64_t iptr = (uint64_t)APtr;
  while (iptr>0) {
    int j = iptr & 0x0f;
    iptr >>= 4;
    *buf++ = _int2hex[j];
  }
  *buf++ = '\0';
}

//----------

// https://stackoverflow.com/questions/7666509/hash-function-for-string

uint32_t KODE_HashString(const char *str) {
  uint32_t /*unsigned long*/ hash = 5381;
  int32_t /*int*/ c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

//----------

/*
  https://stackoverflow.com/questions/20540256/conversion-from-ascii-to-utf-16le-giving-issues

  #include <stdio.h>
  #include <string.h>

  int main() {
    char Input[20];
    char Output[100];
    size_t insize,out_size;
    memset(Input,0,sizeof(Input));
    memset(Output,0,sizeof(Output));
    int nconv=0;
    char *Inptr;
    char *outptr;
    printf("Input data :");
    scanf("%s",Input);
    iconv_t cd = iconv_open("UTF-16LE","ASCII");
    if(cd==(iconv_t)-1) {
      printf("iconv_open has failed ");
      return 0;
    }
    insize=strlen(Input);
    out_size=3*insize;
    Inptr =Input;
    outptr=(char *)Output;
    nconv=iconv(cd,&Inptr,&insize,&outptr,&out_size);
    if(nconv!=0) {
       printf("Unable to perform conversion ");
       return 0;
    }
    //printf("\n Data After conversion from ASCII to UTF-16 = %s \n ",Output);
  }
*/

/*
  char utf16[1024];
  char ascii[1024];
  KODE_Strcpy(ascii,"hello world");
  KODE_AsciiToUTF16(utf16,ascii);
  KODE_Trace("utf16: %s\n",utf16);
  KODE_UTF16ToAscii(ascii,utf16);
  KODE_Trace("ascii: %s\n",ascii);
*/

//#include <stdio.h>
//#include <string.h>
#include <iconv.h>

char* KODE_AsciiToUTF16(char* AUTF16, char* AAscii) {
  iconv_t cd = iconv_open("UTF-16","ASCII");
  if (cd == (iconv_t)-1) return KODE_NULL; // iconv_open failed
  size_t  in_size   = KODE_Strlen(AAscii);
  size_t  out_size  = 1024;
  char*   in_ptr    = AAscii;
  char*   out_ptr   = AUTF16;
  int nconv = iconv(cd,&in_ptr,&in_size,&out_ptr,&out_size);
  if (nconv != 0) return KODE_NULL; // unable to perform conversion
  return AUTF16;
}

//----------

char* KODE_UTF16ToAscii(char* AAscii, char* AUTF16) {
  iconv_t cd = iconv_open("ASCII","UTF-16");
  if (cd == (iconv_t)-1) return KODE_NULL; // iconv_open failed
  size_t  in_size   = KODE_Strlen(AAscii);
  size_t  out_size  = 1024;
  char*   in_ptr    = AUTF16;
  char*   out_ptr   = AAscii;
  int nconv = iconv(cd,&in_ptr,&in_size,&out_ptr,&out_size);
  if (nconv != 0) return KODE_NULL; // unable to perform conversion
  return AAscii;
}

//----------

// problem if there is a dot in the path
// todo: find last dot..
// (is this fixed? KODE_Strrchr..)

// modifies content of APath

char* KODE_StripFileExt(char* APath) {
  //DTrace("...stripping...");
  if (APath) {
    char *dot;
    dot = KODE_Strrchr(APath,'.');
    //if (dot) return dot + 1;
    if (dot) {
      //DTrace("dot");
      *dot = '\0';//return dot + 1;
      return APath;
    }
  }
  return (char*)"NULL";
}

//----------

char* KODE_GetFileExt(char* APath) {
  return KODE_NULL;
}


//----------

// /home/skei/test -> test

const char* KODE_StripPath(const char* APath) {
  if (APath) {
    const char *slash, *backslash;
    slash     = KODE_Strrchr(APath,'/');
    backslash = KODE_Strrchr(APath,'\\');//+1;
    if (slash) return slash + 1;
    else if (backslash) return backslash + 1; // see above
  }
  return (char*)"NULL";
}

const char* KODE_GetPath(const char* APath) {
  return KODE_NULL;
}

  //----------

// returns 'exe', not '.exe'

//const char* KODE_GetFileExt(/*const*/ char* APath) {
//  if (APath) {
//    char *dot;
//    dot = KODE_Strrchr(APath,'.');
//    if (dot) return dot+1;
//  }
//  return (char*)"NULL";
//}

//----------

// /home/skei/test -> /home/skei/

// modifies content of APath

//const char* KODE_GetPath(/*const*/ char* APath) {
//  if (APath) {
//    char *slash, *backslash;
//    slash     = KODE_Strrchr(APath,'/');
//    backslash = KODE_Strrchr(APath,'\\');//+1;
//    if (slash) *(slash+1) = '\0';
//    else if (backslash) *(backslash+1) = '\0'; // see above
//    return APath;
//  }
//  return KODE_NULL;
//}

//----------

/*
  http://rosettacode.org/wiki/Binary_search#Pascal
  Iterative
  Usage:
  var
    list: array[0 .. 9] of real;
    // ...
    indexof := binary_search(123, list);
*/

/*
function binary_search(element: real; list: array of real): integer;
var
    l, m, h: integer;
begin
    l := 0;
    h := High(list) - 1;
    binary_search := -1;
    while l <= h do
    begin
        m := (l + h) div 2;
        if list[m] > element then
        begin
            h := m - 1;
        end
        else if list[m] < element then
        begin
            l := m + 1;
        end
        else
        begin
            binary_search := m;
            break;
        end;
    end;
end;
*/

//----------

// -masm=intel

/*
uint32_t KODE_Rdtsc(void) {
  uint32_t cputime = 0;
  asm(".intel_syntax noprefix");
  asm ("fwait; rdtsc; mov [cputime], eax;");
  asm(".att_syntax prefix");
  return cputime;
}
*/

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

int KODE_IsAlpha(int c)     { return (c>='A' && c<='Z') || (c>='a' && c<='z'); }
int KODE_IsDigit(int c)     { return (c>='0' && c<='9'); }
int KODE_IsPrintable(int c) { return c>=' '; }
int KODE_IsSpace(int c)     { return ((c>=0x09 && c<=0x0D) || (c==0x20)); }
int KODE_IsUpper(int c)     { return (c>='A' && c<='Z'); }
int KODE_IsHexDigit(int c)  { return (c>='0' && c<='9') || (c>='A' && c<='F') || (c>='a' && c<='f'); }
int KODE_IsAlphaNum(int c)  { return KODE_IsAlpha(c) || KODE_IsDigit(c); }

//----------

int KODE_ToLower(int c) {
  if (c < 'A' || c > 'Z') return c;
  return c+0x20;
}

//----------

int KODE_ToUpper(int c) {
  if (c < 'a' || c > 'z') return c;
  return c-0x20;
}

// http://www.cprogramming.com/snippets/source-code/useful-c-string-functions

//----------

// remove specified characters from a string

void KODE_RemoveChars(char *str, char c) {
	char *pos;
	while( ( pos = KODE_Strchr(str,c) ) ) {
		KODE_Memmove(pos, pos + 1, KODE_Strlen(pos));
  }
}

//----------

// remove specified chunks from a string

void KODE_RemoveChunks(char *str, char *cnk) {
	char *pos;
	int clen = KODE_Strlen(cnk);
	while( ( pos = KODE_Strstr(str, cnk) ) ) {
		KODE_Memmove(pos, pos + clen, KODE_Strlen(pos) - clen + 1);
  }
}

//----------

// replace specified characters in a string

void KODE_ReplaceChars(char *str, char c1, char c2) {
	char *pos;
	while( ( pos = KODE_Strchr(str, c1) ) ) {
		*pos = c2;
  }
}

//----------

// replace specified chunks in a string
// (size-independent, just remember about memory)

void KODE_ReplaceChunks(char *str, char *cnk1, char *cnk2)
{
	char *pos;
	int clen1 = KODE_Strlen(cnk1);
	int clen2 = KODE_Strlen(cnk2);
	while( ( pos = KODE_Strstr(str,cnk1) ) )
	{
		KODE_Memmove(pos + clen2, pos + clen1, KODE_Strlen(pos) - clen1 + 1);
		KODE_Memcpy(pos, cnk2, clen2);
	}
}

//----------

//reverse a string

void KODE_Reverse(char *str) {
	int i;
	int len = KODE_Strlen(str) - 1;
	int mid = (len % 2) ? (len / 2) : ((len + 1) / 2);
	for (i=0; i<=mid; ++i) {
		char buf = str[i];
		str[i] = str[len - i];
		str[len - i] = buf;
	}
}

//----------

/*
  assume APos is valid
  not tested.. (esp for-loop ranges..)
*/

//----------

char* KODE_InsertChar(char* ABuffer, int32_t APos, char AChar) {
//var
//  i,len : longint;
  if (!ABuffer) return ABuffer;
  int32_t len = KODE_Strlen(ABuffer);
  for (int32_t i=(len+1); i>=(APos+1);i--) ABuffer[i] = ABuffer[i-1];
  ABuffer[APos] = AChar;
  return ABuffer;
}

//----------

char* KODE_DeleteChar(char* ABuffer, int32_t APos) {
  if (!ABuffer) return ABuffer;
  int32_t len = KODE_Strlen(ABuffer);
  for (int32_t i=APos; i<len; i++) ABuffer[i] = ABuffer[i+1];
  return ABuffer;
}

//----------------------------------------------------------------------

///*
//  assume APos is valid
//  // not tested.. (esp for-loop ranges..)
//*/
//
////----------
//
////void KODE_InsertChar(char* ABuffer, int32_t APos, char AChar) {
//char* KODE_InsertChar(char* ABuffer, int32_t APos, char AChar) {
////var
////  i,len : longint;
//  if (!ABuffer) return ABuffer;
//  int32_t len = KODE_Strlen(ABuffer);
//  for (int32_t i=(len+1); i>=(APos+1);i--) ABuffer[i] = ABuffer[i-1];
//  ABuffer[APos] = AChar;
//  return ABuffer;
//}
//
////----------
//
////void KODE_DeleteChar(char* ABuffer, int32_t KODE_APos) {
//char* KODE_DeleteChar(char* ABuffer, int32_t KODE_APos) {
//  if (!ABuffer) return ABuffer;
//  int32_t len = KODE_Strlen(ABuffer);
//  for (int32_t i=APos; i<len; i++) ABuffer[i] = ABuffer[i+1];
//  return ABuffer;
//}

//----------------------------------------------------------------------
#endif
