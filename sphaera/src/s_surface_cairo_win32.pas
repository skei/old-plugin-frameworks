{$include sphaera.inc}
unit s_surface_cairo_win32;

{
  TODO: split into base/impl/surface, ala canvas
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Cairo,
  CairoWin32,
  Windows,
  s_surface_base;

type

  SSurfaceCairoWin32 = class(SSurfaceBase)
  private
    FIsImage  : Boolean;
    FData     : PByte;//Pointer;
    FDataSize : LongWord;
    FSurface  : Pcairo_surface_t;
  public
    constructor create(hdc:HDC);
    constructor create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
    //
    //constructor create(AWidth,AHeight,ADepth:LongInt); //override;
    constructor create(AWidth,AHeight,ADepth:LongInt); //override;
    constructor create(ASurface:SSurfaceBase; AWidth,AHeight:LongInt); //override;
    constructor create(AFilename:PChar); //override;
    constructor create(AData:Pointer; ASize:LongInt); //override;
    //
    destructor  destroy; override;
    //
    function    getHandle : Pointer; override;
    function    getInternal : LongWord; override;
    function    getWidth  : LongInt; override;
    function    getHeight : LongInt; override;
    function    getData : PByte; override;
    function    getDataSize : LongInt; override;
    procedure   setData(AData:PByte); override;
    procedure   setDataSize(ASize:LongInt); override;
  end;

  SSurfaceImpl = SSurfaceCairoWin32;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_memory;

//----------

function s_readfunc(closure:Pointer; data:PByte; length:longword) : cairo_status_t; cdecl;
var
  s : SSurfaceCairoWin32;
begin
  s := SSurfaceCairoWin32(closure);
  SMemcpy(data,s.getData,length);
  s.setData( s.getData + length );
  result := CAIRO_STATUS_SUCCESS; // CAIRO_STATUS_READ_ERROR;
end;

//----------

{
  Creates a cairo surface that targets the given DC. The DC will be queried for
  its initial clip extents, and this will be used as the size of the cairo
  surface. The resulting surface will always be of format CAIRO_FORMAT_RGB24
}

constructor SSurfaceCairoWin32.create(hdc:HDC);
begin
  FIsImage  := False;
  FSurface  := cairo_win32_surface_create(hdc);
end;

//----------

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

constructor SSurfaceCairoWin32.create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  FIsImage := False;
  case ADepth of
     1: fmt := CAIRO_FORMAT_A1;
     8: fmt := CAIRO_FORMAT_A8;
    //16: fmt := 4; //CAIRO_FORMAT_RGB16_565;
    24: fmt := CAIRO_FORMAT_RGB24;
    32: fmt := CAIRO_FORMAT_ARGB32;
  end;
  FSurface := cairo_win32_surface_create_with_ddb(hdc,fmt,AWidth,AHeight);
end;


//----------

{constructor SSurfaceCairoWin32.create(AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  FIsImage := True;
  //inherited create;
  fmt := CAIRO_FORMAT_ARGB32;
  case ADepth of
    8  : fmt := CAIRO_FORMAT_A8;
    24 : fmt := CAIRO_FORMAT_RGB24;
    32 : fmt := CAIRO_FORMAT_ARGB32;
  end;
  cairo_image_surface_create(fmt,AWidth,AHeight);
end;}

//----------

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

constructor SSurfaceCairoWin32.create(AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  FIsImage := False;
  case ADepth of
     8: fmt := CAIRO_FORMAT_A8;
    24: fmt := CAIRO_FORMAT_RGB24;
    32: fmt := CAIRO_FORMAT_ARGB32;
  end;
  FSurface := cairo_win32_surface_create_with_dib(fmt,AWidth,AHeight);
end;

//----------

constructor SSurfaceCairoWin32.create(AFilename:PChar);
begin
  FIsImage := True;
  //inherited create;
  FSurface := cairo_image_surface_create_from_png(AFilename);
end;

//----------

constructor SSurfaceCairoWin32.create(AData:Pointer; ASize:LongInt); // from memory
begin
  FIsImage  := True;
  FData     := AData;
  FDataSize := ASize;
  FSurface  := cairo_image_surface_create_from_png_stream(@s_readfunc,self);
end;

//----------

constructor SSurfaceCairoWin32.create(ASurface:SSurfaceBase; AWidth,AHeight:LongInt);
var
  srf : Pcairo_surface_t;
begin
  // FIsImage := False or True?
  //inherited create;
  srf := SSurfaceCairoWin32(ASurface).getHandle;
  FSurface := cairo_surface_create_similar(srf,CAIRO_CONTENT_COLOR_ALPHA,AWidth,AHeight);
end;

//----------

{
  cairo_surface_create_for_rectangle
  cairo_surface_create_similar
}

//----------

destructor SSurfaceCairoWin32.destroy;
begin
  cairo_surface_destroy(FSurface);
  inherited;
end;

//----------------------------------------------------------------------

function SSurfaceCairoWin32.getHandle : Pointer;
begin
  result := FSurface;
end;

//----------

function SSurfaceCairoWin32.getInternal : LongWord;
begin
  result := cairo_win32_surface_get_dc(FSurface);
end;

//----------

function  SSurfaceCairoWin32.getWidth : LongInt;
begin
  result := 0;
  if FIsImage then result := cairo_image_surface_get_width(FSurface);
end;

//----------

function SSurfaceCairoWin32.getHeight : LongInt;
begin
  result := 0;
  if FIsImage then result := cairo_image_surface_get_height(FSurface);
end;

//----------

function SSurfaceCairoWin32.getData : PByte;
begin
  //result := nil;
  {if FIsImage then} result := FData;
end;

//----------

function SSurfaceCairoWin32.getDataSize : LongInt;
begin
  //result := 0;
  {if FIsImage then} result := FDataSize;
end;

//----------

procedure SSurfaceCairoWin32.setData(AData:PByte);
begin
  FData := AData;
end;

//----------

procedure SSurfaceCairoWin32.setDataSize(ASize:LongInt);
begin
  FDataSize := ASize;
end;

//----------------------------------------------------------------------
end.
