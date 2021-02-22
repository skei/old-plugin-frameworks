unit z_debug;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

procedure ZTrace(text:PChar);
procedure ZTrace(args:array of const);
procedure ZTrace(text:PChar; args:array of const);

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------
{$ifdef Z_DEBUG}

uses
  sysutils,
  z_string,
  z_util;

//----------------------------------------------------------------------

var
  trace_criticalSection : TRTLCriticalSection;

//----------

procedure z_trace(text:pchar);
begin
  EnterCriticalsection(trace_criticalSection);
  Write(text,LineEnding);
  LeaveCriticalsection(trace_criticalSection);
end;

//----------------------------------------------------------------------

procedure ZTrace(text:pchar);
begin
  z_trace(text);
end;

procedure ZTrace(args:array of const);
begin
  ZTrace('> ',args);
end;

//----------

procedure ZTrace(text:pchar; args:array of const);
var
  i   : LongInt;
  buf : array[0..255] of char;
  b2  : array[0..64] of char;
  //pc2 : PChar;
begin
  ZStrcpy(buf,text);
  if High(args) >= 0 then  // <0
  //begin
  //  ZTrace(buf);
  //  exit;
  //end;
  //Writeln (’Got ’,High(Args)+1,’ arguments :’);
  for i:=0 to High(args) do
  begin
    //write (’Argument ’,i,’ has type ’);
    case args[i].vtype of
      vtinteger:
        begin
          //ZTrace('integer');
          //Writeln (’Integer, Value :’,args[i].vinteger);
          ZIntToString(b2,args[i].vinteger);
          ZStrcat(buf,b2);
        end;
      vtboolean:
        begin
          //ZTrace('boolean');
          //Writeln (’Boolean, Value :’,args[i].vboolean);
          if args[i].VBoolean = true then ZStrcat(buf,'true')
          else ZStrcat(buf,'false');
        end;
      vtchar:
        begin
          //ZTrace('char');
          //Writeln (’Char, value : ’,args[i].vchar);
          b2[0] := args[i].VChar;
          b2[1] := #0;
          ZStrcat(buf,b2);
        end;
      vtextended:
        begin
          //ZTrace('extended');
          //Writeln (’Extended, value : ’,args[i].VExtended^);
          ZFloatToString(b2,args[i].VExtended^);
          ZStrcat(buf,b2);
        end;
      vtString:
        begin
          ZTrace('shortstring');
          //Writeln (’ShortString, value :’,args[i].VString^);
          StrPCopy(b2,args[i].VString^);
          ZStrcat(buf,b2);
        end;
      vtPointer:
        begin
          //ZTrace('pointer');
          //Writeln (’Pointer, value : ’,Longint(Args[i].VPointer));
          ZPtrToString(b2,args[i].VPointer);
          ZStrcat(buf,b2);
        end;
      vtPChar:
        begin
          //ZTrace('pchar');
          //Writeln (’PCHar, value : ’,Args[i].VPChar);
          ZStrcat(buf,args[i].VPChar);
        end;
      vtObject:
        begin
          //ZTrace('object');
          //Writeln (’Object, name : ’,Args[i].VObject.Classname);
          StrPCopy(b2,args[i].VObject.Classname);
          ZStrcat(buf,b2);
        end;
      vtClass:
        begin
          //ZTrace('class');
          //Writeln (’Class reference, name :’,Args[i].VClass.Classname);
          StrPCopy(b2,args[i].VClass.ClassName);
          ZStrcat(buf,b2);
        end;
      vtAnsiString:
        begin
          //ZTrace('ansistring');
          //Writeln (’AnsiString, value :’,AnsiString(Args[I].VAnsiStr
          StrPCopy(b2,AnsiString(args[i].VAnsiString));
          ZStrcat(buf,b2);
        end;
    else
      begin
          ZTrace('unknown');
        //Writeln (’(Unknown) : ’,args[i].vtype);
      end;
    end; // case
  end; // for
  z_trace(buf);
end;

//----------------------------------------------------------------------

initialization
  InitCriticalSection(trace_criticalSection);

finalization
  DoneCriticalsection(trace_criticalSection);

//----------------------------------------------------------------------
{$else}

procedure ZTrace(text:pchar);
begin
end;

procedure ZTrace(args:array of const);
begin
end;

procedure ZTrace(text:PChar; args:array of const);
begin
end;

{$endif}
//----------------------------------------------------------------------
end.

