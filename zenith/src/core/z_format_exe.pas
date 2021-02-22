unit z_format_exe;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_host,
  z_descriptor,
  z_instance,
  z_editor;

type

  ZFormatExe = class
  private
    FDescriptorClass  : ZDescriptorClass;
    FInstanceClass    : ZInstanceClass;
    FEditorClass      : ZEditorClass;
  public
    property  descriptorClass : ZDescriptorClass read FDescriptorClass write FDescriptorClass;
    property  instanceClass   : ZInstanceClass read FInstanceClass write FInstanceClass;
    property  editorClass     : ZEditorClass read FEditorClass write FEditorClass;
  public
    constructor create;
    destructor  destroy; override;
    function    entryPoint(ptr:Pointer) : Pointer;
    procedure   initialize;
  end;

  ZFormatImpl = ZFormatExe;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  z_plugin_base,
  //z_window,       // temp!!!
  //z_rect,
  z_const,
  z_util;

//----------

constructor ZFormatExe.create;
begin
  inherited;
  FDescriptorClass := ZDescriptor;
  FInstanceClass := ZInstance;
  FEditorClass := ZEditor;
end;

//----------

destructor ZFormatExe.destroy;
begin
  inherited;
end;

//--------------------------------------------------

procedure ZFormatExe.initialize;
begin
end;

//----------

function ZFormatExe.entryPoint(ptr:Pointer) : Pointer;
var
  host : ZHost;
  desc : ZDescriptor;
  inst : ZInstance;
  edit : ZEditorBase;
  //win : ZWindow;
begin
  desc := FDescriptorClass.create(nil);
  desc.on_create;
  host := ZHost.create(ptr);
  inst := FInstanceClass.create(host,desc);
  inst.on_create;
  inst.setDefaultParameters; // not tested..

  if ZHasFlag( desc.getFlags, zpf_hasEditor ) then
  begin
    edit := inst.getEditor;
    edit.open(nil);
    edit.eventLoop;
    edit.close
  end;

  inst.on_destroy;
  inst.destroy;
  desc.on_destroy;
  desc.destroy;
  result := ptr;
end;

//----------------------------------------------------------------------
end.

