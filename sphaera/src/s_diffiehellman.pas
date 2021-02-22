{$include sphaera.inc}
unit s_diffiehellman;

// diffie-hellman
// http://www.algorytm.org/inne/algorytm-diffie-hellmana/diffie-hellman-d.html

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------
uses
  {ComObj,}
  ActiveX;

  // hmmm... CoCreateGuid
  // Creates a GUID, a unique 128-bit integer used for CLSIDs and interface identifiers
  // todo: find another prng..

const
  MAX_PRIME_NUMBER   = 2147483648;
  MAX_RANDOM_INTEGER = 2147483648;

type

  DiffieHellman = class
  private
    function    XpowYmodN(xi,yi,ni:int64):int64;
    function    GeneratePrime():int64;
    function    IsItPrime(ni,ai:int64):boolean;
    function    MillerRabin(ni,trials:int64):boolean;
  public
    g,n,a,b,x,y,k : int64;
    constructor Create();
    procedure   CreateKeys(var Generator,Modulus:int64);
    procedure   CreateSenderInterKey(var InterKey:int64);
    procedure   CreateRecipientInterKey(var InterKey,Generator,Modulus:int64);
    procedure   CreateSenderEncryptionKey(var EncryptionKey,RecipientInterKey:int64);
    procedure   CreateRecipientEncryptionKey(var EncryptionKey, SendersInterKey:int64);
end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor DiffieHellman.Create();
begin
  g := 0;
  n := 0;
  a := 0;
  b := 0;
  X := 0;
  Y := 0;
  K := 0;
end;

//----------

function DiffieHellman.XpowYmodN(xi,yi,ni:int64):int64;
var
tmp:int64;
begin
	tmp := 0;
	if (yi=1) then
        result:= (xi mod Ni)
        else
	if ((yi and 1)=0) then
	begin
		tmp:= XpowYmodN(xi,yi div 2,Ni);
		result:=((tmp * tmp) mod Ni);
	end
	else
	begin
		tmp := XpowYmodN(xi,(yi-1) div 2,Ni);
		tmp := ((tmp * tmp) mod Ni);
		tmp := ((tmp * xi) mod Ni);
		result:=tmp;
	end;
end;

//----------

function DiffieHellman.GeneratePrime():int64;
var
RandomNumber:TGUID;
tmp:int64;
begin


	CoCreateGuid(RandomNumber);

	tmp := ((RandomNumber.D1 * (RandomNumber.D2 * RandomNumber.D3)) mod MAX_PRIME_NUMBER);

	//ensure it is an odd number
	if ((tmp and 1)=0) then
		tmp :=tmp+ 1;

	if (MillerRabin(tmp,5)=true) then
        begin
         result:=tmp;
         exit;
        end;

	repeat
         tmp:=tmp+2;
	until (MillerRabin(tmp,5)=false);

	result:=tmp;

end;

//----------

function DiffieHellman.IsItPrime(ni,ai: int64):boolean;
var
d:int64;
begin
	d := XpowYmodN(ai, ni-1, ni);
	if (d=1) then
		result:=true
	else
		result:=false;

end;

//----------

function DiffieHellman.MillerRabin(ni,trials:int64):boolean;
//---http://en.wikipedia.org/wiki/Miller-Rabin_test
var
i,ai:int64;
begin
        ai:= 0;
        i:=0;
        repeat
	begin
		ai:= (random(ni-3))+2;// gets random value in [2..n-1]

		if (IsItPrime (ni,ai)=false) then
		begin
			result:=false;
                        exit;
			//n composite, return false
		end;
                i:=i+1;
	end;
        until (i>trials);
        result:=true;
end;

//----------

procedure DiffieHellman.CreateKeys(var Generator,Modulus:int64);
var
swap:int64;
begin
	swap:=0;

	//Check if keys have already been established
	if ((n<>0) and (g<>0)) then
	begin
		Generator := g;
		Modulus   := n;
	end
	else
	begin
		g := GeneratePrime();
		n := GeneratePrime();

		if (g>n) then
		begin
			swap := g;
			g    := n;
			n    := swap;
		end;
		Generator := g;
		Modulus   := n;
	end;

end;

//----------

procedure DiffieHellman.CreateSenderInterKey(var InterKey:int64);
var
RandomNumber:TGUID;
begin
	CoCreateGuid(RandomNumber);
	a := (RandomNumber.D1 mod MAX_RANDOM_INTEGER);

	X := XpowYmodN(g,a,n);

	InterKey:= X;
end;

//----------

procedure DiffieHellman.CreateRecipientInterKey(var InterKey,Generator,Modulus:int64);
var
RandomNumber:TGUID;
begin
	CoCreateGuid(RandomNumber);
	b := (RandomNumber.D1 mod MAX_RANDOM_INTEGER);
	g := Generator;
	n := Modulus;
	Y := XpowYmodN(g,b,n);
	InterKey := Y;
end;

//----------

procedure DiffieHellman.CreateSenderEncryptionKey(var EncryptionKey,RecipientInterKey:int64);
begin
	Y := RecipientInterKey;
	K := XpowYmodN(Y,a,n);
	EncryptionKey := K;
end;

//----------

procedure DiffieHellman.CreateRecipientEncryptionKey(var EncryptionKey, SendersInterKey:int64);
begin
	X := SendersInterKey;
	K := XpowYmodN(X,b,n);
	EncryptionKey := K;
end;

//----------------------------------------------------------------------
end.

