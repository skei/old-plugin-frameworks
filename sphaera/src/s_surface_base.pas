{$include sphaera.inc}
unit s_surface_base;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

type

  SSurfaceBase = class
  public
    //constructor create(AWidth,AHeight,ADepth:LongInt); // from image
    constructor create(AWidth,AHeight,ADepth:LongInt);
    constructor create(ASurface:SSurfaceBase; AWidth,AHeight:LongInt);
    constructor create(AFilename:PChar); // from file
    constructor create(AData:Pointer; ASize:LongInt); // from memory
  public
    function    getHandle : Pointer; virtual;
    function    getInternal : LongWord; virtual;
    function    getWidth  : LongInt; virtual;
    function    getHeight : LongInt; virtual;
    function    getData : PByte; virtual;
    function    getDataSize : LongInt; virtual;
    procedure   setData(AData:PByte); virtual;
    procedure   setDataSize(ASize:LongInt); virtual;
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{constructor SSurfaceCairoWin32.create(AWidth,AHeight,ADepth:LongInt);
begin
end;}

constructor SSurfaceBase.create(AWidth,AHeight,ADepth:LongInt);
begin
end;

constructor SSurfaceBase.create(AFilename:PChar);
begin
end;

//----------

constructor SSurfaceBase.create(AData:Pointer; ASize:LongInt); // from memory
begin
end;

//----------

constructor SSurfaceBase.create(ASurface:SSurfaceBase; AWidth,AHeight:LongInt);
begin
end;

//----------------------------------------------------------------------

function SSurfaceBase.getHandle : Pointer;
begin
  result := nil;
end;

function SSurfaceBase.getInternal : LongWord;
begin
  result := 0;
end;

function SSurfaceBase.getWidth : LongInt;
begin
  result := 0;
end;

function SSurfaceBase.getHeight : LongInt;
begin
  result := 0;
end;

function SSurfaceBase.getData : PByte;
begin
  result := nil;
end;

function SSurfaceBase.getDataSize : LongInt;
begin
  result := 0;
end;

procedure SSurfaceBase.setData(AData:PByte);
begin
end;

procedure SSurfaceBase.setDataSize(ASize:LongInt);
begin
end;

//----------------------------------------------------------------------
end.
