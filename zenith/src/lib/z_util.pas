unit z_util;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

procedure ZFloatToString(ADest:PChar; AValue:Single);
function  ZFloatToInt(val:Single) : LongInt; inline;
function  ZFourChar(FC:array of char) : LongWord;
function  ZFourChar(c1,c2,c3,c4:Char) : LongWord;
procedure ZIntToString(ADest:PChar; AValue:LongInt);
procedure ZPtrToString(ADest:PChar; APtr:Pointer);

//----------

procedure ZClearFlag(var AValue:LongWord; AFlag:LongWord); inline;
function  ZHasFlag(AValue:LongWord; AFlag:LongWord) : Boolean; inline;
procedure ZSetFlag(var AValue:LongWord; AFlag:LongWord); inline;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  SysUtils; // StrCopy

//----------------------------------------

function ZFourChar(FC:array of char) : LongWord;
begin
  result := Ord(FC[3]) + (Ord(FC[2]) shl 8) + (Ord(FC[1]) shl 16) + (Ord(FC[0]) shl 24);
end;

//----------

function ZFourChar(c1,c2,c3,c4:Char) : LongWord;
begin
  result := Ord(c4)  + (Ord(c3) shl 8) + (Ord(c2) shl 16) + (Ord(c1) shl 24);
end;

//----------------------------------------

// TODO

procedure ZFloatToString(ADest:PChar; AValue:Single);
begin
  StrCopy(ADest,PChar(Format('%f',[AValue])));
end;

//----------

procedure ZIntToString(ADest:PChar; AValue:LongInt);
begin
  StrCopy(ADest,PChar(Format('%d',[AValue])));
end;

//----------

procedure ZPtrToString(ADest:PChar; APtr:Pointer);
begin
  //StrCopy(ADest,PChar(Format('%x',[APtr])));
  StrPCopy(ADest,hexStr(APtr));
end;

//----------

function ZFloatToInt(val:Single) : LongINt;
var
  p : ^LongInt;
begin
  p := @(val);
  result := p^;
end;

//----------------------------------------------------------------------

procedure ZClearFlag(var AValue:LongWord; AFlag:LongWord); inline;
begin
  AValue := ( AValue and (not AFlag) );
end;

//----------

function ZHasFlag(AValue:LongWord; AFlag:LongWord) : Boolean; inline;
begin
  result := ( (AValue and AFlag) <> 0);
end;

//----------

procedure ZSetFlag(var AValue:LongWord; AFlag:LongWord); inline;
begin
  AValue := ( AValue or AFlag );
end;

//----------------------------------------------------------------------

// http://rosettacode.org/wiki/Binary_search#Pascal

// Iterative

{function binary_search(element: real; list: array of real): integer;
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
end.

