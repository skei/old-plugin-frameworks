unit z_instance_ladspa;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base,
  z_descriptor;

type

  ZInstanceLadspa = class(ZInstanceBase);
  public
    constructor create(ADescriptor:ZDescriptor);
    destructor  destroy; override;
  end;

  ZInstanceImpl = ZInstanceLadspa;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZInstanceLadspa.create(ADescriptor:ZDescriptor);
begin
  inherited create;
  ADescriptor := ADescriptor;
end;

//----------

destructor ZInstanceLadspa.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

