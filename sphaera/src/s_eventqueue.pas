{$include sphaera.inc}
unit s_eventqueue;

{
  loses events if buffer is full
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SEvent = record
    FIndex  : LongInt;
    FValue  : Single;
    //FData   : PtrUInt;
  end;

  SEventArray = array of SEvent;

  //----------

  SEventQueue = class
  private
    FBuffer     : SEventArray;
    FBufferSize : LongWord;
    FBufferMask : LongWord;
    FReadPos    : LongWord;
    FWritePos   : LongWord;
  public
    constructor create(ASize:LongWord);
    destructor  destroy; override;
    procedure   clear;
    function    read(var AEvent:SEvent) : Boolean;
    function    write(var AEvent:SEvent) : Boolean;
  //function    hasEventWithIndex(AIndex:LongInt) : Boolean;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SEventQueue.create(ASize:LongWord);
begin
  inherited create;
  SetLength(FBuffer,ASize);
  FBufferSize := ASize;
  FBufferMask := ASize - 1;
  FReadPos    := 0;
  FWritePos   := 0;
end;

//----------

destructor SEventQueue.destroy;
begin
  SetLength(FBuffer,0);
  inherited;
end;

//----------

procedure SEventQueue.clear;
begin
  FReadPos    := 0;
  FWritePos   := 0;
end;

{ TODO: remove when we know it works

  (wp-rp) and FBufferMask = 'items in buffer'
  in write(), we avoid filling the entire buffer, by letting there always be
  at least one free item, because read() could misinterpret a full buffer
  as empty..
  if the write fail (returns false), what should we do?
  - ignore, losing events..
  - sleep(0) or something
  - the reading happens at the start of process, so in worst case,
    if buffer is full already, we must wait until the next processBlock,
  - buffer/cache updates in case the original buffer is full? double-buffer..
  buffer is emptied at the start of process()
}

function SEventQueue.read(var AEvent:SEvent) : Boolean;
var
  wp,rp : LongWord;
  next : LongWord;
begin
  wp :=  FWritePos;
  rp := FReadPos;
  result := false;
  if ( LongWord(wp-rp) and FBufferMask ) > 0 then
  begin
    //AEvent := FBuffer[rp];
    AEvent.FIndex := FBuffer[rp].FIndex;
    AEvent.FValue := FBuffer[rp].FValue;
    next := (rp+1) and FBufferMask;
    FReadPos := next;
    result := true;
  end;
end;

//----------

function SEventQueue.write(var AEvent:SEvent) : Boolean;
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
    //FBuffer[wp] := AEvent;
    FBuffer[wp].FIndex := AEvent.FIndex;
    FBuffer[wp].FValue := AEvent.FValue;
    next := (wp+1) and FBufferMask;
    FWritePos := next;
    result := true;
  end;
end;

//----------

{
  * check if there is an event with same index already in the queue
  * todo: a) update value (replace event)
          b) remove old event, insert new at end of queue
}

(*
function SEventQueue.hasEventWithIndex(AIndex:LongInt) : Boolean;
var
  rpos,wpos : longint;
begin
  result := False;
  rpos := FReadPos;
  wpos := FWritePos;
  while rpos <> wpos do
  begin
    if FBuffer[rpos].FIndex =;
  end;
end;
*)

//----------------------------------------------------------------------
end.

