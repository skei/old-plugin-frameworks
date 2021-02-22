
//#define AX_NO_MALLOC
#define AX_DEBUG_AUTO_STD
#define AX_DEBUG_MEM
#define AX_DEBUG_PNG
//#define AX_DEBUG_LOG  "test_gain_gui_skin.log"

#include "axFormat.h"
#include "axEditor.h"

#include "wdg/wdgImage.h"

#include "../extern/picopng.h"
//#include "../extern/tga_loader.h"

//#include "testpng.h"
#include "testpng2.h"
#include "testpng3.h"
//#include "testtga.h"


//----------------------------------------------------------------------
//----------------------------------------------------------------------

#include "gui/axWidget.h"

class myImage : public wdgImage
{
  public:
    myImage(axWidgetListener* aListener, axRect aRect, int aAlignment=wa_None, axImage* aImage=NULL)
    : wdgImage(aListener,aRect,aAlignment,aImage)
      {


      }

    virtual void doPaint(axCanvas* aCanvas, axRect aRect)
      {
        int dstx = mRect.x;
        int dsty = mRect.y;
        int srcx = 0;
        int srcy = 0;
        int srcw = mRect.w;
        int srch = mRect.h;


        aCanvas->setBrushColor( aCanvas->getColor(0,0,0));
        aCanvas->fillRect(mRect.x,mRect.y,mRect.x2(),mRect.y2());

        aCanvas->renderImage(mImage,dstx+30,dsty+30,srcx,srcy,srcw,srch);

        aCanvas->setBrushColor( aCanvas->getColor(255,255,255) );
        aCanvas->fillCircle(10,10,200,200);
        aCanvas->setTextColor( aCanvas->getColor(255,0,255) );
        aCanvas->drawText(50,150,"dette er en test");

        aCanvas->renderImage(mImage,dstx,dsty,srcx,srcy,srcw,srch);

      }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

class myPlugin : public axFormat,
                 public axWidgetListener
{
  private:
    PNG_info_t* pnginfo;
    axEditor*   m_Editor;
    myImage*    w_Image;
    axSurface*  surface;
    axBitmap*   bitmap;

    int   _w;
    int   _h;
    char* _buf;


  public:

    myPlugin(axContext* aContext)
    : axFormat(aContext, /*pf_None*/AX_PLUG_DEFAULT)
      {
        describe("test_winsize","ccernn","axonlibe example",0,AX_MAGIC+0x0000);
        setupAudio(2,2,false);
        setupEditor(256,256);

        //pnginfo = PNG_decode(testpng,testpng_size);
        //pnginfo = PNG_decode(testpng2,testpng2_size);
        pnginfo = decodePng(testpng3,testpng3_size);
        trace("pnginfo = " << pnginfo);
        trace("png error: " << PNG_error);
        _w   = pnginfo->width;
        _h   = pnginfo->height;
        _buf = (char*)pnginfo->image->data;

        //TGAImg loader;
        //loader.decode((unsigned char*)testtga,testtga_size);

        //trace( loader.GetWidth() << "," << loader.GetHeight() << " * " << loader.GetBPP() );

        //_w   = loader.GetWidth();
        //_h   = loader.GetHeight();
        //_buf = (char*)loader.GetImg();

      }

    virtual ~myPlugin()
      {
        //png_alloc_free_all();
      }

    //--------------------------------------------------
    // do..
    //--------------------------------------------------

    virtual axWindow* doOpenEditor(axContext* aContext)
      {
        m_Editor  = new axEditor(this,aContext,mEditorRect,AX_WIN_DEFAULT);

//        _w   = pnginfo->width;
//        _h   = pnginfo->height;
//        _buf = (char*)pnginfo->image->data;

        bitmap = m_Editor->createBitmap(_w,_h,32);      // 32
        bitmap->createBuffer(_buf);
        bitmap->convertRgbaBgra();
        bitmap->swizzle(
          1,  0,  0,  0,    // b
          0,  1,  0,  0,    // g
          0,  0,  1,  0,    // r
          0,  0,  0,  0.5   // a
        );
        //bitmap->randomAlpha();

          int width = bitmap->getWidth();
          int height = bitmap->getHeight();
          char* buffer = bitmap->getBuffer();

//          for(int y=0; y<height; y++)
//          {
//            for(int x=0; x<width; x++)
//            {
//              int pos = (y*width + x) * 4;
//              unsigned char a = x;//axRandInt(255);
//              buffer[pos+3] = a;
//            } //for x
//          } //for y

        bitmap->premultAlpha();
        bitmap->prepare();

        surface = m_Editor->createSurface(_w,_h,32);    // 32
        axCanvas* canvas = surface->getCanvas();
        canvas->drawBitmap(bitmap,0,0,0,0,_w,_h);
        delete bitmap;

        m_Editor->appendWidget( w_Image = new myImage(m_Editor,axRect(0,0,256,256),wa_None,surface) );
        m_Editor->doRealign();
        m_Editor->show();
        return m_Editor;
      }

    //----------

    virtual void doCloseEditor(void)
      {
        m_Editor->hide();
        delete m_Editor;
        m_Editor = NULL;
        delete surface;
        //delete bitmap;
      }

};

//----------------------------------------------------------------------
AX_ENTRYPOINT(myPlugin)

