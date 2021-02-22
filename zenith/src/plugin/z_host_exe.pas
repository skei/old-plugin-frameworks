unit z_host_exe;

{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  z_plugin_base;
//z_instance;

type

  ZHostExe = class(ZHostBase)
  public
    constructor Create({%H-}AMaster: Pointer);
    destructor  Destroy; override;
  public
    function    getEffect: Pointer; override;
    function    getMaster: Pointer; override;
    function    getTime(var ASampleRate,ASamplePos,ATempo,ABeatPos:single) : longint; override;
    procedure   midiOut({%H-}AOffset:integer; {%H-}AMsg1,{%H-}AMsg2,{%H-}AMsg3:byte); override;
    procedure   midiFlush; override;
    procedure   setEffect({%H-}ptr:Pointer); override;
    procedure   setMaster({%H-}ptr:Pointer); override;
    procedure   updateParameter({%H-}param:longint; {%H-}val:single); override;
  end;

  ZHostImpl = ZHostExe;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

constructor ZHostExe.Create(AMaster: Pointer);
begin
  inherited create;
end;

//----------

destructor ZHostExe.Destroy;
begin
  inherited;
end;

//----------

function ZHostExe.getEffect: Pointer;
begin
  Result := nil;
end;

//----------

function ZHostExe.getMaster: Pointer;
begin
  Result := nil;
end;

//----------

function ZHostExe.getTime(var ASampleRate, ASamplePos, ATempo, ABeatPos: single): longint;
begin
  ASampleRate := 0;
  ASamplePos := 0;
  ATempo := 0;
  ABeatPos := 0;
  Result := 0;
end;

//----------

procedure ZHostExe.midiOut(AOffset: integer; AMsg1, AMsg2, AMsg3: byte);
begin
end;

//----------

procedure ZHostExe.midiFlush;
begin
end;

//----------

procedure ZHostExe.setEffect(ptr:Pointer);
begin
  //FEffect := ptr;
end;

//----------

procedure ZHostExe.setMaster(ptr:Pointer);
begin
  //FMaster := TAudioMasterCallbackFunc(ptr);
end;

//----------

procedure ZHostExe.updateParameter(param:longint; val:single);
begin
end;

//----------------------------------------------------------------------
end.

