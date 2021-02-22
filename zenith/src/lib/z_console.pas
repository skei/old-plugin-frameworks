unit z_console;
{$include zenith.inc}

// TODO

// not thoroughly tested...
// has been crashing.

{$ifdef Z_DEBUG}
  {$ifdef Z_WIN32}
    {$define Z_CON}
  {$endif}
{$endif}

// http://www.halcyon.com/~ast/dload/guicon.htm
// http://msdn.microsoft.com/en-us/library/ms686974%28v=VS.85%29.aspx

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifndef Z_CON}
  type
    ZConsole = class
    public
      constructor create;
      destructor  destroy; override;
      procedure   write(AText:PChar);
    end;
{$else}
  uses
    Windows;
  type
    ZConsole = class
    private
      FStdout     : HANDLE;
    public
      constructor create;
      destructor  destroy; override;
      procedure   write(AText:PChar);
    end;
{$endif}

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{$ifndef Z_CON}

constructor ZConsole.create;
begin
end;

//----------

destructor ZConsole.destroy;
begin
end;

//----------

procedure ZConsole.write(AText:PChar);
begin
end;

//------------------------------

{$else}

uses
  z_string;

//----------

constructor ZConsole.create;
begin
  AllocConsole;
  SetConsoleTitle('zenith console');
  FStdout := GetStdHandle( STD_OUTPUT_HANDLE );
end;

//----------

destructor ZConsole.destroy;
begin
  FreeConsole;
end;

//----------

procedure ZConsole.write(AText:PChar);
var written : LongWord;
begin
  WriteConsole( FStdout,AText,ZStrlen(AText),written,nil);
end;

{$endif}

//----------------------------------------------------------------------
end.

