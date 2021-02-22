unit z_descriptor_ladspa;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZDescriptorLadspa = class(ZDescriptorBase)
  public
    constructor create(ptr:Pointer);
    destructor  destroy; override;
  end;

  ZDescriptorImpl = ZDescriptorLadspa;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZDescriptorLadspa.create(ptr:Pointer);
begin
  inherited create;
  ptr := ptr;
end;

//----------

destructor ZDescriptorLadspa.destroy;
begin
  inherited;
end;

//----------


//----------------------------------------------------------------------
end.

