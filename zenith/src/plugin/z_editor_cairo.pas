unit z_editor_cairo;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base,
  z_widget,
  z_window,
  z_rect;

type

  ZEditorWindow = class(ZWindow)
  private
    FEditor : ZEditorBase;
  public
    constructor create(AEditor:ZEditorBase; ARect:ZRect; AParent:Pointer);
    procedure   do_hint(AWidget: ZWidget; AHint: PChar); override;
    procedure   do_update(AWidget: ZWidget); override;
  end;

  //----------

  ZEditorCairo = class(ZEditorBase)
  protected
    FWindow     : ZEditorWindow;
  public
    constructor create({%H-}AInstance:ZInstanceBase);
    destructor  destroy; override;
  public
    function    getRect: ZRect; override;
    function    isOpen: boolean; override;
    procedure   open({%H-}AParent: Pointer); override;
    procedure   close; override;
    procedure   idle; override;
    procedure   eventLoop; override;
    procedure   updateParameterFromHost({%H-}AIndex: longint; {%H-}AValue: single); override;
    procedure   updateParameterFromWidget({%H-}AIndex: longint; {%H-}AValue: single); override;
  end;

  ZEditorImpl = ZEditorCairo;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_parameter;

constructor ZEditorWindow.create(AEditor:ZEditorBase; ARect:ZRect; AParent:Pointer);
begin
  inherited create(ARect,AParent);
  FEditor := AEditor;
end;

procedure ZEditorWindow.do_hint(AWidget: ZWidget; AHint: PChar);
begin
  //FEditor.handleHint(AWidget,AHint);
end;

procedure ZEditorWindow.do_update(AWidget: ZWidget);
//var p : ZParameter;
begin
  //FEditor.handleUpdate(AWidget,AHint);
  //p := AWidget.getParameter;
  //FEditor.updateParameterFromWidget(index,value);
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

constructor ZEditorCairo.Create(AInstance:ZInstanceBase);
begin
  FInstance := AInstance;
  FIsOpen := False;
  FRect := rect(256, 256);
  FWindow := nil;
  //on_create;
end;

//----------

destructor ZEditorCairo.Destroy;
begin
  //on_destroy;
  //if Assigned(FWindow) then FWindow.destroy;
end;

//----------

//function ZEditor.getWindow: ZWindow;
//begin
//  Result := FWindow;
//end;

//----------------------------------------------------------------------

procedure ZEditorCairo.Open(AParent: Pointer);
begin
  FWindow := ZEditorWindow.create(self,FRect,AParent);
  FWindow.show;
  FIsOpen := true;
  on_open;
end;

//----------

procedure ZEditorCairo.Close;
begin
  on_close;
  FIsOpen := false;
  FWindow.hide;
  FWindow.destroy;
  FWindow := nil;
end;

//----------

procedure ZEditorCairo.idle;
begin
  // update widgets from queue
  on_idle;
end;

//----------

procedure ZEditorCairo.eventLoop;
begin
  FWindow.eventLoop;
end;

//----------

function ZEditorCairo.getRect: ZRect;
begin
  Result := FRect;
end;

//----------

function ZEditorCairo.isOpen: boolean;
begin
  Result := FIsOpen;
end;

//----------

procedure ZEditorCairo.updateParameterFromHost(AIndex: longint; AValue: single);
begin
end;

//----------

procedure ZEditorCairo.updateParameterFromWidget(AIndex: longint; AValue: single);
begin
end;

//----------------------------------------------------------------------
end.

