unit z_descriptor_vst;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZDescriptorVst = class(ZDescriptorBase)
  public
    constructor create({%H-}ptr:Pointer);
    destructor  destroy; override;
  end;

  ZDescriptorImpl = ZDescriptorVst;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

// ptr = audioMasterCallback

constructor ZDescriptorVst.create(ptr:Pointer);
begin
  inherited create;
end;

//----------

destructor ZDescriptorVst.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

