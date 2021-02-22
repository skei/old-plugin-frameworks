#ifndef s3_font_xft_included
#define s3_font_xft_included
//----------------------------------------------------------------------

/*
  http://stackoverflow.com/questions/17078247/linux-c-xft-how-to-use-it
*/

//----------------------------------------------------------------------

#include "common/s3_convert.h"
#include "gui/base/s3_font_base.h"

//----------

class S3_Font_Xft
: public S3_Font_Base {

  private:
    //S3_Color  MColor;

    #ifdef S3_XFT
    XftFont*      MXftFont;
    XftDraw*      MXftDraw;
    XftColor      MXftColor;
    #endif
    //Drawable      MDrawable;
    Display*      MDisplay;
    char          name_buffer[128];
    char          size_buffer[16];

  //------------------------------
  //
  //------------------------------

  public:

    S3_Font_Xft(S3_Drawable* ADrawable)
    : S3_Font_Base(ADrawable) {
      MDisplay = ADrawable->display();
      MXftDraw = XftDrawCreate(MDisplay,ADrawable->drawable(),DefaultVisual(MDisplay,0),DefaultColormap(MDisplay,0));
      MXftFont = S3_NULL;

    }

    //----------

    S3_Font_Xft(S3_Drawable* ADrawable, const char* AName, int32 ASize, S3_Color AColor)
    : S3_Font_Base(ADrawable,AName,ASize,AColor) {
      MDisplay = ADrawable->display();
      MXftDraw = XftDrawCreate(MDisplay,ADrawable->drawable(),DefaultVisual(MDisplay,0),DefaultColormap(MDisplay,0));
      color(AColor);
      size(ASize);
      load(AName);
      //draw
      //XftDrawString8(MXftDraw, &MXftColor, MXftFont, 20,70 , (XftChar8 *)"string", 10);
    }

    //----------

    virtual ~S3_Font_Xft() {
      XftDrawDestroy(MXftDraw);
      XftColorFree(MDisplay,DefaultVisual(MDisplay,0),DefaultColormap(MDisplay,0),&MXftColor);
      //XftFontClose(MDisplay,MXftFont); // crashes..
    }

  //------------------------------
  //
  //------------------------------

  public:

    void color(S3_Color AColor) {
      //MColor = AColor;
      XRenderColor xrcolor;
      xrcolor.red   = (AColor.r * (float)65535);
      xrcolor.green = (AColor.g * (float)65535);
      xrcolor.blue  = (AColor.b * (float)65535);
      xrcolor.alpha = (AColor.a * (float)65535);
      XftColorAllocValue(MDisplay,DefaultVisual(MDisplay,0),DefaultColormap(MDisplay,0),&xrcolor,&MXftColor);
    }

    //----------

    void size(uint32 ASize) {
      S3_IntToString(size_buffer,ASize);
    }

    //----------

    void load(const char* ANAme) {
      char buffer[256];
      S3_Strcpy(buffer,name_buffer);
      S3_Strcat(buffer,"-");
      S3_Strcat(buffer,size_buffer);
      MXftFont = XftFontOpenName(MDisplay,0 /*screen*/,buffer);
    }

    //----------

    //S3_Color  color(void)             { return MColor; }

    XftFont*  font(void)  { return MXftFont; }
    XftDraw*  draw(void)  { return MXftDraw; }
    XftColor* color(void) { return &MXftColor; }


  //------------------------------
  //
  //------------------------------

  public:


};

//----------------------------------------------------------------------
#endif
