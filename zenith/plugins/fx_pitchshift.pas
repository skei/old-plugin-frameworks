unit fx_pitchshift;
{$include zenith.inc}

// see /src/ext/smbPitchShift.pas

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  zenith,
  z_const, z_types,
  z_descriptor,
  z_instance,
  z_parameter,
  z_pitchshift;

var
  str_fft : array[0..12] of PChar = (
    '2','4','8','16','32','64','128','256','512','1024','2048','4096','8192'
  );

type

  myDescriptor = class(ZDescriptor)
  private
  //param_tone  : ZParamFloat;
    param_shift : ZParamFloat;
    param_fft   : ZParamText;
    param_osamp : ZParamInt;
  public
    procedure   on_create; override;
  end;

  //----------

  myInstance = class(ZInstance)
  private
    fft     : LongInt;
    shift   : Single;
    osamp   : LongInt;
    pshift0 : ZPitchShift;
    pshift1 : ZPitchShift;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord); override;
  end;

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

procedure zenith_prepare;
begin
  zenith_register(myDescriptor);
  zenith_register(myInstance);
end;

procedure myDescriptor.on_create;
begin
  FName := 'fx_pitchshift_smb';
  FAuthor := 'stephan m. bernsee';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := Z_MAGIC + $0103;
  //FFlags += [ zpf_perSample ];
  FNumInputs  := 2;
  FNumOutputs := 2;
  //param_tone := ZParamFloat.create('tone', 0, -1, 1 );
  param_fft   := ZParamText.create( 'fft size', 7, 13,  str_fft);  // 7 = 256  2^(n+1)
  param_shift := ZParamFloat.create('shift',    1, 0.5, 2);
  param_osamp := ZParamInt.create(  'osamp',    4, 1,   32);
  appendParameter(param_fft);
  appendParameter(param_shift);
  appendParameter(param_osamp);
end;

//----------------------------------------------------------------------

procedure myInstance.on_create;
begin
  pshift0 := ZPitchShift.create;
  pshift1 := ZPitchShift.create;
  fft := 256;
  shift := 1;
  osamp := 4;
end;

//----------

procedure myInstance.on_destroy;
begin
  pshift0.destroy;
  pshift1.destroy;
end;

//----------

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  p : ZParameter;
  v : Single;
begin
  p := FParameters[AIndex];
  v := p.from01(AValue);
  case AIndex of
    0:  begin
          fft := (1 shl trunc(v+1));
          pshift0.setup(fft,osamp,FSampleRate);
          pshift1.setup(fft,osamp,FSampleRate);
        end;
    1:  begin
          shift := v;
        end;
    2:  begin
          osamp := trunc(v);
          pshift0.setup(fft,osamp,FSampleRate);
          pshift1.setup(fft,osamp,FSampleRate);
        end;
  end;
end;

//----------

procedure myInstance.on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
begin
  pshift0.PitchShift(shift,ALength{,fft,osamp,FSampleRate},AInputs[0],AOutputs[0]);
  pshift1.PitchShift(shift,ALength{,fft,osamp,FSampleRate},AInputs[1],AOutputs[1]);
end;

//----------------------------------------------------------------------
end.

