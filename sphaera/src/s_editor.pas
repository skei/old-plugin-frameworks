{$include sphaera.inc}
unit s_editor;

{$define S_HOSTTOGUI_QUEUESIZE := 1024}
{$define S_MAXNUMPARAMS  := 1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_array,
  s_editor_base,
  s_eventqueue,
  s_parameter,
  s_rect,
  s_widget,
  s_widget_panel,
  s_window;

type

  SEditor = class;

  //----------

  SConnection = class
  public
    FParameter  : SParameter;
    FWidget     : SWidget;
    constructor create(AParameter:SParameter; AWidget:SWidget);
    destructor  destroy; override;
  end;

  SConnections = specialize SArray<SConnection>;

  //----------

  SEditorWindow = class(SWindow)
  private
    FEditor   : SEditor;
  public
    procedure on_timer; override;
    procedure do_update(AWidget: SWidget); override;
    property  editor : SEditor read FEditor write FEditor;
  end;

  //----------

  SEditor = class(SEditorBase)
  private
    p_back        : SWidgetPanel;
  protected
    FPlugin       : Pointer;
    FWindow       : SEditorWindow;
    FRect         : SRect;
    FIsOpen       : Boolean;
    FConnections  : SConnections;
    FHostEvents   : SEventQueue;
    FHostValues    : array[0..S_MAXNUMPARAMS-1] of Single;
  public
    constructor   create(APlugin:Pointer);
    destructor    destroy; override;
    function      getRect : SRect; override;
    function      getWindow : SWindow; override;
    function      isOpen : Boolean; override;
    procedure     open(ARect:SRect; AParent:Pointer); override;
    procedure     close; override;
    procedure     idle; override;
    procedure     connect(AParameter:SParameter; AWidget:SWidget); override;
    procedure     deleteConnections; override;
    procedure     setupDefaultWidgets; override;
    procedure     setupWidgetValues; override;
    procedure     checkParameter(AParam:SParameter; AValue:Single);
    procedure     redrawWidget(AWidget:SWidget); override;
    procedure     updateParameterFromWidget(AWidget:SWidget); override;
    procedure     updateParameterFromHost(AParameter:SParameter; AValue:Single); override;
    procedure     updateParametersInIdle; override;
    procedure     updateParametersInTimer; override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_color,
  s_const,
  //s_debug,
  s_memory,
  s_plugin,
  s_widget_slider;

//----------------------------------------------------------------------
// SConnection
//----------------------------------------------------------------------

constructor SConnection.create(AParameter:SParameter; AWidget:SWidget);
begin
  inherited create;
  FParameter := AParameter;
  FWidget := AWidget;
end;

//----------

destructor SConnection.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
// SEditorWindow
//----------------------------------------------------------------------

procedure SEditorWindow.on_timer;
begin
  //FEditor.updateParametersInTimer;
end;

//----------

procedure SEditorWindow.do_update(AWidget:SWidget);
begin
  FEditor.updateParameterFromWidget(AWidget);
end;

//----------------------------------------------------------------------
// SEditor
//----------------------------------------------------------------------

constructor SEditor.create(APlugin:Pointer);
//constructor SEditor.create(APlugin:SPluginBase);
begin
  inherited;
  FPlugin := APlugin;
  FWindow := nil;
  FRect   := rect(256);
  //FIsOpen := False;
  FConnections := SConnections.create;
  FHostEvents := SEventQueue.create(S_HOSTTOGUI_QUEUESIZE);
  p_back := nil;
  //p_slider := nil;
  SMemset(@FHostValues,0,sizeof(FHostValues));
end;

//----------

destructor SEditor.destroy;
begin
  {$ifndef SPH_NOAUTODELETE}
  deleteConnections;
  {$endif}
  FConnections.destroy;
  inherited
end;

//----------

function SEditor.getRect : SRect;
begin
  result := FRect;
end;

//----------

function SEditor.getWindow : SWindow;
begin
  result := FWindow;
end;

//----------

function SEditor.isOpen : Boolean;
begin
  result := FIsOpen;
end;

//----------

procedure SEditor.open(ARect:SRect; AParent:Pointer);
begin
  FRect   := ARect;
  FWindow := SEditorWindow.create(ARect,AParent);
  FWindow.editor := self;
  FWindow.show;
  FIsOpen := True;
  //FWindow.startTimer(40); // 25hz
end;

//----------

procedure SEditor.close;
begin
  //FWindow.stopTimer;
  FIsOpen := False;
  FWindow.hide;
  FWindow.destroy;
  FWindow := nil;
end;

//----------

procedure SEditor.idle;
begin
  updateParametersInIdle;
end;

//----------------------------------------------------------------------

procedure SEditor.connect(AParameter:SParameter; AWidget:SWidget{; AValue:Single});
var
  index : LongInt;
  value : Single;
begin
  index := FConnections.size;
  value := AParameter.getDefault;
  AParameter.connection := index;
  AWidget._connection := index;
  AWidget._param := AParameter;
  AWidget._value := value;//AParameter.getDefault;//AValue;//AParameter.getFValue;
  //SPlugin(FPlugin).setParamValue(index,value);
  FConnections.append( SConnection.create(AParameter,AWidget) );
end;

//----------

procedure SEditor.deleteConnections;
var
  i : LongInt;
  c : SConnection;
begin
  if FConnections.size > 0 then
  begin
    for i := 0 to FConnections.size-1 do
    begin
      c := FConnections[i];
      c.destroy;
    end;
  end;
end;

//----------------------------------------------------------------------

{
  create default widgets for parameters..
  called from on_openEditor if not overloaded
}

procedure SEditor.setupDefaultWidgets;
var
  win   : SWindow;
  back  : SWidgetPanel;
  wdg   : SWidget;
  i     : longint;
  plug  : SPlugin;
begin
  plug := SPlugin(FPlugin);
  win := FWindow;
  back := SWidgetPanel.create( rect(0), swa_fillClient);
  back.padding(5,5);
  back.margins(20,20);
  back.color := s_grey;
  win.appendWidget(back);
  if plug.getParameters.size > 0 then
  begin
    for i := 0 to plug.getParameters.size-1 do
    begin
      wdg := SWidgetSlider.create(rect(20),swa_fillTop);
      connect(plug.getParameters[i],wdg {paramvalues[i]});
      back.appendWidget(wdg);
    end;
  end;
end;

//----------

procedure SEditor.setupWidgetValues;//(AValues:array of Single);
var
  i     : longint;
  //plug  : SPlugin;
  wdg   : SWidget;
  par   : SParameter;
  val   : Single;
begin
  //plug := SPlugin(FPlugin);
  if FConnections.size > 0 then
  begin
    for i := 0 to FConnections.size-1 do
    begin
      par := FConnections[i].FParameter;
      wdg := FConnections[i].FWidget;
      val := SPlugin(FPlugin).getParamValue(par.index); // AValues[ par.index  ];
      //connect(plug.parameters[i],wdg);
      //back.appendWidget(wdg);
      wdg._value := val;
    end;
  end;
end;

//----------

procedure SEditor.checkParameter(AParam:SParameter; AValue:Single);
begin
end;

//----------

procedure SEditor.redrawWidget(AWidget:SWidget); // from plugin
begin
  if FIsOpen then FWindow.redraw(AWidget);
end;

//----------------------------------------------------------------------

{
  SEditorWindow.do_update
}

//procedure SEditor.updateParameterFromWidget(AIndex:LongInt; AValue:Single);
procedure SEditor.updateParameterFromWidget(AWidget:SWidget);
var
  conn : LongInt;
  param : SParameter;
  index : LongInt;
  value : Single;
  plugin : SPlugin;
begin
  //STrace('SEditor.updateParameterFromWidget');
  //SPlugin(FPlugin).updateParameterFromEditor(AIndex,AValue);
  conn := AWidget._connection;
  if conn >= 0 then
  begin
    param := FConnections[conn].FParameter;
    //if Assigned(par) then
    //begin
    index := param.getIndex;
    value := AWidget._value;
    plugin := SPlugin(FPlugin);
    //plug.setGuiValue(idx,val);
    plugin.updateParameterFromEditor(index,value);
  end;
end;

//----------

{
  vst_setparameter_callback
    SPlugin.vst_setParameter
      SPlugin.updateParameterFromHost
}

procedure SEditor.updateParameterFromHost(AParameter:SParameter; AValue:Single);
var
  index : LongInt;
  event : SEvent;
begin
  index := AParameter.connection;
  if index >= 0 then
  begin
    event.FIndex := index;
    event.FValue := AValue;
    FHostEvents.write(event);
  end;
  //index := AParameter.connection;
  //if index >= 0 then FHostValues[index] := AValue;
end;

//----------

{
  todo: don't redraw same widget several times..
  do_redraw sends invalidate for each redraw..
  rely on low-priority wm_paint combining them into one?
}

procedure SEditor.updateParametersInIdle;
var
  //param : SParameter;
  //index : LongInt;
  //value : Single;
  widget : SWidget;
  event : SEvent;
begin
  while FHostEvents.read(event{%H-}) do
  begin
    widget := FConnections[event.FIndex].FWidget;
    widget._value := event.FValue;
    FWindow.redraw(widget);
  end;

end;

//----------

procedure SEditor.updateParametersInTimer;
begin
  updateParametersInIdle;
end;

//----------------------------------------------------------------------
end.
