unit z_instance;
{$include zenith.inc}

{$define Z_HOSTTOAUDIO_QUEUESIZE:=1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_EXE}    z_instance_exe,    {$endif}
  {$ifdef Z_LADSPA} z_instance_ladspa, {$endif}
  {$ifdef Z_VST}    z_instance_vst,    {$endif}
  z_plugin_base,
  z_parameter,
  //z_program,
  z_events;

type

  ZInstance = class(ZInstanceImpl)
  private
    FHostToAudio: ZEventQueue;
  public
    constructor create(AHost:ZHostBase; ADescriptor:ZDescriptorBase);
    destructor  destroy; override;
  public
    function    getEditor : ZEditorBase; override;
    function    getFlags : LongWord; override;
    function    getParameter(AIndex:LongWord) : ZParameter; override;
    function    getParameters : ZParameterArray; override;
    function    getParameterValue(AIndex:LongWord) : Single; override;
    procedure   setDefaultParameters; override;
    procedure   setParameterValue(AIndex:LongWord; AValue:Single); override;
  //procedure   updateParameterFromEditor({%H-}AIndex:LongInt; {%H-}AValue:Single); override;
    procedure   updateParameterFromHost(AIndex:LongInt; AValue:Single); override; // automation
    procedure   updateParameterFromProcess({%H-}AIndex:LongInt; {%H-}AValue:Single); override; // from audio
    procedure   updateParametersInProcess; override;

  end;

  ZInstanceClass = class of ZInstance;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  zenith;

constructor ZInstance.create(AHost:ZHostBase; ADescriptor:ZDescriptorBase);
begin
  inherited;
  FDescriptor := ADescriptor;
  FEditor :=  zenith_format.editorClass.create(self); //
  FFlags := ADescriptor.getFlags;
  FHost := AHost;
  FParameters := ADescriptor.getParameters;
  SetLength(FParameterValues,FParameters.size);
  FProcessing := false;
  //
  FHostToAudio := ZEventQueue.create(Z_HOSTTOAUDIO_QUEUESIZE);  // automation
  //FAudioToHost := ZEventQueue.create(Z_AUDIOTOHOST_QUEUESIZE);  // parameter-changes (?)
  //FAudioToGui  := ZEventQueue.create(Z_AUDIOTOHOST_QUEUESIZE);   // vu-meter
  //FGuiToHost   := ZEventQueue.create(Z_GUITOHOST_QUEUESIZE);    // knob-tweaking
  //
  FEditor.on_create;
end;

//----------

destructor ZInstance.destroy;
begin
  FEditor.on_destroy;
  FEditor.destroy;
  FHostToAudio.destroy;
  SetLength(FParameterValues,0);
  inherited;
end;

//----------------------------------------
//
//----------------------------------------

function ZInstance.getEditor : ZEditorBase;
begin
  result := FEditor;
end;

//----------

function ZInstance.getFlags : LongWord;
begin
  result := FFlags;
end;

//----------

function ZInstance.getParameter(AIndex:LongWord) : ZParameter;
begin
  result := FParameters[AIndex];
end;

//----------

function ZInstance.getParameters : ZParameterArray;
begin
  result := FParameters;
end;

//----------

function ZInstance.getParameterValue(AIndex:LongWord) : Single;
begin
  result := FParameterValues[AIndex];
end;

//----------

procedure ZInstance.setDefaultParameters;
var i : longint;
    p : ZParameter;
    v : Single;
begin
  if FParameters.size > 0 then
  begin
    for i := 0 to FParameters.size-1 do
    begin
      // TODO
      p := FParameters[i];
      v := p.getDefault;
      //updateParameterFromHost(i,v);
      setParameterValue(i,v);
      on_parameterChange(i,v);
    end;
  end;
end;

//----------

procedure ZInstance.setParameterValue(AIndex:LongWord; AValue:Single);
begin
  FParameterValues[AIndex] := AValue;
end;

//----------

{procedure ZInstance.updateParameterFromEditor(AIndex:LongInt; AValue:Single);
begin
  //todo: queue
  //FHost.updateParameter(AIndex,AValue);
  //setParameterValue(AIndex,AValue);
  //on_parameterChange(AIndex,AValue);
end;}

//----------

procedure ZInstance.updateParameterFromHost(AIndex:LongInt; AValue:Single);
var
  event : ZEvent;
begin
  if FProcessing then
  begin
    event.FIndex := AIndex;
    event.FValue := AValue;
    FHostToAudio.write(event);
  end
  else
  begin
    on_parameterChange(AIndex,AValue);
  end;
  //if FEditorOpen and Assigned(FEditor) then FEditor.updateParameterFromHost(AIndex,AValue);
  if FEditor.isOpen then FEditor.updateParameterFromHost(AIndex,AValue);
end;

//----------

procedure ZInstance.updateParameterFromProcess(AIndex:LongInt; AValue:Single);
begin
  //setParameterValue(AIndex,AValue);
  //on_parameterChange(AIndex,AValue);
  FHost.updateParameter(AIndex,AValue);
end;

//----------

procedure ZInstance.updateParametersInProcess;
var
  event : ZEvent;
begin
  while FHostToAudio.read(event{%H-}) do
  begin
    on_parameterChange(event.FIndex,event.FValue);
  end;
end;


//----------------------------------------------------------------------
end.

