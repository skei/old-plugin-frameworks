{$include sphaera.inc}
unit s_application;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SApplication = class
  public
    procedure initialize({%H-}ptr:Pointer); virtual;
    procedure cleanup; virtual;
  public
    procedure on_create; virtual;
    procedure on_destroy; virtual;
  end;

  //SApplication = class(SExecutable);

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

procedure SApplication.initialize(ptr:Pointer);
begin
  self.on_create;
end;

//----------

procedure SApplication.cleanup;
begin
  self.on_destroy;
end;

//----------------------------------------------------------------------

procedure SApplication.on_create;
begin
end;

//----------

procedure SApplication.on_destroy;
begin
end;

//----------------------------------------------------------------------
end.

