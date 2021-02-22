{$include sphaera.inc}
unit s_program;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_array;

//----------

type

  SProgram = class
  protected
    FName       : array[0..255] of Char;
    FIndex      : LongInt;
    FValues     : PSingle;
  public
    constructor create;
    destructor  destroy; override;
    function    getIndex : LongInt; //virtual;
    procedure   setIndex(AIndex:LongInt); //virtual;
    function    getName : PChar; //virtual;
    procedure   setName(AName:PChar); //virtual;
  public
    property    values : PSingle read FValues write FValues;
  end;

  SProgramArray = specialize SArray<SProgram>;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_string;

//----------

constructor SProgram.create;
begin
  FName := 'program';
  FIndex := -1;
  FValues := nil;
end;

//----------

destructor SProgram.destroy;
begin
end;

//----------

function SProgram.getIndex : LongInt;
begin
  result := FIndex;
end;

//----------

procedure SProgram.setIndex(AIndex:LongInt);
begin
  FIndex := AIndex;
end;

//----------

function SProgram.getName : PChar;
begin
  Result := FName;
end;

//----------

procedure SProgram.setName(AName:PChar);
begin
  SStrncpy(FName,AName,255);
  //FName := AName;
end;

//----------------------------------------------------------------------
end.
