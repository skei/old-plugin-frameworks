unit z_instance_exe;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZInstanceExe = class(ZInstanceBase)
  protected
    FBlockSize      : LongInt;
    FCurrentProgram : longInt;
    FSampleRate     : Single;
  public
    constructor create({%H-}AHost:ZHostBase; {%H-}ADescriptor:ZDescriptorBase);
    destructor  destroy; override;
  end;

  ZInstanceImpl = ZInstanceExe;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZInstanceExe.create(AHost:ZHostBase; ADescriptor:ZDescriptorBase);
begin
  inherited create;
  FBlockSize      := 0;
  FCurrentProgram := 0;
  FSampleRate     := 0;
end;

//----------

destructor ZInstanceExe.destroy;
begin
  if Assigned(FHost) then FHost.destroy;
  inherited;
end;

//----------------------------------------------------------------------
end.

