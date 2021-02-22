{$include sphaera.inc}
unit fx_gain;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  //comobj,
  s_const,
  s_parameter,
  s_plugin,
  s_types,
  s_util;

type

  myPlugin = class(SPlugin)
  private
    FLeft  : Single;
    FRight : Single;
  //var
  //  SavedCW: Word;
  //  SpVoice: Variant;
  public
    procedure on_create; override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPLugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

procedure myPlugin.on_create;
begin
  FName   := 'gain';
  FAuthor := 'tor-helge skei';
  SSetFlag(FFlags,spf_perSample);
  appendParameter( SParameter.create('left', 0.5) );
  appendParameter( SParameter.create('right',0.5) );

  (*
  // http://msdn.microsoft.com/en-us/library/ms723609(v=vs.85).aspx
  SpVoice := CreateOleObject('SAPI.SpVoice');
  // Change FPU interrupt mask to avoid SIGFPE exceptions
  SavedCW := Get8087CW;
  //try
    Set8087CW(SavedCW or $4);
    SpVoice.Speak('this is a test', 0);
  //finally
    // Restore FPU mask
    Set8087CW(SavedCW);
  //end;
  *)

end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  case AIndex of
    0 : FLeft  := AValue;
    1 : FRight := AValue;
  end;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs: PPSingle);
begin
  AOutputs[0]^ := AInputs[0]^ * FLeft;
  AOutputs[1]^ := AInputs[1]^ * FRight;
end;

//----------------------------------------------------------------------
end.

