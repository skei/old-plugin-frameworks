{$include sphaera.inc}
unit s_inifile;

{$define S_LEN_SECTIONNAME := 32}
{$define S_LEN_ITEMNAME    := 32}
{$define S_LEN_ITEMVALUE   := 256}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  INIfiles;

type

  SIniItem = record
    FName   : array[0..S_LEN_ITEMNAME] of Char;
    FValue  : array[0..S_LEN_ITEMVALUE] of Char;
  end;

  //----------

  SIniSection = record
    FName   : array[0..S_LEN_SECTIONNAME] of Char;
    FItems  : array of SIniItem;
  end;

  //----------

  SIniFile = class
  public
    FFileName   : PChar;
    FSections   : array of SIniSection;
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

constructor SIniFile.create;
begin
  inherited;
  SetLength(FSections,0);
end;

//----------

destructor SIniFile.destroy;
begin
  SetLength(FSections,0);
  inherited;
end;

//----------

function SIniFile.load(AFileName:PChar) : LongInt;
begin
  result := -1;
end;

//----------

function SIniFile.save(AFileName:PChar) : LongInt;
begin
  result := -1;
end;

//----------

function SIniFile.readSection(AName:PChar) : LongInt;
begin
  result := -1;
end;

//----------

function SIniFile.readValue(AKey:PChar) : PChar;
begin
  result := '';
end;

//----------

function SIniFile.readValue(AKey:PChar) : Single;
begin
  result := 0;
end;

//----------

function SIniFile.readValue(AKey:PChar) : LongInt;
begin
  result := 0;
end;

//----------

procedure SIniFile.writeSection(AName:PChar);
begin
end;

//----------

procedure SIniFile.writeValue(AKey:PChar; AValue:PChar);
begin
end;

//----------

procedure SIniFile.writeValue(AKey:PChar; AValue:Single);
begin
end;

//----------

procedure SIniFile.writeValue(AKey:PChar; AValue:LongInt);
begin
end;

//----------------------------------------------------------------------
end.

