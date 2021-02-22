{$include sphaera.inc}
unit s_rect;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SRect = object
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
    constructor create(ARect:SRect);
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
    function    intersect(ARect:SRect) : boolean; inline;
    procedure   combine(ARect:SRect); inline;
    procedure   crop(ARect:SRect); inline;
    property    x  : longint read FXpos write FXpos;
    property    y  : longint read FYpos write FYpos;
    property    w  : longint read FWidth write FWidth;
    property    h  : longint read FHeight write FHeight;
    property    x2 : longint read getX2 write setX2;
    property    y2 : longint read getY2 write setY2;
  end;

//----------

function rect0 : SRect; inline;
function rect(ARect:SRect) : SRect; inline;
function rect(ASize:longint) : SRect; inline;
function rect(AWidth,AHeight:longint) : SRect; inline;
function rect(AXpos,AYpos,AWidth,AHeight:longint) : SRect; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SRect.create;
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := 0;
  FHeight := 0;
end;

//----------

constructor SRect.create(AWidth,AHeight:longint);
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

constructor SRect.create(AXpos,AYpos,AWidth,AHeight:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

constructor SRect.create(ARect:SRect);
begin
  FXpos := ARect.x;
  FYpos := ARect.y;
  FWidth := ARect.w;
  FHeight := ARect.h;
end;

//----------

destructor SRect.destroy;
begin
end;

//----------

function SRect.getX2 : longint;
begin
  result := FXpos + FWidth - 1;
end;

//----------

function SRect.getY2 : longint;
begin
  result := FYpos + FHeight - 1;
end;

//----------

procedure SRect.setX2(AValue:longint);
begin
  FWidth := aValue - FXpos + 1;
end;

//----------

procedure SRect.setY2(AValue:longint);
begin
  FHeight := aValue - FYpos + 1;
end;

//----------------------------------------

procedure SRect.clear;
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := 0;
  FHeight := 0;
end;

//----------

procedure SRect.setup(AXpos,AYpos,AWidth,AHeight:longint);
begin
  FXpos := AXpos;
  FYpos := AYpos;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

procedure SRect.setup(AWidth,AHeight:longint);
begin
  FXpos := 0;
  FYpos := 0;
  FWidth := AWidth;
  FHeight := AHeight;
end;

//----------

procedure SRect.add(AXpos,AYpos:longint);
begin
  FXpos += AXpos;
  FYpos += AYpos;
end;

//----------

procedure SRect.add(AXpos,AYpos,AWidth,AHeight:longint);
begin
  FXpos += AXpos;
  FYpos += AYpos;
  FWidth += AWidth;
  FHeight += AHeight;
end;

//----------

procedure SRect.sub(AXpos,AYpos:longint);
begin
  add(-AXpos,-AYpos);
end;

//----------

procedure SRect.sub(AXpos,AYpos,AWidth,AHeight:longint);
begin
  sub(-AXpos,-AYpos,-AWidth,-AHeight);
end;

//----------

function SRect.empty : boolean;
begin
  if (FWidth <= 0) or (FHeight<=0) then result := true
  else result := false;
end;

//----------------------------------------


// return true if rect contains the specified point

function SRect.contain(AXpos,AYpos:longint) : boolean;
begin
  if AXpos < FXpos then exit(false);
  if AYpos < FYpos then exit(false);
  if AXpos >= (FXpos+FWidth) then exit(false);
  if AYpos >= (FYpos+FHeight) then exit(false);
  result := true;;
end;

//----------

// return true if rect overlap the specified rect

function SRect.intersect(AXpos,AYpos,AWidth,AHeight:longint) : boolean;
begin
  if (AXpos+AWidth) < FXpos then exit(false);
  if (AYpos+AHeight) < FYpos then exit(false);
  if AXpos >= (FXpos+FWidth) then exit(false);
  if AYpos >= (FYpos+FHeight) then exit(false);
  result := true;
end;

//----------

function SRect.intersect(ARect:SRect) : boolean;
begin
  result := intersect(ARect.x,ARect.y,ARect.w,ARect.h);
end;

//----------

//  __            _____
// |XX|          :xx:  |
// |XX|          :xx:  |
//     __    ->  |   ..|
//    |  |       |  :  :
//    |__|       |__:__:
//

// create a rect big enough to hold itself and specified rect

procedure SRect.combine(AXpos,AYpos,AWidth,AHeight:longint);
begin
  if AXpos < FXpos then begin FWidth += (FXpos-AXpos); FXpos:=AXpos; end;
  if AYpos < FYpos then begin FHeight += (FYpos-AYpos); FYpos:=AYpos; end;
  if (AXpos+AWidth) >= (FXpos+FWidth) then FWidth := (AXpos+AWidth) - FXpos;
  if (AYpos+AHeight) >= (FYpos+FHeight) then FHeight := (AYpos+AHeight) - FYpos;
end;

//----------

procedure SRect.combine(ARect:SRect);
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

procedure SRect.crop(AXpos,AYpos,AWidth,AHeight:longint);
begin
  if AXpos > FXpos then FXpos := AXpos;
  if AYpos > FYpos then FYpos := AYpos;
  if (AXpos+AWidth) <= (FXpos+FWidth) then FWidth := (AXpos+AWidth) - FXpos;
  if (AYpos+AHeight) <= (FYpos+FHeight) then FHeight := (AYpos+AHeight) - FYpos;
end;

//----------

procedure SRect.crop(ARect:SRect);
begin
  crop(ARect.x,ARect.y,ARect.w,ARect.h);
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

function rect0 : SRect; //inline;
begin
  result.x := 0;
  result.y := 0;
  result.w := 0;
  result.h := 0;
end;

//----------

function rect(ASize:longint) : SRect; //inline
begin
  result.x := 0;
  result.y := 0;
  result.w := ASize;
  result.h := ASize;
end;

//----------

function rect(ARect:SRect) : SRect; //inline
begin
  result.x := ARect.x;
  result.y := ARect.y;
  result.w := ARect.w;
  result.h := ARect.h;
end;

//----------

function rect(AWidth,AHeight:longint) : SRect; //inline
begin
  result.x := 0;
  result.y := 0;
  result.w := AWidth;
  result.h := AHeight;
end;

//----------

function rect(AXpos,AYpos,AWidth,AHeight:longint) : SRect; //inline
begin
  result.x := AXpos;
  result.y := AYpos;
  result.w := AWidth;
  result.h := AHeight;
end;

//----------------------------------------------------------------------
end.


