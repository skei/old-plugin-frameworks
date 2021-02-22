{$include sphaera.inc}
unit s_surface;

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

{$ifdef SPH_CAIRO}
uses
  {$ifdef SPH_WIN32} s_surface_cairo_win32; {$endif}
{$endif}

type

  SSurface = class(SSurfaceImpl)
  end;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

//----------------------------------------------------------------------
end.

(*

{$include sphaera.inc}
unit s_surface;

{
  TODO: split into base/impl/surface, ala canvas
}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Cairo,
  CairoWin32,
  Windows;

type

  SCairoSurface = class
  private
    FIsImage  : Boolean;
    FData     : PByte;//Pointer;
    FDataSize : LongWord;
  protected
    FSurface    : Pcairo_surface_t;
  public
    constructor create(hdc:HDC);
    constructor create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
    //constructor create(AWidth,AHeight,ADepth:LongInt); // from image
    constructor create(AWidth,AHeight,ADepth:LongInt);
    constructor create(ASurface:SCairoSurface; AWidth,AHeight:LongInt);
    constructor create(AFilename:PChar); // from file
    //function    readfunc(closure:Pointer; data:PByte; length:longword) : cairo_status_t;
    constructor create(AData:Pointer; ASize:LongInt); // from memory
    destructor  destroy; override;
    function    getDC : HDC;
    function    getWidth  : LongInt; // png
    function    getHeight : LongInt; // png
  public
    property    _surface : Pcairo_surface_t read FSurface;
    property    _data : PByte read FData write FData;
    property    _datasize : LongWord read FDataSize write FDataSize;
  end;

  SSurface = SCairoSurface;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

uses
  s_memory;

//----------

function s_readfunc(closure:Pointer; data:PByte; length:longword) : cairo_status_t; cdecl;
var
  s : SCairoSurface;
begin
  s := SCairoSurface(closure);
  SMemcpy(data,s._data,length);
  s._data := s._data + length;
  result := CAIRO_STATUS_SUCCESS; // CAIRO_STATUS_READ_ERROR;
end;

//----------

{
  Creates a cairo surface that targets the given DC. The DC will be queried for
  its initial clip extents, and this will be used as the size of the cairo
  surface. The resulting surface will always be of format CAIRO_FORMAT_RGB24
}

constructor SCairoSurface.create(hdc:HDC);
begin
  FIsImage  := False;
  FSurface  := cairo_win32_surface_create(hdc);
end;

//----------

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

constructor SCairoSurface.create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
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

{constructor SCairoSurface.create(AWidth,AHeight,ADepth:LongInt);
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

constructor SCairoSurface.create(AWidth,AHeight,ADepth:LongInt);
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

constructor SCairoSurface.create(AFilename:PChar);
begin
  FIsImage := True;
  //inherited create;
  FSurface := cairo_image_surface_create_from_png(AFilename);
end;

//----------

constructor SCairoSurface.create(AData:Pointer; ASize:LongInt); // from memory
begin
  FIsImage  := True;
  FData     := AData;
  FDataSize := ASize;
  FSurface  := cairo_image_surface_create_from_png_stream(@s_readfunc,self);
end;

//----------

constructor SCairoSurface.create(ASurface:SCairoSurface; AWidth,AHeight:LongInt);
begin
  // FIsImage := False or True?
  //inherited create;
  FSurface := cairo_surface_create_similar(ASurface._surface,CAIRO_CONTENT_COLOR_ALPHA,AWidth,AHeight);
end;

//----------

{
  cairo_surface_create_for_rectangle
  cairo_surface_create_similar
}

//----------

destructor SCairoSurface.destroy;
begin
  cairo_surface_destroy(FSurface);
  inherited;
end;

//----------

function  SCairoSurface.getDC : HDC;
begin
  result := cairo_win32_surface_get_dc(FSurface);
end;

//----------

function  SCairoSurface.getWidth : LongInt;
begin
  result := 0;
  if FIsImage then result := cairo_image_surface_get_width(FSurface);
end;

//----------

function  SCairoSurface.getHeight : LongInt;
begin
  result := 0;
  if FIsImage then result := cairo_image_surface_get_height(FSurface);
end;

//----------------------------------------------------------------------
end.

*)
