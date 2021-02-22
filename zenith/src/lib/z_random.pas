unit z_random;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

//procedure ZRandomSeed(pattern:Single);
function  ZRandom : Single;
function  ZRandomSigned : Single;
function  ZRandomInt : LongWord;
function  ZRandomGaussian : Single;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//http://www.musicdsp.org/archive.php?classid=1#217

{
  "This is quite a good PRNG! The numbers it generates exhibit a slight a
  pattern (obviously, since it's not very sophisticated) but they seem quite
  usable! The real FFT spectrum is very flat and "white" with just one or two
  aberrant spikes while the imaginary spectrum is almost perfect (as is the case
  with most PRNGs). Very nice! Either that or I need more practice with MuPad.."

  "Alternatively you can do:
     double b_noiselast = b_noise;
     b_noise = b_noise + 19;
     b_noise = b_noise * b_noise;
     b_noise = b_noise + ((-b_noise + b_noiselast) * 0.5);
     int i_noise = b_noise;
     b_noise = b_noise - i_noise;
  This will remove the patterning."
}


//const

var
  g_noise : single = 19.1919191919191919191919191919191919191919;
  g_pattern : single = 19; // 1e-19

//----------

{procedure ZRandomSeed(pattern:Single);
begin
  g_pattern := pattern;
end;}

//----------

// 0..1

function ZRandom : Single;
var
  temp : single;
  i : integer;
begin
  temp    := g_noise;
  g_noise := g_noise + g_pattern;
  g_noise := g_noise * g_noise;
  g_noise := (g_noise+temp) * 0.5;
  i       := trunc(g_noise);
  g_noise := g_noise - i;
  result  := g_noise;
end;

//----------

// -1..1

function ZRandomSigned : Single;
var i_noise : longint;
begin
  g_noise := g_noise * g_noise;
  i_noise := trunc(g_noise); // |0;
  g_noise := g_noise - i_noise;
  result  := g_noise - 0.5;
  g_noise := g_noise + g_pattern;
end;

//----------

// Pseudo-Random generator

{
  This can be used to generate random numeric sequences or to synthesise a white
  noise audio signal. If you only use some of the bits, use the most
  significant bits by shifting right. Do not just mask off the low bits.
}


var
  r_randSeed : LongWord = 22222; // Change this for different random sequences.

function ZRandomInt : LongWord;
begin
  r_randSeed := (r_randSeed * 196314165) + 907633515;
  result := r_randSeed;
end;


//----------

// http://rosettacode.org/wiki/Knuth_shuffle#Pascal
(*
  program Knuth;
  const
    max = 10;
  type
    list = array [1..max] of integer;
  procedure shuffle(var a: list);
  var
    i,k,tmp: integer;
  begin
    randomize;
    for i := max downto 2 do begin
       k := random(i) + 1;
       if (a[i] <> a[k]) then begin
         tmp := a[i]; a[i] := a[k]; a[k] := tmp
       end
    end
  end;
  { Test and display }
  var
   a: list;
   i: integer;
  begin
    for i := 1 to max do
      a[i] := i;
    shuffle(a);
    for i := 1 to max do
      write(a[i], ' ');
    writeln
  end.
*)

//----------

// http://www.musicdsp.org/archive.php?classid=5#129

{
  Gaussian random numbers
  This algorithm (adapted from "Natur als fraktale Grafik" by
  Reinhard Scholl) implements a generation method for gaussian
  distributed random numbers with mean=0 and variance=1
  (standard gaussian distribution) mapped to the range of
  -1 to +1 with the maximum at 0.
  For only positive results you might abs() the return value.
  The q variable defines the precision, with q=15 the smallest
  distance between two numbers will be 1/(2^q div 3)=1/10922
  which usually gives good results.

  Note: the random() function used is the standard random
  function from Delphi/Pascal that produces *linear*
  distributed numbers from 0 to parameter-1, the equivalent
  C function is probably rand().
}

const
  gr_q  = 15;
  gr_c1 = (1 shl gr_q)-1;
  gr_c2 = (gr_c1 div 3)+1;
  gr_c3 = 1/gr_c1;

  //----------

function ZRandomGaussian : Single;
begin
  result := (2*(random(gr_c2)+random(gr_c2)+random(gr_c2))-3*(gr_c2-1))*gr_c3;
end;

//----------------------------------------------------------------------
end.



