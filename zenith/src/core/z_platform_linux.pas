unit z_platform_linux;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZPlatformLinux = class
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize;
    function    getBasePath : PChar;
  end;

  ZPlatformImpl = ZPlatformLinux;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZPlatformLinux.create;
begin
  inherited;
end;

//----------

destructor ZPlatformLinux.destroy;
begin
  inherited;
end;

//----------

procedure ZPlatformLinux.initialize;
begin
end;

//----------

function ZPlatformLinux.getBasePath : PChar;
begin
  result := '';
end;

//----------------------------------------------------------------------
end.

