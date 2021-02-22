{$include sphaera.inc}
unit s_color;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SColor = record
    r,g,b,a : single;
    //sys : longword;
  end;
  //PZColor = ^ZColor;

function color(c:Single) : SColor; inline;
function color(r,g,b:Single) : SColor; inline;
function color(r,g,b,a:Single) : SColor; inline;

//---

function s_white          : SColor; inline;
function s_light_grey     : SColor; inline;
function s_light_red      : SColor; inline;
function s_light_yellow   : SColor; inline;
function s_light_green    : SColor; inline;
function s_light_cyan     : SColor; inline;
function s_light_blue     : SColor; inline;
function s_light_magenta  : SColor; inline;
function s_grey           : SColor; inline;
function s_red            : SColor; inline;
function s_yellow         : SColor; inline;
function s_green          : SColor; inline;
function s_cyan           : SColor; inline;
function s_blue           : SColor; inline;
function s_magenta        : SColor; inline;
function s_dark_grey      : SColor; inline;
function s_dark_red       : SColor; inline;
function s_dark_yellow    : SColor; inline;
function s_dark_green     : SColor; inline;
function s_dark_cyan      : SColor; inline;
function s_dark_blue      : SColor; inline;
function s_dark_magenta   : SColor; inline;
function s_black          : SColor; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

function color(c:Single) : SColor;
begin
  result.r := c;
  result.g := c;
  result.b := c;
  result.a := 1;
end;

function color(r,g,b:Single) : SColor;
begin
  result.r := r;
  result.g := g;
  result.b := b;
  result.a := 1;
end;

function color(r,g,b,a:Single) : SColor;
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

function s_white          : SColor; begin result := color( 1, 1, 1, 1 ); end;
function s_light_grey     : SColor; begin result := color( L, L, L, 1 ); end;
function s_light_red      : SColor; begin result := color( L, 0, 0, 1 ); end;
function s_light_yellow   : SColor; begin result := color( L, L, 0, 1 ); end;
function s_light_green    : SColor; begin result := color( 0, L, 0, 1 ); end;
function s_light_cyan     : SColor; begin result := color( 0, L, L, 1 ); end;
function s_light_blue     : SColor; begin result := color( 0, 0, L, 1 ); end;
function s_light_magenta  : SColor; begin result := color( L, 0, L, 1 ); end;
function s_grey           : SColor; begin result := color( M, M, M, 1 ); end;
function s_red            : SColor; begin result := color( M, 0, 0, 1 ); end;
function s_yellow         : SColor; begin result := color( M, M, 0, 1 ); end;
function s_green          : SColor; begin result := color( 0, M, 0, 1 ); end;
function s_cyan           : SColor; begin result := color( 0, M, M, 1 ); end;
function s_blue           : SColor; begin result := color( 0, 0, M, 1 ); end;
function s_magenta        : SColor; begin result := color( M, 0, M, 1 ); end;
function s_dark_grey      : SColor; begin result := color( D, D, D, 1 ); end;
function s_dark_red       : SColor; begin result := color( D, 0, 0, 1 ); end;
function s_dark_yellow    : SColor; begin result := color( D, D, 0, 1 ); end;
function s_dark_green     : SColor; begin result := color( 0, D, 0, 1 ); end;
function s_dark_cyan      : SColor; begin result := color( 0, D, D, 1 ); end;
function s_dark_blue      : SColor; begin result := color( 0, 0, D, 1 ); end;
function s_dark_magenta   : SColor; begin result := color( D, 0, D, 1 ); end;
function s_black          : SColor; begin result := color( 0, 0, 0, 1 ); end;

//----------------------------------------------------------------------
end.

