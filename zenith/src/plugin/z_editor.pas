unit z_editor;
{$include zenith.inc}

// TODO/REDO:
//   {$ifmdef Z_NOGUI}
//   {$endif}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_NOGUI}  z_editor_nogui;
  {$else}
    {$ifdef Z_NATIVE} z_editor_native; {$endif}
    {$ifdef Z_CAIRO}  z_editor_cairo;  {$endif}
    {$ifdef Z_OPENGL} z_editor_opengl; {$endif}
  {$endif}

type

  ZEditor = ZEditorImpl;
  ZEditorClass = class of ZEditor;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------


//----------------------------------------------------------------------
end.

