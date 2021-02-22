unit z_host_ladspa;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base,
  z_instance;

type

  ZHostLadspa = class(ZHostBase)
  public
    constructor create(AMaster:Pointer);
    destructor  destroy; override;
  end;

  ZHostImpl = ZHostLadspa;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZHostLadspa.create(AMaster:Pointer);
begin
  inherited create;
end;

//----------

destructor ZHostLadspa.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

