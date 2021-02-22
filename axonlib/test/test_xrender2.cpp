#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/Xrender.h>
#include <stdlib.h>

//----------------------------------------------------------------------

Display*  display;
int       screen;
Window    root;
Window    window;
XEvent    event;

//----------------------------------------------------------------------

// create a 1x1 argb picture, repeated
// and use that as a filling pen

Picture create_pen(int red, int green, int blue, int alpha)
{
  // create color
	XRenderColor color = {red,green,blue,alpha};
	// find 32-bit format
	XRenderPictFormat *fmt = XRenderFindStandardFormat(display,PictStandardARGB32);
	// create 1x1 Pixmap
	Pixmap pm = XCreatePixmap(display,window,1,1,32);
	// create Picture
	XRenderPictureAttributes pict_attr;
	pict_attr.repeat = 1;
	Picture picture = XRenderCreatePicture(display,pm,fmt,CPRepeat,&pict_attr);
	// fill Picture w/color
	XRenderFillRectangle(display,PictOpOver,picture,&color,0,0,1,1);
	return picture;
}

//----------------------------------------------------------------------

Picture create_pen2(int red, int green, int blue, int alpha)
{
  // create color
	XRenderColor color = {red,green,blue,alpha};
	// find 32-bit format
	XRenderPictFormat *fmt = XRenderFindStandardFormat(display,PictStandardARGB32);
	// create 1x1 Pixmap
	Pixmap pm = XCreatePixmap(display,window,16,16,32);
	// create Picture
	XRenderPictureAttributes pict_attr;
	pict_attr.repeat = 1;
	Picture picture = XRenderCreatePicture(display,pm,fmt,CPRepeat,&pict_attr);
	// fill Picture w/color
	XRenderFillRectangle(display,PictOpOver,picture,&color,0,0,16,16);
	return picture;
}

//----------------------------------------------------------------------

int main(int argc, char *argv[])
{
	display = XOpenDisplay(NULL);
	screen  = DefaultScreen(display);
	root    = DefaultRootWindow(display);
	window  = XCreateWindow(display,root,0,0,640,480,0,DefaultDepth(display,screen),InputOutput,DefaultVisual(display,screen),0,NULL);
	XSelectInput(display, window, KeyPressMask|KeyReleaseMask|ExposureMask|ButtonPressMask|StructureNotifyMask);
	//XMapWindow(display, window);

  //----------
  // xrender

  // check if extension available
  int render_event_base;
  int render_error_base;
  int render_present = XRenderQueryExtension(display,&render_event_base,&render_error_base);
  if (!render_present) { fprintf(stderr,"RENDER extension missing!\n"); abort(); }
  // find format for screen buffer
  XRenderPictFormat* fmt = XRenderFindStandardFormat(display, PictStandardRGB24);
  // create Picture (screen buffer)
  XRenderPictureAttributes pict_attr;
  pict_attr.poly_edge = PolyEdgeSmooth;
  pict_attr.poly_mode = PolyModeImprecise;
  Picture picture = XRenderCreatePicture(display,window,fmt,CPPolyEdge|CPPolyMode,&pict_attr);
  // set up some defaults
  Picture red           = create_pen2( 0x7fff,0,     0,     0x3fff );
  Picture green         = create_pen2( 0,     0x7fff,0,     0x3fff );
  Picture blue          = create_pen2( 0x7fff,0,     0x7fff,0x3fff );
  XRenderColor yellow   =            { 0x7fff,0x7fff,0,     0x3fff }; // yellow trans
  XRenderColor cyan     =            { 0,     0x7fff,0x7fff,0x3fff }; // cyam
  XRenderColor magenta  =            { 0x7fff,0,     0x7fff,0x3fff }; // cyam
  XRenderColor black    =            { 0,     0,     0,     0x7fff }; // cyam
  XRenderColor back     =            { 0x7fff,0x7fff,0x7fff,0xffff }; // background

  //----------

	// now make the window visible
	XMapWindow(display, window);

  XTriangle tr =
  {
    { 320<<16,  10<<16},
    {  10<<16, 240<<16},
    { 630<<16, 300<<16}
  };

	while(1)
	{
		XNextEvent(display,&event);
		switch(event.type)
		{
			case Expose:
				XRenderFillRectangle(display,PictOpOver,picture,&back,0,0,640,480);
				XRenderFillRectangle(display,PictOpOver,picture,&yellow,0,0,200,200);
        XRenderCompositeTriangles(display,PictOpOver,red,picture,0,0,0,&tr,1);

				break;
			case DestroyNotify:
				return 0;
		}
	}
	return 0;
}
