unit z_widget_value;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  //z_color,
  z_rect,
  z_widget;

type

  ZWidgetValue = class(ZWidget)
  private
    FDragging   : boolean;
    FClickVal   : Single;
    FClickX     : LongInt;
    FClickY     : LongInt;
  protected
    FDragSens   : Single;
  public
    constructor create(ARect:ZRect; AAlignment:Longword; APainter:ZWidgetPainter);
    destructor  destroy; override;
  public
    procedure   on_MouseDown(AXpos,AYpos:LongInt; AButton,AState:LongWord); override;
    procedure   on_MouseMove(AXpos,AYpos:LongInt; AState:LongWord); override;
    procedure   on_MouseUp(AXpos,AYpos:LongInt; AButton,AState:LongWord); override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  //z_util,
//  z_painters;

constructor ZWidgetValue.create(ARect:ZRect; AAlignment:Longword; APainter:ZWidgetPainter);
begin
  inherited; // create({AParent,}ARect);
  FText     := 'ZWdgValue';
  FDragging := false;
  FDragSens := 0.002;
  //FPainter  := nil;//ZValuePainter.create;
end;

destructor ZWidgetValue.destroy;
begin
  //FPainter.destroy;
  inherited;
end;

procedure ZWidgetValue.on_MouseDown(AXpos,AYpos:LongInt; AButton,AState:LongWord);
begin
  inherited;
  //writeln('ZWdgValue click');
  if not FDragging then
  begin
    FDragging := true;
    FClickVal := FValue;
    FClickX := AXpos;
    FClickY := AYpos;
  end;
end;

procedure ZWidgetValue.on_MouseMove(AXpos,AYpos:LongInt; AState:LongWord);
var
  dist : Single;
begin
  inherited;
  if FDragging then
  begin
    //writeln('drag');
    dist := (FClickY - AYPos);  // drag up to increats (neg y)
    FValue := FClickVal + (dist*FDragSens);
    if FValue > 1 then FValue := 1.0;
    if FValue < 0 then FValue := 0.0;
    //writeln('dist:',dist,' FValue:',FValue);
    // if value <> prev value then ..
    do_update(self);
    do_redraw(self);
  end;
end;

procedure ZWidgetValue.on_MouseUp(AXpos,AYpos:LongInt; AButton,AState:LongWord);
begin
  inherited;
  FDragging := false;
end;

//----------------------------------------------------------------------
end.


