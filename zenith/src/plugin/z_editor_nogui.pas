unit z_editor_nogui;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZEditorNogui = class(ZEditorBase)
  public
    constructor create({%H-}AInstance:ZInstanceBase);
    destructor  destroy; override;
  end;

  ZEditorImpl = ZEditorNogui;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  z_rect;

//----------

constructor ZEditorNogui.create(AInstance:ZInstanceBase);
begin
  inherited create;
end;

//----------

destructor ZEditorNogui.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

