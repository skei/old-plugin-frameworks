{$include sphaera.inc}
unit s_widget_value;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  s_color,
  s_rect,
  s_widget,
  s_window;

type

  SWidgetValue = class(SWidget)
  protected
    FDragging   : boolean;
    FClickVal   : Single;
    FClickX     : LongInt;
    FClickY     : LongInt;
    FDragSens   : Single;
    FDragDir    : LongInt;
    FDrawDir    : LongInt;
    FDrawBack   : Boolean;
    FBackColor  : SColor;
    FTextColor  : SColor;
    FTextAlign  : LongInt;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
  public
    procedure   on_MouseDown(AXpos,AYpos:LongInt; AButton,AState:LongInt); override;
    procedure   on_MouseMove(AXpos,AYpos,AState:LongInt); override;
    procedure   on_mouseUp(AXpos, AYpos, AButton, AState: LongInt); override;
    procedure   on_enter(AWidget: SWidget); override;
    //procedure on_leave(AWidget: SWidget); override;
    procedure   on_paint(ACanvas:SCanvas; ARect:SRect); override;
  public
    property    sens      : Single read FDragSens write FDragSens;
    property    dragDir   : LongInt read FDragDir write FDragDir;
    property    drawDir   : LongInt read FDrawDir write FDrawDir;
    property    drawBack  : Boolean read FDrawBack write FDrawBack;
    property    backColor : SColor read FBackColor write FBackColor;
    property    textColor : SColor read FTextColor write FTextColor;
    property    textAlign : LongInt read FTextAlign write FTextAlign;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_parameter,
  s_util;

//----------

constructor SWidgetValue.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText       := 'SWidgetValue';
  FDragging   := false;
  FDragSens   := 0.002;
  FDragDir    := sdd_up;
  FCursor     := smc_default;
  FDrawBack   := True;
  FBackColor  := s_dark_grey;
  FTextColor  := s_light_grey;
  FTextAlign  := sta_center;
end;

//----------

destructor SWidgetValue.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetValue.on_MouseDown(AXpos,AYpos:LongInt; AButton,AState:LongInt);
begin
  if SHasFlag(AButton,smb_left) then
  begin
    if not FDragging then
    begin
      FDragging := true;
      FClickVal := FValue;
      FClickX   := AXpos;
      FClickY   := AYpos;
    end;
  end;
end;

//----------

procedure SWidgetValue.on_MouseMove(AXpos,AYpos,AState:LongInt);
var
  dist : Single;
begin
  if FDragging then
  begin
    dist := 0;
    case FDragDir of
      sdd_up:     dist := (FClickY-AYpos);
      sdd_down:   dist := (AYpos-FClickY);
      sdd_left:   dist := (FClickX-AXPos);
      sdd_right:  dist := (AXPos-FClickX);
    end;
    FValue := FClickVal + (dist*FDragSens);
    if FValue > 1 then FValue := 1.0;
    if FValue < 0 then FValue := 0.0;
    // if value <> prev value then ..
    do_update(self);
    do_redraw(self);
  end;
end;

//----------

procedure SWidgetValue.on_mouseUp(AXpos, AYpos, AButton, AState: LongInt);
begin
  inherited;
  if SHasFlag(AButton,smb_left) then
  begin
    FDragging := false;
  end;
end;

//----------

procedure SWidgetValue.on_enter(AWidget: SWidget);
var
  c : longint;
begin
  c := smc_default;
  case FDragDir of
    sdd_left:   c := smc_ArrowLeftRight;
    sdd_right:  c := smc_ArrowLeftRight;
    sdd_up:     c := smc_ArrowUpDown;
    sdd_down:   c := smc_ArrowUpDown;
  end;
  do_cursor(self,c);
end;

//----------

{procedure SWidgetDrag.on_leave(AWidget: SWidget);
begin
  do_cursor(self,smc_default);
end;}

//----------

procedure SWidgetValue.on_Paint(ACanvas:SCanvas; ARect:SRect);
var
  r : SRect;
  s : PChar;
  ss : array[0..255] of char; // 2..255 ??
  //width : single;
  v : single;
  p : SParameter;
begin
  r := FRect;
  v := FValue;
  p := FParameter;
  { background }
  if FDrawBack then
  begin
    ACanvas.setColor(FBackColor);
    ACanvas.rectangle(r.x,r.y,r.w,r.h);
    ACanvas.fill;
  end;
  { value/text }
  ACanvas.setColor(FTextColor);
  //ACanvas.selectFontFace('Arial',sfs_Normal,sfw_Bold);
  ACanvas.setFontSize(12);
  { value }
  if Assigned(p) then s := p.getDisplay(v)
  else
  begin
    SFloatToString(@ss,v);
    s := pchar(ss);
  end;
  ACanvas.drawTextBox(r.x+3,r.y,r.w-6,r.h,s,FTextAlign);
  //inherited; // paint sub-widgets
end;


//----------------------------------------------------------------------
end.

