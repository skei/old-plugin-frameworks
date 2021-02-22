{$include sphaera.inc}
unit s_basepath;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

function SGetBaseName(path:PChar) : PChar;
function SGetBasePath(path:PChar) : PChar;
function SFromBasePath(path:PChar; filename:PChar) : PChar;
function SGetFileName(path:PChar) : PChar;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  Windows,
  s_string;

//----------

function SGetBaseName(path:PChar) : PChar;
begin
  path[0] := #0;
  GetModuleFileName(MainInstance,path,255);
  result := path;
end;

//----------

function SGetBasePath(path:PChar) : PChar;
var
  slash : PChar;
  filepath : array[0..255] of Char;
  len : longint;
begin
  filepath := '';
  GetModuleFileName(MainInstance,filepath,255);
  slash := SStrrchr(filepath,'\');
  if Assigned(slash) then
  begin
    len := (slash+1) - PChar(filepath);
    SStrncpy(path,filepath,len);
    path[len] := #0;
  end
  else SStrcat(path,'.\');
  result := path;
end;

//----------

function SFromBasePath(path:PChar; filename:PChar) : PChar;
begin
  SGetBasePath(path);
  SStrcat(path,filename);
  result := path;
end;

//----------


function SGetFileName(path:PChar) : PChar;
var
  slash : PChar;
  backslash : PChar;
begin
  if not Assigned(path) then result := 'NULL';
  slash := SStrrchr(path, '/');
  backslash := SStrrchr(path, '\');
  if Assigned(slash) then result := slash + 1
  else if Assigned(backslash) then result := backslash + 1;
  result := path;
end;

//----------------------------------------------------------------------
end.

