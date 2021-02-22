{$include sphaera.inc}
unit s_module;

{$define S_MAXPINS := 256}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

const
  { pin flags }
  spf_none    = 0;
  spf_input   = 1; // ptr to data in another module
  spf_signal  = 2; // call
  spf_output  = 0; // ptr to internal data
  spf_data    = 0; // audio

  { module flags }
  smf_none = 0;

type

  SPModuleInstance  = ^SModuleInstance;
  SPModuleInfo      = ^SModuleInfo;
  SPModulePin       = ^SModulePin;

  //----------

  SModuleInfo = record
    name      : PChar;
    flags     : LongWord;
    numPins   : LongInt;
    pins      : SPModulePin;
    create    : function(AInfo:SPModuleInfo; AUser:Pointer) : SPModuleInstance;
    destroy   : procedure (AInstance:SPModuleInstance);
  end;

  //----------

  SModuleInstance = record
    info      : SPModuleInfo;
    user      : Pointer;
    pinPtr    : PPointer;
    init      : function( AModule:SPModuleInstance) : Boolean; // aka 'constructor'
    exit      : procedure(AModule:SPModuleInstance); // aka 'destructor'
    start     : procedure(AModule:SPModuleInstance); // aka 'compile'
    stop      : procedure(AModule:SPModuleInstance);
    process   : procedure(AModule:SPModuleInstance);
    signal    : procedure(AModule:SPModuleInstance; AIndex:LongInt; AValue:Single); // aka 'dispatcher', aka 'bang'
  end;

  //----------

  SModulePin = record
    name  : PChar;
    flags : LongWord;
  end;

//----------

function  SModuleCreate(AInfo:SPModuleInfo; AUser:Pointer) : SPModuleInstance;
procedure SModuleDestroy(AInstance:SPModuleInstance);
//function  SModuleConnect(AOutMod,AInMod:SPModuleInstance; AOutPin,AInPin:LongInt) : LongInt;

//----------

//exports
function sphaera_get_module_info(AIndex:LongInt) : SPModuleInfo;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_memory;

//----------------------------------------------------------------------
// default callbacks
//----------------------------------------------------------------------

function module_init_default(AModule:SPModuleInstance) : Boolean;
begin
  //writeln('module_init');
  result := true;
end;

//----------

procedure module_exit_default(AModule:SPModuleInstance);
begin
  //writeln('module_exit');
end;

//----------

procedure module_start_default(AModule:SPModuleInstance);
begin
  //writeln('module_start');
end;

//----------

procedure module_stop_default(AModule:SPModuleInstance);
begin
  //writeln('module_stop');
end;

//----------

procedure module_process_default(AModule:SPModuleInstance);
begin
  //writeln('module_process');
end;

//----------

procedure module_signal_default(AModule:SPModuleInstance; AIndex:LongInt; AValue:Single);
begin
  //writeln('module_signal');
end;

//----------------------------------------------------------------------
// default
//----------------------------------------------------------------------

function SModuleCreate(AInfo:SPModuleInfo; AUser:Pointer) : SPModuleInstance;
var
  module : SPModuleInstance;
begin
  module := GetMem( SizeOf(SModuleInstance) );
  SMemset(module,0,sizeof(SModuleInstance));
  module^.pinPtr := Getmem(AInfo^.numPins*sizeof(Pointer));
  SMemset(module^.pinPtr,0,AInfo^.numPins*sizeof(Pointer));
  module^.info := AInfo;
  module^.user    := AUser;
  module^.init    := @module_init_default;
  module^.exit    := @module_exit_default;
  module^.start   := @module_start_default;
  module^.stop    := @module_stop_default;
  module^.process := @module_process_default;
  module^.signal  := @module_signal_default;
  result := module;
end;

//----------

procedure SModuleDestroy(AInstance:SPModuleInstance);
begin
  FreeMem(AInstance^.pinPtr);
  Freemem(AInstance);
end;

//----------------------------------------------------------------------
// example module
//----------------------------------------------------------------------

function mi_create(AInfo:SPModuleInfo; AUser:Pointer) : SPModuleInstance;
begin
  result := SModuleCreate(AInfo,AUser);
end;

//----------

procedure mi_destroy(AInstance:SPModuleInstance);
begin
  SModuleDestroy(AInstance);
end;

//----------------------------------------------------------------------

var

  module_pins : array[0..1] of SModulePin =
  ((
    name  : 'pin1';
    flags : spf_input + spf_signal;
  ),(
    name  : 'pin2';
    flags : spf_output + spf_data;
  ));

  //----------

  module_info : SModuleInfo =
  (
    name      : 'module';
    flags     : smf_none;
    numPins   : 2;
    pins      : @module_pins;
    create    : @mi_create;
    destroy   : @mi_destroy;
  );

//----------------------------------------------------------------------

function sphaera_get_module_info(AIndex:LongInt) : SPModuleInfo;
begin
  result := nil;
  if aIndex = 0 then result := @module_info;
end;

//exports sphaera_get_module_info;

//----------------------------------------------------------------------

{
initialization
  SRegisterModule(0,@module_info);
finalization
  SUnregisterModule(0,@module_info);
}

//----------------------------------------------------------------------
end.

