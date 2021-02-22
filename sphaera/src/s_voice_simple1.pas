{$include sphaera.inc}
unit s_voice_simple1;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_dsp_osc,
  s_dsp_rc,
  s_voice;

type

  SVoiceSimple1 = class(SVoice)
  private
    gain, attack,release, tuneoct, tunesemi, tunecent, vel : Single;
    att1,rel1 : SDspRC;
    osc1,osc2 : SDspOsc;
  public
    constructor create;
    destructor  destroy; override;
    procedure   on_setSampleRate(ARate: Single); override;
    procedure   on_noteOn(ANote, AVel: LongInt); override;
    procedure   on_noteOff(ANote, AVel: LongInt); override;
    procedure   on_control(AIndex: LongInt; AVal: Single); override;
    procedure   on_process(outs: PSingle); override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  s_const;

//----------

constructor SVoiceSimple1.create;
begin
  inherited;
  osc1 := SDspOsc.create;;
  osc2 := SDspOsc.create;;
  att1 := SDspRC.create;;
  rel1 := SDspRC.create;;
  osc1.setup(osc_Ramp,0,0);
  osc2.setup(osc_Squ,0,0);
  att1.setup(0,1,0);
  rel1.setup(0,0,0);
end;

//----------

destructor SVoiceSimple1.destroy;
begin
  inherited;
end;

//----------

procedure SVoiceSimple1.on_setSampleRate(ARate: Single);
begin
  osc1.setSampleRate(aRate);
  osc2.setSampleRate(aRate);
  inherited;
end;

//----------

procedure SVoiceSimple1.on_noteOn(ANote, AVel: LongInt);
var
  detune : single;
  fr1,fr2:single;
begin
  vel := Single(aVel) * S_INV127;
  att1._value := 0;         // start from 0
  att1._weight := attack;   // fade up
  rel1._value := 1;         // initially, set to full on
  rel1._Weight := 0;        // fade speed = 0 (until note off)
  detune := (tuneoct*12) + tunesemi + tunecent;
  fr1 := 440 * power(2.0,(ANote-69.0) / 12);
  fr2 := 440 * power(2.0,(ANote-69.0+detune) / 12);
  osc1._freq := fr1;
  osc2._freq := fr2;
end;

//----------

procedure SVoiceSimple1.on_noteOff(ANote, AVel: LongInt);
begin
  rel1._value := att1._value; // start from current amplitude
  rel1._weight := release;    // and let it fade down
end;

//----------

procedure SVoiceSimple1.on_control(AIndex: LongInt; AVal: Single);
begin
  case AIndex of
    0: gain     := aVal;
    1: attack   := aVal;
    2: release  := aVal;
    3: tuneoct  := aVal;
    4: tunesemi := aVal;
    5: tunecent := aVal;
  end;
end;

//----------

procedure SVoiceSimple1.on_process(outs: PSingle);
var
  out1,out2:single;
  a,r:single;
begin
  out1 := osc1.process;
  out2 := osc2.process;
  a := att1.process;
  r := rel1.process;
  if  r < S_EPSILON then FState := svs_off;
  outs[0] := out1*vel*a*r*gain;
  outs[1] := out2*vel*a*r*gain;
end;


//----------------------------------------------------------------------
end.

