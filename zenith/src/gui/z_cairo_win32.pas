unit z_cairo_win32;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  Windows,
  Cairo,
  CairoWin32;

type

  ZCairoSurfaceWin32 = class
  protected
    FSurface    : Pcairo_surface_t;
  public
    constructor create(hdc:HDC);
    constructor create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
    constructor create(AWidth,AHeight,ADepth:LongInt);
    destructor  destroy; override;
    //procedure   setSize(AWidth,AHeight:longint);
  public
    property    surface : Pcairo_surface_t read FSurface;
  end;

  ZCairoSurfaceImpl = ZCairoSurfaceWin32;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{
  Creates a cairo surface that targets the given DC. The DC will be queried for
  its initial clip extents, and this will be used as the size of the cairo
  surface. The resulting surface will always be of format CAIRO_FORMAT_RGB24
}

constructor ZCairoSurfaceWin32.create(hdc:HDC);
begin
  FSurface := cairo_win32_surface_create(hdc);
end;

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

  {
  CAIRO_FORMAT_INVALID   = -1,
  CAIRO_FORMAT_ARGB32    = 0,
  CAIRO_FORMAT_RGB24     = 1,
  CAIRO_FORMAT_A8        = 2,
  CAIRO_FORMAT_A1        = 3,
  CAIRO_FORMAT_RGB16_565 = 4
  }

constructor ZCairoSurfaceWin32.create(hdc:HDC; AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  case ADepth of
     8: fmt := CAIRO_FORMAT_A8;
    24: fmt := CAIRO_FORMAT_RGB24;
    32: fmt := CAIRO_FORMAT_ARGB32;
  end;
  FSurface := cairo_win32_surface_create_with_ddb(hdc,fmt,AWidth,AHeight);
end;

{
  Creates a device-independent-bitmap surface not associated with any particular
  existing surface or device context. The created bitmap will be uninitialized.
}

constructor ZCairoSurfaceWin32.create(AWidth,AHeight,ADepth:LongInt);
var
  fmt : cairo_format_t;
begin
  case ADepth of
     8: fmt := CAIRO_FORMAT_A8;
    24: fmt := CAIRO_FORMAT_RGB24;
    32: fmt := CAIRO_FORMAT_ARGB32;
  end;
  FSurface := cairo_win32_surface_create_with_dib(fmt,AWidth,AHeight);
end;

destructor ZCairoSurfaceWin32.destroy;
begin
  cairo_surface_destroy(FSurface);
  inherited;
end;

//----------------------------------------------------------------------
end.

