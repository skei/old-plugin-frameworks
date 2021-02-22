unit z_memory;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

procedure ZMemcpy(dst,src:Pointer; count:Integer); inline;
function  ZMemmove(dst,src:Pointer; n:Cardinal): Pointer; inline;
procedure ZMemset(P:Pointer; B:Integer; count:Integer); inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

procedure ZMemcpy(dst,src:Pointer; count:Integer); inline;
begin
  Move(src^,dst^,count);
end;

//----------

function ZMemmove(dst,src:Pointer; n:Cardinal): Pointer; inline;
begin
  Move(src^,dst^,n);
  Result := dst;
end;

//----------

procedure ZMemset(P:Pointer; B:Integer; count:Integer); inline;
begin
  FillChar(P^,count,B);
end;

//----------------------------------------------------------------------

//void* h_Memchr (register const void* src, int c, unsigned int len)
//{
//  register const unsigned char* _src = (const unsigned char*) src;
//  c &= 0xff;
//  while (len--)
//  {
//    if (*_src == c) return (char*) _src;
//    _src++;
//  }
//  return NULL;
//}

//unsigned int h_Memcmp (register const void* m1, register const void* m2, register unsigned int n)
//{
//  register unsigned char* s1 = (unsigned char*) m1;
//  register unsigned char* s2 = (unsigned char*) m2;
//  while (n--)
//  {
//    if (*s1 != *s2) return *s1 - *s2;
//    s1++;
//    s2++;
//  }
//  return 0;
//}

//void* h_Memcpy (register void* dest, register const void* src, register unsigned int len)
//{
//  register char* _d = (char*) dest;
//  register char* _s = (char*) src;
//  while (len--) *_d++ = *_s++;
//  return dest;
//}

//void* h_Memmove (register void* dst, register const void* src, unsigned int len)
//{
//  register char* _dst = (char*) dst;
//  register const char* _src = (char*) src;
//  if (_src < _dst && _dst < _src + len)
//  {
//    _src += len;
//    _dst += len;
//    while (len--) *--_dst = *--_src;
//  }
//  else
//    while (len--) *_dst++ = *_src++;
//  return dst;
//}

//void* h_Memset (register void* dest, register int val, register unsigned int len)
//{
//  register unsigned char _v = (unsigned char) val;
//  register char* _d = (char*) dest;
//  while (len--) *_d++ = _v;
//  return dest;
//}

//----------------------------------------------------------------------
end.
