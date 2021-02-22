{$include sphaera.inc}
unit s_dsp_svf;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const
  svf_none = 0;
  svf_lp   = 1;
  svf_hp   = 2;
  svf_bp   = 3;
  svf_n    = 4;


type

  SDspSVF = class
  private
    z1,z2     : Single;
    FMode     : LongInt;
    FFreq     : Single;
    FBW       : Single;
  public
    constructor create;
    function    process(AValue:Single) : Single;
    property    _mode : LongInt read FMode write FMode;
    property    _freq : Single read FFreq write FFreq;
    property    _bw : Single read FBW write FBW;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const;

//----------

constructor SDspSVF.create;
begin
  inherited;
  z1  := 0;
  z2  := 0;
  FMode := 0;
  FFreq := 1;
  FBW   := 1;
end;

//----------

function SDspSVF.process(AValue:Single) : Single;
var
  //in_L : Single;
  //out_L : Single;
  L,H,B,N : Single;
begin
  //in_L := AValue;
  result{out_L} := 0;
  if FMode = 0 then result{out_L} := AValue{in_L}
  else
  begin
    L := z2 + FFreq * z1;
    H := AValue{in_L} - L - FBW   * z1;
    B := FFreq * H + z1;
    N := H + L;
    z1 := B;
    z2 := L;
    case FMode of
      1: result{out_L} := L;
      2: result{out_L} := H;
      3: result{out_L} := B;
      4: result{out_L} := N;
    end; //case
  end; //FMode
  //result := out_L;
end;

//----------------------------------------------------------------------
end.

