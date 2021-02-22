#ifndef kode_stdlib_str_included
#define kode_stdlib_str_included
//----------------------------------------------------------------------

#if 0

//------------------------------
// str 1
//------------------------------

const char* KODE_Strcat(char* s1, const char* s2) {
  while (*s1) s1++;
  while ((*s1++ = *s2++));
  return s1;
}

//char* KODE_Strchr (const char* str, const int c) {
//  while (*str && *str != c) str++;
//  if (*str != c) str = NULL;
//  return (char*) str;
//}

const char *KODE_Strchr(const char *str, int ch) {
  while (*str) {
    if (*str == ch) return str;
    str++;
  }
  return 0;
}

int KODE_Strcmp(const char* s1, const char* s2) {
  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*(unsigned char *) s1) - (*(unsigned char*) s2);
}

char* KODE_Strcpy(char* dest,const char* src) {
  while ((*dest++=*src++));
  return dest;
}

//char *KODE_Strdup(const char *src) {
//  if (!src) return 0;
//  char *dst = (char*)KODE_Malloc(strlen(src)+1);
//  strcpy(dst, src);
//  return dst;
//}

unsigned int KODE_Strlen(const char* str) {
  unsigned int num = 0;
  while (*str++) num++;
  return num;
}

char* KODE_Strncat(char* s1, char* s2, unsigned int n) {
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

char* KODE_Strncpy(char* dest, const char* src, unsigned int n) {
  while (n-- && (*dest++=*src++) != '\0');
  return dest;
}

char* KODE_Strrchr(const char* s, /*int*/char c) {
  char* p = KODE_NULL;
  while (*s++) if (*s == c) p = (char*)s;
  return p;
}

// http://rosettacode.org/wiki/Reverse_a_string#C
// This function assumes the passed pointer points to a valid, zero-terminated string

void KODE_Strrev(char *s) {
  char *t = s;
  while (*t != '\0') t++;
  while (s < t) {
    int c = *s;
    *s++ = *--t;
    *t = c;
  }
}

const char *KODE_Strstr(/*const*/ char *str, /*const*/ char *substr) {
  int str_len = SStrlen(str);
  int substr_len = SStrlen(substr);
  if (substr_len == 0) return str;
  if (str_len < substr_len) return 0;
  for (int i = 0; i < (int)(str_len-substr_len+1); i++) {
    if (!SStrcmp(&str[i], substr)) return (const char *)(&str[i]);
  }
  return 0;
}

//------------------------------
// str 2
//------------------------------

const char* KODE_Strcat(char* s1, const char* s2) {
  while (*s1) s1++;
  while ((*s1++ = *s2++));
  return s1;
}

//char* KODE_Strchr (const char* str, const int c) {
//  while (*str && *str != c) str++;
//  if (*str != c) str = NULL;
//  return (char*) str;
//}

const char *KODE_Strchr(const char *str, int ch) {
  while (*str) {
    if (*str == ch) return str;
    str++;
  }
  return 0;
}

int KODE_Strcmp(const char* s1, const char* s2) {
  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*(unsigned char *) s1) - (*(unsigned char*) s2);
}

char* KODE_Strcpy(char* dest,const char* src) {
  while ((*dest++=*src++));
  return dest;
}

//char *KODE_Strdup(const char *src) {
//  if (!src) return 0;
//  char *dst = (char*)KMalloc(strlen(src)+1);
//  strcpy(dst, src);
//  return dst;
//}

unsigned int KODE_Strlen(const char* str) {
  unsigned int num = 0;
  while (*str++) num++;
  return num;
}

char* KODE_Strncat(char* s1, char* s2, unsigned int n) {
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

char* KODE_Strncpy(char* dest, const char* src, unsigned int n) {
  while (n-- && (*dest++=*src++) != '\0');
  return dest;
}

char* KODE_Strrchr(const char* s, char c) {
  char* p = KODE_NULL;
  while (*s++) if (*s == c) p = (char*)s;
  return p;
}

// http://rosettacode.org/wiki/Reverse_a_string#C
// This function assumes the passed pointer points to a valid, zero-terminated string

void KODE_Strrev(char *s) {
  char *t = s;
  while (*t != '\0') t++;
  while (s < t) {
    int c = *s;
    *s++ = *--t;
    *t = c;
  }
}

const char *KODE_Strstr(char *str, char *substr) {
  int str_len = SStrlen(str);
  int substr_len = SStrlen(substr);
  if (substr_len == 0) return str;
  if (str_len < substr_len) return 0;
  for (int i = 0; i < (int)(str_len-substr_len+1); i++) {
    if (!SStrcmp(&str[i], substr)) return (const char *)(&str[i]);
  }
  return 0;
}

#endif // 0

//----------------------------------------------------------------------
#endif
