unit z_host;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_EXE}    z_host_exe;    {$endif}
  {$ifdef Z_LADSPA} z_host_ladspa; {$endif}
  {$ifdef Z_VST}    z_host_vst;    {$endif}

type

  ZHost = class(ZHostImpl)
  public
    constructor Create({%H-}AMaster: Pointer);
    destructor  Destroy; override;
  end;

  //----------

  ZHostClass = class of ZHost;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZHost.Create(AMaster: Pointer);
begin
  inherited;
end;

//----------

destructor ZHost.Destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

