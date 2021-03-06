#ifndef s3_paint_source_included
#define s3_paint_source_included
//----------------------------------------------------------------------

// not used

/*
  off-screen surface
  something you can paint onto, and use as s source when blitting
  to another surface (target)
  aka blitSource ??
*/

//----------------------------------------------------------------------

class S3_PaintSource {
  public:

    virtual bool isWindow(void)   { return false; }
    virtual bool isSurface(void)  { return false; }
    virtual bool isImage(void)    { return false; }

    virtual int32 width(void)     { return 0; }
    virtual int32 height(void)    { return 0; }
    virtual int32 depth(void)     { return 0; }

    //virtual void* buffer(void)    { return S3_NULL; }

    #ifdef S3_WIN32
      virtual HBITMAP hbitmap(void) { return 0; }
    #endif

    #ifdef S3_LINUX
      virtual Drawable  drawable(void)  { return 0; }
      virtual Display*  display(void)   { return S3_NULL; }
      virtual XImage*   image()         { return S3_NULL; }
      #ifdef S3_XRENDER
      virtual Picture   picture(void)   { return 0; }
      #endif
      //virtual Pixmap    pixmap(void)    { return 0; }
      //virtual Visual    visual(void)    { return 0; }
      //virtual Colormap  colormap(void)  { return 0; }
      //virtual Screen    screen(void)    { return 0; }
    #endif

};

//----------------------------------------------------------------------
#endif
