unit z_rect;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZRect = object
  private
    FXpos   : LongInt;
    FYpos   : LongInt;
    FWidth  : LongInt;
    FHeight : LongInt;
  private
    function    getX2 : longint; inline;
    function    getY2 : longint; inline;
    procedure   setX2(AValue:longint); inline;
    procedure   setY2(AValue:longint); inline;
  public
    constructor create;
    constructor create(AWidth,AHeight:longint);
    constructor create(AXpos,AYpos,AWidth,AHeight:longint);
    constructor create(ARect:ZRect);
    destructor  destroy;
    procedure   clear; inline;
    procedure   setup(AWidth,AHeight:longint); inline;
    procedure   setup(AXpos,AYpos,AWidth,AHeight:longint); inline;
    procedure   add(AXpos,AYpos:longint); inline;
    procedure   add(AXpos,AYpos,AWidth,AHeight:longint); inline;
    procedure   sub(AXpos,AYpos:longint); inline;
    procedure   sub(AXpos,AYpos,AWidth,AHeight:longint); inline;
    function    empty : boolean; inline;
    function    contain(AXpos,AYpos:longint) : boolean;
    function    intersect(AXpos,AYpos,AWidth,AHeight:longint) : boolean;
    procedure   combine(AXpos,AYpos,AWidth,AHeight:longint);
    procedure   crop(AXpos,AYpos,AWidth,AHeight:longint);
    function    intersect(ARect:ZRect) : boolean; inline;
    procedure   combine(ARect:ZRect); inline;
    procedure   crop(ARect:ZRect); inline;
    property    x  : longint read FXpos write FXpos;
    property    y  : longint read FYpos write FYpos;
    property    w  : longint read FWidth write FWidth;
    property    h  : longint read FHeight write FHeight;
    property    x2 : longint read getX2 write setX2;
    property    y2 : longint read getY2 write setY2;
  end;

//----------

function rect0 : ZRect; inline;
function rect(ARect:ZRect) : ZRect; inline;
function rect(ASize:longint) : ZRect; inline;
function rect(AWidth,AHeight:longint) : ZRect; inline;
function rect(AXpos,AYpos,AWidth,AHeight:longint) : ZRect; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZRect.create;
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := 0;
  FHeight := 0;
end;

//----------

constructor ZRect.create(AWidth,AHeight:longint);
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

constructor ZRect.create(AXpos,AYpos,AWidth,AHeight:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

constructor ZRect.create(ARect:ZRect);
begin
  FXpos := ARect.x;
  FYpos := ARect.y;
  FWidth := ARect.w;
  FHeight := ARect.h;
end;

//----------

destructor ZRect.destroy;
begin
end;

//----------

function ZRect.getX2 : longint;
begin
  result := FXpos + FWidth - 1;
end;

//----------

function ZRect.getY2 : longint;
begin
  result := FYpos + FHeight - 1;
end;

//----------

procedure ZRect.setX2(AValue:longint);
begin
  FWidth := aValue - FXpos + 1;
end;

//----------

procedure ZRect.setY2(AValue:longint);
begin
  FHeight := aValue - FYpos + 1;
end;

//----------------------------------------

procedure ZRect.clear;
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := 0;
  FHeight := 0;
end;

//----------

procedure ZRect.setup(AXpos,AYpos,AWidth,AHeight:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

procedure ZRect.setup(AWidth,AHeight:longint);
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

procedure ZRect.add(AXpos,AYpos:longint);
begin
  FXpos += AXpos;
  FYpos += AYpos;
end;

//----------

procedure ZRect.add(AXpos,AYpos,AWidth,AHeight:longint);
begin
  FXpos += AXpos;
  FYpos += AYpos;
  FWidth += AWidth;
  FHeight += AHeight;
end;

//----------

procedure ZRect.sub(AXpos,AYpos:longint);
begin
  add(-AXpos,-AYpos);
end;

//----------

procedure ZRect.sub(AXpos,AYpos,AWidth,AHeight:longint);
begin
  sub(-AXpos,-AYpos,-AWidth,-AHeight);
end;

//----------

function ZRect.empty : boolean;
begin
  if (FWidth <= 0) or (FHeight<=0) then result := true
  else result := false;
end;

//----------------------------------------


// return true if rect contains the specified point

function ZRect.contain(AXpos,AYpos:longint) : boolean;
begin
  if AXpos < FXpos then exit(false);
  if AYpos < FYpos then exit(false);
  if AXpos >= (FXpos+FWidth) then exit(false);
  if AYpos >= (FYpos+FHeight) then exit(false);
  result := true;;
end;

//----------

// return true if rect overlap the specified rect

function ZRect.intersect(AXpos,AYpos,AWidth,AHeight:longint) : boolean;
begin
  if (AXpos+AWidth) < FXpos then exit(false);
  if (AYpos+AHeight) < FYpos then exit(false);
  if AXpos >= (FXpos+FWidth) then exit(false);
  if AYpos >= (FYpos+FHeight) then exit(false);
  result := true;
end;

//----------

function ZRect.intersect(ARect:ZRect) : boolean;
begin
  result := intersect(ARect.x,ARect.y,ARect.w,ARect.h);
end;

//----------

//  __            _____
// |XX|          :xx:  |
// |XX|          :..:  |
//     __    ->  |   ..|
//    |  |       |  :  :
//    |__|       |__:__:
//

// create a rect big enough to hold itself and specified rect

procedure ZRect.combine(AXpos,AYpos,AWidth,AHeight:longint);
begin
  if AXpos < FXpos then begin FWidth += (FXpos-AXpos); FXpos:=AXpos; end;
  if AYpos < FYpos then begin FHeight += (FYpos-AYpos); FYpos:=AYpos; end;
  if (AXpos+AWidth) >= (FXpos+FWidth) then FWidth := (AXpos+AWidth) - FXpos;
  if (AYpos+AHeight) >= (FYpos+FHeight) then FHeight := (AYpos+AHeight) - FYpos;
end;

//----------

procedure ZRect.combine(ARect:ZRect);
begin
  combine(ARect.x,ARect.y,ARect.w,ARect.h);
end;

//----------

//    ___
//   |  _|_          _:.
//   | |X|X|   ->   |X| :
//   | |X|X|        |X|.:
//   |___|            :
//
// X = argument

// keep only intersecting area

procedure ZRect.crop(AXpos,AYpos,AWidth,AHeight:longint);
begin
  if AXpos > FXpos then FXpos := AXpos;
  if AYpos > FYpos then FYpos := AYpos;
  if (AXpos+AWidth) <= (FXpos+FWidth) then FWidth := (AXpos+AWidth) - FXpos;
  if (AYpos+AHeight) <= (FYpos+FHeight) then FHeight := (AYpos+AHeight) - FYpos;
end;

//----------

procedure ZRect.crop(ARect:ZRect);
begin
  crop(ARect.x,ARect.y,ARect.w,ARect.h);
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

function rect0 : ZRect; //inline;
begin
  result.x := 0;
  result.y := 0;
  result.w := 0;
  result.h := 0;
end;

//----------

function rect(ASize:longint) : ZRect; //inline
begin
  result.x := 0;
  result.y := 0;
  result.w := ASize;
  result.h := ASize;
end;

//----------

function rect(ARect:ZRect) : ZRect; //inline
begin
  result.x := ARect.x;
  result.y := ARect.y;
  result.w := ARect.w;
  result.h := ARect.h;
end;

//----------

function rect(AWidth,AHeight:longint) : ZRect; //inline
begin
  result.x := 0;
  result.y := 0;
  result.w := AWidth;
  result.h := AHeight;
end;

//----------

function rect(AXpos,AYpos,AWidth,AHeight:longint) : ZRect; //inline
begin
  result.x := AXpos;
  result.y := AYpos;
  result.w := AWidth;
  result.h := AHeight;
end;

//----------------------------------------------------------------------
end.


