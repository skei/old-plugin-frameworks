{$include sphaera.inc}
unit s_string;

// uses
//   strings;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

function SStrcat(s1,s2:PChar) : PChar;
function SStrchr(s:PChar; c:Char) : PChar;
function SStrcmp(s1,s2:PChar) : longint;
function SStrcpy(dst,src:PChar) : PChar;
function SStrdup(src:PChar) : PChar;
function SStrlen(s:PChar) : integer;
function SStrncmp(s1,s2:PChar; n:longword) : longword;
function SStrncpy(dst,src:PChar; n:longword) : PChar;
function SStrrchr(s:PChar; c:Char) : PChar;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

function SStrcat(s1,s2:PChar) : PChar;
begin
  while s1^ <> #0 do inc(s1);
  while s2^ <> #0 do
  begin
    s1^ := s2^;
    inc(s1);
    inc(s2);
  end;
  s1^ := #0;
  inc(s1);
  result := s1;
end;

//----------

function SStrchr(s:PChar; c:Char) : PChar;
begin
  if assigned(s) then
  begin
    //while (s^<>#0) do inc(s);
    while (s^<>c) do inc(s);
  end;
  result := s;
end;

//----------

function SStrcmp(s1,s2:PChar) : longint;
begin
  while (s1^<>#0) and (s1^=s2^) do
  begin
    inc(s1);
    inc(s2);
  end;
  result := longint(s1^) - longint(s2^); // ???
end;

//----------

function SStrcpy(dst,src:PChar) : PChar;
begin
  //while dst^ = src^ do
  //while longint(src^) <> 0 do
  while src^ <> #0 do
  begin
    dst^ := src^;
    inc(dst);
    inc(src);
  end;
  dst^ := #0;
  inc(dst);
  result := dst;
end;

//----------

function SStrdup(src:PChar) : PChar;
begin
  result := GetMem(SStrlen(src)+1);
  if assigned(result) then SStrcpy(result,src);
end;

//----------

function SStrlen(s:PChar) : integer;
begin
  result := 0;
  while s^ <> #0 do
  begin
    inc(s);
    inc(result);
  end;
end;

//----------

function SStrncmp(s1,s2:PChar; n:longword) : longword;
var u1,u2:Byte;//Char;
begin
  while n>0 do
  begin
    dec(n);
    u1 := Byte(s1^);
    u2 := Byte(s2^);
    if u1 <> u2 then exit(u1-u2);
    if u1=0 then exit(0);
  end;
  result := 0;
end;

//----------

function SStrncpy(dst,src:PChar; n:longword) : PChar;
begin
  while (src^<>#0) and (n>0) do
  begin
    dst^ := src^;
    inc(dst);
    inc(src);
    dec(n);
  end;
  dst^ := #0;
  inc(dst);
  result := dst;
end;

//----------

function SStrrchr(s:PChar; c:Char) : PChar;
var p:Pchar = nil;
begin
  while s^ <> #0 do
  begin
    if s^ = c then p := s;
    inc(s);
  end;
  result := p;
end;

//----------------------------------------------------------------------
end.

// TODO


//char* h_Strncat (register char* s1, register char* s2, register unsigned int n)
//{
//  register char* s = s1;
//  while (*s1)
//    s1++;
//  while ( n-- && (*s1++ = *s2++) )
//    if (n == 0)
//      *s1 = '\0';
//  return s;
//}

//char* h_Strstr (register const char* s1, register const char* s2)
//{
//  register unsigned int i;
//  while (*s1++)
//  {
//    i = 0;
//    while (1)
//    {
//      if (!s2[i])
//        return (char*) s1;
//      if (s2[i] != s1[i])
//        break;
//      i++;
//    }
//  }
//  return (char*) NULL;
//}

//unsigned int h_Strspn (register const char* s1, register const char* s2)
//{
//  register const char *_s1, *_s2;
//  _s1 = s1;
//  while (*_s1)
//  {
//    _s2 = s2;
//    while (*_s2 && *_s2 != *_s1)
//      _s2++;
//    if (*_s2 == '\0')
//      break;
//    _s1++;
//  }
//  return _s1 - s1;
//}

//unsigned int h_Strcspn (register const char* s1, register const char* s2)
//{
//  register const char *_s1, *_s2;
//  _s1 = s1;
//  while (*_s1)
//  {
//    _s2 = s2;
//    while (*_s2 != *_s1 && *_s2)
//      _s2++;
//    if (*_s2)
//      break;
//    _s1++;
//  }
//  return _s1 - s1;
//}

//char* h_Strpbrk (register const char* s1, register const char* s2)
//{
//  register const char* _s1;
//  while (*s1++)
//  {
//    _s1 = s2;
//    while (*_s1 && *_s1 != *s1)
//      _s1++;
//    if (*_s1)
//      return (char*) s1;
//  }
//  return (char*) NULL;
//}

//char* h_Strtok (register char* str, const char* spr)
//{
//  register char *s1, *s2;
//  // static buffer = kaboom ?
//  static char* _save;
//  // --
//  if (str == NULL)
//  {
//    str = _save;
//    if (str == NULL)
//      return (char*) NULL;
//  }
//  s1 = str + h_Strspn(str, spr);
//  if (*s1 == '\0')
//    return (char*) (_save = NULL);
//  s2 = h_Strpbrk(s1, spr);
//  if (s2 != NULL)
//    *s2++ = '\0';
//  _save = s2;
//  return s1;
//}

//char* h_Strrev (register char* str)
//{
//  register unsigned int i = 0;
//  register unsigned int j = h_Strlen(str)-1;
//  while (i < j)
//  {
//    str[i] ^= str[j];
//    str[j] ^= str[i];
//    str[i] ^= str[j];
//    ++i;
//    --j;
//  }
//  return str;
//}

//----------------------------------------------------------------------
// conversation
//----------------------------------------------------------------------

{*
 * h_Itoa(string, intnumber, maximumchars, base, flag)
 *
 * base: 16 = hex, 10 = dec, 2 = bin, etc.
 *
 * flag (works only for base = 10):
 * 0 - show minus sign only
 * 1 - show minus or plus sign
 * 2 - show minus sign or indent positive values with one space
 *
 * 'maximumchars' is the maximum number of characters including the
 * sign. maximum is 33, default is 10
 * for base 2, intnumber=2147483647 requires string length=33
 * for base 10 use 'maximumchars' = N to show N chars max
 * (e.g. -9999, +9999)
 * allocated memory for 'string' should be >= 'maximumchars'
 *
 *}

//char* h_Itoa( register char* _st, int n, unsigned int maxlen = 10, unsigned int base = 10, unsigned int fg = 0)
//{
//  if (!_st || maxlen > 33) return (char*)"0";
//  register unsigned int v, p = 1;
//  register int i = 0;
//  char* st = _st;
//  char _t[33];
//  register char* t = _t;
//  v = (unsigned int) n;
//  if (base == 10)
//  {
//    if (n < 0)
//    {
//      v = -n;
//      *st++ = '-';
//      maxlen--;
//    }
//    else
//    {
//      if (fg == 1) { *st++ = '+'; maxlen--; }
//      if (fg == 2) { *st++ = ' '; maxlen--; }
//    }
//  }
//  while (i < (int)maxlen)
//  {
//    p *= 10;
//    i++;
//  }
//  if (base == 10 && v >= (p-1))
//  {
//    while (maxlen--)
//      *st++ = '9';
//  }
//  else
//  {
//    while (v || t == _t)
//    {
//      i = v % base;
//      v = v / base;
//      if (i < 10)
//        *t++ = '0' + i;
//      else
//        *t++ = 'a' + i - 10;
//    }
//    while (t > _t)
//      *st++ = *--t;
//  }
//  *st = 0;
//  return _st;
//}

//int h_Atoi (register const char* s)
//{
//  if (!s) return 0;
//  const char digits[] = "0123456789";
//  register unsigned val = 0;
//  register int neg = 0;
//  while (*s == ' ' || *s == '\t') s++;
//  if (*s == '-')
//  {
//    neg = 1;
//    s++;
//  }
//  else if (*s == '+') s++;
//  while (*s)
//  {
//    const char *w;
//    unsigned digit;
//    w = h_Strchr(digits, *s);
//    if (!w) break;
//    digit = (w - digits);
//    val = val*10 + digit;
//    s++;
//  }
//  if (neg) return -val;
//  return val;
//}

{*
 * h_Ftoa(string, floatnumber, maximumchars, flag)
 *
 * flag:
 * 0 - show minus sign only
 * 1 - show minus or plus sign
 * 2 - show minus sign or indent positive values with one space
 *
 * 'maximumchars' is the maximum number of characters including the
 * sign and decimal point.
 * allocated memory for 'string' should be >= 'maximumchars'
 *
 *}

//char* h_Ftoa( register char* st, register double f, register int maxlen = 5, const unsigned int fg = 0)
//, const bool e = false)
//{
//  if (!st) return (char*)"0";
//  char* ret = st;
//  register int exp = 0;
//  register int z;
//  int j = 0;
//  if (f < 0)
//  {
//   *st++ = '-';
//   j++;
//   f = -f;
//  }
//  else
//  {
//    if (fg == 1) { *st++ = '+';  j++; }
//    if (fg == 2) { *st++ = ' ';  j++; }
//  }
//  if (f)
//  {
//    while (f < 1.f)
//    {
//      f *= 10.f;
//      exp--;
//    }
//    while ( (f >= 10.f && exp < maxlen) || exp < 0 )
//    {
//      f *= 0.1f;
//      exp++;
//    }
//  }
//  if (exp > maxlen - ( j + 1 ))
//  {
//    maxlen -= j;
//    while (maxlen--) *st++ = '9';
//  }
//  else
//  {
//    while ( (exp > 0) && (exp <= maxlen) && j < maxlen-1 )
//    {
//      *st++ = '0' + (char)f;
//      z = (int)f;
//      f -= z;
//      f *= 10.f;
//      exp--;
//      j++;
//    }
//    *st++ = '0' + (char)f;
//    z = (int)f;
//    f -= z;
//    f *= 10.f;
//    j++;
//    if (j < maxlen-1)
//    {
//      *st++ = '.';
//      j++;
//      register unsigned int i = 0;
//      while (j < maxlen)
//      {
//        *st++ = '0' + (char)f;
//        z = (int)f;
//        f -= z;
//        f *= 10.f;
//        i++;
//        j++;
//      }
//    }
//  }
//  /*
//  // note: exponent output is disabled. instead it writes the maximum integer.
//  if (exp != 0 && e)
//  {
//    *st++ = 'e';
//    if (exp < 0)
//    {
//      *st++ = '-';
//      exp = -exp;
//    }
//    else *st++ = '+';
//    register int expd10 = exp/10;
//    *st++ = '0' + expd10;
//    *st++ = '0' + (exp -= expd10 * 10);
//  }
//  */
//  *st++ = 0;
//  return ret;
//}

//float h_Atof (register char* s)
//{
//  if (!s) return 0.f;
//  register float a = 0.f;
//  register int e = 0;
//  register unsigned int c;
//  float _asign = 1.f;
//  if ( s[0] == '-' )
//  {
//    _asign = -1.f;
//    s++; //*s++;
//  }
//  while ( (c = *s++) != '\0' && h_IsDigit(c) )
//    a = a*10.f + (c - '0');
//  if (c == '.')
//    while ( (c = *s++) != '\0' && h_IsDigit(c) )
//    {
//      a = a*10.f + (c - '0');
//      e = e-1;
//    }
//  if (c == 'e' || c == 'E')
//  {
//    int sign = 1;
//    register int i = 0;
//    c = *s++;
//    if (c == '+') c = *s++;
//    else if (c == '-')
//    {
//      c = *s++;
//      sign = -1;
//    }
//    while ( h_IsDigit(c) )
//    {
//      i = i*10 + (c - '0');
//      c = *s++;
//    }
//    e += i*sign;
//  }
//  while (e > 0)
//  {
//    a *= 10.f;
//    e--;
//  }
//  while (e < 0)
//  {
//    a *= 0.1f;
//    e++;
//  }
//  return a*_asign;
//}

//----------------------------------------------------------------------

{
Free Pascal supports the Delphi implementation of the PChar type. PChar is
defined as a pointer to a Char type, but allows additional operations. The
PChar type can be understood best as the Pascal equivalent of a C-style
null-terminated string, i.e. a variable of type PChar is a pointer that points
to an array of type Char, which is ended by a null-character (#0). Free Pascal
supports initializing of PChar typed constants, or a direct assignment. For
example, the following pieces of code are equivalent:

program one;
var p : PChar;
begin
  P := ’This is a null-terminated string.’;
  WriteLn (P);
end.

Results in the same as

program two;
const P : PChar = ’This is a null-terminated string.’
begin
  WriteLn (P);
end.

These examples also show that it is possible to write the contents of the
string to a file of type Text. The strings unit contains procedures and
functions that manipulate the PChar type as in the standard C library. Since it
is equivalent to a pointer to a type Char variable, it is also possible to do
the following:

Program three;
Var S : String[30];
    P : PChar;
begin
  S := ’This is a null-terminated string.’#0;
  P := @S[1];
  WriteLn (P);
end.

This will have the same result as the previous two examples. Null-terminated
strings cannot be added as normal Pascal strings. If two PChar strings must be
concatenated; the functions from the unit strings must be used.

However, it is possible to do some pointer arithmetic. The operators + and -
can be used to do operations on PChar pointers. In table (3.5), P and Q are of
type PChar, and I is of type Longint.

PChar pointer arithmetic

Operation Result

P + I     Adds I to the address pointed to by P.
I + P     Adds I to the address pointed to by P.
P - I     Substracts I from the address pointed to by P.
P - Q     Returns, as an integer, the distance between 2 addresses
          (or the number of characters between P and Q)
----------

PChar related functions

Most PChar functions are the same as their counterparts in the STRINGS unit.
The following functions are the same :

StrCat    : Concatenates two PChar strings.
StrComp   : Compares two PChar strings.
StrCopy   : Copies a PChar string.
StrECopy  : Copies a PChar string and returns a pointer to the terminating null byte.
StrEnd    : Returns a pointer to the terminating null byte.
StrIComp  : Case insensitive compare of 2 PChar strings.
StrLCat   : Appends at most L characters from one PChar to another PChar.
StrLComp  : Case sensitive compare of at most L characters of 2 PChar strings.
StrLCopy  : Copies at most L characters from one PChar to another.
StrLen    : Returns the length (exclusive terminating null byte) of a PChar string.
StrLIComp : Case insensitive compare of at most L characters of 2 PChar strings.
StrLower  : Converts a PChar to all lowercase letters.
StrMove   : Moves one PChar to another.
StrNew    : Makes a copy of a PChar on the heap, and returns a pointer to this copy.
StrPos    : Returns the position of one PChar string in another?
StrRScan  : returns a pointer to the last occurrence of on PChar string in another one.
StrScan   : returns a pointer to the first occurrence of on PChar string in another one.
StrUpper  : Converts a PChar to all uppercase letters.

The subsequent functions are different from their counterparts in STRINGS,
although the same examples can be used.
}



(*
  3.2.4
  Ansistrings

  Ansistrings are strings that have no length limit. They are reference counted
  and are guaranteed to be null terminated. Internally, an ansistring is
  treated as a pointer: the actual content of the string is stored on the heap,
  as much memory as needed to store the string content is allocated.
  This is all handled transparantly, i.e. they can be manipulated as a normal
  short string. Ansistrings can be defined using the predefined AnsiString type.

  Remark: The null-termination does not mean that null characters (char(0) or
  #0) cannot be used: the null-termination is not used internally, but is there
  for convenience when dealing with external routines that expect a
  null-terminated string (as most C routines do).

  If the {$H} switch is on, then a string definition using the regular String
  keyword and that doesn’t contain a length specifier, will be regarded as an
  ansistring as well. If a length specifier is present, a short string will be
  used, regardless of the {$H} setting.
  If the string is empty (”), then the internal pointer representation of the
  string pointer is Nil. If the string is not empty, then the pointer points to
  a structure in heap memory.
  The internal representation as a pointer, and the automatic null-termination
  make it possible to typecast an ansistring to a pchar. If the string is empty
  (so the pointer is Nil) then the compiler makes sure that the typecasted pchar
  will point to a null byte.

*)


