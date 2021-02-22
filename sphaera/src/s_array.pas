{$include sphaera.inc}
unit s_array;

{$define S_ARRAY_INITIALSIZE := 4}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  generic SArray<_T> = class
  private
    FData       : array of _T;
    FSize       : integer;
    function    getItem(index:integer) : _T;
    function    getSize : integer;
  public
    constructor create;
    destructor  destroy; override;
    procedure   append(item:_T);
    //todo
    procedure   insert(index:LongInt; item:_T);
    procedure   remove(index:LongInt);
    function    find(item:_T; from:LongInt=0) : LongInt;
  public
    property    items[index:integer] : _T read GetItem; default;
    property    size : integer read FSize;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SArray.create;
begin
  SetLength(FData,S_ARRAY_INITIALSIZE);
  FSize := 0;
end;

//----------

destructor SArray.destroy;
begin
  SetLength(FData,0);
end;

//----------

function SArray.getItem(index:integer) : _T;
begin
  result := FData[index];
end;

//----------

function SArray.getSize : integer;
begin
  result := FSize;
end;

//----------

procedure SArray.append(item:_T);
var
  realsize : integer;
begin
  realsize := Length(FData);
  if (FSize+1) > realsize then
  begin
    realsize *= 2;
    SetLength(FData,realsize);
  end;
  FData[FSize] := item;
  inc(FSize);
end;

//----------
// todo
//----------

procedure   SArray.insert(index:LongInt; item:_T);
begin
end;

procedure SArray.remove(index:LongInt);
begin
end;

function SArray.find(item:_T; from:LongInt) : LongInt;
begin
  result := -1;
end;

//----------------------------------------------------------------------
end.

