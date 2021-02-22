{$include sphaera.inc}
unit s_markov;

{
  Dynamic Markov Compression (DMC)
  http://en.wikipedia.org/wiki/Dynamic_Markov_Compression
  http://plg1.cs.uwaterloo.ca/~ftp/dmc/

  i think there is a bug with this.. don't remember what it was, but
  in zenith, when i tried to reuse the class, or do a second conpress
  or something...
  todo: double-check original sources and translation!
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  PMarkovNode = ^SMarkovNode;
  SMarkovNode = record
    count : array[0..1] of Single;
    next  : array[0..1] of PMarkovNode;
  end;

  //----------

  SMarkov = class
  private
    rbuf        : PByte;
    rbufsize    : LongInt;
    rbufpos     : LongInt;
    wbuf        : PByte;
    //wbufsize  : LongInt;
    wbufpos     : LongInt;
    memsize     : LongInt;
    threshold   : LongInt;
    bigthresh   : LongInt;
    P           : PMarkovNode;
    N           : PMarkovNode;
    nodes       : array[0..255,0..255] of SMarkovNode;
    nodebuf     : PMarkovNode;
    nodemaxp    : PMarkovNode;
    nodesptr    : PMarkovNode;
  private
    function    get_char : LongInt;
    procedure   put_char(c:LongInt);
  public
    constructor create;
    destructor  destroy; override;
    procedure   init(msize:LongInt);
    procedure   cleanup;
    procedure   flush;
    procedure   reset;
    function    predict : Single;
    procedure   update(b:LongInt);
    function    compress(outbuf,inbuf:PByte; insize:LongInt) : LongInt;
    function    expand(outbuf,inbuf:PByte; insize:LongInt) : LongInt;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const;

const
  s_markov_eof       = -1;
  s_markov_threshold = 8;
  s_markov_bigthresh = 8;

//----------------------------------------------------------------------

constructor SMarkov.create;
begin
  inherited;
  rbuf := nil;
  rbufsize := 0;
  rbufpos := 0;
  wbuf := nil;
  //wbufsize := 0;
  wbufpos := 0;
  memsize  := $1000000;
  threshold := s_markov_threshold;//2;
  bigthresh := s_markov_bigthresh;//2;
  nodebuf := nil;
end;

//----------

destructor SMarkov.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------

function SMarkov.get_char : LongInt;
var
  c : Byte;
begin
  //return getchar();
  if rbufpos < rbufsize then
  begin
    c := rbuf[rbufpos];
    inc(rbufpos);
    result := LongInt(c);;
  end
  else result := s_markov_eof;//-1;
end;

//----------

procedure SMarkov.put_char(c:LongInt);
var
  ch : Byte;
begin
  //putchar(c);
  //if (wbufpos<wbufsize)
  //{
    ch := Byte(c);
    wbuf[wbufpos] := ch;
    inc(wbufpos);
    //return (int)ch;
  //}
  //else return h_EOF;//-1
end;

//----------------------------------------------------------------------

procedure SMarkov.init(msize:LongInt);
begin
  //fprintf(stderr,"using %d bytes of predictor memory\n",memsize);
  nodebuf := PMarkovNode( GetMem(msize) );
  //if (nodebuf == (h_dmc_node *)NULL)
  //{
  //  fprintf(stderr,"memory alloc failed; try smaller predictor memory\n");
  //  exit(1);
  //}
  nodemaxp := nodebuf + (msize div sizeof(PMarkovNode)) - 20;
  flush;
end;

//----------

procedure SMarkov.cleanup;
begin
  if Assigned(nodebuf) then FreeMem(nodebuf);
  nodebuf := nil;
end;

//----------

procedure SMarkov.flush;
var
  i,j : LongInt;
begin
  for j := 0 to 255 do
  begin
    for i := 0 to 126 do
    begin
      nodes[j][i].count[0] := 0.2;
      nodes[j][i].count[1] := 0.2;
      nodes[j][i].next[0]  := @nodes[j][2*i + 1];
      nodes[j][i].next[1]  := @nodes[j][2*i + 2];
    end;
    for i := 127 to 254 do
    begin
      nodes[j][i].count[0] := 0.2;
      nodes[j][i].count[1] := 0.2;
      nodes[j][i].next[0]  := @nodes[i+1][0];
      nodes[j][i].next[1]  := @nodes[i-127][0];
    end;
  end;
  nodesptr := nodebuf;
  reset;
end;

//----------

procedure SMarkov.reset;
begin
  P := @nodes[0][0];
end;

//----------

function SMarkov.predict : Single;
begin
  result := P^.count[0] / (P^.count[0] + P^.count[1]);
end;

//----------

procedure SMarkov.update(b:LongInt);
var
  r : Single;
  //temp : Single;
begin
  if (P^.count[b] >= threshold) and (P^.next[b]^.count[0]+P^.next[b]^.count[1] >= bigthresh + P^.count[b]) then
  begin
    N := nodesptr;
    inc(nodesptr);
    r := P^.count[b]/(P^.next[b]^.count[1]+P^.next[b]^.count[0]);
    N^.count[0] := P^.next[b]^.count[0] * r;
    P^.next[b]^.count[0] -= N^.count[0];// = P^.next[b]^.count[0] * r;
    N^.count[1] := P^.next[b]^.count[1] * r;
    P^.next[b]^.count[1] -= N^.count[1];// = P^.next[b]^.count[1] * r;
    N^.next[0] := P^.next[b]^.next[0];
    N^.next[1] := P^.next[b]^.next[1];
    P^.next[b] := N;
  end;
  P^.count[b] += 1;
  P := P^.next[b];
  if nodesptr > nodemaxp then
  begin
    //fprintf(stderr,"flushing ...\n");
    flush;
  end;
end;

//----------------------------------------------------------------------

function SMarkov.compress(outbuf,inbuf:PByte; insize:LongInt) : LongInt;
var
  max, min, inbytes, outbytes, pout : LongInt;
  c,i,bit,mid : LongInt;
begin
  rbuf     := inbuf;
  rbufsize := insize;
  rbufpos  := 0;
  wbuf     := outbuf;
  //wbufsize := 0;
  wbufpos  := 0;
  max := $1000000;
  min := 0;
  inbytes  := 0;
  outbytes := 3;
  pout     := 3;
  init(memsize);
  //TODO: for (0..buffersize)
  while true do // for(;;)
  begin
    c := get_char;
    if c = s_markov_eof then
    begin
      min := max-1;
      //fprintf(stderr,"compress done: bytes in %d, bytes out %d, ratio %f\n",inbytes,outbytes,(float)outbytes/inbytes);
      break;
    end;
    for i := 0 to 7 do
    begin
      bit := (c shl i) and $80;
      //Hint: Converting the operands to "Int64" before doing the subtract could prevent overflow errors.
      mid := LongInt(Single(min + (max-min-1)) * predict);
      if bit <> 0 then update(S_TRUE) else update(S_FALSE); // p_update(bit != 0);
      if mid = min then mid+=1;
      if mid = (max-1) then mid-=1;
      if (bit<>0) then min := mid // if (bit) ..
      else max := mid;
      while (max-min) < 256 do
      begin
        if (bit<>0) then max -= 1;
        put_char(min shr 16);
        outbytes += 1;
        min := (min shl 8) and $ffff00;
        max := ((max shl 8) and $ffff00 ) ;
        if min >= max then max := $1000000;
      end; //max min
    end; //i 0..7
    //if (!(++inbytes & 0xff))
    inbytes += 1;
    if (inbytes and $ff) = S_FALSE then
    begin
      //if (!(inbytes & 0xffff))
      //{
      //  //fprintf(stderr,"compressing... bytes in %d, bytes out %d, ratio %f\r",inbytes,outbytes,(float)outbytes/inbytes);
      //}
      if (outbytes-pout) > 256 then flush; // compression failing
      pout := outbytes;
    end; //inbytes
  end; // while true
  put_char( min shr 16);
  put_char((min shr 8) and $ff);
  put_char( min and $00ff);
  cleanup;
  result := wbufpos;
end;

//----------------------------------------------------------------------

//int expand(unsigned char*  unsigned char*  int insize)
function SMarkov.expand(outbuf,inbuf:PByte; insize:LongInt) : LongInt;
var
  max, min, val, pin, inbytes, outbytes : LongInt;
  c,i,mid,bit : LongInt;
begin
  rbuf     := inbuf;
  rbufsize := insize;
  rbufpos  := 0;
  wbuf     := outbuf;
  wbufpos  := 0;
  max := $1000000;
  min := 0;
  inbytes:=3;
  pin:=3;
  outbytes:=0;
  init(memsize);
  val  := get_char << 16;
  val += get_char << 8;
  val += get_char;
  while (true) do
  begin
    c := 0;
    if val = (max-1) then
    begin
      //fprintf(stderr,"expand: input %d output %d\n",inbytes,outbytes);
      break;
    end;
    for i := 0 to 7 do
    begin
      // Hint: Converting the operands to "Int64" before doing the subtract could prevent overflow errors.
      mid := LongInt(Single(min + (max-min-1)) * predict);
      if mid = min then mid += 1;
      if mid = (max-1) then mid -= 1;
      if val >= mid then
      begin
        bit := 1;
        min := mid;
      end
      else
      begin
        bit := 0;
        max := mid;
      end;
      if bit <> 0 then update(S_TRUE) else update(S_FALSE); // p_update(bit != 0);
      c := c + c + bit;
      while ((max-min) < 256) do
      begin
        if (bit<>0) then max -= 1;
        inbytes += 1;
        //val =  (val shl 8) & 0xffff00  | (get_char()& 0xff);
        val := ((val shl 8) and $ffff00) or (get_char and  $ff);
        min :=  (min shl 8) and $ffff00;
        max := ((max shl 8) and $ffff00);
        if (min>=max) then max := $1000000;
      end; //max min
    end; // for
    put_char(c);
    //if (!(++outbytes & 0xff)) then
    outbytes += 1;
    if (outbytes and $ff) = S_FALSE then
    begin
      if (inbytes - pin) > 256 then flush; // compression was failing
      pin := inbytes;
    end;
  end; //while
  cleanup;
  result := wbufpos;
end;

//----------------------------------------------------------------------
end.

