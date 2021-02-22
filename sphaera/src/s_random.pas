{$include sphaera.inc}
unit s_random;

// FreePascal's function random uses the MersenneTwister
// (for further details, see the file rtl/inc/system.inc)

{
  http://en.wikipedia.org/wiki/List_of_pseudorandom_number_generators
  http://en.wikipedia.org/wiki/Linear_congruential_generator
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

procedure SRandomSeed(ASeed:LongWord); inline;
function  SRandom : Single; inline;
function  SRandomSigned : Single; inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

// uses

//----------------------------------------------------------------------
//http://www.musicdsp.org/archive.php?classid=1#217
//----------------------------------------------------------------------

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

var
  rand1_noise   : single = 19.1919191919191919191919191919191919191919;
  rand1_pattern : single = 19; // 1e-19

//----------

procedure SRandomSeed1(ASeed:Single);
begin
  rand1_pattern := ASeed;
end;

// 0..1

function SRandom1 : Single;
var
  temp : single;
  i : integer;
begin
  temp        := rand1_noise;
  rand1_noise := rand1_noise + rand1_pattern;
  rand1_noise := rand1_noise * rand1_noise;
  rand1_noise := (rand1_noise+temp) * 0.5;
  i           := trunc(rand1_noise);
  rand1_noise := rand1_noise - i;
  result      := rand1_noise;
end;

//----------

// -1..1

function SRandomSigned1 : Single;
var i_noise : longint;
begin
  rand1_noise := rand1_noise * rand1_noise;
  i_noise := trunc(rand1_noise); // |0;
  rand1_noise := rand1_noise - i_noise;
  result  := rand1_noise - 0.5;
  rand1_noise := rand1_noise + rand1_pattern;
end;

//----------------------------------------------------------------------
// Pseudo-Random generator
//----------------------------------------------------------------------

{
  This can be used to generate random numeric sequences or to synthesise a white
  noise audio signal. If you only use some of the bits, use the most
  significant bits by shifting right. Do not just mask off the low bits.
}

var
  rand2_seed : LongWord = 22222; // Change this for different random sequences.

//----------

procedure SRandomSeed2(aSeed:LongWord);
begin
  rand2_seed := aSeed;
end;

//----------

function SRandomWord2 : LongWord;
begin
  rand2_seed := (rand2_seed * 196314165) + 907633515;
  result := rand2_seed;
end;

//----------

function SRandomByte2 : Byte;
begin
  //rand2_seed := (rand2_seed * 196314165) + 907633515;
  //result := rand2_seed shr 24;
  result := SRandomWord2 shr 24;
end;

//----------------------------------------------------------------------
// http://www.musicdsp.org/archive.php?classid=5#129
//----------------------------------------------------------------------

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
  rand3_q  = 15;
  rand3_c1 = (1 shl rand3_q)-1;
  rand3_c2 = (rand3_c1 div 3)+1;
  rand3_c3 = 1/rand3_c1;

  //----------

function SRandomGaussian : Single;
begin
  result := (2*(random(rand3_c2)+random(rand3_c2)+random(rand3_c2))-3*(rand3_c2-1))*rand3_c3;
end;

//----------------------------------------------------------------------
// ranmar
//----------------------------------------------------------------------

{
  This random number generator originally appeared in "Toward a Universal
  Random Number Generator" by George Marsaglia and Arif Zaman.
  Florida State University Report: FSU-SCRI-87-50 (1987)

  It was later modified by F. James and published in "A Review of Pseudo-
  random Number Generators"

  THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
  (However, a newly discovered technique can yield
  a period of 10^600. But that is still in the development stage.)

  It passes ALL of the tests for random number generators and has a period
  of 2^144, is completely portable (gives bit identical results on all
  machines with at least 24-bit mantissas in the floating point
  representation).

  The algorithm is a combination of a Fibonacci sequence (with lags of 97
  and 33, and operation "subtraction plus one, modulo one") and an
  "arithmetic sequence" (using subtraction).

  C language version was written by Jim Butler, and was based on a
  FORTRAN program posted by David LaSalle of Florida State University.

  Adapted for Delphi by Anton Zhuchkov (fireton@mail.ru) in February, 2002
}

//----------

{
  This is the initialization routine for the random number generator RANMAR()
  NOTE: The seed variables can have values between:
    0 <= IJ <= 31328
    0 <= KL <= 30081
  The random number sequences created by these two seeds are of sufficient
  length to complete an entire calculation with. For example, if sveral
  different groups are working on different parts of the same calculation,
  each group could be assigned its own IJ seed. This would leave each group
  with 30000 choices for the second seed. That is to say, this random
  number generator can create 900 million different subsequences -- with
  each subsequence having a length of approximately 10^30.

  Use IJ = 1802 & KL = 9373 to test the random number generator. The
  subroutine RANMAR should be used to generate 20000 random numbers.
  Then display the next six random numbers generated multiplied by 4096*4096
  If the random number generator is working properly, the random numbers
  should be:
    6533892.0  14220222.0  7275067.0
    6172232.0  8354498.0   10633180.0
}

const
  ranmar_seeded : Boolean = False;

var
  ranmar_u : array [0..97] of Double;
	ranmar_c, ranmar_cd, ranmar_cm : Double;
  ranmar_i97, ranmar_j97  : Integer;

//----------

procedure SRandomRanmarSeed(IJ:Integer=1802; KL:Integer=9373);
var
	i, j, k, l, ii, jj, m : Integer;
  s, t : Double;
begin
	if (ij<0) or (ij>31328) or (kl<0) or (kl>30081) then exit; // Random generator seed not within the valid range
	i := (ij div 177) mod 177 + 2;
	j := ij mod 177 + 2;
	k := (kl div 169) mod 178 + 1;
	l := kl mod 169;
 	for ii:=1 to 97 do
  begin
		s := 0.0;
		t := 0.5;
		for jj:=1 to 24 do
    begin
			m := (((i*j) mod 179)*k) mod 179;
			i := j;
			j := k;
			k := m;
			l := (53*l + 1) mod 169;
			if ((l*m) mod 64 >= 32) then s := s + t;
			t := t*0.5;
		end;
		ranmar_u[ii] := s;
	end;
	ranmar_c := 362436.0 / 16777216.0;
	ranmar_cd := 7654321.0 / 16777216.0;
	ranmar_cm := 16777213.0 / 16777216.0;
	ranmar_i97 := 97;
	ranmar_j97 := 33;
	ranmar_seeded  := True;
end;

//----------

{
  This is the random number generator proposed by George Marsaglia in
  Florida State University Report: FSU-SCRI-87-50
}

function SRandomRanmar: Double;
var
	uni: Double;
begin
	if not ranmar_seeded then exit(0);//raise Exception.Create('Random generator not seeded!');
  uni := ranmar_u[ranmar_i97] - ranmar_u[ranmar_j97];
  if (uni < 0.0) then uni := uni + 1.0;
  ranmar_u[ranmar_i97] := uni;
  Dec(ranmar_i97);
  if ranmar_i97 = 0 then ranmar_i97 := 97;
  dec(ranmar_j97);
  if ranmar_j97 = 0 then ranmar_j97 := 97;
  ranmar_c := ranmar_c - ranmar_cd;
  if ranmar_c < 0.0 then ranmar_c := ranmar_c + ranmar_cm;
  uni := uni - ranmar_c;
  if uni < 0.0 then uni := uni + 1.0;
  Result := uni;
end;

//----------------------------------------------------------------------
// http://en.wikipedia.org/wiki/Xorshift
//----------------------------------------------------------------------

(*
uint32_t xor128(void)
{
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;
  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}
*)

{function SRandomXorShift128 : LongWord;
var
  x,y,z,w,t : longword;
begin
  x := 123456789;
  y := 362436069;
  z := 521288629;
  w := 88675123;
  t := x ** x << 11; //   x ^ (x << 11);
  x := y;
  y := z;
  z := w;
  w := w ** (w >> 19) ** (t ** (t >> 8));
  result := w;
end;}

//----------------------------------------------------------------------
// Linear Congruential Generator
//----------------------------------------------------------------------

// http://rosettacode.org/wiki/Linear_congruential_generator#Pascal

var
  x1 : int64;
  x2 : int64;

//----------

function SRandomLCGbsd : LongInt;
const
  a = 1103515245;
  c = 12345;
  m = 2147483648;
begin
  x1 := (a * x1 + c) mod m;
  result := x1;
end;

//----------

function SRandomLCGms : LongInt;
const
  a = 214013;
  c = 2531011;
  m = 2147483648;
begin
  x2 := (a * x2 + c) mod m;
  result := x2 div 65536;
end;

//----------

(*
var
  i: longint;
begin
  writeln('      BSD            MS');
  x1 := 0;
  x2 := 0;
  for i := 1 to 10 do
    writeln(bsdrand:12, msrand:12);
end.
*)

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

procedure SRandomSeed(ASeed:LongWord); inline;
begin
  SRandomSeed1(ASeed);
end;

//----------

function SRandom : Single; inline;
begin
  result := SRandom1;
end;

//----------

function SRandomSigned : Single; inline;
begin
  result := SRandomSigned1;
end;

//----------------------------------------------------------------------
end.

