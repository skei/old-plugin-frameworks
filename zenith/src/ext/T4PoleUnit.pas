
// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=181

{
  Another 4-pole lowpass...
  Type : 4-pole LP/HP
  References : Posted by fuzzpilz [AT] gmx [DOT] net
  Notes :
  Vaguely based on the Stilson/Smith Moog paper, but going in a rather different
  direction from others I've seen here.
  The parameters are peak frequency and peak magnitude (g below); both are
  reasonably accurate for magnitudes above 1. DC gain is 1.
  The filter has some undesirable properties - e.g. it's unstable for low peak
  freqs if implemented in single precision (haven't been able to cleanly
  separate it into biquads or onepoles to see if that helps), and it responds so
  strongly to parameter changes that it's not advisable to update the
  coefficients much more rarely than, say, every eight samples during sweeps,
  which makes it somewhat expensive.
  I like the sound, however, and the accuracy is nice to have, since many
  filters are not very strong in that respect.
  I haven't looked at the HP again for a while, but IIRC it had approximately
  the same good and bad sides.
  [..c code..]
}
  
{
  Comments
  Added on : 04/04/05 by Christian[ AT ]savioursofsoul[ DOT ]de
  Comment :
  Yet untested object pascal translation:
}

unit T4PoleUnit;

interface

type

  TFilterType=(ftLowPass, ftHighPass);
  
  T4Pole=class(TObject)
  private
    fGain     : Double;
    fFreq     : Double;
    fSR       : Single;
  protected
    fCoeffs     : array[0..8] of Double;
    d           : array[0..3] of Double;
    fFilterType : TFilterType;
    procedure SetGain(s:Double);
    procedure SetFrequency(s:Double);
    procedure SetFilterType(v:TFilterType);
    procedure Calc;
  public
    constructor Create;
    function Process(s:single):single;
  published
    property Gain: Double read fGain write SetGain;
    property Frequency: Double read fFreq write SetFrequency;
    property SampleRate: Single read fSR write fSR;
    property FilterType: TFilterType read fFilterType write SetFilterType;
  end;

implementation

uses
  math;

//const
//  kDenorm = 1.0e-25;

constructor T4Pole.Create;
begin
  inherited create;
  fFreq:=1000;
  fSR:=44100;
  Calc;
end;

procedure T4Pole.SetFrequency(s:Double);
begin
  fFreq:=s;
  Calc;
end;

procedure T4Pole.SetGain(s:Double);
begin
  fGain:=s;
  Calc;
end;

procedure T4Pole.SetFilterType(v:TFilterType);
begin
  fFilterType:=v;
  Calc;
end;

procedure T4Pole.Calc;
var k,p,q,b,s : Double;
    a : array[0..4] of Double;
begin
  fGain:=1;
  if fFilterType=ftLowPass
    then s:=1
    else s:=-1;
  // calculating coefficients:
  k:=(4.0*fGain-3.0)/(fGain+1.0);
  p:=1.0-0.25*k;
  p:=p*p;
  if fFilterType=ftLowPass
    then b:=1.0/(tan(pi*fFreq/fSR)*(1.0+p))
    else b:=tan(pi*fFreq/fSR)/(1.0+p);
  p:=1.0+b;
  q:=s*(1.0-b);
  a[0] := 1.0/(  k+p*p*p*p);
  a[1] := 4.0*(s*k+p*p*p*q);
  a[2] := 6.0*(  k+p*p*q*q);
  a[3] := 4.0*(s*k+p*q*q*q);
  a[4] :=     (  k+q*q*q*q);
  p    := a[0]*(k+1.0);
  fCoeffs[0]:=p;
  fCoeffs[1]:=4.0*p*s;
  fCoeffs[2]:=6.0*p;
  fCoeffs[3]:=4.0*p*s;
  fCoeffs[4]:=p;
  fCoeffs[5]:=-a[1]*a[0];
  fCoeffs[6]:=-a[2]*a[0];
  fCoeffs[7]:=-a[3]*a[0];
  fCoeffs[8]:=-a[4]*a[0];

  // ths:

  d[0] := 0;
  d[1] := 0;
  d[2] := 0;
  d[3] := 0;

end;

function T4Pole.Process(s:single):single;
begin
  Result:=fCoeffs[0]*s+d[0];
  d[0]:=fCoeffs[1]*s+fCoeffs[5]*Result+d[1];
  d[1]:=fCoeffs[2]*s+fCoeffs[6]*Result+d[2];
  d[2]:=fCoeffs[3]*s+fCoeffs[7]*Result+d[3];
  d[3]:=fCoeffs[4]*s+fCoeffs[8]*Result;
end;

end.              
