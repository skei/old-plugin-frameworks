{$include sphaera.inc}
unit s_library;

// procedure DoSomething; external 'MYLIB.DLL';

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  dynlibs;

type

  SLibrary = class
  protected
    mHandle     : TLibHandle;
  public
    constructor Create;
    destructor  Destroy; override;
    function    LoadLib(aFilename:pchar) : LongInt; virtual;
    procedure   CloseLib; virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor SLibrary.Create;
begin
  inherited;
  mHandle := 0;//nil;
end;

//----------

destructor SLibrary.Destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------

function SLibrary.LoadLib(aFilename:pchar) : LongInt;
begin
  if (mHandle<>0) then exit(-1);
  mHandle := LoadLibrary(aFilename);
  if (mHandle=0) then exit(-2);
  //pointer(mDispatcher) := GetProcedureAddress(mHandle,'libaam');
  //if not Assigned(mDispatcher) then exit(-3);
  result := 0;
end;

//----------

procedure SLibrary.CloseLib;
begin
  if (mHandle=0) then exit;
  UnloadLibrary(mHandle);
  mHandle := 0;
  //mDispatcher := nil;
end;


//----------------------------------------------------------------------
end.

