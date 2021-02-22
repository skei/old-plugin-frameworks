unit z_queue;
{$include zenith.inc}

{
  lock-free queue
  based on /src/extern/flqueue.pas
  (from http://www.emadar.com/fpc/lockfree.htm)

  single producer/consumer

  TODO?:
  - 'Hint: Conversion between ordinals and pointers is not portable'
  - would be nice to have a way to check if an item is already in the queue
    before adding it..
}


//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  classes;

type

  generic ZQueue<_T> = class
  private
    FBuffer     : array of _T;
    FBufferSize : PtrUInt; //  longword;
    FBufferMask : PtrUInt;
    FHead       : PtrUInt;
    FTail       : PtrUInt;
    FTemp       : {integer}PtrUInt;
    procedure   setObject(lp:{integer}PtrUInt;const AObject:_T);
    function    getObject(lp:{integer}PtrUInt):_T;
  public
    constructor create(APower:{integer}PtrUInt); {allocate FBuffer with size equal 2^aPower}
    procedure   push(const tm:_T);
    function    pop(var tm:_T):boolean;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------


constructor ZQueue.create(APower:{integer}PtrUInt);
begin
  FBufferMask := not ($FFFFFFFF shl APower);
  FBufferSize := 1 shl aPower;
  setLength(FBuffer,FBufferSize);
  FHead := 0;
  FTail := 0;
  FTemp := 0;
end;

//----------

procedure ZQueue.setObject(lp:{integer}PtrUInt; const AObject:_T);
begin
  FBuffer[lp and FBufferMask] := AObject;
end;

//----------

function ZQueue.getObject(lp:{integer}PtrUInt) : _T;
begin
  result := FBuffer[lp and FBufferMask];
end;

//----------

procedure ZQueue.push(const tm : _T);
var
  newTemp   : {integer}PtrUInt;
  lastTail  : {integer}PtrUInt;
  newTail   : {integer}PtrUInt;
begin
  newTemp := interlockedIncrement(FTemp);
  lastTail := newTemp - 1;
  setObject(lastTail,tm);
  repeat
    //pointer(newTail) := InterlockedCompareExchange( pointer(FTail), pointer(newTemp), pointer(lastTail) );
    newTail := InterlockedCompareExchange( FTail, newTemp, lastTail );
  until ( newTail = lastTail );
end;

//----------

function ZQueue.pop(var tm:_T):boolean;
var
  newhead   : {integer}PtrUInt;
  lastHead  : {integer}PtrUInt;
begin
  repeat
    lastHead := FHead;
    if FTail <> FHead then
    begin
      //pointer(newHead) := InterlockedCompareExchange( pointer(FHead), pointer(lastHead+1), pointer(lasthead) );
      newHead := InterlockedCompareExchange( FHead, lastHead+1, lasthead );
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

//----------------------------------------------------------------------
end.

