{$include sphaera.inc}
unit s_point;

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

  SPoint = object
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

function point(AXpos,AYpos:longint) : SPoint; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SPoint.create;
begin
  FXpos := 0;
  FYpos := 0;
end;

//----------

constructor SPoint.create(AXpos,AYpos:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
end;

//----------

destructor SPoint.destroy;
begin
end;

//----------

procedure SPoint.clear;
begin
  FXpos := 0;
  FYpos := 0;
end;

//----------

procedure SPoint.setup(AXpos,AYpos:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
end;

//----------

procedure SPoint.add(AXpos,AYpos:longint);
begin
  FXpos += AXpos;
  FYpos += AYpos;
end;

//----------------------------------------------------------------------

function point(AXpos,AYpos:longint) : SPoint;
begin
  result.x := AXpos;
  result.y := AYpos;
end;

//----------------------------------------------------------------------
end.


