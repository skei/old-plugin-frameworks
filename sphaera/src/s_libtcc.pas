{$include sphaera.inc}
unit s_libtcc;

{#ifdef LIBTCC_AS_DLL
  #define LIBTCCAPI __declspec(dllexport)
#else
  #define LIBTCCAPI
#endif}

// {$define LIBTCCAPI:=cdecl; external name 'libtcc';}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  dynlibs,
  s_library;

const
  TCC_OUTPUT_MEMORY         = 0; // output will be ran in memory (no output file) (default)
  TCC_OUTPUT_EXE            = 1; // executable file
  TCC_OUTPUT_DLL            = 2; // dynamic library
  TCC_OUTPUT_OBJ            = 3; // object file
  TCC_OUTPUT_PREPROCESS     = 4; // preprocessed file (used internally)

  TCC_OUTPUT_FORMAT_ELF     = 0; // default output format: ELF
  TCC_OUTPUT_FORMAT_BINARY  = 1; // binary image output
  TCC_OUTPUT_FORMAT_COFF    = 2; // COFF

type

  TCCState  = Pointer;
  PTCCState = ^TCCState;
  tcc_error_func_t = procedure(opaque:pointer; const msg:PChar);

  // state

  {LIBTCCAPI TCCState *tcc_new(       void );}
  {LIBTCCAPI void tcc_delete(         TCCState *s );}
  {LIBTCCAPI void tcc_enable_debug(   TCCState *s );}
  {LIBTCCAPI void tcc_set_error_func( TCCState *s, void *error_opaque, void (*error_func)(void *opaque, const char *msg) );}
  {LIBTCCAPI int tcc_set_warning(     TCCState *s, const char *warning_name, int value );}

  tcc_new_t             = function : PTCCState; cdecl;                                                      // create a new TCC compilation context
  tcc_delete_t          = procedure(s:PTCCState); cdecl;                                                    // free a TCC compilation context
  tcc_enable_debug_t    = procedure(s:PTCCState); cdecl;                                                    // add debug information in the generated code
  tcc_set_error_func_t  = procedure(s:PTCCState; error_opaque:pointer; error_func:tcc_error_func_t); cdecl; // set error/warning display callback
  tcc_set_warning_t     = function(s:PTCCState; const warning_name:PChar; value:LongInt) : LongInt; cdecl;  // set/reset a warning

  // preprocessor

  {LIBTCCAPI int tcc_add_include_path(    TCCState *s, const char *pathname );}
  {LIBTCCAPI int tcc_add_sysinclude_path( TCCState *s, const char *pathname );}
  {LIBTCCAPI void tcc_define_symbol(      TCCState *s, const char *sym, const char *value );}
  {LIBTCCAPI void tcc_undefine_symbol(    TCCState *s, const char *sym );}

  tcc_add_include_path_t    = function(s:PTCCState; const pathname:PChar) : LongInt; cdecl;       // add include path
  tcc_add_sysinclude_path_t = function(s:PTCCState; const pathname:PChar) : LongInt; cdecl;       // add in system include path
  tcc_define_symbol_t       = procedure(s:PTCCState; const sym:PChar; const value:PChar); cdecl;  // define preprocessor symbol 'sym'. Can put optional value
  tcc_undefine_symbol_t     = procedure(s:PTCCState); cdecl;                                      // undefine preprocess symbol 'sym'

  // compiling

  {LIBTCCAPI int tcc_add_file(       TCCState *s, const char *filename );}
  {LIBTCCAPI int tcc_compile_string( TCCState *s, const char *buf );}

  tcc_add_file_t        = function(s:PTCCState; const filename:PChar) : LongInt ; cdecl;  // add a file (either a C file, dll, an object, a library or an ld script). Return -1 if error.
  tcc_compile_string_t  = function(s:PTCCState; const buf:PChar) : LongInt; cdecl;        // compile a string containing a C source. Return non zero if error.

  // linking

  {LIBTCCAPI int tcc_set_output_type(  TCCState *s, int output_type );}
  {LIBTCCAPI int tcc_add_library_path( TCCState *s, const char *pathname );}
  {LIBTCCAPI int tcc_add_library(      TCCState *s, const char *libraryname );}
  {LIBTCCAPI int tcc_add_symbol(       TCCState *s, const char *name, void *val );}
  {LIBTCCAPI int tcc_output_file(      TCCState *s, const char *filename );}
  {LIBTCCAPI int tcc_run(              TCCState *s, int argc, char **argv );}
  {LIBTCCAPI int tcc_relocate(         TCCState *s1, void *ptr );}
  {LIBTCCAPI void *tcc_get_symbol(     TCCState *s, const char *name );}
  {LIBTCCAPI void tcc_set_lib_path(    TCCState *s, const char *path );}

  tcc_set_output_type_t   = function(s:PTCCState; output_type:LongInt) : LongInt; cdecl;            // set output type. MUST BE CALLED before any compilation
  tcc_add_library_path_t  = function(s:PTCCState; const pathname:PChar) : LongInt; cdecl;           // equivalent to -Lpath option
  tcc_add_library_t       = function(s:PTCCState; const libraryname:PChar) : LongInt; cdecl;        // the library name is the same as the argument of the '-l' option
  tcc_add_symbol_t        = function(s:PTCCState; const name:PChar; val:Pointer) : LongInt; cdecl;  // add a symbol to the compiled program
  tcc_output_file_t       = function(s:PTCCState; const filename:PChar) : LongInt; cdecl;           // output an executable, library or object file. DO NOT call tcc_relocate() before.
  tcc_run_t               = function(s:PTCCState; argc:LongInt; argv:PPChar) : LongInt; cdecl;      // link and run main() function and return its value. DO NOT call tcc_relocate() before.
  tcc_relocate_t          = function(s1:PTCCState; ptr:Pointer) : LongInt; cdecl;                   // copy code into memory passed in by the caller and do all relocations (needed before using tcc_get_symbol()). returns -1 on error and required size if ptr is NULL
  tcc_get_symbol_t        = function(s:PTCCState; const name:PChar) : Pointer; cdecl;               // return symbol value or NULL if not found
  tcc_set_lib_path_t      = procedure(s:PTCCState; const path:PChar); cdecl;                        // set CONFIG_TCCDIR at runtime

  //----------

  SLibtcc = class(SLibrary)
  public
    tcc_new                 : tcc_new_t;
    tcc_delete              : tcc_delete_t;
    tcc_enable_debug        : tcc_enable_debug_t;
    tcc_set_error_func      : tcc_set_error_func_t;
    tcc_set_warning         : tcc_set_warning_t;
    tcc_add_include_path    : tcc_add_include_path_t;
    tcc_add_sysinclude_path : tcc_add_sysinclude_path_t;
    tcc_define_symbol       : tcc_define_symbol_t;
    tcc_undefine_symbol     : tcc_undefine_symbol_t;
    tcc_add_file            : tcc_add_file_t;
    tcc_compile_string      : tcc_compile_string_t;
    tcc_set_output_type     : tcc_set_output_type_t;
    tcc_add_library_path    : tcc_add_library_path_t;
    tcc_add_library         : tcc_add_library_t;
    tcc_add_symbol          : tcc_add_symbol_t;
    tcc_output_file         : tcc_output_file_t;
    tcc_run                 : tcc_run_t;
    tcc_relocate            : tcc_relocate_t;
    tcc_get_symbol          : tcc_get_symbol_t;
    tcc_set_lib_path        : tcc_set_lib_path_t;
  public
    constructor Create;
    destructor  Destroy; override;
    function    LoadLib(aFilename:PChar) : LongInt; override;
    procedure   CloseLib; override;
  private
    procedure   Initialize;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//const

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

constructor SLibtcc.Create;
begin
  inherited;
  Initialize;
end;

//----------

destructor SLibtcc.Destroy;
begin
  inherited;
end;

//----------

procedure SLibtcc.Initialize;
begin
  tcc_new := nil;
  tcc_delete := nil;
  tcc_enable_debug:= nil;
  tcc_set_error_func:= nil;
  tcc_set_warning:= nil;
  tcc_add_include_path:= nil;
  tcc_add_sysinclude_path:= nil;
  tcc_define_symbol:= nil;
  tcc_undefine_symbol:= nil;
  tcc_add_file:= nil;
  tcc_compile_string:= nil;
  tcc_set_output_type:= nil;
  tcc_add_library_path:= nil;
  tcc_add_library:= nil;
  tcc_add_symbol:= nil;
  tcc_output_file:= nil;
  tcc_run:= nil;
  tcc_relocate:= nil;
  tcc_get_symbol:= nil;
  tcc_set_lib_path:= nil;
end;

function SLibtcc.LoadLib(aFilename:pchar) : LongInt;
begin
  inherited;
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_new');                  if not Assigned(tcc_new) then exit(-3);
  pointer(tcc_delete) := GetProcedureAddress(mHandle,'tcc_delete');               if not Assigned(tcc_delete) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_enable_debug');         if not Assigned(tcc_enable_debug) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_set_error_func');       if not Assigned(tcc_set_error_func) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_set_warning');          if not Assigned(tcc_set_warning) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_add_include_path');     if not Assigned(tcc_add_include_path) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_add_sysinclude_path');  if not Assigned(tcc_add_sysinclude_path) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_define_symbol');        if not Assigned(tcc_define_symbol) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_undefine_symbol');      if not Assigned(tcc_undefine_symbol) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_add_file');             if not Assigned(tcc_add_file) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_compile_string');       if not Assigned(tcc_compile_string) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_set_output_type');      if not Assigned(tcc_set_output_type) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_add_library_path');     if not Assigned(tcc_add_library_path) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_add_library');          if not Assigned(tcc_add_library) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_add_symbol');           if not Assigned(tcc_add_symbol) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_output_file');          if not Assigned(tcc_output_file) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_run');                  if not Assigned(tcc_run) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_relocate');             if not Assigned(tcc_relocate) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_get_symbol');           if not Assigned(tcc_get_symbol) then exit(-3);
  pointer(tcc_new)    := GetProcedureAddress(mHandle,'tcc_set_lib_path');         if not Assigned(tcc_set_lib_path) then exit(-3);
  result := 0;
end;

//----------

procedure SLibtcc.CloseLib;
begin
  inherited;
  Initialize;
end;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

// ...

//----------------------------------------------------------------------
end.




