{$include sphaera.inc}
unit s_console;

{
  not thoroughly tested...

  * conflicts with console & heaptrc?
    initialization/finalization order? (heaptrc)

  http://www.halcyon.com/~ast/dload/guicon.htm
  http://msdn.microsoft.com/en-us/library/ms686974%28v=VS.85%29.aspx
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Windows;

type

  SConsole = class
  private
    {$ifdef SPH_DEBUG}
    FStdout : HANDLE;
    {$endif}
    //FTicks      : LongWord;
  public
    constructor create;
    destructor  destroy; override;
    procedure   ln;
    procedure   write(AText:PChar);
    procedure   writei(AValue:LongInt);
    procedure   writef(AValue:Single);
    procedure   writeln(AText:PChar);
    procedure   writelni(AText:PChar; AValue:LongInt);
    procedure   writelnf(AText:PChar; AValue:Single);
  end;

//{$endif}

//var
//  sphaera_console :  SConsole = nil;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_string,
  s_util;

//----------

constructor SConsole.create;
begin
  {$ifdef SPH_DEBUG}
  AllocConsole;
  SetConsoleTitle('sphaera console');
  FStdout := GetStdHandle( STD_OUTPUT_HANDLE );
  //FTicks := GetTickCount();
  {$endif}
end;

//----------

destructor SConsole.destroy;
begin
  {$ifdef SPH_DEBUG}
  FreeConsole;
  {$endif}
end;

//------------------------------

procedure SConsole.ln;
begin
  {$ifdef SPH_DEBUG}
  self.write(#13#10);
  {$endif}
end;

//----------

procedure SConsole.write(AText:PChar);
{$ifdef SPH_DEBUG}
var
  written : LongWord;
{$endif}
begin
  {$ifdef SPH_DEBUG}
  WriteConsole( FStdout,AText,SStrlen(AText),written{%H-},nil);
  {$endif}
end;

//----------

procedure SConsole.writei(AValue:LongInt);
{$ifdef SPH_DEBUG}
var
  buf : array[0..256] of char;
{$endif}
begin
  {$ifdef SPH_DEBUG}
  SIntToString(buf,AValue);
  self.write(buf);
  {$endif}
end;

//----------

procedure SConsole.writef(AValue:Single);
{$ifdef SPH_DEBUG}
var
  buf : array[0..256] of char;
{$endif}
begin
  {$ifdef SPH_DEBUG}
  SFloatToString(buf,AValue);
  self.write(buf);
  {$endif}
end;

//------------------------------

procedure SConsole.writeln(AText:PChar);
begin
  {$ifdef SPH_DEBUG}
  self.write(Atext);
  self.ln;
  {$endif}
end;

procedure SConsole.writelni(AText:PChar; AValue:LongInt);
begin
  {$ifdef SPH_DEBUG}
  self.write(Atext);
  self.writei(AValue);
  self.ln;
  {$endif}
end;

procedure SConsole.writelnf(AText:PChar; AValue:Single);
begin
  {$ifdef SPH_DEBUG}
  self.write(Atext);
  self.writef(AValue);
  self.ln;
  {$endif}
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

{.$ifdef SPH_DEBUG}

//initialization
//  sphaera_console := SConsole.create;
//
//finalization
//  sphaera_console.destroy;

{.$endif}

//----------------------------------------------------------------------
end.

