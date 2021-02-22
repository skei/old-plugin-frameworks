unit z_events;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZEvent = record
    FIndex  : LongInt;
    FValue  : Single;
    //FData   : PtrUInt;
  end;

  ZEventArray = array of ZEvent;

  //----------

  ZEventQueue = class
  private
    FBuffer     : ZEventArray;
    FBufferSize : LongWord;
    FBufferMask : LongWord;
    FReadPos    : LongWord;
    FWritePos   : LongWord;
  public
    constructor create(ASize:LongWord);
    destructor  destroy; override;
    function    read(var AEvent:ZEvent) : Boolean;
    function    write(var AEvent:ZEvent) : Boolean;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZEventQueue.create(ASize:LongWord);
begin
  inherited create;
  SetLength(FBuffer,ASize);
  FBufferSize := ASize;
  FBufferMask := ASize - 1;
  FReadPos    := 0;
  FWritePos   := 0;
end;

//----------

destructor ZEventQueue.destroy;
begin
  SetLength(FBuffer,0);
  inherited;
end;

//----------

{ TODO: remove when we know it works

  (wp-rp) and FBufferMask = 'items in buffer'
  in write(), we avoid filling the entire buffer, by letting there always be
  at least one free item, because read() could misinterpret a full buffer
  as empty..
  
  if the write fail (returns false), what should we do?
  - ignore, losing events..
  - sleep(0) or something

  - the reading happens at the start of process, so in worst case,
    if buffer is full already, we mist wait until the next processBlock,

  - buffer/cache updates in case the riginal buffer is full? double-buffer..

  buffer is emptied at the very start of process(),

}

function ZEventQueue.read(var AEvent:ZEvent) : Boolean;
var
  wp,rp : LongWord;
  next : LongWord;
begin
  wp :=  FWritePos;
  rp := FReadPos;
  result := false;
  if ( LongWord(wp-rp) and FBufferMask ) > 0 then
  begin
    //writeln('read');
    AEvent.FIndex := FBuffer[rp].FIndex;
    AEvent.FValue := FBuffer[rp].FValue;
    //AEvent := FBuffer[rp];
    next := (rp+1) and FBufferMask;
    FReadPos := next;
    result := true;
  end;
end;

//----------

function ZEventQueue.write(var AEvent:ZEvent) : Boolean;
var
  rp,wp : LongWord;
  next : LongWord;
begin
  rp := FReadPos;
  wp := FWritePos;
  result := false;
  if ( LongWord(wp-rp) and FBufferMask ) < (FBufferSize-1) then
  // -1 to avoid filling the entire buffer..
  // .. which could be mis-interpreted by read() as empty..
  begin
    //writeln('write');
    FBuffer[wp].FIndex := AEvent.FIndex;
    FBuffer[wp].FValue := AEvent.FValue;
    //FBuffer[wp] := AEvent;
    next := (wp+1) and FBufferMask;
    FWritePos := next;
    result := true;
  end;
end;

//----------------------------------------------------------------------
end.

