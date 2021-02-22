unit test1;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

procedure zenith_prepare;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses

  zenith,
  z_types,
  z_const,
  z_util,
  //z_rect,
  z_plugin_base,  // ZEditorBase
  z_descriptor,
  z_instance,
  z_editor,
  z_parameter;

//----------

type

  myDescriptor = class(ZDescriptor)
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
  end;

  myInstance = class(ZInstance)
  public
    procedure   on_closeEditor; override;
    procedure   on_create; override;
    procedure   on_destroy; override;
  //procedure   on_idleEditor; virtual;
    procedure   on_midiEvent({%H-}AOffset:LongWord; {%H-}AMsg1,{%H-}AMsg2,{%H-}AMsg3:Byte); override;
    procedure   on_openEditor({%H-}AEditor:ZEditorBase); override;
    procedure   on_parameterChange({%H-}AIndex:LongInt; {%H-}AValue:Single); override;
    procedure   on_postProcess; override;
    procedure   on_processBlock({%H-}AInputs,{%H-}AOutputs:PPZSample; {%H-}ALength:LongWord); override;
    procedure   on_processSample({%H-}AInputs,{%H-}AOutputs:PPZSample); override;
    procedure   on_stateChange({%H-}AState:LongWord); override;
    //procedure   on_transportChange({%H-}ATransport:LongWord); override;
  end;

  myEditor = class(ZEditor)
  public
    procedure   on_create; override;
    procedure   on_destroy; override;
  end;

//--------------------------------------------------
// descriptor
//--------------------------------------------------

procedure myDescriptor.on_create;
begin
  //writeln('myDescriptor.on_create');
  FName     := 'test1';
  FAuthor   := 'th.skei';
  FProduct  := 'zenith test plugin';
  FUniqueId := Z_MAGIC + $dead;
  ZSetFlag(FFlags,(
    //zpf_isSynth +
    zpf_hasEditor +
    zpf_receiveMidi +
    zpf_sendMidi
  ));
  appendParameter( ZParameter.create('p1',0) );
end;

procedure myDescriptor.on_destroy;
begin
  //writeln('myDescriptor.on_destroy');
end;

//--------------------------------------------------
// instance
//--------------------------------------------------

procedure myInstance.on_create;
begin
  //FEditorRect := rect(256,256);
  //writeln('myInstance.on_create');
end;

procedure myInstance.on_destroy;
begin
  //writeln('myInstance.on_destroy');
end;

procedure myInstance.on_stateChange(AState:LongWord);
begin
  //writeln('myInstance.on_stateChange');
end;

//procedure myInstance.on_transportChange(ATransport:LongWord);
//begin
//  //writeln('myInstance.on_transportChange');
//end;

// process thread

procedure myInstance.on_parameterChange(AIndex:LongInt; AValue:Single);
begin
  //writeln('myInstance.on_parameterChange');
end;

procedure myInstance.on_midiEvent(AOffset:LongWord; AMsg1,AMsg2,AMsg3:Byte);
begin
  //writeln('myInstance.on_midiEvent');
end;

procedure myInstance.on_processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
begin
  //writeln('myInstance.on_processBlock');
end;

procedure myInstance.on_processSample(AInputs,AOutputs:PPZSample);
begin
  AOutputs[0]^ := AInputs[0]^;
  AOutputs[1]^ := AInputs[1]^;
  //writeln('myInstance.on_processSample');
end;

procedure myInstance.on_postProcess;
begin
  //writeln('myInstance.on_postProcess');
end;

// gui thread

procedure myInstance.on_openEditor(AEditor:ZEditorBase);
begin
  //writeln('myInstance.on_openEditor');
end;

procedure myInstance.on_closeEditor;
begin
  //writeln('myInstance.on_closeEditor');
end;

{procedure myInstance.on_idleEditor;
begin
  writeln('myInstance.on_idleEditor');
end;}

//--------------------------------------------------
// editor
//--------------------------------------------------

procedure myEditor.on_create;
begin
  //writeln('myEditor.on_create');
end;

procedure myEditor.on_destroy;
begin
  //writeln('myEditor.on_destroy');
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

procedure zenith_prepare;
begin
  //writeln('zenith_prepare');
  zenith_register(myDescriptor);
  zenith_register(myInstance);
  zenith_register(myEditor);
end;

//----------------------------------------------------------------------
end.

