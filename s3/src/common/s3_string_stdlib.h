#ifndef s3_string_stdlib_included
#define s3_string_stdlib_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#ifndef S3_REPLACE_STDLIB

  #include <string.h>
  #define S3_Strcat   strcat
  #define S3_Strchr   strchr
  #define S3_Strcmp   strcmp
  #define S3_Strcpy   strcpy
  #define S3_Strlen   strlen
  #define S3_Strncat  strncat
  #define S3_Strncpy  strncpy
  #define S3_Strrchr  strrchr
  #define S3_Strrev   strrev
  #define S3_Strstr   strstr

#else // S3_REPLACE_STDLIB

  const char* S3_Strcat(char* s1, const char* s2);
  const char *S3_Strchr(const char *str, int ch);
  int S3_Strcmp(const char* s1, const char* s2);
  char* S3_Strcpy(char* dest,const char* src);
  unsigned int S3_Strlen(const char* str);
  char* S3_Strncat(char* s1, char* s2, unsigned int n);
  char* S3_Strncpy(char* dest, const char* src, unsigned int n);
  char* S3_Strrchr(const char* s, char c);
  void S3_Strrev(char *s);
  const char *S3_Strstr(/*const*/ char *str, /*const*/ char *substr);

#endif // S3_REPLACE_STDLIB

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

#ifdef S3_REPLACE_STDLIB

const char* S3_Strcat(char* s1, const char* s2) {
  while (*s1) s1++;
  while ((*s1++ = *s2++));
  return s1;
}

//char* S3_Strchr (const char* str, const int c) {
//  while (*str && *str != c) str++;
//  if (*str != c) str = NULL;
//  return (char*) str;
//}

const char *S3_Strchr(const char *str, int ch) {
  while (*str) {
    if (*str == ch) return str;
    str++;
  }
  return 0;
}

int S3_Strcmp(const char* s1, const char* s2) {
  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*(unsigned char *) s1) - (*(unsigned char*) s2);
}

char* S3_Strcpy(char* dest,const char* src) {
  while ((*dest++=*src++));
  return dest;
}

//char *S3_Strdup(const char *src) {
//  if (!src) return 0;
//  char *dst = (char*)S3_Malloc(strlen(src)+1);
//  strcpy(dst, src);
//  return dst;
//}

unsigned int S3_Strlen(const char* str) {
  unsigned int num = 0;
  while (*str++) num++;
  return num;
}

char* S3_Strncat(char* s1, char* s2, unsigned int n) {
  char* s = s1;
  while (*s1) s1++;
  while ( n-- && (*s1++ = *s2++) ) if (n == 0) *s1 = '\0';
  return s;
}

//int strncmp(const char *s1, const char *s2, size_t n) {
//  if (!n) return 0;
//  const unsigned char *p1 = (const unsigned char*)s1;
//  const unsigned char *p2 = (const unsigned char*)s2;
//  for (size_t i = 0; i < n; i++) {
//    if (!p1[i] || p1[i] != p2[i]) return p1[i] - p2[i];
//  }
//  return 0;
//}

char* S3_Strncpy(char* dest, const char* src, unsigned int n) {
  while (n-- && (*dest++=*src++) != '\0');
  return dest;
}

char* S3_Strrchr(const char* s, /*int*/char c) {
  char* p = S3_NULL;
  while (*s++) if (*s == c) p = (char*)s;
  return p;
}

// http://rosettacode.org/wiki/Reverse_a_string#C
// This function assumes the passed pointer points to a valid, zero-terminated string

void S3_Strrev(char *s) {
  char *t = s;
  while (*t != '\0') t++;
  while (s < t) {
    int c = *s;
    *s++ = *--t;
    *t = c;
  }
}

const char *S3_Strstr(/*const*/ char *str, /*const*/ char *substr) {
  int str_len = SStrlen(str);
  int substr_len = SStrlen(substr);
  if (substr_len == 0) return str;
  if (str_len < substr_len) return 0;
  for (int i = 0; i < (int)(str_len-substr_len+1); i++) {
    if (!SStrcmp(&str[i], substr)) return (const char *)(&str[i]);
  }
  return 0;
}

#endif // S3_REPLACE_STDLIB

//----------------------------------------------------------------------
#endif
