{$include sphaera.inc}
unit s_widget_button;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_canvas,
  s_color,
  s_rect,
  s_widget;

type

  SWidgetButton = class(SWidget)
  protected
    FMode       : LongInt;
    FOnColor    : SColor;
    FOffColor   : SColor;
    FOnText     : PChar;
    FOffText    : PChar;
    //FXRatio     : Single;
    //FYRatio     : Single;
  public
    constructor create(ARect:SRect; AAlignment:Longword);
    destructor  destroy; override;
    procedure   on_MouseDown({%H-}AXpos,{%H-}AYpos:LongInt; AButton,{%H-}AState:LongInt); override;
    procedure   on_mouseUp({%H-}AXpos, {%H-}AYpos, AButton, {%H-}AState: LongInt); override;
    procedure   on_paint(ACanvas:SCanvas; {%H-}ARect:SRect); override;
    property    mode     : LongInt read FMode write FMode;
    property    onColor  : SColor read FOnColor write FOnColor;
    property    offColor : SColor read FOffColor write FOffColor;
    property    onText   : PChar read FOnText write FOnText;
    property    offText  : PChar read FOffText write FOffText;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_util;
//  s_window;

//----------

constructor SWidgetButton.create(ARect:SRect; AAlignment:Longword);
begin
  inherited;
  FText     := 'SWidgetButton';
  FMode     := scm_spring;
  FCursor   := smc_Finger;
  FOnColor  := s_light_green;
  FOffColor := s_dark_green;
  FOnText   := 'on';
  FOffText  := 'off';
end;

//----------

destructor SWidgetButton.destroy;
begin
  inherited;
end;

//----------

procedure SWidgetButton.on_MouseDown(AXpos,AYpos:LongInt; AButton,AState:LongInt);
begin
  if SHasFlag(AButton,smb_left) then
  begin
    //FXRatio := (AXpos - FRect.x) / FRect.w;
    //FYRatio := (AYpos - FRect.y) / FRect.h;
    case FMode of
      scm_toggle:
        begin
          if FValue > 0.5 then FValue := 0
          else FValue := 1;
        end;
      scm_spring:
        begin
          FValue := 1;
        end;
    end; // case
    do_update(self);
    do_redraw(self);
  end;
end;

//----------

procedure SWidgetButton.on_mouseUp(AXpos, AYpos, AButton, AState: LongInt);
begin
  if SHasFlag(AButton,smb_left) then
  begin
    if FMode = scm_spring then
    begin
      FValue := 0;
      do_update(self);
      do_redraw(self);
    end;
  end;
end;

//----------

procedure SWidgetButton.on_paint(ACanvas:SCanvas; ARect:SRect);
var
  r:SRect;
  v:single;
  t:Pchar;
begin
  r := FRect;
  v := FValue;
  if v > 0.5 then ACanvas.setColor(FOnColor) else ACanvas.setColor(FOffColor);
  ACanvas.rectangle(r.x,r.y,r.w,r.h);
  ACanvas.fill;
  ACanvas.setColor(s_black);
  //ACanvas.selectFontFace('Arial',sfs_Normal,sfw_Normal);
  ACanvas.setFontSize(12);
  if v > 0.5 then t := FOnText else t := FOffText;
  ACanvas.drawTextBox(r.x,r.y,r.w,r.h,t,sta_center);
  ACanvas.setLineWidth(1);
  ACanvas.setColor(s_black);
  ACanvas.drawFrame(r.x,r.y,r.x2,r.y2);
  //inherited; // paint sub-widgets
end;

//----------------------------------------------------------------------
end.

