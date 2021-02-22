
//----------------------------------------------------------------------
{$ifdef Z_EXE}
//----------------------------------------------------------------------

begin
  //if zenith_initialize then zenith_prepare;
  zenith_initialize;
  zenith_prepare;
  zenith_main(nil);
end.

{$endif} // Z_EXE
//----------------------------------------------------------------------
{$ifdef Z_LADSPA}
//----------------------------------------------------------------------

// TODO

{$endif} // Z_LADSPA
//----------------------------------------------------------------------
{$ifdef Z_VST}
//----------------------------------------------------------------------

// function main(audioMaster:TAudioMasterCallbackFunc): PAEffect; cdecl; export;
function main(APointer:pointer): pointer; cdecl; export;
begin
  if zenith_initialize then zenith_prepare;
  result := zenith_main(APointer);
end;

exports main; // VSTPluginMain

//begin
end.

{
  Note that the exported function should now be called VstPluginMain instead of
  Main, though Steinberg seem to provide a Main stub function as well in their
  examples.
}

{$endif} // Z_VST
//----------------------------------------------------------------------

