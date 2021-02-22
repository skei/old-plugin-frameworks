#ifndef s3_surface_xlib_included
#define s3_surface_xlib_included
//----------------------------------------------------------------------

/*
  pixmap
  In contrast to windows, where drawing has no effect if the window is not
  visible, a pixmap can be drawn to at any time because it resides in memory.
*/

//----------------------------------------------------------------------

#include "gui/base/s3_surface_base.h"
#include "gui/s3_drawable.h"

//----------------------------------------------------------------------

/*
#ifdef S3_XRENDER

static XRenderPictFormat argb_templ = {
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

static uint32 argb_templ_mask =
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

#endif
*/

//----------------------------------------------------------------------

class S3_Surface_Xlib
: public S3_Surface_Base
, public S3_Drawable {

  private:
    Display*  MDisplay;
    Pixmap    MPixmap;
    #ifdef S3_XRENDER
    Picture   MPicture;
    #endif

  private:
    int32   MWidth;
    int32   MHeight;
    int32   MDepth;

  //------------------------------
  //
  //------------------------------

  public:

    /*
      we can create the pixmap from the rootwindow drawable..
      DefaultRootWindow(FDisplay);
      DefaultDepthOfScreen
      (but we need the display pointer later)
    */

    S3_Surface_Xlib(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight)
    : S3_Surface_Base(ADrawable,AWidth,AHeight) {
      MDisplay  = ADrawable->display();
      MWidth    = AWidth;
      MHeight   = AHeight;
      MDepth    = ADrawable->depth(); //ADepth;
      //STrace("surface depth %i\n",MDepth); // prints 24
      MPixmap = s3_xlib_create_pixmap(MDisplay,ADrawable->drawable(),MWidth,MHeight,MDepth);
      #ifdef S3_XRENDER
      MPicture = s3_xrender_create_picture(MDisplay,MPixmap,MDepth);
      #endif
    }

    //----------

    /*
      ADepth
        1   monochrome
        8   greyscale
        16
        24
        32  rgba
    */

    S3_Surface_Xlib(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
    : S3_Surface_Base(ADrawable,AWidth,AHeight,ADepth) {
      MDisplay  = ADrawable->display();
      MWidth    = AWidth;
      MHeight   = AHeight;
      MDepth    = ADepth;
      MPixmap   = s3_xlib_create_pixmap(MDisplay,ADrawable->drawable(),MWidth,MHeight,MDepth);
      #ifdef S3_XRENDER
      MPicture = s3_xrender_create_picture(MDisplay,MPixmap,MDepth);
      #endif
    }

    //----------

    virtual ~S3_Surface_Xlib() {
      s3_xlib_delete_pixmap(MDisplay,MPixmap);
      #ifdef S3_XRENDER
      s3_xrender_delete_picture(MDisplay,MPicture);
      #endif
    }

  //------------------------------
  // S3_Drawable
  //------------------------------

  //public:
  //
  //  virtual int32   width(void)   { return MWidth; }
  //  virtual int32   height(void)  { return MHeight; }
  //  virtual int32   depth(void)   { return MDepth; }
  //  //virtual void*   buffer(void)    { return S3_NULL; }

  public:

    bool      isSurface(void) { return true; }
    int32     width(void)     { return MWidth; }
    int32     height(void)    { return MHeight; }
    int32     depth(void)     { return MDepth; }
    Display*  display(void)   { return MDisplay; }
    Pixmap    pixmap(void)    { return MPixmap; }
    Drawable  drawable(void)  { return MPixmap; }
    #ifdef S3_XRENDER
    Picture   picture(void)   { return MPicture; }
    #endif


  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
#endif
