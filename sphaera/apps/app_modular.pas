{$include sphaera.inc}
unit app_modular;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  s_application,
  s_module;

type

  myApp = class(SApplication)
  private
    FInfo     : SPModuleInfo;
    FModule   : SPModuleInstance;
    data      : pointer;
  public
    procedure on_create; override;
    procedure on_destroy; override;
  end;

  SPHAERA_APPLICATION = myApp;

  module_data = record
    tekst : PChar;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------


procedure myApp.on_create;
begin
  FInfo :=  sphaera_get_module_info(0);
  data := Getmem(sizeof(module_data));
  FModule := FInfo^.create(FInfo,data);
  FModule^.init(FModule);
  FModule^.start(FModule);
  FModule^.process(FModule);
end;

//----------

procedure myApp.on_destroy;
begin
  FModule^.stop(FModule);
  FreeMem(data);
  FModule^.exit(FModule);
  FInfo^.destroy(FModule);
end;

//----------------------------------------------------------------------
end.

