{$include sphaera.inc}
unit syn_poly;

{$define S_MAXVOICES := 16}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  //s_dsp_osc,
  //s_dsp_rc,
  s_plugin,
  s_types,
  s_voice_simple1,
  s_voicemanager;

type

  myPlugin = class(SPlugin)
  private
    FVoices : SVoiceManager;
  public
    procedure on_create; override;
    procedure on_destroy; override;
    procedure on_parameterChange(AIndex: LongInt; AValue: Single); override;
    procedure on_midiEvent(AOffset: LongWord; AMsg1, AMsg2, AMsg3: Byte); override;
    procedure on_processBlock(inputs, outputs: PPSingle; frames: LongInt); override;
    procedure on_processSample(inputs,outputs:PPSingle); override;
    procedure on_postProcess; override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_parameter,
  s_util;

//----------

procedure myPlugin.on_create;
var
  i:longint;
begin
  FName := 'syn_poly';
  FAuthor := 'th.skei';
  SSetFlag(FFlags,spf_isSynth+spf_receiveMidi+spf_perSample);
  FVoices := SVoiceManager.create;
  for i := 0 to S_MAXVOICES-1 do FVoices.appendVoice( SVoiceSimple1.create );
  appendParameter( SParamFloat.create('gain', 1,  0, 1  ));
  appendParameter( SParamFloat.create('att',  3,  1, 50 ));
  appendParameter( SParamFloat.create('rel',  20, 1, 50 ));
  appendParameter( SParamInt.create(  'oct',  0, -4, 4  ));
  appendParameter( SParamInt.create(  'semi', 0, -12,12 ));
  appendParameter( SParamInt.create(  'cent', 0, -50,50 ));
end;

//----------

procedure myPlugin.on_destroy;
begin
  FVoices.destroy;
end;

//----------

procedure myPlugin.on_parameterChange(AIndex: LongInt; AValue: Single);
begin
  case AIndex of
    0: FVoices.control(0, AValue*AValue*AValue);
    1: FVoices.control(1, 1 / ((AValue*AValue*AValue)+S_EPSILON) );
    2: FVoices.control(2, 1 / ((AValue*AValue*AValue)+S_EPSILON) );
    3: FVoices.control(3, AValue);
    4: FVoices.control(4, AValue);
    5: FVoices.control(5, AValue/100);
  end;
end;

//----------

procedure myPlugin.on_midiEvent(AOffset: LongWord; AMsg1, AMsg2, AMsg3: Byte);
begin
  FVoices.midi(AOffset,AMsg1,AMsg2,AMsg3);
end;

//----------

procedure myPlugin.on_processBlock(inputs, outputs: PPSingle; frames: LongInt);
begin
  FVoices.setSampleRate( _sampleRate );
  FVoices.preProcess;
end;

//----------

procedure myPlugin.on_processSample(inputs,outputs:PPSingle);
var
  outs : array[0..1] of single;
begin
  FVoices.process( outs );
  outputs[0]^ := outs[0];
  outputs[1]^ := outs[1];
end;

//----------

procedure myPlugin.on_postProcess;
begin
  FVoices.postProcess;
end;

//----------------------------------------------------------------------
end.

