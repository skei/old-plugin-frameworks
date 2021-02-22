const char* s3_vst_dispatcher_opcode_names[] = {
  "effOpen",
  "effClose",
  "effSetProgram",
  "effGetProgram",
  "effSetProgramName",
  "effGetProgramName",
  "effGetParamLabel",
  "effGetParamDisplay",
  "effGetParamName",
  "effGetVu (deprecated)",
  "effSetSampleRate",
  "effSetBlockSize",
  "effMainsChanged",
  "effEditGetRect",
  "effEditOpen",
  "effEditClose",
  "effEditDraw (deprecated)",
  "effEditMouse (deprecated)",
  "effEditKey (deprecated)",
  "effEditIdle",
  "effEditTop (deprecated)",
  "effEditSleep (deprecated)",
  "effIdentify (deprecated)",
  "effGetChunk",
  "effSetChunk",
  // vst 2
  "effProcessEvents",
  "effCanBeAutomated",
  "effString2Parameter",
  "effGetNumProgramCategories (deprecated)",
  "effGetProgramNameIndexed",
  "effCopyProgram (deprecated)",
  "effConnectInput (deprecated)",
  "effConnectOutput (deprecated)",
  "effGetInputProperties",
  "effGetOutputProperties",
  "effGetPlugCategory",
  "effGetCurrentPosition (deprecated)",
  "effGetDestinationBuffer (deprecated)",
  "effOfflineNotify",
  "effOfflinePrepare",
  "effOfflineRun",
  "effProcessVarIo",
  "effSetSpeakerArrangement",
  "effSetBlockSizeAndSampleRate (deprecated)",
  "effSetBypass",
  "effGetEffectName",
  "effGetErrorText (deprecated)",
  "effGetVendorString",
  "effGetProductString",
  "effGetVendorVersion",
  "effVendorSpecific",
  "effCanDo",
  "effGetTailSize",
  "effIdle (deprecated)",
  "effGetIcon (deprecated)",
  "effSetViewPosition (deprecated)",
  "effGetParameterProperties",
  "effKeysRequired (deprecated)",
  "effGetVstVersion",
  // vst 2.1
  "effEditKeyDown",
  "effEditKeyUp",
  "effSetEditKnobMode",
  "effGetMidiProgramName",
  "effGetCurrentMidiProgram",
  "effGetMidiProgramCategory",
  "effHasMidiProgramsChanged",
  "effGetMidiKeyName",
  "effBeginSetProgram",
  "effEndSetProgram",
  // vst 2.3
  "effGetSpeakerArrangement",
  "effShellGetNextPlugin",
  "effStartProcess",
  "effStopProcess",
  "effSetTotalSampleToProcess",
  "effSetPanLaw",
  "effBeginLoadBank",
  "effBeginLoadProgram",
  // vst 2.4
  "effSetProcessPrecision",
  "effGetNumMidiInputChannels",
  "effGetNumMidiOutputChannels"
};