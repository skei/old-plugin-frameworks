{$include sphaera.inc}
unit fx_pitch;

{
  buggy?
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin;

type

  myPlugin = class(SPlugin)
  private
    in_pos    : Single;
    out_pos   : Single;
    base      : Single;
    old_base  : Single;
    fade_base : Single;
    inc_      : Single;
    len_      : Single;
    fade_     : Single;
    buffer    : array[0..1024*1024] of Single;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_math,
  s_memory,
  s_parameter,
  s_util;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_pitch';
  FAuthor := 'gruhn';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample);
  FNumInputs  := 2;
  FNumOutputs := 2;
  {
  slider1:100<50,200,1>Pitch (%)
  slider2:100<0,200,1>Window (ms)
  slider3:100<0,100,1>Cross-Fade (%)
  }
  appendParameter( SParamFloat.create('pitch (%)',    100, 50,200,1) );
  appendParameter( SParamFloat.create('window (ms)',  100,  0,200,1) );
  appendParameter( SParamFloat.create('crossfade (%)',100,  0,100,1) );
  in_pos    := 0;
  out_pos   := 0;
  base      := 0;
  old_base  := 0;
  fade_base := 0;
  inc_      := 0;
  len_      := 0;
  fade_     := 0;
  SMemset(@buffer,0,sizeof(buffer));
end;

//----------

procedure myPlugin.on_destroy;
begin
end;

//----------

{
  inc = slider1/100;
  // len must be multiple of 2 otherwise there will be drift due to floating point math in 1sdft93hgosdh
  len = floor(slider2/1000*srate/2)*2;
  fade = slider3/100 * len/2;
}

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  case AIndex of
    0: inc_  := AValue/100;
    1: len_  := trunc(AValue/1000*_sampleRate/2)*2;
    2: fade_ := AValue/100 * len_/2;
  end;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  gain      : single;
  in_,out_  : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  //
  in_ := spl0;
  // fill buffer
  buffer[ trunc(in_pos) ] := in_;
  // increment in_pos
  in_pos := trunc(in_pos+1) mod trunc(len_);
  // generate the output; first calculate the xfade gain; then mix the to
  // from both base and old_base and xfade
  gain := SMin(out_pos/fade_,1);
  out_ := buffer[ trunc(base+out_pos) mod trunc(len_) ] * gain
        + buffer[ trunc(fade_base+out_pos) mod trunc(len_) ] * (1-gain);
  // increment the out_pos by inc (pitch!) ref 1sdft93hgosdh and check whether
  // we need to initialize a fade
  out_pos += inc_;
  if out_pos >= (len_-1-fade_) then
  begin
  	fade_base := base + len_ - 1 - fade_;
  	out_pos := 0;
    base := in_pos;
  end;
  spl0 := out_;
  spl1 := out_;
  //
  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
end.

