{$include sphaera.inc}
unit s_dsp_dcf;

{
  DC offset filter.

  one pole, one zero DC offset filter from julius orion smith (jos). <br>
  http://www-ccrma.stanford.edu/~jos/filters/ <br>
  usage:
  // include "dspDCF.h"
  // create instance
  dspDCF dcfilter;
  // setup
  float dcout = dcfilter.process(input); // process input sample
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SDspDCF = class
  private
    x1,y1 : Single;
    R : Single;
  public
    constructor create;
    function    process(AValue:Single) : Single;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const;

//----------

constructor SDspDCF.create;
begin
  inherited;
  x1 := 0;
  y1 := 0;
  R := 0.999;
end;

//----------

function SDspDCF.process(AValue:Single) : Single;
var
  y : Single;
begin
  y := AValue - x1 + (R*y1) + S_DENORM;
  x1 := AValue;
  y1 := y;
  result := y - S_DENORM;
end;

//----------------------------------------------------------------------
end.

