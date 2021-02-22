{$include sphaera.inc}
unit s_window;

{
  TODO: split into base/impl/window, ala canvas
}


{.$define S_NOSHRINKBUFFER}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Windows,
  s_canvas,
  s_rect,
  s_surface,
  s_widget;


var
  win32_cursors : array[0..19] of PChar =
  (
    // Hint: Conversion between ordinals and pointers is not portable
    PChar( IDC_ARROW ),       // zcu_default
    PChar( IDC_ARROW ),       // zcu_Arrow
    PChar( IDC_UPARROW ),     // zcu_ArrowUp
    PChar( IDC_ARROW ),       // zcu_ArrowDown
    PChar( IDC_ARROW ),       // zcu_ArrowLeft
    PChar( IDC_ARROW ),       // zcu_ArrowRight
    PChar( IDC_SIZENS ),      // zcu_ArrowUpDown
    PChar( IDC_SIZEWE ),      // zcu_ArrowLeftRight
    PChar( IDC_SIZENESW ),    // zcu_ArrowDiagLeft
    PChar( IDC_SIZENWSE ),    // zcu_ArrowDiagRight
    PChar( IDC_SIZEALL ),     // zcu_Move
    PChar( IDC_WAIT ),        // zcu_Wait
    PChar( IDC_APPSTARTING ), // zcu_ArrowWait
    PChar( IDC_HAND),         // zcu_Hand
    PChar( IDC_HAND),         // zcu_Finger
    PChar( IDC_CROSS),        // zcu_Cross
    PChar( IDC_ARROW ),       // zcu_Pencil
    PChar( IDC_ARROW ),       // zcu_Plus
    PChar( IDC_ARROW ),       // zcu_Question
    PChar( IDC_ARROW )        // zcu_Ibeam
  );

type

  SWin32Window = class(SWidget)
  private
    FInstance     : HINST;
    FWindow       : HWND;
    FWinCursor    : HCURSOR;
    FPaintStruct  : TPAINTSTRUCT;
    FPaintDC      : HDC;
    FCairoWidth   : LongInt;
    FCairoHeight  : LongInt;
    FEmbedded     : Boolean;
    FPrevCursor   : LongInt;//HCURSOR;
  protected
    FCanvas       : SCanvas;
    FSurface      : SSurface;
    FTitle        : PChar;
  public
    property    _embedded : Boolean read FEmbedded;
    property    _instance : HINST read FInstance;
    property    _window : HWND read FWindow;
    //procedure   win32_eventHandler;
  public
    constructor create(ARect:SRect; AParent:Pointer=nil);
    destructor  destroy; override;
    function    win32_eventHandler(hWnd:HWND; message:UINT; wParam:WPARAM; lParam:LPARAM) : LRESULT;
    function    remapKey(AState:LongInt) : LongInt;
    function    remapButton(AButton:LongInt) : LongInt;
    procedure   redraw(ARect:SRect);
    procedure   redraw;
    procedure   redraw(AWidget:SWidget);
    procedure   resizeBuffer(AWidth,AHeight:LongInt);
    procedure   blitBuffer(ARect:SRect; ADC:HDC);
    procedure   paint(ARect:SRect);
  public
    procedure   beginPaint;
    procedure   endPaint;
    procedure   eventLoop;
    procedure   flush;
    procedure   grabCursor;
    procedure   hide;
    procedure   hideCursor;
    procedure   invalidate(AXpos,AYpos,AWidth,AHeight:LongInt);
    procedure   lock;
    procedure   releaseCursor;
    procedure   reparent(AParent:Pointer);
    procedure   resetCursor;
    procedure   sendEvent(AValue:longword);
    procedure   setCursor(ACursor:LongInt);
    procedure   setCursorPos({%H-}AXpos,{%H-}AYpos:LongInt);
    procedure   setPos(AXpos,AYpos:LongInt);
    procedure   setSize(AWidth,AHeight:LongInt);
    procedure   setTitle({%H-}ATitle:PChar);
    procedure   show;
    procedure   showCursor;
    procedure   startTimer(ms:LongInt);
    procedure   stopTimer;
    procedure   sync;
    procedure   unlock;
  public
    procedure   do_cursor({%H-}AWidget:SWidget; ACursor:LongInt); override;
    procedure   do_hint({%H-}AWidget:SWidget; {%H-}AHint:PChar); override;
    procedure   do_modal({%H-}AWidget:SWidget; {%H-}AMode:LongInt); override;
    procedure   do_move({%H-}AWidget:SWidget; {%H-}AXpos,{%H-}AYpos:LongInt); override;
    procedure   do_redraw(AWidget:SWidget; {%H-}AState:LongInt=0); override;
    procedure   do_resize({%H-}AWidget:SWidget; {%H-}AWidth,{%H-}AHeight:LongInt); override;
    procedure   do_update({%H-}AWidget:SWidget); override;
  end;

  //----------

  SWindow = SWin32Window;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  //SysUtils, // sleep
  s_const,
  s_memory,
  s_string,
  s_util;

//----------

var
  SWin32WindowClassRegistered  : Boolean = false;
  SWin32WindowClassName        : array[0..255] of char;
  {%H-}SWin32WindowClassAtom   : ATOM;

//----------------------------------------------------------------------
// internal
//----------------------------------------------------------------------

// LRESULT CALLBACK win32_eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

function win32_eventProc(hWnd:HWND; message:UINT; wParam:WPARAM; lParam:LPARAM) : LRESULT; stdcall;
var win : SWin32Window;
begin
  win := SWin32Window( Windows.GetWindowLongPtr(hWnd,GWL_USERDATA) );
  if Assigned(win) then result := win.win32_eventHandler(hWnd,message,wParam,lParam)
  else result := Windows.DefWindowProc(hWnd,message,wParam,lParam);
end;

//----------

procedure win32_registerWindow;
var
  buf : array[0..32] of char;
  wc : WNDCLASS;
begin
  if not SWin32WindowClassRegistered then
  begin
    //SWin32WindowInstance := MainInstance;
    SStrcpy(SWin32WindowClassName,'zenith_window_');
    SPtrToString(buf,nil); // ???
    SStrcat(SWin32WindowClassName,buf);
    SMemset(@wc,0,sizeof(wc));
    wc.style         := (CS_HREDRAW or CS_VREDRAW);
    wc.lpfnWndProc   := @win32_eventProc;
    wc.hInstance     := MainInstance;
    wc.lpszClassName := @SWin32WindowClassName;
    //wc.hCursor       := LoadCursor(0,IDC_CROSS);
    //wc.hIcon         := LoadIcon(global_instance,IDI_APPLICATION);
    SWin32WindowClassAtom := Windows.RegisterClass(@wc);
    SWin32WindowClassRegistered := true;
  end;
end;

//----------

procedure win32_unregisterWindow;
begin
  if SWin32WindowClassRegistered then UnregisterClass(SWin32WindowClassName,MainInstance);
end;

//----------------------------------------------------------------------
// SWin32Window
//----------------------------------------------------------------------

constructor SWin32Window.create(ARect:SRect; AParent:Pointer);
var
  rc    : Windows.RECT;
  wx,wy : LongWord;
begin
  inherited create(ARect);
  //if not SWin32WindowClassRegistered then win32_registerWindow;
  FTitle := 'SWin32Window';
  FInstance   := MainInstance;
  //FWinCursor     := Windows.LoadCursor(0,IDC_ARROW); // crash??
  FPrevCursor := -1;
  setCursor(smc_default);
  rc.Left   := FRect.x;
  rc.Top    := FRect.y;
  rc.Right  := FRect.x2;
  rc.Bottom := FRect.y2;
  if Assigned(AParent) then
  begin
    FEmbedded := true;
    Windows.AdjustWindowRectEx(@rc,{WS_OVERLAPPEDWINDOW|} WS_POPUP, FALSE, WS_EX_TOOLWINDOW );
    FWindow := Windows.CreateWindowEx(
      WS_EX_TOOLWINDOW,
      SWin32WindowClassName,
      nil,
      WS_POPUP,
      rc.left,
      rc.top,
      rc.right-rc.left+1,
      rc.bottom-rc.top+1,
      0,//HWND(AParent),//0,
      0,
      FInstance,
      nil
    );
    reparent(AParent);
  end // embedded
  else
  begin
    FEmbedded := false;
    Windows.AdjustWindowRectEx(@rc,WS_OVERLAPPEDWINDOW,FALSE,WS_EX_OVERLAPPEDWINDOW);
    wx := ((Windows.GetSystemMetrics(SM_CXSCREEN)-ARect.w) shr 1) + rc.left;
    wy := ((Windows.GetSystemMetrics(SM_CYSCREEN)-ARect.h) shr 1) + rc.top;
    FWindow := Windows.CreateWindowEx(
      WS_EX_OVERLAPPEDWINDOW, // dwExStyle
      SWin32WindowClassName,  // lpClassName
      nil,                    // lpWindowName
      WS_OVERLAPPEDWINDOW,    // dwStyle
      wx,                     // center x
      wy,                     // center y
      rc.right -rc.left+1,    // wWidth,
      rc.bottom-rc.top +1,    // wHeight,
      0,                      // hWndParent
      0,                      // hMenu
      FInstance,              // hInstance
      nil                     // lpParam
    );
    Windows.SetFocus(FWindow);
  end; // windowed
  Windows.SetWindowLongPtr(FWindow,GWL_USERDATA, LONG_PTR(self));
  //UpdateWindow(self.hwnd)

  FSurface := SSurface.create(ARect.w,ARect.h,32); // dib
  FCanvas := SCanvas.create(FSurface);
  FCairoWidth := ARect.w;
  FCairoHeight := ARect.h;
end;

//----------

destructor  SWin32Window.destroy;
begin
  Windows.DestroyWindow(FWindow);
  FCanvas.destroy;
  FSurface.destroy;
  inherited;
end;

//----------------------------------------------------------------------

function SWin32Window.remapButton(AButton:LongInt) : LongInt;
begin
  result := AButton;
end;

//----------

function SWin32Window.remapKey(AState:LongInt) : LongInt;
begin
  result := smb_none;
  if (AState and MK_SHIFT)   <> 0     then result += smb_shift;
  if (AState and MK_CONTROL) <> 0     then result += smb_ctrl;
  if Windows.GetKeyState(VK_MENU) < 0 then result += smb_alt;
end;

//----------

procedure SWin32Window.redraw(ARect:SRect);
begin
  //FCanvas.resetClip;
  //on_paint(FCanvas,ARect);
  //invalidate( ARect.x, ARect.y, ARect.w, ARect.h );
  paint(ARect);
end;

//----------

procedure SWin32Window.redraw;
begin
  //on_paint(FCanvas,FRect);
  //invalidate( FRect.x, FRect.y, FRect.w, FRect.h );
  redraw(FRect);
end;

//----------

procedure SWin32Window.redraw(AWidget:SWidget);
begin
  redraw(AWidget._rect);
end;

//----------

procedure SWin32Window.resizeBuffer(AWidth,AHeight:LongInt);
var
  w,h:longint;
begin
  w := FCairoWidth;
  h := FCairoHeight;
  while AWidth  > w do w := w * 2;
  while AHeight > h do h := h * 2;
  {$ifndef SPH_NOSHRINKBUFFER}
  while AWidth  < (w div 2) do w := w div 2;
  while AHeight < (h div 2) do h := h div 2;
  {$endif}
  if (w <> FCairoWidth) or (h <> FCairoHeight) then
  begin
    FCairoWidth := w;
    FCairoHeight := h;
    if Assigned(FCanvas) then FCanvas.destroy;
    if Assigned(FSurface) then FSurface.destroy;
    FSurface := SSurface.create(FCairoWidth,FCairoHeight,32); // dib
    FCanvas := SCanvas.create(FSurface);
    //redraw;
  end;
end;

//----------

procedure SWin32Window.blitBuffer(ARect:SRect; ADC:HDC);
//var
//  src{,dst} : HDC;
begin
  //dst := getDC(FWindow); //FPaintStruct.hdc;
  //dst := FPaintStruct.hdc;
  //src := FSurface.getDC;
  BitBlt(ADC,ARect.x,ARect.y,ARect.w,ARect.h,{src}FSurface.getInternal,ARect.x,ARect.y,SRCCOPY);
  //ReleaseDC(FWindow,dst);
end;

//----------

procedure SWin32Window.paint(ARect:SRect);
begin
  FCanvas.resetClip;
  on_paint(FCanvas,ARect);
  invalidate( ARect.x, ARect.y, ARect.w, ARect.h );
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

procedure SWin32Window.beginPaint;
begin
  FPaintDC := Windows.BeginPaint(FWindow,@FPaintStruct);
end;

//----------

procedure SWin32Window.endPaint;
begin
  Windows.EndPaint(FWindow,@FPaintStruct);
end;

//----------

procedure SWin32Window.eventLoop;
var Msg : TMsg;
begin
  // Hint: Local variable "Msg" does not seem to be initialized
  while Windows.GetMessage(Msg{%H-},0,0,0) do
  begin
    Windows.TranslateMessage(Msg);
    Windows.DispatchMessage(Msg);
  end;
end;

//----------

procedure SWin32Window.flush;
begin
  Windows.GdiFlush;
end;

//----------

procedure SWin32Window.grabCursor;
begin
  Windows.SetCapture(FWindow);
end;

//----------

procedure SWin32Window.hide;
begin
  Windows.ShowWindow(FWindow,SW_HIDE);
end;

//----------

procedure SWin32Window.hideCursor;
begin
  Windows.ShowCursor(false);
end;

//----------

procedure SWin32Window.invalidate(AXpos,AYpos,AWidth,AHeight:longint);
var
  R : Windows.RECT;
begin
  //writeln('invalidate: ',AXpos,',',AYpos,',',AWidth,',',AHeight);
  R.left   := AXpos;
  R.top    := AYpos;
  R.right  := AXpos + AWidth {- 1};
  R.bottom := AYpos + AHeight {- 1};
  // @R=nil works, but redraws entire window :-/
  Windows.InvalidateRect(FWindow,@R,false);
  //UpdateWindow(FWindow);
end;

//----------

procedure SWin32Window.lock;
begin
end;

//----------

procedure SWin32Window.releaseCursor;
begin
  Windows.ReleaseCapture;
end;

//----------

procedure SWin32Window.reparent(AParent:Pointer); // 64-bit !!!
begin
  //FParent : AParent;
  Windows.SetWindowLong(FWindow,GWL_STYLE,(Windows.GetWindowLong(FWindow,GWL_STYLE)
                                          and (not WS_POPUP)) or WS_CHILD);
  // Hint: Conversion between ordinals and pointers is not portable
  // (HWND = HANDLE = System.THandle (DWord/QWord : 32/64 bit))
  Windows.SetParent(FWindow,{%H-}HWND(AParent));
end;

//----------

procedure SWin32Window.resetCursor;
begin
end;

//----------

procedure SWin32Window.sendEvent(AValue:longword);
begin
  // stop complaining! :-)
  AValue := AValue;
end;

//----------

// TODO: redo.. cursor + mode?
{
  array of 'real' values to use
  const for scu_arrow, etc.. smc_ (mouse cursor)
}

procedure SWin32Window.setCursor(ACursor:LongInt);
begin
  //writeln('setcursor');
  if ACursor <> FPrevCursor then
  begin
    FPrevCursor := ACursor;
    FWinCursor := Windows.LoadCursor(0,win32_cursors[ACursor]);
    Windows.SetCursor(FWinCursor);
    //end;
  end;
end;

//----------

procedure SWin32Window.setCursorPos(AXpos,AYpos:LongInt);
begin
  Windows.SetCursorPos(AXpos,AYpos);
end;

//----------

procedure SWin32Window.setPos(AXpos,AYpos:LongInt);
begin
  Windows.SetWindowPos(FWindow,0,AXpos,AYpos,0,0,SWP_NOSIZE or SWP_NOACTIVATE or SWP_NOZORDER);
end;

//----------

procedure SWin32Window.setSize(AWidth,AHeight:LongInt);
//var
//  w,h : longint;
begin
  //w := AWidth  + FAdjustWidth + 0;
  //h := AHeight + FAdjustHeight + 0;
  Windows.SetWindowPos(FWindow,HWND_TOP,0,0,AWidth,AHeight, SWP_NOMOVE);
  //SetWindowPos(mWindow,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
  //FRect.w := AWidth;
  //FRect.h := AHeight;

end;

//----------

procedure SWin32Window.setTitle(ATitle:PChar);
begin
  Windows.SetWindowText(FWindow, ATitle);
end;

//----------

procedure SWin32Window.show;
begin
  //SetWindowPos(mWindow,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
  Windows.ShowWindow(FWindow,SW_SHOW);
end;

//----------

procedure SWin32Window.showCursor;
begin
  Windows.ShowCursor(true);
end;

//----------

procedure SWin32Window.startTimer(ms:LongInt);
begin
  {mTimer:=} Windows.SetTimer(FWindow,sts_timer,ms,nil{timerProc});
end;

//----------

procedure SWin32Window.stopTimer;
begin
  Windows.KillTimer(FWindow, sts_timer);
end;

//----------

procedure SWin32Window.sync;
begin
end;

//----------

procedure SWin32Window.unlock;
begin
end;

//----------------------------------------------------------------------
// eventHandler
//----------------------------------------------------------------------

function SWin32Window.win32_eventHandler(hWnd:HWND; message:UINT; wParam:WPARAM; lParam:LPARAM) : LRESULT;
var
  //DC : HDC;
  //surface : SSurface;
  //cairo   : SCairo;
  r     : SRect;
  //x,y     : LongInt;
  w,h     : LongInt;
  b       : LongInt;
  //R       : RECT;
begin
  result := 0;
  case message of

    //WM_CLOSE:
    //  begin
    //    writeln('WM_CLOSE');
    //    if not FEmbedded then PostQuitMessage(0);
    //  end;

    WM_DESTROY:
      begin
        //writeln('WM_DESTROY');
        if not FEmbedded then Windows.PostQuitMessage(0);
      end;

    WM_GETDLGCODE:
      begin
        //writeln('WM_GETDLGCODE');
        result := DLGC_WANTALLKEYS;
      end;

    WM_ERASEBKGND:
      begin
        result := 1; // tell windows not to erase background
      end;

    WM_KEYDOWN:
      begin
        //writeln('WM_KEYDOWN');
        on_keyDown(wParam,lParam);
      end;

    WM_KEYUP:
      begin
        //writeln('WM_KEYUP');
        on_KeyDown(wParam,lParam);
      end;

    WM_LBUTTONDOWN,
    WM_MBUTTONDOWN,
    WM_RBUTTONDOWN:
      begin
        //writeln('mouse down');
        case message of
          WM_LBUTTONDOWN: b := smb_left;
          WM_MBUTTONDOWN: b := smb_middle;
          WM_RBUTTONDOWN: b := smb_right;
        end;
        //FClickedButton := b;
        on_mouseDown( LOWORD(lParam), HIWORD(lParam), b, wParam {or remapKey(wParam)} );
        if Assigned(FCapturedWidget) then grabCursor;
      end;

    WM_MOUSEMOVE:
        begin
          //writeln('mouse move');
          //x := SmallInt( LOWORD(lParam) );
          //y := SmallInt( HIWORD(lParam) );
          //writeln('x,y :', x, ',' , y );
          on_mouseMove( SmallInt(LOWORD(lParam)), SmallInt(HIWORD(lParam)), {FClickedButton or} remapKey(wParam));
        end;

    WM_LBUTTONUP,
    WM_MBUTTONUP,
    WM_RBUTTONUP:
      begin
        //writeln('mouse up');
        case message of
          WM_LBUTTONUP: b := smb_left;
          WM_MBUTTONUP: b := smb_middle;
          WM_RBUTTONUP: b := smb_right;
        end;
        //FClickedButton := btn;
        on_mouseUp( LOWORD(lParam), HIWORD(lParam), b,wParam {or remapKey(wParam)});
        if not Assigned(FCapturedWidget) then ReleaseCapture;
      end;

    {
      possible conflict:
      - we get WM_PAINT when some other thread is drawing to the backbuffer
        is this possible? or is all gui activity (mouse handling, painting, ..)
        from the same thread? and not happening concurrently? (at the same time)
    }

    WM_PAINT:
      begin
        beginPaint;
        //GetUpdateRect(FWindow,@R,false);
        //ValidateRect(FWindow,@R);
        r.setup( FPaintStruct.rcPaint.Left,
                   FPaintStruct.rcPaint.Top,
                   FPaintStruct.rcPaint.Right - FPaintStruct.rcPaint.Left + 1,
                   FPaintStruct.rcPaint.Bottom - FPaintStruct.rcPaint.Top +1 );
        //on_paint(FCairo,r);
        blitBuffer(r,FPaintDC);
        endPaint;
      end;

    // called every mouse move, compare with previous value!
    WM_SETCURSOR:
      begin
        //writeln('WM_SETCURSOR');
        if LOWORD(lParam) = HTCLIENT then
        begin
          Windows.SetCursor(FWinCursor);
          result := 1;
        end
        else result := Windows.DefWindowProc(hWnd,message,wParam,lParam);
      end;

    WM_SIZE:
      begin
        w := LOWORD(lParam);
        h := HIWORD(lParam);
        //writeln('WM_SIZE: ',w,',',h);
        resizeBuffer(w,h);
        on_setSize(w,h);
        if (FFlags and swf_align) <> 0 then on_align;
        redraw;
      end;

    //WM_WINDOWPOSCHANGED:
    //  begin
    //  end;

    WM_TIMER:
      begin
        //writeln('WM_TIMER');
        if wParam = sts_timer then
        begin
          on_timer;
        end;
      end;

    else
      result := Windows.DefWindowProc(hWnd,message,wParam,lParam);

  end; // case
end;

//----------------------------------------------------------------------
// do_ (events from sub-widgets)
//----------------------------------------------------------------------

procedure SWin32Window.do_move(AWidget:SWidget; AXpos,AYpos:LongInt);
begin
  //writeln('SWin32Window.do_move');
end;

//----------

procedure SWin32Window.do_resize(AWidget:SWidget; AWidth,AHeight:LongInt);
begin
  //writeln('SWin32Window.do_resize');
end;

//----------

procedure SWin32Window.do_redraw(AWidget:SWidget; AState:LongInt);
begin
  //writeln('SWin32Window.do_redraw: ',r.x,',',r.y,',',r.w,',',r.h);
  redraw(AWidget);
end;

//----------

procedure SWin32Window.do_update(AWidget:SWidget);
begin
  //writeln('SWin32Window.do_update');
end;

//----------

procedure SWin32Window.do_cursor(AWidget:SWidget; ACursor:LongInt);
begin
  //writeln('SWin32Window.do_cursor');
  setCursor(ACursor);
end;

//----------

procedure SWin32Window.do_hint(AWidget:SWidget; AHint:PChar);
begin
  //writeln('SWin32Window.do_hint');
  //setHint(AHint);
end;

//----------

procedure SWin32Window.do_modal(AWidget:SWidget; AMode:LongInt);
begin
  //writeln('SWin32Window.do_modal');
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

initialization
  //SWin32WindowClassRegistered := false;
  //SWin32WindowClassName[0] := #0;
  //SWin32WindowClassAtom := 0;
  win32_registerWindow;

//----------

finalization
  {if SWin32WindowClassRegistered then} win32_unregisterWindow;

//----------------------------------------------------------------------
end.

