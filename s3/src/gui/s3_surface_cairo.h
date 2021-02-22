#ifndef s3_surface_cairo_included
#define s3_surface_cairo_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include "gui/s3_surface_xlib.h"

//----------

class S3_Surface_Cairo
: public S3_Surface_Xlib {
  private:
    cairo_surface_t* MCairoSurface;
  public:
    S3_Surface_Cairo(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=0);
    virtual ~S3_Surface_Cairo();
  public:
    void setSize(uint32 AX, uint32 AY) override;
    void flush(void) override;
  public: // drawable
    bool              isCairo(void)       { return true; }
    cairo_surface_t*  cairoSurface(void)  { return MCairoSurface; }
};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

/*
  Creates an Xlib surface that draws to the given drawable. The way that colors
  are represented in the drawable is specified by the provided visual.
  Note: If drawable is a Window, then the function
  cairo_xlib_surface_set_size() must be called whenever the size of the window
  changes.
*/


S3_Surface_Cairo::S3_Surface_Cairo(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
: S3_Surface_Xlib(ADrawable,AWidth,AHeight,ADepth) {

  // ignoring the pixmap created by the xlib constructor?
  MCairoSurface = cairo_xlib_surface_create( MDisplay, ADrawable->drawable(), ADrawable->visual(), AWidth, AHeight );
  //MCairoSurface = cairo_xlib_surface_create( MDisplay, MPixmap, ADrawable->visual(), AWidth, AHeight );

}

//----------

S3_Surface_Cairo::~S3_Surface_Cairo() {
  cairo_surface_destroy(MCairoSurface);
}

//----------

/*
  Informs cairo of the new size of the X Drawable underlying the surface. For
  a surface created for a Window (rather than a Pixmap), this function must be
  called each time the size of the window changes. (For a subwindow, you are
  normally resizing the window yourself, but for a toplevel window, it is
  necessary to listen for ConfigureNotify events.)
  A Pixmap can never change size, so it is never necessary to call this
  function on a surface created for a Pixmap.
*/

void S3_Surface_Cairo::setSize(uint32 AX, uint32 AY) {
  cairo_xlib_surface_set_size(MCairoSurface,AX,AY);
}

//----------

/*
  Most surface types allow accessing the surface without using Cairo functions.
  If you do this, keep in mind that it is mandatory that you call
  cairo_surface_flush() before reading from or writing to the surface and that
  you must use cairo_surface_mark_dirty() after modifying it.

  void cairo_surface_mark_dirty (cairo_surface_t *surface);
  Tells cairo that drawing has been done to surface using means other than
  cairo, and that cairo should reread any cached areas. Note that you must call
  cairo_surface_flush() before doing such drawing.

  Do any pending drawing for the surface and also restore any temporary
  modifications cairo has made to the surface's state. This function must be
  called before switching from drawing on the surface with cairo to drawing on
  it directly with native APIs, or accessing its memory outside of Cairo. If
  the surface doesn't support direct access, then this function does nothing.
*/

void S3_Surface_Cairo::flush(void) {
  cairo_surface_flush(MCairoSurface);
}

//----------------------------------------------------------------------
#endif
