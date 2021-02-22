
//----------------------------------------------------------------------
// fpc
//----------------------------------------------------------------------


{$asmmode intel}
{$goto on}
{$macro on}
{$mode objfpc}
// {$H+}

//----------------------------------------------------------------------
// config
//----------------------------------------------------------------------

{$define SPH_WIN32}

{.$define SPH_DEBUG}
{.$define SPH_DEBUG_HEAP}
{.$define SPH_DEBUG_CON}

{$define SPH_VST}
{$define SPH_REAPER}

{$define SPH_GUI}
{$define SPH_CAIRO}

{.$define SPH_NOAUTODELETE}       // define to handle all appended objects yourself
{.$define SPH_NOSHRINKBUFFER}     // cairo double buffering, only grow?
{.$define S_SPH_NORESETQUEUEINRESUME} // reset event queue in resume


