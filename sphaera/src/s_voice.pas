{$include sphaera.inc}
unit s_voice;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_array,
  s_list;

const
  // voice states
  svs_off       = 0;
  svs_playing   = 1;
  svs_released  = 2;

type

  SVoice = class(SListNode)
  protected
    FState : LongInt;
    FIRate  : Single;
  public
    constructor create; //mState = vs_Off;
    destructor  destroy; override;
    procedure   on_setSampleRate(ARate:Single); virtual;
    procedure   on_noteOn(ANote,AVel:LongInt); virtual;
    procedure   on_noteOff(ANote,AVel:LongInt); virtual;
    procedure   on_ctrlChange(ACtrl,AVal:LongInt); virtual;
    procedure   on_control(AIndex:LongInt; AVal:Single); virtual;
    procedure   on_process(outs:PSingle); virtual;
    //functionl float on_process(void) : Single; virtual;
  public
    property _state : LongInt read FState write FState;
  end;

  SVoices = specialize SArray<SVoice>;


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SVoice.create;
begin
  inherited;
  FState := svs_off;
end;

//----------

destructor SVoice.destroy;
begin
  inherited;
end;

//----------

procedure SVoice.on_setSampleRate(ARate:Single);
begin
  FIRate := 1/ARate;
end;

//----------

procedure SVoice.on_noteOn(ANote,AVel:LongInt);
begin
end;

//----------

procedure SVoice.on_noteOff(ANote,AVel:LongInt);
begin
end;

//----------

procedure SVoice.on_ctrlChange(ACtrl,AVal:LongInt);
begin
end;

//----------

procedure SVoice.on_control(AIndex:LongInt; AVal:Single);
begin
end;

//----------

procedure SVoice.on_process(outs:PSingle);
begin
end;

//----------

//functionl SVoice.on_process : Single;
//begin
//  result := 0;
//end;

//----------------------------------------------------------------------
end.

