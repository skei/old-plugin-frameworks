

// function main(audioMaster:TAudioMasterCallbackFunc): PAEffect; cdecl; export;
function main(ptr: Pointer): Pointer; cdecl; export;
var
  plugin  : SPHAERA_PLUGIN;
begin
  plugin := SPHAERA_PLUGIN.create;
  result := plugin.initialize(ptr);
end;

//----------

exports main; // VSTPluginMain

