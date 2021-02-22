{$include sphaera.inc}
unit s_util;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

procedure SFloatToString(ADest:PChar; AValue:Single);
function  SFloatToInt(val:Single) : LongInt; inline;
function  SFourChar(FC:array of char) : LongWord;
function  SFourChar(c1,c2,c3,c4:Char) : LongWord;
procedure SIntToString(ADest:PChar; AValue:LongInt);
procedure SPtrToString(ADest:PChar; APtr:Pointer);

//----------

procedure SClearFlag(var AValue:LongWord; AFlag:LongWord); inline;
function  SHasFlag(AValue:LongWord; AFlag:LongWord) : Boolean; inline;
procedure SSetFlag(var AValue:LongWord; AFlag:LongWord); inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  SysUtils; // StrCopy

//----------------------------------------

function SFourChar(FC:array of char) : LongWord;
begin
  result := Ord(FC[3]) + (Ord(FC[2]) shl 8) + (Ord(FC[1]) shl 16) + (Ord(FC[0]) shl 24);
end;

//----------

function SFourChar(c1,c2,c3,c4:Char) : LongWord;
begin
  result := Ord(c4)  + (Ord(c3) shl 8) + (Ord(c2) shl 16) + (Ord(c1) shl 24);
end;

//----------------------------------------

// TODO

procedure SFloatToString(ADest:PChar; AValue:Single);
begin
  FormatSettings.DecimalSeparator := '.';
  StrCopy(ADest,PChar(Format('%f',[AValue])));
end;

//----------

procedure SIntToString(ADest:PChar; AValue:LongInt);
begin
  StrCopy(ADest,PChar(Format('%d',[AValue])));
end;

//----------

procedure SPtrToString(ADest:PChar; APtr:Pointer);
begin
  //StrCopy(ADest,PChar(Format('%x',[APtr])));
  StrPCopy(ADest,hexStr(APtr));
end;

//----------

function SFloatToInt(val:Single) : LongINt;
var
  p : ^LongInt;
begin
  p := @(val);
  result := p^;
end;

//----------------------------------------------------------------------

procedure SClearFlag(var AValue:LongWord; AFlag:LongWord); inline;
begin
  AValue := ( AValue and (not AFlag) );
end;

//----------

function SHasFlag(AValue:LongWord; AFlag:LongWord) : Boolean; inline;
begin
  result := ( (AValue and AFlag) <> 0);
end;

//----------

procedure SSetFlag(var AValue:LongWord; AFlag:LongWord); inline;
begin
  AValue := ( AValue or AFlag );
end;

//----------------------------------------------------------------------

// http://rosettacode.org/wiki/Binary_search#Pascal

// Iterative

{function binary_search(element:real; list:array of real): integer;
var
  l, m, h: integer;
begin
  l := 0;
  h := High(list) - 1;
  binary_search := -1;
  while l <= h do
  begin
    m := (l + h) div 2;
    if list[m] > element then
    begin
      h := m - 1;
    end
    else if list[m] < element then
    begin
      l := m + 1;
    end
    else
    begin
      binary_search := m;
      break;
    end;
  end;
end;}

// Usage:
{var
  list: array[0 .. 9] of real;
  // ...
  indexof := binary_search(123, list);}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

//initialization
//  DecimalSeparator := '.';

end.

