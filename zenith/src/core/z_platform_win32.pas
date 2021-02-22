unit z_platform_win32;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  ZPlatformWindows = class
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize;
    function    getBasePath : PChar;
  end;

  ZPlatformImpl = ZPlatformWindows;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZPlatformWindows.create;
begin
  inherited;
end;

//----------

destructor ZPlatformWindows.destroy;
begin
  inherited;
end;

//----------

procedure ZPlatformWindows.initialize;
begin
end;

//----------

function ZPlatformWindows.getBasePath : PChar;
begin
  result := '';
end;

//----------------------------------------------------------------------
end.

