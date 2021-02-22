{$include sphaera.inc}
unit fx_dist3;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_const,
  s_types,
  s_plugin;

type

  myPlugin = class(SPlugin)
  private
    lo0,lo1,hi0,hi1,n0,n1 : Single;
    lo_mid : Single;
    mid_hi : Single;
    loboost : Single;
    midboost : Single;
    hiboost : Single;
    loclip : Single;
    midclip : Single;
    hiclip : Single;
    lopost : Single;
    midpost : Single;
    hipost : Single;
    filter : Single;
    vol : Single;
  public
    procedure   on_create; override;
    //procedure   on_transportChange(AState: LongInt); override;
    procedure   on_parameterChange(AIndex:LongInt; AValue:Single); override;
    procedure   on_processSample(AInputs,AOutputs:PPSingle); override;
    procedure   on_openEditor; override;
    //procedure   on_closeEditor; override;
    //procedure   on_idleEditor; override;
  end;

  SPHAERA_PLUGIN = myPlugin;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Math,
  s_debug,
  s_parameter,
  s_rect,
  s_util,
  s_widget,
  s_widget_knob,
  s_widget_panel,
  s_widget_slider,
  s_window;

//----------

procedure myPlugin.on_create;
begin
  FName := 'fx_dist3';
  FAuthor := 'th.skei';
  FVersion := 0;
  FUniqueId := S_MAGIC + $0101;
  SSetFlag(FFlags,spf_perSample+spf_hasEditor);
  FNumInputs  := 2;
  FNumOutputs := 2;
  FEditorRect := rect(430,170);
  //
  appendParameter( SParamFloat.create( 'low/mid',   0.20, 0, 1 ));
  appendParameter( SParamFloat.create( 'mid/hi',    0.80, 0, 1 ));

  appendParameter( SParamFloat.create( 'filter',    1,    0, 1 ));
  appendParameter( SParamFloat.create( 'vol',       1,    0, 1 ));

  appendParameter( SParamFloat.create( 'low boost', 1,    0, 2 ));
  appendParameter( SParamFloat.create( 'low clip',  1,    0, 1 ));
  appendParameter( SParamFloat.create( 'low post',  1,    0, 2 ));

  appendParameter( SParamFloat.create( 'mid boost', 1,    0, 2 ));
  appendParameter( SParamFloat.create( 'mid clip',  1,    0, 1 ));
  appendParameter( SParamFloat.create( 'mid post',  1,    0, 2 ));

  appendParameter( SParamFloat.create( 'high boost',1,    0, 2 ));
  appendParameter( SParamFloat.create( 'high clip', 1,    0, 1 ));
  appendParameter( SParamFloat.create( 'high post', 1,    0, 2 ));

  //
  lo0 := 0;
  lo1 := 0;
  hi0 := 0;
  hi1 := 0;
  n0  := 0;
  n1  := 0;
end;

//----------

(*procedure myPlugin.on_transportChange(AState: LongInt);
var
  d : double;
begin
  updateHostInfo;
  {$ifdef SPH_DEBUG}
    console.writeln('FHostCanDo: ',   FHostCanDo);
    console.writeln('FHostVersion: ', FHostVersion);
    console.writeln('FHostId: ',      FHostId);
    console.writeln('FHostVstVers: ', FHostVstVers);
    {$ifdef SPH_REAPER}
      console.writelns( 'reaper.getPlayPosition: ', reaper.getPlayPosition() );
      console.writeln('play state: ', reaper.getPlayState() );
    {$endif}
  {$endif}
end;*)

procedure myPlugin.on_parameterChange(AIndex:LongInt; AValue:Single);
var
  a2,a3:single;
begin
  STrace('p ',AIndex);
  a2 := AValue*AValue;
  a3 := AValue*AValue*AValue;
  case AIndex of
    0:  lo_mid   := a3;
    1:  mid_hi   := a3;
    2: filter    := a3;
    3: vol       := a3;

    4:  loboost  := a2;
    5:  loclip   := a3;
    6:  lopost   := a2;

    7:  midboost := a2;
    8:  midclip  := a3;
    9:  midpost  := a2;

    10:  hiboost  := a2;
    11:  hiclip   := a3;
    12: hipost    := a2;
  end;
end;

//----------

procedure myPlugin.on_processSample(AInputs,AOutputs:PPSingle);
var
  spl0,spl1       : single;
  low0,mid0,high0 : single;
  low1,mid1,high1 : single;
begin
  //updateSync;
  spl0 := AInputs[0]^;
  spl1 := AInputs[1]^;

  lo0   += ((spl0-lo0) * lo_mid);
  hi0   += ((spl0-hi0) * mid_hi);
  low0  := loboost * lo0;
  mid0  := midboost * (hi0-lo0);
  high0 := hiboost * (spl0-hi0);
  low0  := lopost * max(-loclip,min(loclip,low0));
  mid0  := midpost * max(-midclip,min(midclip,mid0));
  high0 := hipost * max(-hiclip,min(hiclip,high0));
  spl0  := (low0+mid0+high0);
  n0    += ((spl0-n0)*filter);
  spl0  := (n0)*vol;

  lo1   += ((spl1-lo1) * lo_mid);
  hi1   += ((spl1-hi1) * mid_hi);
  low1  := loboost * lo1;
  mid1  := midboost * (hi1-lo1);
  high1 := hiboost * (spl1-hi1);
  low1  := lopost * max(-loclip,min(loclip,low1));
  mid1  := midpost * max(-midclip,min(midclip,mid1));
  high1 := hipost * max(-hiclip,min(hiclip,high1));
  spl1  := (low1+mid1+high1);
  n1    += ((spl1-n1)*filter);
  spl1  := (n1)*vol;

  AOutputs[0]^ := spl0;
  AOutputs[1]^ := spl1;
end;

//----------------------------------------------------------------------

procedure myPlugin.on_openEditor;
var
  win : SWindow;
  wdg : SWidget;
  back : SWidget;
begin
  win := _editor.getWindow;

  {}

  back := SWidgetPanel.create( rect(0), swa_fillClient );
  win.appendWidget(back);

  {}

  wdg := SWidgetKnob.create( rect(10,10, 200,35), swa_none );
  _editor.connect( FParameters[3], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(220,10, 200,35), swa_none );
  _editor.connect( FParameters[2], wdg );
  back.appendWidget(wdg);

  //

  wdg := SWidgetSlider.create( rect(10,50, 200,20), swa_none );
  _editor.connect( FParameters[0], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetSlider.create( rect(220,50, 200,20), swa_none );
  _editor.connect( FParameters[1], wdg );
  back.appendWidget(wdg);

  {}

  wdg := SWidgetKnob.create( rect(10,75, 130,25), swa_none );
  _editor.connect( FParameters[4], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(10,105, 130,25), swa_none );
  _editor.connect( FParameters[5], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(10,135, 130,25), swa_none );
  _editor.connect( FParameters[6], wdg );
  back.appendWidget(wdg);


  wdg := SWidgetKnob.create( rect(150,75, 130,25), swa_none );
  _editor.connect( FParameters[7], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(150,105, 130,25), swa_none );
  _editor.connect( FParameters[8], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(150,135, 130,25), swa_none );
  _editor.connect( FParameters[9], wdg );
  back.appendWidget(wdg);


  wdg := SWidgetKnob.create( rect(290,75, 130,25), swa_none );
  _editor.connect( FParameters[10], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(290,105, 130,25), swa_none );
  _editor.connect( FParameters[11], wdg );
  back.appendWidget(wdg);

  wdg := SWidgetKnob.create( rect(290,135, 130,25), swa_none );
  _editor.connect( FParameters[12], wdg );
  back.appendWidget(wdg);

end;

//----------

{procedure myPlugin.on_closeEditor;
begin
end;}

//----------

{procedure myPlugin.on_idleEditor;
begin
end;}

//----------------------------------------------------------------------
end.

