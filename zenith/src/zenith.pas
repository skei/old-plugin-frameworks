unit zenith;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses

  // core
  z_platform,
  z_interface,
  z_format,

  // plugin
  z_descriptor,
  z_instance,
  z_editor;

//----------------------------------------------------------------------

// procedure zenith_prepare; // implemented in main prog/plug..

function  zenith_initialize : Boolean;
function  zenith_main(ptr:Pointer) : Pointer;

function  zenith_platform : ZPlatform;
function  zenith_interface : ZInterface;
function  zenith_format : ZFormat;

procedure zenith_register(ADescriptorClass:ZDescriptorClass);
procedure zenith_register(AInstanceClass:ZInstanceClass);
procedure zenith_register(AEditorClass:ZEditorClass);


//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

type

  ZGlobal = class
  private
    FInitialized : Boolean;
    FPlatform    : ZPlatform;
    FInterface   : ZInterface;
    FFormat      : ZFormat;
  public
    constructor create;
    destructor  destroy; override;
    function    initialize : Boolean;
  end;

//------------------------------

var
  zenith_global : ZGlobal = nil;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

constructor ZGlobal.create;
begin
  inherited;
  FInitialized := false;
  FPlatform    := ZPlatform.create;
  FInterface   := ZInterface.create;
  FFormat      := ZFormat.create;
end;

//----------

destructor ZGlobal.destroy;
begin
  FFormat.destroy;
  FInterface.destroy;
  FPlatform.destroy;
  inherited;
end;

//----------

// returns true if this is first time

function ZGlobal.initialize : Boolean;
begin
  if not FInitialized then
  begin
    FInitialized := true;
    FPlatform.initialize;
    FInterface.initialize;
    FFormat.initialize;
    result := true;
  end
  else result := false;
end;

//----------------------------------------------------------------------

{
  in zenith_main.inc, we only call zenith_prepare if this returns true
}

// returns true if this is first time

function zenith_initialize : Boolean;
begin
  result := zenith_global.initialize;
end;

//----------

function  zenith_main(ptr:Pointer) : Pointer;
begin
  result := zenith_global.FFormat.entryPoint(ptr);
end;

//------------------------------

function zenith_platform : ZPlatform;
begin
  result := zenith_global.FPlatform;
end;

//----------

function zenith_interface : ZInterface;
begin
  result := zenith_global.FInterface;
end;

//----------

function zenith_format : ZFormat;
begin
  result := zenith_global.FFormat;
end;

//------------------------------

procedure zenith_register(ADescriptorClass:ZDescriptorClass);
begin
  //zenith_global.FFormat.registerDescriptor(ADescriptorClass);
  zenith_global.FFormat.descriptorClass := ADescriptorClass;
end;

//----------

procedure zenith_register(AInstanceClass:ZInstanceClass);
begin
  //zenith_global.FFormat.registerInstance(AInstanceClass);
  zenith_global.FFormat.instanceClass := AInstanceClass;
end;

//----------

procedure zenith_register(AEditorClass:ZEditorClass);
begin
  //zenith_global.FFormat.registerEditor(AEditorClass);
  zenith_global.FFormat.editorClass := AEditorClass;
end;

//----------------------------------------------------------------------

initialization
  zenith_global := ZGlobal.create;

//----------

finalization
  zenith_global.destroy;

//----------------------------------------------------------------------
end.

