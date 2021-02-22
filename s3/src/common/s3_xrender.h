#ifndef s3_xrender_included
#define s3_xrender_included
//----------------------------------------------------------------------

#ifdef S3_XRENDER

#include "common/s3_color.h"
#include <X11/extensions/Xrender.h>

//----------------------------------------------------------------------

XTransform s3_xrender_identity_matrix = {{
  { XDoubleToFixed(1), XDoubleToFixed(0), XDoubleToFixed(0) },
  { XDoubleToFixed(0), XDoubleToFixed(1), XDoubleToFixed(0) },
  { XDoubleToFixed(0), XDoubleToFixed(0), XDoubleToFixed(1) }
}};

//----------

XRenderPictFormat s3_xrender_argb_templ = {
  0,              // id
  PictTypeDirect, // type
  32,             // depth
  {               // direct
    16,           // direct.red
    0xff,         // direct.redMask
    8,            // direct.green
    0xff,         // direct.greenMask
    0,            // direct.blue
    0xff,         // direct.blueMask
    24,           // direct.alpha
    0xff,         // direct.alphaMask
  },
  0,              // colormap
};

//----------

uint32 s3_xrender_argb_templ_mask =
  PictFormatType      |
  PictFormatDepth     |
  PictFormatRed       |
  PictFormatRedMask   |
  PictFormatGreen     |
  PictFormatGreenMask |
  PictFormatBlue      |
  PictFormatBlueMask  |
  PictFormatAlpha     |
  PictFormatAlphaMask;

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

/*
XRenderPictFormat* s3_xrender_find_format(Display* ADisplay, int32 ADepth) {
  //case ADepth of
  //  24: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB24);
  //  32: result := XRenderFindStandardFormat(FDisplay,PictStandardRGB32);
  //end;
  switch(ADepth) {
    case 24: s3_xrender_argb_templ.depth = 24; break;
    case 32: s3_xrender_argb_templ.depth = 32; break;
  }
  return XRenderFindFormat(ADisplay, s3_xrender_argb_templ_mask, &s3_xrender_argb_templ, 0);
}
*/

XRenderPictFormat* s3_xrender_find_format(Display* ADisplay, int32 ADepth) {
  if (ADepth == 24) return XRenderFindStandardFormat(ADisplay,PictStandardRGB24);
  else return XRenderFindStandardFormat(ADisplay,PictStandardARGB32);
}

//----------

Picture s3_xrender_create_picture(Display* ADisplay, Drawable ADrawable, int32 ADepth) {
  XRenderPictFormat*        fmt;
  XRenderPictureAttributes  pict_attr;
  int32                     pict_bits;

  /*
  if FDepth = 24 then  fmt := find_format(FDisplay,24{PictStandardRGB24})
  else fmt := find_format(FDisplay,32{PictStandardARGB32});
  */

  if (ADepth == 24) fmt = XRenderFindStandardFormat(ADisplay,PictStandardRGB24);
  else fmt = XRenderFindStandardFormat(ADisplay,PictStandardARGB32);

  //fmt = s3_xrender_find_format(ADisplay,ADepth);

  pict_attr.poly_edge = PolyEdgeSmooth;
  pict_attr.poly_mode = PolyModeImprecise;
  /*
  pict_attr.component_alpha = true;
  */
  pict_bits = /*CPComponentAlpha |*/ CPPolyEdge | CPPolyMode;
  return  XRenderCreatePicture(
    ADisplay,
    ADrawable,//MPixmap,
    fmt,
    pict_bits,
    &pict_attr
  );
}

//----------

void s3_xrender_delete_picture(Display* ADisplay, Picture APicture) {
  XRenderFreePicture(ADisplay,APicture);
}

//----------

Picture s3_xrender_create_brush(Display* ADisplay, Drawable ADrawable, S3_Color AColor, int32 ADepth) {

  XRenderColor color;
  color.red   = (int)((AColor.r*AColor.a) * 255) << 8;
  color.green = (int)((AColor.g*AColor.a) * 255) << 8;
  color.blue  = (int)((AColor.b*AColor.a) * 255) << 8;
  color.alpha = (int)((AColor.a) * 255) << 8;
  //color.flags = (DoRed | DoGreen | DoBlue);

	Pixmap pm = XCreatePixmap(ADisplay,ADrawable,1,1,ADepth);
	XRenderPictureAttributes pict_attr;
	pict_attr.repeat = 1;

	XRenderPictFormat* fmt;// = s3_xrender_find_format(ADisplay,ADepth);
  if (ADepth == 24) fmt = XRenderFindStandardFormat(ADisplay,PictStandardRGB24);
  else fmt = XRenderFindStandardFormat(ADisplay,PictStandardARGB32);

	Picture picture = XRenderCreatePicture(ADisplay, pm, fmt, CPRepeat, &pict_attr);
	XRenderFillRectangle(ADisplay,PictOpOver,picture,&color,0,0,1,1);
	XFreePixmap(ADisplay,pm);

	return picture;
}

#endif

//----------------------------------------------------------------------
#endif
