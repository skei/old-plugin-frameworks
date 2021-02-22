#ifndef s3_xrender_included
#define s3_xrender_included
//----------------------------------------------------------------------

#ifdef S3_XRENDER

#include <X11/extensions/Xrender.h>

//static
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

static
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

//----------

/*
  http://www.winehq.org/pipermail/wine-patches/2005-August/020119.html
  Avoid using XRenderFindStandardFormat as older libraries don't have it
*/

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

//----------

//void create_picture(Drawable ADrawable) {
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



#endif

//----------------------------------------------------------------------
#endif
