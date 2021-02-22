unit z_editor_native;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZEditorNative = class(ZEditorBase)
  public
    constructor create({%H-}AInstance:ZInstanceBase);
    destructor  destroy; override;
  end;

  ZEditorImpl = ZEditorNative;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  z_rect;

//----------

constructor ZEditorNative.create(AInstance:ZInstanceBase);
begin
  inherited create;
end;

//----------

destructor ZEditorNative.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

