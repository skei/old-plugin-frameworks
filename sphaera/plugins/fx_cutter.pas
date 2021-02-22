{$include sphaera.inc}
unit fx_cutter;

{$define s_buffer_size := 1024*1024*10}

{
8 beats
samplerate 44100, 120 bpm = 0.5 sec per beat = 22050 * 2 = stereo = 44100
44100 per beat * 8 =/ 50k*8 0= 400k
96khz = *4
30 bpm * 4 = *16
400 * 16
100 * 64 = 6400k 6mb
}

{
  todo:
  - statechange, pos = 0, reset..
}


//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  //s_editor,
  //s_painter,
  //s_paint_color,
  //s_paint_knob,
  //s_paint_wave,
  s_parameter,
  s_plugin,
  s_types,
  s_widget,
  s_widget_wave,
  s_window;

type

  myPlugin = class(SPlugin)
  private
    FBuffer       : array[0..s_buffer_size-1] of Single;
    FBufferSize   : longint;
    FRecPos       : LongInt;
    FPlayPos      : Single;
    FSubPos       : single;
    FSubSize      : single;
    FIsRepeating  : Boolean;
    FPlayRate     : Single;
    FRepeatStart  : single;
    FRepeatSize   : single;
    //
    //p_back        : SPaintColor;
    //p_knob        : SPaintKnob;
    //p_wave        : SPaintWave;
    w_wave        : SWidgetWave;
    //
    num_beats     : longint;
    num_subdiv    : longint;
    max_repeat    : longint;
    cut_prob      : single;
    prob_pitchup  : single;
    prob_pitchdown : single;
    prob_halve    : single;
    prob_double   : single;
    //
  public
    procedure on_create; override;
    procedure on_stateChange(AState: LongInt); override;
    procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure on_processSample(AInputs,AOutputs:PPSingle); override;
    procedure on_processBlock({%H-}inputs,{%H-}outputs:PPSingle; {%H-}frames:LongInt); override;
    procedure on_openEditor; override;
    procedure on_closeEditor; override;
    procedure on_idleEditor; override;
  end;

  //----------

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //s_color,
  //s_math,
  s_random,
  s_rect,
  s_util,
  s_widget_knob,
  s_widget_panel;

//----------------------------------------------------------------------
// plugin
//----------------------------------------------------------------------

procedure myPlugin.on_create;
begin
  FName       := 'fx_cutter';
  FAuthor     := 'th.skei';
  FVersion    := 0;
  FUniqueId   := S_MAGIC + $0101;
  FNumInputs  := 2;
  FNumOutputs := 2;
  FEditorRect := rect(260,190);
  SSetFlag(FFlags,spf_perSample + spf_hasEditor);
  appendParameter( SParamInt.create('num beats',4,1,8) );
  appendParameter( SParamInt.create('beat subdiv',2,1,4) );
  appendParameter( SParamInt.create('max repeat',3,1,8) );
  appendParameter( SParameter.create('cut prob',0.25) );
  appendParameter( SParameter.create('speed up',0.25) );
  appendParameter( SParameter.create('speed down',0.25) );
  appendParameter( SParameter.create('double',0.25) );
  appendParameter( SParameter.create('halve',0.25) );
  //
  FRecPos := 0;
  FSubPos := 0;
  FIsRepeating := False;
end;

//----------

procedure myPlugin.on_stateChange(AState:LongInt);
begin
  case AState of
    sps_resume:
      begin
        FRecPos := 0;
        FIsRepeating:=False;;
      end;
  end; // case
end;

//----------

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  case AIndex of
    0: num_beats := trunc(AValue);
    1: num_subdiv := trunc(AValue);
    2: max_repeat := trunc(AValue);
    3: cut_prob := AValue;
    4: prob_pitchup := AValue;
    5: prob_pitchdown := AValue;
    6: prob_double := AValue;
    7: prob_halve := AValue;
  end;
end;

//----------

procedure myPlugin.on_processBlock(inputs,outputs:PPSingle; frames:LongInt);
var
  //playstate : longint;
  //samplerate,samplepos,tempo,beatpos : single;
  beat_size : single;
begin
  //playstate := getTime(samplerate,samplepos,tempo,beatpos);
  updateSync;
  beat_size := _sampleRate * (60/_tempo);
  FBufferSize := trunc( beat_size * num_beats );
  FSubSize := beat_size / num_subdiv;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
  num,p2 : longint;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  { trigger }

  FSubPos += 1;
  if FSubPos >= FSubSize then
  begin
    { tick }
    spl0 := 0.9;
    FSubPos := 0;
    if FIsRepeating then
    begin
      if SRandom < prob_pitchup then FPlayRate *= 2;
      if SRandom < prob_pitchdown then FPlayRate *= 0.5;
      if SRandom < prob_double then FRepeatSize *= 2;
      if SRandom < prob_halve then FRepeatSize *= 0.5;
      FRepeatSize -= 1;
      if FRepeatSize <= 0 then
      begin
        FIsRepeating := False;
        FPlayRate:=1;
      end;
    end;
    //else
    if not FIsRepeating then
    begin
      // start new repeat?
      if SRandom < cut_prob then
      begin
        // start new cut
        num :=  trunc( SRandom * max_repeat ); // TODO!!
        FIsRepeating := True;
        FPlayRate := 1;
        FRepeatStart := FRecPos;
        FRepeatSize := FSubSize * num;
      end;
    end;
  end;

  { record }

  p2 := FRecPos * 2;
  FBuffer[p2] := spl0;
  FBuffer[p2+1] := spl1;
  FRecPos += 1;
  if FRecPos >= FBufferSize then
  begin
    spl1 := 0.9;
    FRecPos := 0;
    FSubPos := 0;
  end;

  { play }

  p2 := trunc(FPlayPos) * 2;
  spl0 := FBuffer[p2];
  spl1 := FBuffer[p2+1];
  FPlayPos += FPlayRate;
  if FPlayPos >= FBufferSize then FPlayPos := 0;

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------
// editor
//----------------------------------------------------------------------

procedure myPlugin.on_openEditor;
var
  win  : SWindow;
  back : SWidget;
  wdg  : SWidget;
  //wave : SWidgetWave;
begin

  //p_back := SPaintColor.create(s_grey);
  //p_knob := SPaintKnob.create(s_dark_grey,s_light_grey,s_light_grey);
  //p_wave := SPaintWave.create(s_dark_grey,s_light_grey,s_light_yellow);

  win := _editor.getWindow;
  back := SWidgetPanel.create( FEditorRect, swa_fillClient{, p_back} );
  win.appendWidget(back);

  wdg := SWidgetKnob.create( rect(10,10,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[0], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(10,40,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[1], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(10,70,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[2], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(10,100,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[3], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(130,10,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[4], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(130,40,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[5], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(130,70,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[6], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(130,100,120,30), swa_none{, p_knob} );
  _editor.connect( FParameters[7], wdg );
  back.appendWidget(wdg);

  w_wave := SWidgetWave.create( rect(10,130,240,50), swa_none{, p_wave} );
  back.appendWidget(w_wave);

end;

//----------

procedure myPlugin.on_closeEditor;
begin
  //p_back.destroy;
  //p_knob.destroy;
  //p_wave.destroy;
end;

//----------

procedure myPlugin.on_idleEditor;
begin

  w_wave.buffer := FBuffer;
  w_wave.size := FBufferSize;
  w_wave.numbeats := num_beats;
  w_wave.subdiv := num_subdiv;
  w_wave.recpos := FRecPos;

  // force redraw of wave display
  // invalidate -> wm_paint -> on_paint widget hierarchy -> blit
  _editor.getWindow.redraw(w_wave);

end;

//----------------------------------------------------------------------
end.

