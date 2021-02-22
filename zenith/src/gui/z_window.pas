unit z_window;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_LINUX} z_window_linux, {$endif}
  {$ifdef Z_WIN32} z_window_win32, {$endif}
  z_widget;

type
  ZWindow = class(ZWindowImpl)
  public
    procedure   do_cursor(AWidget:ZWidget; ACursor:LongInt); override;
    procedure   do_hint(AWidget:ZWidget; AHint:PChar); override;
    procedure   do_modal(AWidget:ZWidget; AMode:LongInt); override;
    procedure   do_move(AWidget:ZWidget; AXpos,AYpos:LongInt); override;
    procedure   do_redraw(AWidget:ZWidget; AState:LongInt=0); override;
    procedure   do_resize(AWidget:ZWidget; AWidth,AHeight:LongInt); override;
    procedure   do_update(AWidget:ZWidget); override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_rect;

//----------

procedure ZWindow.do_cursor(AWidget:ZWidget; ACursor:LongInt);
begin
  setCursor(ACursor);
end;

procedure ZWindow.do_hint(AWidget:ZWidget; AHint:PChar);
begin
  //writeln('hint: ',AHint);
end;

procedure ZWindow.do_modal(AWidget:ZWidget; AMode:LongInt);
begin
end;

procedure ZWindow.do_move(AWidget:ZWidget; AXpos,AYpos:LongInt);
begin
end;

procedure ZWindow.do_redraw(AWidget:ZWidget; AState:LongInt);
var
  r:ZRect;
begin
  r := AWidget.getRect;
  invalidate(r.x,r.y,r.w,r.h);
end;

procedure ZWindow.do_resize(AWidget:ZWidget; AWidth,AHeight:LongInt);
begin
end;

procedure ZWindow.do_update(AWidget:ZWidget);
begin
end;

//----------------------------------------------------------------------
end.

