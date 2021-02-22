{$include sphaera.inc}
unit s_isaac;

// http://www.wolfgang-ehrhardt.de/misc_en.html

// todo: class-ify

//{$define CONST}     {const in proc declaration}
//{$define V7PLUS}    {TP7 or higher}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  isaac_ctx = record                    // context of random number generator
    randmem : array[0..255] of longint; // the internal state
    randrsl : array[0..255] of longint; // the results given to the user
    randa   : longint;                  // accumulator
    randb   : longint;                  // the last result
    randc   : longint;                  // counter, guarantees cycle >= 2^40
    nextres : longint;                  // the next result
    randidx : word;                     // the index in randrsl[]
  end;

  // Type cast to increment untyped pointer
  Ptr2Inc = pByte;

//----------

// Init context from randrsl[0]=seed, randrsl[i]=0 otherwise
procedure isaac_init (var ctx: isaac_ctx; seed: longint);
// Init context from randseed
procedure isaac_init0(var ctx: isaac_ctx);
// Init all context variables with separate seeds, klen: number of seeds
procedure isaac_inita(var ctx: isaac_ctx; const key: array of longint; klen: integer);
// Next step of PRNG
procedure isaac_next(var ctx: isaac_ctx);
// Read len bytes from the PRNG to dest
procedure isaac_read(var ctx: isaac_ctx; dest: pointer; len: longint);
// Next random positive longint
function  isaac_long(var ctx: isaac_ctx): longint;
// Next 32 bit random dword (cardinal or longint)
function  isaac_dword(var ctx: isaac_ctx): longint;
// Next random word
function  isaac_word(var ctx: isaac_ctx): word;
// Next random double [0..1) with 32 bit precision
function  isaac_double(var ctx: isaac_ctx): double;
// Next random double in [0..1) with full double 53 bit precision
function  isaac_double53(var ctx: isaac_ctx): double;
// Simple self-test of ISAAC PRNG
//function  isaac_selftest: boolean;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

// generate next 256 result values, ie refill randrsl

procedure isaac_generate(var ctx: isaac_ctx);
var
  x,y: longint;
  xi : integer absolute x;  {better performance for BIT16}
  i  : integer;
begin
  with ctx do
  begin
    inc(randc);
    inc(randb, randc);
    for i:=0 to 255 do
    begin
      case i and 3 of
        0: randa := randa xor (randa shl 13);
        1: randa := randa xor (randa shr  6);
        2: randa := randa xor (randa shl  2);
        3: randa := randa xor (randa shr 16);
      end;
      x := randmem[i];
      inc(randa,randmem[(i+128) and 255]);
      y := randmem[(xi shr 2) and 255] + randa + randb;
      randmem[i] := y;
      randb := randmem[(y shr 10) and 255] + x;
      randrsl[i] := randb;
    end;
    // reset result index
    randidx:=0;
  end;
end;

//----------

// Init state, use randrsl if flag=true

procedure internal_init(var ctx: isaac_ctx; flag: boolean);
var
  i,j: integer;
  m: array[0..7] of longint;
  // mix the array
  procedure Mix;
  begin
    m[0] := m[0] xor (m[1] shl 11); inc(m[3], m[0]); inc(m[1], m[2]);
    m[1] := m[1] xor (m[2] shr  2); inc(m[4], m[1]); inc(m[2], m[3]);
    m[2] := m[2] xor (m[3] shl  8); inc(m[5], m[2]); inc(m[3], m[4]);
    m[3] := m[3] xor (m[4] shr 16); inc(m[6], m[3]); inc(m[4], m[5]);
    m[4] := m[4] xor (m[5] shl 10); inc(m[7], m[4]); inc(m[5], m[6]);
    m[5] := m[5] xor (m[6] shr  4); inc(m[0], m[5]); inc(m[6], m[7]);
    m[6] := m[6] xor (m[7] shl  8); inc(m[1], m[6]); inc(m[7], m[0]);
    m[7] := m[7] xor (m[0] shr  9); inc(m[2], m[7]); inc(m[0], m[1]);
  end;
begin
  with ctx do
  begin
    randa := 0;
    randb := 0;
    randc := 0;
    for i:=0 to 7 do m[i] := longint($9e3779b9); {the golden ratio}
    for i:=0 to 3 do Mix;
    i := 0;
    while i<256 do
    begin
      // fill in randmem[] with messy stuff
      if flag then
      begin
        // use all the information in the seed
        for j:=0 to 7 do inc(m[j], randrsl[i+j]);
      end;
      Mix;
      move(m, randmem[i], sizeof(m));
      inc(i,8);
    end;
    if flag then
    begin
      // do a second pass to make all of the seed affect all of randmem
      i := 0;
      while i<256 do
      begin
        for j:=0 to 7 do inc(m[j], randmem[i+j]);
        Mix;
        move(m, randmem[i], sizeof(m));
        inc(i,8);
      end;
    end;
    // generate first set of results
    isaac_generate(ctx);
    // prepare to use the first set of results
    randidx := 0;
  end;
end;

//----------------------------------------------------------------------

// Init context from randrsl[0]=seed, randrsl[i]=0 otherwise

procedure isaac_init(var ctx: isaac_ctx; seed: longint);
begin
  with ctx do
  begin
    fillchar(randrsl, sizeof(randrsl),0);
    randrsl[0] := seed;
  end;
  internal_init(ctx, true);
end;

//----------

// Init context from randseed and randrsl[i]:=random

procedure isaac_init0(var ctx: isaac_ctx);
var
  i,j: integer;
  tl: longint;
  ta: packed array[0..3] of byte absolute tl;
begin
  with ctx do
  begin
    for i:=0 to 255 do
    begin
      for j:=0 to 3 do ta[j] := byte(random(256));
      randrsl[i] := tl;
    end;
  end;
  internal_init(ctx, true);
end;

//----------

// Init all context variables with separate seeds, klen: number of seeds

procedure isaac_inita(var ctx: isaac_ctx; const key: array of longint; klen: integer);
var
  i: integer;
begin
  with ctx do
  begin
    for i:=0 to 255 do
    begin
      if i<klen then randrsl[i]:=key[i] else randrsl[i]:=0;
    end;
  end;
  internal_init(ctx, true);
end;

//----------

// Next step of PRNG

procedure isaac_next(var ctx: isaac_ctx);
begin
  with ctx do
  begin
    if randidx>255 then isaac_generate(ctx);
    nextres := randrsl[randidx];
    inc(randidx);
  end;
end;

//----------

// Read len bytes from the PRNG to dest

procedure isaac_read(var ctx: isaac_ctx; dest: pointer; len: longint);
type
  plong = ^longint;
begin
  // not optimized
  while len>3 do
  begin
    isaac_next(ctx);
    plong(dest)^ := ctx.nextres;
    inc(Ptr2Inc(dest),4);
    dec(len, 4);
  end;
  if len>0 then
  begin
    isaac_next(ctx);
    move(ctx.nextres, dest^, len and 3);
  end;
end;

//----------

// Next random positive longint

function isaac_long(var ctx: isaac_ctx): longint;
begin
  isaac_next(ctx);
  isaac_long := ctx.nextres shr 1;
end;

//----------

// Next 32 bit random dword (cardinal or longint)

function isaac_dword(var ctx: isaac_ctx): longint;
begin
  isaac_next(ctx);
  isaac_dword := ctx.nextres;
end;

//----------

// Next random word

function isaac_word(var ctx: isaac_ctx): word;
type
  TwoWords = packed record
    L,H: word
  end;
begin
  isaac_next(ctx);
  isaac_word := TwoWords(ctx.nextres).H;
end;

//----------

// Next random double [0..1) with 32 bit precision

function isaac_double(var ctx: isaac_ctx): double;
begin
  isaac_next(ctx);
  isaac_double := (ctx.nextres + 2147483648.0) / 4294967296.0;
end;

//----------

// Next random double in [0..1) with full double 53 bit precision

function isaac_double53(var ctx: isaac_ctx): double;
var
  hb,lb: longint;
begin
  isaac_next(ctx);
  hb := ctx.nextres shr 5;
  isaac_next(ctx);
  lb := ctx.nextres shr 6;
  isaac_double53 := (hb*67108864.0+lb)/9007199254740992.0;
end;

//----------

// Simple self-test of ISAAC PRNG

(*
function isaac_selftest: boolean;
var
  ctx: isaac_ctx;
begin
  fillchar(ctx,sizeof(ctx),0);
  internal_init(ctx, true);
  isaac_generate(ctx);
  // check first and last longint of randvec.txt
  if ctx.randrsl[0]<>longint($f650e4c8) then
  begin
    isaac_selftest := false;
    exit;
  end;
  isaac_generate(ctx);
  isaac_selftest := ctx.randrsl[255] = longint($4bb5af29);
end;
*)

//----------------------------------------------------------------------
end.



