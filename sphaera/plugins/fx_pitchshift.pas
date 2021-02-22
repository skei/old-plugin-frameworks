{$include sphaera.inc}
unit fx_pitchshift;

// see /src/ext/smbPitchShift.pas

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin,
  s_parameter,
  s_pitchshift;

var
  str_fft : array[0..12] of PChar = (
    '2','4','8','16','32','64','128','256','512','1024','2048','4096','8192'
  );

type

  {myDescriptor = class(ZDescriptor)
  private
  //param_tone  : ZParamFloat;
    param_shift : ZParamFloat;
    param_fft   : ZParamText;
    param_osamp : ZParamInt;
  public
    procedure   on_create; override;
  end;}

  //----------

  myPlugin = class(SPlugin)
  private
    fft     : LongInt;
    shift   : Single;
    osamp   : LongInt;
    pshift0 : SPitchShift;
    pshift1 : SPitchShift;
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processBlock(AInputs,AOutputs:PPSingle; ALength:LongInt); override;
  end;

  SPHAERA_PLUGIN = myPLugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{procedure myPlugin.on_create;
begin
  FName := 'fx_pitchshift_smb';
  FAuthor := 'stephan m. bernsee';
  FProduct := 'powered by zenith';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0103;
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
end;}

//----------------------------------------------------------------------

procedure myPlugin.on_create;
begin
  FName := 'fx_pitchshift';
  FAuthor := 'stephan m. bernsee';
  FProduct := 'ported by skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0103;
  //FFlags += [ zpf_perSample ];
  FNumInputs  := 2;
  FNumOutputs := 2;
  //param_tone := ZParamFloat.create('tone', 0, -1, 1 );
  {param_fft   :=} appendParameter( SParamText.create( 'fft size', 7, 13,  str_fft ));  // 7 = 256  2^(n+1)
  {param_shift :=} appendParameter( SParamFloat.create('shift',    1, 0.5, 2 ));
  {param_osamp :=} appendParameter( SParamInt.create(  'osamp',    4, 1,   32 ));
  //appendParameter(param_fft);
  //appendParameter(param_shift);
  //appendParameter(param_osamp);
  //
  pshift0 := SPitchShift.create;
  pshift1 := SPitchShift.create;
  fft := 256;
  shift := 1;
  osamp := 4;
end;

//----------

procedure myPlugin.on_destroy;
begin
  pshift0.destroy;
  pshift1.destroy;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  //p : ZParameter;
  v : Single;
begin
  //p := FParameters[AIndex];
  v := AValue;//p.from01(AValue);
  case AIndex of
    0:  begin
          fft := (1 shl trunc(v+1));
          pshift0.setup(fft,osamp,_sampleRate);
          pshift1.setup(fft,osamp,_sampleRate);
        end;
    1:  begin
          shift := v;
        end;
    2:  begin
          osamp := trunc(v);
          pshift0.setup(fft,osamp,_sampleRate);
          pshift1.setup(fft,osamp,_sampleRate);
        end;
  end;
end;

//----------

procedure myPlugin.on_processBlock(AInputs,AOutputs:PPSingle; ALength:LongInt);
begin
  pshift0.PitchShift(shift,ALength{,fft,osamp,FSampleRate},AInputs[0],AOutputs[0]);
  pshift1.PitchShift(shift,ALength{,fft,osamp,FSampleRate},AInputs[1],AOutputs[1]);
end;

//----------------------------------------------------------------------
end.

