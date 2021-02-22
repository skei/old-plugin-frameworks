unit z_format_vst;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_descriptor,
  z_instance,
  z_editor;

type

  ZFormatVst = class
  private
    FDescriptorClass  : ZDescriptorClass;
    FInstanceClass    : ZInstanceClass;
    FEditorClass      : ZEditorClass;
    FDescriptor       : ZDescriptor;
  public
    property  descriptorClass : ZDescriptorClass read FDescriptorClass write FDescriptorClass;
    property  instanceClass   : ZInstanceClass read FInstanceClass write FInstanceClass;
    property  editorClass     : ZEditorClass read FEditorClass write FEditorClass;
  public
    constructor create;
    destructor  destroy; override;
    procedure   initialize;
    function    entryPoint(ptr:Pointer) : Pointer;
  end;

  ZFormatImpl = ZFormatVst;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  DAEffect,
  //DAEffectX,
  z_host,
  z_const,
  z_util;

//----------------------------------------------------------------------
// vst callbacks
//----------------------------------------------------------------------

function z_vst_dispatcher_callback(effect:PAEffect; opcode,index:VstInt32; value:VstIntPtr; ptr:pointer; opt:single): VstIntPtr; cdecl;
var
  inst : ZInstance;
begin
  result := 0;
  inst := ZInstance( effect^.vObject );
  if assigned(inst) then
  begin
    if opCode = effClose then
    begin
      inst.vst_dispatcher(opcode,index,value,ptr,opt);
      inst.on_destroy;
      inst.destroy;
      exit(1);
    end
    else
      result := inst.vst_dispatcher(opcode,index,value,ptr,opt);
  end;
end;

//----------

procedure z_vst_process_callback(effect:PAEffect; inputs,outputs:PPSingle; sampleframes:VstInt32); cdecl;
var
  inst : ZInstance;
begin
  inst := ZInstance( effect^.vObject );
  if assigned(inst) then inst.vst_process(inputs,outputs,sampleframes);
end;

//----------

procedure z_vst_processdouble_callback(effect:PAEffect; inputs,outputs:PPDouble; sampleFrames:VstInt32); cdecl;
var
  inst : ZInstance;
begin
  inst := ZInstance( effect^.vObject );
  if assigned(inst) then inst.vst_processdouble(inputs,outputs,sampleframes);
end;

//----------

procedure z_vst_setparameter_callback(effect:PAEffect; index:VstInt32; value:single); cdecl;
var
  inst : ZInstance;
begin
  inst := ZInstance( effect^.vObject );
  if assigned(inst) then inst.vst_setparameter(index,value);
end;

//----------

function z_vst_getparameter_callback(effect:PAEffect; index:VstInt32): single; cdecl;
var
  inst : ZInstance;
begin
  inst := ZInstance( effect^.vObject );
  result := 0;
  if assigned(inst) then result := inst.vst_getparameter(index);
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

constructor ZFormatVst.create;
begin
  inherited;
  FDescriptorClass := ZDescriptor;
  FInstanceClass := ZInstance;
  FEditorClass := ZEditor;
  FDescriptor := nil;
end;

//----------

destructor ZFormatVst.destroy;
begin
  if Assigned(FDescriptor) then
  begin
    FDescriptor.on_destroy;
    FDescriptor.destroy;
  end;
  inherited;
end;

//----------

procedure ZFormatVst.initialize;
begin
  // called from start of main(), from zenith_initialize
end;

//----------

function ZFormatVst.entryPoint(ptr:Pointer) : Pointer;
var
  host  : ZHost;
  inst  : ZInstance;
  pae   : PAEffect;
  flags : LongWord;
begin
  if not Assigned(FDescriptor) then
  begin
    FDescriptor := FDescriptorClass.create(ptr);
    FDescriptor.on_create;
  end;
  host := ZHost.create(ptr); // deleted in ZDescriptor.destroy
  inst := FInstanceClass.create(host,FDescriptor); // destroyed in z_vst_dispatcher_callback (effClose)
  pae := inst.getEffect;
  host.setEffect(pae{inst.getPAEffect});
  inst.on_create;
  flags := effFlagsCanReplacing;
  if ZHasFlag(FDescriptor.getFlags,zpf_hasEditor) then flags += effFlagsHasEditor;
  if ZHasFlag(FDescriptor.getFlags,zpf_isSynth) then flags += effFlagsIsSynth;
  pae^.magic                  := ZFourChar('VstP');
  pae^.uniqueID               := FDescriptor.getUniqueId;
  pae^.version                := FDescriptor.getVersion;
  pae^.flags                  := flags;
  pae^.numParams              := FDescriptor.getNumParameters;
  pae^.numPrograms            := FDescriptor.getNumPrograms;
  pae^.numInputs              := FDescriptor.getNumInputs;
  pae^.numOutputs             := FDescriptor.getNumOutputs;
  pae^.initialDelay           := FDescriptor.getLatency;
  pae^.dispatcher             := @z_vst_dispatcher_callback;
  pae^.setParameter           := @z_vst_setparameter_callback;
  pae^.getParameter           := @z_vst_getparameter_callback;
  pae^.processReplacing       := @z_vst_process_callback;
  pae^.processDoubleReplacing := @z_vst_processdouble_callback;
  pae^.process                := nil; // (deprecated)
  pae^.vObject                := inst;
  pae^.user                   := nil;// self; pae?
  result := pae;
end;

//----------------------------------------------------------------------
end.

