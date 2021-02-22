unit z_modular;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_types,
  z_array;

const

  // pin type
  zpt_none   = 0;
  zpt_event  = 1;
  zpt_signal = 2;
  zpt_audio  = 3;
  zpt_video  = 4;

  // pin direction
  zpd_input   = 0;
  zpd_output  = 1;

type

  ZPin = class;
  ZWire = class;
  ZModule = class;
  ZGraph = class;

  ZProcessProc = procedure;

  ZPinArray = specialize ZArray<ZPin>;
  ZModuleArray = specialize ZArray<ZModule>;

  //----------

  ZPin = class
  private
    FName : PChar;
    FOwner : ZModule;
    FIndex : LongInt;
    FType : LongWord;
    FDir : LongWord;
  end;

  ZWire = class
  private
    FSrcPin : ZPin;
    FDstPin : ZPin;
    FSrcModule : ZModule;
    FDstModule : ZModule;
  end;

  ZModule = class
  private
    FName       : PChar;
    FOwner      : ZGraph;
    FParent     : ZModule;
    FPins       : ZPinArray;
  public
    constructor create;
    destructor  destroy; override;
    procedure   handleEvent(AIndex:LongInt; AValue:Single);
    procedure   processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
    procedure   processSample(AInputs,AOutputs:PPZSample);
    function    compile(AMode:LongInt) : ZProcessProc;
  end;

  ZGraph = class(ZModule)
  private
    FModules : ZModuleArray;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

// pin

//----------------------------------------------------------------------

// wire

//----------------------------------------------------------------------

// module

constructor ZModule.create;
begin
end;

//----------

destructor ZModule.destroy;
begin
end;

//----------

procedure ZModule.handleEvent(AIndex:LongInt; AValue:Single);
begin
end;

//----------

procedure ZModule.processBlock(AInputs,AOutputs:PPZSample; ALength:LongWord);
begin
end;

//----------

procedure ZModule.processSample(AInputs,AOutputs:PPZSample);
begin
end;

//----------

function  ZModule.compile(AMode:LongInt) : ZProcessProc;
begin
  result := nil;
end;

//----------------------------------------------------------------------

// graph

//----------------------------------------------------------------------
end.

