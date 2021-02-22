unit z_format_ladspa;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_descriptor,
  z_instance,
  z_editor;

type

  ZFormatLadspa = class
  private
    FDescriptorClass  : ZDescriptorClass;
    FInstanceClass    : ZInstanceClass;
    FEditorClass      : ZEditorClass;
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize;
    procedure   registerDescriptor(ADescriptorClass:ZDescriptorClass);
    procedure   registerInstance(AInstanceClass:ZInstanceClass);
    procedure   registerEditor(AEditorClass:ZEditorClass);
    function    entryPoint(ptr:Pointer) : Pointer;
  end;

  ZFormatImpl = ZFormatLadspa;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZFormatLadspa.create;
begin
  inherited;
  FDescriptorClass := ZDescriptor;
  FInstanceClass := ZInstance;
  FEditorClass := ZEditor;
end;

//----------

destructor ZFormatLadspa.destroy;
begin
  inherited;
end;

//----------

procedure ZFormatLadspa.initialize;
begin
end;

//----------

procedure ZFormatLadspa.registerDescriptor(ADescriptorClass:ZDescriptorClass);
begin
  FDescriptorClass := ADescriptorClass;
end;

//----------

procedure ZFormatLadspa.registerInstance(AInstanceClass:ZInstanceClass);
begin
  FInstanceClass := AInstanceClass;
end;

//----------

procedure ZFormatLadspa.registerEditor(AEditorClass:ZEditorClass);
begin
  FEditorClass := AEditorClass;
end;

//----------

function ZFormatLadspa.entryPoint(ptr:Pointer) : Pointer;
begin
  result := ptr;
end;

//----------------------------------------------------------------------
end.

