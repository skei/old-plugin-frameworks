unit z_window_win32;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Windows,
  z_gui_base,
  z_rect,
  z_cairo;

var
  win32_cursors : array[0..19] of HCURSOR =
    (
      LongInt( IDC_ARROW ),       // zcu_default
      LongInt( IDC_ARROW ),       // zcu_Arrow
      LongInt( IDC_UPARROW ),     // zcu_ArrowUp
      LongInt( IDC_ARROW ),       // zcu_ArrowDown
      LongInt( IDC_ARROW ),       // zcu_ArrowLeft
      LongInt( IDC_ARROW ),       // zcu_ArrowRight
      LongInt( IDC_SIZENS ),      // zcu_ArrowUpDown
      LongInt( IDC_SIZEWE ),      // zcu_ArrowLeftRight
      LongInt( IDC_SIZENESW ),    // zcu_ArrowDiagLeft
      LongInt( IDC_SIZENWSE ),    // zcu_ArrowDiagRight
      LongInt( IDC_SIZEALL ),     // zcu_Move
      LongInt( IDC_WAIT ),        // zcu_Wait
      LongInt( IDC_APPSTARTING ), // zcu_ArrowWait
      LongInt( IDC_HAND),         // zcu_Hand
      LongInt( IDC_HAND),         // zcu_Finger
      LongInt( IDC_CROSS),        // zcu_Cross
      LongInt( IDC_ARROW ),       // zcu_Pencil
      LongInt( IDC_ARROW ),       // zcu_Plus
      LongInt( IDC_ARROW ),       // zcu_Question
      LongInt( IDC_ARROW )        // zcu_Ibeam
    );

  {
    #define DEF_CURSOR    cu_Arrow
    // #DEFINE idc_arrow       (32512)
    // #DEFINE idc_ibeam       (32513)
    // #DEFINE idc_wait        (32514)
    // #DEFINE idc_cross       (32515)
    // #DEFINE idc_uparrow     (32516)
    // #DEFINE idc_size        (32640)
    // #DEFINE idc_icon        (32641)
    // #DEFINE idc_sizenwse    (32642)
    // #DEFINE idc_sizenesw    (32643)
    // #DEFINE idc_sizewe      (32644)
    // #DEFINE idc_sizens      (32645)
  }

type

  ZWindowWin32 = class(ZWindowBase)
  private
    FCursor       : HCURSOR;
    FEmbedded     : Boolean;
    FInstance     : HINST;
    FPaintStruct  : TPAINTSTRUCT;
    FPrevCursor   : HCURSOR;
    FWindow       : HWND;
    FPaintDC      : HDC;
  public
    property    _embedded : Boolean read FEmbedded;
    property    _instance : HINST read FInstance;
    property    _window : HWND read FWindow;
    //procedure   win32_eventHandler;
  public
    constructor create(ARect:ZRect; AParent:Pointer=nil);
    destructor  destroy; override;
    function    win32_eventHandler(hWnd:HWND; message:UINT; wParam:WPARAM; lParam:LPARAM) : LRESULT;
    function    win32_remapKey(AState:LongInt) : LongInt;
    function    win32_remapButton(AButton:LongInt) : LongInt;
  public
    procedure   beginPaint; override;
    procedure   endPaint; override;
    procedure   eventLoop; override;
    procedure   flush; override;
    procedure   grabCursor; override;
    procedure   hide; override;
    procedure   hideCursor; override;
    procedure   invalidate(AXpos,AYpos,AWidth,AHeight:LongInt); override;
    procedure   lock; override;
    procedure   releaseCursor; override;
    procedure   reparent(AParent:Pointer); override;
    procedure   resetCursor; override;
    procedure   sendEvent(AValue:longword); override;
    procedure   setCursor(ACursor:LongInt); override;
    procedure   setCursorPos({%H-}AXpos,{%H-}AYpos:LongInt); override;
    procedure   setPos(AXpos,AYpos:LongInt); override;
    procedure   setSize(AWidth,AHeight:LongInt); override;
    procedure   setTitle({%H-}ATitle:PChar); override;
    procedure   show; override;
    procedure   showCursor; override;
    procedure   startTimer(ms:LongInt); override;
    procedure   stopTimer; override;
    procedure   sync; override;
    procedure   unlock; override;
  end;

  ZWindowImpl = ZWindowWin32;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  zenith,
  SysUtils,   // sleep
  z_const,
  z_util,
  z_cairo_win32;

//----------------------------------------------------------------------
// Window
//----------------------------------------------------------------------

constructor ZWindowWin32.create(ARect:ZRect; AParent:Pointer); // [ZWindowBase] / ZWidget
var
  rc    : Windows.RECT;
  wx,wy : LongWord;
begin
  inherited create(ARect);
  FInstance   := zenith_interface.getInstance;
  FCursor     := Windows.LoadCursor(0,IDC_ARROW); // crash??
  FPrevCursor := 0;
  rc.Left   := ARect.x;
  rc.Top    := ARect.y;
  rc.Right  := ARect.x2;
  rc.Bottom := ARect.y2;
  if Assigned(AParent) then
  begin
    FEmbedded := true;
    Windows.AdjustWindowRectEx(@rc,{WS_OVERLAPPEDWINDOW|} WS_POPUP, FALSE, WS_EX_TOOLWINDOW );
    FWindow := Windows.CreateWindowEx(
      WS_EX_TOOLWINDOW,
      zenith_interface.getClassName,
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
      WS_EX_OVERLAPPEDWINDOW,         // dwExStyle
      zenith_interface.getClassName,  // lpClassName
      nil,                            // lpWindowName
      WS_OVERLAPPEDWINDOW,            // dwStyle
      wx,                             // center x
      wy,                             // center y
      rc.right -rc.left+1,            // wWidth,
      rc.bottom-rc.top +1,            // wHeight,
      0,                              // hWndParent
      0,                              // hMenu
      FInstance,                      // hInstance
      nil                             // lpParam
    );
    Windows.SetFocus(FWindow);
  end; // windowed
  Windows.SetWindowLongPtr(FWindow,GWL_USERDATA, LONG_PTR(self));
end;

//----------

destructor  ZWindowWin32.destroy;
begin
  Windows.DestroyWindow(FWindow);
  inherited;
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

procedure ZWindowWin32.beginPaint;
begin
  FPaintDC := Windows.BeginPaint(FWindow,@FPaintStruct);
end;

//----------

procedure ZWindowWin32.endPaint;
begin
  Windows.EndPaint(FWindow,@FPaintStruct);
end;

//----------

procedure ZWindowWin32.eventLoop;
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

procedure ZWindowWin32.flush;
begin
  Windows.GdiFlush;
end;

//----------

procedure ZWindowWin32.grabCursor;
begin
  Windows.SetCapture(FWindow);
end;

//----------

procedure ZWindowWin32.hide;
begin
  Windows.ShowWindow(FWindow,SW_HIDE);
end;

//----------

procedure ZWindowWin32.hideCursor;
begin
  Windows.ShowCursor(false);
end;

//----------

procedure ZWindowWin32.invalidate(AXpos,AYpos,AWidth,AHeight:longint);
var
  R : Windows.RECT;
begin
  R.left   := AXpos;
  R.top    := AYpos;
  R.right  := AXpos + AWidth - 1;
  R.bottom := AYpos + AHeight - 1;
  Windows.InvalidateRect(FWindow,@R,false);
  //UpdateWindow(mWindow);
end;

//----------

procedure ZWindowWin32.lock;
begin
end;

//----------

procedure ZWindowWin32.releaseCursor;
begin
  Windows.ReleaseCapture;
end;

//----------

procedure ZWindowWin32.reparent(AParent:Pointer); // 64-bit !!!
begin
  //FParent : AParent;
  Windows.SetWindowLong(FWindow,GWL_STYLE,(Windows.GetWindowLong(FWindow,GWL_STYLE)
                                          and (not WS_POPUP)) or WS_CHILD);
  // Hint: Conversion between ordinals and pointers is not portable
  // (HWND = HANDLE = System.THandle (DWord/QWord : 32/64 bit))
  Windows.SetParent(FWindow,{%H-}HWND(AParent));
end;

//----------

procedure ZWindowWin32.resetCursor;
begin
end;

//----------

procedure ZWindowWin32.sendEvent(AValue:longword);
begin
  // stop complaining! :-)
  AValue := AValue;
end;

//----------

// TODO: redo.. cursor + mode?
{
  array of 'real' values to use
  const for zcu_arrow, etc..
}

procedure ZWindowWin32.setCursor(ACursor:LongInt);
begin
  if ACursor < 0 then
  begin
    hideCursor;// aCursor = DEF_CURSOR;
    FPrevCursor := ACursor;
  end //-1
  else
  begin
    // Warning: unreachable code (showCursor)
    if FPrevCursor < 0 then showCursor;
    // Hint: Mixing signed expressions and longwords gives a 64bit result
    if ACursor <> FPrevCursor then
    begin
      // Hint: Conversion between ordinals and pointers is not portable
      FCursor := Windows.LoadCursor(0,PChar(ACursor));
      Windows.SetCursor( FCursor );
      FPrevCursor := ACursor;
    end;
  end; //>0
end;

//----------

procedure ZWindowWin32.setCursorPos(AXpos,AYpos:LongInt);
begin
  Windows.SetCursorPos(AXpos,AYpos);
end;

//----------

procedure ZWindowWin32.setPos(AXpos,AYpos:LongInt);
begin
  Windows.SetWindowPos(FWindow,0,AXpos,AYpos,0,0,SWP_NOSIZE or SWP_NOACTIVATE or SWP_NOZORDER);
end;

//----------

procedure ZWindowWin32.setSize(AWidth,AHeight:LongInt);
//var
//  w,h : longint;
begin
  //w := AWidth  + FAdjustWidth + 0;
  //h := AHeight + FAdjustHeight + 0;
  Windows.SetWindowPos(FWindow,HWND_TOP,0,0,AWidth,AHeight, SWP_NOMOVE);
  //SetWindowPos(mWindow,0,0,0,aWidth,aHeight,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOZORDER);
end;

//----------

procedure ZWindowWin32.setTitle(ATitle:PChar);
begin
  Windows.SetWindowText(FWindow, ATitle);
end;

//----------

procedure ZWindowWin32.show;
begin
  //SetWindowPos(mWindow,0,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);
  Windows.ShowWindow(FWindow,SW_SHOW);
end;

//----------

procedure ZWindowWin32.showCursor;
begin
  Windows.ShowCursor(true);
end;

//----------

procedure ZWindowWin32.startTimer(ms:LongInt);
begin
  {mTimer:=} Windows.SetTimer(FWindow,zts_timer,ms,nil{timerProc});
end;

//----------

procedure ZWindowWin32.stopTimer;
begin
  Windows.KillTimer(FWindow, zts_timer);
end;

//----------

procedure ZWindowWin32.sync;
begin
end;

//----------

procedure ZWindowWin32.unlock;
begin
end;

//----------------------------------------------------------------------
// private
//----------------------------------------------------------------------

function ZWindowWin32.win32_remapButton(AButton:LongInt) : LongInt;
begin
  result := AButton;
end;

//----------

function ZWindowWin32.win32_remapKey(AState:LongInt) : LongInt;
begin
  result := zbu_none;
  if (AState and MK_SHIFT)   <> 0 then result += zbu_shift;
  if (AState and MK_CONTROL) <> 0 then result += zbu_ctrl;
  if Windows.GetKeyState(VK_MENU) < 0 then result += zbu_alt;
end;

//----------------------------------------------------------------------
// eventHandler
//----------------------------------------------------------------------

function ZWindowWin32.win32_eventHandler(hWnd:HWND; message:UINT; wParam:WPARAM; lParam:LPARAM) : LRESULT;
var
  surface : ZCairoSurface;
  cairo   : ZCairo;
  rec     : ZRect;
  w,h     : LongInt;
  b       : LongInt;
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
          WM_LBUTTONDOWN: b := zbu_left;
          WM_MBUTTONDOWN: b := zbu_middle;
          WM_RBUTTONDOWN: b := zbu_right;
        end;
        //FClickedButton := b;
        on_mouseDown( LOWORD(lParam), HIWORD(lParam), b, wParam {or remapKey(wParam)} );
        if Assigned(FCapturedWidget) then grabCursor;
      end;

    WM_MOUSEMOVE:
        begin
          //writeln('mouse move');
          on_mouseMove( LOWORD(lParam), HIWORD(lParam), {FClickedButton or} win32_remapKey(wParam));
        end;

    WM_LBUTTONUP,
    WM_MBUTTONUP,
    WM_RBUTTONUP:
      begin
        //writeln('mouse up');
        case message of
          WM_LBUTTONUP: b := zbu_left;
          WM_MBUTTONUP: b := zbu_middle;
          WM_RBUTTONUP: b := zbu_right;
        end;
        //FClickedButton := btn;
        on_mouseUp( LOWORD(lParam), HIWORD(lParam), b,wParam {or remapKey(wParam)});
        if not Assigned(FCapturedWidget) then ReleaseCapture;
      end;

    WM_PAINT:
      begin
        beginPaint;
        rec.setup(FPaintStruct.rcPaint.Left,
                  FPaintStruct.rcPaint.Top,
                  FPaintStruct.rcPaint.Right - FPaintStruct.rcPaint.Left + 1,
                  FPaintStruct.rcPaint.Bottom - FPaintStruct.rcPaint.Top + 1  );
        //writeln('WM_PAINT: ',rec.x,',',rec.y,',',rec.w,',',rec.h);
        surface := ZCairoSurface( ZCairoSurfaceWin32.create( FPaintStruct.hdc) );
        cairo := ZCairo.create(surface);
        if ZHasFlag(FFlags,zwf_clip) then
        begin
          cairo.rectangle(rec.x,rec.y,rec.w,rec.h);
          cairo.clip;
        end;

        //on_paint(cairo,rec);

        cairo.setSourceColor(0,0,0);
        cairo.rectangle(rec.x,rec.y,rec.w,rec.h);
        cairo.fill;

        cairo.setSourceColor(1,0,0);
        cairo.arc(rec.w div 2, rec.h div 2, 50,0,Z_PI*2);
        cairo.stroke;

        //if (FFlags and zwf_clip) <> 0 then cairo.resetClip;
        cairo.destroy;
        surface.destroy;
        endPaint;
      end;

    // called every mouse move, compare with previous value!
    WM_SETCURSOR:
      begin
        //writeln('WM_SETCURSOR');
        if LOWORD(lParam) = HTCLIENT then
        begin
          Windows.SetCursor(FCursor);
          result := 1;
        end
        else result := Windows.DefWindowProc(hWnd,message,wParam,lParam);
      end;

    WM_SIZE:
      begin
        w := LOWORD(lParam);
        h := HIWORD(lParam);
        //writeln('WM_SIZE: ',w,',',h);
        on_setSize(w,h);
        if (FFlags and zwf_align) <> 0 then
        begin
          on_align;
          //redrawAll;
        end;
      end;

    WM_TIMER:
      begin
        //writeln('WM_TIMER');
        if wParam = zts_timer then
        begin
          on_timer;
        end;
      end;

    else
      result := Windows.DefWindowProc(hWnd,message,wParam,lParam);

  end; // case
end;

//----------------------------------------------------------------------
end.

