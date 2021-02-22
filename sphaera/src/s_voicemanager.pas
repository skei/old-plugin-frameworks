{$include sphaera.inc}
unit s_voicemanager;

{$define MAX_EVENTS := 1024}

{
  - max block size: 999999 samples
      a biiig offset that we will hopefully never reach
  - max events per block: #define MAX_EVENTS 1024
      a static buffer for the incoming events, to avoid runtime memory juggling
  - very basic nna/note-stealing (works ok-ish)
  - currently only note on/off
  - converts events in processSample
      could be done in processBlock (does it matter?)
  - not optimized
      but 16 basic saw voices uses only around 0.8% cpu (reaper/wine)
      so i've postponed that until it gets problematic
  - bugs?
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_list,
  s_voice;

type

  vm_event = record
    ofs:longint;
    msg1,msg2,msg3:byte;
    padding:byte;
  end;

  //----------

  SVoiceManager = class
  private
    FOffset     : Longint;
    FNextEvent  : Longint;
    FCurrEvent  : Longint;
    FNumEvents  : Longint;
    FEvents     : array[0..MAX_EVENTS-1] of vm_event;
    FNumPlaying : Longint;
  protected
    FNoteMap        : array[0..127] of SVoice;
    FAllVoices      : SVoices;
    FFreeVoices     : SList;
    FPlayingVoices  : SList;
    FReleasedVoices : SList;
  public
    constructor create;
    destructor  destroy; override;
    procedure   appendVoice(v:SVoice);
    procedure   deleteVoices;
    procedure   setSampleRate(ARate:Single);
    procedure   noteOn(ANote,AVel:LongInt);
    procedure   noteOff(ANote,AVel:LongINt);
    procedure   control(AIndex:LongInt; AVal:Single);
    procedure   midi(ofs:longint; msg1,msg2,msg3:byte);
    procedure   preProcess;
    procedure   process(outs:PSingle);
    procedure   postProcess;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_memory;

//----------

{

  [assume events in buffer are sorted by their offset in the audiobuffer]

  we have:
  - an array with all available voices (allocated at startup)
  - a list of available voices (initially containing ptr to all available voices)
  - a list of playing voices
  - a list of released (decaying) voices
  note on:
  - grab a voice from the free voices list, and append it to the playing list
  - if no voice available in the free list, take the oldest voice from decaying voices
  note off:
  - move the voice from the playing to the released list
  process:
  - process both playing and released lists,
    call process for each voice, and add their outputs
    voices can set their mState to vs_Off to shut themselves off (see post-process)
  post-process:
  - move all voices with mState == vs_Off from released to free voices list
  - cleanup buffers and counters

  midi:
  we copy all incoming midi events to a buffer.
  during sample processing:
  keep track of current offset (position in buffer), and next event to process
  when offset reaches next event, we fire off note on/ff etc events
  and continue, looking at the next event and its offset


  after the block has finished, we reset the buffers and offset
  and prepare for next block

}

constructor SVoiceManager.create;
begin
  inherited;
  SMemset(@FNoteMap[0],0,sizeof(FNoteMap));
  SMemset(@FEvents[0],0,sizeof(FEvents));
  FAllVoices      := SVoices.create;
  FFreeVoices     := SList.create;
  FPlayingVoices  := SList.create;
  FReleasedVoices := SList.create;
  FOffset         := 0;
  FCurrEvent      := 0;
  FNextEvent      := 999999;
  FNumEvents      := 0;
  FNumPlaying     := 0;
end;

//----------

destructor SVoiceManager.destroy;
begin
  {$ifndef S_NOAUTODELETE}
  deleteVoices;
  {$endif}
  FAllVoices.destroy;
  FFreeVoices.destroy;
  FPlayingVoices.destroy;
  FReleasedVoices.destroy;
  inherited;
end;

//----------------------------------------

procedure SVoiceManager.appendVoice(v:SVoice);
begin
  FAllVoices.append(V);
  FFreeVoices.append(V);
end;

//----------

procedure SVoiceManager.deleteVoices;
var
  i : longint;
begin
  if FAllVoices.size > 0 then
  begin
    for i := 0 to FAllVoices.size-1 do FAllVoices[i].destroy;
  end;
end;

//----------

procedure SVoiceManager.setSampleRate(ARate:Single);
var
  i : longint;
begin
  if FAllVoices.size > 0 then
  begin
    for i := 0 to FAllVoices.size-1 do
    begin
      FAllVoices[i].on_setSampleRate(ARate);
    end;
  end;
end;

//----------------------------------------

procedure SVoiceManager.noteOn(ANote,AVel:LongInt);
var
  V:SVoice;
begin
  V := SVoice( FFreeVoices._tail );
  if Assigned(V) then FFreeVoices.removeTail
  else
  begin
    V := SVoice( FReleasedVoices._head );
    if Assigned(V) then FReleasedVoices.removeHead;
  end;
  if Assigned(V) then
  begin
    FNoteMap[ANote] := V;
    FPlayingVoices.append(V);
    V._state := svs_playing;
    V.on_noteOn(ANote,AVel);
    inc(FNumPlaying);
  end;
end;

//----------

procedure SVoiceManager.noteOff(ANote,AVel:LongInt);
var
  V:SVoice;
begin
  V := FNoteMap[ANote];
  if Assigned(V) then
  begin
    FNoteMap[aNote] := nil;
    FPlayingVoices.remove(V);
    FReleasedVoices.append(V);
    V._state := svs_released;
    V.on_noteOff(ANote,AVel);
    inc(FNumPlaying);
  end;
end;

//----------

procedure SVoiceManager.control(AIndex:LongInt; AVal:Single);
var
  i:longint;
begin
  if FAllVoices.size > 0 then
  begin
    for i := 0 to FAllVoices.size-1 do FAllVoices[i].on_control(AIndex,AVal);
  end;
end;

//----------

procedure SVoiceManager.midi(ofs:longint; msg1,msg2,msg3:byte);
begin
  //trace(" ofs "<<ofs << " msg1 "<<msg1 << " msg2 "<<msg2 << " msg3 "<<msg3);
  if FNumEvents < MAX_EVENTS then
  begin
    FEvents[FNumEvents].ofs  := ofs;
    FEvents[FNumEvents].msg1 := msg1;
    FEvents[FNumEvents].msg2 := msg2;
    FEvents[FNumEvents].msg3 := msg3;
    inc(FNumEvents);
  end;
end;

//----------

procedure SVoiceManager.preProcess;
begin
  FOffset := 0;
  FCurrEvent := 0;
  if FNumEvents > 0 then FNextEvent := FEvents[0].ofs
  else FNextEvent := 999999;
end;

//----------

{
  stereo version..
  todo: multichannel..
}

procedure SVoiceManager.process(outs:PSingle);
var
  V:SVoice;
  msg,note,vel : longint;
  left,right : single;
  _outs : array[0..1] of single;
begin

  { events }

  while FOffset = FNextEvent do
  begin
    //int chn  = mEvents[mCurrEvent].msg1 & 0x0f;
    msg  := FEvents[FCurrEvent].msg1 and $f0;
    note := FEvents[FCurrEvent].msg2;
    vel  := FEvents[FCurrEvent].msg3;
    case msg of
      $90: { note on }
        begin
          if vel = 0 then noteOff(note,vel)
          else noteOn(note,vel);
        end;
      $80: { note off }
        begin
          note := FEvents[FCurrEvent].msg2;
          vel  := FEvents[FCurrEvent].msg3;
          noteOff(note,vel);
        end;
    end; // case
    inc(FCurrEvent);
    if FCurrEvent < FNumEvents then FNextEvent := FEvents[FCurrEvent].ofs
    else FNextEvent := 999999;
  end; // while
  inc(FOffset);
  //float out = 0;
  left := 0;
  right := 0;

  { playing voices }

  V := SVoice(FPlayingVoices._head);
  while Assigned(V) do
  begin
    //out += V->process();
    V.on_process(@_outs[0]);
    left += _outs[0];
    right += _outs[1];
    V := SVoice(V._next);
  end;

  { released voices }

  V := SVoice(FReleasedVoices._head);
  while Assigned(V) do
  begin
    if V._state <> svs_off then //out += V->process();
    begin
      V.on_process(@_outs[0]);
      left += _outs[0];
      right += _outs[1];
    end;
    V := SVoice(V._next);
  end;

  outs[0] := left;
  outs[1] := right;
end;

//----------

procedure SVoiceManager.postProcess;
var
  V,next:SVoice;
begin
  V := SVoice(FReleasedVoices._head);
  while Assigned(V) do
  begin
    if V._state = svs_off then
    begin
      next := SVoice(V._next);
      FReleasedVoices.remove(V);
      FFreeVoices.append(V);
      V := next;
    end
    else V := SVoice(V._next);
  end;
  FNumEvents := 0;
end;

//----------------------------------------------------------------------
end.

