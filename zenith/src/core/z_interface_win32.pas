unit z_interface_win32;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifndef Z_NOGUI}
uses
  Windows;
{$endif}

type

  ZInterfaceWin32 = class
  private
    {$ifndef Z_NOGUI}
    FRegistered : Boolean;
    FInstance   : HINST;
    FClassName  : array[0..255] of char;
    FClassAtom  : ATOM;
    {$endif}
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize;
    {$ifndef Z_NOGUI}
    function    getInstance : HINST;
    function    getClassName : PChar;
    function    getClassAtom : ATOM;
    {$endif}
  end;

  ZInterfaceImpl = ZInterfaceWin32;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  {$ifndef Z_NOGUI}
  z_window_win32,
  z_string,
  z_memory,
  z_util,
  {$endif}
  zenith;

//----------

{$ifndef Z_NOGUI}

//LRESULT CALLBACK win32_eventProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
function win32_eventProc(hWnd:HWND; message:UINT; wParam:WPARAM; lParam:LPARAM) : LRESULT; stdcall;
var win : ZWindowWin32;
begin
  win := ZWindowWin32( Windows.GetWindowLongPtr(hWnd,GWL_USERDATA) );
  if Assigned(win) then result := win.win32_eventHandler(hWnd,message,wParam,lParam)
  else result := Windows.DefWindowProc(hWnd,message,wParam,lParam);
end;

{$endif}

//----------

constructor ZInterfaceWin32.create;
begin
  inherited;
  {$ifndef Z_NOGUI}
  FRegistered := false;
  FInstance := MainInstance;
  FClassName[0] := #0; //to be sure...
  FClassAtom := 0;
  {$endif}
end;

//----------

destructor ZInterfaceWin32.destroy;
begin
  {$ifndef Z_NOGUI}
  if FRegistered then UnregisterClass(FClassName,FInstance);
  {$endif}
  inherited;
end;

//----------

procedure ZInterfaceWin32.initialize;
{$ifndef Z_NOGUI}
var
  buf : array[0..32] of char;
  wc : WNDCLASS;
{$endif}
begin
  {$ifndef Z_NOGUI}
  //global_instance := System.MainInstance;
  ZStrcpy(FClassName,'zenith_window_');
  ZPtrToString(buf,self);
  ZStrcat(FClassName,buf);
  ZMemset(@wc,0,sizeof(wc));
  wc.style         := (CS_HREDRAW or CS_VREDRAW);
  wc.lpfnWndProc   := @win32_eventProc;
  wc.hInstance     := FInstance;
  wc.lpszClassName := @FClassName;
  //wc.hCursor       := LoadCursor(0,IDC_CROSS);
  //wc.hIcon         := LoadIcon(global_instance,IDI_APPLICATION);
  FClassAtom := Windows.RegisterClass(@wc);
  FRegistered := true;;
  {$endif}
end;

//----------

{$ifndef Z_NOGUI}

function ZInterfaceWin32.getInstance : HINST;
begin
  result := FInstance;
end;

//----------

function ZInterfaceWin32.getClassName : PChar;
begin
  result := @FClassName;
end;

//----------

function ZInterfaceWin32.getClassAtom : ATOM;
begin
  result := FClassAtom;
end;

{$endif}

//----------------------------------------------------------------------
end.

