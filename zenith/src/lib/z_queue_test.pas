unit z_queue;
{$include zenith.inc}

{
  http://www.emadar.com/fpc/lockfree.htm
  http://pages.videotron.com/aminer/

  SPSC = single producer,   single conosumer
  SPMC = single producer,   multiple conosumer
  MPMC = multiple producer, multiple conosumer

  todo:
    check that 1000 limit in mpmc, we only need a few threads
    is the min-10-power because of this too?
    2^10 = 1024, larger than 1000
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------


uses
  SysUtils;

// limited to 1000 threads...
const
  //margin = 1000;
  margin = 16;

type

  // based on /src/extern/flqueue.pas

  generic ZQueueSPSC<_TSPSC> = class
  private
    FArray      : array of _TSPSC;
    FSize       : longword;
    FMask       : longword;
    FTail       : longword;
    FHead       : longword;
    FTemp       : longword;
  private
    procedure   setObject(lp:longword; const aobject:_TSPSC);
    function    getObject(lp:longword) : _TSPSC;
    //function    isEmpty:boolean;
  public
    constructor create(APower:longword);
    destructor  destroy; override;
    procedure   write(const tm:_TSPSC);
    function    read(var tm:_TSPSC) : boolean;
  //public
  //  property    empty : boolean read isEmpty;
  end;

  //----------

  // based on /src/ext/lockfree_spmc.pas

  //ZQueueSPMCCache = array[0..15] of LongWord;

  generic ZQueueSPMC<_TSPMC> = class
  private
    tail        : array[0..15] of LongWord; //ZQueueSPMCCache;
    head        : array[0..15] of LongWord; //ZQueueSPMCCache;
    fMask       : longword;
    fSize       : longword; // ths
    tab         : array of _TSPMC;
  private
    function    CAS(var Target:LongWord; Comperand:LongWord; NewValue:LongWord): Boolean; assembler; stdcall;
    function    getLength:longword;
    function    getObject(lp : longword) : _TSPMC;
    function    getSize:longword;
    function    lockedInc(var Target: longword): longword; assembler;
    procedure   setobject(lp : longword;const aobject:_TSPMC);
  public
    //allocate tab with size equal 2^aPower, for 20 size is equal 1048576
    constructor create(aPower:longword=10);
    destructor  Destroy; override;
    function    write(tm:_TSPMC) : boolean;
    function    read(var obj:_TSPMC) : boolean;
  //property    length : longword read getLength;
  //property    size : longword read getSize;
  end;

  //----------

  // based on /src/ext/lockfree_mpmc.pas

  //ZQueueMPMCCache = array[0..15] of LongWord;

  generic ZQueueMPMC<_TMPMC> = class
  private
    FTail       : array[0..15] of LongWord; //ZQueueMPMCCache;
    FHead       : array[0..15] of LongWord; //ZQueueMPMCCache;
    FMask       : longword;
    FSize       : longword;
    FTemp       : longword;
    FArray      : array of _TMPMC;
  private
    function    CAS(var Target:longword; Comperand:longword; NewValue:longword) : Boolean; assembler; stdcall;
    function    lockedIncLong(var Target:longword) : LongWord;
    function    getLength : longword;
    function    getObject(lp:longword) : _TMPMC;
    function    getSize : longword;
    procedure   setobject(lp:longword; const aobject:_TMPMC);
  public
    // allocate tab with size equal 2^aPower, for 20 size is equal 1048576
    constructor create(aPower:longword=10);
    destructor  destroy; override;
    function    write(tm:_TMPMC) : boolean;
    function    read(var obj:_TMPMC) : boolean;
  //property    size : longword read getLength;
  //property    arraySize : longword read getSize;
  //property    count  : longword read getLength;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses z_debug;

//----------------------------------------------------------------------
// SPSC
//----------------------------------------------------------------------

// allocate tab with size equal 2^aPower

constructor ZQueueSPSC.create(APower:longword);
begin
  //ZTrace('ZQueue.create');
  FSize := 1 shl APower;
  FMask := not($FFFFFFFF shl APower);
  setLength(FArray,FSize);
  FTail := 0;
  FHead := 0;
  FTemp := 0;
  //ZTrace('FSize: ',[FSize]);
  //ZTrace('FMask: ',[FSize]);
end;

//----------

// ccernn:
// no destructor on flqueue.pas...
// do we want one? are these arrays ref-counted?
// manual: dynamic arrays are reference counted.
//         the memory will be disposed of at the exit of the current proc/func

destructor ZQueueSPSC.destroy;
begin
  setLength(FArray,0);
  //inherited;
end;

//----------

procedure ZQueueSPSC.setObject(lp:longword; const AObject:_TSPSC);
begin
  FArray[lp and FMask] := AObject;
end;

//----------

function ZQueueSPSC.getObject(lp:longword) : _TSPSC;
begin
  result := FArray[lp and FMask];
end;

//----------

procedure ZQueueSPSC.write(const tm:_TSPSC);
var
  newTemp,lastTail,newTail : longword;
begin
  newTemp := interlockedIncrement(FTemp);
  lastTail := newTemp - 1;
  setObject(lastTail,tm);
  repeat
    pointer(newTail) := interlockedCompareExchange(pointer(FTail),pointer(newTemp),pointer(lastTail));
  until (newTail = lastTail);
end;

//----------

function ZQueueSPSC.read(var tm:_TSPSC):boolean;
var
  newhead,lastHead : longword;
begin
  repeat
    lastHead := FHead;
    if FTail <> FHead then
    begin
      pointer(newHead) := interlockedCompareExchange(pointer(FHead),pointer(lastHead+1),pointer(lasthead));
      if newHead = lastHead then
      begin
        tm := getObject(lastHead);
        result := true;
        exit;
      end;
    end
    else
    begin
      result := false;
      exit;
    end;
  until false;
end;

//----------

(*
function ZQueueSPSC.isEmpty:boolean;
begin
  //result := (MTail=MHead);
  if FTail = FHead then result := true
  else result := false;
end;
*)

//----------------------------------------------------------------------

//----------------------------------------------------------------------
// ZQueueSPMC
//----------------------------------------------------------------------

constructor ZQueueSPMC.create(aPower:longword);
begin
  fMask := not ($FFFFFFFF shl aPower);
  fSize := 1 shl aPower;
  setLength(tab,fSize);
  tail[0] := 0;
  head[0] := 0;
end;

//----------

destructor ZQueueSPMC.Destroy;
begin
  inherited Destroy;
end;

//----------

function ZQueueSPMC.CAS(var Target:longword; Comperand:longword; NewValue:longword) : boolean; assembler; stdcall;
asm
  mov ecx,Target
  mov edx,NewValue
  mov eax,Comperand
  lock cmpxchg [ecx],edx
  JNZ @@2
  MOV AL,01
  JMP @@Exit
  @@2:
  XOR AL,AL
  @@Exit:
end;

//----------

function ZQueueSPMC.getLength : longword;
var
  head1,tail1 : longword;
begin
  head1 := head[0];
  tail1 := tail[0];
  if tail1 < head1 then result := (High(LongWord) - head1) + (1 + tail1)
  else result := (tail1-head1);
end;

//----------

function ZQueueSPMC.getObject(lp : longword) : _TSPMC;
begin
  result := tab[lp and fMask];
end;

//----------

function ZQueueSPMC.getSize:longword;
begin
  result := fSize;
end;

//----------

// 'assembler' added by ths
function ZQueueSPMC.lockedInc(var Target:longword) : longword; assembler;
asm
  MOV ECX,EAX
  MOV EAX,1
  LOCK XADD [ECX],EAX
  INC EAX
end;

//----------

function ZQueueSPMC.read(var obj:_TSPMC) : boolean;
var
  lastHead : longword;
begin
  repeat
    lastHead := head[0];
    if tail[0] <> head[0] then
    begin
      obj := getObject(longword(lastHead));
      if CAS(head[0],lasthead,lasthead+1) then
      begin
        result := true;
        exit;
      end;
    end
    else
    begin
      result := false;
      exit;
    end;
  until false;
end;

//----------

procedure ZQueueSPMC.setObject(lp:longword;const aobject:_TSPMC);
begin
  tab[lp and fMask] := aObject;
end;

//----------

function ZQueueSPMC.write(tm:_TSPMC):boolean;
begin
  result := true;
  if getlength >= fsize then // Hint: Mixing signed expressions and longwords gives a 64bit result
  begin
    result := false;
    exit;
  end;
  setObject(Tail[0],tm);
  inc(tail[0]);
end;

//----------------------------------------------------------------------
// ZQueueMPMC
//----------------------------------------------------------------------

constructor ZQueueMPMC.create(aPower:longword);
begin
  // ths
  //if aPower < 10 then
  //begin
  //  //writeln('Power must be superior or equal to 10');
  //  exit;
  //end;
  fMask := not ($FFFFFFFF shl aPower);
  fSize := (1 shl aPower) - margin;
  setLength(FArray,1 shl aPower);
  FTail[0] := 0;
  FHead[0] := 0;
  FTemp := 0;
end;

//----------

destructor ZQueueMPMC.Destroy;
begin
  SetLength(FArray,0);
  inherited Destroy;
end;

//----------

function ZQueueMPMC.CAS(var Target:longword; Comperand:longword; NewValue:longword) : boolean; assembler; stdcall;
//begin
asm
  mov ecx,Target
  mov edx,NewValue
  mov eax,Comperand
  sfence
  lock cmpxchg [ecx],edx
  JNZ @@2
  MOV AL,01
  JMP @@Exit
  @@2:
  XOR AL,AL
  @@Exit:
//end;
end;

//----------

function ZQueueMPMC.getLength : longword;
var
  head1,tail1:longword;
begin
  head1 := FHead[0];
  tail1 := FTail[0];
  if tail1 < head1 then result := (High(LongWord) - head1) + (1 + tail1)
  else result := (tail1 - head1);
end;

//----------

function ZQueueMPMC.getObject(lp:longword) : _TMPMC;
begin
  result := FArray[lp and FMask];
end;

//----------

function ZQueueMPMC.getSize : longword;
begin
  result := FSize;
end;

//----------

function ZQueueMPMC.LockedIncLong(var Target: longword) : longword; assembler; // 'assembler' added by ths
//begin
asm
  {$IFDEF CPU32}
  // --> EAX Target
  // <-- EAX Result
  MOV ECX,EAX
  MOV EAX,1
  sfence
  LOCK XADD [ECX],EAX
  INC EAX
  {$ENDIF CPU32}
  {$IFDEF CPU64}
  // --> RCX Target
  // <-- EAX Result
  MOV EAX,1
  sfence
  LOCK XADD [RCX],EAX
  INC EAX
  {$ENDIF CPU64}
//end;
end;

//----------

function ZQueueMPMC.read(var obj:_TMPMC) : boolean;
var
  lastHead : LongWord;
begin
  repeat
    lastHead := FHead[0];
    if FTail[0] <> FHead[0] then
    begin
      obj := getObject(lastHead);
      if CAS(FHead[0],lasthead,lasthead+1) then
      begin
        result := true;
        exit;
      end;
    end
    else
    begin
      result := false;
      exit;
    end;
  until false;
end;

//----------

procedure ZQueueMPMC.setObject(lp:LongWord;const AObject:_TMPMC);
begin
  FArray[lp and FMask] := AObject;
end;

//----------

function ZQueueMPMC.write(tm:_TMPMC) : boolean; //stdcall;
var
  lasttail,newtemp : longword;
begin
  if getlength >= FSize then
  begin
    result := false;
    Exit;
  end;
  result := true;
  //newTemp:=windows.interlockedincrement(temp);
  newTemp := LockedIncLong(FTemp);
  lastTail := newTemp - 1;
  setObject(lastTail,tm);
  repeat
    if CAS(FTail[0],lasttail,newtemp) then
    begin
      //{$IFDEF MUTIPLE_PRODUCER}
      // sleep(0);
      //{$ENDIF}
      Exit;
    end;
    Sleep(0);
  until false;
end;

//----------------------------------------------------------------------
end.

  (*
    function InterlockedCompareExchange(var Target:Pointer; NewValue:Pointer; Comperand:Pointer) : Pointer;
    + LongInt, Cardinal (unsigned 32-bits integer)

    InterlockedCompareExchange does an compare-and-exchange operation on the
    specified values in a thread-safe way. The function compares Target and
    Comparand and exchanges Target with NewValue if Target and Comparand are
    equal. It returns the old value of Target. This is done in a thread-safe way,
    i.e., only one processor is accessing the Target variable at a time.
  *)


