unit fx_tempodelay;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  zenith,
  z_const,
  z_types,
  z_descriptor,
  z_instance,
  z_parameter;

{
  buffersize = beats * ( (60/tempo) * samplerate )
  tempo = 30, samplerate = 192k, beats = 4
    4 * ( (60/30) * 192000 )
  = 4 * (  2      * 192k )
  = 4 * 384
  = 1536k * stereo (2)
  = 3072k
  = 3mb

}

const
  MAX_DELAY_LENGTH = 1024*1024*4;

type

  myDescriptor = class(ZDescriptor)
  public
    procedure   on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    FBeats      : Single;
    FFeedback   : Single;
    FWet        : Single;
    FDry        : Single;
    FBuffer     : array of Single;
    FBufferPos  : LongWord;
    FBufferSize : LongWord;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPZSample); override;
    procedure   on_processBlock({%H-}AInputs,{%H-}AOutputs:PPZSample; {%H-}ALength:LongWord); override;
  end;

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_util;

//----------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

//----------------------------------------------------------------------

procedure myDescriptor.on_create;
begin
  FName := 'fx_tempodelay';
  FAuthor := 'th.skei';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := Z_MAGIC + $0106;
  FNumInputs := 2;
  FNumOutputs := 2;
  ZSetFlag(FFlags,zpf_perSample);
  appendParameter( ZParamFloat.create('beats',    0.75, (1/16), 4, (1/16) ));
  appendParameter( ZParamFloat.create('feedback', 0.75 ));
  appendParameter( ZParamFloat.create('dry',      1 ));
  appendParameter( ZParamFloat.create('wet',      0.25 ));
end;

//----------------------------------------------------------------------

procedure myInstance.on_create;
begin
  FBeats := 0;
  FFeedback := 0;
  FWet := 0;
  FDry := 1;
  SetLength(FBuffer,MAX_DELAY_LENGTH*2);
  FBufferPos :=  0;
  FBufferSize := 0;

  {FPlayState  := 0;
  FSampleRate := 0;
  FSamplePos  := 0;
  FTempo      := 0;
  FBeatPos    := 0;}

end;

//----------

procedure myInstance.on_destroy;
begin
  SetLength(FBuffer,0);
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : Single;
begin
  //writeln('...');
  p := FParameters[AIndex];
  v := p.from01(AValue);
  case AIndex of
    0: FBeats := v;
    1: FFeedback := v;
    2: FDry := v;
    3: FWet := v;
  end;
end;

//----------

procedure myInstance.on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
var
  beatsize : Single;
  //playstate : LongInt;
  samplerate : Single;
  samplepos : Single;
  tempo : Single;
  beatpos : Single;
begin
  // Hint: Local variable "FBeatPos" does not seem to be initialized
  {playstate :=} FHost.getTime(samplerate{%H-},samplepos{%H-},tempo{%H-},beatpos{%H-});
  beatsize := ( 60.0 / tempo ) * samplerate;
  FBufferSize := trunc( FBeats * beatsize );
  if FBufferPos > 0 then
    while FBufferPos >= FBufferSize do FBufferPos -= FBufferSize;
end;

//----------

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
var
  in0,in1 : single;
  buf0,buf1 : single;
  //out0,out1 : single;
  pos2 : LongWord;
begin
  pos2 := FBufferPos * 2;
  FBufferPos += 1;
  if FBufferPos >= FBufferSize then FBufferPos := 0;
  in0   := AInputs[0]^;
  in1   := AInputs[1]^;
  buf0 := FBuffer[pos2];
  buf1 := FBuffer[pos2+1];
  FBuffer[pos2] := in0 + buf0*FFeedback;
  FBuffer[pos2+1] := in1 + buf1*FFeedback;
  AOutputs[0]^ := in0*FDry + buf0*FWet;
  AOutputs[1]^ := in1*FDry + buf1*FWet;
end;

//----------------------------------------------------------------------
end.
