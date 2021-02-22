unit z_point;
{$include zenith.inc}

{
  TODO:
  - distance (from another point)
  - length
  - ..
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZPoint = object
  private
    FXpos : LongInt;
    FYpos : LongINt;
  public
    constructor create;
    constructor create(AXpos,AYpos:longint);
    destructor  destroy;
    procedure   clear; inline;
    procedure   setup(AXpos,AYpos:longint); inline;
    procedure   add(AXpos,AYpos:longint); inline; { x+=a_X; y+=a_Y; }
  public
    property    x : longint read FXpos write FXpos;
    property    y : longint read FYpos write FYpos;
  end;

//----------

function point(AXpos,AYpos:longint) : ZPoint; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZPoint.create;
begin
  FXpos := 0;
  FYpos := 0;
end;

//----------

constructor ZPoint.create(AXpos,AYpos:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
end;

//----------

destructor ZPoint.destroy;
begin
end;

//----------

procedure ZPoint.clear;
begin
  FXpos := 0;
  FYpos := 0;
end;

//----------

procedure ZPoint.setup(AXpos,AYpos:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
end;

//----------

procedure ZPoint.add(AXpos,AYpos:longint);
begin
  FXpos += AXpos;
  FYpos += AYpos;
end;

//----------------------------------------------------------------------

function point(AXpos,AYpos:longint) : ZPoint;
begin
  result.x := AXpos;
  result.y := AYpos;
end;

//----------------------------------------------------------------------
end.


