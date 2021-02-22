{$include sphaera.inc}
unit s_plugin_base;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_parameter,
  s_program,
  s_rect,
  s_types;

type
  SPluginBase = class
  protected
    FFlags        : LongWord;
    FName         : PChar;
    FAuthor       : PChar;
    FProduct      : PChar;
    FUniqueId     : LongWord;
    FVersion      : LongWord;
    FParameters   : SParameterArray;
    FPrograms     : SProgramArray;
    FNumInputs    : LongWord;
    FNumOutputs   : LongWord;
    FInitialDelay : LongWord;
    FEditorRect   : SRect;
  public
    constructor create;
    destructor  destroy; override;
  public
    function    initialize({%H-}ptr:Pointer) : Pointer; virtual;
    function    getTime(var {%H-}ASampleRate,{%H-}ASamplePos,{%H-}ATempo,{%H-}ABeatPos:Single) : LongInt; virtual;
    procedure   midiOut({%H-}AOffset:integer; {%H-}AMsg1,{%H-}AMsg2,{%H-}AMsg3:byte); virtual;
    procedure   midiFlush; virtual;
    procedure   updateSync; virtual;
    procedure   updateHostInfo; virtual;
  public
    function    appendParameter({%H-}AParameter:SParameter) : LongInt; virtual;
    function    appendProgram({%H-}AProgram:SProgram) : LongInt; virtual;
    procedure   deleteParameters; virtual;
    procedure   deletePrograms; virtual;
  public
    function    getParameters : SParameterArray; virtual;
    function    getParamValues : PSingle; virtual;
    function    getParamValue({%H-}AIndex:LongInt) : Single; virtual;
    procedure   setParamValue({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
    //function    getHostValue({%H-}AIndex:LongInt) : Single; virtual;
    //procedure   setHostValue({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
    //function    getGuiValue({%H-}AIndex:LongInt) : Single; virtual;
    //procedure   setGuiValue({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
    procedure   setDefaultParameters; virtual;
    //procedure   checkParametersInResume; virtual;
    procedure   clearEvents; virtual;
  public
    procedure   updateParameterFromHost({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
    procedure   updateParameterFromEditor({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
    //procedure updateParameterFromProcess(AIndex:LongInt; AValue:Single); virtual;
    procedure   updateParametersInProcess; virtual;
    procedure   updateHostFromEditor({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
  public
    procedure   on_create; virtual;
    procedure   on_destroy; virtual;
    procedure   on_stateChange({%H-}AState:LongInt); virtual;
    procedure   on_transportChange({%H-}AState:LongInt); virtual;
    procedure   on_parameterChange({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual;
    procedure   on_programChange({%H-}AIndex:LongInt); virtual;
    procedure   on_midiEvent({%H-}AOffset:LongWord; {%H-}AMsg1,{%H-}AMsg2,{%H-}AMsg3:Byte); virtual;
    procedure   on_processBlock({%H-}inputs, {%H-}outputs: PPSingle; {%H-}frames: LongInt); virtual;
    procedure   on_processSample({%H-}inputs, {%H-}outputs: PPSingle); virtual;
    //procedure on_processBlock({%H-}inputs, {%H-}outputs: PPDouble; {%H-}frames: LongInt); virtual;
    //procedure on_processSample({%H-}inputs, {%H-}outputs: PPDouble); virtual;
    procedure   on_postProcess; virtual;
    procedure   on_openEditor; virtual;
    procedure   on_closeEditor; virtual;
    procedure   on_idleEditor; virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_const;

//----------

constructor SPluginBase.create;
begin
  inherited;
  FFlags        := spf_none;
  FName         := 'plugin';
  FAuthor       := 'author';
  FProduct      := 'product';
  FUniqueId     := 0;
  FVersion      := 0;
  FParameters   := SParameterArray.create;
  FPrograms     := SProgramArray.create;
  FNumInputs    := 2;
  FNumOutputs   := 2;
  FInitialDelay := 0;
  FEditorRect   := rect(256);

end;

destructor SPluginBase.destroy;
begin
  FParameters.destroy;
  FPrograms.destroy;
  inherited;
end;

//----------

function SPluginBase.initialize({%H-}ptr:Pointer) : Pointer;
begin
  result := nil;
end;

function SPluginBase.getTime(var ASampleRate,ASamplePos,ATempo,ABeatPos:Single) : LongInt;
begin
  result := 0;
end;

procedure SPluginBase.midiOut(AOffset:integer; AMsg1,AMsg2,AMsg3:byte);
begin
end;

procedure SPluginBase.midiFlush;
begin
end;

procedure SPluginBase.updateSync;
begin
end;

procedure SPluginBase.updateHostInfo;
begin
end;

//----------

function SPluginBase.getParameters : SParameterArray;
begin
  result := nil;
end;

//

function SPluginBase.getParamValue(AIndex:LongInt) : Single;
begin
  result := 0;
end;

function SPluginBase.getParamValues : PSingle;
begin
  result := nil;
end;

procedure SPluginBase.setParamValue(AIndex:LongInt; AValue:Single);
begin
end;

{function SPluginBase.getHostValue(AIndex:LongInt) : Single;
begin
  result := 0;
end;}

{procedure SPluginBase.setHostValue(AIndex:LongInt; AValue:Single);
begin
end;}

{function SPluginBase.getGuiValue(AIndex:LongInt) : Single;
begin
  result := 0;
end;}

{procedure SPluginBase.setGuiValue(AIndex:LongInt; AValue:Single);
begin
end;}

//----------

function SPluginBase.appendParameter(AParameter:SParameter) : LongInt;
begin
  result := 0;
end;

procedure SPluginBase.deleteParameters;
begin
end;

function SPluginBase.appendProgram(AProgram:SProgram) : LongInt;
begin
  result := 0;
end;

procedure SPluginBase.deletePrograms;
begin
end;

procedure SPluginBase.setDefaultParameters;
begin
end;

{procedure SPluginBase.checkParametersInResume;
begin
end;}

procedure SPluginBase.clearEvents;
begin
end;

//----------



procedure SPluginBase.updateParameterFromHost(AIndex:LongInt; AValue:Single);
begin
end;

procedure SPluginBase.updateParameterFromEditor(AIndex:LongInt; AValue:Single);
begin
end;

procedure SPluginBase.updateHostFromEditor(AIndex:LongInt; AValue:Single);
begin
end;

//procedure SPluginVst.updateParameterFromProcess(AIndex:LongInt; AValue:Single);
//begin
//end;

procedure SPluginBase.updateParametersInProcess;
begin
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

procedure SPluginBase.on_create;
begin
end;

procedure SPluginBase.on_destroy;
begin
end;

procedure SPluginBase.on_stateChange(AState:LongInt);
begin
end;

procedure SPluginBase.on_transportChange(AState:LongInt);
begin
end;

procedure SPluginBase.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
end;

procedure SPluginBase.on_programChange(AIndex:LongInt);
begin
end;

procedure SPluginBase.on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
begin
end;

procedure SPluginBase.on_processBlock(inputs,outputs:PPSingle; frames:LongInt);
begin
end;

procedure SPluginBase.on_processSample(inputs,outputs:PPSingle);
begin
end;

{procedure SPluginBase.on_processBlock(inputs,outputs:PPDouble; frames:LongInt);
begin
end;}

{procedure SPluginBase.on_processSample(inputs,outputs:PPDouble);
begin
end;}

procedure SPluginBase.on_postProcess;
begin
end;

procedure SPluginBase.on_openEditor;
begin
end;

procedure SPluginBase.on_closeEditor;
begin
end;

procedure SPluginBase.on_idleEditor;
begin
end;

//----------------------------------------------------------------------
end.

