unit z_interface_linux;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZInterfaceLinux = class
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize;
  end;

  ZInterfaceImpl = ZInterfaceLinux;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{$ifndef Z_NOGUI}
uses
  Xlib;
{$endif}

//----------

constructor ZInterfaceLinux.create;
begin
  inherited;
end;

//----------

destructor ZInterfaceLinux.destroy;
begin
  inherited;
end;

//----------

procedure ZInterfaceLinux.initialize;
begin
  {$ifndef Z_NOGUI} XInitThreads; {$endif}
end;

//----------------------------------------------------------------------
end.

