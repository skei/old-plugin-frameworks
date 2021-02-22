unit z_array;
{$include zenith.inc}

{$define Z_ARRAY_INITIALSIZE := 4}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  generic ZArray<_T> = class
  private
    FData       : array of _T;
    FSize       : integer;
    function    getItem(index:integer) : _T;
    function    getSize : integer;
  public
    constructor create;
    destructor  destroy; override;
    procedure   append(item:_T);
    property    items[index:integer] : _T read GetItem; default;
    property    size : integer read FSize;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZArray.create;
begin
  SetLength(FData,Z_ARRAY_INITIALSIZE);
  FSize := 0;
end;

//----------

destructor ZArray.destroy;
begin
  SetLength(FData,0);
end;

//----------

function ZArray.getItem(index:integer) : _T;
begin
  result := FData[index];
end;

//----------

function ZArray.getSize : integer;
begin
  result := FSize;
end;

//----------

procedure ZArray.append(item:_T);
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

//----------------------------------------------------------------------
end.


