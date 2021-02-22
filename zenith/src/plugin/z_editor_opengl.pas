unit z_editor_opengl;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;

type

  ZEditorOpenGl = class(ZEditor)
  public
    constructor create({%H-}AInstance:ZInstanceBase);
    destructor  destroy; override;
  end;

  ZEditorImpl = ZEditorOpenGl;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//uses
//  z_rect;

//----------

constructor ZEditorOpenGl.create(AInstance:ZInstanceBase);
begin
  inherited create;
end;

//----------

destructor ZEditorOpenGl.destroy;
begin
  inherited;
end;

//----------------------------------------------------------------------
end.

