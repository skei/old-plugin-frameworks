{$include sphaera.inc}
unit s_editor_base;

{$define S_HOSTTOGUI_QUEUESIZE := 1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  //s_array,
  //s_editor_base,
  //s_eventqueue,
  s_parameter,
  s_rect,
  s_widget,
  //s_widget_panel,
  s_window;

type

  SEditorBase = class
  public
    constructor   create({%H-}APlugin:Pointer);
    destructor    destroy; override;
    function      getRect : SRect; virtual;
    function      getWindow : SWindow; virtual;
    function      isOpen : Boolean; virtual;
    procedure     open({%H-}ARect:SRect; {%H-}AParent:Pointer); virtual;
    procedure     close; virtual;
    procedure     idle; virtual;
    procedure     connect({%H-}AParameter:SParameter; {%H-}AWidget:SWidget); virtual;
    procedure     deleteConnections; virtual;
    procedure     setupDefaultWidgets; virtual;
    procedure     setupWidgetValues; virtual;
    procedure     redrawWidget({%H-}AWidget:SWidget); virtual;
    procedure     updateParameterFromWidget({%H-}AWidget:SWidget); virtual;
    procedure     updateParameterFromHost({%H-}AParameter:SParameter; {%H-}AValue:Single); virtual;
    procedure     updateParametersInIdle; virtual;
    procedure     updateParametersInTimer; virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
  //s_color,
  //s_const,
  //s_plugin,
  //s_widget_slider;

//----------

constructor SEditorBase.create(APlugin:Pointer);
begin
  inherited create;
end;

destructor SEditorBase.destroy;
begin
  inherited
end;

//----------

function SEditorBase.getRect : SRect;
begin
  result := rect(0);
end;

function SEditorBase.getWindow : SWindow;
begin
  result := nil;
end;

function SEditorBase.isOpen : Boolean;
begin
  result := false;
end;

//----------

procedure SEditorBase.open(ARect:SRect; AParent:Pointer);
begin
end;

procedure SEditorBase.close;
begin
end;

procedure SEditorBase.idle;
begin
end;

//----------

procedure SEditorBase.connect(AParameter:SParameter; AWidget:SWidget);
begin
end;

procedure SEditorBase.deleteConnections;
begin
end;

//----------

procedure SEditorBase.setupDefaultWidgets;
begin
end;

procedure SEditorBase.setupWidgetValues;
begin
end;

procedure SEditorBase.redrawWidget(AWidget:SWidget);
begin
end;

//----------

procedure SEditorBase.updateParameterFromWidget(AWidget:SWidget);
begin
end;

procedure SEditorBase.updateParameterFromHost(AParameter:SParameter; AValue:Single);
begin
end;

procedure SEditorBase.updateParametersInIdle;
begin
end;

procedure SEditorBase.updateParametersInTimer;
begin
end;

//----------------------------------------------------------------------
end.
