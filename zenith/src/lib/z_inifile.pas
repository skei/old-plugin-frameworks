unit z_inifile;
{$include zenith.inc}

{$define Z_LEN_SECTIONNAME := 32}
{$define Z_LEN_ITEMNAME    := 32}
{$define Z_LEN_ITEMVALUE   := 256}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  INIfiles;

type

  ZIniItem = record
    FName   : array[0..Z_LEN_ITEMNAME] of Char;
    FValue  : array[0..Z_LEN_ITEMVALUE] of Char;
  end;

  ZIniSection = record
    FName   : array[0..Z_LEN_SECTIONNAME] of Char;
    FItems  : array of ZIniItem;
  end;

  ZIniFile = class
  public
    FFileName   : PChar;
    FSections   : array of ZIniSection;
  public
    constructor create;
    destructor  destroy; override;
    function    load(AFileName:PChar) : LongInt;
    function    save(AFileName:PChar) : LongInt;
    function    readSection(AName:PChar) : LongInt;
    function    readValue(AKey:PChar) : PChar;
    function    readValue(AKey:PChar) : Single;
    function    readValue(AKey:PChar) : LongInt;
    procedure   writeSection(AName:PChar);
    procedure   writeValue(AKey:PChar; AValue:PChar);
    procedure   writeValue(AKey:PChar; AValue:Single);
    procedure   writeValue(AKey:PChar; AValue:LongInt);
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZIniFile.create;
begin
  inherited;
  SetLength(FSections,0);
end;

destructor ZIniFile.destroy;
begin
  SetLength(FSections,0);
  inherited;
end;

function ZIniFile.load(AFileName:PChar) : LongInt;
begin
  result := -1;
end;

function ZIniFile.save(AFileName:PChar) : LongInt;
begin
  result := -1;
end;

function ZIniFile.readSection(AName:PChar) : LongInt;
begin
  result := -1;
end;

function ZIniFile.readValue(AKey:PChar) : PChar;
begin
  result := '';
end;

function ZIniFile.readValue(AKey:PChar) : Single;
begin
  result := 0;
end;

function ZIniFile.readValue(AKey:PChar) : LongInt;
begin
  result := 0;
end;

procedure ZIniFile.writeSection(AName:PChar);
begin
end;

procedure ZIniFile.writeValue(AKey:PChar; AValue:PChar);
begin
end;

procedure ZIniFile.writeValue(AKey:PChar; AValue:Single);
begin
end;

procedure ZIniFile.writeValue(AKey:PChar; AValue:LongInt);
begin
end;

//----------------------------------------------------------------------
end.

