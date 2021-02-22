#ifndef kode_convert_included
#define kode_convert_included
//----------------------------------------------------------------------

// parts of the code by lubomir i ivanov (for axonlib)
// used with permission

//#define KODE_CONVERT_USE_STDLIB

//----------------------------------------------------------------------

#ifndef KODE_NO_STDLIB

  #include <stdio.h>  // sprintf
  #include <stdlib.h> // atof

  char* KODE_FloatToString(char* AString, float AValue, uint32_t ADigits=3) {
    char format_string[16];
    format_string[0] = '%';
    format_string[1] = '.';
    format_string[2] = 48 + ADigits;
    format_string[3] = 'f';
    format_string[4] = 0;
    sprintf(AString,format_string,AValue);
    return AString;
  }

  char* KODE_IntToString(char* AString, int32_t AValue) {
    char format_string[16];
    format_string[0] = '%';
    format_string[1] = 'i';
    format_string[2] = 0;
    sprintf(AString,format_string,AValue);
    return AString;
  }

  float KODE_StringToFloat(char* AString) {
    return atof(AString);
  }

  int32_t KODE_StringToInt(char* AString) {
    return atoi(AString);
  }
  
//----------------------------------------------------------------------
//#else // KODE_CONVERT_USE_STDLIB
#else // KODE_NO_STDLIB
//----------------------------------------------------------------------

  #include "base/kode_string_utils.h" // KODE_IsDigit

/*
  ASign:
    0 - show minus sign only
    1 - show minus or plus sign
    2 - show minus sign or indent positive values with one space
  AMaxLen:
    maximum number of characters including the sign and decimal point.
    allocated memory for AString should be >= AMaxLen
*/

char* KODE_FloatToString(char* AString, double AValue, int AMaxLen=16, const unsigned int ASign=0) {
    if (!AString) return (char*)"0";
    char* ret = AString;
    register int exp = 0;
    register int z; // CppCheck: scope of z can be reduced
    int j = 0;
    if (AValue < 0) {
        *AString++ = '-';
        j++;
        AValue = -AValue;
    }
    else {
        if (ASign == 1) { *AString++ = '+';  j++; }
        if (ASign == 2) { *AString++ = ' ';  j++; }
    }
    if (AValue) {
        while (AValue < 1.f) {
            AValue *= 10.f;
            exp--;
        }
        while ( (AValue >= 10.f && exp < AMaxLen) || exp < 0 ) {
            AValue *= 0.1f;
            exp++;
        }
    }
    if (exp > AMaxLen - ( j + 1 )) {
        AMaxLen -= j;
        while (AMaxLen--) *AString++ = '9';
    }
    else {
        while ( (exp > 0) && (exp <= AMaxLen) && j < AMaxLen-1 ) {
            *AString++ = '0' + (char)AValue;
            z = (int)AValue;
            AValue -= z;
            AValue *= 10.f;
            exp--;
            j++;
        }
        *AString++ = '0' + (char)AValue;
        z = (int)AValue;
        AValue -= z;
        AValue *= 10.f;
        j++;
        if (j < AMaxLen-1) {
            *AString++ = '.';
            j++;
            register unsigned int i = 0;
            while (j < AMaxLen) {
                *AString++ = '0' + (char)AValue;
                z = (int)AValue;
                AValue -= z;
                AValue *= 10.f;
                i++; // cppcheck: Variable 'i' is modified but its new value is never used
                j++;
            }
        }
    }
    /*
    // note: exponent output is disabled. instead it writes the maximum integer.
    if (exp != 0 && e) {
        *AString++ = 'e';
        if (exp < 0) {
            *AString++ = '-';
            exp = -exp;
        }
        else *AString++ = '+';
        register int expd10 = exp/10;
        *AString++ = '0' + expd10;
        *AString++ = '0' + (exp -= expd10 * 10);
    }
    */
    *AString++ = 0;
    return ret;
}

//----------

/*
      h_Itoa(string, intnumber, maximumchars, ABase, flag)
      ABase: 16 = hex, 10 = dec, 2 = bin, etc.
      ASign (works only for base = 10):
      0 - show minus sign only
      1 - show minus or plus sign
      2 - show minus sign or indent positive values with one space
      'maximumchars' is the maximum number of characters including the
      sign. maximum is 33, default is 10
      for base 2, intnumber=2147483647 requires string length=33
      for base 10 use 'maximumchars' = N to show N chars max
      (e.g. -9999, +9999)
      allocated memory for 'string' should be >= 'maximumchars'
*/

char* KODE_IntToString(char* AString, int AValue, unsigned int AMaxLen=16, unsigned int ABase=10, unsigned int ASign=0) {
    if (!AString || AMaxLen > 33) return (char*)"0";
    unsigned int v, p = 1;
    int i = 0;
    char* st = AString;
    char _t[33];
    char* t = _t;
    v = (unsigned int) AValue;
    if (ABase == 10) {
        if (AValue < 0) {
            v = -AValue;
            *st++ = '-';
            AMaxLen--;
        }
        else {
            if (ASign == 1) { *st++ = '+'; AMaxLen--; }
            if (ASign == 2) { *st++ = ' '; AMaxLen--; }
        }
    }
    while (i < (int)AMaxLen) {
        p *= 10;
        i++;
    }
    if (ABase == 10 && v >= (p-1)) {
        while (AMaxLen--)
            *st++ = '9';
    }
    else {
        while (v || t == _t) {
            i = v % ABase;
            v = v / ABase;
            if (i < 10) *t++ = '0' + i;
            else *t++ = 'A' + i - 10;
        }
        while (t > _t) *st++ = *--t;
    }
    *st = 0;
    return AString;
}

float KODE_StringToFloat(char* AString) {
    if (!AString) return 0.f;
    register float a = 0.f;
    register int e = 0;
    register unsigned int c;
    float _asign = 1.f;
    if ( AString[0] == '-' ) {
        _asign = -1.f;
        AString++; //*AString++;
    }
    while ( (c = *AString++) != '\0' && KODE_IsDigit(c) )
        a = a*10.f + (c - '0');
    if (c == '.')
        while ( (c = *AString++) != '\0' && KODE_IsDigit(c) ) {
            a = a*10.f + (c - '0');
            e = e-1;
        }
    if (c == 'e' || c == 'E') {
        int sign = 1;
        register int i = 0;
        c = *AString++;
        if (c == '+') c = *AString++;
        else if (c == '-') {
            c = *AString++;
            sign = -1;
        }
        while ( KODE_IsDigit(c) ) {
            i = i*10 + (c - '0');
            c = *AString++;
        }
        e += i*sign;
    }
    while (e > 0) {
        a *= 10.f;
        e--;
    }
    while (e < 0) {
        a *= 0.1f;
        e++;
    }
    return a*_asign;
}

int KODE_StringToInt(const char* AString) {
    if (!AString) return 0;
    const char digits[] = "0123456789";
    unsigned val = 0;
    int neg = 0;
    while (*AString == ' ' || *AString == '\t') AString++;
    if (*AString == '-') {
        neg = 1;
        AString++;
    }
    else if (*AString == '+') AString++;
    while (*AString) {
        const char *w;
        unsigned digit;
        w = KODE_Strchr(digits, *AString);
        if (!w) break;
        digit = (w - digits);
        val = val*10 + digit;
        AString++;
    }
    if (neg) return -val;
    return val;
}

#endif // KODE_CONVERT_USE_STDLIB

//----------------------------------------------------------------------
#endif
