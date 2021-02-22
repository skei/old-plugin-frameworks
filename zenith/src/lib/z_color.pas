unit z_color;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZColor = record
    r,g,b,a : single;
    sys : longword;
  end;
  //PZColor = ^ZColor;

function color(c:Single) : ZColor; inline;
function color(r,g,b:Single) : ZColor; inline;
function color(r,g,b,a:Single) : ZColor; inline;

//---

function z_white          : ZColor; inline;
function z_light_grey     : ZColor; inline;
function z_light_red      : ZColor; inline;
function z_light_yellow   : ZColor; inline;
function z_light_green    : ZColor; inline;
function z_light_cyan     : ZColor; inline;
function z_light_blue     : ZColor; inline;
function z_light_magenta  : ZColor; inline;
function z_grey           : ZColor; inline;
function z_red            : ZColor; inline;
function z_yellow         : ZColor; inline;
function z_green          : ZColor; inline;
function z_cyan           : ZColor; inline;
function z_blue           : ZColor; inline;
function z_magenta        : ZColor; inline;
function z_dark_grey      : ZColor; inline;
function z_dark_red       : ZColor; inline;
function z_dark_yellow    : ZColor; inline;
function z_dark_green     : ZColor; inline;
function z_dark_cyan      : ZColor; inline;
function z_dark_blue      : ZColor; inline;
function z_dark_magenta   : ZColor; inline;
function z_black          : ZColor; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

function color(c:Single) : ZColor;
begin
  result.r := c;
  result.g := c;
  result.b := c;
  result.a := 1;
end;

function color(r,g,b:Single) : ZColor;
begin
  result.r := r;
  result.g := g;
  result.b := b;
  result.a := 1;
end;

function color(r,g,b,a:Single) : ZColor;
begin
  result.r := r;
  result.g := g;
  result.b := b;
  result.a := a;
end;

//{$define _L:=0.8}
//{$define _M:=0.6}
//{$define _D:=0.4}

const
  L = 0.8;
  M = 0.6;
  D = 0.4;

function z_white          : ZColor; begin result := color( 1, 1, 1, 1 ); end;
function z_light_grey     : ZColor; begin result := color( L, L, L, 1 ); end;
function z_light_red      : ZColor; begin result := color( L, 0, 0, 1 ); end;
function z_light_yellow   : ZColor; begin result := color( L, L, 0, 1 ); end;
function z_light_green    : ZColor; begin result := color( 0, L, 0, 1 ); end;
function z_light_cyan     : ZColor; begin result := color( 0, L, L, 1 ); end;
function z_light_blue     : ZColor; begin result := color( 0, 0, L, 1 ); end;
function z_light_magenta  : ZColor; begin result := color( L, 0, L, 1 ); end;
function z_grey           : ZColor; begin result := color( M, M, M, 1 ); end;
function z_red            : ZColor; begin result := color( M, 0, 0, 1 ); end;
function z_yellow         : ZColor; begin result := color( M, M, 0, 1 ); end;
function z_green          : ZColor; begin result := color( 0, M, 0, 1 ); end;
function z_cyan           : ZColor; begin result := color( 0, M, M, 1 ); end;
function z_blue           : ZColor; begin result := color( 0, 0, M, 1 ); end;
function z_magenta        : ZColor; begin result := color( M, 0, M, 1 ); end;
function z_dark_grey      : ZColor; begin result := color( D, D, D, 1 ); end;
function z_dark_red       : ZColor; begin result := color( D, 0, 0, 1 ); end;
function z_dark_yellow    : ZColor; begin result := color( D, D, 0, 1 ); end;
function z_dark_green     : ZColor; begin result := color( 0, D, 0, 1 ); end;
function z_dark_cyan      : ZColor; begin result := color( 0, D, D, 1 ); end;
function z_dark_blue      : ZColor; begin result := color( 0, 0, D, 1 ); end;
function z_dark_magenta   : ZColor; begin result := color( D, 0, D, 1 ); end;
function z_black          : ZColor; begin result := color( 0, 0, 0, 1 ); end;

//----------------------------------------------------------------------
end.

