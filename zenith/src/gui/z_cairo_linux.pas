unit z_cairo_linux;
{$include zenith.inc}

//----------------------------------------------------------------------
interface
//----------------------------------------------------------------------

uses
  X,
  Xlib,
  Cairo,
  CairoXlib;

type

  ZCairoSurfaceLinux = class
  protected
    FSurface    : Pcairo_surface_t;
  public
    //constructor create;
    constructor create(ADisplay:PDisplay; ADrawable:TDrawable; AVisual:PVisual; AWidth,AHeight:longint);
    //constructor create(ADisplay:PDisplay; APixmap:TPixmap; AScreen:PScreen; AWidth,AHeight:longint);
    destructor  destroy; override;
    procedure   setSize(AWidth,AHeight:longint);
  public
    property    surface : Pcairo_surface_t read FSurface;
  end;

  ZCairoSurfaceImpl = ZCairoSurfaceLinux;

//----------------------------------------------------------------------
implementation
//----------------------------------------------------------------------

{constructor ZCairoSurfaceLinux.create;
begin
  inherited;
end;}

//----------

{
  Note: If drawable is a Window, then the function
  cairo_xlib_surface_set_size() must be called whenever the size of the
  window changes.
  }

constructor ZCairoSurfaceLinux.create(ADisplay:PDisplay; ADrawable:TDrawable; AVisual:PVisual; AWidth,AHeight:longint);
begin
  //inherited create;
  FSurface := cairo_xlib_surface_create(ADisplay,ADrawable,AVisual,AWidth,AHeight);
end;

//----------

//constructor ZCairoSurface.create(ADisplay:PDisplay; APixmap:TPixmap; AScreen:PScreen; AWidth,AHeight:longint);
//begin
//  inherited create;
//  cairo_xlib_surface_create_for_bitmap(ADisplay,APixmap,AScreen,AWidth,AHeight);
//end;

//----------

destructor ZCairoSurfaceLinux.destroy;
begin
  cairo_surface_destroy(FSurface);
  inherited;
end;


  {
  Informs cairo of the new size of the X Drawable underlying the surface. For
  a surface created for a Window (rather than a Pixmap), this function must be
  called each time the size of the window changes. (For a subwindow, you are
  normally resizing the window yourself, but for a toplevel window, it is
  necessary to listen for ConfigureNotify events.)

  A Pixmap can never change size, so it is never necessary to call this
  function on a surface created for a Pixmap.
  }

procedure ZCairoSurfaceLinux.setSize(AWidth,AHeight:longint);
begin
  cairo_xlib_surface_set_size(FSurface,AWidth,AHeight);
end;

//----------

  {
  Informs cairo of a new X Drawable underlying the surface. The drawable must
  match the display, screen and format of the existing drawable or the
  application will get X protocol errors and will probably terminate. No
  checks are done by this function to ensure this compatibility.
  }

// void cairo_xlib_surface_set_drawable( cairo_surface_t *surface, Drawable drawable, int width, int height);


//----------------------------------------------------------------------
end.

