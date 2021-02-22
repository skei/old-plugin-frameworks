{$include sphaera.inc}
unit s_list;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SListNode = class
  private // protected
    FPrev : SListNode;
    FNext : SListNode;
  public
    property _next : SListNode read FNext;
    property _prev : SListNode read FPrev;

  end;

  SList = class
  private // protected
    FHead : SListNode;
    FTail : SListNode;
    //FSize : LongInt;
  public
    constructor create;
    procedure   reset;
    procedure   deleteNodes;
    procedure   appendHead(ANode:SListNode);
    procedure   appendTail(ANode:SListNode);
    procedure   removeHead;
    procedure   removeTail;
    procedure   append(ANode:SListNode);
    procedure   remove(ANode:SListNode);
  public
    property _head : SListNode read Fhead;
    property _tail : SListNode read FTail;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SList.create;
begin
  self.reset;
end;

//----------

//inline axListNode* getHead(void) { return FHead; }
//inline axListNode* getTail(void) { return FTail; }

//inline axListNode* getNext(axListNode* ANode) { return ANode.FNext; }
//inline axListNode* getPrev(axListNode* ANode) { return ANode.FPrev; }

//----------

procedure SList.reset;
begin
  // todo: delete nodes..
  FHead := nil;
  FTail := nil;
end;

//----------

procedure SList.deleteNodes;
var
  node,next : SListNode;
begin
  node := FHead;
  while Assigned(node) do
  begin
    next := node.FNext;
    node.Destroy;
    node := next;
  end;
  FHead := nil;
  FTail := nil;
end;

//----------

procedure SList.appendHead(ANode:SListNode);
begin
  ANode.FPrev := nil;
  if FTail = nil then
  begin
    FTail := ANode;
    ANode.FNext := nil;
  end
  else
  begin
    FHead.FPrev := ANode;
    ANode.FNext := FHead;
  end;
  FHead := ANode;
  //inc(FSize);
end;

//----------

procedure SList.appendTail(ANode:SListNode);
begin
  ANode.FNext := nil;
  if FHead = nil then
  begin
    FHead := ANode;
    ANode.FPrev := nil;
  end
  else
  begin
    FTail.FNext := ANode;
    ANode.FPrev := FTail;
  end;
  FTail := ANode;
  //inc(FSize);
end;

//----------

procedure SList.removeHead;
begin
  if FHead = FTail then self.reset
  else
  begin
    FHead := FHead.FNext;
    FHead.FPrev := nil;
  end;
end;

//----------

procedure SList.removeTail;
begin
  if FHead = FTail then self.reset
  else
  begin
    FTail := FTail.FPrev;
    FTail.FNext := nil;
  end;
end;

//----------

procedure SList.append(ANode:SListNode);
begin
  appendTail(ANode);
end;

//----------

procedure SList.remove(ANode:SListNode);
begin
  if ANode = FHead then removeHead
  else if ANode = FTail then removeTail
  else
  begin
    ANode.FNext.FPrev := ANode.FPrev;
    ANode.FPrev.FNext := ANode.FNext;
  end;
end;

//----------

//----------------------------------------------------------------------
end.

