#ifndef s3_image_xlib_included
#define s3_image_xlib_included
//----------------------------------------------------------------------

#include "gui/s3_image_base.h"
//#include "common/s3_memory_stdlib.h"
#include "gfx/s3_bitmap.h"
#include "gui/s3_drawable.h"

//----------

class S3_Image_Xlib
: public S3_Image_Base
, public S3_Drawable {

  private:
    Display*    MDisplay; // from drawable
    XImage*     MImage;

  private:
    int32       MWidth;
    int32       MHeight;
    int32       MDepth;
    //int32     MStride;
    uint32*     MBuffer;


  //------------------------------
  //
  //------------------------------

  public:

    /*
      //http://tronche.com/gui/x/xlib/utilities/XCreateImage.html
      does not allocate space for the image itself
      initializes the structure byte-order, bit-order, and bitmap-unit values from the display

      //http://www.sbin.org/doc/Xlib/chapt_06.html
      XCreateImage()
      Allocates memory for an XImage structure and sets various members.
      Note that it uses the server's data format, which is often not appropriate.
      The byte- and bit-order fields should usually be changed directly to the client-native format.
      However, then the call _XInitImageFuncPtrs(image) should be issued to reset the mapping
      to the appropriate versions of the functions for manipulating the image.
      This call is supposed to be private to Xlib and, therefore, should be watched for changes
      in later releases, but this is currently the accepted method.
    */

    /*
    SImage_Xlib(SDrawable* ADrawable, SBitmap* ABitmap, int32 ADepth)
    : SImage_Base(ADrawable,ABitmap) {
    }
    */

    S3_Image_Xlib(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=32)
    : S3_Image_Base(ADrawable,AWidth,AHeight,ADepth) {
      MWidth  = AWidth;
      MHeight = AHeight;
      MDepth  = ADepth;
      MBuffer = (uint32*)S3_Malloc(MWidth*MHeight*sizeof(uint32));
      //switch (MDepth) {
      //  case 1:   MStride = MWidth >> 3; break;
      //  case 8:   MStride = MWidth;      break;
      //  case 16:  MStride = MWidth * 2;  break;
      //  case 24:  MStride = MWidth * 3;  break;
      //  case 32:  MStride = MWidth * 4;  break;
      //}
      MDisplay = ADrawable->display();
      //MBitmap = ABitmap;
      //MDepth  = ADrawable->depth(); // 32; // DefaultDepth(MDisplay,DefaultScreen(MDisplay));
      //MDepth  = ADepth;
      MImage = XCreateImage(
        MDisplay,
        DefaultVisual(MDisplay,DefaultScreen(MDisplay)),  //CopyFromParent // visual
        MDepth,                   // depth
        ZPixmap,                  // format
        0,                        // offset
        (char*)MBuffer, // data
        MWidth,
        MHeight,        // size
        32,                       // pad
        0                         // bytes per line
      );
    }

    //----------

    /*
      //http://www.sbin.org/doc/Xlib/chapt_06.html
      XDestroyImage()
      Frees the data field in an image structure if the image structure was allocated in the application.
      If the image was created using XCreateImage(), XGetImage(), or XGetSubImage(),
      XDestroyImage() frees both the data and the image structure. Note that if the image data is stored
      in static memory in the application, it cannot be freed
      --to free an image created with XCreateImage() that has statically allocated data, you must set NULL
      nto the data field before calling XDestroyImage().
    */

    virtual ~S3_Image_Xlib() {
      MImage->data = S3_NULL;   // we want to delete it ourselves...
      XDestroyImage(MImage);      // frees data too
      delete MBuffer;
    }

  //------------------------------
  // S3_Drawable
  //------------------------------

  public:

    bool    isImage(void) override { return true; }
    XImage* image(void)   override { return MImage; }
    int32   width(void)   override { return MWidth; }
    int32   height(void)  override { return MHeight; }
    int32   depth(void)   override { return MDepth; }
    void*   buffer(void)  override { return MBuffer; }

};

//----------------------------------------------------------------------
#endif
