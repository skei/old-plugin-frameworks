unit z_format;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_EXE}    z_format_exe;    {$endif}
  {$ifdef Z_LADSPA} z_format_ladspa; {$endif}
  {$ifdef Z_VST}    z_format_vst;    {$endif}

type
  ZFormat = ZFormatImpl;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
end.

