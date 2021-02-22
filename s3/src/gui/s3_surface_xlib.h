#ifndef s3_surface_xlib_included
#define s3_surface_xlib_included
//----------------------------------------------------------------------

#include "gui/s3_drawable.h"
#include "gui/s3_surface_base.h"

//----------

//class S3_Surface_Xlib;
//typedef S3_Surface_Xlib S3_Surface_Implementation;

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

class S3_Surface_Xlib
: public S3_Surface_Base
, public S3_Drawable {
  //private:
  protected:
    Display*  MDisplay; // from drawable
    Visual*   MVisual;  // from drawable
    Pixmap    MPixmap;
    int32     MWidth;
    int32     MHeight;
    int32     MDepth;
    #ifdef S3_XRENDER
    Picture   MPicture;
    #endif
  public:
    S3_Surface_Xlib(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth=0);
    virtual ~S3_Surface_Xlib();
  public: // drawable
    bool      isSurface(void) override  { return true; }
    int32     width(void)     override  { return MWidth; }
    int32     height(void)    override  { return MHeight; }
    int32     depth(void)     override  { return MDepth; }
    Display*  display(void)   override  { return MDisplay; }
    Visual*   visual(void)    override  { return MVisual; }
    Drawable  drawable(void)  override  { return MPixmap; }
    #ifdef S3_XRENDER
    Picture   picture(void)   { return MPicture; }
    #endif

  //public:
  //  Pixmap    pixmap(void) { return MPixmap; }

};

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

// clear pixmap:
//int scr = DefaultScreen(MDisplay);
//GC gc = DefaultGC(MDisplay,scr);
//XSetForeground(MDisplay,gc,S3_BGR(0,0,0));
//XFillRectangle(MDisplay,MPixmap,gc,0,0,AWidth,AHeight);

/*
  if ADepth is 0, use depth from drawable

  S3_Window constructor:
    #ifdef S3_CAIRO
      MWindowSurface = new S3_Surface(this,MRect.w,MRect.h);
      ..
    #endif
*/

S3_Surface_Xlib::S3_Surface_Xlib(S3_Drawable* ADrawable, int32 AWidth, int32 AHeight, int32 ADepth)
: S3_Surface_Base(ADrawable,AWidth,AHeight) {

  MDisplay  = ADrawable->display();
  MVisual   = ADrawable->visual();
  MWidth    = AWidth;
  MHeight   = AHeight;
  if (ADepth > 0) MDepth = ADepth;
  else MDepth = ADrawable->depth();
  MPixmap = XCreatePixmap(MDisplay,ADrawable->drawable(),MWidth,MHeight,MDepth);
  #ifdef S3_XRENDER
    //MPicture = s3_xrender_create_picture(MDisplay,ADrawable->drawable(),MDepth);
    MPicture = s3_xrender_create_picture(MDisplay,MPixmap,MDepth);
  #endif

}

//----------

S3_Surface_Xlib::~S3_Surface_Xlib(void) {
  if (MPixmap) XFreePixmap(MDisplay,MPixmap);
  #ifdef S3_XRENDER
    s3_xrender_delete_picture(MDisplay,MPicture);
  #endif

}

//----------------------------------------------------------------------
#endif
