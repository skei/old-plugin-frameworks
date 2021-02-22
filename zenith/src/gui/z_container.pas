unit z_container;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_gui_base,
  z_widget,
  z_array,
  z_rect;

type

  ZContainer = class(ZWidget)
  protected
    FSubWidgets   : ZWidgetArray;
    FAlignment    : LongWord;
    FContentRect  : ZRect;
  public
    constructor create(ARect:ZRect);
    destructor  destroy; override;
    procedure   appendWidget(AWidget:ZWidget; AInheritOwner:Boolean); virtual;
    procedure   deleteWidgets; virtual;
  public {ZWidgetBase}
    procedure   setOwner(AOwner:ZWindowBase); override;
  public {ZWidgetBase}
    {procedure   on_align; override;
    procedure   on_keyDown(AKey,AState:LongInt); override;
    procedure   on_keyUp(AKey,AState:LongInt); override;
    procedure   on_mouseDown(AXpos,AYpos,AButton,AState:LongInt); override;
    procedure   on_mouseMove(AXpos,AYpos,AState:LongInt); override;
    procedure   on_mouseUp(AXpos,AYpos,AButton,AState:LongInt); override;
    procedure   on_paint(*(ACanvas:ZCanvas; ARect:ZRect)*); override;
    procedure   on_timer; override;}
  end;

  ZContainerArray = specialize ZArray<ZContainer>;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZContainer.create(ARect:ZRect);
begin
  inherited;
  FSubWidgets := ZWidgetArray.create;
end;

//----------

destructor ZContainer.destroy;
begin
  {$ifndef Z_NOAUTODELETE}
  deleteWidgets;
  {$endif}
  FSubWidgets.destroy;
  inherited;
end;

//----------

procedure ZContainer.appendWidget(AWidget:ZWidget; AInheritOwner:Boolean);
begin
  if AInheritOwner then AWidget.setOwner(FOwner);
  AWidget.setParent(self);
  FSubWidgets.append(AWidget);
end;

//----------

procedure ZContainer.deleteWidgets;
var i:LongInt;
begin
  if FSubWidgets.size > 0 then
  begin
    for i := 0 to FSubWidgets.size-1 do
    begin
      FSubWidgets[i].on_destroy;
      FSubWidgets[i].destroy;
    end;
  end;
end;

//----------

procedure ZContainer.setOwner(AOwner:ZWindowBase);
var
  i : LongInt;
begin
  inherited; {widget}
  if FSubWidgets.size > 0 then
  begin
    for i := 0 to FSubWidgets.size-1 do FSubWidgets[i].setOwner(AOwner);
  end;
end;

//----------------------------------------------------------------------

{
procedure ZContainer.on_align;
begin
  //inherited;
end;

//----------

procedure ZContainer.on_keyDown(AKey,AState:LongInt);
begin
  //inherited;
end;

//----------

procedure ZContainer.on_keyUp(AKey,AState:LongInt);
begin
  //inherited;
end;

//----------

procedure ZContainer.on_mouseDown(AXpos,AYpos,AButton,AState:LongInt);
begin
  //inherited;
end;

//----------

procedure ZContainer.on_mouseMove(AXpos,AYpos,AState:LongInt);
begin
  //inherited;
end;

//----------

procedure ZContainer.on_mouseUp(AXpos,AYpos,AButton,AState:LongInt);
begin
  //inherited;
end;

//----------

procedure ZContainer.on_paint(*(ACanvas:ZCanvas; ARect:ZRect)*);
begin
  //inherited;
end;

//----------

procedure ZContainer.on_timer;
begin
  //inherited;
end;
}

//----------------------------------------------------------------------
end.

