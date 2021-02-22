unit z_object;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZObject = class
  public
    procedure on_create; virtual;
    procedure on_destroy; virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

procedure ZObject.on_create;
begin
end;

//----------

procedure ZObject.on_destroy;
begin
end;

//----------------------------------------------------------------------
end.

