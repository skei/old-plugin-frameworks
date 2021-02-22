
// todo: command-line or compiler argument (batch file)
{.$define Z_VST}
{.$define Z_EXE}

{.$define Z_NOGUI}
{$define Z_CAIRO}
{.$define Z_CAIRO}

//----------------------------------------------------------------------

{
  TODO:
  all common arguments here, instead of command line..
  simplifies batch files, etc..
}

//{$apptype console}

{$asmmode intel}
{$fputype sse2} // x87,sse,sse2
{$inline on}
{$interfaces corba} // no parent, no refcount
{$macro on}
{$mode objfpc}
{$writeableconst on}

//----------------------------------------------------------------------

{$ifdef linux}
  {$define Z_LINUX}
{$endif}

{$ifdef windows}
  {$define Z_WIN32}
{$endif}

{$ifdef CPU32}
  {$define Z_32BIT}
{$endif}

{$ifdef CPU64}
  {$define Z_64BIT}
{$endif}

//----------------------------------------------------------------------

{$undef format_defined}

//---

{$ifdef Z_EXE}
  {$define format_defined}
  {$undef Z_LIB}
{$endif}

{$ifdef Z_LADSPA}
  {$define format_defined}
  {$define Z_LIB}
  {$define Z_NOGUI}
{$endif}

{$ifdef Z_VST}
  {$define format_defined}
  {$define Z_LIB}
{$endif}

//---

{$ifndef format_defined}
  // {$error no format specified
  // assume vst if not specified
  {$define Z_VST}
  {$define Z_LIB}
{$endif}

//----------------------------------------------------------------------

{$undef gui_defined}

//---

{$ifdef Z_NOGUI}
  {$define gui_defined}
  {$undef Z_CAIRO}
  {$undef Z_OPENGL}
{$endif}

{$ifdef Z_CAIRO}
  {$define gui_defined}
{$endif}

{$ifdef Z_OPENGL}
  {$define gui_defined}
{$endif}

//---

//{$ifdef Z_NOGUI}
{$ifndef gui_defined}
  // {$error gui type specified
  // assume no-gui
  {$define Z_NOGUI}
{$endif}

//----------------------------------------------------------------------

