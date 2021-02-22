unit z_descriptor;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  {$ifdef Z_EXE}    z_descriptor_exe,    {$endif}
  {$ifdef Z_LADSPA} z_descriptor_ladspa, {$endif}
  {$ifdef Z_VST}    z_descriptor_vst,    {$endif}
  z_const,
  z_parameter,
  z_program;

  //----------

type

  ZDescriptor = class(ZDescriptorImpl)
  public
    constructor create(ptr:Pointer);
    destructor  destroy; override;
  public
    { ZDescriptorBase }
    procedure   appendParameter(AParameter:ZParameter); override;
    procedure   deleteParameters; override;
    function    getAuthor : PChar; override;
    function    getFlags : LongWord; override;
    function    getLatency : LongWord; override;
    function    getName : PChar; override;
    function    getNumInputs : LongWord; override;
    function    getNumOutputs : LongWord; override;
    function    getNumParameters : LongWord; override;
    function    getNumPrograms : LongWord; override;
    function    getParameter(AIndex:LongWord) : ZParameter; override;
    function    getParameters : ZParameterArray; override;
    function    getProduct : PChar; override;
    function    getPrograms : ZProgramArray; override;
    function    getUniqueId : LongWord; override;
    function    getVersion : LongWord; override;
  end;

  //----------

  ZDescriptorClass = class of ZDescriptor;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZDescriptor.create(ptr:Pointer);
begin
  inherited;
  FAuthor     := 'author';
  FFlags      := zpf_none;
  FLatency    := 0;
  FName       := 'name';
  FNumInputs  := 2;
  FNumOutputs := 2;
  FParameters := ZParameterArray.create;
  FProduct    := 'product';
  FPrograms   := ZProgramArray.create;
  FUniqueId   := 0; //Z_MAGIC;
  FVersion    := 0;
end;

//----------

destructor ZDescriptor.destroy;
begin
  {$ifndef Z_NOAUTODELETE} deleteParameters; {$endif}
  FParameters.destroy;
  FPrograms.destroy;
  inherited;
end;

//----------------------------------------
//
//----------------------------------------

procedure ZDescriptor.appendParameter(AParameter:ZParameter);
begin
  AParameter.setIndex( FParameters.size );
  FParameters.append(AParameter);
end;

//----------

procedure ZDescriptor.deleteParameters;
var i : longint;
begin
  if FParameters.size > 0 then
  begin
    for i := 0 to FParameters.size-1 do FParameters[i].destroy;
  end;
end;

//----------

function ZDescriptor.getAuthor : PChar;
begin
  result := FAuthor;
end;

//----------

function ZDescriptor.getFlags : LongWord;
begin
  result := FFlags;
end;

//----------

function ZDescriptor.getLatency : LongWord;
begin
  result := FLatency;
end;

//----------

function ZDescriptor.getName : PChar;
begin
  result := FName;
end;

//----------

function ZDescriptor.getNumInputs : LongWord;
begin
  result := FNumInputs;
end;

//----------

function ZDescriptor.getNumOutputs : LongWord;
begin
  result := FNumOutputs;
end;

//----------

function ZDescriptor.getNumParameters : LongWord;
begin
  result := FParameters.size;
end;

//----------

function ZDescriptor.getNumPrograms : LongWord;
begin
  result := FPrograms.size;
end;

//----------

function ZDescriptor.getParameter(AIndex:LongWord) : ZParameter;
begin
  result := FParameters[AIndex];
end;

//----------

function ZDescriptor.getParameters : ZParameterArray;
begin
  result := FParameters;
end;

//----------

function ZDescriptor.getPrograms : ZProgramArray;
begin
  result := FPrograms;
end;

//----------

function ZDescriptor.getProduct : PChar;
begin
  result := FProduct;
end;

//----------

function ZDescriptor.getUniqueId : LongWord;
begin
  result := FUniqueId;
end;

//----------

function ZDescriptor.getVersion : LongWord;
begin
  result := FVersion;
end;

//----------------------------------------------------------------------
end.

