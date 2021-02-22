unit z_descriptor_exe;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZDescriptorExe = class(ZDescriptorBase)
  public
    constructor create({%H-}ptr:Pointer);
    destructor  destroy; override;
  end;

  ZDescriptorImpl = ZDescriptorExe;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZDescriptorExe.create(ptr:Pointer);
begin
  inherited create;
  //ptr := ptr;
end;

//----------

destructor ZDescriptorExe.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

