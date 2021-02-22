{$include sphaera.inc}
unit s_plugin;

{
  idea: thread that simulates the old 'idle'..
        check if process is being called, either by testing playstate, or
        checking if some frame counter is increasing..
}

{$define S_MAXNUMPARAMS  := 1024}
{$define S_HOSTQUEUESIZE := 1024}
{$define S_GUIQUEUESIZE  := 1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses

  //s_console,
  s_debug,
  s_eventqueue,
  s_parameter,
  s_program,
  {$ifdef SPH_VST} s_plugin_vst; {$endif}

//----------

type

  SPlugin = class(SPluginImpl)
  private
    FParamValues  : array[0..S_MAXNUMPARAMS-1] of Single;
    FHostValues   : array[0..S_MAXNUMPARAMS-1] of Single;
    //FGuiValues    : array[0..S_MAXNUMPARAMS-1] of Single;
    FHostEvents   : SEventQueue;
    FGuiEvents    : SEventQueue;
  public
    constructor create;
    destructor  destroy; override;
    function    appendParameter(AParameter:SParameter) : LongInt; override;
    function    appendProgram(AProgram:SProgram) : LongInt; override;
    procedure   deleteParameters; override;
    procedure   deletePrograms; override;
    function    getParameters : SParameterArray; override;
    function    getParamValue(AIndex:LongInt) : Single; override;
    procedure   setParamValue(AIndex:LongInt; AValue:Single); override;
    //function    getHostValue(AIndex:LongInt) : Single; override;
    //procedure   setHostValue(AIndex:LongInt; AValue:Single); override;
    //function    getGuiValue(AIndex:LongInt) : Single; override;
    //procedure   setGuiValue(AIndex:LongInt; AValue:Single); override;
    procedure   setDefaultParameters; override;
    //procedure   checkParametersInResume; override;
    procedure   clearEvents; override;
  public
    procedure   updateParameterFromHost(AIndex:LongInt; AValue:Single); override;
    procedure   updateParameterFromEditor(AIndex:LongInt; AValue:Single); override;
    //procedure updateParameterFromProcess(AIndex:LongInt; AValue:Single); override;
    procedure   updateParametersInProcess; override;
    //
  public
    procedure on_openEditor; override;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const,
  s_memory,
  s_util;

//----------

constructor SPlugin.create;
begin
  inherited;
  SMemset(@FParamValues,0,sizeof(FParamValues));
  SMemset(@FHostValues,0,sizeof(FHostValues));
  //SMemset(@FGuiValues,0,sizeof(FGuiValues));
  FHostEvents := SEventQueue.create(S_HOSTQUEUESIZE);
  FGuiEvents := SEventQueue.create(S_GUIQUEUESIZE);
end;

//----------

destructor SPlugin.destroy;
begin
  {$ifndef SPH_NOAUTODELETE}
    deleteParameters;
    deleteprograms;
  {$endif}
  FHostEvents.destroy;
  FGuiEvents.destroy;
  inherited;
end;


//----------

function SPlugin.appendParameter(AParameter:SParameter) : LongInt;
var
  i : longint;
begin
  i := FParameters.size;
  AParameter.setIndex(i);
  FParameters.append(AParameter);
  result := i;
end;

//----------

procedure SPlugin.deleteParameters;
var i : longint;
begin
  if FParameters.size > 0 then
  begin
    for i := 0 to FParameters.size-1 do FParameters[i].destroy;
  end;
end;

//----------

function SPlugin.appendProgram(AProgram:SProgram) : LongInt;
var
  i : longint;
begin
  i := FPrograms.size;
  AProgram.setIndex(i);
  FPrograms.append(AProgram);
  result := i;
end;

//----------

procedure SPlugin.deletePrograms;
var i : longint;
begin
  if FPrograms.size > 0 then
  begin
    for i := 0 to FPrograms.size-1 do FPrograms[i].destroy;
  end;
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

function SPlugin.getParameters : SParameterArray;
begin
  result := FParameters;
end;


function SPlugin.getParamValue(AIndex:LongInt) : Single;
begin
  result := FParamValues[AIndex];
end;

//----------

procedure SPlugin.setParamValue(AIndex:LongInt; AValue:Single);
begin
  FParamValues[AIndex] := AValue;
end;

//----------

{function SPlugin.getHostValue(AIndex:LongInt) : Single;
begin
  result := FHostValues[AIndex];
end;}

//----------

{procedure SPlugin.setHostValue(AIndex:LongInt; AValue:Single);
begin
  FHostValues[AIndex] := AValue;
end;}

//----------

{function SPlugin.getGuiValue(AIndex:LongInt) : Single;
begin
  result := FGuiValues[AIndex];
end;}

//----------

{procedure SPlugin.setGuiValue(AIndex:LongInt; AValue:Single);
begin
  FGuiValues[AIndex] := AValue;
end;}

//----------

{
  sets default parameter values, and notifies each parameter
}

procedure SPlugin.setDefaultParameters;
var
  param : SParameter;
  index : LongInt;
  value : Single;
begin
  if FParameters.size > 0 then
  begin
    for index := 0 to FParameters.size-1 do
    begin
      param := FParameters[index];
      value := param.getDefault;
      setParamValue(index,value);
      //setHostValue(index,value);
      //setGuiValue(index,value);
      on_parameterChange(index, param.from01(value));
    end;
  end;
end;

//----------

{
  if processReplacing is not called, but _processing flag is on,
  internal parameeter value might not be updated..
  check all, and update the ones with a difference..
}

{procedure SPlugin.checkParametersInResume;
var
  param : SParameter;
  index : LongInt;
  value : Single;
begin
  STrace('check params');
  if FParameters.size > 0 then
  begin
    for index := 0 to FParameters.size-1 do
    begin
      _editor.checkParameter(FParameters[index],FParamValues[index]);
    end;
  end;
end;}

//----------

procedure SPlugin.clearEvents;
begin
  FHostEvents.clear;
  FGuiEvents.clear;
end;

//----------------------------------------------------------------------
// updateParameter...
//----------------------------------------------------------------------

{
  vst_setparameter_callback
    SPluginVst.vst_setParameter

  pause: FProcessing = true (resume/bypass) but processReplacing not called?

  idea:
  - we change FParamValues directly, and let the plugin know that the parameter
    changed. we don't pass the parameter value, only the index..
    the plugin reads the value when it wants it..

  should setParamValue be called here or in vst_setParameter?
}

procedure SPlugin.updateParameterFromHost(AIndex:LongInt; AValue:Single);
var
  param : SParameter;
  event : SEvent;
  value : Single;
begin
  //STrace('SPlugin.updateParameterFromHost');

  //setParamValue(AIndex,AValue); // already done (vst_setParameter)

  { notify plugin }
  if _processing then
  begin
    event.FIndex := AIndex;
    event.FValue := AValue;
    FHostEvents.write(event);
  end
  else
  begin
    //setParamValue(AIndex,AValue);
    value := FParameters[AIndex].from01(AValue);
    on_parameterChange(AIndex,value);
  end;

  { notify editor }
  {$ifdef SPH_GUI}
  if _editor.isOpen then
  begin
    param := FParameters[AIndex];
    _editor.updateParameterFromHost(param,AValue);
  end;
  {$endif}

end;

//----------

{
  SEditorWindow.do_update
    SEditor.updateParameterFromWidget

  * todo: if processing is not being called, and _processing flag is not
    set correctly (reaper, pause, doesn't call stopprocess, or suspend, unless
    rec enabled track..

}

procedure SPlugin.updateParameterFromEditor(AIndex:LongInt; AValue:Single);
{$ifdef SPH_GUI}
var
  event : SEvent;
  value : Single;
{$endif}
begin
  //STrace('SPlugin.updateParameterFromEditor');
  {$ifdef SPH_GUI}
  //setGuiValue(AIndex,AValue);

  { notify plugin }
  if _processing then
  begin
    event.FIndex := AIndex;
    event.FValue := AValue;
    FGuiEvents.write(event);
  end
  else
  begin
    setParamValue(AIndex,AValue);
    value := FParameters[AIndex].from01(AValue);
    on_parameterChange(AIndex,value);
  end;

  { notify host}
  if SHasFlag(FFlags,spf_automate) then
  begin
    setParamValue(AIndex,AValue);
    updateHostFromEditor(AIndex,AValue);
  end;
  {$endif}
end;

//----------

{procedure SPlugin.updateParameterFromProcess(AIndex:LongInt; AValue:Single);
begin
  //setParameterValue(AIndex,AValue);
  //on_parameterChange(AIndex,AValue);
  //FHost.updateParameter(AIndex,AValue);
end;}

//----------

{
  vst_process_callback
    SPluginVst.vst_process

  if processReplacing isn't called, this will not be called
  and parameters will not be updated..

  todo: meaning: 'you need to update your values'
}

procedure SPlugin.updateParametersInProcess;
var
  event : SEvent;
  index : LongInt;
  value : Single;
begin
  //STrace('SPlugin.updateParametersInProcess');

  { events from editor - widget.on_change }
  while FGuiEvents.read(event{%H-}) do
  begin
    STrace('SPlugin.updateParametersInProcess - gui event');
    index := event.FIndex;
    value := event.FValue; // getGuiValue(index);
    //if value <> getParamValue(index) then
    begin
      //setParamValue(index,value);
      value := FParameters[index].from01(value);
      on_parameterChange(index,value);
    end;
  end;

  { events from host - setParameter }
  while FHostEvents.read(event{%H-}) do
  begin
    STrace('SPlugin.updateParametersInProcess - host event');
    index := event.FIndex;
    value := event.FValue; // getHostValue(index);
    //if value <> getParamValue(index) then
    begin
      //setParamValue(index,value);
      value := FParameters[index].from01(value);
      on_parameterChange(index,value);
    end;
  end;

end;

//----------------------------------------------------------------------

{
  if the plugin has the spf_hasEditor flag, but doesn't override this
  method, we setup a basic editor..
}

procedure SPlugin.on_openEditor;
begin
  if SHasFlag(FFlags,spf_hasEditor) then _editor.setupDefaultWidgets;
end;

//----------------------------------------------------------------------
end.
