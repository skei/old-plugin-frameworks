{$include sphaera.inc}
unit plug_test1;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_parameter,
  s_plugin,
  s_types,
  s_widget_panel,
  s_widget_text,
  s_window;

type

  myPlugin = class(SPlugin)
  private
    panel : SWidgetPanel;
    txt1 : SWidgetText;
    txt2 : SWidgetText;
    txt3 : SWidgetText;
    txt4 : SWidgetText;
    txt5 : SWidgetText;
    txt1_buf : array[0..32] of char;
    txt2_buf : array[0..32] of char;
    txt3_buf : array[0..32] of char;
    txt4_buf : array[0..32] of char;
    txt5_buf : array[0..32] of char;
  public
    procedure on_create; override;
    procedure on_stateChange(AState:LongInt); override;
    procedure on_transportChange(AState:LongInt); override;
    //procedure on_parameterChange(AIndex:LongInt; AValue:Single); override;
    //procedure on_processSample(AInputs,AOutputs:PPSingle); override;
    procedure on_openEditor; override;
  end;

  //----------

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_math,
  s_rect,
  s_util,
  s_widget;

//----------------------------------------------------------------------

procedure myPlugin.on_create;
begin
  FName       := 'plug_test1';
  FAuthor     := 'th.skei';
  FVersion    := 0;
  FUniqueId   := S_MAGIC + $0101;
  FNumInputs  := 2;
  FNumOutputs := 2;
  FEditorRect := rect(400,300);
  SSetFlag(FFlags,{spf_perSample +} spf_hasEditor + spf_autoSync);
  txt1_buf[0] := #0;
  txt2_buf[0] := #0;
  txt3_buf[0] := #0;
  txt4_buf[0] := #0;
  txt5_buf[0] := #0;
end;

//----------

procedure myPlugin.on_stateChange(AState:LongInt);
begin
  case AState of
    sps_suspend:
      begin
        txt1_buf := 'suspend';
        FEditor.redrawWidget(txt1);
      end;
    sps_resume:
      begin
        txt1_buf := 'resume';
        FEditor.redrawWidget(txt1);
      end;
    sps_start:
      begin
        txt1_buf := 'start';
        FEditor.redrawWidget(txt1);
      end;
    sps_stop:
      begin
        txt1_buf := 'stop';
        FEditor.redrawWidget(txt1);
      end;
  end;
end;

procedure myPlugin.on_transportChange(AState:LongInt);
begin
  (*
  {play state }
  if SHasFlag(AState,spt_play)   then txt1_buf[0] := 'p' else txt1_buf[0] := '-';
  if SHasFlag(AState,spt_cycle)  then txt1_buf[1] := 'c' else txt1_buf[1] := '-';
  if SHasFlag(AState,spt_record) then txt1_buf[2] := 'r' else txt1_buf[2] := '-';
  txt1_buf[3] := ' ';
  if SHasFlag(AState,spt_awrite) then txt1_buf[4] := 'w' else txt1_buf[4] := '-';
  if SHasFlag(AState,spt_aread)  then txt1_buf[5] := 'r' else txt1_buf[5] := '-';
  txt1_buf[6] := #0;
  txt1.text := txt1_buf;
  SFloatToString(txt2_buf,FSampleRate);
  txt2.text := txt2_buf;
  SFloatToString(txt3_buf,FSamplePos);
  txt3.text := txt3_buf;
  SFloatToString(txt4_buf,FTempo);
  txt4.text := txt4_buf;
  SFloatToString(txt5_buf,FBeatPos);
  txt5.text := txt5_buf;
  //FEditor.redrawWidget(txt1);
  //FEditor.redrawWidget(txt2);
  //FEditor.redrawWidget(txt3);
  //FEditor.redrawWidget(txt4);
  //FEditor.redrawWidget(txt5);
  FEditor.redrawWidget(panel);
  *)
end;


{procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  v : Single;
begin
  case AIndex of
    0:
      begin

        v := AValue;
        FLimit := v*v*v;
        if v < 0.00001 then v := 0.00001;
        FGain := 1 / v;
      end;
  end; // case
end;}

//----------

{procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1 : single;
begin
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;
  AOutputs[0]^ := SClamp(spl0,-FLimit,FLimit) * FGain;
  AOutputs[1]^ := SClamp(spl1,-FLimit,FLimit) * FGain;
end;}

//----------------------------------------------------------------------

procedure myPlugin.on_openEditor;
var
  win : SWindow;
begin
  win := FEditor.window;
  panel := SWidgetPanel.create(FEditorRect,swa_fillClient);
  win.appendWidget(panel);

  txt1 := SWidgetText.create( rect(20),swa_fillTop);
  txt1.fillBack := True;
  txt1.text := txt1_buf;
  txt1.textAlign := sta_left;
  panel.appendWidget(txt1);

  txt2 := SWidgetText.create( rect(20),swa_fillTop);
  txt2.fillBack := True;
  txt2.text := txt2_buf;
  txt2.textAlign := sta_left;
  panel.appendWidget(txt2);

  txt3 := SWidgetText.create( rect(20),swa_fillTop);
  txt3.fillBack := True;
  txt3.text := txt3_buf;
  txt3.textAlign := sta_left;
  panel.appendWidget(txt3);

  txt4 := SWidgetText.create( rect(20),swa_fillTop);
  txt4.fillBack := True;
  txt4.text := txt4_buf;
  txt4.textAlign := sta_left;
  panel.appendWidget(txt4);

  txt5 := SWidgetText.create( rect(20),swa_fillTop);
  txt5.fillBack := True;
  txt5.text := txt5_buf;
  txt5.textAlign := sta_left;
  panel.appendWidget(txt5);

end;

//----------------------------------------------------------------------
end.

