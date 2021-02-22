{$include sphaera.inc}
unit s_debug;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifdef SPH_DEBUG}
{$ifdef SPH_DEBUG_HEAP}
uses
  heaptrc;
{$endif}
{$endif}

//----------

procedure STrace(AText:PChar); inline;
procedure STrace(AText:PChar; AValue:LongInt); inline;
procedure STrace(AText:PChar; AValue:Single); inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{$ifdef SPH_DEBUG}
{$ifdef SPH_DEBUG_CON}
uses
  s_console;
var
  debug_console :  SConsole = nil;
{$endif}
{$endif}

//----------------------------------------------------------------------

procedure STrace(AText:PChar); inline;
begin
  {$ifdef SPH_DEBUG}
  {$ifdef SPH_DEBUG_CON}
  debug_console.writeln(AText);
  {$else}
  //writeln(AText);
  {$endif}
  {$endif}
end;

//----------

procedure STrace(AText:PChar; AValue:LongInt); inline;
begin
  {$ifdef SPH_DEBUG}
  {$ifdef SPH_DEBUG_CON}
  debug_console.writelni(AText,AValue);
  {$else}
  //writeln(AText,AValue);
  {$endif}
  {$endif}
end;

//----------

procedure STrace(AText:PChar; AValue:Single); inline;
begin
  {$ifdef SPH_DEBUG}
  {$ifdef SPH_DEBUG_CON}
  debug_console.writelnf(AText,AValue);
  {$else}
  //writeln(AText,AValue);
  {$endif}
  {$endif}
end;

//----------------------------------------------------------------------

{$ifdef SPH_DEBUG}
{$ifdef SPH_DEBUG_CON}

initialization
  debug_console := SConsole.create;

finalization
  debug_console.destroy;

{$endif}
{$endif}

//----------------------------------------------------------------------
end.
