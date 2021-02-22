unit z_beatdetector;
{$include zenith.inc}

// http://www.musicdsp.org/showArchiveComment.php?ArchiveID=200

{
  This class was designed for a VST plugin. Basically, it's just a 2nd order LP
  filter, followed by an enveloppe detector (thanks Bram), feeding a Schmitt
  trigger. The rising edge detector provides a 1-sample pulse each time a beat
  is detected. Code is self documented...
  Note : The class uses a fixed comparison level, you may need to change it.
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZBeatDetector = class
  private
    KBeatFilter   : single;   // Filter coefficient
    Filter1Out    : single;
    Filter2Out    : single;
    BeatRelease   : single;   // Release time coefficient
    PeakEnv       : single;   // Peak enveloppe follower
    BeatTrigger   : boolean;  // Schmitt trigger output
    PrevBeatPulse : boolean;  // Rising edge memory
    BeatPulse     : boolean;  // Beat detector output
  public
    constructor create;
    procedure   setup(SampleRate:single);
    function    process(input:single) : boolean;
  end;

//function fabs(value:single):Single;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

const
  FREQ_LP_BEAT  = 150.0;                            // Low Pass filter frequency
  BEAT_RTIME    = 0.02;                             // Release time of enveloppe detector in second
  T_FILTER      = 1.0 / (2.0 * PI * FREQ_LP_BEAT);  // Low Pass filter time constant

//----------

function fabs(value:single) : Single;
begin
  asm
    fld value
    fabs
    fwait
  end;
end;

//----------

// Beat detector constructor

constructor ZBeatDetector.create;
begin
  inherited;
  Filter1Out    := 0.0;
  Filter2Out    := 0.0;
  PeakEnv       := 0.0;
  BeatTrigger   := false;
  PrevBeatPulse := false;
  setup(44100);
end;

//----------

// Compute all sample frequency related coeffs

procedure ZBeatDetector.setup(sampleRate:single);
begin
  KBeatFilter := 1.0 / (sampleRate*T_FILTER);
  BeatRelease := exp( -1.0 / (sampleRate*BEAT_RTIME) );
end;

//----------

// Process incoming signal

function ZBeatDetector.process(input:single) : boolean;
var
  EnvIn:Single;
begin

  // Step 1 : 2nd order low pass filter (made of two 1st order RC filter)

  Filter1Out := Filter1Out + (KBeatFilter * (input - Filter1Out) );
  Filter2Out := Filter2Out + (KBeatFilter * (Filter1Out - Filter2Out) );

  // Step 2 : peak detector

  EnvIn := fabs(Filter2Out);
  if EnvIn>PeakEnv then PeakEnv := EnvIn  // Attack time = 0
  else
  begin
    PeakEnv := PeakEnv * BeatRelease;
    PeakEnv := PeakEnv + (1.0-BeatRelease) * EnvIn;
  end;

  // Step 3 : Schmitt trigger

  if not BeatTrigger then
  begin
    if PeakEnv>0.3 then BeatTrigger := true;
  end
  else
  begin
    if PeakEnv<0.15 then BeatTrigger:=false;
  end;

  // Step 4 : rising edge detector

  BeatPulse := false;
  if (BeatTrigger=true) and (not PrevBeatPulse) then BeatPulse:=true;
  PrevBeatPulse:=BeatTrigger;

  result := BeatPulse;

end;

//----------------------------------------------------------------------
end.
