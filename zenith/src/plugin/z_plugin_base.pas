unit z_plugin_base;

{$include zenith.inc}

// {$define Z_HOSTTOAUDIO_QUEUESIZE:=1024}
// {$define Z_AUDIOTOHOST_QUEUESIZE:=1024}
// {$define Z_GUITOAUDIO_QUEUESIZE:=256}
// {$define Z_AUDIOTOGUI_QUEUESIZE:=256}
// {$define Z_EDITORQUEUE_SIZE:=256}
// {$define Z_PROCESSQUEUE_SIZE:=1024}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_types,
  z_object,
  z_rect,
  z_parameter,
  z_program;

type

  ZDescriptorBase = class;
  ZInstanceBase = class;
  ZEditorBase = class;
  ZHostBase = class;

  //----------------------------------------
  // descriptor
  //----------------------------------------

  ZDescriptorBase = class(ZObject)
  protected
    FName       : PChar;
    FAuthor     : PChar;
    FProduct    : PChar;
    FUniqueId   : longword;
    FVersion    : longword;
    FFlags      : longword;
    FNumInputs  : longword;
    FNumOutputs : longword;
    FParameters : ZParameterArray;
    FPrograms   : ZProgramArray;
    FLatency    : longword;
  public
    procedure   appendParameter(AParameter:ZParameter); virtual; abstract;
    procedure   deleteParameters; virtual; abstract;
    function    getAuthor: PChar; virtual; abstract;
    function    getFlags: longword; virtual; abstract;
    function    getLatency: longword; virtual; abstract;
    function    getName: PChar; virtual; abstract;
    function    getNumInputs: longword; virtual; abstract;
    function    getNumOutputs: longword; virtual; abstract;
    function    getNumParameters: longword; virtual; abstract;
    function    getNumPrograms: longword; virtual; abstract;
    function    getParameter(AIndex:longword): ZParameter; virtual; abstract;
    function    getParameters: ZParameterArray; virtual; abstract;
    function    getProduct: PChar; virtual; abstract;
    function    getPrograms: ZProgramArray; virtual; abstract;
    function    getUniqueId: longword; virtual; abstract;
    function    getVersion: longword; virtual; abstract;
  end;

  //----------------------------------------
  // instance
  //----------------------------------------

  ZParameterValues = array of single; // ZSample
  PParameterValues = ^ZParameterValues;

  //----------

  ZInstanceBase = class(ZObject)
  protected
    FDescriptor     : ZDescriptorBase;
    FEditor         : ZEditorBase;
    FFlags          : longword;
    FHost           : ZHostBase;
    FParameters     : ZParameterArray;
    FParameterValues: ZParameterValues;
    FProcessing     : boolean;
  public
    function  getEditor : ZEditorBase; virtual; abstract;
    function  getEffect: pointer; virtual; abstract;
    function  getFlags: longword; virtual; abstract;
    function  getParameter(AIndex: longword): ZParameter; virtual; abstract;
    function  getParameters: ZParameterArray; virtual; abstract;
    function  getParameterValue(AIndex: longword): single; virtual; abstract;
    procedure setDefaultParameters; virtual; abstract;
    procedure setParameterValue(AIndex: longword; AValue: single); virtual; abstract;
  //procedure updateParameterFromEditor({%H-}AIndex:LongInt; {%H-}AValue:Single); virtual; abstract;
    procedure updateParameterFromHost(AIndex: longint; AValue: single); virtual; abstract;// automation
    procedure updateParameterFromProcess({%H-}AIndex: longint; {%H-}AValue: single); virtual; abstract;// from audio
    procedure updateParametersInProcess; virtual; abstract;
  public
    procedure on_closeEditor; virtual;
    procedure on_idleEditor; virtual;
    procedure on_midiEvent({%H-}AOffset: longword; {%H-}AMsg1,{%H-}AMsg2,{%H-}AMsg3: byte); virtual;
    procedure on_openEditor({%H-}AEditor: ZEditorBase); virtual;
    procedure on_parameterChange({%H-}AIndex: longint; {%H-}AValue: single); virtual;
    procedure on_programChange; virtual;
    procedure on_postProcess; virtual;
    procedure on_processBlock({%H-}AInputs,{%H-}AOutputs: PPZSample; {%H-}ALength: longword); virtual;
    procedure on_processSample({%H-}AInputs,{%H-}AOutputs: PPZSample); virtual;
    procedure on_stateChange({%H-}AState: longword); virtual;
    //procedure   on_transportChange({%H-}ATransport:LongWord); virtual;
  end;

  //----------------------------------------
  // editor
  //----------------------------------------

  ZEditorBase = class(ZObject)
  protected
    FInstance : ZInstanceBase;
    FRect     : ZRect;
    FIsOpen   : boolean;
  public
    function  getRect: ZRect; virtual; abstract;
    function  isOpen: boolean; virtual; abstract;
    procedure open({%H-}AParent: Pointer); virtual; abstract;
    procedure close; virtual; abstract;
    procedure idle; virtual; abstract;
    procedure eventLoop; virtual; abstract;
    procedure updateParameterFromHost({%H-}AIndex: longint; {%H-}AValue: single); virtual; abstract;
    procedure updateParameterFromWidget({%H-}AIndex: longint; {%H-}AValue: single); virtual; abstract;
  public
    procedure on_open; virtual;
    procedure on_close; virtual;
    procedure on_idle; virtual;
  end;

  //----------------------------------------
  // host
  //----------------------------------------

  ZHostBase = class(ZObject)
  public
    function  getEffect: Pointer; virtual; abstract;
    function  getMaster: Pointer; virtual; abstract;
    function  getTime(var ASampleRate, ASamplePos, ATempo, ABeatPos: single): longint; virtual; abstract;
    procedure midiOut({%H-}AOffset: integer; {%H-}AMsg1,{%H-}AMsg2,{%H-}AMsg3: byte); virtual; abstract;
    procedure midiFlush; virtual; abstract;
    procedure setEffect({%H-}ptr: Pointer); virtual; abstract;
    procedure setMaster({%H-}ptr: Pointer); virtual; abstract;
    procedure updateParameter({%H-}param: longint; {%H-}val: single); virtual; abstract;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  zenith;

//----------------------------------------------------------------------

procedure ZInstanceBase.on_closeEditor;
begin
end;

procedure ZInstanceBase.on_idleEditor;
begin
end;

procedure ZInstanceBase.on_midiEvent(AOffset: longword; AMsg1, AMsg2, AMsg3: byte);
begin
end;

procedure ZInstanceBase.on_openEditor(AEditor: ZEditorBase);
begin
end;

procedure ZInstanceBase.on_parameterChange(AIndex: longint; AValue: single);
begin
end;

procedure ZInstanceBase.on_postProcess;
begin
end;

procedure ZInstanceBase.on_programChange;
begin
end;

procedure ZInstanceBase.on_processBlock(AInputs, AOutputs: PPZSample; ALength: longword);
begin
end;

procedure ZInstanceBase.on_processSample(AInputs, AOutputs: PPZSample);
begin
end;

procedure ZInstanceBase.on_stateChange(AState: longword);
begin
end;

{procedure ZInstanceBase.on_transportChange(ATransport:LongWord);
begin
end;}

//----------------------------------------------------------------------

procedure ZEditorBase.on_open;
begin
end;

procedure ZEditorBase.on_close;
begin
end;

procedure ZEditorBase.on_idle;
begin
end;

//----------------------------------------------------------------------
end.

